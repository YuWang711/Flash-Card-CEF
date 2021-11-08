#ifndef _FLASH_CARD_VIEW_WINDOW_H
#define _FLASH_CARD_VIEW_WINDOW_H

#include <set>
#include <vector>
#include <string>

#include "include/views/cef_browser_view.h"
#include "include/views/cef_browser_view_delegate.h"
#include "include/views/cef_window.h"
#include "include/views/cef_window_delegate.h"

class ViewWindow : public CefWindowDelegate
                  ,public CefBrowserViewDelegate{
 public:
     class Delegate {
        public:
            virtual void OnViewsWindowCreated(CefRefPtr<ViewWindow> window) = 0;
            virtual void OnViewsWindowDestroyed(CefRefPtr<ViewWindow> window) = 0;
            virtual void OnExit() = 0;

        protected:
            virtual ~Delegate() {}
    };
  static CefRefPtr<ViewWindow> Create(CefRefPtr<CefClient> client,
                                    const CefString& url,
                                    const CefBrowserSettings& settings,
                                    CefRefPtr<CefRequestContext> request_context);

  // Functionality:
  void Show();
  void Hide();
  void Minimize();
  void Maximize();
  void SetBounds(const CefRect& bounds);
  void SetBrowserSize(const CefSize& size,                      bool has_position,
                      const CefPoint& position);
  void Close(bool force);
  void SetFullscreen(bool fullscreen);
  void SetDraggableRegions(const std::vector<CefDraggableRegion>& regions);

  // CefWindowDelegate methods:
  void OnWindowCreated(CefRefPtr<CefWindow> window) override;
  void OnWindowDestroyed(CefRefPtr<CefWindow> window) override;
  CefRect GetInitialBounds(CefRefPtr<CefWindow> window) override;
  cef_show_state_t GetInitialShowState(CefRefPtr<CefWindow> window) override;
  bool IsFrameless(CefRefPtr<CefWindow> window) override;
  bool CanResize(CefRefPtr<CefWindow> window) override;
  bool CanClose(CefRefPtr<CefWindow> window) override;
  CefRefPtr<CefWindow> GetParentWindow(CefRefPtr<CefWindow> window,
      bool* is_menu,
      bool* can_activate_menu) override { return nullptr;}

  // CefViewDelegate methods:
  CefSize GetMinimumSize(CefRefPtr<CefView> view) override;
  /*
  void OnFocus(CefRefPtr<CefView> view) override;
  void OnBlur(CefRefPtr<CefView> view) override;
  void OnWindowChanged(CefRefPtr<CefView> view, bool added) override;
  void OnLayoutChanged(CefRefPtr<CefView> view,
                       const CefRect& new_bounds) override;*/

  private:
   ViewWindow(CefRefPtr<CefBrowserView> browser_view);
   void SetBrowserView(CefRefPtr<CefBrowserView> browser_view);
   void AddBrowserView();
   CefRect GetInitialBounds() const;

   CefRefPtr<CefBrowserView> browser_view_;
   bool frameless_;
   CefRefPtr<CefWindow> window_;
   CefSize minimum_window_size_;
   cef_show_state_t initial_show_state_ = CEF_SHOW_STATE_NORMAL;

   IMPLEMENT_REFCOUNTING(ViewWindow);
   DISALLOW_COPY_AND_ASSIGN(ViewWindow);
};
#endif