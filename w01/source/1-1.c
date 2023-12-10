/**
 * Nguyen Hai Binh (#20020189) - 1.1
 */
#include <stdio.h>
#include <string.h>

int main()
{
  char *buf;
  buf = (char *)malloc(sizeof(char) * 100);

  scanf("%s", buf);

  int character_count = 0,
      digit_count = 0,
      symbol_count = 0;

  int i = -1;

  while (buf[++i] != 0)
  {
    if (buf[i] >= 'a' && buf[i] <= 'z')
    {
      character_count++;
      continue;
    }

    if (buf[i] >= 'A' && buf[i] <= 'Z')
    {
      character_count++;
      continue;
    }

    if (buf[i] >= '0' && buf[i] <= '9')
    {
      digit_count++;
      continue;
    }

    symbol_count++;
  }

  printf("Characters count: %d\n", character_count);
  printf("Digits count: %d\n", digit_count);
  printf("Symbol count: %d\n", symbol_count);

  return 0;
}