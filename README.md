# Basecamp
RIght now this repository is a work-in-progress and this readme is more a series of notes of what I'll actually put in the readme when the repository is more developed.

Basic data structures implemented from scratch in pure, old-fashioned C89.

The goals of this project are to strengthen my knowledge of, and experience working with, C and to refresh my knowledge of basic 
data structures. These data structures are all implemented from scratch without the standard library. The data structures also
include basic operations associated with them, but nothing advanced enough to consider an algorithm.

All of these data structures are thoroughly tested with [Unity](https://github.com/ThrowTheSwitch/Unity). Githubs language
detection says that much of this project is C++, but thats incorrect. All of it is in C with a tiny bit of Makefile. All data
structures are implemented from scratch with no standard library headers except "stdlib.h". This is unavoidable as many data
structures heavily use pointers (such as the linked list) and the 'malloc' function is required there. The data structure classes
and operations are all compiled with the '-std=89' and '-pedantic' flags with no warnings, but the tests and test frameworks are
not.
