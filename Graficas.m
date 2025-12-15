%% Script para leer resultados y graficar SpeedUp y Error por neurona
clear; clc; close all;

% --- Configuración ---
archivosSpike = {'resultadosSpike1.txt', ...
                 'resultadosSpike2.txt', ...
                 'resultadosSpike3.txt', ...
                 'resultadosSpike4.txt', ...
                 'resultadosSpike5.txt', ...
                 'resultadosSpike6.txt', ...
                 'resultadosSpike7.txt', ...
                 'resultadosSpike8.txt'};
archivoNormal = 'resultados.txt'; % Paralelo normal sin neurona
numNeuronas = length(archivosSpike);

% --- Crear carpeta para guardar las gráficas ---
if ~exist('Graficas', 'dir')
    mkdir('Graficas');
end

% --- Leer datos del algoritmo normal ---
dataNorm = load(archivoNormal);
hilosNorm = dataNorm(:,1);
tiemposNorm = dataNorm(:,2);
erroresNorm = dataNorm(:,3);

% --- Inicializar matrices para SpeedUp y Error ---
speedupSpike = zeros(numNeuronas, length(hilosNorm));
errorSpike = zeros(numNeuronas, length(hilosNorm));

% --- Leer datos de Spike ---
for n = 1:numNeuronas
    data = load(archivosSpike{n});
    tiempos = data(:,2);
    errores = data(:,3);

    speedupSpike(n,:) = tiemposNorm(1) ./ tiempos;  % SpeedUp relativo al T1 normal
    errorSpike(n,:) = errores;
end

%% --- Crear subplots de SpeedUp ---
figure('Name','SpeedUp por neurona','NumberTitle','off');
tiledlayout(3,3,'TileSpacing','Compact','Padding','Compact'); % 9 subplots

% Normal
nexttile;
plot(hilosNorm, tiemposNorm(1)./tiemposNorm,'-o','LineWidth',2);
xlabel('Número de hilos'); ylabel('SpeedUp');
title('Normal');
grid on;
ylim([0 max(tiemposNorm(1)./tiemposNorm)*1.1]); % Normalizar eje Y
saveas(gcf, fullfile('Graficas','SpeedUp_Normal.png'));

% Spike 1-8
for n = 1:numNeuronas
    nexttile;
    plot(hilosNorm, speedupSpike(n,:), '-o','LineWidth',1.5);
    xlabel('Número de hilos'); ylabel('SpeedUp');
    title(sprintf('Spike %d', n));
    grid on;
    ylim([0 max(speedupSpike(n,:))*1.1]); % Normalizar eje Y
    saveas(gcf, fullfile('Graficas',sprintf('SpeedUp_Spike%d.png',n))); % guardar cada subplot individual
end

% Guardar la figura completa
saveas(gcf, fullfile('Graficas','SpeedUp_Subplots.png'));

%% --- Crear subplots de Error ---
figure('Name','Error por neurona','NumberTitle','off');
tiledlayout(3,3,'TileSpacing','Compact','Padding','Compact');

% Normal
nexttile;
plot(hilosNorm, erroresNorm,'-o','LineWidth',2);
xlabel('Número de hilos'); ylabel('Error');
title('Normal');
grid on;
ylim([0 max(erroresNorm)*1.1]); % Normalizar eje Y
saveas(gcf, fullfile('Graficas','Error_Normal.png'));

% Spike 1-8
for n = 1:numNeuronas
    nexttile;
    plot(hilosNorm, errorSpike(n,:), '-o','LineWidth',1.5);
    xlabel('Número de hilos'); ylabel('Error');
    title(sprintf('Spike %d', n));
    grid on;
    ylim([0 max(errorSpike(n,:))*1.1]); % Normalizar eje Y
    saveas(gcf, fullfile('Graficas',sprintf('Error_Spike%d.png',n))); % guardar cada subplot individual
end

% Guardar la figura completa
saveas(gcf, fullfile('Graficas','Error_Subplots.png'));

disp('Gráficas de SpeedUp y Error normalizadas y guardadas en la carpeta "Graficas".');
