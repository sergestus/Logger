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

#include <stdio.h>


class LogFilePolicy 
{
public:
	LogFilePolicy( IConfig100 *iConfig )
	{
		char fname[1024];		
		if( iConfig->GetString("common","appname",fname,sizeof(fname),"unknown") == false )
			strcpy_s(fname,"unknown");
		strcat_s(fname,".log");
		errno_t error = fopen_s(&fp,fname,"a");
		if( error ) throw error;
	}

	virtual ~LogFilePolicy()
	{
		fclose(fp);
	}

	void Write( char *out )
	{
		fprintf(fp,out);
		fflush(fp);
	}

	unsigned __int64 GetTimeMs()
	{
		return GetTickCount64();
	}

protected:
	FILE *fp;
};
