//------------------------------------------------------------------

// University of Central Florida

// CIS3360 - Summer 2015

// Program Author: Zachary Gill

//------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

bool isAlphaChar(char);
char toLowerCase(char);
void printBlock(string);
string padPlaintext(string, int);
string encrypt(string, string, string);
string encryptBlock(string, string);
char charXOR(char, char);

int main(int argc, char *argv[])
{
  string plaintextFile = string(argv[1]);
  string vigenereKey = string(argv[2]);
  string initVector = string(argv[3]);

  cout << "Plaintext file name: " << plaintextFile << endl
    << "Vigenere keyword: " << vigenereKey << endl
    << "Initialization vector: " << initVector << endl << endl;

  char tmpChar;
  string plaintext = "";

  fstream fs;
  fs.open(argv[1], fstream::in);
  while (!fs.eof()) {
    fs.get(tmpChar);
    if (isAlphaChar(tmpChar))
      plaintext += toLowerCase(tmpChar);
  }
  fs.close();

  cout << "Clean Plaintext:" << endl << endl;
  printBlock(plaintext);
  cout << endl;


  string paddedPlaintext = padPlaintext(plaintext, vigenereKey.length());
  string ciphertext = encrypt(paddedPlaintext, vigenereKey, initVector);


  cout << "Ciphertext:" << endl << endl;
  printBlock(ciphertext);
  cout << endl;

  cout << "Number of characters in clean plaintext file: " << plaintext.length() << endl;
  cout << "Block size = " << vigenereKey.length() << endl;
  cout << "Number of pad characters added: " << paddedPlaintext.length() - plaintext.length();

  return 0;
}

bool isAlphaChar(char c)
{
  if (c >= 65 && c <= 90)
    return true;
  if (c >= 97 && c <= 122)
    return true;
  return false;
}

char toLowerCase(char c)
{
  if (c >= 65 && c <= 90)
    return (char)(c + 32);
  return c;
}

void printBlock(string text)
{
  int l = 0;

  for (int i = 0; i < (int)text.length(); i++) {
    cout << text[i];
    l++;
    if (l == 80) {
      cout << endl;
      l = 0;
    }
  }

  cout << endl;
}

string padPlaintext(string plaintext, int blockLength)
{
  string paddedPlaintext = plaintext;
  int padCount = blockLength - (plaintext.length() % blockLength);

  for (int i = 0; i < padCount; i++)
    paddedPlaintext += 'x';

  return paddedPlaintext;
}

string encrypt(string plaintext, string vigenereKey, string initVector)
{
  string ciphertext = "";
  string C = initVector;
  int CLength = C.length();

  int index = 0;
  string block = plaintext.substr(index, CLength);
  while (block.length() == CLength) {
    C = encryptBlock(encryptBlock(block, C), vigenereKey);
    ciphertext += C;
    index += CLength;
    block = plaintext.substr(index, CLength);
  }

  return ciphertext;
}

string encryptBlock(string block, string vector)
{
  string C = "";

  for (int i = 0; i < (int)vector.length(); i++)
    C += charXOR(block[i], vector[i]);

  return C;
}

char charXOR(char a, char b)
{
  return (char)((((a - 'a') + (b - 'a')) % 26) + 'a');
}