/**
 * Nguyen Hai Binh (#20020189) - 1.4
 */
#include <stdio.h>
#include <string.h>

const int MB = 1024 * 1024;
const int MAX_FILE_SIZE = 10 * MB;

int main()
{
  char *file_name = (char *)malloc(sizeof(char) * 100);
  char *file_content = (char *)malloc(sizeof(char) * MAX_FILE_SIZE);

  printf("File name: ");
  scanf("%s", file_name);

  FILE *file = fopen(file_name, "rb");
  sprintf(file_name, "%s.clone", file_name);
  FILE *w_file = fopen(file_name, "wb");

  fseek(file, 0L, SEEK_END);
  int file_size = ftell(file);
  fseek(file, 0L, SEEK_SET);

  fread(file_content, sizeof(char), file_size, file);

  int i = -1, cursor = 0;

  fwrite(file_content, sizeof(char), file_size, w_file);

  fclose(file);
  fclose(w_file);

  return 0;
}
