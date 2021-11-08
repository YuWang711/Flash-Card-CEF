//View window is the one that created the View.
//Root_Window go through the configuration
//Configuration goes into client handler / simplehandler in our case.

#include "./ViewWindow.h"
#include <algorithm>

#include "include/base/cef_build.h"
#include "include/base/cef_callback.h"
#include "include/cef_app.h"
#include "include/cef_i18n_util.h"
#include "include/views/cef_box_layout.h"
#include "include/wrapper/cef_helpers.h"

enum ControlIds {
  ID_WINDOW = 1,
  ID_BROWSER_VIEW,
};

ViewWindow::ViewWindow(CefRefPtr<CefBrowserView> browser_view) {
  if (browser_view)
    SetBrowserView(browser_view);

  CefRefPtr<CefCommandLine> command_line =
      CefCommandLine::GetGlobalCommandLine();
  // Without a window frame.
  frameless_ = true;
  /*
  const std::string& show_state =
      command_line->GetSwitchValue(switches::kInitialShowState);
  if (show_state == "minimized") {
    initial_show_state_ = CEF_SHOW_STATE_MINIMIZED;
  } else if (show_state == "maximized") {
    initial_show_state_ = CEF_SHOW_STATE_MAXIMIZED;
  } else if (show_state == "fullscreen") {
    initial_show_state_ = CEF_SHOW_STATE_FULLSCREEN;
  }*/

}

CefRefPtr<ViewWindow> ViewWindow::Create(
    CefRefPtr<CefClient> client,
    const CefString& url,
    const CefBrowserSettings& settings,
    CefRefPtr<CefRequestContext> request_context) {
  CEF_REQUIRE_UI_THREAD();

  // Create a new ViewsWindow.
  CefRefPtr<ViewWindow> views_window = new ViewWindow(nullptr);

  // Create a new BrowserView.
  CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
      client, url, settings, nullptr, request_context, views_window);

  // Associate the BrowserView with the ViewsWindow.
  views_window->SetBrowserView(browser_view);

  CefWindow::CreateTopLevelWindow(views_window);

  return views_window;
}

// Functionality:
void ViewWindow::Show() {
  CEF_REQUIRE_UI_THREAD();
  if (window_) {
    window_->Show();
  }
  browser_view_->RequestFocus();
}
void ViewWindow::Hide() {
  CEF_REQUIRE_UI_THREAD();
  if (window_) {
    window_->Hide();
  }
}
void ViewWindow::Minimize() {
  CEF_REQUIRE_UI_THREAD();
  if (window_)
    window_->Minimize();
}
void ViewWindow::Maximize() {
  CEF_REQUIRE_UI_THREAD();
  if (window_)
    window_->Maximize();
}

void ViewWindow::SetBounds(const CefRect& bounds) {
  CEF_REQUIRE_UI_THREAD();
  if (window_)
    window_->SetBounds(bounds);
}
void ViewWindow::SetBrowserSize(const CefSize& size,
                                 bool has_position,
                                 const CefPoint& position) {
  CEF_REQUIRE_UI_THREAD();
  if (browser_view_)
    browser_view_->SetSize(size);
  if (window_) {
    window_->SizeToPreferredSize();
    if (has_position)
      window_->SetPosition(position);
  }
}
void ViewWindow::Close(bool force) {
  CEF_REQUIRE_UI_THREAD();
  if (!browser_view_)
    return;

  CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
  if (browser) {
    // This will result in a call to CefWindow::Close() which will then call
    // ViewsWindow::CanClose().
    browser->GetHost()->CloseBrowser(force);
  }
}
void ViewWindow::SetFullscreen(bool fullscreen) {
  CEF_REQUIRE_UI_THREAD();
  if (window_) {
    // Hide the top controls while in full-screen mode.
    window_->SetFullscreen(fullscreen);
  }
}


