#include "parser.h"

#include <regex>
#include <bitset>

#include <iostream>

using regex = std::regex;
using match = std::smatch;
template<int n>
using bitset = std::bitset<n>;

static regex Nothing { R"(^$)" };
static regex Label  { R"(^\(([a-zA-Z._$][a-zA-Z._$0-9]*)\)$)" };
static regex AInstr { R"(^@([a-zA-Z0-9._$]+)$)" };
static regex CInstr { R"(^(.*=.*;.*|.*;.*|.*=.*)$)" };

static regex Number { R"([1-9][0-9]*|0)" };

static regex CInstrNoJMP  { R"(^(.*)=(.*)$)" };
static regex CInstrNoSET  { R"(^(.*);(.*)$)" };
static regex CInstrJMPSET { R"(^(.*)=(.*);(.*)$)" };

static regex CInstrDST    { R"(^(A)?(M)?(D)?$)" };

static regex JMPLT { R"(JLT|JNE|JLE|JMP)" };
static regex JMPEQ { R"(JEQ|JGE|JLE|JMP)" };
static regex JMPGT { R"(JGT|JGE|JNE|JMP)" };

// regex* because regex doesn't have operator< defined, which is needed
static map<regex*, size_t> wordCountLookup = {
 	{&AInstr, 1},
 	{&CInstr, 1},
 	{&Label, 0},
	{&Nothing, 0}
};

static regex CInstrZero    { R"(^0$)" };
static regex CInstrOne     { R"(^1$)" };
static regex CInstrNegOne  { R"(^-1$)" };
static regex CInstrD       { R"(^D$)" };
static regex CInstrAM      { R"(^[AM]$)" };
static regex CInstrNotD    { R"(^!D$)" };
static regex CInstrNotAM   { R"(^![AM]$)" };
static regex CInstrNegD    { R"(^-D$)" };
static regex CInstrNegAM   { R"(^-[AM]$)" };
static regex CInstrIncD    { R"(^D+1$)" };
static regex CInstrIncAM   { R"(^[AM]+1$)" };
static regex CInstrDecD    { R"(^D-1$)" };
static regex CInstrDecAM   { R"(^[AM]-1$)" };
static regex CInstrSum     { R"(^D+[AM]$)" };
static regex CInstrDLessAM { R"(^D-[AM]$)" };
static regex CInstrAMLessD { R"(^[AM]-D$)" };
static regex CInstrDAndAM  { R"(^D&[AM]$)" };
static regex CInstrDOrAM   { R"(^D[|][AM]$)" };

static map<regex*, bitset<16>> ControlList = {
	{ &CInstrZero,    0b101010 << 6},
	{ &CInstrOne,     0b111111 << 6},
	{ &CInstrNegOne,  0b111010 << 6},
	{ &CInstrD,       0b001100 << 6},
	{ &CInstrAM,      0b110000 << 6},
	{ &CInstrNotD,    0b001101 << 6},
	{ &CInstrNotAM,   0b110001 << 6},
	{ &CInstrNegD,    0b001111 << 6},
	{ &CInstrNegAM,   0b110011 << 6},
	{ &CInstrIncD,    0b011111 << 6},
	{ &CInstrIncAM,   0b110111 << 6},
	{ &CInstrDecD,    0b001110 << 6},
	{ &CInstrDecAM,   0b110010 << 6},
	{ &CInstrSum,     0b000010 << 6},
	{ &CInstrDLessAM, 0b010011 << 6},
	{ &CInstrAMLessD, 0b000111 << 6},
	{ &CInstrDAndAM,  0b000000 << 6},
	{ &CInstrDOrAM,   0b010101 << 6}
};

static int const CInstrBase = 0xE000;

static int const IndirectionBit{12};

static int const ZXBit    {11};
static int const NXBit    {10};
static int const ZYBit    {9};
static int const NYBit    {8};
static int const FBit     {7};
static int const ZOBit    {6};

static int const DstABit  {5};
static int const DstDBit  {4};
static int const DstMBit  {3};

