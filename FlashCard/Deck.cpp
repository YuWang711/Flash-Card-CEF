#include "Deck.h"

Deck::Deck() {
   Deck_Name = "";
   this->createCard();
   Deck_Index = 0;
}

Deck::Deck(std::string deckName) {
	this->setDeckName(deckName);
	this->createCard();
	Deck_Index = 0;
}

Deck::~Deck() {
}

void Deck::createCard() {
   FlashCard* newCard = new FlashCard();
   FlashCard_Deck.push_back(newCard);
   Deck_Index = this->lastCard();
}

void Deck::createCard(std::string frontContent, std::string backContent) {
   FlashCard* newCard = new FlashCard();
   newCard->setFront(frontContent);
   newCard->setBack(backContent);
   FlashCard_Deck.push_back(newCard);
   Deck_Index = this->lastCard();
}

void Deck::nextCard() {
	if(Deck_Index <= (FlashCard_Deck.size()-1)) {
		Deck_Index++;
	}
	if (Deck_Index > (FlashCard_Deck.size() - 1)) {
		Deck_Index = 0;
	}
}

int Deck::lastCard() {
  if (FlashCard_Deck.size() > 0) {
    return ((int)FlashCard_Deck.size() - 1);
  }
  else {
	return 0;
  }
}

void Deck::prevousCard() {
   if (Deck_Index == 0) {
		Deck_Index = this->lastCard();
   } else {
		Deck_Index--;
   }
}

void Deck::goToCard(int goToIndex) {
   Deck_Index = goToIndex;
}

void Deck::setCurrentFrontCard(std::string frontContent) {
	FlashCard_Deck.at(Deck_Index)->setFront(frontContent);
}

void Deck::setCurrentBackCard(std::string backContent) {
	FlashCard_Deck.at(Deck_Index)->setBack(backContent);
}

FlashCard* Deck::getCard() {
  return FlashCard_Deck.at(Deck_Index);
}

void Deck::deleteCard() {
  FlashCard_Deck.erase(FlashCard_Deck.begin() + Deck_Index);
  prevousCard();
}

//Might have an error
void Deck::setDeckName(std::string new_Name) {
	Deck_Name = new_Name;
}

std::string Deck::getDeckName() {
	return Deck_Name;
}

unsigned int Deck::getDeckSize() {
	return (unsigned int)FlashCard_Deck.size();
}

unsigned int Deck::getDeckIndex() {
	return Deck_Index;
}