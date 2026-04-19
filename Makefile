# Nombre del ejecutable
TARGET = sha0sum

# Detección de sistema operativo
ifeq ($(OS),Windows_NT)
    # Configuración agresiva para MSVC (cl)
    EXE = $(TARGET).exe
    RM = del /Q
    # /O2 (Optimización), /W4 (Nivel de alertas alto), /Fe (Nombre del output)
    CC = cl
    CFLAGS = /O2 /W4 /MD /D_CRT_SECURE_NO_WARNINGS
    COMPILE = $(CC) $(CFLAGS) sha0sum.c /Fe:$(EXE)
else
    # Configuración para Linux
    EXE = $(TARGET)
    RM = rm -f
    CFLAGS = -O3 -Wall
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
