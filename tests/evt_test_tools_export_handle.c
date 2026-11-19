/*
 * Tools export_handle type test program
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

#include <string.h>

#include "evt_test_libcerror.h"
#include "evt_test_macros.h"
#include "evt_test_unused.h"

#include "../evttools/export_handle.h"

#include "../libevt/libevt_event_record.h"
#include "../libevt/libevt_record.h"
#include "../libevt/libevt_record_values.h"
#include "../libevt/libevt_strings_array.h"

int evt_test_tools_export_handle_export_record_event_message(
     libevt_record_t *record );

/* Runs a message format test
 * Returns 1 if successful or 0 if not
 */
int evt_test_tools_export_handle_print_message_case(
     libevt_record_t *record,
     const system_character_t *format_string,
     const char *expected_string )
{
	libcerror_error_t *error = NULL;
	message_string_t message_string;
	export_handle_t export_handle;
	FILE *stream             = NULL;
	char output_string[ 256 ];
	size_t output_size       = 0;
	int result               = 0;

	memory_set(
	 &message_string,
	 0,
	 sizeof( message_string_t ) );
	memory_set(
	 &export_handle,
	 0,
	 sizeof( export_handle_t ) );

	stream = tmpfile();

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "stream",
	 stream );

	export_handle.notify_stream = stream;

	message_string.string      = (system_character_t *) format_string;
	message_string.string_size = system_string_length(
	                              format_string ) + 1;

	result = export_handle_print_message(
	          &export_handle,
	          NULL,
	          0,
	          &message_string,
	          record,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( fflush(
	     stream ) != 0 )
	{
		goto on_error;
	}
	if( fseek(
	     stream,
	     0,
	     SEEK_SET ) != 0 )
	{
		goto on_error;
	}
	output_size = fread(
	               output_string,
	               1,
	               sizeof( output_string ) - 1,
	               stream );

	output_string[ output_size ] = 0;

	if( strcmp(
	     output_string,
	     expected_string ) != 0 )
	{
		fprintf(
		 stdout,
		 "unexpected message string: \"%s\"\n",
		 output_string );

		goto on_error;
	}
	fclose(
	 stream );

	return( 1 );

on_error:
	if( stream != NULL )
	{
		fclose(
		 stream );
	}
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the message parameter expansion
 * Returns 1 if successful or 0 if not
 */
int evt_test_tools_export_handle_print_message(
     void )
{
	libcerror_error_t *error               = NULL;
	libevt_event_record_t *event_record    = NULL;
	libevt_record_values_t *record_values  = NULL;
	libevt_record_t *record                = NULL;
	libevt_strings_array_t *strings_array  = NULL;
	uint8_t strings_data[ 18 ]             = {
		'4', 0, '2', 0, 0, 0,
		'a', 0, 'l', 0, 'p', 0, 'h', 0, 'a', 0, 0, 0 };
	int result                             = 0;

	result = libevt_strings_array_initialize(
	          &strings_array,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "strings_array",
	 strings_array );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libevt_strings_array_read_data(
	          strings_array,
	          strings_data,
	          18,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libevt_event_record_initialize(
	          &event_record,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "event_record",
	 event_record );

	event_record->strings = strings_array;
	strings_array         = NULL;

	result = libevt_record_values_initialize(
	          &record_values,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "record_values",
	 record_values );

	record_values->event_record = event_record;
	event_record                = NULL;

	result = libevt_record_initialize(
	          &record,
	          NULL,
	          NULL,
	          record_values,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "record",
	 record );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	EVT_TEST_RUN_WITH_ARGS(
	 "message parameter",
	 evt_test_tools_export_handle_print_message_case,
	 record,
	 _SYSTEM_STRING( "Value: %1" ),
	 "Message string\t\t\t: Value: 42\n" );

	EVT_TEST_RUN_WITH_ARGS(
	 "message parameter type",
	 evt_test_tools_export_handle_print_message_case,
	 record,
	 _SYSTEM_STRING( "Value: %1!d!" ),
	 "Message string\t\t\t: Value: 42\n" );

	EVT_TEST_RUN_WITH_ARGS(
	 "message parameter hex",
	 evt_test_tools_export_handle_print_message_case,
	 record,
	 _SYSTEM_STRING( "Value: %1!08x!" ),
	 "Message string\t\t\t: Value: 0000002a\n" );

	EVT_TEST_RUN_WITH_ARGS(
	 "message parameter precision",
	 evt_test_tools_export_handle_print_message_case,
	 record,
	 _SYSTEM_STRING( "Value: %2!.3s!" ),
	 "Message string\t\t\t: Value: alp\n" );

	EVT_TEST_RUN_WITH_ARGS(
	 "message multiple parameters",
	 evt_test_tools_export_handle_print_message_case,
	 record,
	 _SYSTEM_STRING( "%2=%1" ),
	 "Message string\t\t\t: alpha=42\n" );

	EVT_TEST_RUN_WITH_ARGS(
	 "message unsupported parameter type",
	 evt_test_tools_export_handle_print_message_case,
	 record,
	 _SYSTEM_STRING( "%1!q!" ),
	 "Message string\t\t\t: 42\n" );

	EVT_TEST_RUN_WITH_ARGS(
	 "message escape sequences",
	 evt_test_tools_export_handle_print_message_case,
	 record,
	 _SYSTEM_STRING( "%2%n%1%t%%" ),
	 "Message string\t\t\t: alpha\n42\t%\n" );

	EVT_TEST_RUN_WITH_ARGS(
	 "message terminate",
	 evt_test_tools_export_handle_print_message_case,
	 record,
	 _SYSTEM_STRING( "%1%0ignored" ),
	 "Message string\t\t\t: 42" );

	EVT_TEST_RUN_WITH_ARGS(
	 "export_handle_export_record_event_message",
	 evt_test_tools_export_handle_export_record_event_message,
	 record );

	result = libevt_record_free(
	          &record,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	result = libevt_record_values_free(
	          &record_values,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( record != NULL )
	{
		libevt_record_free(
		 &record,
		 NULL );
	}
	if( record_values != NULL )
	{
		libevt_record_values_free(
		 &record_values,
		 NULL );
	}
	if( event_record != NULL )
	{
		libevt_event_record_free(
		 &event_record,
		 NULL );
	}
	if( strings_array != NULL )
	{
		libevt_strings_array_free(
		 &strings_array,
		 NULL );
	}
	return( 0 );
}

/* Tests the record event message fields
 * Returns 1 if successful or 0 if not
 */
int evt_test_tools_export_handle_export_record_event_message(
     libevt_record_t *record )
{
	libcerror_error_t *error = NULL;
	export_handle_t export_handle;
	FILE *stream             = NULL;
	char output_string[ 256 ];
	size_t output_size       = 0;
	int result               = 0;

	memory_set(
	 &export_handle,
	 0,
	 sizeof( export_handle_t ) );

	stream = tmpfile();

	EVT_TEST_ASSERT_IS_NOT_NULL(
	 "stream",
	 stream );

	export_handle.notify_stream = stream;

	result = export_handle_export_record_event_message(
	          &export_handle,
	          record,
	          NULL,
	          0,
	          0,
	          NULL,
	          &error );

	EVT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	EVT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( fflush(
	     stream ) != 0 )
	{
		goto on_error;
	}
	if( fseek(
	     stream,
	     0,
	     SEEK_SET ) != 0 )
	{
		goto on_error;
	}
	output_size = fread(
	               output_string,
	               1,
	               sizeof( output_string ) - 1,
	               stream );

	output_string[ output_size ] = 0;

	if( strcmp(
	     output_string,
	     "Number of strings\t\t: 2\n"
	     "String: 1\t\t\t: 42\n"
	     "String: 2\t\t\t: alpha\n" ) != 0 )
	{
		fprintf(
		 stdout,
		 "unexpected record fields: \"%s\"\n",
		 output_string );

		goto on_error;
	}
	fclose(
	 stream );

	return( 1 );

on_error:
	if( stream != NULL )
	{
		fclose(
		 stream );
	}
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
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
	 "export_handle_print_message",
	 evt_test_tools_export_handle_print_message );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}
