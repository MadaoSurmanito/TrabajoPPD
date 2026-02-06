clc; clear; close all;

%% =======================
% Cargar datos
%% =======================
load("Analisis/Datos/DatosCompletos.mat");  % contiene Mutacion, Tiempos, Poblacion

baseGraf = 'Analisis/Graficas/Poblacion';
modelos = {'SinNeurona', ...
           'Neurona_CH', 'Neurona_FS', 'Neurona_IB', ...
           'Neurona_LTS', 'Neurona_RS', 'Neurona_RZ', ...
           'Neurona_TC1', 'Neurona_TC2'};

ventanaBox = 50;  % ventana para boxplot

%% =======================
% Tipos de ejecución
%% =======================
tiposEjec = {'Secuencial','Paralelo'};

%% =======================
% Crear carpetas
%% =======================
for t = 1:length(tiposEjec)
    ejec = tiposEjec{t};
    dirCurvas  = fullfile(baseGraf,['Curvas_' ejec]);
    dirBoxplots = fullfile(baseGraf,['Boxplots_' ejec]);
    if ~isfolder(dirCurvas),  mkdir(dirCurvas); end
    if ~isfolder(dirBoxplots), mkdir(dirBoxplots); end
end

%% =======================
% Procesar cada modelo y cada tipo de ejecución
%% =======================
for t = 1:length(tiposEjec)
    ejec = tiposEjec{t};
    fprintf('Procesando ejecución: %s\n', ejec);

    for m = 1:length(modelos)
        modelo = modelos{m};
        fprintf('  Modelo: %s\n', modelo);

        %% =======================
        % Determinar número de generaciones e islas
        %% =======================
        nFicheros = length(Poblacion.(ejec).(modelo));
        maxGen = 0;
        nIslas = 1; % secuencial = 1, paralelo detecta automáticamente

        for k = 1:nFicheros
            nombre = Poblacion.(ejec).(modelo)(k).nombre;

            if strcmp(ejec,'Secuencial')
                tokens = regexp(nombre,'Gen(\d+)','tokens');
                gen = str2double(tokens{1}{1});
            else
                tokens = regexp(nombre,'Isla(\d+)_Gen(\d+)','tokens');
                isla = str2double(tokens{1}{1});
                gen  = str2double(tokens{1}{2});
                if isla+1 > nIslas, nIslas = isla+1; end
            end

            if gen > maxGen, maxGen = gen; end
        end

        numGens = maxGen + 1;

        %% =======================
        % Preasignación de estadísticos
        %% =======================
        bestCostGlobal   = nan(numGens,1);
        worstCostGlobal  = nan(numGens,1);
        meanCostGlobal   = nan(numGens,1);
        medianCostGlobal = nan(numGens,1);
        stdCostGlobal    = nan(numGens,1);

        if strcmp(ejec,'Paralelo')
            bestCostIsla   = nan(numGens,nIslas);
            worstCostIsla  = nan(numGens,nIslas);
            meanCostIsla   = nan(numGens,nIslas);
            medianCostIsla = nan(numGens,nIslas);
            stdCostIsla    = nan(numGens,nIslas);
        end

        datosPorGenGlobal = cell(numGens,1);
        if strcmp(ejec,'Paralelo')
            datosPorGenIsla = cell(numGens,nIslas);
        end

        %% =======================
        % Extracción de datos
        %% =======================
        for k = 1:nFicheros
            data = Poblacion.(ejec).(modelo)(k).data;
            costes = data{3};
            if isempty(costes), continue; end

            nombre = Poblacion.(ejec).(modelo)(k).nombre;

            if strcmp(ejec,'Secuencial')
                tokens = regexp(nombre,'Gen(\d+)','tokens');
                gen = str2double(tokens{1}{1});
                idx = gen+1;

                % Guardar datos globales
                if isempty(datosPorGenGlobal{idx})
                    datosPorGenGlobal{idx} = costes;
                else
                    datosPorGenGlobal{idx} = [datosPorGenGlobal{idx}; costes];
                end
            else
                tokens = regexp(nombre,'Isla(\d+)_Gen(\d+)','tokens');
                isla = str2double(tokens{1}{1});
                gen  = str2double(tokens{1}{2});
                idx = gen+1;

                % Global
                if isempty(datosPorGenGlobal{idx})
                    datosPorGenGlobal{idx} = costes;
                else
                    datosPorGenGlobal{idx} = [datosPorGenGlobal{idx}; costes];
                end

                % Por isla
                if isempty(datosPorGenIsla{idx,isla+1})
                    datosPorGenIsla{idx,isla+1} = costes;
                else
                    datosPorGenIsla{idx,isla+1} = [datosPorGenIsla{idx,isla+1}; costes];
                end
            end
        end

        %% =======================
        % Estadísticos globales y por isla
        %% =======================
        for g = 1:numGens
            if ~isempty(datosPorGenGlobal{g})
                bestCostGlobal(g)   = min(datosPorGenGlobal{g});
                worstCostGlobal(g)  = max(datosPorGenGlobal{g});
                meanCostGlobal(g)   = mean(datosPorGenGlobal{g});
                medianCostGlobal(g) = median(datosPorGenGlobal{g});
                stdCostGlobal(g)    = std(datosPorGenGlobal{g});
            end

            if strcmp(ejec,'Paralelo')
                for i = 1:nIslas
                    if ~isempty(datosPorGenIsla{g,i})
                        bestCostIsla(g,i)   = min(datosPorGenIsla{g,i});
                        worstCostIsla(g,i)  = max(datosPorGenIsla{g,i});
                        meanCostIsla(g,i)   = mean(datosPorGenIsla{g,i});
                        medianCostIsla(g,i) = median(datosPorGenIsla{g,i});
                        stdCostIsla(g,i)    = std(datosPorGenIsla{g,i});
                    end
                end
            end
        end

        gens = 0:numGens-1;

        %% =======================
        % Gráfica global
        %% =======================
        figCurvas = figure('Color','w','Name',['Evolución Global - ' modelo],'Visible','off');
        plot(gens, bestCostGlobal,   'LineWidth',1.5); hold on;
        plot(gens, meanCostGlobal,   'LineWidth',1.5);
        plot(gens, medianCostGlobal, 'LineWidth',1.5);
        plot(gens, worstCostGlobal,  'LineWidth',1.2);

        xlabel('Generación'); ylabel('Coste');
        title(['GA Global - ' ejec ' / ' modelo]);
        legend('Mejor','Media','Mediana','Peor','Location','best');
        grid on;

        dirCurvas = fullfile(baseGraf,['Curvas_' ejec]);
        saveas(figCurvas, fullfile(dirCurvas,[modelo '_Global.png']));
        close(figCurvas);

        %% =======================
        % Boxplots global
        %% =======================
        figBox = figure('Color','w','Name',['Boxplots Global - ' modelo],'Visible','off');
        gensBox = 0:ventanaBox:(numGens-1);
        allData = []; group = [];
        for w = 1:length(gensBox)
            g = gensBox(w)+1; % índice MATLAB
            if ~isempty(datosPorGenGlobal{g})
                allData = [allData; datosPorGenGlobal{g}];
                group   = [group; repmat(gensBox(w), length(datosPorGenGlobal{g}),1)];
            end
        end
        boxplot(allData, group, 'Positions', gensBox, 'Widths', ventanaBox*0.7);
        xlabel('Generación'); ylabel('Coste');
        title(['Distribución Global cada ' num2str(ventanaBox) ' generaciones']);
        grid on;

        dirBoxplots = fullfile(baseGraf,['Boxplots_' ejec]);
        saveas(figBox, fullfile(dirBoxplots,[modelo '_Global.png']));
        close(figBox);

        %% =======================
        % Opcional: curvas por isla (solo paralelo)
        %% =======================
        if strcmp(ejec,'Paralelo')
            figIslas = figure('Color','w','Name',['Curvas Islas - ' modelo],'Visible','off');
            hold on;
            colors = lines(nIslas);
            for i = 1:nIslas
                plot(gens, medianCostIsla(:,i), 'Color', colors(i,:), 'LineWidth',1.5);
            end
            xlabel('Generación'); ylabel('Coste');
            title(['GA por Isla - ' modelo]);
            legend(arrayfun(@(x) sprintf('Isla %d',x-1), 1:nIslas,'UniformOutput',false),'Location','best');
            grid on;
            saveas(figIslas, fullfile(dirCurvas,[modelo '_Islas.png']));
            close(figIslas);
        end
    end
end

disp('Todas las gráficas de población (secuencial y paralelo) guardadas correctamente.');
