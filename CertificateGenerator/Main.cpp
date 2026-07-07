/* ========================================================================
 * Copyright (c) 2005-2026, OPC Federation AISBL, All rights reserved.
 *
 * OPC Foundation MIT License 1.00
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * The complete license agreement can be found here:
 * http://opcfoundation.org/License/MIT/1.00/
 * ======================================================================*/
/* System */
#define WIN32_LEAN_AND_MEAN
#include "targetver.h"
#include <windows.h>
#include <stdio.h>
#include <errno.h>
#include <conio.h>
#include <crtdbg.h>
#include <direct.h>
#include <mbstring.h>
#include <string>
#include <map>
#include <vector>
#include <opcua.h>
#include "StatusCodeException.h"
#include "CommandLineArgs.h"
#include "Application.h"

extern OpcUa_StatusCode OpcUa_StringToUnicode(
    OpcUa_StringA a_sSource,
    OpcUa_Char**  a_pUnicode);

int wmain(int argc, wchar_t* argv[])
{
	CommandLineArgs args;
    Application application;

    try
	{
		#if _DEBUG
		MessageBoxW(0, argv[1], 0, 0);
		#endif


        application.Initialize();
		
		if (!args.ProcessCommandLine(argc, argv))
		{
			args.WriteOutput();
			return 0;
		}

		int result = 0;
		OpcUa_StatusCode uStatus = OpcUa_Good;
		
		if (args.StorePath.length() > 0)
		{
			OpcUa_Char* wszFilePath = 0;
			uStatus = OpcUa_StringToUnicode((OpcUa_StringA)args.StorePath.c_str(), &wszFilePath);

			if (OpcUa_IsBad(uStatus))
			{
				args.OutputParameters["-error"] = "Could not access certificate store.";
				args.OutputParameters["-storePath"] = args.StorePath;
				args.WriteOutput();
				return 0;
			}

			// create the store.
			result = _wmkdir((wchar_t*)wszFilePath);

			if (result != 0)
			{
				result = errno;
			}

			OpcUa_Free(wszFilePath);
			wszFilePath = 0;
		}

        if (result != 0 && result != EEXIST)
        {
			if (_strnicmp(args.StorePath.c_str(), "LocalMachine", strlen("LocalMachine")) != 0 && _strnicmp(args.StorePath.c_str(), "CurrentUser", strlen("CurrentUser")) != 0)
			{
				args.OutputParameters["-error"] = "Could not access certificate store.";
				args.OutputParameters["-storePath"] = args.StorePath;
				args.WriteOutput();
				return 0;
            }
        }

        // create a new certificate.
		if (args.Command.empty() || args.Command == "issue")
        {
			application.Issue(args);
			args.WriteOutput();
            return 0;
        }

        // revoke a certificate
		if (args.Command == "revoke" || args.Command == "unrevoke")
		{
			application.Revoke(args);
			args.WriteOutput();
			return 0;
        }

        // convert a certificate
		if (args.Command == "convert" || args.Command == "install")
		{
			application.Convert(args);
			args.WriteOutput();
			return 0;
		}

		// change certificate password
		if (args.Command == "password")
		{
			application.Convert(args);
			args.WriteOutput();
			return 0;
		}

		// convert a replace
		if (args.Command == "replace")
		{
			application.Replace(args);
			args.WriteOutput();
			return 0;
		}

		// create a certificate request.
		if (args.Command == "request")
		{
			application.CreateRequest(args);
			args.WriteOutput();
			return 0;
		}

		// process a certificate request.
		if (args.Command == "process")
		{
			application.ProcessRequest(args);
			args.WriteOutput();
			return 0;
		}

		args.OutputParameters["-error"] = "Unsupported command.";
		args.OutputParameters["-command"] = args.Command;
		args.WriteOutput();
    }
    catch (StatusCodeException e)
	{
		args.OutputParameters["-error"] = e.GetMessage();

		try
		{
			args.WriteOutput();
		}
		catch (...)
		{
			// ignore.
		}
    }
    catch (...)
	{
		args.OutputParameters["-error"] = "Unhandled exception.";

		try
		{
			args.WriteOutput();
		}
		catch (...)
		{
			// ignore.
		}
    }

    application.Uninitialize();
    
    return 0;
}
