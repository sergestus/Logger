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

#ifndef LOG_DISABLE

	#define LOG_INTERFACE_VERSION 100

	#define LOG_DEBUG	  0
	#define LOG_INFO	  1
	#define LOG_WARNING	  2
	#define LOG_ERROR	  3

	#define LOG_ENABLE_DEBUG
	#define LOG_ENABLE_INFO
	#define LOG_ENABLE_WARNING
	#define LOG_ENABLE_ERROR

	#ifdef LOG_RELEASE
		#define LOG_DISABLE_DEBUG
	#endif // LOG_RELEASE


	#ifdef LOG_DISABLE_DEBUG
		#undef LOG_ENABLE_DEBUG
	#endif // LOG_DISABLE_DEBUG

	#ifdef LOG_DISABLE_INFO
		#undef LOG_ENABLE_INFO
	#endif // LOG_DISABLE_INFO

	#ifdef LOG_DISABLE_WARNING
		#undef LOG_ENABLE_WARNING
	#endif // LOG_DISABLE_WARNING

	#ifdef LOG_DISABLE_ERROR
		#undef LOG_ENABLE_ERROR
	#endif // LOG_DISABLE_ERROR


	#ifdef LOG_ENABLE_DEBUG
		#ifdef LOG_CODE_LOCATION_INFO
			#define log_dbg(fmt,...) DefaultLog::GetSingletonObject()->Log( LOG_DEBUG, "%s:%s:%d: " fmt,__FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
		#else // LOG_CODE_LOCATION_INFO
			#define log_dbg(fmt,...) DefaultLog::GetSingletonObject()->Log( LOG_DEBUG, fmt, ##__VA_ARGS__)
		#endif // LOG_CODE_LOCATION_INFO
	#else // LOG_ENABLE_DEBUG
		#define log_dbg(fmt,...)
	#endif // LOG_ENABLE_DEBUG

	#ifdef LOG_ENABLE_INFO
		#ifdef LOG_CODE_LOCATION_INFO
			#define log_inf(fmt,...) DefaultLog::GetSingletonObject()->Log( LOG_INFO, "%s:%s:%d: " fmt,__FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
		#else // LOG_CODE_LOCATION_INFO
			#define log_inf(fmt,...) DefaultLog::GetSingletonObject()->Log( LOG_INFO, fmt, ##__VA_ARGS__)
		#endif // LOG_CODE_LOCATION_INFO
	#else // LOG_ENABLE_INFO
		#define log_inf(fmt,...)
	#endif // LOG_ENABLE_INFO
	
	#ifdef LOG_ENABLE_WARNING
		#ifdef LOG_CODE_LOCATION_INFO
			#define log_wrn(fmt,...) DefaultLog::GetSingletonObject()->Log( LOG_WARNING, "%s:%s:%d: " fmt,__FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
		#else // LOG_CODE_LOCATION_INFO
			#define log_wrn(fmt,...) DefaultLog::GetSingletonObject()->Log( LOG_WARNING, fmt, ##__VA_ARGS__)
		#endif // LOG_CODE_LOCATION_INFO
	#else // LOG_ENABLE_WARNING
		#define log_wrn(fmt,...)
	#endif // LOG_ENABLE_WARNING

	#ifdef LOG_ENABLE_ERROR
		#ifdef LOG_CODE_LOCATION_INFO
			#define log_err(fmt,...) DefaultLog::GetSingletonObject()->Log( LOG_ERROR, "%s:%s:%d: " fmt,__FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
		#else // LOG_CODE_LOCATION_INFO
			#define log_err(fmt,...) DefaultLog::GetSingletonObject()->Log( LOG_ERROR, fmt, ##__VA_ARGS__)
		#endif // LOG_CODE_LOCATION_INFO
	#else // LOG_ENABLE_ERROR
		#define log_err(fmt,...)
	#endif // LOG_ENABLE_ERROR

	#define log_redirect(fun) DefaultLog::GetSingletonObject()->RedirectLog(fun)
	#define log_release DefaultLog::ReleaseSingletonObject


	struct ILog100 
	{
		virtual void Log( unsigned int messageId, const char *fmt, ... ) = 0;  
		virtual void RedirectLog( void (*log) (char *)) = 0;
	};


	class DummyLog: public ILog100
	{
		public:
			virtual void Log( unsigned int messageId, const char *fmt, ... ){};  
			virtual void RedirectLog( void (*log) (char *)){};
			virtual ~DummyLog(){};
	};
		
	struct IConfig100
	{
		virtual bool GetString(char *section, char *name, char *value, int length, char *defval ) = 0;
		virtual bool GetUInteger(char *section, char *name, unsigned int *value, unsigned int defval ) = 0;
		virtual bool GetInteger(char *section, char *name, int *value, int defval ) = 0;
		virtual bool GetBoolean(char *section, char *name, bool *value, bool defval ) = 0;
	};

	template <class CreatePolicy, class MainInterface, class NamedMutexObject> class TSingleton: public CreatePolicy
	{
	private:
		static TSingleton *instance;

		TSingleton():CreatePolicy(){}
		TSingleton( const TSingleton& ){}
		TSingleton& operator=( TSingleton& ){}
		virtual ~TSingleton(){}

	public:
		static MainInterface* GetSingletonObject()
		{
			if(!instance)
			{
				NamedMutexObject mutex( CreatePolicy::GetMutexName() );		
				if(!instance) instance = new TSingleton();
			}
			return instance->mainInterface;
		}

		static void ReleaseSingletonObject()
		{
			if(instance)
			{
				NamedMutexObject mutex( CreatePolicy::GetMutexName() );		
				if(instance)
				{
					delete instance;
					instance = NULL;
				}
			}
		}
	};

	template <class CreatePolicy, class MainInterface, class NamedMutexObject> 
		TSingleton< CreatePolicy, MainInterface, NamedMutexObject > * 
		TSingleton< CreatePolicy, MainInterface, NamedMutexObject >::instance = 0;

	#ifndef LOG_DISABLE_DEFAULT_POLICIES

		#include <windows.h>

		class NamedMutexObject
		{

		public:
			NamedMutexObject( const char *mutexName )
			{
				mutex = ::CreateMutex( NULL, FALSE, mutexName );		
				WaitForSingleObject(mutex, INFINITE);
			}

			~NamedMutexObject()
			{
				ReleaseMutex(mutex);
			}

			HANDLE mutex;
		};
			

		#define DLL_FUNCTIONS\
			DEFINE_FUNCTION( int, GetLogInterfaceVersion, () )\
			DEFINE_FUNCTION( ILog100*, CreateLogObject, () )\
			DEFINE_FUNCTION( void, ReleaseLogObject, () )

		#define DEFINE_FUNCTION( ret, func, sign ) typedef ret (*func##Type) sign;
			DLL_FUNCTIONS
		#undef DEFINE_FUNCTION

		class LogFromDllPolicy;

		#define DefaultLog TSingleton< LogFromDllPolicy, ILog100, NamedMutexObject >
		
		class LogFromDllPolicy
		{
		private:
			LogFromDllPolicy():handle(NULL)
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
					mainInterface = new DummyLog();
					mainInterface->Log( LOG_ERROR, "Failed to create log object from dll: %s", error );			
				}
			}

			virtual ~LogFromDllPolicy()
			{
				if(created)	ReleaseLogObject();				
				else delete (DummyLog*) mainInterface;
				if(handle) FreeLibrary(handle);
			}

			#define DEFINE_FUNCTION( ret, func, sign )	func##Type func;
				DLL_FUNCTIONS
			#undef DEFINE_FUNCTION


			ILog100 *mainInterface;         

			static const char *GetMutexName()
			{
				return "log_creare_mutex";        
			}

			template <class CreatePolicy, class MainInterface, class NamedMutexObject> 
				friend class TSingleton;

			HINSTANCE handle;
			bool created;
		};
	#endif // LOG_DISABLE_DEFAULT_POLICIES

#else // LOG_DISABLE
	
	#define log_dbg(fmt,...) 
	#define log_inf(fmt,...)
	#define log_wrn(fmt,...)
	#define log_err(fmt,...)

	#define log_redirect(fun)

#endif // LOG_DISABLE


