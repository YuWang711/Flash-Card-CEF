#ifndef _FLASH_CARD_RENDER_PROCESS_
#define _FLASH_CARD_RENDER_PROCESS_

#include "include/wrapper/cef_message_router.h"
#include "include/cef_app.h"
#include "include/cef_render_process_handler.h"


class RenderProcessHandler : public CefApp, public CefRenderProcessHandler {
    public:
        RenderProcessHandler() {
            CefMessageRouterConfig config;
            message_render_router = CefMessageRouterRendererSide::Create(config);
        };

        //CefApp Methods
        CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override{
            return this;
        }

        void OnWebKitInitialized() override {
            // Create the renderer-side router for query handling.

        }
        void OnContextCreated(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context) override {

            message_render_router->OnContextCreated(browser, frame, context);
        }

        void OnContextReleased(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context) override {
            message_render_router->OnContextReleased(browser, frame, context);
        }

        bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefProcessId source_process,
            CefRefPtr<CefProcessMessage> message) override {
            return message_render_router->OnProcessMessageReceived(browser, frame,
                source_process, message);
        }


    private:
        CefRefPtr<CefMessageRouterRendererSide> message_render_router;

        IMPLEMENT_REFCOUNTING(RenderProcessHandler);
        DISALLOW_COPY_AND_ASSIGN(RenderProcessHandler);
};


#endif
#pragma once