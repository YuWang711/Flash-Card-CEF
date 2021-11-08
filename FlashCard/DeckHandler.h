#ifndef _FLASH_CARD_DECK_HANDLER_
#define _FLASH_CARD_DECK_HANDLER_

#include <iostream>
#include <map>
#include <vector>
#include <fstream>

#include "Deck.h"
#include "FileHandler.h"
#include "include/cef_browser.h"

class DeckHandler {
	public:
		DeckHandler();
        ~DeckHandler();

        std::unique_ptr<Deck> createDeck(std::string DeckName);

        //Call file handler or find local csv file in resource directory
        std::unique_ptr<Deck>  readFiletoDeck(std::string fileName);
        //Read local valid deck file
        bool writeDecktoFile(std::string DeckName);

        //Should be call in onCreate();
        void findValidCSVFileinLocal(CefRefPtr<CefBrowser> browser,std::string ID);
        bool deleteDeck(std::string fileName);

        void eraseAllSubStr(std::string& mainStr, const std::string& toErase);
        Deck* getDeck(std::string deckName);
	private:
        //Map<FileName, Deck>
		std::map<std::string, Deck*> deckList;
        FileHandler fileHandler;
        std::vector<std::string> deckFileName;
};


#endif