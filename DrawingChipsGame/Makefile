objects = main.o panel.o gamerunning.o computer.o

game : $(objects)
	gcc -o game $(objects)

$(objects) : panel.h

.PHONY : clean

clean :
	rm game *.o
