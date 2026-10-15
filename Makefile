GPP = g++ -I./include
RM = rm -rf

OBJ = ./obj/Procedimento.o ./obj/Fila.o ./obj/Escalonador.o ./obj/Evento.o

all: ./src/main.cpp $(OBJ)
	$(GPP) ./src/main.cpp $(OBJ) -o ./bin/tp2.out

./obj/Procedimento.o: ./src/Procedimento.cpp
	$(GPP) -c ./src/Procedimento.cpp -o ./obj/Procedimento.o

./obj/Fila.o: ./src/Fila.cpp
	$(GPP) -c ./src/Fila.cpp -o ./obj/Fila.o

./obj/Escalonador.o: ./src/Escalonador.cpp ./src/Evento.cpp
	$(GPP) -c ./src/Escalonador.cpp -o ./obj/Escalonador.o

./obj/Evento.o: ./src/Evento.cpp
	$(GPP) -c ./src/Evento.cpp -o ./obj/Evento.o

clean:
	$(RM) ./obj/*.o ./bin/*
