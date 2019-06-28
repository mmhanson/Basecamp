[![CC0 Stamp](http://i.creativecommons.org/p/zero/1.0/88x31.png)](http://creativecommons.org/publicdomain/zero/1.0/)

To the extend possible under law, [Max Hanson](https://github.com/mmhanson/Basecamp) has waived all copyright and
related or neighboring rights to Basecamp. This work is published from: United
States.

# Basecamp
A header-only library of basic data structures and algorithms implemented from
scratch in C89.

This project comes from my desire for a library of basic data structures and
algorithms in C that *just works*. When engineers programming in C are faced
with the need for a data structure or algorithm, often the response is to just
"roll your own" right into the code where it's needed. I believe this ultimately
results in subpar software. Injecting data structures and algorithms into code
where it is needed reduces readability. Oftentimes these data structures and
algorithms are implemented many, *many* times. How many linked lists have been
written in C? Doing this is not only unproductive, it opens the door for bugs to
nest deep into software. Mishandling pointers is incredibly easy. Forever
rewriting linked lists is asking for trouble.

But those points are not new and attempts to remedy this situation have been
made. System-specific libraries exist like [BSD-libc](https://en.wikipedia.org/wiki/C_standard_library#BSD_libc) or [glibc](https://en.wikipedia.org/wiki/GNU_C_Library), but these aren't
portable or extensive enough for most use cases. Large-scale projects in C like
the [Linux kernel](https://en.wikipedia.org/wiki/Linux_kernel) often consolidate data structures and algorithms like the
previously mentioned [linked list](https://github.com/torvalds/linux/blob/master/include/linux/list.h), but borrowing these implementations often
means entering [legal gray-areas](https://en.wikipedia.org/wiki/Viral_license) and including half the project as dependencies.
Many system-independent libraries already exist such as [glib](https://github.com/GNOME/glib), [klib](https://github.com/attractivechaos/klib), [stb lib](https://github.com/nothings/stb),
and [CCAN](https://github.com/rustyrussell/ccan/). But headers in these libraries are also extremely codependent.
Furthermore, documentation in these libraries is often lacking and testing is
nowhere to be seen. Trade-offs are central to computer science, but engineers
shouldn't have to choose between a buggy library, an undocumented library, a
bloated library, a cryptic library, or no library.

None of these solutions meet the need for an easy to use, portable, and lean
library with clear licensing and documentation. This project is my attempt to
fill that need.


## Project Goals
This project is designed to be:
  * Easy to understand and integrate into other projects. 
    * All code is well and clearly documented.
      * Names of all functions, structs, and macros are designed to be
        self-documenting and intuitive rather than terse and complex.
      * The precise behavior and purpose for each function, struct and macro
        are thoroughly and plainly documented above its definition.
      * Examples, usage guides, and readmes are included *for each header*.
    * All data structures and algorithms are header-only.
    * All original code is public domain. `#include` them without licensing
      headaches or worry!
    * All data structures and algorithms are written in strict C89 for
      maximum portability.
    * All data structures and algorithms are strictly generic.
  * Lean and fast.
    * Only the most basic and useful operations are included in each header to
      minimize bloat.
    * Headers do not manage memory at all (i.e. call `malloc` or `free`).
  * Bug-free.
    * All headers are thoroughly unit-tested with [Unity](https://github.com/ThrowTheSwitch/Unity).

## Project Navigation
At the top of this repository, there is a 'deps' folder for the Unity testing
framework and a folder for each data structure and algorithm header. In each
sub-folder is the header for that data structure or algorithm, examples of
usage, unit tests, and a readme.

## Using Headers
To use a header just find it, download it, paste it in your project, and
`#include` it. That's it. All headers are public domain, no need to fumble with
licensing details. However, you cannot remove my name as author from any header
you include in your project. That would be plagiarism.
