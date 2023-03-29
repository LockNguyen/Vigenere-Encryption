/*-------------------------------------------------------------------------------------------------------------------------
| This contains prototypes for Vigenere object, which hosts the Vigenere encryption algorithm.
-------------------------------------------------------------------------------------------------------------------------*/

#pragma once
#include <string>

class Vigenere
{
public:
	
	// Default Constructor
	Vigenere();

	// Constructor Initialized with Key
	Vigenere(std::string k);

	// Set en/decryption key
	void setKey(std::string k);

	// Retrieve en/decryption key
	std::string getKey() const;

	// Convert encryption key to uppercase before encrypting/decrypting
	std::string toUpperCase(std::string k);

	// Encrypt word token
	std::string encrypt(std::string word);

	// Decrypt word token
	std::string decrypt(std::string word);

private:

	std::string key;
};