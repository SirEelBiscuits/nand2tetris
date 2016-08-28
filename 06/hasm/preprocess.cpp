#include "preprocess.h"

static regex whitespace{"[[:space:]]"};
static regex comments  {"//.*$"};

string Preprocess(string line) {
	line = regex_replace(line, whitespace, "");
	line = regex_replace(line, comments, "");
	return line;
}
