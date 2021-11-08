#ifndef _FLASH_CARD_FLASH_CARD_
#define _FLASH_CARD_FLASH_CARD_

#include <iostream>
#include <string>


class FlashCard {
 public:
	FlashCard();
	~FlashCard();

	void setFront(std::string content);
	void setBack(std::string content);

	void eraseFront();
	void eraseBack();

	std::string getFront();
	std::string getBack();

 private:
	std::string* Front;
	std::string* Back;
};


#endif	
#pragma once
