/******************************************************************************
 * File:		 steganography.cpp
 * Author:	 Missaglia, Luca
 * Created:	 Apr 13, 2018
 * Course:	 FGCU, COP 2001, 201801, 10410
 * Description: Use the concept of steganography to encrypt a string of text
 * 				into a bitmap image file, and then decrypt the text from the
 * 				encrypted image. Incorporate a menu of encrypt and decrypt
 * 				functions to run, plus the ability to exit.
 *****************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>							// Header for pow function
#include "bitmap.h"
using namespace std;

//declare global enum RUN_MODE and variable of its type
enum RUN_MODE {ENCRYPT = 1, DECRYPT = 2, EXIT = 3};

RUN_MODE getRunMode();
string getTextToEncrypt();
string readString();
int getImageFileIn(ifstream& fIn, string& filePathIn);
int getImageFileOut(ofstream& fOut, string& filePathOut);
int encryptString(string textToEncrypt, BITMAPINFO& bi);
string decimalToBinary(int decimal);
string valueToDigit(int value);
int toDecimal(string strNumberIn);
int digitToValue(char digit, int base);
int decryptString(string& decryptedString, BITMAPINFO bi);


int main(){
	// Run program until user enters 3 (or EXIT)
	RUN_MODE myRun;

	do{
		myRun = getRunMode();

		if(myRun == ENCRYPT){
			ifstream fIn;
			ofstream fOut;
			string filePathIn, filePathOut, textToEncrypt = "";
			BITMAP bmap;
			int bytesRead, bytesWritten, pixelsEncrypted;

			cout << "Begin Encrypting Image:" << endl;
			cout << endl;

			textToEncrypt = getTextToEncrypt();

			// Propagation of error from functions
			if(getImageFileIn(fIn, filePathIn) == -1){
				return -1;
			}

			// Read input bitmap from specified path
			bytesRead = readBitmap(fIn, bmap);

			// Encrypt string in BITMAPINFO
			pixelsEncrypted = encryptString(textToEncrypt, bmap.bmi);

			// Propagation of error from encryptString()
			if(pixelsEncrypted == -1){
				return -1;
			}

			if(getImageFileOut(fOut, filePathOut) == -1){
				return -1;
			}

			// Use BITMAPINFO to create new bitmap at specified path
			bytesWritten = writeBitmap(fOut, bmap);

			cout << "Encrypted Image: " << filePathIn << endl;
			cout << "       Bytes Read: " << bytesRead << endl;
			cout << "       Pixels Encrypted: " << pixelsEncrypted << endl;
			cout << "Created Image: " << filePathOut << endl;
			cout << "       Bytes Written: " << bytesWritten << endl;
			cout << endl;

			// Close streams
			fIn.close();
			fOut.close();
		}
		else if(myRun == DECRYPT){
			ifstream fIn;
			string filePathIn, encryptedString;
			BITMAP bmap;
			int bytesRead, pixelsEncrypted;

			cout << "Begin Decrypting Image: " << endl;
			cout << endl;

			// Propagation of error from function
			if(getImageFileIn(fIn, filePathIn) == -1){
				return -1;
			}

			// Read input bitmap from specified path
			bytesRead = readBitmap(fIn, bmap);

			// Decrypt string from specified bitmap
			pixelsEncrypted = decryptString(encryptedString, bmap.bmi);

			cout << "Decrypted Image: " << filePathIn << endl;
			cout << "       Bytes Read: " << bytesRead << endl;
			cout << "       Pixels Encrypted: " << pixelsEncrypted << endl;
			cout << "Encrypted String: " << encryptedString << endl;
			cout << endl;

			// Close stream
			fIn.close();
		}

	}while(myRun != EXIT);
	return 0;
}

/**
 * Displays the user menu, then inputs and returns the run mode
 * selected. Reads an integer and then casts the number into
 * a RUN_MODE type and returns it.
 *
 * mode - int - mode selected by user. Can be either 1, 2, or 3.
 *
 * return - RUN_MODE - mode
 */
