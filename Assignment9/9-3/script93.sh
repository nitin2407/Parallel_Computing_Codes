mpicxx -I ../mrmpi/src -c -o 9-3.o 9-3.cpp
mpicxx -o 9-3 9-3.o ../mrmpi/src/libmrmpi_linux.a
mpirun -n 1 ./9-3 ../texts/1.txt