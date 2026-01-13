clc; clear; close all;
archivo = 'Datos\DatosPoblacion.txt';
data = load(archivo);

v = data(:,1);
u = data(:,2);
i = data(:,3);

figure;

% Subplot 1: V
subplot(3,1,1);
plot(v, 'b', 'LineWidth', 1.2);
ylabel('Mejor');

% Subplot 2: U
subplot(3,1,2);
plot(u, 'r', 'LineWidth', 1.2);
ylabel('Promedio');
grid on;

% Subplot 3: I
subplot(3,1,3);
plot(i, 'g', 'LineWidth', 1.2);
ylabel('Peor');
grid on;

