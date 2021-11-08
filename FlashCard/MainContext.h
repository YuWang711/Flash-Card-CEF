#ifndef _FLASH_CARD_MAIN_CONTEXT_
#define _FLASH_CARD_MAIN_CONTEXT_

#include<filesystem>
#include <string>

#include "./ViewWindow.h"
#include "./simple_handler.h"
#include "include/views/cef_window.h"
#include "include/cef_app.h"

// Combine both maincontext and rootmanager


class MainContext : public ViewWindow::Delegate, 
                    public SimpleHandler::Delegate{
	public:
        MainContext(bool terminate_when_all_windows_closed);
        bool Init(const CefMainArgs& args,
                         const CefSettings& settings,
                         CefRefPtr<CefApp> application,
                         void* windows_sandbox_info);
        std::string GetResourceDirectory();
		void Shutdown();

        // CefApp Methods:

        //ViewWindow delegate method
        void OnViewsWindowCreated(
                    CefRefPtr<ViewWindow> window) override;
        void OnViewsWindowDestroyed(
                    CefRefPtr<ViewWindow> window) override;
        void OnExit() override;

        //SimpleHandler delegate method
        void NotifyToSetDraggableRegion(
                    const std::vector<CefDraggableRegion>& regions) override;

	private:
        CefRefPtr<SimpleHandler> Handler_;
         CefRefPtr<ViewWindow> ViewWindow_;
        std::string resourceDirectory;
        std::string startup_url;
        

        bool initialized_;
        bool shutdown_;
        bool terminate_when_all_windows_closed_;

        DISALLOW_COPY_AND_ASSIGN(MainContext);
};





#endif


#pragma once
