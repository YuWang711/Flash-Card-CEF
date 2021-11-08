#include "./MainContext.h"
//Store all default settings
//Create Client Handler HERe

MainContext::MainContext(bool terminate_when_all_windows_closed)
    : terminate_when_all_windows_closed_(terminate_when_all_windows_closed){
    initialized_ = false;
    shutdown_ = false;

}
void MainContext::OnViewsWindowCreated(CefRefPtr<ViewWindow> window) {

}
void MainContext::OnViewsWindowDestroyed(CefRefPtr<ViewWindow> window) {

}
void MainContext::OnExit() {

}


bool MainContext::Init(const CefMainArgs& args,
                const CefSettings& settings,
                CefRefPtr<CefApp> application,
                void* windows_sandbox_info) {
    DCHECK(!initialized_);
    DCHECK(!shutdown_);

    if (!CefInitialize(args, settings, application.get(), windows_sandbox_info)) {
        return false;
    }
    initialized_ = true;
    //Create Client Handler
    Handler_ = new SimpleHandler(this,true);
    
    CefBrowserSettings browser_settings; //Settings include background color and windowless framerate,
                                        //Implement a function call PopulateWindowSetting() when needed.
    

    startup_url = std::filesystem::current_path().string();
    
    startup_url.append("\\..\\..\\FlashCard\\Resource\\html\\homepage.html");  // For now use google
                                            //Needs to implement a file locator for the resources
 

    CefRefPtr<CefRequestContext> request_context;
    //How does the handler call SetDraggableRegions
    //Into Call Create View Window
    //Look into, CefBrowserSettings, and request_context,
    ViewWindow_ = ViewWindow::Create(Handler_, startup_url, browser_settings,
                       request_context);
    return true;
}

std::string MainContext::GetResourceDirectory() {
    return "";
}

void MainContext::Shutdown() {
  DCHECK(initialized_);
  DCHECK(!shutdown_);

  CefShutdown();

  shutdown_ = true;
}

void MainContext::NotifyToSetDraggableRegion(const std::vector<CefDraggableRegion>& regions) {
    ViewWindow_->SetDraggableRegions(regions);
}

