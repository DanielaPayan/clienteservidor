CC=g++


all: client bank

client: client.cc
	$(CC) -c client.cc -std=gnu++11
	$(CC) -o client client.o -lzmq -lczmq
	

bank:  bank.cc
	$(CC) -c bank.cc -std=gnu++11
	$(CC) -o bank bank.o -lzmq -lczmq

clean:
	rm -f client.o bank.o client bank *~