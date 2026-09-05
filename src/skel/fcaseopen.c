#define _DEFAULT_SOURCE

#include "fcaseopen.h"

#if !defined(_WIN32)
# include <stdlib.h>
# include <string.h>

# include <dirent.h>
# include <errno.h>
# include <unistd.h>

# include "types.h"

/* real_path must have strlen(path) + 3 bytes */
const char*
casepath(char const *path)
{
  char *real_path = (char*)malloc(sizeof(char)*(strlen(path)+3));
    char *dir_path = (char*)malloc(sizeof(char)*(strlen(path)+1));
  strcpy(dir_path, path);
  size_t rp_length = 0;

  /*
   * If given path is found at root level, move towards it and drag the position 1 byte further.
   * Otherwise, move to local directory and set real_path accordingly.
   */  
  DIR *directory;
  switch (dir_path[0])
  {
    case '/':
    {
      directory = opendir("/");
      dir_path = dir_path + 1;
    }
    default:
    {
      directory = opendir(".");
      real_path[0] = '.';
      real_path[1] = 0;
      rp_length = 1;
    } break;
  }
  
  /* Get an array of tokens separated by the '/' character. */
  int last = 0;
  char *token = strsep(&dir_path, "/");
  while (token)
  {
    /* If the directory of provided tokens do not exist, then exit function. */
    if (!directory)
    { goto end; }
    
    /* No file found will return an error. */
    switch (last)
    {
      case 0: break;
      default:
      {
        closedir(directory);
        goto end;
      } break;
    }
    
    /* Add a separator between last token and the new one. */
    real_path[rp_length] = '/';
    rp_length += 1;
    real_path[rp_length] = 0;
    
    /*
     * List all entries from given directory path while it is valid.
     * If the given entry is valid, the path will be copied to real_path.
     * If invalid, it will read again.
     */
    struct dirent *entry = readdir(directory);
    bool validation = false;
    while (entry && !validation)
    {
      switch (strcasecmp(token, entry->d_name))
      {
        case 0:
        {
          strcpy(real_path + rp_length, entry->d_name);
          rp_length += strlen(entry->d_name);

          closedir(directory);
          directory = opendir(real_path);

          validation = true;
        } break;
        default: entry = readdir(directory); break;
      }
    }
    
    /* If no entry is found it means the directory does not contain any, therefore it will not reveal a file. */
    if (!entry)
    {
      strcpy(real_path + rp_length, token);
      rp_length += strlen(token);
      last = 1;
    }
    
    /* Again, get an array of tokens separated by the '/' character. */
    token = strsep(&dir_path, "/");
  }
  
  /* If directory is still loaded, close it. */
  if (directory)
  { closedir(directory); }

end:
  free(dir_path);
  return real_path;
}
#endif

/*
 * Windows: Read file from given path.
 * Any other OS: Read file using case sensitivity filter on given path.
 */
FILE*
fcaseopen(char const *path, char const *mode)
{
  FILE *stream = fopen(path, mode);
  #if !defined(_WIN32)
    if (!stream)
    {
      char *real_path = (char*)casepath(path);
      if (real_path)
      {
        stream = fopen(real_path, mode);
        free(real_path);
      }
    }
  #endif
  return stream;
}

/*
 * Windows: Change directory to given path.
 * Any other OS: Change directory using case sensitivity filter on given path.
 */
void
casechdir(char const *path)
{
  #if !defined(_WIN32)
    char *real_path = (char*)casepath(path);
    if (real_path)
    {
      chdir(real_path);
      free(real_path);
    }
    else
    { errno = ENOENT; }
  #else
    chdir(path);
  #endif
}

