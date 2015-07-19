                         LOG C++ LIBRARY

1. INSTALL

Just include log.h into your project. If you need configurable log build 
log.dll and copy it into the current application directory. If you need 
special log implement it in the log.dll.


2. PLATFORMS

Winodws Visual Studio 2010.


3. FEATURES

3.1 FULLY CONFIGURABLE

There are two types of settings: compile-time and run-time. The library 
core locates in a dll library, what makes the log mechanism independable 
from the application.

3.2 EASE OF USE

You need to include into your project only log.h. You need not add any 
preprocessor definitions to enable logging.

3.3 EXTENSIBILITY

The main logger class based on policies. By creating and composing policies 
in the main logger class you can achieve the functionality you need. The 
modifications do not affect on the application because library code is in 
a separate dll. 


4. COMPILE-TIME SETTINGS

4.1 APPLICATION

By default all types of log messages enabled. Below is the preppocessor 
settings, which may be used in an applicaion:
  LOG_DISABLE - disables all types of messages.
  LOG_RELEASE - disables debug messages.
  LOG_DISABLE_DEFAULT_POLICIES - disables default policies in the log.h. It 
    may be useful when using for different than windows platform.
  LOG_DISABLE_DEBUG, LOG_DISABLE_INFO, LOG_DISABLE_WARNING, LOG_DISABLE_ERROR - 
    disables corresponding messages.

4.2 LIBRARY

The library settings define policies, which use the logger:
  LOG_DEBUG_POLICY – library writes messages into the standard windows log.
  LOG_FILE_POLICY – library writes messages into a file.


5. RUN-TIME SETTINGS

The run-time settings are defined in the current config policy. The policy read 
settings from an ini file, which name is «application_name_log.ini» and location 
is a current application directory. For the current moment there is only one setting 
was implemented: it is «filterLevel» – defines which messages are enabled, posible 
values: debug (default), info, warning, error, disable.


6. USING

6.1 APPLICATION

You need include only log.h into your project. You need not add any preprocessor 
definitions to enable logging. There are macroses below, which may be used in the 
application:
  log_dbg(…) – writes a debug message,
  log_inf(…)– writes an information message,
  log_wrn(…)– writes a warning message,
  log_err(…)– writes an error message,
  log_redirect(void (*fun)(char*))– redirects messages into an user log,
  log_release() - stops and deletes the log object.

6.2 LOG.DLL

The library works so, that when application first time output a message to the log, 
log.dll is loaded and logger object is created. If there is no log.dll in the current 
directory, then all messages write to the standard windows log (LOG_DEBUG_POLICY is 
using).


7. CLASSES

7.1 THE MAIN LOGGER CLASS

template < class LogConfigPolicy, class LogPolicy > class TLog;
parameters:
  LogConfigPolicy – a logger config policy, 
  LogPolicy - a logger policy.

7.2 SINGLETON

template <class CreatePolicy, class MainInterface, class NamedMutexObject> class TSingleton;
This is a class, which is using for creating logger object in an appliction, 
parameters:
  CreatePolicy – create and destroy logger policies,
  MainInterface – logger interface,
  NamedMutexObject – synchronization class.


8. INTERFACES

  ILog100 - logger interface,
  IConfig100 - config interface.


9. POLICIES
 
  LogBaseConfigPolicy – base config policy (defines only an application name).
  LogFileConfigPolicy – config policy, which is reading settings from ini file, 
   which is locating in the current directory.
  LogDebugPolicy – logger policy, writing messages in the standard windows log.
  LogFilePolicy – logger policy, writing messages in a file, in the current 
   directory with the same as application name, with extension log.
  LogFromDllPolicy – policy loading and creating logger from dll.
