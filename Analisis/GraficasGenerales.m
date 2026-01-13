clc; clear; close all

%% Cargar Datos
load("Datos\aumenta_solo_gens\resultados.txt");

% Datos Funcion Normal
for j = 1:5
    ini = 1 + 8*(j-1);
    fin = 8*j;

    gens = 5000*j;
    TPoblacion = 250*j;
    datos{1,j} = [ ...
        gens*ones(8,1), ...
        TPoblacion*ones(8,1), ...
        resultados(ini:fin,:) ...
    ];

end

% Datos Funciones Spike
for k = 1:8
    nombreFichero = sprintf("Datos\\aumenta_solo_gens\\resultadosSpike%d.txt", k);
    resultadosSpike = load(nombreFichero);

    for j = 1:5
        ini = 1 + 8*(j-1);
        fin = 8*j;

        gens = 5000*j;
        TPoblacion = 250*j;

        datos{k+1, j} = [ ...
            gens*ones(8,1), ...
            TPoblacion*ones(8,1), ...
            resultadosSpike(ini:fin,:) ...
        ];
    end
end

clearvars -except datos

% Crear carpeta principal si no existe
if ~exist('Graficas', 'dir')
    mkdir('Graficas');
end

% Crear subcarpetas para SpeedUps y Paralelicidad
if ~exist(fullfile('Graficas','SpeedUps'), 'dir')
    mkdir(fullfile('Graficas','SpeedUps'));
end
if ~exist(fullfile('Graficas','Paralelicidad'), 'dir')
    mkdir(fullfile('Graficas','Paralelicidad'));
end

%% Grafico Bigotes Error Promedio
numModelos = size(datos,1);
j = 5;   % mismo caso que antes

errores = [];
grupos  = [];

for i = 1:numModelos
    D = datos{i,j};
    errorModelo = D(:,5);   % columna de error

    errores = [errores; errorModelo];
    grupos  = [grupos; i*ones(size(errorModelo))];
end

fig = figure('Visible','on');
boxplot(errores, grupos);
grid on;

xlabel('Modelo');
ylabel('Error');
title('Distribución del error por modelo (Boxplot)');

xticks(1:numModelos);

nombreArchivo = fullfile('Graficas','ErroresBoxplot.png');
saveas(fig, nombreArchivo);

%{
%% SpeedUp y Paralelicidad
for i = 1:size(datos,1)      % modelos (1 normal, 2-9 spike)
    j = size(datos,2);  % trozos (gens / poblacion)
    
    D = datos{i,j};

    % Extraer columnas
    hilos  = D(:,3);
    tiempo = D(:,4);

    % Tiempo secuencial (hilos=0)
    Tseq = tiempo(1);

    % Tiempo paralelo
    tiempoPar = tiempo(2:end);
    hilosPar  = hilos(2:end);

    % Añadir el caso 1 hilo al principio
    tiempoPar = [tiempo(2); tiempoPar]; 
    hilosPar  = [1; hilosPar];          

    % SpeedUp
    speedup = Tseq ./ tiempoPar;

    % Paralelicidad (1 hilo = 1)
    eficiencia = tiempoPar(1) ./ tiempoPar;

    %% ---- SpeedUp ----
    fig1 = figure('Visible', 'off');  % figura oculta
    plot(hilosPar, speedup, '-o', 'LineWidth', 2);
    grid on;
    xlabel('Número de hilos');
    ylabel('SpeedUp');
    title(sprintf('SpeedUp - Modelo %d, Caso %d', i, j));
    
    % Guardar la figura en Graficas/SpeedUps
    nombreArchivo = fullfile('Graficas','SpeedUps', sprintf('SpeedUp_Modelo%d_Caso%d.png', i, j));
    saveas(fig1, nombreArchivo);
    close(fig1);

    %% ---- Paralelicidad ----
    fig2 = figure('Visible', 'off');  % figura oculta
    plot(hilosPar, eficiencia, '-o', 'LineWidth', 2);
    grid on;
    xlabel('Número de hilos');
    ylabel('Paralelicidad (Eficiencia)');
    title(sprintf('Paralelicidad - Modelo %d, Caso %d', i, j));

    % Guardar la figura en Graficas/Paralelicidad
    nombreArchivo = fullfile('Graficas','Paralelicidad', sprintf('Paralelicidad_Modelo%d_Caso%d.png', i, j));
    saveas(fig2, nombreArchivo);
    close(fig2);
end
%}
