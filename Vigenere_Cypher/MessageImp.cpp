/*-------------------------------------------------------------------------------------------------------------------------
| This is the Implementation of Message.h, which serves as an API connecting user requests to the Vigenere processing algorithm.
-------------------------------------------------------------------------------------------------------------------------*/

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>
#include "Vigenere.h"
#include "Message.h"

// Key k is passed into Message constructor --> Passed into Vigenere's setKey() because Vigenere object "v" already created.
Message::Message(std::string k)
{
	v.setKey(k);
}

// Cast each char pointer to work token, encrypt, and add to words vector
void Message::encryptWord(char* token)
{
	word = static_cast<std::string>(token);
	word = v.encrypt(word);
	words.push_back(word);
}

// Cast each char pointer to work token, decrypt, and add to words vector
void Message::decryptWord(char* token)
{
	word = static_cast<std::string>(token);
	word = v.decrypt(word);
	words.push_back(word);
}

// Create file and output encrypted content (words vector)
void Message::makeFile(std::string fileName) const
{	
	std::ofstream outFile(fileName, std::ios::out);
		// ios::out		: overwrite
		// ios::app		: append to end
		// ios::app		: append to beginning, then move cursor to end
		// ios::trunc	: if file exists, truncate file before open

	// Print word vector 
	for (int i = 0; i < words.size(); ++i)
	{
		outFile << words[i] << " ";
	}

	outFile.close();
}

// Output encrypted content (words vector) to console
void Message::showWords() const
{
	// Print word vector but go down new line if run out of lines + printing animation
	std::chrono::milliseconds timespan(3);
	for (int i = 0; i < words.size(); ++i)
	{
		if (i % 18 == 0)
			std::cout << std::endl;
		std::cout << words[i] << " ";
		std::this_thread::sleep_for(timespan);
	}
}


// private:
	// std::string word;
	// std::vector<std::string> words;
	// Vigenere v;