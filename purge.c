#include "purge.h"
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void print_border(char stroke, int width, int col)
{
  for (int i = 0; i < width; i++) {
    if (i == 0 || i == col || i == width - 1)
      printf("+");
    else
      printf("%c", stroke);
  }
  printf("\n");
}

void print_row(char *label, char *value, int width, int col)
{
  for (int i = 0; i < width; i++) {
    if (i == 0 || i == col || i == width - 1)
      printf("|");
    else if (i == 2) 
      i += printf("%s", label) - 1;
    else if (col > 0 && i == col + 2)
      i += printf("%s", value) - 1;
    else
      printf(" ");
  }
  printf("\n");
}

void print_entry(struct dirent *entry, struct stat s)
{
  static char thick = '=';
  static char thin = '-';
  static int width = 72;
  static int col = 12;

  print_border(thick, width, 0);
  print_row(entry->d_name, NULL, width, 0);
  print_border(thick, width, col);
  char size[32];
  sprintf(size, "%lld", s.st_size);
  print_row("Size", size, width, col);
  print_border(thin, width, col);
  char *type;
  type = (entry->d_type == DT_REG) ? "Regular file" : "Symbolic link";
  print_row("Type", type, width, col);
  print_border(thin, width, col);
  char time[64];
  sprintf(time, "%ld", s.st_mtimespec.tv_sec);
  print_row("Modified", time, width, col);
  print_border(thin, width, col);
  char accessed[64];
  sprintf(accessed, "%ld", s.st_atimespec.tv_sec);
  print_row("Accessed", accessed, width, col);
  print_border(thick, width, 0);
}
  

int purge(char *path) 
{
  struct dirent *entry;
  DIR *dir;

  dir = opendir(path);
  if (dir == NULL) {
    printf("Unable to open directory '%s'\n", path);
    return -1;
  }

  while ((entry = readdir(dir)) != NULL) {
    if ((DT_REG == entry->d_type || DT_LNK == entry->d_type) && ('.' != entry->d_name[0])) {
      size_t len = strlen(path) + entry->d_namlen;
      char file[len];
      sprintf(file, "%s/%s", path, entry->d_name);

      struct stat s;
      int code = lstat(file, &s);
      if (code < 0) {
	printf("Unable to read '%s'\n", entry->d_name);
      }
      else {
	print_entry(entry, s);
	printf("Delete %s? [Y/n]\n", entry->d_name);
	char c;
	c = getchar();
	if (!('N' == c || 'n' == c)) {
	  int result = remove(file);
	  if (result != 0) 
	    printf("Could not remove file '%s'\n", file);
	  else
	    printf("File '%s' was removed\n", file);
	}
      }
    }
  }

  return 0;
}

