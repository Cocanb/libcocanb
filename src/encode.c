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

int __cocanb_append_word(
  char **suf_top, char **word, int *word_len, char **dest)
{
  if (*word_len != 0) {
    int counted_wl = 0;
    int npos = -1;
    for (int i = 0; i < *word_len; i++) {
      if (npos == -1) {
        switch ((*word)[i]) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '.':
        case ',':
          npos = i;
          break;
        default:
          break;
        }
        ++counted_wl;
      } else {
        switch ((*word)[i]) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '.':
        case ',':
          break;
        default:
          npos = -1;
          ++counted_wl;
          break;
        }
      }
    }

    int quotient = counted_wl / 26;
    int remainder = counted_wl % 26;
    int nlen;
    if (npos == -1) {
      nlen = 1;
    } else {
      nlen = *word_len - npos;
    }
    int suf_len = strlen(*suf_top);
    *suf_top
      = realloc(*suf_top, (suf_len + nlen + quotient + 2) * sizeof(char));
    if (*suf_top == NULL) {
      free(*dest);
      free(*word);
      free(*suf_top);
      return 1;
    }

    for (int j = 0; j < nlen; j++) {
      (*suf_top)[suf_len + j] = tolower((*word)[*word_len - nlen + j]);
    }
    for (int j = 0; j < quotient; j++) {
      (*suf_top)[suf_len + nlen + j] = '@';
    }
    (*suf_top)[suf_len + nlen + quotient] = 96 + remainder;
    (*suf_top)[suf_len + nlen + quotient + 1] = 0;

    char temp[strlen(*dest) + *word_len];
    strcpy(temp, *dest);
    strncat(temp, *word, *word_len - nlen);
    free(*dest);
    *dest = calloc(strlen(*dest) + *word_len, sizeof(char));
    if (*dest == NULL) {
      free(*word);
      free(*suf_top);
      return 1;
    }
    strcpy(*dest, temp);

    free(*word);
    *word = calloc(1, sizeof(char));
    if (*word == NULL) {
      free(*dest);
      free(*suf_top);
      return 1;
    }
    *word_len = 0;
  }
  return 0;
}

