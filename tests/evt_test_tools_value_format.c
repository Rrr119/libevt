/*
 * Tools value_format type test program
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

#include <common.h>
#include <file_stream.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "evt_test_libcerror.h"
#include "evt_test_macros.h"
#include "evt_test_unused.h"

#include "../evttools/value_format.h"

/* Tests a value format case
 * Returns 1 if successful or 0 if not
 */
int evt_test_tools_value_format_case(
     const system_character_t *format_string,
     const system_character_t *value_string,
     const system_character_t *expected_string )
{
	libcerror_error_t *error     = NULL;
	system_character_t *string   = NULL;
	size_t expected_string_length = 0;
	size_t string_size           = 0;
	int result                   = 0;

	expected_string_length = system_string_length(
	                          expected_string );

	result = evttools_value_format_allocate(
	          format_string,
	          system_string_length(
	           format_string ),
	          value_string,
	          system_string_length(
	           value_string ),
	          &string,
	          &string_size,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "string",
	 string );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	EVT_TEST_ASSERT_EQUAL_SIZE(
	 "string_size",
	 string_size,
	 expected_string_length + 1 );

	if( system_string_compare(
	     string,
	     expected_string,
	     expected_string_length ) != 0 )
	{
		fprintf(
		 stdout,
		 "unexpected formatted string: %" PRIs_SYSTEM "\n",
		 string );

		goto on_error;
	}
	memory_free(
	 string );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( string != NULL )
	{
		memory_free(
		 string );
	}
	return( 0 );
}

/* Tests the value format support
 * Returns 1 if successful or 0 if not
 */
