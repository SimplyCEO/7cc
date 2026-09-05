#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include "main.h"

#include "lua.h"
#include "safe_alloc.h"
#include "toolbox.h"
#include "types.h"
#include "xml.h"

bool compile = false;
char* output = NULL;
int identation = 0;

static void
help(void)
{
  printf(
    "7 Days to Die Code Compiler %d.%d.%d - Copyright(C) 2026 SimplyCEO <simplyceo.developer@gmail.com>\n"
    "Usage: 7cc [options...] [-o outfile] [-c] infile(s)...\n"
    "       7cc [options...] infile (or --) [arguments...]\n"
    "General options:\n"
    "  -c             compile only - generate an xml file\n"
    "  -o outfile     set output filename\n"
    "  -i identation  single line (-1), hard tabs (*0), soft tabs(1)\n"
    "  -v --version show version\n", PROJECT_MAJOR, PROJECT_MINOR, PROJECT_PATCH
  );
}

static void
version(void)
{
  printf("7 Days to Die Code Compiler %d.%d.%d - Copyright(C) 2026 SimplyCEO <simplyceo.developer@gmail.com>\n", PROJECT_MAJOR, PROJECT_MINOR, PROJECT_PATCH);
}

int
main(int argc, char* argv[])
{
  if (argc == 1)
  { help(); return 1; }

  size_t i = 0;
  const char* l_file = NULL;
  for (; i<=argc; i++)
  {
    size_t length = strlen(argv[i]);
    if (length > 4)
    {
      if (strncmp(argv[i] + length - 4, ".lua", 4) == 0)
      { l_file = argv[i]; break; }
    }
  }

  const struct option opts[] =
  {
    { .name = "compile-only", .has_arg = no_argument,       .flag = NULL, .val = 'c' },
    { .name = "outfile",      .has_arg = required_argument, .flag = NULL, .val = 'o' },
    { .name = "identation",   .has_arg = required_argument, .flag = NULL, .val = 'i' },
    { .name = "help",         .has_arg = no_argument,       .flag = NULL, .val = 'h' },
    { .name = "version",      .has_arg = no_argument,       .flag = NULL, .val = 'v' }
  };

  int opt = 0;
  int opt_index = 0;
  while ((opt = getopt_long(argc, argv, "co:i:hv", opts, &opt_index)) != -1)
  {
    switch (opt)
    {
      case 'c': compile = true; break;
      case 'o': output = strdup(optarg); break;
      case 'i': identation = atoi(optarg); break;
      case 'v': version(); return 0;
      default: help(); return 0;
    }
  }

  l_init();
  l_run(l_file);
  l_free();

  safe_free(output);

  return 0;
}

