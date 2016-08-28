#pragma once

#include <string>
#include <map>
#include <vector>
#include <cinttypes>
#include <regex>

using Word = uint16_t;

using string    = std::string;
template<typename T, typename S>
using map    = std::map<T,S>;
template<typename T>
using vector = std::vector<T>;

using SymbolTable = map<string, Word>;
using InstructionList = vector<string>;

using regex = std::regex;
using match = std::smatch;
template<int n>
using bitset = std::bitset<n>;
