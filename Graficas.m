%% Script para leer resultados y graficar SpeedUp y Error Promedio por modelo
clear; clc; close all;

%% --- Configuración ---
archivosSpike = { ...
    'resultadosSpike1.txt', ...
    'resultadosSpike2.txt', ...
    'resultadosSpike3.txt', ...
    'resultadosSpike4.txt', ...
    'resultadosSpike5.txt', ...
    'resultadosSpike6.txt', ...
    'resultadosSpike7.txt', ...
    'resultadosSpike8.txt'};

archivoNormal = 'resultados.txt'; % Paralelo normal
numNeuronas = length(archivosSpike);

%% --- Crear carpeta para guardar las gráficas ---
if ~exist('Graficas','dir')
    mkdir('Graficas');
end

%% --- Leer datos del algoritmo normal ---
dataNorm = load(archivoNormal);
hilosNorm   = dataNorm(:,1);
tiemposNorm = dataNorm(:,2);
erroresNorm = dataNorm(:,3);

%% --- Inicialización ---
speedupSpike = zeros(numNeuronas, length(hilosNorm));
errorSpike   = zeros(numNeuronas, length(hilosNorm));

%% --- Leer datos de Spike ---
for n = 1:numNeuronas
    data = load(archivosSpike{n});
    tiempos = data(:,2);
    errores = data(:,3);

    % SpeedUp relativo al T1 normal
    speedupSpike(n,:) = tiemposNorm(1) ./ tiempos;

    % Error (para promedio posterior)
    errorSpike(n,:) = errores;
end

%% ============================================================
%% ===================== SPEEDUP ==============================
%% ============================================================

figure('Name','SpeedUp por modelo','NumberTitle','off');
tiledlayout(3,3,'TileSpacing','Compact','Padding','Compact');

% --- Normal ---
nexttile;
plot(hilosNorm, tiemposNorm(1)./tiemposNorm,'-o','LineWidth',2);
grid on;
xlabel('Número de hilos');
ylabel('SpeedUp');
title('Normal');
ylim([0 max(tiemposNorm(1)./tiemposNorm)*1.1]);

% --- Spike 1-8 ---
for n = 1:numNeuronas
    nexttile;
    plot(hilosNorm, speedupSpike(n,:),'-o','LineWidth',1.5);
    grid on;
    xlabel('Número de hilos');
    ylabel('SpeedUp');
    title(sprintf('Spike %d',n));
    ylim([0 max(speedupSpike(n,:))*1.1]);
end

saveas(gcf, fullfile('Graficas','SpeedUp_Subplots.png'));

%% ============================================================
%% =========== ERROR PROMEDIO (SIN HILOS) =====================
%% ============================================================

% Error promedio por modelo
errorPromedio = zeros(numNeuronas+1,1);
labels = cell(numNeuronas+1,1);

% Normal
errorPromedio(1) = mean(erroresNorm);
labels{1} = 'Normal';

% Spike 1-8
for n = 1:numNeuronas
    errorPromedio(n+1) = mean(errorSpike(n,:));
    labels{n+1} = sprintf('Spike %d',n);
end

%% --- Histograma (barras) ---
figure('Name','Error promedio por modelo','NumberTitle','off');

bar(errorPromedio);
grid on;

set(gca,'XTick',1:length(labels));
set(gca,'XTickLabel',labels);
xtickangle(45);

xlabel('Modelo');
ylabel('Error promedio');
title('Error promedio de los modelos');

ylim([0 max(errorPromedio)*1.1]);

saveas(gcf, fullfile('Graficas','Error_Promedio_por_Modelo.png'));

%% --- Fin ---
disp('SpeedUp y Error promedio por modelo generados y guardados en "Graficas".');
