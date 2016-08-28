#include "langDefinition.h"
#include "preprocess.h"

#ifdef DEBUG
#	include <iostream>
#endif

static map<regex*, size_t> wordCountLookup = {
 	{&AInstr, 1},
 	{&CInstr, 1},
 	{&Label, 0},
	{&Nothing, 0}
};

static int getWordsInLine(string line) {
	for(auto kvp : wordCountLookup) {
		if(regex_match(line, *kvp.first))
			return kvp.second;
	}
	return 0;
}

SymbolTable getSymbols(InstructionList lines) {
	auto table = SymbolTable {
		{ "R0", 0 },
		{ "R1", 1 },
		{ "R2", 2 },
		{ "R3", 3 },
		{ "R4", 4 },
		{ "R5", 5 },
		{ "R6", 6 },
		{ "R7", 7 },
		{ "R8", 8 },
		{ "R9", 9 },
		{ "R10", 10 },
		{ "R11", 11 },
		{ "R12", 12 },
		{ "R13", 13 },
		{ "R14", 14 },
		{ "R15", 15 },
		{ "SP", 0 },
		{ "LCL", 1 },
		{ "ARG", 2 },
		{ "THIS", 3 },
		{ "THAT", 4 },
		{ "SCREEN", 16384 },
		{ "KBD", 24567 }
	};
	auto lineCount = 0;
	for(auto line : lines) {
		auto linePP = Preprocess(line);
		auto res = match{};
		if(regex_match(linePP, res, Label)) {
			table[res[1]] = lineCount;
#ifdef DEBUG
		std::cout << "added label (" << lineCount << "): " <<res[1].str() << std::endl;
#endif
		}
		auto words = getWordsInLine(linePP);
		lineCount += words;
	}

	auto newVariableAddress = 16;
	for(auto line : lines) {
		auto linePP = Preprocess(line);
		auto res = match{};
		if (
				regex_match(linePP, res, AInstr)
				&& !regex_match(res[1].str(), Number)
				&& table.find(res[1].str()) == table.end()
		) {
#ifdef DEBUG
			std::cout << "new variable (" << newVariableAddress << "): " << res[1].str() << std::endl;
#endif
			table[res[1]] = newVariableAddress;
			++newVariableAddress;
		}
	}
	return table;
}
