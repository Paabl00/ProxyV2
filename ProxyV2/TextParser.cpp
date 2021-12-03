#include "includes.h"

std::string TextParser::CharacterArrayToString(char* obj, int length) {
	return std::string(obj, 0, length);
}

std::string TextParser::Split(std::string str, int index) {
	str.insert(index, " ");
	return str;
}

std::string TextParser::HexConvert(char* buffer, int count) {
	std::stringstream stream;
	for (int i = 0; i < count; i++) {
		stream << std::hex << (int)buffer[i];
	}
	return stream.str();
}

/*
std::string printAsHex(char* buffer) {
	std::stringstream stream;
	for (int i = 0; i < 20; i++) {
		stream << std::hex << (int)buffer[i];
	}
	return stream.str();
}
*/