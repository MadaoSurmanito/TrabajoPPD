clc; clear; close all

%% ==================== CARGAR DATOS ====================
% Datos normales
resultados = load("Analisis\Datos\resultados.txt");
hilos = resultados(:,1);
tiempoNormal = resultados(:,2);
errorNormal  = resultados(:,3);

% Datos spikes
numSpikes = 8;
spikes = struct();
for k = 1:numSpikes
    data = load(sprintf("Analisis\\Datos\\resultadosSpike%d.txt", k));
    spikes(k).tiempo = data(:,2);
    spikes(k).error  = data(:,3);
end

% Nombres de modelos
modelos = ["Normal", "Spike1", "Spike2", "Spike3", "Spike4", "Spike5", "Spike6", "Spike7", "Spike8"];
numModelos = length(modelos);

%% ==================== BOXPLOT DE ERRORES ====================
errores = [errorNormal];
grupos  = ones(size(errorNormal));

for k = 1:numSpikes
    errores = [errores; spikes(k).error];
    grupos  = [grupos; (k+1)*ones(size(spikes(k).error))];
end

figBox = figure('Visible','on');
boxplot(errores, grupos);
grid on;
xlabel('Modelo');
ylabel('Error');
title('Distribución del error por modelo (Boxplot)');
xticks(1:numModelos);
xticklabels(modelos);

% Guardar boxplot
folderGraficas = 'Analisis\Graficas';
if ~exist(folderGraficas, 'dir'); mkdir(folderGraficas); end
saveas(figBox, fullfile(folderGraficas,'ErroresBoxplot.png'));

%% ==================== CALCULO DE SPEEDUP Y PARALELIZACION ====================
% Preparar vectores de tiempo por modelo
tiempos = cell(1, numSpikes + 1);
tiempos{1} = tiempoNormal;
for k = 1:numSpikes
    tiempos{k+1} = spikes(k).tiempo;
end

% Preparar índices y valores de hilos
idxUsados = find(hilos ~= 0);    % índices de hilos > 0
idxParal  = find(hilos > 1);     % índices de hilos > 1
hilosUsados = hilos(idxUsados);  % eje X para speedup
hilosParal  = hilos(idxParal);   % eje X para paralelización

% Inicializar celdas
speedup = cell(1,numModelos);
paralelizacion = cell(1,numModelos);

for m = 1:numModelos
    T = tiempos{m};
    T_seq = T(hilos == 0);        % tiempo secuencial
    T1    = T(hilos == 1);        % tiempo con 1 hilo
    
    speedup{m} = T_seq ./ T(idxUsados);    % speedup vs secuencial
    paralelizacion{m} = T1 ./ T(idxParal); % paralelización vs 1 hilo
end

%% ==================== GRAFICOS ====================
colors = lines(numModelos);

% Crear carpetas
folderSpeedup = fullfile(folderGraficas, 'Speedup');
folderParalel = fullfile(folderGraficas, 'Paralelizacion');
if ~exist(folderSpeedup,'dir'); mkdir(folderSpeedup); end
if ~exist(folderParalel,'dir'); mkdir(folderParalel); end

% --- Speedup individual por modelo ---
for m = 1:numModelos
    fig = figure('Name', sprintf('Speedup %s', modelos(m)), 'NumberTitle','off');
    plot(hilosUsados, speedup{m}, '-o', 'LineWidth', 1.5, 'Color', colors(m,:));
    xlabel('Número de hilos'); ylabel('Speedup');
    title(['Speedup - ' modelos(m)]);
    grid on;
    saveas(fig, fullfile(folderSpeedup, sprintf('Speedup_%s.png', modelos(m))));
end

% --- Paralelización individual por modelo ---
for m = 1:numModelos
    fig = figure('Name', sprintf('Paralelizacion %s', modelos(m)), 'NumberTitle','off');
    plot(hilosParal, paralelizacion{m}, '-o', 'LineWidth', 1.5, 'Color', colors(m,:));
    xlabel('Número de hilos'); ylabel('Paralelización');
    title(['Paralelización - ' modelos(m)]);
    grid on;
    saveas(fig, fullfile(folderParalel, sprintf('Paralelizacion_%s.png', modelos(m))));
end

disp('Todas las figuras individuales y conjuntas se han generado y guardado correctamente.');

%% ==================== SPEEDUP CONJUNTO ====================
figSpeedupAll = figure('Name','Speedup - Todos los modelos','NumberTitle','off');
hold on;

for m = 1:numModelos
    plot(hilosUsados, speedup{m}, '-o', ...
        'LineWidth', 1.5, ...
        'Color', colors(m,:), ...
        'DisplayName', modelos(m));
end

grid on;
xlabel('Número de hilos');
ylabel('Speedup');
title('Speedup - Comparación de todos los modelos');
legend('Location','northwest');
hold off;

% Guardar imagen
saveas(figSpeedupAll, fullfile(folderSpeedup,'Speedup_Todos_Modelos.png'));

%% ==================== PARALELIZACION CONJUNTA ====================
figParalAll = figure('Name','Paralelización - Todos los modelos','NumberTitle','off');
hold on;

for m = 1:numModelos
    plot(hilosParal, paralelizacion{m}, '-o', ...
        'LineWidth', 1.5, ...
        'Color', colors(m,:), ...
        'DisplayName', modelos(m));
end

grid on;
xlabel('Número de hilos');
ylabel('Paralelización');
title('Paralelización - Comparación de todos los modelos');
legend('Location','northeast');
hold off;

% Guardar imagen
saveas(figParalAll, fullfile(folderParalel,'Paralelizacion_Todos_Modelos.png'));

