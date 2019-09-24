# Makefile for graph examples and tests.
#
# Written by Max Hanson, September 2019.
# Released into the public domain under CC0. See README.txt for details.

clean:
	rm -rf obj/*
	rm -rf tests
	rm -rf example

tests: obj/graph_tests.o obj/unity.o src/graph.h
	gcc -g -o tests obj/graph_tests.o obj/unity.o

example: obj/graph_example.o
	gcc -g -o example obj/graph_example.o

obj/graph_example.o: src/graph_example.c
	# Compile under strct c89 and ansi standards
	mkdir -p obj
	gcc -g -c --std=c89 -ansi -pedantic -o obj/graph_example.o src/graph_example.c

obj/graph_tests.o: src/graph_tests.c
	mkdir -p obj
	gcc -g -c -o obj/graph_tests.o src/graph_tests.c

obj/unity.o: ../deps/unity/unity.c
	mkdir -p obj
	gcc -g -c -o obj/unity.o ../deps/unity/unity.c
