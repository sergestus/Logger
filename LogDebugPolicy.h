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

class LogDebugPolicy 
{
public:
	LogDebugPolicy(IConfig100 *iConfig)
	{
		if( iConfig->GetString("common","appName",prefix,sizeof(prefix),"unknown") == false )
			strcpy_s(prefix,"unknown");
		strcat_s(prefix,": ");
	}

	virtual ~LogDebugPolicy()
	{
	}

	void Write( char *out )
	{
		OutputDebugString( prefix );
		OutputDebugString( out );
	}

	unsigned __int64 GetTimeMs()
	{
		#ifdef ENABLE_FULL_WINAPI
			return GetTickCount64();
		#else
			return (__int64) GetTickCount();
		#endif	// ENABLE_FULL_WINAPI
	}


protected:
	char prefix[512];
};
