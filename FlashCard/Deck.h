#ifndef _FLASH_CARD_DECK_
#define _FLASH_CARD_DECK_

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include "Flashcard.h"

class Deck {
	public:
		Deck();
		Deck(std::string deckName);
        ~Deck();
		
		void createCard();
        void createCard(std::string frontContent, std::string backContent);

		void nextCard();
        void prevousCard();
        void goToCard(int goToIndex);
        int lastCard();

		void setCurrentFrontCard(std::string frontContent);
        void setCurrentBackCard(std::string frontContent);

		FlashCard* getCard();

		void deleteCard();

		void setDeckName(std::string new_Name);
        std::string getDeckName();

		unsigned int getDeckSize();
		unsigned int getDeckIndex();
 private:
    unsigned int Deck_Index;
	std::vector<FlashCard*> FlashCard_Deck;
	std::string Deck_Name;
};

#endif
#pragma once
