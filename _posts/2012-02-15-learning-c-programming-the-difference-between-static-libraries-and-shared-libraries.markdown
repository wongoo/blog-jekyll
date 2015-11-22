---
author: admin
comments: false
date: 2012-02-15 13:26:30+00:00
excerpt: The difference between Static libraries and Shared libraries
layout: post
slug: learning-c-programming-the-difference-between-static-libraries-and-shared-libraries
title: 'Learning C Programming: The difference between Static libraries and Shared
  libraries'
wordpress_id: 199
categories:
- Experience
tags:
- C
- library
- Programming
---

1. Static libraries are compiled into the program itself, shared libraries are compiled separately and referenced by the program. This enables the program to be much smaller, but requires the shared libraries be available to run.

2. The suffix of Static libraries file is '.a', while Shared libraries is '.so'.

3. When the executable file is started its loader function must find the shared library in order to load it into memory. By default the loader searches for shared libraries only in a predefined set of system directories, such as ‘/usr/local/lib’ and ‘/usr/lib’. If the library is not located in one of these directories it must be added to the load path.The simplest way to set the load path is through the environment variable _**LD_LIBRARY_PATH**_

4. How to Create library file:
Static library:
_>> gcc -Wall -c buddy2/buddy2.c -o obj/buddy2.o_
_        >> ar csr stclib/libbuddy2.a obj/buddy2.o_
Shared library:
_>> gcc -Wall -c -fPIC buddy2/buddy2.c -o obj/buddy2.o_
_        >> gcc -Wall -shared -o shrlib/libbuddy2.so obj/buddy2.o_

5. How to link library :
Static library:
_>> gcc -Wall _-static_ -o buddy_stc buddy_test.o -lbuddy2_
Shared library:
_ >> gcc -Wall -o buddy_shr buddy_test.o libbuddy2.so_



* * *



**GCC Command Options Desc:**

** -Wl,option**
Pass option as an option to the linker. If option contains commas, it is split into multiple options at the commas.
** -static**
On systems that support dynamic linking, this prevents linking with the shared libraries. On other systems, this option has no effect.
** -Wall**
This enables all the warnings about constructions that some users consider questionable, and that are easy to avoid (or modify to prevent the warning), even in conjunction with macros.



* * *



**Reference:**
How to create static library and shared library: [http://www.adp-gmbh.ch/cpp/gcc/create_lib.html](http://www.adp-gmbh.ch/cpp/gcc/create_lib.html)
Shared libraries and static libraries: [http://www.network-theory.co.uk/docs/gccintro/gccintro_25.html
](http://www.network-theory.co.uk/docs/gccintro/gccintro_25.html)GCC link options: [http://gcc.gnu.org/onlinedocs/gcc-3.3.5/gcc/Link-Options.html](http://gcc.gnu.org/onlinedocs/gcc-3.3.5/gcc/Link-Options.html)
GCC Warning options:[ http://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html]( http://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html)
