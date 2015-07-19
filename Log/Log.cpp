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

#include "stdafx.h"
#include "..\log.h"
#include "LogFilePolicy.h"
#include "LogFileConfigPolicy.h"

#if defined(LOG_DEBUG_POLICY)
	typedef TLog< LogFileConfigPolicy, LogDebugPolicy > LogType;
#elif defined(LOG_FILE_POLICY) 
	typedef TLog< LogFileConfigPolicy, LogFilePolicy > LogType;
#else
	#error Log policy wasn't defined
#endif

LogType *Log;

int GetLogInterfaceVersion()
{
  return LOG_INTERFACE_VERSION;
}

ILog100* CreateLogObject()
{
	try
	{
		Log = new LogType();
	}
	catch(...)
	{
		Log = NULL;
	}
	return Log;
}

void ReleaseLogObject()
{
	if(Log)
	{
		delete Log;
		Log = NULL;
	}
}
