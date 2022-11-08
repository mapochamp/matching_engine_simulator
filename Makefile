CC=g++
FLAGS= -Iinclude/ -Wall --std=c++17 -g

#The Binary is the main thing to make
bin/book: src/simple_cross.cpp obj/orderbook.o obj/matching_engine.o obj/parser.o obj/order.o
	${CC} ${CFLAGS} -o bin/simple_cross src/simple_cross.cpp obj/*.o

#Build all the objects we need
obj/matching_engine.o: src/matching_engine.cpp src/matching_engine.h src/orderbook.h
	${CC} ${CFLAGS} -c -o obj/matching_engine.o src/matching_engine.cpp

obj/orderbook.o: src/orderbook.cpp src/orderbook.h src/order.h
	${CC} ${CFLAGS} -c -o obj/orderbook.o src/orderbook.cpp

obj/parser.o: src/parser.cpp src/parser.h src/order.h
	${CC} ${CFLAGS} -c -o obj/parser.o src/parser.cpp

obj/order.o: src/order.cpp src/order.h
	${CC} ${CFLAGS} -c -o obj/order.o src/order.cpp

#Clean up everything
clean:
	rm -f obj/*.o
	rm -f bin/book
