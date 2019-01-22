/*
 * Perhaps the fastest Levenshtein Edit Distance measure ever
 * Author: skywalker.deja@gmail.com
 */

#include <nan.h>

#define MAX_LEN 256
static int D[MAX_LEN * 4];

/*
 * UTF8 String decoder
 */
static int _utf8_dec(const char *src, int* dst) {
  int len = -1;
  unsigned char *p = (unsigned char*)(src);
  unsigned char c = *(p);
  while (c != 0) {
    if (c < 128 || c >= 192) {
      dst[++len] = (int)(c & 0x7F);
    } else {
      dst[len] = (dst[len] << 8) + (int)(c & 0x3F);
    }
    c = *(++p);
  }
  return len + 1;
}

static int _min(int a, int b, int c) {
  if (a < b) return ((a < c) ? a : c);
  return ((b < c) ? b : c);
}

/*
 * Levenshtein Edit Distance measure function
 */
NAN_METHOD(levenshtein) {
  if (info.Length() != 2) {
    return Nan::ThrowTypeError("two string args are required");
  }

  Nan::Utf8String v0(info[0]);
  Nan::Utf8String v1(info[1]);

  // convert to a std::string
  std::string str0(*v0, v0.length());
  std::string str1(*v1, v1.length());

  // convert to UTF-8 string
  int *s0 = D, *s1 = (D + 256);
  int len0 = _utf8_dec(str0.c_str(), s0);
  int len1 = _utf8_dec(str1.c_str(), s1);

  // Let s0 as smaller string
  if (len0 > len1) {
    int len = len0; len0 = len1; len1 = len;
    int *s = s0; s0 = s1; s1 = s;
  }

  // Initialize buffers
  int *d0 = (D + 512), *d1 = (D + 768);
  for (int i = 0; i <= len0; i++) { d0[i] = i; }
  // Calculation
  for (int i = 0; i < len1; i++) {
    int c = s1[i];
    d1[0] = i + 1;
    for (int j = 0; j < len0; j++) {
      d1[j+1] = (c == s0[j]) ? d0[j] : (_min(d1[j], d0[j], d0[j+1]) + 1);
    }
    // Swap buffers
    int *t = d0; d0 = d1; d1 = t;
  }
  int dist = d0[len0];
  info.GetReturnValue().Set(Nan::New<v8::Int32>(dist));
}

NAN_MODULE_INIT(Initialize) {
  NAN_EXPORT(target, levenshtein);
}

NODE_MODULE(distance, Initialize);
