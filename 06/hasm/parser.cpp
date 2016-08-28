#include "parser.h"

#include <regex>
#include <bitset>

#include <iostream>

#include "preprocess.h"
#include "langDefinition.h"

#include "symbolgen.h"

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

static int const DstABit  {5};
static int const DstDBit  {4};
static int const DstMBit  {3};

static int const JMPLTBit {2};
static int const JMPEQBit {1};
static int const JMPGTBit {0};

static string assembleAInstr(string const& line, SymbolTable const& symbols) {
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

static bitset<16> assembleControlFlags(string const& VAL) {
	for(auto kvp : ControlList) {
		if(regex_match(VAL, *kvp.first)) {
			return kvp.second;
		}
	}
	// ???
	return bitset<16> {0};
}

static bitset<16> assembleDSTFlags(string const& SET) {
	auto m = match{};
	auto ret = bitset<16>{0};
	if(!regex_match(SET, m, CInstrDST)) {
		//???
		return 0;
	} else if(m[1].length() + m[2].length() + m[3].length() == 0) {
		//???
		return 0;
	}
	ret[DstABit] = m[1].length() != 0;
	ret[DstMBit] = m[2].length() != 0;
	ret[DstDBit] = m[3].length() != 0;
	return ret;
}

static bitset<16> assembleJMPFlags(string const& JMP) {
	auto ret = bitset<16>{0};
	ret[JMPLTBit] = regex_match(JMP, JMPLT);
	ret[JMPEQBit] = regex_match(JMP, JMPEQ);
	ret[JMPGTBit] = regex_match(JMP, JMPGT);
	return ret;
}

static bitset<16> assembleIndirectionFlag(string const& VAL) {
	auto ret = bitset<16>{0};
	ret[IndirectionBit] = regex_match(VAL, regex(R"(.*M.*)"));
	return ret;
}

static string assembleCInstr(string const& line, SymbolTable const& symbols) {
#ifdef DEBUG
	std::cout << "CInstr ";
#endif
	// categorise into JMP/SET/BOTH
	match m;
	bitset<16> instruction(CInstrBase);

	// sub captures: 1: DST, 2: VAL, 3: JMP
	if(regex_match(line, m, CInstrJMPSET)) {
		instruction |= assembleDSTFlags(m[1].str());
		instruction |= assembleIndirectionFlag(m[2].str());
		instruction |= assembleControlFlags(m[2].str());
		instruction |= assembleJMPFlags(m[3].str());

	// sub captures: 1: DST, 2: VAL
	} else if(regex_match(line, m, CInstrNoJMP)) {
		instruction |= assembleDSTFlags(m[1].str());
		instruction |= assembleIndirectionFlag(m[2].str());
		instruction |= assembleControlFlags(m[2].str());

	// sub captures: 1: VAL, 2: JMP
	} else if(regex_match(line, m, CInstrNoSET)) {
		instruction |= assembleIndirectionFlag(m[1].str());
		instruction |= assembleControlFlags(m[1].str());
		instruction |= assembleJMPFlags(m[2].str());

	} else {
		// ???
		return "";
	}

	return instruction.to_string();
}

static string assembleLabel(string const &line, SymbolTable const& symbols) {
#ifdef DEBUG
	auto res = match{};
	regex_match(line, res, Label);
	std::cout << "Label: \"" << res[1].str() << "\"";
#endif
	return "";
}

static string assembleNothing(string const &line, SymbolTable const& symbols) {
#ifdef DEBUG
	std::cout << "Nothing ";
#endif
	return "";
}

static string assembleLine(string const& line, SymbolTable const& symbols) {
	static map<regex*, string(*)(string const&, SymbolTable const&)> instrAssembler = {
		{ &AInstr,  &assembleAInstr },
		{ &CInstr,  &assembleCInstr },
		{ &Label,   &assembleLabel  },
		{ &Nothing, &assembleNothing }
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
		auto assembledLine = assembleLine(Preprocess(line), symbols);
		if(assembledLine.length() > 0)
			output += assembledLine + "\n";
#ifdef DEBUG
		std::cout << "-> " << assembledLine << std::endl;
#endif
	}
	return output;
}

