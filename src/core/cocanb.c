/**
 * libcocanb - Encodes texts into Cocánb
 * Copyright (C) 2024 salade malade
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "cocanb.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int cocanb_encode(const char *__text, char **__dest, int __maxq)
{
  *__dest = calloc(1, sizeof(char));
  if (*__dest == NULL) {
    return 1;
  }

  char *word = calloc(1, sizeof(char));
  if (word == NULL) {
    free(*__dest);
    return 1;
  }
  int word_len = 0;

  char *suf_stack[__maxq + 1];
  int suf_ptr = 0;

  suf_stack[0] = calloc(4, sizeof(char));
  if (suf_stack[0] == NULL) {
    free(*__dest);
    free(word);
    return 1;
  }
  strcpy(suf_stack[0], "non");

  char qt_stack[__maxq];
  int qt_ptr = -1;

  for (int i = 0; i < strlen(__text); i++) {
    switch (__text[i]) {
    case ' ':
    case '\f':
    case '\n':
    case '\r':
    case '\t':
    case '\v':
      if (word_len != 0) {
        int quotient = word_len / 26;
        int remainder = word_len % 26;
        int suf_len = strlen(suf_stack[suf_ptr]);
        suf_stack[suf_ptr] = realloc(suf_stack[suf_ptr], (suf_len + quotient + 3) * sizeof(char));
        if (suf_stack[suf_ptr] == NULL) {
          free(__dest);
          free(word);
          while (--suf_ptr != -1) {
            free(suf_stack[suf_ptr]);
          }
          return 1;
        }
        suf_stack[suf_ptr][suf_len] = tolower(word[word_len - 1]);
        for (int j = 0; j < quotient; j++) {
          suf_stack[suf_ptr][suf_len + j + 1] = '@';
        }
        suf_stack[suf_ptr][suf_len + quotient + 1] = 96 + remainder;
        suf_stack[suf_ptr][suf_len + quotient + 2] = 0;

        char temp[strlen(*__dest) + word_len];
        strcpy(temp, *__dest);
        strncat(temp, word, word_len - 1);
        free(*__dest);
        *__dest = calloc(strlen(*__dest) + word_len, sizeof(char));
        if (*__dest == NULL) {
          free(word);
          while (suf_ptr != -1) {
            free(suf_stack[suf_ptr--]);
          }
          return 1;
        }
        strcpy(*__dest, temp);

        free(word);
        word = calloc(1, sizeof(char));
        if (word == NULL) {
          free(*__dest);
          while (suf_ptr != -1) {
            free(suf_stack[suf_ptr--]);
          }
          return 1;
        }
        word_len = 0;
      }
      break;
    case '.':
    case '?':
    case '!':
      if (word_len != 0) {
        int quotient = word_len / 26;
        int remainder = word_len % 26;
        int suf_len = strlen(suf_stack[suf_ptr]);
        suf_stack[suf_ptr] = realloc(suf_stack[suf_ptr], (suf_len + quotient + 3) * sizeof(char));
        if (suf_stack[suf_ptr] == NULL) {
          free(*__dest);
          free(word);
          while (--suf_ptr != -1) {
            free(suf_stack[suf_ptr]);
          }
          return 1;
        }
        suf_stack[suf_ptr][suf_len] = tolower(word[word_len - 1]);
        for (int j = 0; j < quotient; j++) {
          suf_stack[suf_ptr][suf_len + j + 1] = '@';
        }
        suf_stack[suf_ptr][suf_len + 1] = 96 + remainder;
        suf_stack[suf_ptr][suf_len + 2] = 0;

        char temp[strlen(*__dest) + word_len + suf_len + quotient + 2];
        int temp_len = strlen(*__dest) + word_len + suf_len + quotient + 1;
        strcpy(temp, *__dest);
        strncat(temp, word, word_len - 1);
        strcat(temp, suf_stack[suf_ptr]);
        free(*__dest);
        free(suf_stack[suf_ptr--]);
        *__dest = calloc(temp_len + 2, sizeof(char));
        if (*__dest == NULL) {
          free(word);
          while (suf_ptr != -1) {
            free(suf_stack[suf_ptr--]);
          }
          return 1;
        }
        strcpy(*__dest, temp);
        (*__dest)[temp_len] = __text[i];

        free(word);
        word = calloc(1, sizeof(char));
        if (word == NULL) {
          free(*__dest);
          while (suf_ptr != -1) {
            free(suf_stack[suf_ptr--]);
          }
          return 1;
        }
        word_len = 0;
      } else {
        int suf_len = strlen(suf_stack[suf_ptr]);
        char temp[strlen(*__dest) + suf_len + 1];
        int temp_len = strlen(*__dest) + suf_len;
        strcpy(temp, *__dest);
        strcat(temp, suf_stack[suf_ptr]);
        free(*__dest);
        free(suf_stack[suf_ptr--]);
        *__dest = calloc(temp_len + 2, sizeof(char));
        if (*__dest == NULL) {
          free(word);
          while (suf_ptr != -1) {
            free(suf_stack[suf_ptr--]);
          }
          return 1;
        }
        strcpy(*__dest, temp);
        (*__dest)[temp_len] = __text[i];

        free(word);
        word = calloc(1, sizeof(char));
        if (word == NULL) {
          free(*__dest);
          while (suf_ptr != -1) {
            free(suf_stack[suf_ptr--]);
          }
          return 1;
        }
        word_len = 0;
      }
      break;
    case '\"':
    case '\'':
      if (word_len != 0) {
        int quotient = word_len / 26;
        int remainder = word_len % 26;
        int suf_len = strlen(suf_stack[suf_ptr]);
        suf_stack[suf_ptr] = realloc(suf_stack[suf_ptr], (suf_len + quotient + 3) * sizeof(char));
        if (suf_stack[suf_ptr] == NULL) {
          free(*__dest);
          free(word);
          while (--suf_ptr != -1) {
            free(suf_stack[suf_ptr]);
          }
          return 1;
        }
        suf_stack[suf_ptr][suf_len] = tolower(word[word_len - 1]);
        for (int j = 0; j < quotient; j++) {
          suf_stack[suf_ptr][suf_len + j + 1] = '@';
        }
        suf_stack[suf_ptr][suf_len + quotient + 1] = 96 + remainder;
        suf_stack[suf_ptr][suf_len + quotient + 2] = 0;

        char temp[strlen(*__dest) + word_len];
        strcpy(temp, *__dest);
        strncat(temp, word, word_len - 1);
        free(*__dest);
        *__dest = calloc(strlen(*__dest) + word_len, sizeof(char));
        if (*__dest == NULL) {
          return 1;
        }
        strcpy(*__dest, temp);

        free(word);
        word = calloc(1, sizeof(char));
        if (word == NULL) {
          free(*__dest);
          while (suf_ptr != -1) {
            free(suf_stack[suf_ptr--]);
          }
          return 1;
        }
        word_len = 0;
      }
      if (qt_ptr == -1 || qt_stack[qt_ptr] != __text[i]) {
        if (qt_ptr + 1 == __maxq) {
          free(*__dest);
          free(word);
          while (suf_ptr != -1) {
            free(suf_stack[suf_ptr--]);
          }
          return 3;
        }
        qt_stack[++qt_ptr] = __text[i];
        suf_stack[++suf_ptr] = calloc(4, sizeof(char));
        if (suf_stack[suf_ptr] == NULL) {
          free(*__dest);
          free(word);
          while (--suf_ptr != -1) {
            free(suf_stack[suf_ptr]);
          }
          return 1;
        }
        strcpy(suf_stack[suf_ptr], "non");
      } else {
        if (suf_ptr != qt_ptr--) {
          char temp[strlen(*__dest) + strlen(suf_stack[suf_ptr]) + 1];
          int temp_len = strlen(*__dest) + strlen(suf_stack[suf_ptr]);
          strcpy(temp, *__dest);
          strcat(temp, suf_stack[suf_ptr]);
          free(*__dest);
          free(suf_stack[suf_ptr--]);
          *__dest = calloc(temp_len + 1, sizeof(char));
          if (*__dest == NULL) {
            free(word);
            while (suf_ptr != -1) {
              free(suf_stack[suf_ptr--]);
            }
            return 1;
          }
          strcpy(*__dest, temp);
        }
      }
      {
        int len = strlen(*__dest);
        *__dest = realloc(*__dest, (len + 1) * sizeof(char));
        if (*__dest == NULL) {
          free(word);
          while (suf_ptr-- != -1) {
            free(suf_stack[suf_ptr]);
          }
          return 1;
        }
        (*__dest)[len] = __text[i];
        (*__dest)[len + 1] = 0;
      }
      break;
    case ',':
    case ':':
    case ';':
      break;
    default:
      if (suf_ptr == qt_ptr) {
        suf_stack[++suf_ptr] = calloc(4, sizeof(char));
        if (suf_stack[suf_ptr] == NULL) {
          free(*__dest);
          free(word);
          while (--suf_ptr != -1) {
            free(suf_stack[suf_ptr]);
          }
          return 1;
        }
        strcpy(suf_stack[suf_ptr], "non");
      }
      word = realloc(word, (strlen(word) + 2) * sizeof(char));
      if (word == NULL) {
        free(*__dest);
        while (suf_ptr != -1) {
          free(suf_stack[suf_ptr--]);
        }
        return 1;
      }
      word[word_len] = tolower(__text[i]);
      word[++word_len] = 0;
      break;
    }
  }

  if (word_len != 0) {
    int quotient = word_len / 26;
    int remainder = word_len % 26;
    int suf_len = strlen(suf_stack[suf_ptr]);
    suf_stack[suf_ptr] = realloc(suf_stack[suf_ptr], (suf_len + quotient + 3) * sizeof(char));
    if (suf_stack[suf_ptr] == NULL) {
      free(*__dest);
      free(word);
      while (--suf_ptr != -1) {
        free(suf_stack[suf_ptr]);
      }
      return 1;
    }
    suf_stack[suf_ptr][suf_len] = tolower(word[word_len - 1]);
    for (int j = 0; j < quotient; j++) {
      suf_stack[suf_ptr][suf_len + j + 1] = '@';
    }
    suf_stack[suf_ptr][suf_len + 1] = 96 + remainder;
    suf_stack[suf_ptr][suf_len + 2] = 0;

    char temp[strlen(*__dest) + word_len + suf_len + quotient + 2];
    int temp_len = strlen(*__dest) + word_len + suf_len + quotient + 1;
    strcpy(temp, *__dest);
    strncat(temp, word, word_len - 1);
    strcat(temp, suf_stack[suf_ptr]);
    free(*__dest);
    free(suf_stack[suf_ptr--]);
    *__dest = calloc(temp_len + 1, sizeof(char));
    if (*__dest == NULL) {
      free(*__dest);
      free(word);
      while (suf_ptr != -1) {
        free(suf_stack[suf_ptr--]);
      }
      return 1;
    }
    strcpy(*__dest, temp);
  }

  if (suf_ptr != -1) {
    if (qt_ptr != -1) {
      free(*__dest);
      free(word);
      while (suf_ptr != -1) {
        free(suf_stack[suf_ptr--]);
      }
      return 2;
    }
    char temp[strlen(*__dest) + strlen(suf_stack[suf_ptr])];
    int temp_len = strlen(*__dest) + strlen(suf_stack[suf_ptr]);
    strcpy(temp, *__dest);
    strcat(temp, suf_stack[suf_ptr]);
    free(*__dest);
    free(suf_stack[suf_ptr--]);
    *__dest = calloc(temp_len + 1, sizeof(char));
    if (*__dest == NULL) {
      free(word);
      while (suf_ptr != -1) {
        free(suf_stack[suf_ptr--]);
      }
      return 1;
    }
    strcpy(*__dest, temp);
  }

  free(word);

  return 0;
}
