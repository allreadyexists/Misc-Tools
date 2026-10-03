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

/* System Headers */
#include <windows.h>
#include <time.h>

/* UA platform definitions */
#include <opcua.h>

/*============================================================================
* CreateGuid
*===========================================================================*/
OpcUa_Guid* OpcUa_P_Guid_Create(OpcUa_Guid* Guid)
{
	if (UuidCreate((UUID*)Guid) != RPC_S_OK)
	{
		Guid = OpcUa_Null;
		return OpcUa_Null;
	}

	return Guid;
}

/*============================================================================
* Calculate DateTime Difference In Seconds (Rounded)
*===========================================================================*/
OpcUa_StatusCode OpcUa_P_GetDateTimeDiffInSeconds32(
	OpcUa_DateTime  a_Value1,
	OpcUa_DateTime  a_Value2,
	OpcUa_Int32*    a_pDifference)
{
	INT64 llValue1 = 0;
	INT64 llValue2 = 0;
	INT64 llResult = 0;

	OpcUa_ReturnErrorIfArgumentNull(a_pDifference);

	*a_pDifference = (OpcUa_Int32)0;

	llValue1 = a_Value1.dwHighDateTime;
	llValue1 = (llValue1 << 32) + a_Value1.dwLowDateTime;

	llValue2 = a_Value2.dwHighDateTime;
	llValue2 = (llValue2 << 32) + a_Value2.dwLowDateTime;

	llResult = llValue2 - llValue1;
	llResult /= 10000000;

	if (llResult < OpcUa_Int32_Min || llResult > OpcUa_Int32_Max)
	{
		return OpcUa_BadOutOfRange;
	}

	*a_pDifference = (OpcUa_Int32)llResult;

	return OpcUa_Good;
}

/*============================================================================
* The OpcUa_UtcNow function (returns the time in OpcUa_DateTime format)
*===========================================================================*/
OpcUa_DateTime OpcUa_P_DateTime_UtcNow()
{
	FILETIME ftTime;

	OpcUa_DateTime tmpDateTime;

	GetSystemTimeAsFileTime(&ftTime);

	tmpDateTime.dwHighDateTime = (OpcUa_UInt32)ftTime.dwHighDateTime;
	tmpDateTime.dwLowDateTime = (OpcUa_UInt32)ftTime.dwLowDateTime;

	return tmpDateTime;
}
