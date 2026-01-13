clc; clear; close all;
archivo = 'Datos\DatosNeurona.txt';
data = load(archivo);

v = data(:,1);
u = data(:,2);
i = data(:,3);

figure;

% Subplot 1: V
subplot(3,1,1);
plot(v, 'b', 'LineWidth', 1.2);
ylabel('V');

% Subplot 2: U
subplot(3,1,2);
plot(u, 'r', 'LineWidth', 1.2);
ylabel('U');
grid on;

% Subplot 3: I
subplot(3,1,3);
plot(i, 'g', 'LineWidth', 1.2);
ylabel('I');
grid on;


V = data(:,1);
U = data(:,2);
I = data(:,3);

window = 200;               % Tamaño de la ventana
total = length(V);          % Número total de datos
step = 1;                   % Incremento de la ventana (puedes poner >1 para acelerar)
pauseTime = 0.0025;           % Pausa entre frames (ajusta velocidad)

figure;

% Inicializar subplots
subplot(3,1,1);
hV = plot(V(1:window), 'b', 'LineWidth', 1.2);
ylabel('V');
grid on;
ylim([min(V), max(V)]);     % Ajusta límites del eje

subplot(3,1,2);
hU = plot(U(1:window), 'r', 'LineWidth', 1.2);
ylabel('U');
grid on;
ylim([min(U), max(U)]);     % Ajusta límites del eje

subplot(3,1,3);
hI = plot(I(1:window), 'g', 'LineWidth', 1.2);
ylabel('I');
grid on;
ylim([min(I)-10, max(I)+10]);     % Ajusta límites del eje

% --- Bucle para animar ---
for i = 1:step:(total - window)
    % Actualizar datos de la ventana
    set(hV, 'YData', V(i:i+window-1));
    set(hU, 'YData', U(i:i+window-1));
    set(hI, 'YData', I(i:i+window-1));
    
    % Actualizar eje X
    subplot(3,1,1);
    xlim([1 window]);
    subplot(3,1,2);
    xlim([1 window]);
    subplot(3,1,3);
    xlim([1 window]);
    
    drawnow;              % Actualiza la figura
    pause(pauseTime);     % Pausa para ver la animación
end
