mpicxx -I ../mrmpi/src -c -o 9-2.o 9-2.cpp
mpicxx -o 9-2 9-2.o ../mrmpi/src/libmrmpi_linux.a
mpirun -n 1 ./9-2 ../texts/1.txt