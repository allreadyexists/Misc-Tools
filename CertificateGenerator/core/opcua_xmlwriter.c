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

#include <opcua.h>

#ifdef OPCUA_HAVE_XMLAPI

#include <opcua_stream.h>
#include <opcua_xmlwriter.h>

#define OPCUA_P_XMLWRITER_CREATE OpcUa_ProxyStub_g_PlatformLayerCalltable->CreateXmlWriter
#define OPCUA_P_XMLWRITER_DELETE OpcUa_ProxyStub_g_PlatformLayerCalltable->DeleteXmlWriter

/*============================================================================
 * OpcUa_XmlWriter_WriteCallback
 *===========================================================================*/
static OpcUa_StatusCode OpcUa_XmlWriter_WriteCallback(
    struct _OpcUa_XmlWriter*    a_pXmlWriter,
    OpcUa_Void*                 a_pWriteContext,
    OpcUa_Byte*                 a_pWriteBuffer,
    OpcUa_UInt32                a_uBufferLength)
{
    OpcUa_OutputStream* pOutputStream = OpcUa_Null;

    OpcUa_ReturnErrorIfArgumentNull(a_pXmlWriter);
    OpcUa_ReturnErrorIfArgumentNull(a_pWriteContext);

    pOutputStream = (OpcUa_OutputStream*)a_pWriteContext;

    return pOutputStream->Write(pOutputStream, a_pWriteBuffer, a_uBufferLength);
}

/*============================================================================
 * OpcUa_XmlWriter_CloseCallback
 *===========================================================================*/
static OpcUa_StatusCode OpcUa_XmlWriter_CloseCallback(
    struct _OpcUa_XmlWriter*    a_pXmlWriter,
    OpcUa_Void*                 a_pWriteContext)
{
    OpcUa_ReferenceParameter(a_pXmlWriter);
    OpcUa_ReferenceParameter(a_pWriteContext);

    return OpcUa_Good;
}

/*============================================================================
 * OpcUa_XmlWriter_Create
 *===========================================================================*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_XmlWriter_Create(
    struct _OpcUa_XmlWriter**   a_ppXmlWriter,
    struct _OpcUa_OutputStream* a_pOutputStream)
{
OpcUa_InitializeStatus(OpcUa_Module_XmlWriter, "OpcUa_XmlWriter_Create");

    OpcUa_ReturnErrorIfArgumentNull(a_ppXmlWriter);
    OpcUa_ReturnErrorIfArgumentNull(a_pOutputStream);

    *a_ppXmlWriter = (OpcUa_XmlWriter*)OpcUa_Alloc(sizeof(OpcUa_XmlWriter));
    OpcUa_GotoErrorIfAllocFailed(*a_ppXmlWriter);

    uStatus = OPCUA_P_XMLWRITER_CREATE(a_pOutputStream,
                                       OpcUa_XmlWriter_WriteCallback,
                                       OpcUa_XmlWriter_CloseCallback,
                                       *a_ppXmlWriter);
    OpcUa_GotoErrorIfBad(uStatus);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    OpcUa_Free(*a_ppXmlWriter);
    *a_ppXmlWriter = OpcUa_Null;

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_XmlWriter_Delete
 *===========================================================================*/
OPCUA_EXPORT OpcUa_Void OpcUa_XmlWriter_Delete(
    struct _OpcUa_XmlWriter**   a_ppXmlWriter)
{
    if(a_ppXmlWriter != OpcUa_Null && *a_ppXmlWriter != OpcUa_Null)
    {
        OPCUA_P_XMLWRITER_DELETE(*a_ppXmlWriter);
        OpcUa_Free(*a_ppXmlWriter);
        *a_ppXmlWriter = OpcUa_Null;
    }
}

