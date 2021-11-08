// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_

#include "include/cef_app.h"
#include "include/cef_browser_process_handler.h"
#include "./simple_handler.h"
#include "./DeckHandler.h"


// Implement application-level callbacks for the browser process.
class BrowserApp : public CefApp, public CefBrowserProcessHandler {
public:
    BrowserApp() {}

    // CefApp methods:
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override {
        return this;
    }

    void OnBeforeCommandLineProcessing(
        const CefString& process_type,
        CefRefPtr<CefCommandLine> command_line) override {
        // Command-line flags can be modified in this callback.
        // |process_type| is empty for the browser process.
        if (process_type.empty()) {
            #if defined(OS_MACOSX)
            // Disable the macOS keychain prompt. Cookies will not be encrypted.
            command_line->AppendSwitch("use-mock-keychain");
            #endif
        }
    }

    // CefBrowserProcessHandler methods:
    void OnContextInitialized() override {
        // Create the browser window.

    }

private:
    IMPLEMENT_REFCOUNTING(BrowserApp);
    DISALLOW_COPY_AND_ASSIGN(BrowserApp);
};

    
#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