void ViewWindow::SetDraggableRegions(
    const std::vector<CefDraggableRegion>& regions) {
  CEF_REQUIRE_UI_THREAD();

  if (!window_ || !browser_view_)
    return;

  std::vector<CefDraggableRegion> window_regions;

  // Convert the regions from BrowserView to Window coordinates.
  std::vector<CefDraggableRegion>::const_iterator it = regions.begin();
  for (; it != regions.end(); ++it) {
    CefDraggableRegion region = *it;
    CefPoint origin = CefPoint(region.bounds.x, region.bounds.y);
    browser_view_->ConvertPointToWindow(origin);
    region.bounds.x = origin.x;
    region.bounds.y = origin.y;
    window_regions.push_back(region);
  }
  window_->SetDraggableRegions(window_regions);
}

// CefWindowDelegate methods:
void ViewWindow::OnWindowCreated(CefRefPtr<CefWindow> window) {
  CEF_REQUIRE_UI_THREAD();
  DCHECK(browser_view_);
  DCHECK(!window_);
  DCHECK(window);

  window_ = window;
  //delegate_->OnViewsWindowCreated(this);

  const CefRect bounds = GetInitialBounds();
  if (bounds.x == 0 && bounds.y == 0) {
    // Size the Window and center it.
    window_->CenterWindow(CefSize(bounds.width, bounds.height));
  } else {
    // Set the Window bounds as specified.
    window_->SetBounds(bounds);
  }

  // Add the BrowserView and other controls to the Window.
  AddBrowserView();

  // Add keyboard accelerators to the Window.
  // AddAccelerators();

  // Hide the top controls while in full-screen mode.
  // Add the BrowserView as the only child of the Window.
  window_->AddChildView(browser_view_);

   // Choose a reasonable minimum window size.
   minimum_window_size_ = CefSize(100, 100);
   // Show the Window.
   Show();
}
void ViewWindow::OnWindowDestroyed(CefRefPtr<CefWindow> window) {
  browser_view_ = nullptr;
  window_ = nullptr;
}

CefRect ViewWindow::GetInitialBounds(CefRefPtr<CefWindow> window) {
  CEF_REQUIRE_UI_THREAD();
  if (frameless_) {
    // Need to provide a size for frameless windows that will be centered.
    const CefRect bounds = GetInitialBounds();
    if (bounds.x == 0 && bounds.y == 0) {
      return bounds;
    }
  }
  return CefRect();
}

cef_show_state_t ViewWindow::GetInitialShowState(CefRefPtr<CefWindow> window) {
  CEF_REQUIRE_UI_THREAD();
  return initial_show_state_;
}
bool ViewWindow::IsFrameless(CefRefPtr<CefWindow> window) {
    return frameless_;
}
bool ViewWindow::CanResize(CefRefPtr<CefWindow> window) {
  return true;
}
bool ViewWindow::CanClose(CefRefPtr<CefWindow> window) {
  CEF_REQUIRE_UI_THREAD();
  if (!browser_view_) {
    return false;
  }

  CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
  if (browser) {
    return browser->GetHost()->TryCloseBrowser();
  }
  return true;
}

// CefViewDelegate methods:
CefSize ViewWindow::GetMinimumSize(CefRefPtr<CefView> view) {
  CEF_REQUIRE_UI_THREAD();

  if (view->GetID() == ID_WINDOW)
    return minimum_window_size_;

  return CefSize();
}

void ViewWindow::SetBrowserView(CefRefPtr<CefBrowserView> browser_view) {
  DCHECK(!browser_view_);
  DCHECK(browser_view);
  DCHECK(browser_view->IsValid());
  DCHECK(!browser_view->IsAttached());  
  browser_view_ = browser_view;
  browser_view_->SetID(ID_BROWSER_VIEW);
}
void ViewWindow::AddBrowserView() {
  CefBoxLayoutSettings window_layout_settings;
  window_layout_settings.horizontal = false;
  window_layout_settings.between_child_spacing = 2;
  CefRefPtr<CefBoxLayout> window_layout =
      window_->SetToBoxLayout(window_layout_settings);

  window_->AddChildView(browser_view_);
  window_layout->SetFlexForView(browser_view_, 1);
}
CefRect ViewWindow::GetInitialBounds() const {
  CEF_REQUIRE_UI_THREAD();
  CefRect bounds;
  if (bounds.IsEmpty()) {
    // Use the default size.
    bounds.width = 800;
    bounds.height = 600;
  }
  return bounds;
}

