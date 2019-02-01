# Steganography

Steganography is the practice of concealing a file, message, image, or video within another file, message, image, or video. ([read more about it on Wikipedia](https://en.wikipedia.org/wiki/Steganography)).

This project was proposed by Professor Allen and his COP 2001 - Programming Methodology class.

The language used is **C++**.

## Project Description

This project is aimed towards using the concept of steganography to encrypt a string of text into a bitmap image file, and then decrypt the text from the encrypted image. Additionally, there is a menu that allows the user to choose which function to run (between encrypt and decrypt), along with the ability to exit.

The entire process works by switching the blue component of an RGB pixel off, so that over 8 pixels we can encode 1 character of the string message (1 char = 1 byte). At the end of the string, padding is added in order to be able to know when to stop reading the bitmap file. Once the message is encoded in the picture, the user is not going to be able to notice any changes from the original (although information has been added).

This practice can work especially well if the starting message is also encrypted.

Note: The bytes and bitmap handling files were provided by the instructor.

## Built With

* [Microsoft Visual Studio](https://visualstudio.microsoft.com/) - IDE of choice

## Authors

* **Luca Missaglia** - *Project Developer* - [Lollators](https://github.com/Lollators)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
