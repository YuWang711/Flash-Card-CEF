// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "./simple_handler.h"
#include "./MessageRouter.h"

#include <sstream>
#include <string>

#include "include/base/cef_callback.h"
#include "include/cef_parser.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include "include/cef_command_line.h"

namespace {

SimpleHandler* g_instance = nullptr;

// Returns a data: URI with the specified contents.
std::string GetDataURI(const std::string& data, const std::string& mime_type) {
  return "data:" + mime_type + ";base64," +
         CefURIEncode(CefBase64Encode(data.data(), data.size()), false)
             .ToString();
}

}  // namespace

SimpleHandler::SimpleHandler(Delegate* delegate,bool use_views)
   : use_views_(use_views), is_closing_(false), delegate_(delegate) {
     DCHECK(!g_instance);
     g_instance = this;

}

SimpleHandler::~SimpleHandler() {
 g_instance = nullptr;
}

// static
SimpleHandler* SimpleHandler::GetInstance() {
  return g_instance;
}

void SimpleHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                  const CefString& title) {
  CEF_REQUIRE_UI_THREAD();

  if (use_views_) {
    // Set the title of the window using the Views framework.
    CefRefPtr<CefBrowserView> browser_view =
        CefBrowserView::GetForBrowser(browser);
    if (browser_view) {
      CefRefPtr<CefWindow> window = browser_view->GetWindow();
      if (window)
        window->SetTitle(title);
    }
  } else if (!IsChromeRuntimeEnabled()) {
    // Set the title of the window using platform APIs.
    PlatformTitleChange(browser, title);
  }
}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();
  

  if (message_router == nullptr) {
      CefMessageRouterConfig config;
      config.js_query_function = "cefQuery";
      config.js_cancel_function = "cefQueryCancel";
      message_router = CefMessageRouterBrowserSide::Create(config);
      message_handler = new Message_Handler();
      message_router->AddHandler(message_handler, true);
  }

  // Add to the list of existing browsers.
  browser_list_.push_back(browser);
}

bool SimpleHandler::DoClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Closing the main window requires special handling. See the DoClose()
  // documentation in the CEF header for a detailed destription of this
  // process.
  if (browser_list_.size() == 1) {
    // Set a flag to indicate that the window close should be allowed.
    is_closing_ = true;
  }

  // Allow the close. For windowed browsers this will result in the OS close
  // event being sent.
  return false;
}

void SimpleHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Remove from the list of existing browsers.
  BrowserList::iterator bit = browser_list_.begin();
  for (; bit != browser_list_.end(); ++bit) {
    if ((*bit)->IsSame(browser)) {
      browser_list_.erase(bit);
      break;
    }
  }
  
  if (browser_list_.empty()) {
    // All browser windows have closed. Quit the application message loop.
      message_router->RemoveHandler(message_handler);
      message_router->Release();
      CefQuitMessageLoop();
  }
}

CefRefPtr<CefResourceRequestHandler> SimpleHandler::GetResourceRequestHandler(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request,
    bool is_navigation,
    bool is_download,
    const CefString& request_initiator,
    bool& disable_default_handling) {

    return this;
}

bool SimpleHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefFrame> frame,
                                            CefProcessId source_process,
                                            CefRefPtr<CefProcessMessage> message) {
    CEF_REQUIRE_UI_THREAD();

    bool testing = message_router->OnProcessMessageReceived(browser, frame,
        source_process, message);
    return testing;
}

void SimpleHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
    TerminationStatus status) {
    CEF_REQUIRE_UI_THREAD();

    message_router->OnRenderProcessTerminated(browser);
}

bool SimpleHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect)
{
    CEF_REQUIRE_UI_THREAD();
    message_router->OnBeforeBrowse(browser, frame);
    return false;
}

void SimpleHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                ErrorCode errorCode,
                                const CefString& errorText,
                                const CefString& failedUrl) {
    CEF_REQUIRE_UI_THREAD();

  // Allow Chrome to show the error page.
  if (IsChromeRuntimeEnabled())
    return;

  // Don't display an error for downloaded files.
  if (errorCode == ERR_ABORTED)
    return;

  // Display a load error message using a data: URI.
  std::stringstream ss;
  ss << "<html><body bgcolor=\"white\">"
        "<h2>Failed to load URL "
     << std::string(failedUrl) << " with error " << std::string(errorText)
     << " (" << errorCode << ").</h2></body></html>";

  frame->LoadURL(GetDataURI(ss.str(), "text/html"));
}

void SimpleHandler::CloseAllBrowsers(bool force_close) {
  if (!CefCurrentlyOn(TID_UI)) {
    // Execute on the UI thread.
    CefPostTask(TID_UI, base::BindOnce(&SimpleHandler::CloseAllBrowsers, this,
                                       force_close));
    return;
  }

  if (browser_list_.empty())
    return;

  BrowserList::const_iterator it = browser_list_.begin();
  for (; it != browser_list_.end(); ++it)
    (*it)->GetHost()->CloseBrowser(force_close);
}

// static
bool SimpleHandler::IsChromeRuntimeEnabled() {
  static int value = -1;
  if (value == -1) {
    CefRefPtr<CefCommandLine> command_line =
        CefCommandLine::GetGlobalCommandLine();
    value = command_line->HasSwitch("enable-chrome-runtime") ? 1 : 0;
  }
  return value == 1;
}

//CefDragHandler functions
bool SimpleHandler::OnDragEnter(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefDragData> dragData,
    CefDragHandler::DragOperationsMask mask) {
    CEF_REQUIRE_UI_THREAD();

    // Forbid dragging of URLs and files.
    if ((mask & DRAG_OPERATION_LINK) && !dragData->IsFragment()) {
        return true;
    }

    return false;
}

void SimpleHandler::OnDraggableRegionsChanged(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    const std::vector<CefDraggableRegion>& regions) {
    CEF_REQUIRE_UI_THREAD();

    HRGN draggable_region_ = ::CreateRectRgn(0, 0, 0, 0);    
    //Reset draggable region
    ::SetRectRgn(draggable_region_, 0, 0, 0, 0);

    // Determine new draggable region.
    std::vector<CefDraggableRegion>::const_iterator it = regions.begin();
    for (; it != regions.end(); ++it) {
        HRGN region = ::CreateRectRgn(it->bounds.x, it->bounds.y,
            it->bounds.x + it->bounds.width,
            it->bounds.y + it->bounds.height);
        ::CombineRgn(draggable_region_, draggable_region_, region,
            it->draggable ? RGN_OR : RGN_DIFF);
        ::DeleteObject(region);
    }
    delegate_->NotifyToSetDraggableRegion(regions);
}