#include <stdlib.h>

#include "chunk.h"
#include "memory.h"


void initChunk(Chunk* chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  initValueArray(&chunk->constants);
  initValueArray(&chunk->longConstants);
}


void freeChunk(Chunk* chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
  initChunk(chunk);
  freeValueArray(&chunk->constants);
  freeValueArray(&chunk->longConstants);
}


void writeLongChunk(Chunk* chunk, uint32_t byte, int line) {
  uint32_t mask = 0x000000FF;
  uint32_t bite3 = mask & byte;
  byte = byte >> 8;
  uint32_t bite2 = mask & byte;
  byte = byte >> 8;
  uint32_t bite1 = mask & byte;
  writeChunk(chunk, bite3, line);
  writeChunk(chunk, bite2, line);
  writeChunk(chunk, bite1, line);
}


void writeConstant(Chunk* chunk, Value value, int line) {
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code,
        oldCapacity, chunk->capacity);
    chunk->lines = GROW_ARRAY(int, chunk->lines,
        oldCapacity, chunk->capacity);
  }

  chunk->code[chunk->count] = byte;
  chunk->lines[chunk->count] = line;
  chunk->count++;
}

int addConstant(Chunk* chunk, Value value) {
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}

int addLongConstant(Chunk* chunk, Value value) {
  writeValueArray(&chunk->longConstants, value);
  return chunk->longConstants.count - 1;
}