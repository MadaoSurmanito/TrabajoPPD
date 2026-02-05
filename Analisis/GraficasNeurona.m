clc; clear; close all;

data = load('Datos\DatosNeurona.txt');

V = data(:,1);
U = data(:,2);
I = data(:,3);

window = 200;
total = length(V);
step = 1;
delayGIF = 0.05;   % velocidad del GIF

figure('Color','w');

% Subplots iniciales
subplot(3,1,1);
hV = plot(V(1:window),'b','LineWidth',1.2);
ylabel('V'); grid on;
ylim([min(V) max(V)])

subplot(3,1,2);
hU = plot(U(1:window),'r','LineWidth',1.2);
ylabel('U'); grid on;
ylim([min(U) max(U)])

subplot(3,1,3);
hI = plot(I(1:window),'g','LineWidth',1.2);
ylabel('I'); grid on;
ylim([min(I)-10 max(I)+10])

filename = 'izhikevich.gif';

for k = 1:step:(total-window)

    set(hV,'YData',V(k:k+window-1));
    set(hU,'YData',U(k:k+window-1));
    set(hI,'YData',I(k:k+window-1));

    drawnow;

    frame = getframe(gcf);
    im = frame2im(frame);
    [A,map] = rgb2ind(im,256);

    if k == 1
        imwrite(A,map,filename,'gif','LoopCount',Inf,'DelayTime',delayGIF);
    else
        imwrite(A,map,filename,'gif','WriteMode','append','DelayTime',delayGIF);
    end
end
