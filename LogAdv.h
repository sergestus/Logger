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

#include "Log.h"
#include "TLog.h"
#include "LogBaseConfigPolicy.h"
#include "LogDebugPolicy.h"
#include "LogFromDllPolicyAdv.h"

#undef DefaultLog
#define DefaultLog TSingleton< LogFromDllPolicyAdv, ILog100, NamedMutexObject >