/*-------------------------------------------------------------------------------------------------------------------------
| This is the Driver Code for Vigenere Cypher application.
-------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------
| Loc Nguyen
| Lab 5: Vigenere Cypher
| Ask user for secret cypher key and encrypt/decrypt file using Vigenere Cypher algorithm
| 3 / 24 / 23
-------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------
|	Resources Used & Citations
|
|	Color Coding: https://replit.com/talk/ask/how-do-i-change-the-color-of-the-text-on-c/137999
|	Get & Set Cursor Position (for Masking Password): https://www.grepper.com/answers/322340/c%2B%2B+get+cursor+position+console
|	Miscellaneous functions & libraries: StackOverflow, Microsoft Learn Documentations
|	wait() Function: Dr. Sipantzi
-------------------------------------------------------------------------------------------------------------------------*/

#include <iostream>		// Header file for std in/out
#include <string>		// Header file for strtok_s() and strings
#include <fstream>		// Header file for file opening, reading, writing
#include <windows.h>	// Header file for cursor positions (x, y) -- masking the password
#undef max					// Disable max() that comes with this Header file, which clashes with our numeric_limits<streamsize>::max()
#include <chrono>		// Header file for time -- loading screen
#include <thread>		// Header file for time -- loading screen
#include <iomanip>		// Header file for formatting -- loading screen
#include "Message.h"	// Header file for encryption/decryption

using namespace std;

// User Interaction
void displayMenu();									// show option menu
int getValidOption();								// get option + error check

// Encryption API
string createSecretKey();							// create secret key for Message object
void encryptFile(Message&, char contentArray[]);	// break char array into tokens + encrypt
void decryptFile(Message&, char contentArray[]);	// break char array into tokens + decrypt

// File Handling
void getFile(char contentArray[], string action);	// get file to encrypt + show included message
void makeFile(Message&, string action);				// show encrypted message + make file

// Miscancellaneous
void pinkArrowAnimation();							// encryption loading animation
void cout_color(string coutMessage, string color);	// print any message in color
void wait();										// wait for user to see screen

// Driver Code -- The magic starts here
int main()
{
	bool done{ false };
	int option{ 0 };
	string key;

	// While user not done (press 3)
	while (!done)
	{
		char contentArray[1001];	//character array is null-terminated --> 1000 characters + 1 '\0'

		// Clear screen:
		system("cls");

		// Show title:
		cout_color(" Vigenere Cypher", "blue_flashing");
		cout << endl;

		// Show menu:
		displayMenu();

		// Get option (error-checked):
		option = getValidOption();

		// If user chose 1 or 2
		if (option != 3)
		{
			// "Enter secret key"
			key = createSecretKey();
		}

		// Create Message object with key:
		Message m(key);

		// Execute requested option:
		switch (option)
		{
		case 1:								// "1 - Encrypt File"
			getFile(contentArray, "encrypt");
			encryptFile(m, contentArray);
			makeFile(m, "encrypt");
			break;
		case 2:								// "2 - Decrypt File"
			getFile(contentArray, "decrypt");
			decryptFile(m, contentArray);
			makeFile(m, "decrypt");
			break;
		case 3:								// "3 - Quit"
			done = true;
			break;
		default:
			break;
		}
	}
	
	cout << endl;
	cout_color("Have a good day!\n", "blue");
	cout << endl;
	wait();
	return 0;
}

