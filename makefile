# Nombre del ejecutable
TARGET = main

# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -g
LDFLAGS = -lm

# Fuentes del proyecto
SRC = main.c cruce.c evaluar.c grafo.c mutacion.c poblacion.c seleccion.c algSec.c

# Objetos generados
OBJ = $(SRC:.c=.o)

# Regla por defecto: compilar
all: $(TARGET)

# Enlazar el programa final
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

# Regla genérica para compilar cada .c → .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Limpiar archivos generados
clean:
	rm -f $(OBJ) $(TARGET)

# Limpiar todo incluyendo core dumps
fclean: clean
	rm -f core

# Recompilar desde cero
rebuild: fclean all
