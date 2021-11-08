// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_

#include "include/cef_client.h"
#include "include/wrapper/cef_message_router.h"
#include "./MessageRouter.h"
#include "./renderProcessHandler.h"
#include "include/cef_drag_handler.h"

#include <memory>
#include <list>
#include <iostream>

//CEFMessageRouterBrowserSide
//Should get a delegate?

class SimpleHandler : public CefClient,
                      public CefDisplayHandler,
                      public CefLifeSpanHandler,
                      public CefLoadHandler,
                      public CefRequestHandler,
                      public CefDragHandler,
                      public CefResourceRequestHandler {
 public:
     class Delegate {
     public:
         virtual void NotifyToSetDraggableRegion(const std::vector<CefDraggableRegion>& regions) = 0;
     protected:
         virtual ~Delegate() {}

  };
  explicit SimpleHandler(Delegate* delegate, bool use_views);
  ~SimpleHandler();

  // Provide access to the single global instance of this object.
  static SimpleHandler* GetInstance();

  // CefClient methods:
  virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override {
    return this;
  }
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
    return this;
  }
  virtual CefRefPtr< CefRequestHandler> GetRequestHandler() override {
      return this;
  }
  virtual CefRefPtr< CefDragHandler > GetDragHandler() override {
      return this;
  }
  virtual CefRefPtr<CefResourceRequestHandler> GetResourceRequestHandler(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      CefRefPtr<CefRequest> request,
      bool is_navigation,
      bool is_download,
      const CefString& request_initiator,
      bool& disable_default_handling);


  // CefDisplayHandler methods:
  void OnTitleChange(CefRefPtr<CefBrowser> browser,
                             const CefString& title) override;

  // CefLifeSpanHandler methods:
  void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
  bool DoClose(CefRefPtr<CefBrowser> browser) override;
  void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

  // CefLoadHandler methods:
  
  virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      ErrorCode errorCode,
      const CefString& errorText,
      const CefString& failedUrl) override;


  virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      CefProcessId source_process,
      CefRefPtr<CefProcessMessage> message);

  virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
      TerminationStatus status);

  virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect);

  //CefDragHandler methods:
  virtual void OnDraggableRegionsChanged(CefRefPtr<CefBrowser> browser,
                                         CefRefPtr<CefFrame> frame,
                                        const std::vector<CefDraggableRegion>& regions);
  virtual bool OnDragEnter( CefRefPtr< CefBrowser > browser,
                            CefRefPtr< CefDragData > dragData,
                            CefDragHandler::DragOperationsMask mask);

  // Request that all existing browser windows close.
  void CloseAllBrowsers(bool force_close);

  bool IsClosing() const { return is_closing_; }

  // Returns true if the Chrome runtime is enabled.
  static bool IsChromeRuntimeEnabled();

 private:
  // Platform-specific implementation.
  void PlatformTitleChange(CefRefPtr<CefBrowser> browser,
                           const CefString& title);

  // True if the application is using the Views framework.
  const bool use_views_;
  //base::ThreadChecked thread_checker;
  // List of existing browser windows. Only accessed on the CEF UI thread.
  typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
  BrowserList browser_list_;
  Delegate* delegate_;
  bool is_closing_;
  //Handler the browser side of query routing.
  CefRefPtr<CefMessageRouterBrowserSide> message_router;
  CefMessageRouterBrowserSide::Handler* message_handler;
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(SimpleHandler);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
