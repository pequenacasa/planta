
// MIT License

// Copyright (c) 2024 Antônio Ivo da Silva Gomes

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PLANTA_FILEPATH "planta.txt"
#define MIDDLE_DELIMITER ';'
#define END_DELIMITER '\n'

enum { PASS_MODE = 0, BUILD_MODE = 1 };

typedef struct {
  char *title;
  char *filepath;
  unsigned char mode;
} PLANTA_FileInfo;

typedef struct {
  PLANTA_FileInfo *infos;
  unsigned int info_count;
} PLANTA_Context;

unsigned int CountLines(FILE *file) {
  unsigned int line_count = 0;
  char c;
  while (!feof(file)) {
    c = fgetc(file);
    if (c == '\n') {
      line_count++;
    }
  }

  rewind(file);
  return line_count;
}

char *ReadUntil(FILE *file, const char delimiter) {
  char text[100], c, *buffer;

  int index = 0;
  do {
    c = fgetc(file);
    text[index] = c;
    index++;
  } while (c != delimiter);
  text[--index] = '\0';

  buffer = malloc(index);
  strcpy(buffer, text);

  return buffer;
}

unsigned int ConvertStringToInt(char *string) {
  unsigned int mode;
  sscanf(string, "%d", &mode);
  return mode;
}

PLANTA_Context *PLANTA_CreateContext(const char *filepath) {
  PLANTA_Context *ctx = malloc(sizeof(PLANTA_Context));
  if (ctx == NULL) {
    return NULL;
  }

  FILE *fptr = fopen(filepath, "r");
  if (fptr == NULL) {
    free(ctx);
    return NULL;
  }

  int line_count = CountLines(fptr);

  ctx->infos = malloc(sizeof(PLANTA_FileInfo) * line_count);
  if (ctx->infos == NULL) {
    free(ctx);
    fclose(fptr);
    return NULL;
  }
  ctx->info_count = line_count;

  for (int i = 0; i < line_count; i++) {
    ctx->infos[i].title = ReadUntil(fptr, MIDDLE_DELIMITER);
    ctx->infos[i].filepath = ReadUntil(fptr, MIDDLE_DELIMITER);

    char *s = ReadUntil(fptr, END_DELIMITER);
    ctx->infos[i].mode = ConvertStringToInt(s);
    free(s);
  }

  fclose(fptr);
  return ctx;
}

void PLANTA_DestroyContext(PLANTA_Context *ctx) {
  for (int i = 0; i < ctx->info_count; i++) {
    free(ctx->infos[i].title);
    free(ctx->infos[i].filepath);
  }

  free(ctx->infos);
  free(ctx);
}

int main(int argc, char *argv[]) {
  PLANTA_Context *ctx = PLANTA_CreateContext(PLANTA_FILEPATH);
  if (ctx == NULL) {
    return 1;
  }

  PLANTA_DestroyContext(ctx);
  return 0;
}
