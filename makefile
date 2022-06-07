# -lpthread
all: cliente.exe
cliente.exe:
	g++ -o cliente.exe cliente.cpp
clean:
	rm -f cliente.exe 