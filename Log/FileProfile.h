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

#ifndef __FileProfile_h__
#define __FileProfile_h__

bool GetPrivateProfileBoolF( char *fileName, char *sectionName, char *keyName, char *defaultValue, bool *value );
void SetPrivateProfileBoolF( char *fileName, char *sectionName, char *keyName, bool value );
bool GetPrivateProfileIntF( char *fileName, char *sectionName, char *keyName, char *defaultValue, int *value );
void SetPrivateProfileIntF( char *fileName, char *sectionName, char *keyName, int value );
bool GetPrivateProfileHexF( char *fileName, char *sectionName, char *keyName, char *defaultValue, int *value );
void SetPrivateProfileHexF( char *fileName, char *sectionName, char *keyName, int value );
DWORD WritePrivateProfileStringF( char *section, char *keyName, char *value, char *fileName );
DWORD GetPrivateProfileStringF( char *section, char *keyName, char *defaultValue, char *value, int size, char *fileName );
bool ComparePrivateProfileStringF( char *section, char *keyName, char *defaultValue, char *value, char *fileName );
bool ComparePrivateProfileIntF( char *fileName, char *sectionName, char *keyName, char *defaultValue, int value );
bool ComparePrivateProfileBoolF( char *fileName, char *sectionName, char *keyName, char *defaultValue, bool value );

#endif // __FileProfile_h__