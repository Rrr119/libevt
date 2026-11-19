/*
 * Value format functions
 *
 * Copyright (C) 2011-2026, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _VALUE_FORMAT_H )
#define _VALUE_FORMAT_H

#include <common.h>
#include <types.h>

#include "evttools_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

int evttools_value_format_allocate(
     const system_character_t *format_string,
     size_t format_string_length,
     const system_character_t *value_string,
     size_t value_string_length,
     system_character_t **formatted_string,
     size_t *formatted_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _VALUE_FORMAT_H ) */
