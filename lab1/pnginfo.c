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

#define BUF_LEN  (256*16)
#define BUF_LEN2 (256*32)

uint8_t getPngIHDRData(struct data_IHDR *out, FILE *file, uint32_t offset){
  
  fseek(file, offset, SEEK_SET);
  fread(&out->bit_depth, 1, 1, file);
  fseek(file, offset + 1, SEEK_SET);
  fread(&out->color_type, 1, 1, file);
  fseek(file, offset + 2, SEEK_SET);
  fread(&out->compression, 1, 1, file);
  fseek(file, offset + 3, SEEK_SET);
  fread(&out->filter, 1, 1, file);
  fseek(file, offset + 4, SEEK_SET);
  fread(&out->interlace, 1, 1, file);

  return 0;
}

int getPngHeight(FILE *file) {
  int heightPositionOffset = 20;
  int size = 4;

  uint32_t temp = 0;
  uint8_t dimension[4] = {0};
  fseek(file, heightPositionOffset, SEEK_SET);
  fread(dimension, 1, size, file);
  temp = temp | dimension[3];
  temp = temp | (dimension[2] << 8);
  temp = temp | (dimension[1] << 16);
  temp = temp | (dimension[0] << 24);
  return temp;
}

int getPngWidth(FILE *file) {
  int widthPositionOffset = 16;
  int size = 4;

  uint32_t temp = 0;
  uint8_t dimension[4] = {0};
  fseek(file, widthPositionOffset, SEEK_SET);
  fread(dimension, 1, size, file);
  temp = temp | dimension[3];
  temp = temp | (dimension[2] << 8);
  temp = temp | (dimension[1] << 16);
  temp = temp | (dimension[0] << 24);
  return temp;
}

bool isPNG(char *fileName) {
  unsigned char buff[4];
  FILE *file;
  file = fopen(fileName, "rb");

  if (file == NULL) {
    fclose(file);
    return 0;
  }

  int readResult = fread(buff, sizeof(buff), 1, file);

  if (readResult != 1) {
    fclose(file);
    return 0;
  }

  fclose(file);

  // Ignore buff[0] since it is top bit, and there to ensure bytes aren't stripped
  if (buff[1] == 0x50 && buff[2] == 0x4E && buff[3] == 0x47) {
    return true;
  } else {
    return false;
  }
}

int printPngSize(char *fileName) {
  FILE *file;
  file = fopen(fileName, "rb");

  if (file == NULL) exit(2);

  // Get PNG width and height and print
  data_IHDR_p ihdr = malloc(sizeof(struct data_IHDR));
  ihdr->width = getPngWidth(file);
  ihdr->height = getPngHeight(file);
  getPngIHDRData(ihdr, file, 24);

  printf("%s: %d x %d\n", fileName, ihdr->width, ihdr->height);
  free(ihdr);
  fclose(file);

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Inproper command format. Proper Syntax: %s <file name>\n", argv[0]);
    return 0;
  }

  if (!isPNG(argv[1])) {
    printf("%s: Not a PNG file\n", argv[1]);
    return -1;
  }

  printPngSize(argv[1]);

  return 0;
}