RUN_MODE getRunMode(){
	int mode;

	// Get mode from user input, as long as it is either 1, 2, or 3.
	// Otherwise asks again.
	do{
		cout << "Select the run mode: " << endl;
		cout << "         1) Encrypt" << endl;
		cout << "         2) Decrypt" << endl;
		cout << "         3) Exit" << endl;
		cout << "=>";
		cin >> mode;
	}while(mode != 1 && mode != 2 && mode != 3);

	// Since cin >> mode reads a number there's a '/n' left on the stream
	// that can be fetched by the next cin. Using cin.ignore() takes care of it.
	cin.ignore();

	return (RUN_MODE)(mode);
}

/**
 * Reads in a string from the standard input stream into a string and
 * returns the string to the caller.
 *
 * myString - string - variable that holds the string read from user
 *
 * return - string - myString
 */
string readString(){
	string myString;

	getline(cin, myString);

	return myString;
}

/**
 * Prompts the user for the text to insert into the image and
 * returns it as a string. Call readString.
 *
 * myString - string - variable that holds the string read from user
 *
 * return - string - myString
 */
string getTextToEncrypt(){
	cout << "Enter the text to encrypt: ";

	string myString = readString();

	return myString;
}

/**
 * Prompts the user for the name (with path if required) of a bitmap
 * file to open, then returns an input file stream and the name of the file
 * the user enters through the output parameters, and returns an integer
 * 0 if the file-open succeeds or -1 if it fails. Returns -1 if it fails
 * and displays an error message. Call readString.
 * Both parameters are called by reference since there is a need for multiple
 * returns.
 *
 * fIn - ifstream - input file stream
 * filePathIn - string - name (with path) of the input file

 * return - int - status of the file-open
 */
int getImageFileIn(ifstream& fIn, string& filePathIn){
	cout << "Enter the name (with path) of image file to read: ";
	filePathIn = readString();

	fIn.open(filePathIn, ios::binary);
	if(fIn.fail()){
		cerr << "Failed to open file: " << filePathIn << endl;
		return -1;
	}

	return 0;
}

/**
 * Prompts the user for the name (with path if required) of a bitmap
 * file to create, then returns an output file stream and the name of the file
 * entered by user through output parameters, and returns an integer 0 if the
 * the file-open succeeds or -1 if it fails and displays an error message.
 * Call readString.
 * Both parameters are called by reference since there is a need for multiple
 * returns.
 *
 * fOut - ofstream - output file stream
 * filePathOut - string - name (with path) of the file to create
 *
 * return - int - status of the file-open
 */
int getImageFileOut(ofstream& fOut, string& filePathOut){
	cout << "Enter the name (with path) of image file to write: ";
	filePathOut = readString();

	fOut.open(filePathOut, ios::binary);
	if(fOut.fail()){
		cerr << "Failed to open file: " << filePathOut << endl;
		return -1;
	}

	return 0;
}

/**
 * Receives the string to encrypt as a parameter and the BITMAPINFO
 * structure as an output parameter, and returns the number of pixels
 * used to encrypt the text. The BITMAPINFO is passed by reference
 * as the function writes the least significant blue bit. This
 * BITMAPINFO is then used to create the output file in main.
 * Checks that string length + padding do not exceed number of pixels in bitmap.
 *
 * pixelCount - int - counter of encrypted pixels (counts footer as well)
 * newRgbBlue - int - new decimal value for blue portion of rgb pixel
 * asciiDecimal - int - decimal value of single char digit of message
 * count - int - counter to keep track of where the function is at
 * 				 when it writes bits into blue portion of rgb pixel
 * rgbBlueBinary - string - binary string of decimal value of blue
 * 						   portion of rgb pixel
 * asciiBinary - string - binary string (8 bits) of decimal value
 * 						 of single char digit of message
 *
 * return - int - pixelCount
 */
