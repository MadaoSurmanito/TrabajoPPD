# Nombre del ejecutable
TARGET = main

# Compilador
CC = gcc

# Directorios
SRC_DIRS = . Estructuras Operadores

INCLUDE=$(foreach dir,$(SRC_DIRS),-I$(dir) )

# Buscar automáticamente todos los .c dentro de las carpetas
SRC = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))

# Generar lista de .o correspondiente
OBJ = $(SRC:.c=.o)

# Librerías para enlazar
LDFLAGS = -lm

# Flags
CFLAGS = -Wall -Wextra -O2 -g -fopenmp $(INCLUDE)

# Regla por defecto
all: $(TARGET)

# Enlazar ejecutable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

# Compilar cualquier *.c de cualquier directorio
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Ejecutar
run: $(TARGET)
	./$(TARGET)

# Limpiar binarios
clean:
	rm -f $(OBJ) $(TARGET)

# Limpiar completo
fclean: clean
	rm -f core

# Recompilar desde cero
rebuild: fclean all
