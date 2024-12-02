#pragma once
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//

#include <windows.h>      // For common windows data types and function headers
#define STRICT_TYPED_ITEMIDS
#include <shlobj.h>
#include <objbase.h>      // For COM headers
#include <shobjidl.h>     // for IFileDialogEvents and IFileDialogControlEvents
#include <shlwapi.h>
#include <knownfolders.h> // for KnownFolder APIs/datatypes/function headers
#include <propvarutil.h>  // for PROPVAR-related functions
#include <propkey.h>      // for the Property key APIs/datatypes
#include <propidl.h>      // for the Property System APIs
#include <strsafe.h>      // for StringCchPrintfW
#include <shtypes.h>      // for COMDLG_FILTERSPEC
#include <new>
#include <string>

#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

const COMDLG_FILTERSPEC c_rgSaveTypes[] =
{
    {L"Word Document (*.doc)",       L"*.doc"},
    {L"Web Page (*.htm; *.html)",    L"*.htm;*.html"},
    {L"Text Document (*.txt)",       L"*.txt"},
    {L"All Documents (*.*)",         L"*.*"}
};

// Indices of file types
#define INDEX_WORDDOC 1
#define INDEX_WEBPAGE 2
#define INDEX_TEXTDOC 3

// Controls
#define CONTROL_GROUP           2000
#define CONTROL_RADIOBUTTONLIST 2
#define CONTROL_RADIOBUTTON1    1
#define CONTROL_RADIOBUTTON2    2       // It is OK for this to have the same ID as CONTROL_RADIOBUTTONLIST,
                                        // because it is a child control under CONTROL_RADIOBUTTONLIST

// IDs for the Task Dialog Buttons
#define IDC_BASICFILEOPEN                       100
#define IDC_ADDITEMSTOCUSTOMPLACES              101
#define IDC_ADDCUSTOMCONTROLS                   102
#define IDC_SETDEFAULTVALUESFORPROPERTIES       103
#define IDC_WRITEPROPERTIESUSINGHANDLERS        104
#define IDC_WRITEPROPERTIESWITHOUTUSINGHANDLERS 105

/* File Dialog Event Handler *****************************************************************************************************/

class CDialogEventHandler : public IFileDialogEvents,
    public IFileDialogControlEvents
{
public:
    // IUnknown methods
    IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv)
    {
        static const QITAB qit[] = {
            QITABENT(CDialogEventHandler, IFileDialogEvents),
            QITABENT(CDialogEventHandler, IFileDialogControlEvents),
            { 0 },
#pragma warning(suppress:4838)
        };
        return QISearch(this, qit, riid, ppv);
    }

    IFACEMETHODIMP_(ULONG) AddRef()
    {
        return InterlockedIncrement(&_cRef);
    }

    IFACEMETHODIMP_(ULONG) Release()
    {
        long cRef = InterlockedDecrement(&_cRef);
        if (!cRef)
            delete this;
        return cRef;
    }

    // IFileDialogEvents methods
    IFACEMETHODIMP OnFileOk(IFileDialog*) { return S_OK; };
    IFACEMETHODIMP OnFolderChange(IFileDialog*) { return S_OK; };
    IFACEMETHODIMP OnFolderChanging(IFileDialog*, IShellItem*) { return S_OK; };
    IFACEMETHODIMP OnHelp(IFileDialog*) { return S_OK; };
    IFACEMETHODIMP OnSelectionChange(IFileDialog*) { return S_OK; };
    IFACEMETHODIMP OnShareViolation(IFileDialog*, IShellItem*, FDE_SHAREVIOLATION_RESPONSE*) { return S_OK; };
    IFACEMETHODIMP OnTypeChange(IFileDialog* pfd);
    IFACEMETHODIMP OnOverwrite(IFileDialog*, IShellItem*, FDE_OVERWRITE_RESPONSE*) { return S_OK; };

    // IFileDialogControlEvents methods
    IFACEMETHODIMP OnItemSelected(IFileDialogCustomize* pfdc, DWORD dwIDCtl, DWORD dwIDItem);
    IFACEMETHODIMP OnButtonClicked(IFileDialogCustomize*, DWORD) { return S_OK; };
    IFACEMETHODIMP OnCheckButtonToggled(IFileDialogCustomize*, DWORD, BOOL) { return S_OK; };
    IFACEMETHODIMP OnControlActivating(IFileDialogCustomize*, DWORD) { return S_OK; };

    CDialogEventHandler() : _cRef(1) {};
private:
    ~CDialogEventHandler() {};
    long _cRef;
};

// Instance creation helper
HRESULT CDialogEventHandler_CreateInstance(REFIID riid, void** ppv);

/* Utility Functions *************************************************************************************************************/

// A helper function that converts UNICODE data to ANSI and writes it to the given file.
// We write in ANSI format to make it easier to open the output file in Notepad.
HRESULT _WriteDataToFile(HANDLE hFile, PCWSTR pszDataIn);

// Helper function to write property/value into a custom file format.
//
// We are inventing a dummy format here:
// [APPDATA]
// xxxxxx
// [ENDAPPDATA]
// [PROPERTY]foo=bar[ENDPROPERTY]
// [PROPERTY]foo2=bar2[ENDPROPERTY]
HRESULT _WritePropertyToCustomFile(PCWSTR pszFileName, PCWSTR pszPropertyName, PCWSTR pszValue);

// Helper function to write dummy content to a custom file format.
//
// We are inventing a dummy format here:
// [APPDATA]
// xxxxxx
// [ENDAPPDATA]
// [PROPERTY]foo=bar[ENDPROPERTY]
// [PROPERTY]foo2=bar2[ENDPROPERTY]
HRESULT _WriteDataToCustomFile(PCWSTR pszFileName);

/* Common File Dialog Snippets ***************************************************************************************************/

// This code snippet demonstrates how to work with the common file dialog interface
HRESULT BasicFileOpen();

// The Common Places area in the File Dialog is extensible.
// This code snippet demonstrates how to extend the Common Places area.
// Look at CDialogEventHandler::OnItemSelected to see how messages pertaining to the added
// controls can be processed.
HRESULT AddItemsToCommonPlaces();

// This code snippet demonstrates how to add custom controls in the Common File Dialog.
HRESULT AddCustomControls();

// This code snippet demonstrates how to add default metadata in the Common File Dialog.
// Look at CDialogEventHandler::OnTypeChange to see to change the order/list of properties
// displayed in the Common File Dialog.
HRESULT SetDefaultValuesForProperties();

// The following code snippet demonstrates two things:
// 1.  How to write properties using property handlers.
// 2.  Replicating properties in the "Save As" scenario where the user choses to save an existing file
//     with a different name.  We need to make sure we replicate not just the data,
//     but also the properties of the original file.
HRESULT WritePropertiesUsingHandlers();

// This code snippet demonstrates how to write properties without using property handlers.
HRESULT WritePropertiesWithoutUsingHandlers();

 // 打开选择窗口
int CommonFileDialog();

bool OpenFolderDialog(std::string& path);
