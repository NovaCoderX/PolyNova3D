// PolyNova3D (version 3.3)
/***************************************************************
 Copyright (C) 1999 Novasoft Consulting

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License as published by the Free Software Foundation; either
 version 2 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.

 You should have received a copy of the GNU Library General Public
 License along with this library; if not, write to the Free
 Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "logging.h"
#include "ini_file.h"

static char *configData = NULL;

static void String_Trim(char *string) {
	char *s = string + strlen(string) - 1;
	while (s >= string && isspace((int) *s)) {
		*s = '\0';
		s--;
	}
}

static char* Ini_GetString(const char *category, const char *key, const char *defaultValue, char *dest, int length, char *source) {
	char *s;
	char buffer[1024];
	int catLength;
	char *current;
	char *ret;

	if (dest != NULL) {
		*dest = '\0';
		/* Set the default value in case we jump out early */
		if (defaultValue != NULL)
			strncpy(dest, defaultValue, length);
		dest[length - 1] = '\0';
	}

	if (source == NULL)
		return NULL;

	sprintf(buffer, "[%s]", category);
	for (s = buffer; *s != '\0'; s++)
		*s = toupper(*s);
	catLength = (int) strlen(buffer);

	ret = source;

	for (current = source; current != NULL; current++) {
		const char *end;

		current = strchr(current, '[');
		if (current == NULL)
			break;

		if (strncasecmp(current, buffer, catLength) != 0)
			continue;
		if (current != source && *(current - 1) != '\r' && *(current - 1) != '\n')
			continue;

		current += catLength;
		while (isspace((int) *current))
			current++;

		/* Find the end of this block */
		for (end = current; end != NULL; end++) {
			end = strchr(end, '[');
			if (end == NULL)
				break;

			if (*(end - 1) == '\r' || *(end - 1) == '\n')
				break;
		}

		/* If there is no other '[', take the last char of the file */
		if (end == NULL)
			end = current + strlen(current);

		if (key != NULL) {
			int keyLength = (int) strlen(key);

			ret = current;

			while (current < end) {
				char *value;
				char *lineEnd;

				/* Check to see if there is nothing behind the key ('a' should not match 'aa') */
				value = current + keyLength;
				while (isspace((int) *value))
					value++;

				/* Now validate the size and if we match at all */
				if (*value != '=' || strncasecmp(current, key, keyLength) != 0) {
					/* Search for LF to support both CR/LF and LF line endings. */
					current = strchr(current, '\n');
					if (current == NULL)
						break;
					while (isspace((int) *current))
						current++;
					if (current > end)
						break;

					continue;
				}

				ret = current;

				/* Get the value */
				current = value + 1;

				/* Find the end of the line */
				lineEnd = strchr(current, '\n');
				if (lineEnd == NULL)
					break;
				while (isspace((int) *lineEnd))
					lineEnd++;
				if (lineEnd > end)
					break;

				/* Copy the value */
				if (dest != NULL) {
					int len = (int) (lineEnd - current);
					memcpy(dest, current, len);
					*(dest + len) = '\0';

					String_Trim(dest);
				}

				return ret;
			}

			/* Failed to find the key. Return anyway. */
			return NULL;
		}

		ret = current;
		if (dest == NULL)
			return ret;

		/* Read all the keys from this section */
		while (true) {
			int len;
			char *lineEnd;

			lineEnd = strchr(current, '=');
			if (lineEnd == NULL || lineEnd > end)
				break;

			len = (int) (lineEnd - current);
			memcpy(dest, current, len);
			*(dest + len) = '\0';

			String_Trim(dest);
			dest += strlen(dest) + 1;

			/* Find the next line, ignoring all \r\n */
			current = strchr(current, '\n');
			if (current == NULL)
				break;
			while (isspace((int) *current))
				current++;
			if (current > end)
				break;
		}

		*dest++ = '\0';
		/* end the list with a zero element */
		*dest++ = '\0';

		return ret;
	}

	return NULL;
}

static int Ini_GetInteger(const char *category, const char *key, int defaultValue, char *source) {
	char value[16];
	char buffer[16];

	sprintf(value, "%d", defaultValue);
	Ini_GetString(category, key, value, buffer, 15, source);
	return atoi(buffer);
}

static float Ini_GetFloat(const char *category, const char *key, float defaultValue, char *source) {
	char value[16];
	char buffer[16];

	sprintf(value, "%f", defaultValue);
	Ini_GetString(category, key, value, buffer, 15, source);
	return atof(buffer);
}

bool Open_IniFile() {
	FILE *f = NULL;
	long fileSize;
	char path[PATH_MAX];

	LogMessage("Opening the INI file\n");

	snprintf(path, sizeof(path), "%spolynova.ini", "PROGDIR:");
	f = fopen(path, "rb");
	if (f == NULL) {
		LogWarningMessage("polynova.ini file not found.\n");
		return false;
	}

	if (fseek(f, 0, SEEK_END) < 0) {
		LogWarningMessage("Cannot get polynova.ini file size.\n");
		fclose(f);
		return false;
	}
	fileSize = ftell(f);
	if (fileSize < 0) {
		LogWarningMessage("Cannot get polynova.ini file size.\n");
		fclose(f);
		return false;
	}
	rewind(f);
	configData = (char*) malloc(fileSize + 1);
	if (configData == NULL) {
		LogWarningMessage("Cannot allocate %ld bytes\n", fileSize + 1);
		fclose(f);
		return false;
	}
	if ((long) fread(configData, 1, fileSize, f) != fileSize) {
		LogWarningMessage("Failed to read polynova.ini\n");
		fclose(f);
		free(configData);
		configData = NULL;
		return false;
	}

	configData[fileSize] = '\0';
	fclose(f);
	return true;
}

void Close_IniFile() {
	LogMessage("Closing the INI file\n");

	if (configData) {
		free(configData);
		configData = NULL;
	}
}

char* IniFile_GetString(const char *key, const char *defaultValue, char *dest, int length) {
	char *p;
	int i;
	/* if g_openduneini is NULL, Ini_GetString() still does what we expect */
	p = Ini_GetString("polynova", key, defaultValue, dest, length, configData);
	if (p) {
		/* Trim space from the beginning of the dest */
		for (i = 0; i < length && (dest[i] == ' ' || dest[i] == '\t') && (dest[i] != '\0'); i++)
			;
		if (i > 0 && i < length)
			memmove(dest, dest + i, length - i);
	}
	return p;
}

int IniFile_GetInteger(const char *key, int defaultValue) {
	if (configData == NULL)
		return defaultValue;
	return Ini_GetInteger("polynova", key, defaultValue, configData);
}

float IniFile_GetFloat(const char *key, float defaultValue) {
	if (configData == NULL)
		return defaultValue;
	return Ini_GetFloat("polynova", key, defaultValue, configData);
}

