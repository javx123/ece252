#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char* getFileType(char *directory) {
  char *ptr = NULL;
  struct stat buf;

  if (lstat(directory, &buf) < 0) {
    printf("%s\n", directory);
    perror("lstat error");
    return ptr;
  }

  if (S_ISREG(buf.st_mode))  ptr = "regular";
  else if (S_ISDIR(buf.st_mode))  ptr = "directory";
  else if (S_ISCHR(buf.st_mode))  ptr = "character special";
  else if (S_ISBLK(buf.st_mode))  ptr = "block special";
  else if (S_ISFIFO(buf.st_mode)) ptr = "fifo";
#ifdef S_ISLNK
  else if (S_ISLNK(buf.st_mode))  ptr = "symbolic link";
#endif
#ifdef S_ISSOCK
  else if (S_ISSOCK(buf.st_mode)) ptr = "socket";
#endif
  else ptr = "**unknown mode**";
  return ptr;
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

int checkForPNG(char *directory, bool *containsPNG) {
  strcat(directory, "/");
  DIR *pDir = opendir(directory);

  if (pDir == NULL) exit(2);

  struct dirent *pDirent;

  while ((pDirent = readdir(pDir)) != NULL) {
    if (pDirent->d_name == NULL) exit(3);

    char * filePath = malloc(sizeof(char) * (strlen(directory) + strlen(pDirent->d_name) + 3));
    strcpy(filePath, directory);
    strcat(filePath, pDirent->d_name);

    const char* type = getFileType(filePath);

    if (!strcmp(type, "regular") && isPNG(filePath)) {
      // Found an image, set flag and print image file path
      *containsPNG = true;
      printf("%s\n", filePath);
    } else if (!strcmp(type, "directory")) {
      if (strcmp(pDirent->d_name, ".") && strcmp(pDirent->d_name, "..")) {
        // At a folder, need to traverse a level deeper
        checkForPNG(filePath, containsPNG);
      }
    }

    free(filePath);
  }

  if (closedir(pDir) != 0 ) exit(3);

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Inproper command format. Proper Syntax: %s <directory name>\n", argv[0]);
    return 0;
  }

  bool containsPNG = false;
  checkForPNG(argv[1], &containsPNG);

  if (!containsPNG) printf("findpng: No PNG file found\n");

  return 0;
}
