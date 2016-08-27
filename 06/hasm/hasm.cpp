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

#include "types.h"
#include "parser.h"

InstructionList getWholeFile();
SymbolTable     getSymbols  (InstructionList lines);
int             writeFile   (string fileContents);

int main(int argc, char* argv[]) {
	auto wholeFile = getWholeFile();
	auto output    = assembleFile(wholeFile);
	return writeFile(output);
}

InstructionList getWholeFile(string fileName) {
	using std::ifstream;

	ifstream file;
	file.open(fileName);
	string line;
	InstructionList lines{};
	while(getline(file, line)
			lines.push_back(line);

	return lines;
}

int writeFile(string fileContents) {
	return EXIT_SUCCESS;
}
