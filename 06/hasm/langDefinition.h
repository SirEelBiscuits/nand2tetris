#include "types.h"

extern regex Nothing ;
extern regex Label  ;
extern regex AInstr ;
extern regex CInstr ;

extern regex Number ;

extern regex CInstrNoJMP  ;
extern regex CInstrNoSET  ;
extern regex CInstrJMPSET ;
extern regex CInstrDST    ;

extern regex JMPLT ;
extern regex JMPEQ ;
extern regex JMPGT ;

extern regex CInstrZero    ;
extern regex CInstrOne     ;
extern regex CInstrNegOne  ;
extern regex CInstrD       ;
extern regex CInstrAM      ;
extern regex CInstrNotD    ;
extern regex CInstrNotAM   ;
extern regex CInstrNegD    ;
extern regex CInstrNegAM   ;
extern regex CInstrIncD    ;
extern regex CInstrIncAM   ;
extern regex CInstrDecD    ;
extern regex CInstrDecAM   ;
extern regex CInstrSum     ;
extern regex CInstrDLessAM ;
extern regex CInstrAMLessD ;
extern regex CInstrDAndAM  ;
extern regex CInstrDOrAM   ;
