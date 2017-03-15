mpicxx -I ../mrmpi/src -c -o 9-1.o 9-1.cpp
mpicxx -o 9-1 9-1.o ../mrmpi/src/libmrmpi_linux.a
mpirun -n 1 ./9-1 ../texts/1.txt