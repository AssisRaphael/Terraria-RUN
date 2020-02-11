CC=gcc
OPCOES_COMPILADOR=-I. -lSOIL -lglut -lGL -lGLEW -lGLU -lSDL -lSDL_mixer
TODOS_ARQUIVOS_OBJ = Terraria_Run.o

%.o: ../%.c $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	gcc -o main $^ $(OPCOES_COMPILADOR)

run: all
	./main

clean:
	rm *.o main

