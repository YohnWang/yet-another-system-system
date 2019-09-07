CC = riscv64-unknown-elf-gcc

INCLUDES =  -I./cpu \
			-I./init \
			-I./kernel
	
WARNINGS= -Wall

#-fno-strict-aliasing can use different type in same address. 
#with -fno-builtin we can write same name as the std lib,like 'exit()'
CFLAGS =  \
	$(WARNINGS) $(INCLUDES)\
	-DDEBUG -g \
	-fno-strict-aliasing -fno-builtin \
	-mcmodel=medany 
	
LDFLAGS	 = -T link.ld -nostartfiles -nostdlib 

SRC =  $(wildcard ./*.c ./cpu/*.c ./init/*.c ./kernel/*.c)
SRS =  $(wildcard ./cpu/*.S ./init/*.S)
INC =  $(wildcard ./*.h ./cpu/*.h ./init/*.h ./kernel/*.h)

OBJ1 = $(SRC:%.c=%.o)
OBJ2 = $(SRS:%.S=%.o)
OBJ = $(OBJ1) $(OBJ2)

%.o: %.c $(INC)
	$(CC) -c $(CFLAGS) $< -o $@
	
%.o: %.S 
	$(CC) -c $(CFLAGS) $< -o $@

all: yyos.exe

yyos.exe: $(OBJ) $(INC)
	$(CC) $(LDFLAGS) $(OBJ) -o yyos.exe 

clean:
	-find . -name "*.o" |xargs rm 
	-find . -name "*.exe" |xargs rm

run: all 
	spike yyos.exe
