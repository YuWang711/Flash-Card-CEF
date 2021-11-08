#ifndef _FLASH_CARD_MESSAGE_ROUTER
#define _FLASH_CARD_MESSAGE_ROUTER

#include "include/wrapper/cef_message_router.h"
#include "include/cef_client.h"
#include "include/wrapper/cef_helpers.h"
#include "./simple_handler.h"
#include "include/cef_app.h"
#include "./DeckHandler.h"

#include <string>

//BroswerSide JS Handler

class Message_Handler : public CefMessageRouterBrowserSide::Handler {
	public:
        Message_Handler();
        ~Message_Handler();
        bool OnQuery(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            int64 query_id,
            const CefString& request,
            bool persistent,
            CefRefPtr<Callback> callback);


        
     private:
        //const CefString startup_url_;
        //Auto Save every 10 minutes
        //Implement manual save first.--Can use onlosefocus to trigger an update 
        DeckHandler Deck_Handler_;
        std::unique_ptr<Deck> current_Deck;
        DISALLOW_COPY_AND_ASSIGN(Message_Handler);
};


#endif


#pragma once