int cocanb_encode(const char *text, char **dest, int maxq)
{
  *dest = calloc(1, sizeof(char));
  if (*dest == NULL) {
    return 1;
  }

  char *word = calloc(1, sizeof(char));
  if (word == NULL) {
    free(*dest);
    return 1;
  }
  int word_len = 0;

  char *suf_stack[maxq + 1];
  int suf_ptr = 0;

  suf_stack[0] = calloc(4, sizeof(char));
  if (suf_stack[0] == NULL) {
    free(*dest);
    free(word);
    return 1;
  }
  strcpy(suf_stack[0], "non");

  char qt_stack[maxq];
  int qt_ptr = -1;

  for (int i = 0; i < strlen(text); i++) {
    if (qt_ptr != -1 && qt_stack[qt_ptr] == '<') {
      int len = strlen(*dest);
      *dest = realloc(*dest, (len + 1) * sizeof(char));
      (*dest)[len] = text[i];
      (*dest)[len + 1] = 0;
      if (text[i] == '>') {
        --qt_ptr;
      }
      continue;
    }

    switch (text[i]) {
    case ' ':
    case '\f':
    case '\n':
    case '\r':
    case '\t':
    case '\v':

      if (__cocanb_append_word(&suf_stack[suf_ptr], &word, &word_len, dest)) {
        while (--suf_ptr != -1) {
          free(suf_stack[suf_ptr]);
        }
        return 1;
      }

      break;
    case '.':
      if (isdigit(text[i + 1])) {
        word = realloc(word, (strlen(word) + 2) * sizeof(char));
        if (word == NULL) {
          free(*dest);
          while (suf_ptr != -1) {
            free(suf_stack[suf_ptr--]);
          }
          return 1;
        }
        word[word_len] = tolower(text[i]);
        word[++word_len] = 0;
        break;
      }
    case '?':
    case '!':
      if (__cocanb_append_word(&suf_stack[suf_ptr], &word, &word_len, dest)) {
        while (--suf_ptr != -1) {
          free(suf_stack[suf_ptr]);
        }
        return 1;
      }
      {

        int suf_len = strlen(suf_stack[suf_ptr]);
        char temp[strlen(*dest) + suf_len + 1];
        int temp_len = strlen(*dest) + suf_len;
        strcpy(temp, *dest);
        strcat(temp, suf_stack[suf_ptr]);
        free(*dest);
        free(suf_stack[suf_ptr--]);
        *dest = calloc(temp_len + 2, sizeof(char));
        if (*dest == NULL) {
          free(word);
          while (suf_ptr != -1) {
            free(suf_stack[suf_ptr--]);
          }
          return 1;
        }
        strcpy(*dest, temp);
        (*dest)[temp_len] = text[i];

        free(word);
        word = calloc(1, sizeof(char));
        if (word == NULL) {
          free(*dest);
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
      if (__cocanb_append_word(&suf_stack[suf_ptr], &word, &word_len, dest)) {
        while (--suf_ptr != -1) {
          free(suf_stack[suf_ptr]);
        }
        return 1;
      }

      if (qt_ptr == -1 || qt_stack[qt_ptr] != text[i]) {
        if (qt_ptr + 1 >= maxq) {
          free(*dest);
          free(word);
          while (suf_ptr != -1) {
            free(suf_stack[suf_ptr--]);
          }
          return 3;
        }
        qt_stack[++qt_ptr] = text[i];
        suf_stack[++suf_ptr] = calloc(4, sizeof(char));
        if (suf_stack[suf_ptr] == NULL) {
          free(*dest);
          free(word);
          while (--suf_ptr != -1) {
            free(suf_stack[suf_ptr]);
          }
          return 1;
        }
        strcpy(suf_stack[suf_ptr], "non");
      } else {
        if (suf_ptr != qt_ptr--) {
          char temp[strlen(*dest) + strlen(suf_stack[suf_ptr]) + 1];
          int temp_len = strlen(*dest) + strlen(suf_stack[suf_ptr]);
          strcpy(temp, *dest);
          strcat(temp, suf_stack[suf_ptr]);
          free(*dest);
          free(suf_stack[suf_ptr--]);
          *dest = calloc(temp_len + 1, sizeof(char));
          if (*dest == NULL) {
            free(word);
            while (suf_ptr != -1) {
              free(suf_stack[suf_ptr--]);
            }
            return 1;
          }
          strcpy(*dest, temp);
        }
      }
      {
        int len = strlen(*dest);
        *dest = realloc(*dest, (len + 1) * sizeof(char));
        if (*dest == NULL) {
          free(word);
          while (suf_ptr-- != -1) {
            free(suf_stack[suf_ptr]);
          }
          return 1;
        }
        (*dest)[len] = text[i];
        (*dest)[len + 1] = 0;
      }
      break;
    case '(':
      if (__cocanb_append_word(&suf_stack[suf_ptr], &word, &word_len, dest)) {
        while (--suf_ptr != -1) {
          free(suf_stack[suf_ptr]);
        }
        return 1;
      }
      if (qt_ptr + 1 >= maxq) {
        free(*dest);
        free(word);
        while (suf_ptr != -1) {
          free(suf_stack[suf_ptr--]);
        }
        return 3;
      }
      {
        qt_stack[++qt_ptr] = text[i];
        suf_stack[++suf_ptr] = calloc(4, sizeof(char));
        strcpy(suf_stack[suf_ptr], "non");
        int len = strlen(*dest);
        *dest = realloc(*dest, (len + 2) * sizeof(char));
        (*dest)[len] = text[i];
        (*dest)[len + 1] = 0;
      }
      break;
    case ')':
      if (__cocanb_append_word(&suf_stack[suf_ptr], &word, &word_len, dest)) {
        while (--suf_ptr != -1) {
          free(suf_stack[suf_ptr]);
        }
        return 1;
      }

      if (qt_ptr != -1 && qt_stack[qt_ptr] == '(') {
        if (suf_ptr != qt_ptr--) {
          char temp[strlen(*dest) + strlen(suf_stack[suf_ptr]) + 1];
          int temp_len = strlen(*dest) + strlen(suf_stack[suf_ptr]);
          strcpy(temp, *dest);
          strcat(temp, suf_stack[suf_ptr]);
          free(*dest);
          free(suf_stack[suf_ptr--]);
          *dest = calloc(temp_len + 1, sizeof(char));
          if (*dest == NULL) {
            free(word);
            while (suf_ptr != -1) {
              free(suf_stack[suf_ptr--]);
            }
            return 1;
          }
          strcpy(*dest, temp);
        }
        int len = strlen(*dest);
        *dest = realloc(*dest, (len + 2) * sizeof(char));
        (*dest)[len] = text[i];
        (*dest)[len + 1] = 0;
      } else {
        free(word);
        free(dest);
        while (suf_ptr != -1) {
          free(suf_stack[suf_ptr--]);
        }
        return 2;
      }
      break;
    case '[':
      if (__cocanb_append_word(&suf_stack[suf_ptr], &word, &word_len, dest)) {
        while (--suf_ptr != -1) {
          free(suf_stack[suf_ptr]);
        }
        return 1;
      }

      if (qt_ptr + 1 >= maxq) {
        free(*dest);
        free(word);
        while (suf_ptr != -1) {
          free(suf_stack[suf_ptr--]);
        }
        return 3;
      }
      {
        qt_stack[++qt_ptr] = text[i];
        suf_stack[++suf_ptr] = calloc(4, sizeof(char));
        strcpy(suf_stack[suf_ptr], "non");
        int len = strlen(*dest);
        *dest = realloc(*dest, (len + 2) * sizeof(char));
        (*dest)[len] = text[i];
        (*dest)[len + 1] = 0;
      }
      break;
    case ']':
      if (__cocanb_append_word(&suf_stack[suf_ptr], &word, &word_len, dest)) {
        while (--suf_ptr != -1) {
          free(suf_stack[suf_ptr]);
        }
        return 1;
      }

      if (qt_ptr != -1 && qt_stack[qt_ptr] == '[') {
        if (suf_ptr != qt_ptr--) {
          char temp[strlen(*dest) + strlen(suf_stack[suf_ptr]) + 1];
          int temp_len = strlen(*dest) + strlen(suf_stack[suf_ptr]);
          strcpy(temp, *dest);
          strcat(temp, suf_stack[suf_ptr]);
          free(*dest);
          free(suf_stack[suf_ptr--]);
          *dest = calloc(temp_len + 1, sizeof(char));
          if (*dest == NULL) {
            free(word);
            while (suf_ptr != -1) {
              free(suf_stack[suf_ptr--]);
            }
            return 1;
          }
          strcpy(*dest, temp);
        }
        int len = strlen(*dest);
        *dest = realloc(*dest, (len + 2) * sizeof(char));
        (*dest)[len] = text[i];
        (*dest)[len + 1] = 0;
      } else {
        free(word);
        free(dest);
        while (suf_ptr != -1) {
          free(suf_stack[suf_ptr--]);
        }
        return 2;
      }
      break;
    case '{':
      if (__cocanb_append_word(&suf_stack[suf_ptr], &word, &word_len, dest)) {
        while (--suf_ptr != -1) {
          free(suf_stack[suf_ptr]);
        }
        return 1;
      }
      if (qt_ptr + 1 >= maxq) {
        free(*dest);
        free(word);
        while (suf_ptr != -1) {
          free(suf_stack[suf_ptr--]);
        }
        return 3;
      }
      {
        qt_stack[++qt_ptr] = text[i];
        suf_stack[++suf_ptr] = calloc(4, sizeof(char));
        strcpy(suf_stack[suf_ptr], "non");
        int len = strlen(*dest);
        *dest = realloc(*dest, (len + 2) * sizeof(char));
        (*dest)[len] = text[i];
        (*dest)[len + 1] = 0;
      }
      break;
    case '}':
      if (__cocanb_append_word(&suf_stack[suf_ptr], &word, &word_len, dest)) {
        while (--suf_ptr != -1) {
          free(suf_stack[suf_ptr]);
        }
        return 1;
      }

      if (qt_ptr != -1 && qt_stack[qt_ptr] == '{') {
        if (suf_ptr != qt_ptr--) {
          char temp[strlen(*dest) + strlen(suf_stack[suf_ptr]) + 1];
          int temp_len = strlen(*dest) + strlen(suf_stack[suf_ptr]);
          strcpy(temp, *dest);
          strcat(temp, suf_stack[suf_ptr]);
          free(*dest);
          free(suf_stack[suf_ptr--]);
          *dest = calloc(temp_len + 1, sizeof(char));
          if (*dest == NULL) {
            free(word);
            while (suf_ptr != -1) {
              free(suf_stack[suf_ptr--]);
            }
            return 1;
          }
          strcpy(*dest, temp);
        }
        int len = strlen(*dest);
        *dest = realloc(*dest, (len + 2) * sizeof(char));
        (*dest)[len] = text[i];
        (*dest)[len + 1] = 0;
      } else {
        free(word);
        free(dest);
        while (suf_ptr != -1) {
          free(suf_stack[suf_ptr--]);
        }
        return 2;
      }
      break;
    case '<':
      if (__cocanb_append_word(&suf_stack[suf_ptr], &word, &word_len, dest)) {
        while (--suf_ptr != -1) {
          free(suf_stack[suf_ptr]);
        }
        return 1;
      }

      if (qt_ptr + 1 >= maxq) {
        free(*dest);
        free(word);
        while (suf_ptr != -1) {
          free(suf_stack[suf_ptr--]);
        }
        return 3;
      }
      {
        qt_stack[++qt_ptr] = text[i];
        int len = strlen(*dest);
        *dest = realloc(*dest, (len + 2) * sizeof(char));
        (*dest)[len] = '<';
        (*dest)[len + 1] = 0;
      }
      break;
    case ',':
      if (isdigit(text[i + 1])) {
        word = realloc(word, (strlen(word) + 2) * sizeof(char));
        if (word == NULL) {
          free(*dest);
          while (suf_ptr != -1) {
            free(suf_stack[suf_ptr--]);
          }
          return 1;
        }
        word[word_len] = tolower(text[i]);
        word[++word_len] = 0;
        break;
      }
    case ':':
    case ';':
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      if (word_len == 0) {
        int len = strlen(*dest);
        *dest = realloc(*dest, (len + 2) * sizeof(char));
        (*dest)[len] = text[i];
        (*dest)[len + 1] = 0;
        break;
      }
    default:
      if (suf_ptr == qt_ptr) {
        suf_stack[++suf_ptr] = calloc(4, sizeof(char));
        if (suf_stack[suf_ptr] == NULL) {
          free(*dest);
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
        free(*dest);
        while (suf_ptr != -1) {
          free(suf_stack[suf_ptr--]);
        }
        return 1;
      }
      word[word_len] = tolower(text[i]);
      word[++word_len] = 0;
      break;
    }
  }

  if (__cocanb_append_word(&suf_stack[suf_ptr], &word, &word_len, dest)) {
    while (--suf_ptr != -1) {
      free(suf_stack[suf_ptr]);
    }
    return 1;
  }

  if (suf_ptr != -1) {
    if (qt_ptr != -1) {
      free(*dest);
      free(word);
      while (suf_ptr != -1) {
        free(suf_stack[suf_ptr--]);
      }
      return 2;
    }
    char temp[strlen(*dest) + strlen(suf_stack[suf_ptr])];
    int temp_len = strlen(*dest) + strlen(suf_stack[suf_ptr]);
    strcpy(temp, *dest);
    strcat(temp, suf_stack[suf_ptr]);
    free(*dest);
    free(suf_stack[suf_ptr--]);
    *dest = calloc(temp_len + 1, sizeof(char));
    if (*dest == NULL) {
      free(word);
      while (suf_ptr != -1) {
        free(suf_stack[suf_ptr--]);
      }
      return 1;
    }
    strcpy(*dest, temp);
  }

  free(word);

  return 0;
}

void cocanb_free(char *text) { free(text); }
