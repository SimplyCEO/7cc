<img src="./.media/7cc_icon.png" width="32" title="7 Days to Die Code Compiler"> 7 Days to Die Code Compiler
============================================================================================================

7cc is inspired by most C compilers.
But instead of compiling C code to machine language, it translates Lua calls to the given XML files.

7cc operation is simple at first, but later it will be fully developed as compiler and interpreter.

At this very moment, 7cc only works with Lua language.

Building
--------

7cc depends on Lua only and it is required to compile:

```sh
git clone https://codeberg.org/SimplyCEO/7cc.git
cd 7cc/
make BUILD_TYPE=Release
```

Makefile:
- CC: C compiler (`gcc`/\*`clang`/`tcc`);
- CFLAGS: C compiler flags;
- LDFLAGS: Linker flags;
- OSNAME: The Operating System type name (\*`unix32`/`unix64`/`win64`);
- BUILD64: Compile binary for 64-bit architecture (\*`0`/`1`);
- BUILD_TYPE: Define flags based on type (\*`Debug`/`Release`/`None`);
- BUILTIN_LUA: Use the compiled library from repository `vendor` (`0`/\*`1`);
- INSTALL_PREFIX: Installation prefix for built binary (`/usr/local`);

Installation
------------

The compiled binary will be located inside `bin` directory.

```sh
su -c 'make install'
```

For portable installation:

```sh
make INSTALL_PREFIX=./build install
```

TODO
----

- Provide 7CC API documentation;
- Release stable version to public;

