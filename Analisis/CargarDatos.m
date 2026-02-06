clc; clear; close all;

%% Directorio base
baseDir = 'Analisis/Datos';

%% Tipos de ejecución
tiposEjec = {'Secuencial', 'Paralelo'};

%% Subtipos dentro de cada ejecución
subtipos = {'SinNeurona', ...
            'Neurona_CH', ...
            'Neurona_FS', ...
            'Neurona_IB', ...
            'Neurona_LTS', ...
            'Neurona_RS', ...
            'Neurona_RZ', ...
            'Neurona_TC1', ...
            'Neurona_TC2'};

%% Carpetas finales
carpetasFinales = {'Tiempos', 'Mutacion', 'Poblacion'};

%% Inicializar estructuras
Tiempos = struct();
Mutacion = struct();
Poblacion = struct();

%% Recorremos cada tipo de ejecución
for t = 1:length(tiposEjec)
    ejec = tiposEjec{t};
    
    for s = 1:length(subtipos)
        sub = subtipos{s};
        
        for c = 1:length(carpetasFinales)
            carpetaFinal = carpetasFinales{c};
            
            % Construir la ruta completa
            ruta = fullfile(baseDir, ejec, sub, carpetaFinal);
            
            % Comprobar si existe
            if ~isfolder(ruta)
                warning('La carpeta %s no existe, se omite.', ruta);
                continue;
            end
            
            %% Dependiendo de la carpeta final
            switch carpetaFinal
                case {'Tiempos', 'Mutacion'}
                    archivos = dir(fullfile(ruta, '*.txt'));
                    for f = 1:length(archivos)
                        nombreVar = erase(archivos(f).name, '.txt');
                        data = load(fullfile(ruta, archivos(f).name));
                        if strcmp(carpetaFinal,'Tiempos')
                            Tiempos.(ejec).(sub).(nombreVar) = data;
                        else
                            Mutacion.(ejec).(sub).(nombreVar) = data;
                        end
                    end
                    
                case 'Poblacion'
                    if strcmp(ejec,'Secuencial')
                        % Archivos directamente en la carpeta
                        archivos = dir(fullfile(ruta, '*.txt'));
                        for f = 1:length(archivos)
                            archivoNombre = archivos(f).name;
                            fid = fopen(fullfile(ruta, archivoNombre), 'r');
                            data = textscan(fid, '%f %s %f'); % num - string - num
                            fclose(fid);
                            Poblacion.(ejec).(sub)(f).data = data;
                            Poblacion.(ejec).(sub)(f).nombre = archivoNombre;
                        end
                    else
                        % Paralelo: cada IslaX es una subcarpeta
                        islas = dir(ruta);
                        islas = islas([islas.isdir]);
                        islas = islas(~ismember({islas.name},{'.','..'}));
                        
                        fCounter = 0;
                        for i = 1:length(islas)
                            rutaIsla = fullfile(ruta, islas(i).name);
                            archivos = dir(fullfile(rutaIsla, '*.txt'));
                            for f = 1:length(archivos)
                                fCounter = fCounter + 1;
                                archivoNombre = archivos(f).name;
                                fid = fopen(fullfile(rutaIsla, archivoNombre), 'r');
                                data = textscan(fid, '%f %s %f'); % num - string - num
                                fclose(fid);
                                Poblacion.(ejec).(sub)(fCounter).data = data;
                                % Nombre único combinando IslaX + archivo
                                Poblacion.(ejec).(sub)(fCounter).nombre = [islas(i).name '_' archivoNombre];
                            end
                        end
                    end
            end
        end
    end
end

disp('Carga completada. Guardando en .mat...');

%% Guardar todo en un único .mat para futuras cargas rápidas
save('DatosCompletos.mat', 'Tiempos', 'Mutacion', 'Poblacion', '-v7.3');

disp('Guardado en DatosCompletos.mat listo para usar.');
