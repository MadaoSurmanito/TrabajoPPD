clc; clear; close all;

%% =======================
% Cargar datos
%% =======================
load("Analisis/Datos/DatosCompletos.mat");  % contiene Mutacion, Tiempos, Poblacion

baseGraf = 'Analisis/Graficas/Mutacion';

%% =======================
% Modelos a procesar
%% =======================
modelos = {'SinNeurona', ...
           'Neurona_CH', 'Neurona_FS', 'Neurona_IB', ...
           'Neurona_LTS', 'Neurona_RS', 'Neurona_RZ', ...
           'Neurona_TC1', 'Neurona_TC2'};

ventana = 50;  % ventana media móvil
nGen = 999;    % número total de generaciones
nMuestras = 100; % número de muestras por gen

%% =======================
% Crear carpetas
%% =======================
dirEventos = fullfile(baseGraf,'Eventos');
dirMovil   = fullfile(baseGraf,'MediaMovil');
dirConfMat = fullfile(baseGraf,'Confusion');

if ~isfolder(dirEventos), mkdir(dirEventos); end
if ~isfolder(dirMovil),   mkdir(dirMovil);   end
if ~isfolder(dirConfMat), mkdir(dirConfMat); end

%% =======================
% Procesar cada modelo
%% =======================
for m = 1:length(modelos)
    modelo = modelos{m};
    
    % Extraer datos
    datos = Mutacion.Secuencial.(modelo).Mutacion;
    gen   = datos(:,1);
    muta  = datos(:,2);
    seleccionado = datos(:,3);

    %% --- Conteo por generación ---
    mutas_por_gen  = zeros(1, nGen);
    spikes_por_gen = zeros(1, nGen);

    for g = 1:nGen
        idx = (gen == g);
        mutas_por_gen(g)  = sum(muta(idx));
        spikes_por_gen(g) = sum(seleccionado(idx));
    end

    %% --- Proporción y media móvil ---
    proporcion_muta  = mutas_por_gen / nMuestras;
    proporcion_spike = spikes_por_gen / nMuestras;

    muta_mov  = movmean(proporcion_muta, ventana);
    spike_mov = movmean(proporcion_spike, ventana);

    %% --- Probabilidades condicionadas ---
    n11 = sum(muta==1 & seleccionado==1);
    n10 = sum(muta==1 & seleccionado==0);
    n01 = sum(muta==0 & seleccionado==1);
    n00 = sum(muta==0 & seleccionado==0);

    P_spike_given_mut    = n11 / (n11 + n10);
    P_spike_given_nomut  = n01 / (n01 + n00);

    %% =======================
    % Gráfica de número de eventos
    %% =======================
    figEventos = figure('Visible','off');
    plot(1:nGen, mutas_por_gen, '-o', 'LineWidth', 1.5); hold on;
    plot(1:nGen, spikes_por_gen, '-s', 'LineWidth', 1.5);
    grid on;
    xlabel('Generación');
    ylabel('Número de eventos (de 100 muestras)');
    title(['Número de mutaciones y selecciones - ' modelo]);
    legend('Mutaciones','selecciones');
    saveas(figEventos, fullfile(dirEventos, [modelo '.png']));
    close(figEventos);

    %% =======================
    % Gráfica media móvil
    %% =======================
    figMovil = figure('Visible','off');
    plot(1:nGen, muta_mov,  'LineWidth', 2); hold on;
    plot(1:nGen, spike_mov, 'LineWidth', 2);
    grid on;
    xlabel('Generación');
    ylabel('Proporción de eventos (media móvil)');
    title(['Dinámica de mutación y selección - ' modelo ' (ventana = ' num2str(ventana) ')']);
    legend('Mutación','seleccionado');
    saveas(figMovil, fullfile(dirMovil, [modelo '.png']));
    close(figMovil);

    %% =======================
    % Matriz de confusión
    %% =======================
    confMat = [n11 n10;  % fila muta=1
               n01 n00]; % fila muta=0

    figConf = figure('Visible','off');
    colLabels = {'seleccionado=1','seleccionado=0'};
    rowLabels = {'Muta=1','Muta=0'};
    h = heatmap(colLabels,rowLabels,confMat);
    h.Title = ['Matriz de confusión - ' modelo];
    h.Colormap = hot;
    h.CellLabelFormat = '%d';
    h.ColorbarVisible = 'on';

    % Guardar heatmap correctamente usando getframe
    frame = getframe(figConf);
    imwrite(frame.cdata, fullfile(dirConfMat,[modelo '.png']));
    close(figConf);

    fprintf('Modelo %s procesado. P(seleccionado|muta)=%.3f, P(seleccionado|no muta)=%.3f\n', ...
        modelo, P_spike_given_mut, P_spike_given_nomut);
end

disp('Todas las gráficas de mutación y seleccionado guardadas correctamente.');
