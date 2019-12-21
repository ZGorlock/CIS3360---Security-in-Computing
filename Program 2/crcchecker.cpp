//------------------------------------------------------------------

// University of Central Florida

// CIS3360 - Summer 2015

// Program Author: Zachary Gill

//------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <iostream>

using namespace std;

string calculateCrc(string input, string polynomial, bool echo);

string trimLeadingZeros(string str);
string formatBinString(string binString);

string hexStringToBinString(string hexString);
string hexCharToBinString(char hexChar);
string binStringToHexString(string binString);
char binStringToHexChar(string binString);

string binStringXor(string binString1, string binString2);

int main(int argc, char *argv[])
{
  string mode = string(argv[1]);
  string input = string(argv[2]);

  const string polynomial = "1010010101000";

  //test mode
  if (mode != "c" && mode != "v") {
    cout << "The mode you entered is not valid, the program will now close!" << endl;
    return 1;
  }

  //set variables
  string binInput = hexStringToBinString(input);
  string crc = "";
  string binCrc = "";
  if (mode == "v") {
    crc = input.substr(input.length() - 3);
    binCrc = hexStringToBinString(crc);
  }

  //header output
  cout << "CRC Checker by " << "Zachary Gill" << endl << endl
    << "Mode of operation: ";
  if (mode == "c")
    cout << "calculate";
  else
    cout << "verify";
  cout << endl
    << "The input string (hex): " << input << endl
    << "The input string (bin): " << formatBinString(binInput) << endl << endl
    << "The polynomial that was used (binary bit string): " << formatBinString(polynomial) << endl;
  if (mode == "c")
    cout << "We will append 12 zeroes at the end of the binary input.";
  else
    cout << "The 12-bit CRC observed at the end of the input: " << formatBinString(binCrc) << " (bin) = " << crc << " (hex)";
  cout << endl << endl;

  //pad input
  binInput += "000000000000";

  if (mode == "c") { //calculate mode
    //calculate crc / intermediate output
    crc = calculateCrc(binInput, polynomial, true);
    binCrc = hexStringToBinString(crc);
    
    //final output
    cout << endl
      << "The computed CRC for the input is: " << formatBinString(binCrc) << " (bin) = " << crc << " (hex)";
  }
  else { //verify mode
    //intermediate output
    calculateCrc(hexStringToBinString(input), polynomial, true);
    
    //calculate crc
    string newInput = input.substr(0, input.length() - 3);
    string newBinInput = hexStringToBinString(newInput) + "000000000000";
    string newCrc = calculateCrc(newBinInput, polynomial, false);
    string newBinCrc = hexStringToBinString(newCrc);

    //final output
    cout << endl
      << "The computed CRC for the input is: " << formatBinString(newBinCrc) << " (bin) = " << newCrc << " (hex)" << endl << endl
      << "Did the CRC check pass? (Yes or No): ";
    if (newCrc == crc)
      cout << "Yes";
    else
      cout << "No";
  }

  return 0;
}

string calculateCrc(string input, string polynomial, bool echo)
{
  string intStr = input;
  string tmpStr = "";

  if (echo) {
    cout << "The binary string difference after each XOR step of the CRC calculation:" << endl << endl
      << formatBinString(intStr) << endl;
  }

  while (trimLeadingZeros(intStr).length() > 12) {
    tmpStr = "";
    bool hit = false;
    for (int i = 0; i < (int)intStr.length(); i++) {
      if (hit) {
        tmpStr += "0";
      }
      else {
        if (intStr[i] == '1') {
          tmpStr += polynomial;
          i += polynomial.length() - 1;
          hit = true;
        }
        else
          tmpStr += "0";
      }
    }

    intStr = binStringXor(intStr, tmpStr);
    if (echo)
      cout << formatBinString(intStr) << endl;
  }

  return binStringToHexString(intStr.substr(intStr.length() - 12));
}

string trimLeadingZeros(string str)
{
  for (int i = 0; i < (int)str.length(); i++) {
    if (str[i] == '1')
      return str.substr(i);
  }
  return "";
}

string formatBinString(string binString)
{
  string out = "";
  for (int i = 0; i < (int)binString.length(); i++) {
    out += binString[i];
    if ((i + 1) % 4 == 0 && i != (int)binString.length() - 1)
      out += " ";
  }
  return out;
}

string hexStringToBinString(string hexString)
{
  string binString = "";
  for (int i = 0; i < (int)hexString.length(); i++)
    binString += hexCharToBinString(hexString[i]);
  return binString;
}

string hexCharToBinString(char hexChar)
{
  switch (hexChar) {
    case '0': return "0000";
    case '1': return "0001";
    case '2': return "0010";
    case '3': return "0011";
    case '4': return "0100";
    case '5': return "0101";
    case '6': return "0110";
    case '7': return "0111";
    case '8': return "1000";
    case '9': return "1001";
    case 'A': return "1010";
    case 'B': return "1011";
    case 'C': return "1100";
    case 'D': return "1101";
    case 'E': return "1110";
    case 'F': return "1111";
  }
  return "0000";
}

string binStringToHexString(string binString) //binString must have a length that is a multiple of 4
{
  string hexString = "";
  for (int i = 0; i < (int)binString.length(); i++) {
      hexString += binStringToHexChar(binString.substr(i, 4));
    i += 3;
  }
  return hexString;
}

char binStringToHexChar(string binString)
{
  int value = 8 * (binString[0] == '1') +
    4 * (binString[1] == '1') +
    2 * (binString[2] == '1') +
    1 * (binString[3] == '1');
  switch (value) {
    case 0: return '0';
    case 1: return '1';
    case 2: return '2';
    case 3: return '3';
    case 4: return '4';
    case 5: return '5';
    case 6: return '6';
    case 7: return '7';
    case 8: return '8';
    case 9: return '9';
    case 10: return 'A';
    case 11: return 'B';
    case 12: return 'C';
    case 13: return 'D';
    case 14: return 'E';
    case 15: return 'F';
  }
  return '0';
}

string binStringXor(string binString1, string binString2)
{
  string xorString = "";
  for (int i = 0; i < (int)binString1.length(); i++) {
    if ((binString1[i] == '1' || binString2[i] == '1') && (binString1[i] == '0' || binString2[i] == '0'))
      xorString += '1';
    else
      xorString += '0';
  }
  return xorString;
}