/*============================================================================
 * OpcUa_XmlWriter_StartElement
 *===========================================================================*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_XmlWriter_StartElement(
    struct _OpcUa_XmlWriter*    a_pXmlWriter,
    OpcUa_StringA               a_sNamespacePrefix,
    OpcUa_StringA               a_sElementName,
    OpcUa_StringA               a_sNamespaceUri)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_XmlWriter);
    OpcUa_ReturnErrorIfArgumentNull(a_pXmlWriter);
    OpcUa_ReturnErrorIfNull(a_pXmlWriter->StartElement, OpcUa_BadNotSupported);

    return a_pXmlWriter->StartElement(a_pXmlWriter, a_sNamespacePrefix, a_sElementName, a_sNamespaceUri);
}

/*============================================================================
 * OpcUa_XmlWriter_EndElement
 *===========================================================================*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_XmlWriter_EndElement(
    struct _OpcUa_XmlWriter*    a_pXmlWriter)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_XmlWriter);
    OpcUa_ReturnErrorIfArgumentNull(a_pXmlWriter);
    OpcUa_ReturnErrorIfNull(a_pXmlWriter->EndElement, OpcUa_BadNotSupported);

    return a_pXmlWriter->EndElement(a_pXmlWriter);
}

/*============================================================================
 * OpcUa_XmlWriter_WriteAttribute
 *===========================================================================*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_XmlWriter_WriteAttribute(
    struct _OpcUa_XmlWriter*    a_pXmlWriter,
    OpcUa_StringA               a_sNamespacePrefix,
    OpcUa_StringA               a_sAttributeName,
    OpcUa_StringA               a_sNamespaceUri,
    OpcUa_StringA               a_sAttributeValue)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_XmlWriter);
    OpcUa_ReturnErrorIfArgumentNull(a_pXmlWriter);
    OpcUa_ReturnErrorIfNull(a_pXmlWriter->WriteAttribute, OpcUa_BadNotSupported);

    return a_pXmlWriter->WriteAttribute(a_pXmlWriter,
                                        a_sNamespacePrefix,
                                        a_sAttributeName,
                                        a_sNamespaceUri,
                                        a_sAttributeValue);
}

/*============================================================================
 * OpcUa_XmlWriter_WriteString
 *===========================================================================*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_XmlWriter_WriteString(
    struct _OpcUa_XmlWriter*    a_pXmlWriter,
    OpcUa_StringA               a_sValue)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_XmlWriter);
    OpcUa_ReturnErrorIfArgumentNull(a_pXmlWriter);
    OpcUa_ReturnErrorIfNull(a_pXmlWriter->WriteString, OpcUa_BadNotSupported);

    return a_pXmlWriter->WriteString(a_pXmlWriter, a_sValue);
}

/*============================================================================
 * OpcUa_XmlWriter_WriteFormatted
 *===========================================================================*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_XmlWriter_WriteFormatted(
    struct _OpcUa_XmlWriter*    a_pXmlWriter,
    OpcUa_StringA               a_sFormat,
                                ...)
{
    OpcUa_P_VA_List           arguments;

OpcUa_InitializeStatus(OpcUa_Module_XmlWriter, "OpcUa_XmlWriter_WriteFormatted");

    OpcUa_ReturnErrorIfArgumentNull(a_pXmlWriter);
    OpcUa_ReturnErrorIfNull(a_pXmlWriter->WriteFormatted, OpcUa_BadNotSupported);

    OPCUA_P_VA_START(arguments, a_sFormat);
    uStatus = a_pXmlWriter->WriteFormatted(a_pXmlWriter, a_sFormat, arguments);
    OPCUA_P_VA_END(arguments);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_XmlWriter_WriteRaw
 *===========================================================================*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_XmlWriter_WriteRaw(
    struct _OpcUa_XmlWriter*    a_pXmlWriter,
    OpcUa_Byte*                 a_pRawData,
    OpcUa_UInt32                a_uDataLength)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_XmlWriter);
    OpcUa_ReturnErrorIfArgumentNull(a_pXmlWriter);
    OpcUa_ReturnErrorIfNull(a_pXmlWriter->WriteRaw, OpcUa_BadNotSupported);

    return a_pXmlWriter->WriteRaw(a_pXmlWriter, a_pRawData, a_uDataLength);
}

/*============================================================================
 * OpcUa_XmlWriter_WriteNode
 *===========================================================================*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_XmlWriter_WriteNode(
    struct _OpcUa_XmlWriter*     a_pXmlWriter,
    struct _OpcUa_XmlReader*     a_pXmlReader)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_XmlWriter);
    OpcUa_ReturnErrorIfArgumentNull(a_pXmlWriter);
    OpcUa_ReturnErrorIfNull(a_pXmlWriter->WriteNode, OpcUa_BadNotSupported);

    return a_pXmlWriter->WriteNode(a_pXmlWriter, a_pXmlReader);
}

/*============================================================================
 * OpcUa_XmlWriter_Flush
 *===========================================================================*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_XmlWriter_Flush(
    struct _OpcUa_XmlWriter*    a_pXmlWriter)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_XmlWriter);
    OpcUa_ReturnErrorIfArgumentNull(a_pXmlWriter);
    OpcUa_ReturnErrorIfNull(a_pXmlWriter->Flush, OpcUa_BadNotSupported);

    return a_pXmlWriter->Flush(a_pXmlWriter);
}

/*============================================================================
 * OpcUa_XmlWriter_Close
 *===========================================================================*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_XmlWriter_Close(
    struct _OpcUa_XmlWriter*    a_pXmlWriter)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_XmlWriter);
    OpcUa_ReturnErrorIfArgumentNull(a_pXmlWriter);
    OpcUa_ReturnErrorIfNull(a_pXmlWriter->Close, OpcUa_BadNotSupported);

    return a_pXmlWriter->Close(a_pXmlWriter);
}

#endif /* OPCUA_HAVE_XMLAPI */