// Create new Message object every loop with a key:
string createSecretKey()
{
	string key;

	// "Please enter a key"
	cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear garbage in buffer after 'cin <<' is used (by getValidOption())
	cout << endl;
	cout << "Please enter an encryption/decryption key:\n";
	getline(cin, key);

	// [ERROR CHECK] If key = blank : "Try again!"
	while (key.size() < 1)
	{
		cout_color("You cannot enter a blank key. Enter an encryption/decryption key:\n", "yellow");
		getline(cin, key);
	}
	
	// Mask the password for security:
	// 1. load current console cursor x, y positions into 'csbi'
	HANDLE hConsoleOutput{ GetStdHandle(STD_OUTPUT_HANDLE) };
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);
	
	// 2. store x, y position in 'passwordCoord', but move up one line
	COORD passwordCoord{ csbi.dwCursorPosition };
	passwordCoord.Y--;
	
	// 3. over-write that line with aterisks
	SetConsoleCursorPosition(hConsoleOutput, passwordCoord);
	for (int i = 0; i < key.size(); ++i)
		cout << "*";
	cout << endl;

	// Create key:
	return key;
}

// Display action menu to user
void displayMenu()
{
	cout << endl;
	cout << "    Main Menu" << endl;
	cout << endl;
	cout << "1 - Encrypt File" << endl;
	cout << "2 - Decrypt File" << endl;
	cout << "3 - Quit" << endl;
	cout << endl;
	cout << endl;
	cout << "Selection: ";
}

// Get option from user (with error-check)
int getValidOption()
{
	int temp;
	cin >> temp;

	// [ERROR CHECK] While option is invalid
	while (cin.fail() || temp < 1 || temp > 3)
	{
		// "Try again"
		cout << "\033[33m" << "I didn't understand that... Kindly choose 1, 2 or 3." << "\033[0m";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << endl;
		cout << "Selection: ";
		cin >> temp;
	}

	return temp;
}

// Get read file content into char array for encryption
void getFile(char contentArray[], string action)
{
	string fileName;
	ifstream inFile;
	
	cout << endl;

	if (action == "encrypt")
	{
		// Show title:
		cout_color("[ ENCRYPTING ]\n", "blue_flashing");
		cout << endl;
		cout << "Enter the name of the file to encrypt: ";
	}
	else
	{
		// Show title:
		cout_color("[ DECRYPTING ]\n", "blue_flashing");
		cout << endl;
		cout << "Enter the name of the file to decrypt: ";
	}

	// Get file name
	getline(cin, fileName);

	// Try to open file
	inFile.open(fileName);

	// [ERROR CHECK] If fail : "Try again!"
	while (!inFile.is_open())
	{
		cout_color("I couldn't find that file... Enter another file name: ", "yellow");
		getline(cin, fileName);
		inFile.open(fileName);
	}

	// If succeed : Get file content, print size + content, tokenize & call Message::encryptWord().
	inFile.getline(contentArray, 1000);
	inFile.close();

	// Preview file content
	cout << endl;
	cout_color("[ File Found! ]\n", "green");
	cout << endl;
	cout << "File size: " << strlen(contentArray) << " characters.\n";
	cout << endl;
	cout << "┌──────────────────────────────────── Message Preview ────────────────────────────────────┐\n";
	
	// Print content array but go down new line if run out of lines + printing animation
	chrono::milliseconds timespan(1);
	for (int i = 0; i < strlen(contentArray); ++i)
	{
		if (i % 91 == 0)
			cout << endl;
		cout << contentArray[i];
		std::this_thread::sleep_for(timespan);
	}
	cout << endl << endl;
	cout << "└─────────────────────────────────────────────────────────────────────────────────────────┘\n";
	cout << endl;

	// Pause for viewing
	wait();
}

