/*
* FileHandler, high-level function call
*	-Store current directory path
*	-Read files for flashcards.
*	-Write files for flashcards.
*/
#ifndef _FLASH_CARD_FILE_HANDLER_
#define _FLASH_CARD_FILE_HANDLER_


#include <iostream>
#include <string>
#include <filesystem>


#include "Deck.h"
#include <fstream>
#include <sstream>

const std::string DeckDirectory = "\\..\\..\\FlashCard\\Resource\\Deck\\";

class FileHandler {
 public:
	FileHandler();
	//Need to return data
	bool ReadFile(std::string Filename, Deck* deck);
	bool WriteFile(std::string Filename, Deck* deck);
	bool deleteFile(std::string filename);
	std::string HandleDoubleQuote(std::string data);
	std::string HandleComma(std::string data);
	int countQuote(std::string data);
	std::string ReplaceQuotes(std::string data);
 private:
	std::string directoryPath;
};


#endif
#pragma once
