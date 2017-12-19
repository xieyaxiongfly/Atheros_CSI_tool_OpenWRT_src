PROFILE = -O2 -s
CFLAGS = $(PROFILE)

all: recvCSI

csi_fun.o: csi_fun.c
	$(CC) $(CFLAGS) -c csi_fun.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

recvCSI: main.o csi_fun.o
	$(CC) -o recvCSI main.o csi_fun.o $(LDFLAGS)

clean:
	rm -f *.o recvCSI

