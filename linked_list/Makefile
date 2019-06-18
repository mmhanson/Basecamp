#
# Makefile for the linked list and its tests.
# Note that the linked list structure is compiled with strict c89 enforcement, but
# the tests and test framework are not.
#
# Written by Max Hanson, June 2019.
#

clean:
	rm -rf obj/*
	rm -rf tests

tests: obj/linked_list.o obj/linked_list_tests.o obj/unity.o
	gcc -g -o tests obj/linked_list.o obj/linked_list_tests.o obj/unity.o

obj/linked_list.o: linked_list.c
	gcc -std=c89 -pedantic -g -c -o obj/linked_list.o linked_list.c

obj/linked_list_tests.o: test/linked_list_tests.c
	gcc -g -c -o obj/linked_list_tests.o test/linked_list_tests.c

obj/unity.o: test/unity.c
	gcc -g -c -o obj/unity.o test/unity.c
