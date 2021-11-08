#include "./MessageRouter.h"

Message_Handler::Message_Handler() {
}

Message_Handler::~Message_Handler() {
   
}


bool Message_Handler::OnQuery(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    int64 query_id,
    const CefString& request,
    bool persistent,
    CefRefPtr<Callback> callback) {
    std::string RequestString;
    if (request.ToString().find("Create_Deck")==0) {
          //Need to check if the string is acceptable in file name
        RequestString = request.ToString().substr(sizeof("Create_Deck"));
        current_Deck = Deck_Handler_.createDeck(RequestString);
        callback->Success(RequestString);
        return true;
    }
    else if (request.ToString().find("Add_Card")==0) {
        RequestString = request.ToString().substr(sizeof("Add_Card"));
        current_Deck->createCard();
        callback->Success(RequestString);
    }
    else if (request.ToString().find("Save_Front")==0) {
        RequestString = request.ToString().substr(sizeof("Save_Front"));
        current_Deck->setCurrentFrontCard(RequestString);
        callback->Success(RequestString);
    }
    else if (request.ToString().find("Save_Back")==0) {
        RequestString = request.ToString().substr(sizeof("Save_Back"));
        current_Deck->setCurrentBackCard(RequestString);
        callback->Success(RequestString);
    }
    else if (request.ToString().find("Save_File")==0) {
        Deck_Handler_.writeDecktoFile(current_Deck->getDeckName());
        callback->Success(RequestString);
    }
    else if (request.ToString().find("Find_File")==0) {
        RequestString = request.ToString().substr(sizeof("Find_File"));
        Deck_Handler_.findValidCSVFileinLocal(browser, RequestString);
        callback->Success(RequestString);
    } else if (request.ToString().find("Load_File") == 0) {
        RequestString = request.ToString().substr(sizeof("Load_File"));
        current_Deck.release();
        current_Deck = Deck_Handler_.readFiletoDeck(RequestString);
        current_Deck->goToCard(0);
        if (current_Deck == nullptr) {
            callback->Failure(1, "unable to load file");
        }
        callback->Success("");
    }
    else if (request.ToString().find("Next_Card") == 0) {
        current_Deck->nextCard();
        callback->Success(RequestString);
    }
    else if (request.ToString().find("Previous_Card") == 0) {
        current_Deck->prevousCard();
        callback->Success(RequestString);
    }
    else if (request.ToString().find("Get_Front") == 0) {
        FlashCard* card = current_Deck->getCard();
        RequestString = card->getFront();
        callback->Success(RequestString);
    }
    else if (request.ToString().find("Get_Back") == 0) {
        FlashCard* card = current_Deck->getCard();
        RequestString = card->getBack();
        callback->Success(RequestString);
    }
    else if (request.ToString().find("Minimize") == 0) {
        CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
        ShowWindow(hwnd, SW_SHOWMINIMIZED);
        callback->Success("");
    }
    else if (request.ToString().find("Maximize") == 0) {
        CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
        ShowWindow(hwnd, SW_SHOWMAXIMIZED);
        callback->Success("");
    }
    else if (request.ToString().find("RestoreDown") == 0) {
        CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
        ShowWindow(hwnd, SW_SHOWDEFAULT);
        callback->Success("");
    }
    else if (request.ToString().find("Delete_Card") == 0) {
        if (current_Deck->getDeckSize() == 1) {
            callback->Failure(2, "Cannot remove the only card in the deck");
            return false;
        }
        current_Deck->deleteCard();
        callback->Success("");
    }
    else if (request.ToString().find("Delete_Deck") == 0) {
        RequestString = request.ToString().substr(sizeof("Load_File"));
        RequestString = RequestString.substr(2);
        if (Deck_Handler_.deleteDeck(RequestString)) {
            callback->Success("");
        }
        callback->Failure(3, "Unable to delete Deck");
    }
    else if (request.ToString().find("Get_Deck_Size") == 0) {
        unsigned int temp = current_Deck->getDeckSize();
        RequestString = std::to_string(temp);
        callback->Success(RequestString);
    }
    else if (request.ToString().find("Get_Index") == 0) {
        unsigned int temp = current_Deck->getDeckIndex();
        RequestString = std::to_string(temp + 1);
        callback->Success(RequestString);
    } else if (request.ToString().find("getName") == 0) {
        RequestString = current_Deck->getDeckName();
        callback->Success(RequestString);
    }
    return true;
}

//JS size
/*
    window.cefquery({
        request:,
        onSuccess:,
        onFailure:
    });

*/