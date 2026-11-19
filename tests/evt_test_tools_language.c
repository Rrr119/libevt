/*
 * Tools language type test program
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "evt_test_macros.h"
#include "evt_test_unused.h"

#include "../evttools/language.h"

/* Tests the language match score function
 * Returns 1 if successful or 0 if not
 */
int evt_test_tools_language_get_match_score(
     void )
{
	EVT_TEST_ASSERT_EQUAL_INT(
	 "exact match",
	 evttools_language_get_match_score( 0x00000409UL, 0x00000409UL ),
	 3 );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "language match",
	 evttools_language_get_match_score( 0x00000809UL, 0x00000409UL ),
	 2 );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "neutral language match",
	 evttools_language_get_match_score( 0x00000407UL, 0x00000007UL ),
	 1 );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "no match",
	 evttools_language_get_match_score( 0x00000407UL, 0x00000409UL ),
	 0 );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the language MUI tag function
 * Returns 1 if successful or 0 if not
 */
int evt_test_tools_language_get_mui_tag(
     void )
{
	const char *language_tag = NULL;

	language_tag = evttools_language_get_mui_tag(
	                0x00000409UL );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "language_tag",
	 language_tag );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "en-US first character",
	 language_tag[ 0 ],
	 'e' );

	language_tag = evttools_language_get_mui_tag(
	                0x00000804UL );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "language_tag",
	 language_tag );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "zh-CN first character",
	 language_tag[ 0 ],
	 'z' );

	language_tag = evttools_language_get_mui_tag(
	                0x00000411UL );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "language_tag",
	 language_tag );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "ja-JP first character",
	 language_tag[ 0 ],
	 'j' );

	return( 1 );

on_error:
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
	 "evttools_language_get_match_score",
	 evt_test_tools_language_get_match_score );

	EVT_TEST_RUN(
	 "evttools_language_get_mui_tag",
	 evt_test_tools_language_get_mui_tag );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}
