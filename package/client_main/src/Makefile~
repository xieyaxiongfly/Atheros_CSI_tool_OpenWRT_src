PROFILE = -O2 -s
CFLAGS = $(PROFILE)

all: client_main 

csi_func.o: csi_func.c csi_func.h
	$(CC) $(CFLAGS) -c csi_func.c

client_main.o: client_main.c csi_func.h
	$(CC) $(CFLAGS) -c client_main.c

client_main: client_main.o csi_func.o
	$(CC) -o client_main client_main.o csi_func.o $(LDFLAGS)

clean:
	rm -f *.o client_main

