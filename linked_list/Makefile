#
# Makefile for the linked list tests and examples.
# Note that the linked list examples are compiled with strict c89 and ansi enforcement,
# but the tests and test framework are not.
#
# Written by Max Hanson, June 2019.
# Licensed under MIT. See LICENSE.txt for more details.
#

clean:
	rm -rf obj/*
	rm -rf tests

tests: obj/linked_list.o obj/linked_list_tests.o obj/unity.o
	gcc -g -o tests obj/linked_list.o obj/linked_list_tests.o obj/unity.o

obj/linked_list.o: src/linked_list.c
	gcc -std=c89 -ansi -pedantic -g -c -o obj/linked_list.o src/linked_list.c

obj/linked_list_tests.o: src/linked_list_tests.c
	gcc -g -c -o obj/linked_list_tests.o src/linked_list_tests.c

obj/unity.o: ../deps/unity/unity.c
	gcc -g -c -o obj/unity.o ../deps/unity/unity.c
