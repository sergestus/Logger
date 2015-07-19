/******************************************************************************/
/*    Log - c++ logging library.                                              */
/*    Copyright (c) 2015 Sergey Bromirskiy.                                   */
/*                                                                            */
/*    This file is part of Log library.                                       */
/*                                                                            */
/*    Log library is free software: you can redistribute it and/or modify     */
/*    it under the terms of the GNU Lesser General Public License as          */
/*    published by the Free Software Foundation, either version 3 of the      */
/*	  License, or (at your option) any later version.                         */
/*                                                                            */
/*    Log library is distributed in the hope that it will be useful,          */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           */
/*    GNU Lesser General Public License for more details.                     */
/*                                                                            */
/*    You should have received a copy of the GNU Lesser General Public        */
/*    License along with Log library.  If not, see                            */
/*    <http://www.gnu.org/licenses/>.                                         */
/******************************************************************************/

#pragma once

#include "windows.h"
#include "FileProfile.h"

class LogFileConfigPolicy : public IConfig100
{
public:
	LogFileConfigPolicy()
	{
		char buffer[MAX_PATH];
		GetModuleFileName( NULL, buffer, MAX_PATH );

		int length = strlen(buffer);
		for(;length;length--) 
		{	
			if(buffer[length]=='\\') break;
			else if(buffer[length]=='.') 
				buffer[length] = 0;
		}
		if(buffer[length]=='\\') 
			strcpy_s( appName, buffer + length + 1 );
		else strcpy_s( appName, "unknown" );

		sprintf_s( ConfigFileName, "%s_log.ini", appName );
	}

	virtual ~LogFileConfigPolicy()
	{
	}

	virtual bool GetString(char *section, char *name, char *value, int length, char *defval )
	{
		if(!_stricmp(section,"common"))
		{
			if(!_stricmp(name,"appName"))
			{
				strcpy_s(value, length, appName);
				return true;
			}
		}		
		return GetPrivateProfileStringF( section, name, defval, value, length, ConfigFileName )!=0;		
	}

	virtual bool GetInteger(char *section, char *name, int *value, int defval )
	{
		return false;
	}

	virtual bool GetUInteger(char *section, char *name, unsigned int *value, unsigned int defval )
	{
		return false;
	}

	virtual bool GetBoolean(char *section, char *name, bool *value, bool defval )
	{
		return false;
	}


private:
	char appName[512];	
	char ConfigFileName[512];
};
