#include "purge.h"
#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>


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
  #define time_size 20
  static const char *time_fmt = "%F %T";
  static const char thick = '=';
  static const char thin = '-';
  static const int width = 72;
  static const int col = 12;

  print_border(thick, width, 0);
  print_row(entry->d_name, NULL, width, 0);
  print_border(thick, width, col);
  char size[32];
  sprintf(size, "%lld", s.st_size);
  print_row("Size", size, width, col);
  print_border(thin, width, col);
  char *type;
  switch (entry->d_type) {
    case DT_REG:
      type = "Regular file";
      break;
    case DT_LNK:
      type = "Symbolic link";
      break;
    default:
      type = "Unrecognized";
      break;
  }
  print_row("Type", type, width, col);
  print_border(thin, width, col);
  char mtime[time_size];
  strftime(mtime, time_size, time_fmt, localtime(&s.st_mtimespec.tv_sec));
  print_row("Modified", mtime, width, col);
  print_border(thin, width, col);
  char atime[time_size];
  strftime(atime, time_size, time_fmt, localtime(&s.st_atimespec.tv_sec));
  print_row("Accessed", atime, width, col);
  print_border(thick, width, 0);
}
  
bool delete_confirmed()
{
  char c;
  struct termios oldt, newt;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  printf("Would you like to delete this file? [Y/n]\n");
  c = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return !('N' == c || 'n' == c);
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
	if (delete_confirmed()) {
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