int encryptString(string textToEncrypt, BITMAPINFO& bi){
	int pixelCount = 0, newRgbBlue, asciiDecimal, count = 0;
	string rgbBlueBinary, asciiBinary;

	// Checks if length of the string is too long to be encrypted in the
	// bitmap (not enough pixels). Each char is 8 bits, hence the total number
	// of pixels has to be divided by 8. The -8 is for the 8 bits end of string padding.
	if(textToEncrypt.length() < ((bi.bmiHeader.biHeight*bi.bmiHeader.biWidth)/8)-8){

		// First for loop goes through the entire message to be encrypted
		for(int i = 0; i < textToEncrypt.length(); i++){

			// Cast char value into decimal
			asciiDecimal = (int) textToEncrypt[i];

			// Converts decimal value to binary string.
			asciiBinary = decimalToBinary(asciiDecimal);

			// Second for loop goes through each character binary string
			// and copies each bit into least significant bit of blue part of rgb pixel
			for(int j = 0; j < 8; j++ ){

				// Converts decimal value of blue into binary string
				rgbBlueBinary = decimalToBinary(bi.bmiColors[j+count].rgbBlue);

				// Writes on least significant bit the new bit (got from character binary string)
				rgbBlueBinary[7] = asciiBinary[j];

				// Converts back the binary string to decimal
				newRgbBlue = toDecimal(rgbBlueBinary);

				// Re-assigns the decimal value to the blue part of rgb pixel
				bi.bmiColors[j+count].rgbBlue = newRgbBlue;

				// Increments pixel count
				pixelCount++;
			 }

			 // Count increases by 8 at each cycle, since each char = 8 pixels
			 count += 8;
		 }

		 // Adds 8 bits of zeros to create a footer for the encrypted message
		 for(int i = count; i < count+8; i++){
			rgbBlueBinary = decimalToBinary(bi.bmiColors[i].rgbBlue);
			rgbBlueBinary[7] = '0';
			newRgbBlue = toDecimal(rgbBlueBinary);
			bi.bmiColors[i].rgbBlue = newRgbBlue;

			pixelCount++;
		 }
	} // end if statement
	else{
		cout << "String is too big to be encrypted in the bitmap!" << endl;
		return -1;
	}

	return pixelCount;
}

/**
 * Receives the string that will be decrypted and the BITMAPINFO structure
 * as an output parameter, and returns the number of pixels used to
 * encrypt the text.
 *
 * pixelCount - int - counter of encrypted pixels (counts footer as well)
 * zeroFlag - int - counter of how many zeros are found
 * i - int - counter to keep track of where in the rgb array the function is
 * rgbBlueBinary - string - binary string of decimal value of blue
 * 						   portion of rgb pixel
 * message - string - binary string that represents encrypted message
 *
 * return - int - pixelCount
 */
int decryptString(string& decryptedString, BITMAPINFO bi){
	int pixelCount = 0, zeroFlag = 0, i = 0;
	string rgbBlueBinary, message = "";

	// While loop that goes until 8 zeros are found consecutively
	while(zeroFlag != 8){

		// Converts decimal value of blue into binary string
		rgbBlueBinary = decimalToBinary(bi.bmiColors[i].rgbBlue);

		// If least significant bit is zero, then increment by 1 zeroFlag
		// if it's not set it to 0 (and effectively resets the counting process)
		if(rgbBlueBinary[7] == '0'){
			zeroFlag++;
		}
		else{
			zeroFlag = 0;
		}

		// Add to the message the least significant bit of blue part of pixel
		message += rgbBlueBinary[7];

		// Increment both pixelCount and i by one
		pixelCount++;
		i++;
	}

	/**
	 * BUG FOUND! The binary string representing the last letter of encrypted
	 * message can end in zeros. These zeros are NOT to be considered part of
	 * the 8 bits footer previously encrypted in the bitmap. If not resolved,
	 * the last letter can be snapped off of the encrypted message.
	 *
	 * EXAMPLE: the binary of letter 'p' is 01110000. In a sentence such as "pump"
	 * the decoded message would be "pum". The last 4 bits of the last 'p' count
	 * towards the footer of the message.
	 *
	 * FIX: Since the message length has to be a multiple of 8 (as every letter
	 * is 8 bits) run a while loop using the modulus operator. If the remainder
	 * of message.length()%8 is not zero, then it means some bits got snapped off.
	 * Add as many zeros as needed to make the length of the string a multiple
	 * of 8 again.
	 */
	while(message.length()%8 != 0){
		message += '0';
		pixelCount++;
	}

	// Remove the 8 bits of footer at the end
	message = message.substr(0,(message.length() - 8));

	for(i = 0; i <= message.length()/8; i++){

		/**
		 * Take substring of 8 bits using counter of for loop to specify
		 * initial index, then convert it to a decimal number.
		 * Cast it to a character and add to the decryptedString variable.
		 */
		decryptedString += (char)(toDecimal(message.substr(i*8,8)));
	}

	return pixelCount;
}

