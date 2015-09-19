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

#ifndef LOG_DISABLE

	#include <stdio.h>	
	#include <stdarg.h>	
	#include <time.h>	
	#include <typeinfo>
	//#include <intrin.h> 

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
		virtual void Log( unsigned int messageId, char *fmt, ... ) = 0;  
		virtual void RedirectLog( void (*log) (char *)) = 0;
	};

	struct IConfig100
	{
		virtual bool GetString(char *section, char *name, char *value, int length, char *defval ) = 0;
		virtual bool GetUInteger(char *section, char *name, unsigned int *value, unsigned int defval ) = 0;
		virtual bool GetInteger(char *section, char *name, int *value, int defval ) = 0;
		virtual bool GetBoolean(char *section, char *name, bool *value, bool defval ) = 0;
	};

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

		virtual void Log( unsigned int messageId, char *fmt, ... )
		{
			char out[1024]; 
			if( FilterMessage(messageId) ) return;

			unsigned __int64 delta = GetTimeMs()-startTime; 
			int length = sprintf_s( out, sizeof(out), "%.2llu:%.2llu:%.2llu:%.3llu:%s: ", 
				delta/3600000, (delta/60000)%60, (delta/1000)%60, delta%1000, GetMessgeTypeString(0xF&messageId) );
			
			va_list body;
			va_start(body, fmt);
			vsprintf_s( out+length, sizeof(out)-length, fmt, body );
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
			int length = sprintf_s( out, comment );
			time_t now = time(0);
			ctime_s( out+length, sizeof(out)-length, &now );
			if(externLog) externLog( out );
			else Write( out );

			sprintf_s( out, "Log Policy: %s\n", typeid(LogPolicy).name() );
			if(externLog) externLog( out );
			else Write( out );

			sprintf_s( out, "Log Config Policy: %s\n", typeid(LogConfigPolicy).name() );
			if(externLog) externLog( out );
			else Write( out );

			sprintf_s( out, "Log filterLevel = %s(0x%x)\n", 
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
			NamedMutexObject(char *mutexName)
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

		class LogBaseConfigPolicy : public IConfig100
		{
		public:
			LogBaseConfigPolicy()
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
			}

			virtual ~LogBaseConfigPolicy()
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
					//if(!_stricmp(name,"filterLevel"))
					//{
					//	strcpy_s(value, length, "info");
					//	return true;
					//}					
				}
				return false;
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
		};

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

		#define DLL_FUNCTIONS\
			DEFINE_FUNCTION( int, GetLogInterfaceVersion, () )\
			DEFINE_FUNCTION( ILog100*, CreateLogObject, () )\
			DEFINE_FUNCTION( void, ReleaseLogObject, () )

		#define DEFINE_FUNCTION( ret, func, sign ) typedef ret (*func##Type) sign;
			DLL_FUNCTIONS
		#undef DEFINE_FUNCTION

		class LogFromDllPolicy;

		typedef TSingleton< LogFromDllPolicy, ILog100, NamedMutexObject > DefaultLog;

		class LogFromDllPolicy
		{
		private:
			LogFromDllPolicy():handle(NULL)
			{		
				mainInterface = NULL;
				created = true;
				char *error = NULL;
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

			virtual ~LogFromDllPolicy()
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

			unsigned __int64 GetTimeMs()
			{
				return GetTickCount64();
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