// Create new file for encrypted message
void makeFile(Message& m, string action)
{
	string fileName;
	int n;
	
	// Preview encrypted content
	cout << endl;
	cout << "┌──────────────────────────────────── Message Preview ────────────────────────────────────┐\n";
	m.showWords();
	cout << endl;
	cout << endl;
	cout << "└─────────────────────────────────────────────────────────────────────────────────────────┘\n";

	// Display message
	cout << endl;
	if (action == "encrypt")
		cout << "A new file will be created for the encrypted message.\n";
	else
		cout << "A new file will be created for the decrypted message.\n";

	// "Please enter file name"
	cout << endl;
	cout << "Please enter the name of the new file to create: ";
	getline(cin, fileName);

	n = fileName.size();

	// [ERROR CHECK] If file name not at least "?.txt" : "Try again!"
	while (n < 5 || (fileName[n - 4] != '.' || fileName[n - 3] != 't' || fileName[n - 2] != 'x' ||fileName[n - 1] != 't'))
	{
		cout_color("File name is invalid. Enter another file name: ", "yellow");
		getline(cin, fileName);
		n = fileName.size();
	}
	
	// Make file
	m.makeFile(fileName);

	// "File created!"
	cout << endl;
	string message = "[ File \"" + fileName + "\" Created Successfully! ]\n";
	cout_color(message, "green");
	cout << endl;

	// Pause for viewing
	wait();
}

// Break char array into word tokens and encrypt each
void encryptFile(Message& m, char contentArray[])
{
	// A fun en/decrypting animation
	pinkArrowAnimation();
	
	char seps[] = " ,\t\n";
	char* token{ nullptr };
	char* next_token{ nullptr };

	// Establish string and get the first token:
	token = strtok_s(contentArray, seps, &next_token);
		 /*'contentArray' : array name is a pointer to 1st word/"token" in the array
		 * 'seps'		  : tells strtok_s() where the cut off point is for the 1st word/"token"
		 * '&next_token'  : pointer to the next word/"token" in the array (will be updated to point to the next position by the function)*/
		
		/* strtok_s skips leading delimiters, find the next delimiter, insert NULL there.
		 * and now we have words from a sentence!
		 * EXP: "This is a test." --> strtok_s(...) --> "This\0is\0a\0test\0"*/

	// While this token is NOT the last one in "contentArray" (array not ended)
	while (token != NULL)
	{
		// Pass current tokenPtr (which we know is NOT the last/NULL) into Message::encryptWord():
		m.encryptWord(token);
		
		// Get next token:
		token = strtok_s(NULL, seps, &next_token);
			// 'NULL'	  : tells strtok_s() to find next token after the most recent NULL.
	}
}

// Break char array into word tokens and decrypt each
void decryptFile(Message& m, char contentArray[])
{
	// A fun en/decrypting animation
	pinkArrowAnimation();

	char seps[] = " ,\t\n";
	char* token{ nullptr };
	char* next_token{ nullptr };

	// Establish string and get the first token:
	token = strtok_s(contentArray, seps, &next_token);

	// While this token is NOT the last one in "contentArray" (array not ended)
	while (token != NULL)
	{
		// Pass current tokenPtr (which we know is NOT the last/NULL) into Message::decryptWord():
		m.decryptWord(token);

		// Get next token:
		token = strtok_s(NULL, seps, &next_token);
	}
}

// Fun loading screen for encryption/decryption
void pinkArrowAnimation()
{
	chrono::milliseconds timespan(500);
	cout << "\033[35m";
	for (int i = 0; i < 4; ++i)
	{
		cout << "                                             |" << endl;
		std::this_thread::sleep_for(timespan);
	}
	cout << "                                             V" << endl;
	cout << "\033[0m";
}

// Cout in color
void cout_color(string coutMessage, string color)
{
	if (color == "blue_flashing")
		cout << "\033[36;5m";
	else if (color == "red")
		cout << "\033[31m";
	else if (color == "green")
		cout << "\033[32m";
	else if (color == "yellow")
		cout << "\033[33m";
	else if (color == "blue")
		cout << "\033[36m";
	else if (color == "pink")
		cout << "\033[95m";

	cout << coutMessage << "\033[0m";
}

// Pause and wait for the user
void wait()
{
	//The following if-statement checks to see how many characters are in cin's buffer
	//If the buffer has characters in it, the clear and ignore methods get rid of them.
	if (cin.rdbuf()->in_avail() > 0) //If the buffer is empty skip clear and ignore
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Clear the input buffer
	}
	char ch;
	cout << "Press the Enter key to continue ... ";
	cin.get(ch);
}