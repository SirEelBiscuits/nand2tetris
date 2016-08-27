#include "parser.h"

#include <regex>

using regex = std::regex;
using match = std::smatch;

static regex Label  { R"(^\(([a-zA-Z._$][a-zA-Z._$0-9]*)\)$)" };
static regex AInstr { R"(^@([a-zA-Z._$][a-zA-Z._$0-9]*)$)" };
static regex CInstr { R"(^(.*=.*;.*|.*;.*|.*=.*)$)" };

static regex CInstrNoJMP  { R"(^(.*)=(.*)$)" };
static regex CInstrNoSET  { R"(^(.*);(.*)$)" };
static regex CInstrJMPSET { R"(^(.*)=(.*);(.*)$)" };

// regex* because regex doesn't have operator< defined, which is needed
static map<regex*, size_t> wordCountLookup = { {&AInstr, 1}, {&CInstr, 1}, {&Label, 0} };
static map<char, Word> regFlags = { {'A', 0x0}, {'M', 0x0}, {'D', 0x0} };

int getWordsInLine(string line) {
	for(auto kvp : wordCountLookup) {
		if(regex_match(line, *kvp.first))
			return kvp.second;
	}
	return -1;
}

SymbolTable getSymbols(InstructionList lines) {
	auto table = SymbolTable{};
	auto lineCount = 0;

	for(auto line : lines) {
		auto res = match{};
		if(regex_match(line, res, Label)) {
			table[res[1]] = lineCount;
		}

		lineCount += getWordsInLine(line);
	}
	return table;
}

string assembleFile(InstructionList program) {
	auto symbols = getSymbols(program);
	for(auto line : program) {
		auto res = match{};
		if(regex_match(line, res, AInstr)) {
			// output 100000000000000b + res[1]
		} else if(regex_match(line, res, CInstr)) {
			// categorise into JMP/SET/BOTH
			// set DST flags
			// set indirection flag
			// set control flags
			// set jump flags
			//output
		}
	}
	return "";
}

