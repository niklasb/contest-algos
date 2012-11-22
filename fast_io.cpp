#include <cstring>
#include <iostream>
using namespace std;

// some damn fast I/O
#define GETCHAR getchar_unlocked
#define FWRITE fwrite_unlocked
const int BUFSIZE = 1<<16;

static char outbuf[BUFSIZE];
int outbuf_size = 0;

void init_io() {
  ios_base::sync_with_stdio(false);
}

void flush() {
  FWRITE(outbuf, outbuf_size, 1, stdout);
  outbuf_size = 0;
}

template <typename T>
bool read_num(T &x) {
  register int c = GETCHAR();
  x = 0;
  bool neg = false;
  while (!isdigit(c) && c != '-' && c != EOF)
    c = GETCHAR();
  if (c == EOF) return false;
  if (c == '-') { neg = true; c = GETCHAR(); }
  while (isdigit(c)) {
    x = x * 10 + c - '0';
    c = GETCHAR();
  }
  if (neg) x = -x;
  return true;
}

void write_char(char c) {
  if (outbuf_size == BUFSIZE)
    flush();
  outbuf[outbuf_size++] = c;
}

void write_str(const char *c) {
  while (*c) write_char(*(c++));
}

// because we only flush when trying to write, we always have
// at least one byte in the buffer after every write operation
bool unwrite() {
  if (!outbuf_size)
    return false;
  outbuf_size--;
  return true;
}

template <typename T>
void write_num(T x) {
  if (x < 0) { write_char('-'); x = -x; }
  static int digs[25];
  register int i = 0;
  while (x) {
    digs[i++] = x % 10; x /= 10;
  }
  if (i) {
    while (i) {
      write_char(digs[i-1] + '0'); --i;
    }
  } else write_char('0');
}