int evt_test_tools_value_format(
     void )
{
	libcerror_error_t *error   = NULL;
	system_character_t *string = NULL;
	size_t string_size         = 0;
	int result                 = 0;

	EVT_TEST_RUN_WITH_ARGS(
	 "value_format: string",
	 evt_test_tools_value_format_case,
	 _SYSTEM_STRING( "" ),
	 _SYSTEM_STRING( "alpha" ),
	 _SYSTEM_STRING( "alpha" ) );

	EVT_TEST_RUN_WITH_ARGS(
	 "value_format: signed decimal",
	 evt_test_tools_value_format_case,
	 _SYSTEM_STRING( "d" ),
	 _SYSTEM_STRING( "-42" ),
	 _SYSTEM_STRING( "-42" ) );

	EVT_TEST_RUN_WITH_ARGS(
	 "value_format: I64 decimal",
	 evt_test_tools_value_format_case,
	 _SYSTEM_STRING( "I64d" ),
	 _SYSTEM_STRING( "1234567890123" ),
	 _SYSTEM_STRING( "1234567890123" ) );

	EVT_TEST_RUN_WITH_ARGS(
	 "value_format: zero padded hex",
	 evt_test_tools_value_format_case,
	 _SYSTEM_STRING( "#08x" ),
	 _SYSTEM_STRING( "255" ),
	 _SYSTEM_STRING( "0x0000ff" ) );

	EVT_TEST_RUN_WITH_ARGS(
	 "value_format: uppercase hex",
	 evt_test_tools_value_format_case,
	 _SYSTEM_STRING( "X" ),
	 _SYSTEM_STRING( "48879" ),
	 _SYSTEM_STRING( "BEEF" ) );

	EVT_TEST_RUN_WITH_ARGS(
	 "value_format: hexadecimal input",
	 evt_test_tools_value_format_case,
	 _SYSTEM_STRING( "08x" ),
	 _SYSTEM_STRING( "0x1f" ),
	 _SYSTEM_STRING( "0000001f" ) );

	EVT_TEST_RUN_WITH_ARGS(
	 "value_format: octal",
	 evt_test_tools_value_format_case,
	 _SYSTEM_STRING( "#o" ),
	 _SYSTEM_STRING( "8" ),
	 _SYSTEM_STRING( "010" ) );

	EVT_TEST_RUN_WITH_ARGS(
	 "value_format: unsigned decimal",
	 evt_test_tools_value_format_case,
	 _SYSTEM_STRING( "u" ),
	 _SYSTEM_STRING( "-1" ),
	 _SYSTEM_STRING( "4294967295" ) );

	EVT_TEST_RUN_WITH_ARGS(
	 "value_format: 64-bit unsigned decimal",
	 evt_test_tools_value_format_case,
	 _SYSTEM_STRING( "I64u" ),
	 _SYSTEM_STRING( "-1" ),
	 _SYSTEM_STRING( "18446744073709551615" ) );

	EVT_TEST_RUN_WITH_ARGS(
	 "value_format: precision",
	 evt_test_tools_value_format_case,
	 _SYSTEM_STRING( ".3s" ),
	 _SYSTEM_STRING( "alpha" ),
	 _SYSTEM_STRING( "alp" ) );

	EVT_TEST_RUN_WITH_ARGS(
	 "value_format: left aligned",
	 evt_test_tools_value_format_case,
	 _SYSTEM_STRING( "-8s" ),
	 _SYSTEM_STRING( "alpha" ),
	 _SYSTEM_STRING( "alpha   " ) );

	EVT_TEST_RUN_WITH_ARGS(
	 "value_format: integer precision",
	 evt_test_tools_value_format_case,
	 _SYSTEM_STRING( "04d" ),
	 _SYSTEM_STRING( "7" ),
	 _SYSTEM_STRING( "0007" ) );

	EVT_TEST_RUN_WITH_ARGS(
	 "value_format: zero precision",
	 evt_test_tools_value_format_case,
	 _SYSTEM_STRING( ".0d" ),
	 _SYSTEM_STRING( "0" ),
	 _SYSTEM_STRING( "" ) );

	EVT_TEST_RUN_WITH_ARGS(
	 "value_format: plus sign",
	 evt_test_tools_value_format_case,
	 _SYSTEM_STRING( "+d" ),
	 _SYSTEM_STRING( "7" ),
	 _SYSTEM_STRING( "+7" ) );

	EVT_TEST_RUN_WITH_ARGS(
	 "value_format: integer width and precision",
	 evt_test_tools_value_format_case,
	 _SYSTEM_STRING( "10.5d" ),
	 _SYSTEM_STRING( "42" ),
	 _SYSTEM_STRING( "     00042" ) );

	EVT_TEST_RUN_WITH_ARGS(
	 "value_format: character",
	 evt_test_tools_value_format_case,
	 _SYSTEM_STRING( "c" ),
	 _SYSTEM_STRING( "a" ),
	 _SYSTEM_STRING( "a" ) );

	EVT_TEST_RUN_WITH_ARGS(
	 "value_format: float",
	 evt_test_tools_value_format_case,
	 _SYSTEM_STRING( ".2f" ),
	 _SYSTEM_STRING( "3.14159" ),
	 _SYSTEM_STRING( "3.14" ) );

	EVT_TEST_RUN_WITH_ARGS(
	 "value_format: unsupported format",
	 evt_test_tools_value_format_case,
	 _SYSTEM_STRING( "q" ),
	 _SYSTEM_STRING( "raw" ),
	 _SYSTEM_STRING( "raw" ) );

	result = evttools_value_format_allocate(
	          NULL,
	          0,
	          _SYSTEM_STRING( "value" ),
	          5,
	          &string,
	          &string_size,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = evttools_value_format_allocate(
	          _SYSTEM_STRING( "s" ),
	          1,
	          NULL,
	          0,
	          &string,
	          &string_size,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	string = (system_character_t *) 0x12345678UL;

	result = evttools_value_format_allocate(
	          _SYSTEM_STRING( "s" ),
	          1,
	          _SYSTEM_STRING( "value" ),
	          5,
	          &string,
	          &string_size,
	          &error );

	string = NULL;

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = evttools_value_format_allocate(
	          _SYSTEM_STRING( "s" ),
	          1,
	          _SYSTEM_STRING( "value" ),
	          5,
	          &string,
	          NULL,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( string != NULL )
	{
		memory_free(
		 string );
	}
	return( 0 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc EVT_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] EVT_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc EVT_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] EVT_TEST_ATTRIBUTE_UNUSED )
#endif
{
	EVT_TEST_UNREFERENCED_PARAMETER( argc )
	EVT_TEST_UNREFERENCED_PARAMETER( argv )

	EVT_TEST_RUN(
	 "evttools_value_format_allocate",
	 evt_test_tools_value_format );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}
