/**
 * hasm
 *
 * An assembler for the hack platform.
 *
 * @author Jameson aka SirEel
 */

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <regex>

#include "types.h"
#include "parser.h"

InstructionList getWholeFile(string FileName);
SymbolTable     getSymbols  (InstructionList lines);
int             writeFile   (string fileContents, string outFileName);

int main(int argc, char* argv[]) {
	if(argc < 3)
		return EXIT_FAILURE;
	auto wholeFile = getWholeFile(argv[1]);
	auto output    = assembleFile(wholeFile);
	return writeFile(output, argv[2]);
}

InstructionList getWholeFile(string fileName) {
	using std::ifstream;

	ifstream file;
	file.open(fileName);
	string line;
	InstructionList lines{};
	while(getline(file, line)) {
		line = regex_replace(line, std::regex("\[[:space:]]"), "");
		lines.push_back(line);
	}

	return lines;
}

int writeFile(string fileContents, string outFileName) {
	using std::ofstream;
	ofstream file;
	file.open(outFileName);
	file << fileContents;
	return EXIT_SUCCESS;
}
