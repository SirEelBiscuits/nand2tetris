#include "types.h"
regex Nothing { R"(^$)" };
regex Label  { R"(^\(([a-zA-Z._$][a-zA-Z._$0-9]*)\)$)" };
regex AInstr { R"(^@([a-zA-Z0-9._$]+)$)" };
regex CInstr { R"(^(.*=.*;.*|.*;.*|.*=.*)$)" };

regex Number { R"([1-9][0-9]*|0)" };

regex CInstrNoJMP  { R"(^(.*)=(.*)$)" };
regex CInstrNoSET  { R"(^(.*);(.*)$)" };
regex CInstrJMPSET { R"(^(.*)=(.*);(.*)$)" };
regex CInstrDST    { R"(^(A)?(M)?(D)?$)" };

regex JMPLT { R"(JLT|JNE|JLE|JMP)" };
regex JMPEQ { R"(JEQ|JGE|JLE|JMP)" };
regex JMPGT { R"(JGT|JGE|JNE|JMP)" };

regex CInstrZero    { R"(^0$)" };
regex CInstrOne     { R"(^1$)" };
regex CInstrNegOne  { R"(^-1$)" };
regex CInstrD       { R"(^D$)" };
regex CInstrAM      { R"(^[AM]$)" };
regex CInstrNotD    { R"(^!D$)" };
regex CInstrNotAM   { R"(^![AM]$)" };
regex CInstrNegD    { R"(^-D$)" };
regex CInstrNegAM   { R"(^-[AM]$)" };
regex CInstrIncD    { R"(^D[+]1$)" };
regex CInstrIncAM   { R"(^[AM][+]1$)" };
regex CInstrDecD    { R"(^D-1$)" };
regex CInstrDecAM   { R"(^[AM]-1$)" };
regex CInstrSum     { R"(^D[+][AM]$)" };
regex CInstrDLessAM { R"(^D-[AM]$)" };
regex CInstrAMLessD { R"(^[AM]-D$)" };
regex CInstrDAndAM  { R"(^D&[AM]$)" };
regex CInstrDOrAM   { R"(^D[|][AM]$)" };
