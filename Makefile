TARGET = sha0sum

ifeq ($(OS),Windows_NT)
    # Configuración para Windows (MSVC - cl)
    EXE = $(TARGET).exe
    RM = del /Q
    CC = cl
    # En MSVC, el nombre del archivo de salida se pone con /Fe:
    OUT_FLAG = /Fe:$(EXE)
    CFLAGS = /O2 /W4 /MD /D_CRT_SECURE_NO_WARNINGS
else
    # Configuración para Linux (GCC)
    EXE = $(TARGET)
    RM = rm -f
    CC = gcc
    OUT_FLAG = -o $(EXE)
    CFLAGS = -O3 -Wall
endif

# Regla principal corregida usando variables
$(EXE): sha0sum.c
	$(CC) $(CFLAGS) sha0sum.c $(OUT_FLAG)

clean:
	$(RM) $(EXE) *.obj *.o 2>nul || true

test: $(EXE)
	./$(EXE) abc
