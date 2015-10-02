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