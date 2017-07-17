main:main.o printf1.o
	gcc -o main main.o printf1.o
main.o:main.c printf1.h
	gcc -c -o main.o main.c
printf1.o:printf1.c printf1.h
	gcc -c -o printf1.o printf1.c
clean:
	rm -rfv *.o main 
