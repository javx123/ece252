#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include "crc.h"
#include "zutil.h"
#include "lab_png.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Inproper command format.");
    return -1;
  }

  return 0;
}
