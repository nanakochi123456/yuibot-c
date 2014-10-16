# for debug
#CC=gcc
#CCOPT=-g -DDEBUG=1

# stable
CC=gcc
CCOPT = -O2 

# gcc 2.7.x... max
#CC=gcc
#CCOPT = -O4 -fstrength-reduce -fexpensive-optimizations -fomit-frame-pointer -funroll-loops -ffast-math -fdefer-pop

# gcc 2.95.x max
#CC=gcc
#CCOPT= -O9  -DREGISTER=register-fstrength-reduce -fexpensive-optimizations -finline-functions -fomit-frame-pointer -funroll-loops -ffast-math -foptimize-register-move -fdefer-pop

# gcc 2.95.x or pgcc 2.95.x max (intel only) ÉTÉCÉYÇÕÇøÇÂÇ¡Ç∆ëùÇ¶Ç‹Ç∑
#CC=pgcc
#CCOPT=-O9  -DREGISTER=register  -mcpu=pentiumpro -march=pentiumpro -fstrength-reduce -fexpensive-optimizations -finline-functions -fomit-frame-pointer -funroll-loops -ffast-math -foptimize-register-move -fdefer-pop

OBJS=chat.o tag.o decode.o error.o jc.o util.o value.o bot.o gethost.o tbl.o
TARGET=chat.cgi

$(TARGET): $(OBJS)
	$(CC) -o ./$@ $?
	rm *.o
#	strip $(TARGET)

.c.o:
	@$(ECHO) "// Compiling $< ($(CC) $(CCOPT))"
	@$(CC) -c $(CCOPT) $< -o $@

clean:
	rm -f *.o
