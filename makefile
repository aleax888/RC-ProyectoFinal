# all: crearTablas.exe
# crearTablas.exe:
# 	g++ -o crearTablas.exe crearTablas.cpp -lsqlite3
# clean:
# 	rm -f crearTablas.exe 
all: NA1.exe cliente.exe servidor.exe
NA1.exe:
	g++ -o NA1.exe NA1.cpp -lsqlite3 -lpthread -w
cliente.exe:
	g++ -o cliente.exe cliente.cpp -lpthread -w
servidor.exe:
	g++ -o servidor.exe servidor.cpp -lpthread -w
clean:
	rm -f NA1.exe cliente.exe servidor.exe