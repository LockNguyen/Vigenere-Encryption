/*-------------------------------------------------------------------------------------------------------------------------
| This contains prototypes for Message object, which serves as an API connecting user requests to the Vigenere encryption algorithm.
-------------------------------------------------------------------------------------------------------------------------*/

#pragma once
#include <string>
#include <vector>
#include "Vigenere.h"

class Message
{
public:

	// Key k is passed into Message constructor --> Passed into Vigenere's setKey() because Vigenere object "v" already created.
	Message(std::string k);

	// Cast each char pointer to work token, encrypt, and add to words vector
	void encryptWord(char* token);

	// Cast each char pointer to work token, decrypt, and add to words vector
	void decryptWord(char* token);

	// Create file and output encrypted content (words vector)
	void makeFile(std::string fileName) const;

	// Output encrypted content (words vector) to console
	void showWords() const;

private:

	std::string word;
	std::vector<std::string> words;
	Vigenere v;
};