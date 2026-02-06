clc; clear; close all;

load("Analisis\Datos\DatosCompletos.mat");
baseGraf = 'Analisis/Graficas';

%% =======================
%  CÁLCULO SPEEDUP Y PARALELICIDAD
%% =======================

modelos = {'SinNeurona', ...
           'Neurona_CH', 'Neurona_FS', 'Neurona_IB', ...
           'Neurona_LTS', 'Neurona_RS', 'Neurona_RZ', ...
           'Neurona_TC1', 'Neurona_TC2'};

Speedup = struct();
Paralelicidad = struct();

for m = 1:length(modelos)
    modelo = modelos{m};

    %% --- Tiempo secuencial ---
    camposSec = fieldnames(Tiempos.Secuencial.(modelo));
    TsecVals = [];
    for k = 1:length(camposSec)
        TsecVals = [TsecVals; Tiempos.Secuencial.(modelo).(camposSec{k})(:)];
    end
    Tsec = mean(TsecVals);

    %% --- Tiempos paralelos ---
    if ~isfield(Tiempos.Paralelo.(modelo), 'Tiempos')
        warning('Modelo %s sin campo Tiempos paralelo', modelo);
        continue;
    end

    datosPar = Tiempos.Paralelo.(modelo).Tiempos;

    % columnas: [nHilos , tiempo]
    nH = datosPar(:,1);
    Tp = datosPar(:,2);

    % ordenar por nº de hilos
    [nH, idx] = sort(nH);
    Tp = Tp(idx);

    %% --- Métricas ---
    Speedup.(modelo).nHilos = nH;
    Speedup.(modelo).valor  = Tsec ./ Tp;

    Paralelicidad.(modelo).nHilos = nH;
    Paralelicidad.(modelo).valor = Tp(1) ./ Tp;
end

%% =======================
%  CREACIÓN DE CARPETAS
%% =======================

dirSpeed = fullfile(baseGraf,'SpeedUp');
dirPara  = fullfile(baseGraf,'Paralelicidad');

if ~isfolder(dirSpeed), mkdir(dirSpeed); end
if ~isfolder(dirPara),  mkdir(dirPara);  end

%% =======================
%  GRÁFICAS INDIVIDUALES
%% =======================

for m = 1:length(modelos)
    modelo = modelos{m};
    
    % SpeedUp
    fig = figure('Visible','off');
    plot(Speedup.(modelo).nHilos,Speedup.(modelo).valor,'-o','LineWidth',2);
    grid on;
    xlabel('Número de hilos');
    ylabel('SpeedUp');
    title(['SpeedUp - ' modelo],'Interpreter','none');
    saveas(fig,fullfile(dirSpeed,[modelo '.png']));
    close(fig);
    
    % Paralelicidad
    fig = figure('Visible','off');
    plot(Paralelicidad.(modelo).nHilos,Paralelicidad.(modelo).valor,'-s','LineWidth',2);
    grid on;
    xlabel('Número de hilos');
    ylabel('Paralelicidad (T_1 / T_n)');
    title(['Paralelicidad - ' modelo],'Interpreter','none');
    saveas(fig,fullfile(dirPara,[modelo '.png']));
    close(fig);
end

%% =======================
%  GRÁFICAS GLOBALES
%% =======================

% SpeedUp global
fig = figure;
hold on; grid on;
for m = 1:length(modelos)
    plot(Speedup.(modelos{m}).nHilos,Speedup.(modelos{m}).valor,'-o','LineWidth',1.5);
end
xlabel('Número de hilos');
ylabel('SpeedUp');
title('SpeedUp - Todos los modelos');
legend(modelos,'Location','northwest','Interpreter','none');
saveas(fig,fullfile(baseGraf,'SpeedUp_Todos.png'));
close(fig);

% Paralelicidad global
fig = figure;
hold on; grid on;
for m = 1:length(modelos)
    plot(Paralelicidad.(modelos{m}).nHilos,Paralelicidad.(modelos{m}).valor,'-s','LineWidth',1.5);
end
xlabel('Número de hilos');
ylabel('Paralelicidad (T_1 / T_n)');
title('Paralelicidad - Todos los modelos');
legend(modelos,'Location','northwest','Interpreter','none');
saveas(fig,fullfile(baseGraf,'Paralelicidad_Todos.png'));
close(fig);

disp('Proceso completo finalizado correctamente.');
