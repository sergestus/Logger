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
#include "FileProfile.h"

#pragma warning(disable: 4995)
#pragma warning(disable: 4996) 

bool GetPrivateProfileBoolF( char *fileName, char *sectionName, char *keyName, char *defaultValue, bool *value )
{
	char str[256];
	if( GetPrivateProfileStringF( sectionName, keyName, defaultValue, str, sizeof(str), fileName ) )
	{
		if(!strcmp(str,"true")) *value = true;
		else if(!strcmp(str,"false")) *value = false;
		else return false;
		return true;
	}					
	return false;
}

void SetPrivateProfileBoolF( char *fileName, char *sectionName, char *keyName, bool value )
{
	WritePrivateProfileStringF( sectionName, keyName, value ? "true" : "false", fileName );
}

bool GetPrivateProfileIntF( char *fileName, char *sectionName, char *keyName, char *defaultValue, int *value )
{
	char str[256];
	if( GetPrivateProfileStringF( sectionName, keyName, defaultValue, str, sizeof(str), fileName ) )
	{		
		sscanf( str, "%d", value );
		return true;
	}					
	return false;
}

void SetPrivateProfileIntF( char *fileName, char *sectionName, char *keyName, int value )
{
	char str[256];
	sprintf( str, "%d", value );
	WritePrivateProfileStringF( sectionName, keyName, str, fileName );
}

bool GetPrivateProfileHexF( char *fileName, char *sectionName, char *keyName, char *defaultValue, int *value )
{
	char str[256];
	if( GetPrivateProfileStringF( sectionName, keyName, defaultValue, str, sizeof(str), fileName ) )
	{		
		sscanf( str, "%x", value );
		return true;
	}					
	return false;
}

void SetPrivateProfileHexF( char *fileName, char *sectionName, char *keyName, int value )
{
	char str[256];
	sprintf( str, "%x", value );
	WritePrivateProfileStringF( sectionName, keyName, str, fileName );
}

DWORD WritePrivateProfileStringF( char *section, char *keyName, char *value, char *fileName )
{
	char path[1024];
	GetCurrentDirectory( sizeof(path), path );
	strcat( path, "\\" );
	strcat( path, fileName );
	return ::WritePrivateProfileString( section, keyName, value, path );
}

DWORD GetPrivateProfileStringF( char *section, char *keyName, char *defaultValue, char *value, int size, char *fileName )
{
	char path[1024];
	GetCurrentDirectory( sizeof(path), path );
	strcat( path, "\\" );
	strcat( path, fileName );
	return ::GetPrivateProfileString( section, keyName, defaultValue, value, size, path );
}

bool ComparePrivateProfileStringF( char *section, char *keyName, char *defaultValue, char *value, char *fileName )
{
	char fileValue[512];
	GetPrivateProfileStringF( section, keyName, defaultValue, fileValue, sizeof(fileValue), fileName );
	return strcmp(value,fileValue) == 0;
}

bool ComparePrivateProfileIntF( char *fileName, char *sectionName, char *keyName, char *defaultValue, int value )
{
	int fileValue;
	char str[256];
	if( GetPrivateProfileStringF( sectionName, keyName, defaultValue, str, sizeof(str), fileName ) )
	{		
		sscanf( str, "%d", &fileValue );
		return value == fileValue;
	}					
	return false;
}

bool ComparePrivateProfileBoolF( char *fileName, char *sectionName, char *keyName, char *defaultValue, bool value )
{
	bool fileValue;
	char str[256];
	if( GetPrivateProfileStringF( sectionName, keyName, "0", str, sizeof(str), fileName ) )
	{
		if(!strcmp(str,"true")) fileValue = true;
		else if(!strcmp(str,"false")) fileValue = false;
		else return false;
		return fileValue == value;
	}					
	return false;
}
