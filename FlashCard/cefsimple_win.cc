// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <windows.h>

#include "include/cef_command_line.h"
#include "include/cef_sandbox_win.h"
#include "./simple_app.h"
#include "./renderProcessHandler.h"
#include "./MainContext.h"

// When generating projects with CMake the CEF_USE_SANDBOX value will be defined
// automatically if using the required compiler version. Pass -DUSE_SANDBOX=OFF
// to the CMake command-line to disable use of the sandbox.
// Uncomment this line to manually enable sandbox support.
// #define CEF_USE_SANDBOX 1

#if defined(CEF_USE_SANDBOX)
// The cef_sandbox.lib static library may not link successfully with all VS
// versions.
#pragma comment(lib, "cef_sandbox.lib")
#endif

// Entry point function for all processes.
int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPTSTR lpCmdLine,
                      int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // Enable High-DPI support on Windows 7 or newer.
  CefEnableHighDPISupport();

  void* sandbox_info = nullptr;

#if defined(CEF_USE_SANDBOX)
  // Manage the life span of the sandbox information object. This is necessary
  // for sandbox support on Windows. See cef_sandbox_win.h for complete details.
  CefScopedSandboxInfo scoped_sandbox;
  sandbox_info = scoped_sandbox.sandbox_info();
#endif

  // Provide CEF with command-line arguments.
  CefMainArgs main_args(hInstance);
  CefRefPtr<CefApp> render_app = new RenderProcessHandler();
  // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
  // that share the same executable. This function checks the command-line and,
  // if this is a sub-process, executes the appropriate logic.
  int exit_code = CefExecuteProcess(main_args, render_app.get(), nullptr);
  if (exit_code >= 0) {
      // The sub-process has completed so return here.
      return exit_code;
  }
  auto context = std::make_unique<MainContext>(true);
  // Parse command-line arguments for use in this method.
  CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
  command_line->InitFromString(::GetCommandLineW());

  // Specify CEF global settings here.
  CefSettings settings;
  if (command_line->HasSwitch("enable-chrome-runtime")) {
      // Enable experimental Chrome runtime. See issue #2969 for details.
      settings.chrome_runtime = true;
  }
  
#if !defined(CEF_USE_SANDBOX)
  settings.no_sandbox = true;
#endif
  context->Init(main_args, settings, render_app, nullptr);
  // SimpleApp implements application-level callbacks for the browser process.
  // It will create the first browser instance in OnContextInitialized() after
  // CEF has initialized.
  /*
  CefRefPtr<SimpleApp> app(new SimpleApp);
   
  // Initialize CEF.
  if (CefInitialize(main_args, settings, app.get(), sandbox_info)) {
      app->OnContextInitialized();
      // Run the CEF message loop. This will block until CefQuitMessageLoop() is
      // called.
  }
  */
  // Shut down CEF.
  CefRunMessageLoop();

  CefShutdown();

  return 0;
}
