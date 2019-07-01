# Makefile for dynamic array examples and tests.
#
# Written by Max Hanson, June 2019.
# Released into the public domain under CC0. See README.md for more details.

clean:
	rm -rf obj/*
	rm -rf tests
	rm -rf example

tests: obj/dynamic_array_tests.o obj/unity.o
	gcc -g -o tests obj/dynamic_array_tests.o obj/unity.o

example: obj/dynamic_array_example.o
	gcc -g -o example obj/dynamic_array_example.o

obj/dynamic_array_example.o: src/dynamic_array_example.c
	# Compile example under strict c89 and ansi standards.
	gcc -g -c --std=c89 -ansi -pedantic -o obj/dynamic_array_example.o src/dynamic_array_example.c

obj/dynamic_array_tests.o: src/dynamic_array_tests.c
	gcc -g -c -o obj/dynamic_array_tests.o src/dynamic_array_tests.c

obj/unity.o: ../deps/unity/unity.c
	gcc -g -c -o obj/unity.o ../deps/unity/unity.c
