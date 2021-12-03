#pragma once
class TextParser{
public:
	std::string CharacterArrayToString(char* obj, int length);
	std::string Split(std::string str, int index);
	std::string HexConvert(char* buffer, int count);
};

