/**
 * Nguyen Hai Binh (#20020189) - 1.3
 */
#include <stdio.h>
#include <string.h>

const int MB = 1024 * 1024;

int is_character(char c)
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ' || c == '\r' || c == '\n';
}

int main()
{
  char *file_name = (char *)malloc(sizeof(char) * 100);
  char *file_content = (char *)malloc(sizeof(char) * MB);
  char *file_content_character_only = (char *)malloc(sizeof(char) * MB);

  printf("File name: ");
  scanf("%s", file_name);

  FILE *file = fopen(file_name, "r");
  sprintf(file_name, "%s.co", file_name);
  FILE *w_file = fopen(file_name, "w");

  fread(file_content, sizeof(char), MB, file);

  int i = -1, cursor = 0;

  while (file_content[++i] != 0)
  {
    if (is_character(file_content[i]))
      file_content_character_only[cursor++] = file_content[i];
  }

  fputs(file_content_character_only, w_file);

  fclose(file);
  fclose(w_file);

  return 0;
}
