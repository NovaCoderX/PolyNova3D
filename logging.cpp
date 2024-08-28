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
#include "poly_nova_3d.h"


static FILE* _logFile = NULL;
static FILE* _warningLogFile = NULL;


extern void LogMessage(const char *fmtString, ... ) {
	char message[MAX_LOG_MESSAGE_LENGTH];
	va_list args;

	va_start(args, fmtString);
	vsnprintf(message, MAX_LOG_MESSAGE_LENGTH, fmtString, args);
	va_end(args);

    if (_logFile) {
        fwrite(message, 1, strlen(message), _logFile);
    }
}

void LogWarningMessage(const char *fmtString, ... ) {
	char message[MAX_LOG_MESSAGE_LENGTH];
	va_list args;

	va_start(args, fmtString);
	vsnprintf(message, MAX_LOG_MESSAGE_LENGTH, fmtString, args);
	va_end(args);

    if (!_warningLogFile) {
        _warningLogFile = fopen("NOVA_WARNING.TXT", "w");
        if (_warningLogFile) {
            setbuf(_warningLogFile, NULL);
        }
    }

    if (_warningLogFile) {
        fwrite(message, 1, strlen(message), _warningLogFile);
    }
}

void LogErrorMessage(const char *message) {
	FILE* errorLogFile = NULL;

	// Always append for errors.
	errorLogFile = fopen("NOVA__ERROR.TXT", "a");
	if (errorLogFile) {
		setbuf(errorLogFile, NULL);
        fwrite(message, 1, strlen(message), errorLogFile);
        fclose (errorLogFile);
        errorLogFile = NULL;
    }
}

void LoggingInit(void) {
	 _logFile = fopen("NOVA_LOG.TXT", "w");
	 if (_logFile) {
		 setbuf(_logFile, NULL);
	 }
}

void LoggingShutdown(void) {
	LogMessage("Shutting down the Logger.....\n");

	if (_logFile) {
        fclose (_logFile);
        _logFile = NULL;
    }

    if (_warningLogFile) {
        fclose (_warningLogFile);
        _warningLogFile = NULL;
    }
}


