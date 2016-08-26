/**
 * hasm
 *
 * An assembler for the hack platform.
 *
 * @author Jameson aka SirEel
 */

#include <string>
#include <vector>
#include <map>

using SymbolTable = std::map<std::string, int>;

std::vector<std::string> getWholeFile();
SymbolTable getSymbols();
int getWordsInLine(std::string line);
std::string assembleFile(SymbolTable symbols);

int main(int argc, char* argv[]) {
	auto wholeFile = getWholeFile;
	auto symbols   = getSymbols(wholeFile);
	auto output    = assembleFile(symbols);
}

std::vector<std::string> getWholeFile() {
	std::vector<std::string> lines{};
	return lines;
}

SymbolTable getSymbols() {
	SymbolTable table{};
	return table;
}

int getWordsInLine(std::string line) {
	return 0;
}

std::string assembleFile(SymbolTable symbols) {
	return "";
}
