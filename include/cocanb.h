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

#define COCANB_NOERR 0;
#define COCANB_ERR 1;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Encodes input text into Cocánb. Returns `COCANB_NOERR` if the text could be
 * encoded successfully, and `COCANB_ERR` otherwise.
 *
 * @param text Text to encode
 * @param dest Pointer to encode into. Has to be `free`d after use.
 * @param maxq Maximum allowed nested quotes.
 *
 * @returns `COCANB_NOERR` (0) if successful, `COCANB_ERR` (1) if not
 * successful
 */
int cocanb_encode(const char *__text, char **__dest, int __maxq);

#ifdef __cplusplus
}
#endif

#endif
