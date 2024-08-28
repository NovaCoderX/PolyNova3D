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

#ifndef __LOGGING_H
#define __LOGGING_H

#define PATH_MAX 255

#define MAX_LOG_MESSAGE_LENGTH 4000

extern void LogMessage(const char *fmtString, ...);
extern void LogWarningMessage(const char *fmtString, ...);
extern void LogErrorMessage(const char *message);

extern void LoggingInit(void);
extern void LoggingShutdown(void);

#endif // __LOGGING_H
