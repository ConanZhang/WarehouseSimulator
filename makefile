# Filename: makefile
# Author: Conan Zhang
# Last Modified: 2/9/2015
# Version: 1.0

# Make file for transaction simulation.


simulate: report.o transaction.o warehouse.o food_item.o
	g++  report.o transaction.o warehouse.o food_item.o

report.o: report.cpp transaction.h
	g++ -c report.cpp

transaction.o: transaction.cpp transaction.h warehouse.h food_item.h
	g++ -c transaction.cpp

warehouse.o: warehouse.cpp warehouse.h food_item.h
	g++ -c warehouse.cpp

food_item.o: food_item.cpp food_item.h
	g++ -c food_item.cpp

clean:
	rm -f *.o a.out