/*
 * Language functions
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
#include <types.h>

#include "language.h"

/* Determines how well a resource language identifier matches the preferred language
 * Returns a match score where a higher value is a better match
 */
int evttools_language_get_match_score(
     uint32_t preferred_language_identifier,
     uint32_t resource_language_identifier )
{
	uint32_t preferred_language = preferred_language_identifier & 0x0000ffffUL;
	uint32_t resource_language  = resource_language_identifier & 0x0000ffffUL;
	uint32_t preferred_language_primary = preferred_language & 0x000003ffUL;
	uint32_t resource_language_primary  = resource_language & 0x000003ffUL;
	uint32_t resource_language_sub      = ( resource_language >> 10 ) & 0x0000003fUL;

	if( preferred_language == resource_language )
	{
		return( 3 );
	}
	if( resource_language_primary == 0 )
	{
		return( 1 );
	}
	if( preferred_language_primary == resource_language_primary )
	{
		if( resource_language_sub == 0 )
		{
			return( 1 );
		}
		return( 2 );
	}
	return( 0 );
}

/* Retrieves the MUI language string for a language identifier
 * Returns the language string or NULL if not supported
 */
const char *evttools_language_get_mui_tag(
             uint32_t language_identifier )
{
	switch( language_identifier & 0x0000ffffUL )
	{
		case( 0x0000 ):
			return( "en-US" );

		case( 0x0401 ):
			return( "ar-SA" );

		case( 0x0404 ):
			return( "zh-TW" );

		case( 0x0405 ):
			return( "cs-CZ" );

		case( 0x0406 ):
			return( "da-DK" );

		case( 0x0407 ):
			return( "de-DE" );

		case( 0x0408 ):
			return( "el-GR" );

		case( 0x0409 ):
			return( "en-US" );

		case( 0x040a ):
			return( "es-ES" );

		case( 0x040b ):
			return( "fi-FI" );

		case( 0x040c ):
			return( "fr-FR" );

		case( 0x040d ):
			return( "he-IL" );

		case( 0x040e ):
			return( "hu-HU" );

		case( 0x0410 ):
			return( "it-IT" );

		case( 0x0411 ):
			return( "ja-JP" );

		case( 0x0412 ):
			return( "ko-KR" );

		case( 0x0413 ):
			return( "nl-NL" );

		case( 0x0414 ):
			return( "nb-NO" );

		case( 0x0415 ):
			return( "pl-PL" );

		case( 0x0416 ):
			return( "pt-BR" );

		case( 0x0418 ):
			return( "ro-RO" );

		case( 0x0419 ):
			return( "ru-RU" );

		case( 0x041a ):
			return( "hr-HR" );

		case( 0x041b ):
			return( "sk-SK" );

		case( 0x041d ):
			return( "sv-SE" );

		case( 0x041e ):
			return( "th-TH" );

		case( 0x041f ):
			return( "tr-TR" );

		case( 0x0421 ):
			return( "id-ID" );

		case( 0x0422 ):
			return( "uk-UA" );

		case( 0x0424 ):
			return( "sl-SI" );

		case( 0x0425 ):
			return( "et-EE" );

		case( 0x0426 ):
			return( "lv-LV" );

		case( 0x0427 ):
			return( "lt-LT" );

		case( 0x0429 ):
			return( "fa-IR" );

		case( 0x042a ):
			return( "vi-VN" );

		case( 0x0436 ):
			return( "af-ZA" );

		case( 0x0439 ):
			return( "hi-IN" );

		case( 0x043e ):
			return( "ms-MY" );

		case( 0x043f ):
			return( "kk-KZ" );

		case( 0x0444 ):
			return( "tt-RU" );

		case( 0x0445 ):
			return( "bn-IN" );

		case( 0x0446 ):
			return( "pa-IN" );

		case( 0x0447 ):
			return( "gu-IN" );

		case( 0x0449 ):
			return( "ta-IN" );

		case( 0x044a ):
			return( "te-IN" );

		case( 0x044b ):
			return( "kn-IN" );

		case( 0x044c ):
			return( "ml-IN" );

		case( 0x044e ):
			return( "mr-IN" );

		case( 0x0450 ):
			return( "mn-MN" );

		case( 0x0453 ):
			return( "km-KH" );

		case( 0x0454 ):
			return( "lo-LA" );

		case( 0x0456 ):
			return( "gl-ES" );

		case( 0x0457 ):
			return( "kok-IN" );

		case( 0x045b ):
			return( "si-LK" );

		case( 0x045e ):
			return( "am-ET" );

		case( 0x0461 ):
			return( "ne-NP" );

		case( 0x0464 ):
			return( "fil-PH" );

		case( 0x0804 ):
			return( "zh-CN" );

		case( 0x0807 ):
			return( "de-CH" );

		case( 0x0809 ):
			return( "en-GB" );

		case( 0x080a ):
			return( "es-MX" );

		case( 0x080c ):
			return( "fr-BE" );

		case( 0x0810 ):
			return( "it-CH" );

		case( 0x0813 ):
			return( "nl-BE" );

		case( 0x0814 ):
			return( "nn-NO" );

		case( 0x0816 ):
			return( "pt-PT" );

		case( 0x081d ):
			return( "sv-FI" );

		case( 0x0c04 ):
			return( "zh-HK" );

		case( 0x0c07 ):
			return( "de-AT" );

		case( 0x0c09 ):
			return( "en-AU" );

		case( 0x0c0a ):
			return( "es-ES" );

		case( 0x0c0c ):
			return( "fr-CA" );

		case( 0x1004 ):
			return( "zh-SG" );

		case( 0x1007 ):
			return( "de-LU" );

		case( 0x1009 ):
			return( "en-CA" );

		case( 0x100c ):
			return( "fr-CH" );

		case( 0x1404 ):
			return( "zh-MO" );

		case( 0x1409 ):
			return( "en-NZ" );

		case( 0x1809 ):
			return( "en-IE" );

		case( 0x1c09 ):
			return( "en-ZA" );

		case( 0x2009 ):
			return( "en-JM" );

		case( 0x2809 ):
			return( "en-BZ" );

		case( 0x2c09 ):
			return( "en-TT" );

		case( 0x3009 ):
			return( "en-ZW" );

		case( 0x3409 ):
			return( "en-PH" );

		default:
			break;
	}
	return( NULL );
}
