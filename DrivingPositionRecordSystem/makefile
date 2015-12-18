obj=fileOperation.o list.o  main.o menu.o function.o 
all1:
gcc main.c RWFileOperation.c RWFileOperation.h RWFunction.c RWFunction.h RWGps.h RWGps.c RWList.c RWList.h RWMenu.c RWMenu.h RWTime.h RWTime.c
all:$(obj)
	gcc $(obj) -o all

list.o: list.c list.h 
	gcc -c list.c list.h
menu.o: menu.c
	gcc -c menu.c menu.h
function.o:function.c function.h
	gcc -c function.c function.h menu.h list.h 
fileOperation.o:fileOperation.c
	gcc -c fileOperation.c

clean:
	rm -f *.o *.txt
