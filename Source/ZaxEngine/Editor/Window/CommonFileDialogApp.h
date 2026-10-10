#pragma once
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//

#include <string>

 // 打开选择窗口
int CommonFileDialog();

// 打开目录选择窗口
bool OpenFolderDialog(std::string& path);

// 打开文件选择窗口，自己补充的
bool OpenFileDialog(std::string& path);
