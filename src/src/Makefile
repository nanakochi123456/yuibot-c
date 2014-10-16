CC = gcc -O2 
OBJS = chat.o decode.o error.o jc.o
TAGET = chat.cgi

chat.cgi: chat.o decode.o error.o jc.o
	$(CC) -o ./$@ $?
	rm *.o
