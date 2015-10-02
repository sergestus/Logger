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
class LogFromDllPolicyAdv
{
private:
	LogFromDllPolicyAdv():handle(NULL)
	{		
		mainInterface = NULL;
		created = true;
		const char *error = NULL;
		do
		{
			handle = LoadLibrary("log.dll");

			if( handle == NULL )
			{				
				error = "library error loading\n";
				break;
			}

			#define DEFINE_FUNCTION( ret, func, sign )	func = (func##Type)GetProcAddress(handle, #func);\
														if(func == NULL)\
														{\
															error = "function " #func " error loading\n";\
															break;\
														}
				DLL_FUNCTIONS
			#undef DEFINE_FUNCTION

			if( LOG_INTERFACE_VERSION > GetLogInterfaceVersion() ) 
			{				
				error = "dll version doesn't correspond to interface\n";;
				break;
			}	

			mainInterface = CreateLogObject();
			if( mainInterface == NULL )
			{
				error = "error creating log object\n";;
				break;
			}

		}while(0);		

		if( mainInterface == NULL )
		{
			created = false;
			mainInterface = new TLog<LogBaseConfigPolicy,LogDebugPolicy>();
			mainInterface->Log( LOG_ERROR, "Failed to create log object from dll: %s", error );			
		}
	}

	virtual ~LogFromDllPolicyAdv()
	{
		if(created)	ReleaseLogObject();				
		else delete (TLog<LogBaseConfigPolicy,LogDebugPolicy>*) mainInterface;
		if(handle) FreeLibrary(handle);
	}

	#define DEFINE_FUNCTION( ret, func, sign )	func##Type func;
		DLL_FUNCTIONS
	#undef DEFINE_FUNCTION


	ILog100 *mainInterface;         

	static char *GetMutexName()
	{
		return "log_creare_mutex";        
	}

	template <class CreatePolicy, class MainInterface, class NamedMutexObject> 
		friend class TSingleton;

	HINSTANCE handle;
	bool created;
};