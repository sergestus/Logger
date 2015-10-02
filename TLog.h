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
#pragma warning( disable : 4355 )
#pragma warning( disable : 4996 )

#include "Log.h"
#include <stdio.h>	
#include <stdarg.h>	
#include <time.h>	
#include <typeinfo>

template < class LogConfigPolicy, class LogPolicy > class TLog : public LogConfigPolicy, public LogPolicy, public ILog100
{
public:
	typedef TLog< LogConfigPolicy, LogPolicy> LogType;

	TLog() : LogConfigPolicy(), LogPolicy(this), externLog(NULL)
	{
		char out[1024];

		defaultFilterLevel = LOG_DEBUG;
		if( GetString("common","filterLevel",out,sizeof(out),"debug") )
			defaultFilterLevel = GetMessgeType(out);

		startTime = GetTimeMs();			
		StartLog( "Log started " );
	}

	virtual ~TLog()
	{
		char out[1024];
		int length = sprintf_s( out, "Log released " );
		time_t now = time(0);
		ctime_s( out+length, sizeof(out)-length, &now );
		if(externLog) externLog( out );
		else Write( out );
	}

	bool FilterMessage( unsigned int messageId )
	{
		unsigned int messageType = messageId&0xF;
		//unsigned int messageCategory = messageId>>4;

		//if( messageCategory == 0)
		{
			if( messageType < defaultFilterLevel ) return true;
			return false;
		}
		//else if( (categoryMask&messageCategory) == 0 ) return true;			
		//unsigned int messageCategoryCount = 32 - __lzcnt(messageCategory);
		//if( messageType < categoryFilterLevel[messageCategory] ) return true;
		//return false;
	}

	virtual void Log( unsigned int messageId, const char *fmt, ... )
	{
		char out[1024]; 
		if( FilterMessage(messageId) ) return;

		unsigned __int64 delta = GetTimeMs()-startTime; 
		int length = _snprintf( out, sizeof(out), "%.2llu:%.2llu:%.2llu:%.3llu:%s: ", 
			delta/3600000, (delta/60000)%60, (delta/1000)%60, delta%1000, GetMessgeTypeString(0xF&messageId) );
			
		va_list body;
		va_start(body, fmt);
		vsnprintf( out+length, sizeof(out)-length-1, fmt, body );
		out[ sizeof(out)-1 ] = 0;

		va_end(body);
		if(externLog) externLog( out );
		else Write( out );
	}

	virtual void RedirectLog( void (*log) (char *))
	{
		if(externLog) externLog("Log redirecting to user defined stream\n" );
		else Write("Log redirecting to user defined stream\n" );
		externLog = log;
		StartLog( "Log redirected to user defined stream " );
	}

	void StartLog( char *comment )
	{
		char out[1024];
		int length = _snprintf( out, sizeof(out), comment );
		time_t now = time(0);
		ctime_s( out+length, sizeof(out)-length, &now );
		if(externLog) externLog( out );
		else Write( out );

		_snprintf( out, sizeof(out), "Log Policy: %s\n", typeid(LogPolicy).name() );
		if(externLog) externLog( out );
		else Write( out );

		_snprintf( out, sizeof(out), "Log Config Policy: %s\n", typeid(LogConfigPolicy).name() );
		if(externLog) externLog( out );
		else Write( out );

		_snprintf( out, sizeof(out), "Log filterLevel = %s(0x%x)\n", 
			GetMessgeTypeString(defaultFilterLevel), defaultFilterLevel );
		if(externLog) externLog( out );
		else Write( out );
	}		

	char *GetMessgeTypeString( unsigned int type)
	{
		if( type == LOG_DEBUG ) return "debug";
		else if( type == LOG_INFO )	return "info";
		else if( type == LOG_WARNING ) return "warning";
		else if( type == LOG_ERROR ) return "error";
		else if( type == 0xf ) return "disable";
		else return "undefined";
	}

	unsigned int GetMessgeType( char *str)
	{
		if( !strcmp(str,"debug") ) return LOG_DEBUG;
		else if( !strcmp(str,"info") ) return LOG_INFO;
		else if( !strcmp(str,"warning") ) return LOG_WARNING;
		else if( !strcmp(str,"error") ) return LOG_ERROR;
		else if( !strcmp(str,"disable") ) return 0xf;
		else return 0xF;
	}

private:

	//static const unsigned int categoryNumbers = 29;
	//unsigned int categoryMask;
	//unsigned int categoryFilterLevel[categoryNumbers];
	unsigned defaultFilterLevel;
	unsigned __int64 startTime;
	void (*externLog) (char *);
};