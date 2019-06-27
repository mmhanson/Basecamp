# Makefile for the dynamic array
#
# Written by Max Hanson, June 2019
# Licensed under MIT. See LICENSE.txt for more details.
# TODO precompile dynamic_array.h with -c89, -pedantic flags

clean:
	rm -rf obj/*
	rm -rf tests
	rm -rf driver

tests: obj/dynamic_array_tests.o obj/unity.o
	gcc -g -o tests obj/dynamic_array_tests.o obj/unity.o

driver: obj/dynamic_array_driver.o
	gcc -g -o driver obj/dynamic_array_driver.o

obj/dynamic_array_driver.o: dynamic_array_driver.c
	# A basic driver to test compiling the dynamic_array.h under pediantic C89 and ansi standards
	gcc -g -c --std=c89 -ansi -pedantic -o obj/dynamic_array_driver.o dynamic_array_driver.c

obj/dynamic_array_tests.o: test/dynamic_array_tests.c
	gcc -g -c -o obj/dynamic_array_tests.o test/dynamic_array_tests.c

obj/unity.o: test/unity.c
	gcc -g -c -o obj/unity.o test/unity.c
