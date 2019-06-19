# Makefile for the dynamic array
# Written by Max Hanson, June 2019
# TODO precompile dynamic_array.h with -c89, -pedantic flags

tests: obj/dynamic_array_tests.o obj/unity.o
	gcc -g -o tests obj/dynamic-array_tests.o obj/unity.o

obj/dynamic_array_tests.o: test/dynamic_array_tests.c
	gcc -g -c -o obj/dynamic_array_tests.o test/dynamic_array_tests.c

obj/unity.o: test/unity.c
	gcc -g -c -o obj/unity.o test/unity.c