static int const JMPLTBit {2};
static int const JMPEQBit {1};
static int const JMPGTBit {0};

int getWordsInLine(string line) {
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
		auto res = match{};
		if(regex_match(line, res, Label)) {
			table[res[1]] = lineCount;
		}
		auto words = getWordsInLine(line);
		lineCount += words;

#ifdef DEBUG
		std::cout << lineCount << "\t" << line << "\t#" << words << std::endl;
#endif
	}
	return table;
}

string assembleAInstr(string const& line, SymbolTable const& symbols) {
#ifdef DEBUG
	std::cout << "AInstr ";
#endif
	match m;
	regex_match(line, m, AInstr);
	auto arg = 0u;

	if(regex_match(m[1].str(), Number))
		arg = stoi(m[1].str());
	else if(symbols.find(m[1].str()) != symbols.end()) {
		arg = symbols.at(m[1].str());
	} else {
		// ???
		return "";
	}

	bitset<16> rep(arg);
	rep[15]=0;

	return rep.to_string();
}

bitset<16> assembleControlFlags(string VAL) {
	for(auto kvp : ControlList) {
		if(regex_match(VAL, *kvp.first)) {
			return kvp.second;
		}
	}
	// ???
	return bitset<16> {0};
}

string assembleCInstr(string const& line, SymbolTable const& symbols) {
#ifdef DEBUG
	std::cout << "CInstr ";
#endif
	// categorise into JMP/SET/BOTH
	string SET{};
	string VAL{};
	string JMP{};
	match m;
	if(regex_match(line, m, CInstrJMPSET)) {
		SET = m[1].str();
		VAL = m[2].str();
		JMP = m[3].str();
	} else if(regex_match(line, m, CInstrNoJMP)) {
		SET = m[1].str();
		VAL = m[2].str();
	} else if(regex_match(line, m, CInstrNoSET)) {
		VAL = m[1].str();
		JMP = m[2].str();
	} else {
		// ???
	}

	bitset<16> instruction(CInstrBase);
	// set DST flags
	// TODO replace with a function
	if(SET.length() != 0) {
		if(!regex_match(SET, m, CInstrDST)) {
			// ??? DST is garbage
			return "";
		} else if(m[1].length() + m[2].length() + m[3].length() == 0) {
			// ??? DST doesn't have A, M, or D set
			return "";
		}
		instruction[DstABit] = m[1].length() != 0;
		instruction[DstMBit] = m[2].length() != 0;
		instruction[DstDBit] = m[3].length() != 0;
	}

	// set indirection flag
	instruction[IndirectionBit] = regex_match(VAL, regex(R"(.*M.*)"));

	// set control flags
	instruction |= assembleControlFlags(VAL);

	// set jump flags
	instruction[JMPLTBit] = regex_match(JMP, JMPLT);
	instruction[JMPEQBit] = regex_match(JMP, JMPEQ);
	instruction[JMPGTBit] = regex_match(JMP, JMPGT);

	//output
	return instruction.to_string();
}

string assembleLine(string const& line, SymbolTable const& symbols) {
	static map<regex*, string(*)(string const&, SymbolTable const&)> instrAssembler = {
		{ &AInstr, &assembleAInstr },
		{ &CInstr, &assembleCInstr }
	};
	auto res = match{};
	for(auto kvp : instrAssembler) {
		if(regex_match(line, res, *kvp.first))
			return kvp.second(line, symbols);
	}
#ifdef DEBUG
	std::cout << "Unknown: \"" << line << "\"";
#endif
	return "";
}

string assembleFile(InstructionList const& program) {
	auto symbols = getSymbols(program);
	string output;
	auto lineNum = 0;
	for(auto line : program) {
		++lineNum;
		auto assembledLine = assembleLine(line, symbols);
		if(assembledLine.length() > 0)
			output += assembledLine + "\n";
#ifdef DEBUG
		std::cout << "-> " << assembledLine << std::endl;
#endif
	}
	return output;
}

