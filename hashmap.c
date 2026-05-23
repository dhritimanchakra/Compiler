#include "chibicc.h"
#define INIT_SIZE 16
#define HIGH_WATERMARK 70
#define LOW_WATERMARK 50
#define TOMBSTONE ((void *)-1)

static uint64_t fnv_hash(char *s,int len){
    uint64_t hash = 0xcbf29ce484222325;
  for (int i = 0; i < len; i++) {
    hash *= 0x100000001b3;
    hash ^= (unsigned char)s[i];
  }
  return hash;
}