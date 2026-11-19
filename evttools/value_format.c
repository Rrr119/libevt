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

#include <common.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

#include <stdio.h>
#include <stdlib.h>

#include "evttools_libcerror.h"
#include "value_format.h"

typedef struct evttools_value_format_spec evttools_value_format_spec_t;

struct evttools_value_format_spec
{
	/* The flags */
	uint8_t alternate_form;
	uint8_t left_align;
	uint8_t plus_sign;
	uint8_t space_sign;
	uint8_t zero_pad;
	uint8_t is_64bit;

	/* The minimum width */
	size_t width;

	/* The precision, -1 if not set */
	int precision;

	/* The conversion character */
	char conversion;
};

/* Copies an ASCII string into a system string
 * Returns 1 if successful or -1 on error
 */
static int evttools_value_format_copy_ascii_string(
            const char *ascii_string,
            size_t ascii_string_length,
            system_character_t **formatted_string,
            size_t *formatted_string_size,
            libcerror_error_t **error )
{
	static char *function = "evttools_value_format_copy_ascii_string";
	size_t string_index   = 0;

	if( ( ascii_string_length + 1 ) > ( MEMORY_MAXIMUM_ALLOCATION_SIZE / sizeof( system_character_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid ASCII string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	*formatted_string = system_string_allocate(
	                     ascii_string_length + 1 );

	if( *formatted_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create formatted string.",
		 function );

		return( -1 );
	}
	for( string_index = 0;
	     string_index < ascii_string_length;
	     string_index++ )
	{
		( *formatted_string )[ string_index ] = (system_character_t) ascii_string[ string_index ];
	}
	( *formatted_string )[ ascii_string_length ] = 0;
	*formatted_string_size                       = ascii_string_length + 1;

	return( 1 );
}

/* Parses a value format specification
 * Returns 1 if successful or 0 if the specification is not supported
 */
static int evttools_value_format_parse_spec(
            evttools_value_format_spec_t *format_spec,
            const system_character_t *format_string,
            size_t format_string_length )
{
	size_t format_string_index = 0;
	uint64_t value_64bit       = 0;

	if( ( format_spec == NULL )
	 || ( format_string == NULL ) )
	{
		return( 0 );
	}
	format_spec->precision = -1;

	while( format_string_index < format_string_length )
	{
		if( format_string[ format_string_index ] == (system_character_t) '-' )
		{
			format_spec->left_align = 1;
		}
		else if( format_string[ format_string_index ] == (system_character_t) '+' )
		{
			format_spec->plus_sign = 1;
		}
		else if( format_string[ format_string_index ] == (system_character_t) ' ' )
		{
			format_spec->space_sign = 1;
		}
		else if( format_string[ format_string_index ] == (system_character_t) '#' )
		{
			format_spec->alternate_form = 1;
		}
		else if( format_string[ format_string_index ] == (system_character_t) '0' )
		{
			format_spec->zero_pad = 1;
		}
		else
		{
			break;
		}
		format_string_index++;
	}
	while( ( format_string_index < format_string_length )
	    && ( format_string[ format_string_index ] >= (system_character_t) '0' )
	    && ( format_string[ format_string_index ] <= (system_character_t) '9' ) )
	{
		value_64bit = ( value_64bit * 10 ) + ( format_string[ format_string_index ] - (system_character_t) '0' );

		if( value_64bit > 1024 )
		{
			return( 0 );
		}
		format_string_index++;
	}
	format_spec->width = (size_t) value_64bit;

	if( ( format_string_index < format_string_length )
	 && ( format_string[ format_string_index ] == (system_character_t) '.' ) )
	{
		format_string_index++;

		value_64bit            = 0;
		format_spec->precision = 0;

		while( ( format_string_index < format_string_length )
		    && ( format_string[ format_string_index ] >= (system_character_t) '0' )
		    && ( format_string[ format_string_index ] <= (system_character_t) '9' ) )
		{
			value_64bit = ( value_64bit * 10 ) + ( format_string[ format_string_index ] - (system_character_t) '0' );

			if( value_64bit > 1024 )
			{
				return( 0 );
			}
			format_string_index++;
		}
		format_spec->precision = (int) value_64bit;
	}
	/* Skip the Windows and C length modifiers. The value is always
	 * converted to a 64-bit integer or double first.
	 */
	while( format_string_index < format_string_length )
	{
		if( ( format_string[ format_string_index ] == (system_character_t) 'I' )
		 && ( ( format_string_index + 2 ) < format_string_length )
		 && ( format_string[ format_string_index + 1 ] == (system_character_t) '6' )
		 && ( format_string[ format_string_index + 2 ] == (system_character_t) '4' ) )
		{
			format_spec->is_64bit = 1;
			format_string_index  += 3;

			continue;
		}
		if( ( format_string[ format_string_index ] == (system_character_t) 'I' )
		 && ( ( format_string_index + 2 ) < format_string_length )
		 && ( format_string[ format_string_index + 1 ] == (system_character_t) '3' )
		 && ( format_string[ format_string_index + 2 ] == (system_character_t) '2' ) )
		{
			format_string_index += 3;

			continue;
		}
		if( ( format_string[ format_string_index ] == (system_character_t) 'l' )
		 && ( ( format_string_index + 1 ) < format_string_length )
		 && ( format_string[ format_string_index + 1 ] == (system_character_t) 'l' ) )
		{
			format_spec->is_64bit = 1;
			format_string_index  += 2;

			continue;
		}
		if( format_string[ format_string_index ] == (system_character_t) 'q' )
		{
			format_spec->is_64bit = 1;
			format_string_index  += 1;

			continue;
		}
		switch( format_string[ format_string_index ] )
		{
			case( 'h' ):
			case( 'l' ):
			case( 'L' ):
			case( 'j' ):
			case( 'z' ):
			case( 't' ):
			case( 'w' ):
			case( 'I' ):
			case( '0' ):
			case( '1' ):
			case( '2' ):
			case( '3' ):
			case( '4' ):
			case( '5' ):
			case( '6' ):
			case( '7' ):
			case( '8' ):
			case( '9' ):
				format_string_index++;
				break;

			default:
				goto parse_conversion;
		}
	}
parse_conversion:
	if( format_string_index >= format_string_length )
	{
		return( 0 );
	}
	format_spec->conversion = (char) format_string[ format_string_index++ ];

	if( format_string_index != format_string_length )
	{
		return( 0 );
	}
	switch( format_spec->conversion )
	{
		case( 'c' ):
		case( 'C' ):
		case( 'd' ):
		case( 'D' ):
		case( 'i' ):
		case( 'u' ):
		case( 'U' ):
		case( 'o' ):
		case( 'O' ):
		case( 'x' ):
		case( 'X' ):
		case( 'f' ):
		case( 'F' ):
		case( 'e' ):
		case( 'E' ):
		case( 'g' ):
		case( 'G' ):
		case( 'a' ):
		case( 'A' ):
		case( 's' ):
		case( 'S' ):
			return( 1 );

		default:
			break;
	}
	return( 0 );
}

/* Parses an integer value
 * Returns 1 if successful or 0 if the value is not numeric
 */
static int evttools_value_format_parse_integer(
            const system_character_t *value_string,
            size_t value_string_length,
            int64_t *signed_value_64bit,
            uint64_t *unsigned_value_64bit )
{
	size_t value_string_index = 0;
	uint64_t value_64bit      = 0;
	uint64_t digit_value      = 0;
	uint8_t is_negative       = 0;
	uint8_t is_hexadecimal    = 0;
	uint8_t has_digits        = 0;

	if( ( value_string == NULL )
	 || ( value_string_length == 0 )
	 || ( signed_value_64bit == NULL )
	 || ( unsigned_value_64bit == NULL ) )
	{
		return( 0 );
	}
	while( ( value_string_index < value_string_length )
	    && ( ( value_string[ value_string_index ] == (system_character_t) ' ' )
	     || ( value_string[ value_string_index ] == (system_character_t) '\t' ) ) )
	{
		value_string_index++;
	}
	if( value_string_index < value_string_length )
	{
		if( value_string[ value_string_index ] == (system_character_t) '-' )
		{
			is_negative = 1;

			value_string_index++;
		}
		else if( value_string[ value_string_index ] == (system_character_t) '+' )
		{
			value_string_index++;
		}
	}
	if( ( value_string_index + 1 ) < value_string_length )
	{
		if( ( value_string[ value_string_index ] == (system_character_t) '0' )
		 && ( ( value_string[ value_string_index + 1 ] == (system_character_t) 'x' )
		  ||  ( value_string[ value_string_index + 1 ] == (system_character_t) 'X' ) ) )
		{
			is_hexadecimal = 1;

			value_string_index += 2;
		}
	}
	while( value_string_index < value_string_length )
	{
		if( ( value_string[ value_string_index ] >= (system_character_t) '0' )
		 && ( value_string[ value_string_index ] <= (system_character_t) '9' ) )
		{
			digit_value = (uint64_t) ( value_string[ value_string_index ] - (system_character_t) '0' );
		}
		else if( ( is_hexadecimal != 0 )
		      && ( value_string[ value_string_index ] >= (system_character_t) 'a' )
		      && ( value_string[ value_string_index ] <= (system_character_t) 'f' ) )
		{
			digit_value = (uint64_t) ( value_string[ value_string_index ] - (system_character_t) 'a' ) + 10;
		}
		else if( ( is_hexadecimal != 0 )
		      && ( value_string[ value_string_index ] >= (system_character_t) 'A' )
		      && ( value_string[ value_string_index ] <= (system_character_t) 'F' ) )
		{
			digit_value = (uint64_t) ( value_string[ value_string_index ] - (system_character_t) 'A' ) + 10;
		}
		else
		{
			break;
		}
		if( is_hexadecimal != 0 )
		{
			if( value_64bit > ( UINT64_MAX >> 4 ) )
			{
				return( 0 );
			}
			value_64bit = ( value_64bit << 4 ) | digit_value;
		}
		else
		{
			if( value_64bit > ( ( UINT64_MAX - digit_value ) / 10 ) )
			{
				return( 0 );
			}
			value_64bit = ( value_64bit * 10 ) + digit_value;
		}
		has_digits = 1;

		value_string_index++;
	}
	if( has_digits == 0 )
	{
		return( 0 );
	}
	while( value_string_index < value_string_length )
	{
		if( ( value_string[ value_string_index ] != (system_character_t) ' ' )
		 && ( value_string[ value_string_index ] != (system_character_t) '\t' )
		 && ( value_string[ value_string_index ] != 0 ) )
		{
			return( 0 );
		}
		value_string_index++;
	}
	if( is_negative != 0 )
	{
		*signed_value_64bit = (int64_t) ( 0 - value_64bit );

		*unsigned_value_64bit = (uint64_t) ( 0 - value_64bit );
	}
	else
	{
		*signed_value_64bit   = (int64_t) value_64bit;
		*unsigned_value_64bit = value_64bit;
	}
	return( 1 );
}

/* Formats an integer value into a system string
 * Returns 1 if successful or 0 if the value is not numeric or -1 on error
 */
static int evttools_value_format_integer(
            const evttools_value_format_spec_t *format_spec,
            const system_character_t *value_string,
            size_t value_string_length,
            system_character_t **formatted_string,
            size_t *formatted_string_size,
            libcerror_error_t **error )
{
	static char *function      = "evttools_value_format_integer";
	system_character_t *string = NULL;
	char digits[ 65 ];
	const char *digits_string  = "0123456789abcdef";
	size_t content_length      = 0;
	size_t digit_count         = 0;
	size_t formatted_length    = 0;
	size_t leading_zero_count  = 0;
	size_t padding_length      = 0;
	size_t prefix_length       = 0;
	size_t string_index        = 0;
	uint64_t magnitude         = 0;
	uint64_t value_64bit       = 0;
	uint64_t digit_value       = 0;
	int64_t signed_value_64bit = 0;
	uint8_t base               = 10;
	uint8_t has_sign           = 0;
	uint8_t is_negative        = 0;
	uint8_t is_signed          = 0;
	uint8_t is_uppercase       = 0;

	switch( format_spec->conversion )
	{
		case( 'd' ):
		case( 'D' ):
		case( 'i' ):
			is_signed = 1;
			break;

		default:
			break;
	}
	if( evttools_value_format_parse_integer(
	     value_string,
	     value_string_length,
	     &signed_value_64bit,
	     &value_64bit ) == 0 )
	{
		return( 0 );
	}
	if( is_signed != 0 )
	{
		if( format_spec->is_64bit == 0 )
		{
			signed_value_64bit = (int64_t) (int32_t) signed_value_64bit;
		}
		if( signed_value_64bit < 0 )
		{
			is_negative = 1;
			magnitude   = (uint64_t) ( -( signed_value_64bit + 1 ) ) + 1;
		}
		else
		{
			magnitude = (uint64_t) signed_value_64bit;
		}
	}
	else
	{
		if( format_spec->is_64bit == 0 )
		{
			value_64bit = (uint64_t) (uint32_t) value_64bit;
		}
		magnitude = value_64bit;
	}
	switch( format_spec->conversion )
	{
		case( 'o' ):
		case( 'O' ):
			base = 8;
			break;

		case( 'x' ):
			base = 16;
			break;

		case( 'X' ):
			base         = 16;
			is_uppercase = 1;
			break;

		default:
			break;
	}
	if( is_uppercase != 0 )
	{
		digits_string = "0123456789ABCDEF";
	}
	while( magnitude > 0 )
	{
		digit_value = magnitude % base;
		magnitude  /= base;

		digits[ digit_count++ ] = digits_string[ digit_value ];
	}
	if( digit_count == 0 )
	{
		if( format_spec->precision == 0 )
		{
			leading_zero_count = 0;
		}
		else
		{
			digits[ digit_count++ ] = '0';
		}
	}
	if( format_spec->precision > (int) digit_count )
	{
		leading_zero_count = (size_t) format_spec->precision - digit_count;
	}
	if( is_negative != 0 )
	{
		has_sign = 1;
	}
	else if( ( is_signed != 0 )
	      && ( ( format_spec->plus_sign != 0 )
	       ||  ( format_spec->space_sign != 0 ) ) )
	{
		has_sign = 1;
	}
	if( ( format_spec->alternate_form != 0 )
	 && ( base == 16 )
	 && ( value_64bit != 0 ) )
	{
		prefix_length = 2;
	}
	else if( ( format_spec->alternate_form != 0 )
	      && ( base == 8 )
	      && ( digit_count > 0 )
	      && ( digits[ digit_count - 1 ] != '0' ) )
	{
		prefix_length = 1;
	}
	content_length = (size_t) has_sign + prefix_length + leading_zero_count + digit_count;

	if( format_spec->width > content_length )
	{
		padding_length = format_spec->width - content_length;
	}
	formatted_length = content_length;

	if( padding_length > 0 )
	{
		formatted_length += padding_length;
	}
	if( ( formatted_length + 1 ) > ( MEMORY_MAXIMUM_ALLOCATION_SIZE / sizeof( system_character_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: formatted string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	string = system_string_allocate(
	          formatted_length + 1 );

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create formatted string.",
		 function );

		return( -1 );
	}
	if( ( format_spec->left_align == 0 )
	 && ( ( format_spec->zero_pad == 0 )
	  ||  ( format_spec->precision >= 0 ) ) )
	{
		while( padding_length > 0 )
		{
			string[ string_index++ ] = (system_character_t) ' ';

			padding_length--;
		}
	}
	if( is_negative != 0 )
	{
		string[ string_index++ ] = (system_character_t) '-';
	}
	else if( ( is_signed != 0 )
	      && ( format_spec->plus_sign != 0 ) )
	{
		string[ string_index++ ] = (system_character_t) '+';
	}
	else if( ( is_signed != 0 )
	      && ( format_spec->space_sign != 0 ) )
	{
		string[ string_index++ ] = (system_character_t) ' ';
	}
	if( prefix_length == 2 )
	{
		string[ string_index++ ] = (system_character_t) '0';
		string[ string_index++ ] = (system_character_t) ( is_uppercase != 0 ? 'X' : 'x' );
	}
	else if( prefix_length == 1 )
	{
		string[ string_index++ ] = (system_character_t) '0';
	}
	if( ( format_spec->left_align == 0 )
	 && ( format_spec->zero_pad != 0 )
	 && ( format_spec->precision < 0 ) )
	{
		while( padding_length > 0 )
		{
			string[ string_index++ ] = (system_character_t) '0';

			padding_length--;
		}
	}
	while( leading_zero_count > 0 )
	{
		string[ string_index++ ] = (system_character_t) '0';

		leading_zero_count--;
	}
	while( digit_count > 0 )
	{
		string[ string_index++ ] = (system_character_t) digits[ --digit_count ];
	}
	if( format_spec->left_align != 0 )
	{
		while( padding_length > 0 )
		{
			string[ string_index++ ] = (system_character_t) ' ';

			padding_length--;
		}
	}
	string[ formatted_length ] = 0;

	*formatted_string      = string;
	*formatted_string_size = formatted_length + 1;

	return( 1 );
}

/* Formats a floating point value into a system string
 * Returns 1 if successful or 0 if the value is not numeric or -1 on error
 */
static int evttools_value_format_float(
            const evttools_value_format_spec_t *format_spec,
            const system_character_t *value_string,
            size_t value_string_length,
            system_character_t **formatted_string,
            size_t *formatted_string_size,
            libcerror_error_t **error )
{
	char *ascii_value_string  = NULL;
	char *value_string_end    = NULL;
	char formatted_value[ 4096 ];
	char format[ 32 ];
	static char *function     = "evttools_value_format_float";
	double value              = 0;
	size_t format_index       = 0;
	size_t string_index       = 0;
	int print_count           = 0;

	if( ( value_string == NULL )
	 || ( value_string_length == 0 )
	 || ( value_string_length > ( MEMORY_MAXIMUM_ALLOCATION_SIZE - 1 ) ) )
	{
		return( 0 );
	}
	ascii_value_string = (char *) memory_allocate(
	                              value_string_length + 1 );

	if( ascii_value_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create ASCII value string.",
		 function );

		return( -1 );
	}
	for( string_index = 0;
	     string_index < value_string_length;
	     string_index++ )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( value_string[ string_index ] > (system_character_t) 0x7f )
		{
			memory_free(
			 ascii_value_string );

			return( 0 );
		}
#endif
		ascii_value_string[ string_index ] = (char) value_string[ string_index ];
	}
	ascii_value_string[ value_string_length ] = 0;

	value = strtod(
	         ascii_value_string,
	         &value_string_end );

	if( value_string_end == ascii_value_string )
	{
		memory_free(
		 ascii_value_string );

		return( 0 );
	}
	memory_free(
	 ascii_value_string );

	format[ format_index++ ] = '%';

	if( format_spec->left_align != 0 )
	{
		format[ format_index++ ] = '-';
	}
	if( format_spec->plus_sign != 0 )
	{
		format[ format_index++ ] = '+';
	}
	if( format_spec->space_sign != 0 )
	{
		format[ format_index++ ] = ' ';
	}
	if( format_spec->alternate_form != 0 )
	{
		format[ format_index++ ] = '#';
	}
	if( format_spec->zero_pad != 0 )
	{
		format[ format_index++ ] = '0';
	}
	format_index += (size_t) sprintf(
	                         &( format[ format_index ] ),
	                         "%u",
	                         (unsigned int) format_spec->width );

	if( format_spec->precision >= 0 )
	{
		format_index += (size_t) sprintf(
		                         &( format[ format_index ] ),
		                         ".%d",
		                         format_spec->precision );
	}
	format[ format_index++ ] = format_spec->conversion;
	format[ format_index ]   = 0;

	print_count = narrow_string_snprintf(
	               formatted_value,
	               4096,
	               format,
	               value );

	if( ( print_count <= 0 )
	 || ( print_count >= 4096 ) )
	{
		return( 0 );
	}
	if( evttools_value_format_copy_ascii_string(
	     formatted_value,
	     (size_t) print_count,
	     formatted_string,
	     formatted_string_size,
	     error ) != 1 )
	{
		return( -1 );
	}
	return( 1 );
}

/* Formats a value string according to a MessageFormat value specification
 * Returns 1 if successful or -1 on error
 */
int evttools_value_format_allocate(
     const system_character_t *format_string,
     size_t format_string_length,
     const system_character_t *value_string,
     size_t value_string_length,
     system_character_t **formatted_string,
     size_t *formatted_string_size,
     libcerror_error_t **error )
{
	evttools_value_format_spec_t format_spec;
	static char *function       = "evttools_value_format_allocate";
	size_t copy_length          = 0;
	size_t formatted_length     = 0;
	size_t string_index         = 0;
	int result                  = 0;

	if( format_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid format string.",
		 function );

		return( -1 );
	}
	if( format_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid format string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( value_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value string.",
		 function );

		return( -1 );
	}
	if( value_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid value string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( formatted_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid formatted string.",
		 function );

		return( -1 );
	}
	if( *formatted_string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid formatted string value already set.",
		 function );

		return( -1 );
	}
	if( formatted_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid formatted string size.",
		 function );

		return( -1 );
	}
	*formatted_string_size = 0;

	memory_set(
	 &format_spec,
	 0,
	 sizeof( evttools_value_format_spec_t ) );

	format_spec.precision  = -1;
	format_spec.conversion = 's';

	if( format_string_length > 0 )
	{
		result = evttools_value_format_parse_spec(
		          &format_spec,
		          format_string,
		          format_string_length );

		if( result == 0 )
		{
			format_spec.width      = 0;
			format_spec.precision  = -1;
			format_spec.conversion = 's';
		}
	}
	switch( format_spec.conversion )
	{
		case( 'd' ):
		case( 'D' ):
		case( 'i' ):
		case( 'u' ):
		case( 'U' ):
		case( 'o' ):
		case( 'O' ):
		case( 'x' ):
		case( 'X' ):
			result = evttools_value_format_integer(
			          &format_spec,
			          value_string,
			          value_string_length,
			          formatted_string,
			          formatted_string_size,
			          error );

			if( result != 0 )
			{
				return( result );
			}
			break;

		case( 'f' ):
		case( 'F' ):
		case( 'e' ):
		case( 'E' ):
		case( 'g' ):
		case( 'G' ):
		case( 'a' ):
		case( 'A' ):
			result = evttools_value_format_float(
			          &format_spec,
			          value_string,
			          value_string_length,
			          formatted_string,
			          formatted_string_size,
			          error );

			if( result != 0 )
			{
				return( result );
			}
			break;

		default:
			break;
	}
	copy_length = value_string_length;

	if( ( format_spec.conversion == 'c' )
	 || ( format_spec.conversion == 'C' ) )
	{
		if( copy_length > 0 )
		{
			copy_length = 1;
		}
	}
	else if( ( format_spec.precision >= 0 )
	      && ( (size_t) format_spec.precision < copy_length ) )
	{
		copy_length = (size_t) format_spec.precision;
	}
	formatted_length = copy_length;

	if( format_spec.width > formatted_length )
	{
		formatted_length = format_spec.width;
	}
	if( ( formatted_length + 1 ) > ( MEMORY_MAXIMUM_ALLOCATION_SIZE / sizeof( system_character_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: formatted string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	*formatted_string = system_string_allocate(
	                     formatted_length + 1 );

	if( *formatted_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create formatted string.",
		 function );

		return( -1 );
	}
	if( format_spec.left_align == 0 )
	{
		while( string_index < ( formatted_length - copy_length ) )
		{
			( *formatted_string )[ string_index++ ] = (system_character_t) ' ';
		}
	}
	if( copy_length > 0 )
	{
		if( system_string_copy(
		     &( ( *formatted_string )[ string_index ] ),
		     value_string,
		     copy_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy value string.",
			 function );

			memory_free(
			 *formatted_string );

			*formatted_string = NULL;

			return( -1 );
		}
		string_index += copy_length;
	}
	while( string_index < formatted_length )
	{
		( *formatted_string )[ string_index++ ] = (system_character_t) ' ';
	}
	( *formatted_string )[ formatted_length ] = 0;
	*formatted_string_size                    = formatted_length + 1;

	return( 1 );
}
