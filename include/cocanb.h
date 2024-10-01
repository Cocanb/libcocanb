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

#ifndef __COCANB_H__
#define __COCANB_H__

#define COCANB_NOERR 0
#define COCANB_ERR_ALLOC 1
#define COCANB_ERR_MISMATCHED_QUOTE 2
#define COCANB_ERR_QUOTE_OVERFLOW 3

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Encodes into Cocánb
 *
 * Encodes input text into Cocánb. Returns `COCANB_NOERR` if the text could be
 * encoded successfully, and another integer if the translation fails.
 *
 * @param[in]  text Text to encode
 * @param[out] dest Pointer to encode into. Has to be `free`d after use.
 * @param[in]  maxq Maximum allowed nested quotes.
 *
 * @returns `COCANB_NOERR` - translation was successful.
 *          `COCANB_ERR_ALLOC` - the translator could not allocate the required
 *          memory to encode the input.
 *          `COCANB_ERR_MISMATCHED_QUOTE` - the input has unclosed or otherwise
 *          mismatching quotes.
 *          `COCANB_ERR_QUOTE_OVERFLOW` - the input has more nested quotes than
 *          is allowed.
 */
int cocanb_encode(const char *text, char **dest, int maxq);

#ifdef __cplusplus
}
#endif

#endif
