// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "./simple_app.h"

#include <string>

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "./simple_handler.h"

/*

Remove most functions and replace it a different way
to initialize everything with browser view.

*/

SimpleApp::SimpleApp() {
}

void SimpleApp::OnContextInitialized() {
    CEF_REQUIRE_UI_THREAD();

}

CefRefPtr<CefClient> SimpleApp::GetDefaultClient() {
    // Called when a new browser window is created via the Chrome runtime UI.

    return SimpleHandler::GetInstance();
}


/*
void SimpleApp::OnBeforeCommandLineProcessing(
    const CefString& process_type,
    CefRefPtr<CefCommandLine> command_line) {
    
    if (process_type.empty()) {
#if defined(OS_MACOSX)
        // Disable the macOS keychain prompt. Cookies will not be encrypted.
        command_line->AppendSwitch("use-mock-keychain");
#endif
    }

}*/