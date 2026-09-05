7 Days to Die Code Compiler
===========================

7cc is inspired by most C compilers.
But instead of compiling C code to machine language, it translates Lua calls to the given XML files.

7cc operation is simple at first, but later it will be fully developed as compiler and interpreter.

At this very moment, 7cc only works with Lua language.

Building
--------

7cc depends on Lua only and it is required to compile:
```sh
make BUILD_TYPE=Release
```

Makefile:
- CC: C compiler;
- CFLAGS: C compiler flags;
- LDFLAGS: Linker flags;
- BUILD_TYPE: `Debug` or `Release`;
- BUILD_STATIC: Force statically linking objects;
- INSTALL_PREFIX: Not fully implemented;

TODO
----

- Receive arguments and apply flags;
- Develop the entire game `Data` XML API;
- Provide 7CC API documentation;
- Crossplatform code linking;
- Release stable version to public;

