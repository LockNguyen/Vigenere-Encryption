/*-------------------------------------------------------------------------------------------------------------------------
| This is the Implementation of Vigenere.h, which hosts the Vigenere encryption algorithm.
-------------------------------------------------------------------------------------------------------------------------*/

#include "Vigenere.h"

// Default Constructor
Vigenere::Vigenere()
{
	key = "";
}

// Constructor Initialized with Key
Vigenere::Vigenere(std::string k)
{
	key = toUpperCase(k);
}

// Set en/decryption key
void Vigenere::setKey(std::string k)
{
	key = toUpperCase(k);
}

// Retrieve en/decryption key
std::string Vigenere::getKey() const
{
	return key;
}

// Convert encryption key to uppercase before encrypting/decrypting
std::string Vigenere::toUpperCase(std::string k)
{	
	char c;
	for (int i = 0; i < k.size(); ++i)
	{
		c = k[i];
		k[i] = toupper(c);
	}

	return k;
}

// Encrypt word token
std::string Vigenere::encrypt(std::string word)
{
	std::string output;
	for (int i = 0, j = 0; i < word.length(); ++i)
	{
		char c = word[i];
		if (c >= 'a' && c <= 'z')
			c += 'A' - 'a';
		else if (c < 'A' || c > 'Z')
			continue;
		output += (c + key[j] - 2 * 'A') % 26 + 'A'; //added 'A' to bring it in range of ASCII alphabet [ 65-90 | A-Z ]
		j = (j + 1) % key.length();
	}
	return output;
}

// Decrypt word token
std::string Vigenere::decrypt(std::string word)
{
	std::string output;
	for (int i = 0, j = 0; i < word.length(); ++i)
	{
		char c = word[i];
		if (c >= 'a' && c <= 'z')
			c += 'A' - 'a';
		else if (c < 'A' || c > 'Z')
			continue;
		output += (c - key[j] + 26) % 26 + 'A';//added 'A' to bring it in range of ASCII alphabet [ 65-90 | A-Z ]
		j = (j + 1) % key.length();
	}
	return output;
}