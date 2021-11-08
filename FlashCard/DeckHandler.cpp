
#include "DeckHandler.h"
#include <algorithm>

DeckHandler::DeckHandler() {
}
DeckHandler::~DeckHandler() {

}

std::unique_ptr<Deck> DeckHandler::createDeck(std::string DeckName) {
	// Create and open a text file
	if (DeckName == "") {
		DeckName = "Default";
	}
	std::unique_ptr<Deck> newDeck;
	if (deckList.find(DeckName) == deckList.end() &&
		std::find(deckFileName.begin(), deckFileName.end(), DeckName) == deckFileName.end()) {
		newDeck = std::make_unique<Deck>(DeckName);
		deckList.insert(std::pair<std::string, Deck*>(DeckName, newDeck.get()));
		deckFileName.push_back(DeckName);
	}
	else {
		int i = 1;
		std::string newDeckName = DeckName;
		while (deckList.find(newDeckName) != deckList.end() &&
			std::find(deckFileName.begin(), deckFileName.end(), newDeckName) != deckFileName.end()) {
			newDeckName = DeckName;
			newDeckName.append("(");
			newDeckName.append(std::to_string(i));
			newDeckName.append(")");
			i++;
		}
		newDeck = std::make_unique<Deck>(newDeckName);
		deckList.insert(std::pair<std::string, Deck*>(newDeckName, newDeck.get()));
		deckFileName.push_back(newDeckName);
	}
	return newDeck;
}

std::unique_ptr<Deck> DeckHandler::readFiletoDeck(std::string fileName) {
	eraseAllSubStr(fileName, ".csv");
	auto pos = deckList.find(fileName);
	if (pos == deckList.end()) {
		std::unique_ptr<Deck> newDeck = std::make_unique<Deck>(fileName);
		if (fileHandler.ReadFile(fileName, newDeck.get())) {
			deckFileName.push_back(fileName);
			deckList.insert(std::pair<std::string, Deck*>(fileName, newDeck.get()));
			return newDeck;
		}
	}
	else {
		std::unique_ptr<Deck> deck(pos->second);
		return deck;
	}
	return nullptr;
}
bool DeckHandler::writeDecktoFile(std::string DeckName) {
	std::string FileName = DeckName;
	FileName.append(".csv");
	auto pos = deckList.find(DeckName);
	if (pos != deckList.end()) {
		if (fileHandler.WriteFile(DeckName.append(".csv"), pos->second)) {
			return true;
		}
	}
	return false;
}
void DeckHandler::findValidCSVFileinLocal(CefRefPtr<CefBrowser> browser, std::string ID) {
	std::string javascriptCode =
		"var localFile = document.createElement('div');"
		"localFile.innerHTML = '"
		"';"
		"localFile.className = 'deckName';"
		"var localFileContainer = document.createElement('div');"
		"localFileContainer.className = 'selectable_Deck non_draggable';"
		"localFileContainer.setAttribute('data-fileName', \""
		"\");"
		"localFileContainer.onclick = function(){Highlight(this);};"
		"localFile.ondblclick = function(){LoadExistingDeck(this);};"
		"localFileDeleteButton = document.createElement(\"BUTTON\");"
		"localFileDeleteButton.innerHTML = 'Delete';"
		"localFileDeleteButton.onclick = function(){deleteDeck(this);};"
		"localFileDeleteButton.className = \"deck_Delete_Button non_draggable\";"
		"localFileDeleteButton.setAttribute('data-fileName', \""
		"\");"
		"localFileContainer.appendChild(localFile);"
		"localFileContainer.appendChild(localFileDeleteButton);"
		"var container = document.getElementById('";
		javascriptCode.append(ID);
		javascriptCode.append("');"
		"container.appendChild(localFileContainer);"
		);
	for (const auto& file : std::filesystem::directory_iterator("../../Flashcard/Resource/Deck/")) {
		auto filePath = file.path();
		if (filePath.extension() == ".csv") {
			//Execute Javascript
			//HightLights Onclick. double click = open.
			CefRefPtr<CefFrame> frame = browser->GetMainFrame();
			std::string executeCode = javascriptCode;
			std::string fileName = filePath.stem().string();
			executeCode.insert(676, fileName);
			executeCode.insert(272, fileName);
			executeCode.insert(69, fileName);
			frame->ExecuteJavaScript(executeCode, frame->GetURL(), 0);
			deckFileName.push_back(fileName);
		}
	}
}

Deck* DeckHandler::getDeck(std::string deckName) {
	auto pos = deckList.find(deckName);
	if (pos != deckList.end()) {
		return pos->second;
	}
	return nullptr;
}

//Helper functions
void DeckHandler::eraseAllSubStr(std::string& mainStr, const std::string& toErase)
{
	size_t pos = std::string::npos;
	// Search for the substring in string in a loop untill nothing is found
	while ((pos = mainStr.find(toErase)) != std::string::npos)
	{
		// If found then erase it from string
		mainStr.erase(pos, toErase.length());
	}
}

bool DeckHandler::deleteDeck(std::string fileName) {
	if (fileHandler.deleteFile(fileName)) {
		return true;
	}
	return false;
}