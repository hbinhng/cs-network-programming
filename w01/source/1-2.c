/**
 * Nguyen Hai Binh (#20020189) - 1.2
 */
#include <stdio.h>
#include <string.h>

void clear(char *buf)
{
  memset(buf, 0, 100);
}

int main()
{
  char *buf = (char *)malloc(sizeof(char) * 100);

  printf("File name: ");
  scanf("%s", buf);
  printf("Enter anything you want to write to %s, ended by $\n", buf);

  FILE *file = fopen(buf, "w");
  clear(buf);

  int ended = -1, buflen;

  while (gets(buf) != NULL)
  {
    buflen = strlen(buf);

    if (buflen == 0)
      continue;

    for (int i = 0; i < buflen; i++)
      if (buf[i] == '$')
      {
        ended = i;
        break;
      }

    int actual_content_length = ended != -1 ? ended : buflen;
    char *flush_content = (char *)malloc(sizeof(char) * actual_content_length);
    strncpy(flush_content, buf, actual_content_length);

    fputs(flush_content, file);
    fputc('\n', file);

    if (ended != -1)
      break;

    clear(buf);
  }

  printf("\nAll done!\n");

  fclose(file);

  return 0;
}
