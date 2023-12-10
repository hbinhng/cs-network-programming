#include "common.hpp"

#include <string>

using namespace std;

#define BUFSIZE 1024

char buffer[BUFSIZE];

string ltrim(const string &s)
{
  int index = 0;

  while (isspace(s[index]))
    ++index;

  return s.substr(index);
}

string rtrim(const string &s)
{
  int index = s.length() - 1;

  while (isspace(s[index]))
    --index;

  return s.substr(0, index + 1);
}

string trim(const string &s)
{
  return ltrim(rtrim(s));
}