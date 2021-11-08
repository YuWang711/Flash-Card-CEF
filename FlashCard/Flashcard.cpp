#include "Flashcard.h"

FlashCard::FlashCard() {
	Front = new std::string("");
	Back = new std::string("");
}
FlashCard::~FlashCard() {
	delete Front;
	delete Back;
}

void FlashCard::setFront(std::string content) {
	*Front = content;
}
void FlashCard::setBack(std::string content) {
	*Back = content;
}

void FlashCard::eraseFront() {
	*Front = "";
}
void FlashCard::eraseBack() {
	*Back = "";
}

std::string FlashCard::getFront() {
	return *Front;
}
std::string FlashCard::getBack() {
	return *Back;
}