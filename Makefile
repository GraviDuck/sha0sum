# Nombre del ejecutable
TARGET = sha0sum

# Detección de sistema operativo
ifeq ($(OS),Windows_NT)
    # Configuración para Windows
    EXE = $(TARGET).exe
    RM = del /Q
    CFLAGS = -O2 -Wall
else
    # Configuración para Linux
    EXE = $(TARGET)
    RM = rm -f
    CFLAGS = -O2 -Wall
endif

# Regla principal: Compilar el ejecutable
$(EXE): sha0sum.c
	gcc $(CFLAGS) sha0sum.c -o $(EXE)

# Regla para limpiar archivos basura
clean:
	$(RM) $(EXE) *.obj *.o

# Regla para ejecutar una prueba rápida (abc)
test: $(EXE)
	./$(EXE) abc