/**
 * Convert a decimal number to a binary number.
 * The binary number has a length of 8 (bits)
 * Uses long division for algorithm.
 *
 * Req: pass the decimal number, and the base that it needs
 * 		to be converted into.
 *
 * remainder - remainder of the long division, at the end it
 * 			   has to be read backwards to give result
 * numberOut- string array that holds remainders (value of decimal
 * 			  converted in other base)
 *
 * return - string - numberOut
 */
string decimalToBinary(int decimal){
	int remainder = 0;
	string numberOut = "";

	do{														//use do-while in order to get value of decimal in first iteration
		remainder = decimal % 2;								//get remainder using modulus
		numberOut.insert(0,valueToDigit(remainder));			//appends remainder in first position of string array (backwards technique for long division)
		decimal = decimal / 2;								//divide decimal by the base that needs to be converted into

	//loop stops when you can't divide anymore (if decimal == 0)
	}while(decimal != 0);

	//if binary number has a length < 8 then add padding of zeros for most significant digits
	while(numberOut.length() < 8){
		numberOut.insert(0,"0");
	}


	return numberOut;
}

/**
 * Convert a integer value to character digit.
 * The return is made to a string on purpose
 * as the decimalToBinary function requires a string
 * to append remainders. The string array only holds 1
 * character.
 *
 * Req: pass the integer value that needs to be converted
 *
 * strDigit - string digit of the value passed in.
 *
 * return - string - strDigit.
 */
string valueToDigit(int value){
	string strDigit = "";

	strDigit = to_string(value);

	return strDigit;
}

/**
 * Convert a string number to decimal.
 * Uses positional notatiton for algorithm.
 *
 * Req: pass the string number and its base
 *
 * strNumberIn - string representing number passed in
 * base - integer base of the string
 *
 * return - int - decimal value of the string
 */
int toDecimal(string strNumberIn){
	int decimal = 0;

	for(int i = 0; i < strNumberIn.length(); i++){		//loops that goes through each char of the string array
		int digitPos = strNumberIn.length() - 1 - i;		//read digits right to left
		char digit = strNumberIn.at(digitPos);			//gets char at position digitPos of the string strNumberIn array
		int digValue = digitToValue(digit, 2);

		if(digValue == -1){								//propagation of error from digitToValue function
			return -1;
		}

		int posVal = pow(2, i);						//base to power
		int digPosVal = digValue * posVal;

		decimal = decimal + digPosVal;					//calculates total sum

	}

	return decimal;
}

/**
 * Convert a character digit to integer value
 *
 * Req: pass the character, function automatically
 * 		converts char to upper case (locally).
 *
 * digit - character digit passed in
 * base - integer base of the digit
 *
 * return - int - value of character digit
 */
int digitToValue(char digit, int base) {
	int value = 0;

	if(digit >= '0' && digit <= '9'){
		value = digit - '0';
	}
	else if(digit >= 'A' && digit <='F'){
		value = digit - 'A' + 10;
	}
	else{
		cout << "Invalid digit!" << endl;
		return -1;
	}

	if(value < 0 || value >= base){
		cout << "Invalid digit for the base!" << endl;
		return -1;
	}

	return value;
}
