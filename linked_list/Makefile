#
# Makefile for the linked list tests and examples.
# Note that the linked list examples are compiled with strict c89 and ansi enforcement,
# but the tests and test framework are not.
#
# Copyright (c) Max Hanson, June 2019.
# Released under the MIT License. Refer to LICENSE.txt for more details.
#

clean:
	rm -rf obj/*
	rm -rf tests
	rm -rf example

tests: obj/linked_list_tests.o obj/unity.o
	gcc -g -o tests obj/linked_list_tests.o obj/unity.o

example: obj/linked_list_example.o
	gcc -o example obj/linked_list_example.o

obj/linked_list_example.o: src/linked_list_example.c
	gcc -std=c89 -ansi -pedantic -c -g -o obj/linked_list_example.o src/linked_list_example.c

obj/linked_list_tests.o: src/linked_list_tests.c
	gcc -g -c -o obj/linked_list_tests.o src/linked_list_tests.c

obj/unity.o: ../deps/unity/unity.c
	gcc -g -c -o obj/unity.o ../deps/unity/unity.c
