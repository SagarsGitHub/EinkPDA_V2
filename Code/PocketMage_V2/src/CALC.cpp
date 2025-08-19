//   .oooooo.         .o.       ooooo          .oooooo.  //
//  d8P'  `Y8b       .888.      `888'         d8P'  `Y8b //
// 888              .8"888.      888         888         //
// 888             .8' `888.     888         888         //
// 888            .88ooo8888.    888         888         //
// `88b    ooo   .8'     `888.   888       o `88b    ooo //
//  `Y8bood8P'  o88o     o8888o o888ooooood8  `Y8bood8P' // 
#include "globals.h"



// !! Commented for code-review
// to-do 
// depreciate ~C~ and ~R~ for flags included in LineView 
// - flags bitmask 0000 0000
//  LF_NONE=0000 0000
//  LF_RIGHT= 0000 0001
//  LF_CENTER= 0000 00010
//  other flags to be determined 
// make convertToRPN && evaluateRPN able to handle standard notation inputs from other apps 
//  - (in tokenize, convert operators into standard notation)
// programming mode
// implement matrix multiplication
// implement for loops and conditionals
// strategy design pattern implementation?
#pragma region setupData
#pragma region helpText

const char hl0[]  PROGMEM = "\n";
const char hl1[]  PROGMEM = " vvv scroll down vvv\n";
const char hl2[]  PROGMEM = " This is the help screen\n";
const char hl3[]  PROGMEM = "\n";
const char hl4[]  PROGMEM = " press enter to exit\n";
const char hl5[]  PROGMEM = "\n";
const char hl6[]  PROGMEM = " NOTES:\n";
const char hl7[]  PROGMEM = "~C~/6 -> EXIT APP\n";
const char hl8[]  PROGMEM = "\n";
const char hl9[]  PROGMEM = "~C~scroll <- ->\n";
const char hl10[] PROGMEM = "\n";
const char hl11[] PROGMEM = "~C~',,' equals '.'\n";
const char hl12[] PROGMEM = "\n";
const char hl13[] PROGMEM = " vvv scroll down vvv\n";
const char hl14[] PROGMEM = "\n";
const char hl15[] PROGMEM = "\n";
const char hl16[] PROGMEM = " commands:\n";
const char hl17[] PROGMEM = "\n";
const char hl18[] PROGMEM = "~C~'/' + <command> \n";
const char hl19[] PROGMEM = " 0 : standard\n";
const char hl20[] PROGMEM = "  \n";
const char hl21[] PROGMEM = " 1 : programming\n";
const char hl22[] PROGMEM = "~C~(not implemented) \n";
const char hl23[] PROGMEM = " 2 : scientific \n";
const char hl24[] PROGMEM = "\n";
const char hl25[] PROGMEM = " 3 : conversions \n";
const char hl26[] PROGMEM = " <- = convA select \n";
const char hl27[] PROGMEM = " -> = convB select \n";
const char hl28[] PROGMEM = " select = unit select \n";
const char hl29[] PROGMEM = " tab = dir select \n";
const char hl30[] PROGMEM = " ' ' = main screen \n";
const char hl31[] PROGMEM = " \n";
const char hl32[] PROGMEM = " 4 : help\n";
const char hl33[] PROGMEM = "  \n";
const char hl34[] PROGMEM = " 5 : export to txt\n";
const char hl35[] PROGMEM = "  \n";
const char hl36[] PROGMEM = " 6 : EXIT\n";
const char hl37[] PROGMEM = "  \n";
const char hl38[] PROGMEM = " rad : radian mode\n";
const char hl39[] PROGMEM = "  \n";
const char hl40[] PROGMEM = " deg : degree mode\n";
const char hl41[] PROGMEM = "  \n";
const char hl42[] PROGMEM = " gon : gradian mode\n";
const char hl43[] PROGMEM = "  \n";
const char hl44[] PROGMEM = " keyboard changes:\n";
const char hl45[] PROGMEM = "~C~default kb state:FUNC\n";
const char hl46[] PROGMEM = "~C~tab && fn(tab) == font\n";
const char hl47[] PROGMEM = "~C~bksp == fn(bskp)\n";
const char hl48[] PROGMEM = "~C~left arrow scroll  ^ \n";
const char hl49[] PROGMEM = "~C~right arrow scroll v \n";
const char hl50[] PROGMEM = "operators:\n";
const char hl51[] PROGMEM = "  \n";
const char hl52[] PROGMEM = " - (unary included)\n";
const char hl53[] PROGMEM = " +\n";
const char hl54[] PROGMEM = " * (type: ' or a(b))\n";
const char hl55[] PROGMEM = " /\n";
const char hl56[] PROGMEM = " %\n";
const char hl57[] PROGMEM = " !\n";
const char hl58[] PROGMEM = " !! repeat prev line\n";
const char hl59[] PROGMEM = " ^ (type: \")\n";
const char hl60[] PROGMEM = " = (type: :)\n";
const char hl61[] PROGMEM = " E (aEb)\n";
const char hl62[] PROGMEM = "\n";
const char hl63[] PROGMEM = "functions: \n";
const char hl64[] PROGMEM = "\n";
const char hl65[] PROGMEM = " sin(a) asin(a) sinh(a)\n";
const char hl66[] PROGMEM = " csc(a) acsc(a) csch(a)\n";
const char hl67[] PROGMEM = " cos(a) acos(a) cosh(a)\n";
const char hl68[] PROGMEM = " sec(a) asec(a) sech(a)\n";
const char hl69[] PROGMEM = " tan(a) atan(a) tanh(a)\n";
const char hl70[] PROGMEM = " cot(a) acot(a) coth(a)\n";
const char hl71[] PROGMEM = " sqrt(a)\n";
const char hl72[] PROGMEM = " exp(a)     log(a)\n";
const char hl73[] PROGMEM = " pow(a,b)   log10(a\n";
const char hl74[] PROGMEM = " floor(a)   ceil(a)\n";
const char hl75[] PROGMEM = " min(a)     max(a)\n";
const char hl76[] PROGMEM = " round(a)\n";
const char hl77[] PROGMEM = " abs(a)\n";
const char hl78[] PROGMEM = " rand(a,b) from min a->b\n";
const char hl79[] PROGMEM = " dice(a,b) a: num b:sides\n";
const char hl80[] PROGMEM = " pick(a,n) a: choices\n";
const char hl81[] PROGMEM = "~C~n: no. choices\n";
const char hl82[] PROGMEM = "\n";
const char hl83[] PROGMEM = "variables: \n";
const char hl84[] PROGMEM = "\n";
const char hl85[] PROGMEM = "must start with alpha\n";
const char hl86[] PROGMEM = "excluding constants \n";
const char hl87[] PROGMEM = "~C~^^^ scroll up ^^^";

// Table of pointers (in PROGMEM) and count
const char* const HELP_LINES[] PROGMEM = {
  hl0, hl1, hl2, hl3, hl4, hl5, hl6, hl7, hl8, hl9,
  hl10, hl11, hl12, hl13, hl14, hl15, hl16, hl17, hl18, hl19,
  hl20, hl21, hl22, hl23, hl24, hl25, hl26, hl27, hl28, hl29,
  hl30, hl31, hl32, hl33, hl34, hl35, hl36, hl37, hl38, hl39,
  hl40, hl41, hl42, hl43, hl44, hl45, hl46, hl47, hl48, hl49,
  hl50, hl51, hl52, hl53, hl54, hl55, hl56, hl57, hl58, hl59,
  hl60, hl61, hl62, hl63, hl64, hl65, hl66, hl67, hl68, hl69,
  hl70, hl71, hl72, hl73, hl74, hl75, hl76, hl77, hl78, hl79,
  hl80, hl81, hl82, hl83, hl84, hl85, hl86, hl87
};

const size_t HELP_COUNT = sizeof(HELP_LINES) / sizeof(HELP_LINES[0]);

#pragma endregion // End of helpText
#pragma region unitTypes

const char ut0[]  PROGMEM = "~C~length\n";
const char ut1[]  PROGMEM = "~C~area\n";
const char ut2[]  PROGMEM = "~C~volume\n";
const char ut3[]  PROGMEM = "~C~weight&mass\n";
const char ut4[]  PROGMEM = "~C~temperature\n";
const char ut5[]  PROGMEM = "~C~energy\n";
const char ut6[]  PROGMEM = "~C~speed\n";
const char ut7[]  PROGMEM = "~C~pressure\n";
const char ut8[]  PROGMEM = "~C~data\n";
const char ut9[]  PROGMEM = "~C~angle";
const char ut10[]  PROGMEM = "~C~time";
const char ut11[]  PROGMEM = "~C~power";
const char ut12[]  PROGMEM = "~C~force";
const char ut13[]  PROGMEM = "~C~frequency";

const char* const UNIT_TYPES_LINES[] PROGMEM = {
  ut0, ut1, ut2, ut3, ut4, ut5, ut6, ut7, ut8, ut9, ut10, ut11, ut12, ut13
};

const size_t UNIT_TYPES_COUNT = sizeof(UNIT_TYPES_LINES) / sizeof(UNIT_TYPES_LINES[0]);

#pragma endregion // End of unitTypes
#pragma region convDir


const char cdir0[]  PROGMEM = "~C~->\n";
const char cdir1[]  PROGMEM = "~C~<-";

const char* const CONV_DIR_LINES[] PROGMEM = {
  cdir0, cdir1
};

const size_t CONV_DIR_COUNT = sizeof(CONV_DIR_LINES) / sizeof(CONV_DIR_LINES[0]);

#pragma endregion // End of convDir
#pragma region convLength

const char cl0[]  PROGMEM = "~C~m\n";
const char cl1[]  PROGMEM = "~C~in\n";
const char cl2[]  PROGMEM = "~C~yd\n";
const char cl3[]  PROGMEM = "~C~ft\n";
const char cl4[]  PROGMEM = "~C~mi\n";
const char cl5[]  PROGMEM = "~C~km\n";
const char cl6[]  PROGMEM = "~C~cm\n";
const char cl7[]  PROGMEM = "~C~mm\n";
const char cl8[]  PROGMEM = "~C~nm\n";
const char cl9[]  PROGMEM = "~C~um\n";
const char cl10[]  PROGMEM = "~C~M";

const char* const CONV_LENGTH_LINES[] PROGMEM = {
  cl0, cl1, cl2, cl3, cl4, cl5, cl6, cl7, cl8, cl9, cl10
};

const size_t CONV_LENGTH_COUNT = sizeof(CONV_LENGTH_LINES) / sizeof(CONV_LENGTH_LINES[0]);

#pragma endregion // End of convLength
#pragma region convArea

const char ca0[]  PROGMEM = "~C~ha\n";
const char ca1[]  PROGMEM = "~C~ac\n";
const char ca2[]  PROGMEM = "~C~m^2\n";
const char ca3[]  PROGMEM = "~C~in^2\n";
const char ca4[]  PROGMEM = "~C~yd^2\n";
const char ca5[]  PROGMEM = "~C~ft^2\n";
const char ca6[]  PROGMEM = "~C~mi^2\n";
const char ca7[]  PROGMEM = "~C~km^2\n";
const char ca8[]  PROGMEM = "~C~cm^2\n";
const char ca9[]  PROGMEM = "~C~mm^2\n";

const char* const CONV_AREA_LINES[] PROGMEM = {
  ca0, ca1, ca2, ca3, ca4, ca5, ca6, ca7, ca8, ca9
};

const size_t CONV_AREA_COUNT = sizeof(CONV_AREA_LINES) / sizeof(CONV_AREA_LINES[0]);

#pragma endregion // End of convLength
#pragma region convVolume

// PROGMEM strings for menu display
const char cv0[]  PROGMEM = "~C~m^3\n";
const char cv1[]  PROGMEM = "~C~cm^3\n";
const char cv2[]  PROGMEM = "~C~mm^3\n";
const char cv3[]  PROGMEM = "~C~km^3\n";
const char cv4[]  PROGMEM = "~C~L\n";
const char cv5[]  PROGMEM = "~C~mL\n";
const char cv6[]  PROGMEM = "~C~in^3\n";
const char cv7[]  PROGMEM = "~C~ft^3\n";
const char cv8[]  PROGMEM = "~C~yd^3\n";
const char cv9[]  PROGMEM = "~C~tsp\n";
const char cv10[]  PROGMEM = "~C~tbsp\n";
const char cv11[]  PROGMEM = "~C~gal\n";
const char cv12[] PROGMEM = "~C~qt\n";
const char cv13[] PROGMEM = "~C~pt\n";
const char cv14[] PROGMEM = "~C~cup\n";
const char cv15[] PROGMEM = "~C~fl oz\n";

const char* const CONV_VOLUME_LINES[] PROGMEM = {
  cv0, cv1, cv2, cv3, cv4, cv5, cv6,
  cv7, cv8, cv9, cv10, cv11, cv12, cv13,
  cv14, cv15
};

const size_t CONV_VOLUME_COUNT = sizeof(CONV_VOLUME_LINES) / sizeof(CONV_VOLUME_LINES[0]);

#pragma endregion // End of convVolume
#pragma region convMass

const char cm0[]  PROGMEM = "~C~kg\n";
const char cm1[]  PROGMEM = "~C~g\n";
const char cm2[]  PROGMEM = "~C~mg\n";
const char cm3[]  PROGMEM = "~C~µg\n";
const char cm4[]  PROGMEM = "~C~t\n";
const char cm5[]  PROGMEM = "~C~lb\n";
const char cm6[]  PROGMEM = "~C~oz\n";
const char cm7[]  PROGMEM = "~C~st\n";
const char cm8[]  PROGMEM = "~C~sh t\n";
const char cm9[]  PROGMEM = "~C~l t\n";

const char* const CONV_MASS_LINES[] PROGMEM = {
  cm0, cm1, cm2, cm3, cm4, cm5, cm6, cm7, cm8, cm9
};

const size_t CONV_MASS_COUNT = sizeof(CONV_MASS_LINES) / sizeof(CONV_MASS_LINES[0]);

#pragma endregion
#pragma region convTemperature

const char ct0[] PROGMEM = "~C~K\n";
const char ct1[] PROGMEM = "~C~C\n";
const char ct2[] PROGMEM = "~C~F\n";
const char ct3[] PROGMEM = "~C~R\n";

const char* const CONV_TEMPERATURE_LINES[] PROGMEM = {
  ct0, ct1, ct2, ct3
};

const size_t CONV_TEMPERATURE_COUNT =
    sizeof(CONV_TEMPERATURE_LINES) / sizeof(CONV_TEMPERATURE_LINES[0]);

#pragma endregion
#pragma region convEnergy

const char ce0[] PROGMEM = "~C~J\n";
const char ce1[] PROGMEM = "~C~kJ\n";
const char ce2[] PROGMEM = "~C~cal\n";
const char ce3[] PROGMEM = "~C~kcal\n";
const char ce4[] PROGMEM = "~C~Wh\n";
const char ce5[] PROGMEM = "~C~kWh\n";
const char ce6[] PROGMEM = "~C~eV\n";
const char ce7[] PROGMEM = "~C~BTU\n";
const char ce8[] PROGMEM = "~C~therm\n";

const char* const CONV_ENERGY_LINES[] PROGMEM = {
  ce0, ce1, ce2, ce3, ce4, ce5, ce6, ce7, ce8
};

const size_t CONV_ENERGY_COUNT =
    sizeof(CONV_ENERGY_LINES) / sizeof(CONV_ENERGY_LINES[0]);

#pragma endregion
#pragma region convSpeed
const char cs0[] PROGMEM = "~C~m/s\n";
const char cs1[] PROGMEM = "~C~km/h\n";
const char cs2[] PROGMEM = "~C~mph\n";
const char cs3[] PROGMEM = "~C~kn\n";
const char cs4[] PROGMEM = "~C~ft/s\n";
const char cs5[] PROGMEM = "~C~cm/s\n";

const char* const CONV_SPEED_LINES[] PROGMEM = { cs0, cs1, cs2, cs3, cs4, cs5 };
const size_t CONV_SPEED_COUNT = sizeof(CONV_SPEED_LINES)/sizeof(CONV_SPEED_LINES[0]);
#pragma endregion
#pragma region convPressure
const char cp0[] PROGMEM = "~C~atm\n";
const char cp1[] PROGMEM = "~C~bar\n";
const char cp2[] PROGMEM = "~C~mbar\n";
const char cp3[] PROGMEM = "~C~Torr\n";
const char cp4[] PROGMEM = "~C~mTorr\n";
const char cp5[] PROGMEM = "~C~Pa\n";
const char cp6[] PROGMEM = "~C~kPa\n";
const char cp7[] PROGMEM = "~C~MPa\n";
const char cp8[] PROGMEM = "~C~psi\n";

const char* const CONV_PRESSURE_LINES[] PROGMEM = {
  cp0, cp1, cp2, cp3, cp4, cp5, cp6, cp7, cp8
};

const size_t CONV_PRESSURE_COUNT = sizeof(CONV_PRESSURE_LINES) / sizeof(CONV_PRESSURE_LINES[0]);
#pragma endregion
#pragma region convData
const char cd0[]  PROGMEM = "~C~bit\n";
const char cd1[]  PROGMEM = "~C~B\n";
const char cd2[]  PROGMEM = "~C~kB\n";
const char cd3[]  PROGMEM = "~C~MB\n";
const char cd4[]  PROGMEM = "~C~GB\n";
const char cd5[]  PROGMEM = "~C~TB\n";
const char cd6[]  PROGMEM = "~C~KiB\n";
const char cd7[]  PROGMEM = "~C~MiB\n";
const char cd8[]  PROGMEM = "~C~GiB\n";
const char cd9[]  PROGMEM = "~C~TiB\n";

const char* const CONV_DATA_LINES[] PROGMEM = {
  cd0, cd1, cd2, cd3, cd4, cd5, cd6, cd7, cd8, cd9
};
const size_t CONV_DATA_COUNT = sizeof(CONV_DATA_LINES)/sizeof(CONV_DATA_LINES[0]);
#pragma endregion
#pragma region convAngle
const char caa0[] PROGMEM = "~C~rad\n";
const char caa1[] PROGMEM = "~C~deg\n";
const char caa2[] PROGMEM = "~C~gon\n";
const char caa3[] PROGMEM = "~C~arcmin\n";
const char caa4[] PROGMEM = "~C~arcsec\n";
const char caa5[] PROGMEM = "~C~rev\n";
const char caa6[] PROGMEM = "~C~mrad\n";

const char* const CONV_ANGLE_LINES[] PROGMEM = { caa0, caa1, caa2, caa3, caa4, caa5, caa6 };
const size_t CONV_ANGLE_COUNT = sizeof(CONV_ANGLE_LINES)/sizeof(CONV_ANGLE_LINES[0]);
#pragma endregion
#pragma region convTime

const char cti0[] PROGMEM = "~C~s\n";
const char cti1[] PROGMEM = "~C~ms\n";
const char cti2[] PROGMEM = "~C~µs\n";
const char cti3[] PROGMEM = "~C~ns\n";
const char cti4[] PROGMEM = "~C~min\n";
const char cti5[] PROGMEM = "~C~h\n";
const char cti6[] PROGMEM = "~C~d\n";
const char cti7[] PROGMEM = "~C~wk\n";
const char cti8[] PROGMEM = "~C~yr\n";

const char* const CONV_TIME_LINES[] PROGMEM = {
  cti0, cti1, cti2, cti3, cti4, cti5, cti6, cti7, cti8
};

const size_t CONV_TIME_COUNT = sizeof(CONV_TIME_LINES) / sizeof(CONV_TIME_LINES[0]);

#pragma endregion
#pragma region convPower

const char cpo0[] PROGMEM = "~C~W\n";
const char cpo1[] PROGMEM = "~C~kW\n";
const char cpo2[] PROGMEM = "~C~MW\n";
const char cpo3[] PROGMEM = "~C~hp\n";
const char cpo4[] PROGMEM = "~C~erg/s\n";

const char* const CONV_POWER_LINES[] PROGMEM = {
  cpo0, cpo1, cpo2, cpo3, cpo4
};

const size_t CONV_POWER_COUNT = sizeof(CONV_POWER_LINES) / sizeof(CONV_POWER_LINES[0]);

#pragma endregion
#pragma region convForce

const char cf0[] PROGMEM = "~C~N\n";
const char cf1[] PROGMEM = "~C~dyn\n";
const char cf2[] PROGMEM = "~C~kgf\n";
const char cf3[] PROGMEM = "~C~lbf\n";
const char cf4[] PROGMEM = "~C~ozf\n";

const char* const CONV_FORCE_LINES[] PROGMEM = {
  cf0, cf1, cf2, cf3, cf4
};

const size_t CONV_FORCE_COUNT = sizeof(CONV_FORCE_LINES) / sizeof(CONV_FORCE_LINES[0]);

#pragma endregion
#pragma region convFrequency

const char cfq0[] PROGMEM = "~C~Hz\n";
const char cfq1[] PROGMEM = "~C~kHz\n";
const char cfq2[] PROGMEM = "~C~MHz\n";
const char cfq3[] PROGMEM = "~C~GHz\n";
const char cfq4[] PROGMEM = "~C~rpm\n";

const char* const CONV_FREQUENCY_LINES[] PROGMEM = {
  cfq0, cfq1, cfq2, cfq3, cfq4
};

const size_t CONV_FREQUENCY_COUNT = sizeof(CONV_FREQUENCY_LINES) / sizeof(CONV_FREQUENCY_LINES[0]);

#pragma endregion
#pragma endregion

///////////////////////////// HELPER FUNCTIONS
// CONFRIM TOKEN IS A NUMBER
inline bool isNumberToken(const String& token) {
    if (token.isEmpty()) return false;
    size_t start = 0;
    // check value after unary negative operators
    if (token[0] == '-') {
        start = 1;
        if (token.length() == 1) return false;
    }
    bool hasDecimal = false;
    // check if float is valid (no trailing '.' + no alpha)
    for (size_t i = start; i < token.length(); i++) {
        if (token[i] == '.') {
            if (hasDecimal) return false;
            hasDecimal = true;
            // check for back '.'
            if (i == token.length() - 1) {
                return false; // "." at end
            }
        }
        else if (token[i] != '.' && !isDigit(token[i])) {
            return false;
        }
    }
    return true;
}
// CONFIRM TOKEN IS A FUNCTION
inline bool isFunctionToken(const String& token) {
  for (size_t i=0;i<functionsCalcCount;++i) {
    if (token == functionsCalc[i]) return true;
  }
  return false;
}
// CONFIRM A TOKEN IS A VARIABLE INCLUDING CONSTANTS
inline bool isVariableToken(const String& token) {
    if (token.isEmpty()) return false;
    // exclude constants and functions
    if (isConstantToken(token)) return false;
    if (isFunctionToken(token)) return false;

    // check if alphanumeric starting with alpha 
    if (!isAlpha(token[0])) return false;
    for (size_t i = 1; i < token.length(); i++) {
        if (!isAlphaNumeric(token[i])) return false;
    }
    return true;
}
// COMPARE TO SET OF CONSTANTS
inline bool isConstantToken(const String& token) {
  for (size_t i=0;i<constantsCalcCount;++i) {
    if (token == constantsCalc[i]) return true;
  }
  return false;
}
// COMPARE TO SET OF OPERATORS
inline bool isOperatorToken(const String& token) {
  for (size_t i=0;i<OPS_N;++i) if (token.equals(OPS[i].token)) return true;
  return false;
}
// CHECK OPERATOR PRECEDENCE
inline uint8_t precedenceOfToken(const String& token) {
  for (size_t i=0;i<OPS_N;++i) if (token.equals(OPS[i].token)) return OPS[i].prec;
  return 0;
}
// CHECK OPERATOR RIGHT ASSOCIATIVITY
inline bool isRightAssociative(const String& token) {
  for (size_t i=0;i<OPS_N;++i) if (token.equals(OPS[i].token)) return OPS[i].rightAssoc;
  return false;
}
// CONVERT TRIG INPUTS
inline double convertTrig(double input,int trigType,bool reverse){
  switch (trigType){
    // 0 = degree mode
    case (0):
      return reverse? ((input*180)/PI) : ((input*PI)/180);
    break;
    // 1 = radian mode
    case (1):
      return input;
    break;
    // 2 = grad mode
    case (2):
      return reverse? ((input*200)/PI) : ((input*PI)/200);
    break;
    default:
      return input;
    break;
  }
}
// CONVERT UNITA TO UNITB
inline double convert(double value,const Unit* from,const Unit* to) {
    // convert to 
    double inBase = (value + from->offset) * from->factor;
    return (inBase / to->factor) - to->offset;
}

// ADD LINE TO CALCLINES
inline void calcAppend(const String& s, bool right, bool center) {
  uint8_t f = right ? LF_RIGHT : center ? LF_CENTER : LF_NONE;
  calcLines.pushLine(s.c_str(), (uint16_t)s.length(), f);
}
// GET UNIT FROM CURRENT UNIT TYPES
const Unit* getUnit(const String& sym) {
  const UnitSet* s = CurrentUnitSet;
  for (size_t i = 0; i < s->size; ++i) {
    // compare case-insensitively to handle "m^2" vs "M^2"
    Serial.println("Searching set index " + String(i));
    Serial.println("syn =  " + sym + "  s->data[i].symbol = " + String(s->data[i].symbol));
    if (sym.equalsIgnoreCase(s->data[i].symbol)) {
      Serial.println("Found match! " + String(i));
      return &s->data[i];
    }
  }
  return nullptr;
}
// UPDATE UNIT FROM FRAME CHOICE
void selectUnitType(int idx) {
  //Serial.println("Applying selected Unit at idk " + String(idx));
  // clamp to known catalog size
  if (idx < 0) idx = 0;
  if ((size_t)idx >= (UnitCatalogCount)) {
    idx = UnitCatalogCount - 1;
  }
  CurrentUnitSet = &UnitCatalog[idx];
  // pair the visual list with the same index; guard if not defined yet
  if ((size_t)idx < (AllUnitListsCount) && allUnitLists[idx]) {
    CurrentUnitListSrc = allUnitLists[idx];
  }
  // point the visible frames at the new list, reset scroll/choice
  conversionFrameA.source = CurrentUnitListSrc;
  conversionFrameB.source = CurrentUnitListSrc;
  conversionFrameA.scroll = conversionFrameB.scroll = 0;
  conversionFrameA.prevScroll = conversionFrameB.prevScroll = -1;
  conversionFrameA.choice = conversionFrameB.choice = 0;
}

///////////////////////////// MAIN FUNCTIONS
// CALC INITIALIZETION
void CALC_INIT() {
  // open calc
  CurrentAppState = CALC;
  CurrentKBState = FUNC;
  CurrentFrameState = &calcScreen;
  frames.clear();
  frames.push_back(&calcScreen);

  // not clean, relying on magic number 11 since the frame hasn't been drawn with EinkFrameDynamic
  helpScreen.scroll = helpSrc.size() - 11;
  conversionFrameA.extendedBottom = FRAME_BOTTOM + 8;
  conversionFrameA.overlap = 1;
  conversionFrameB.extendedBottom = FRAME_BOTTOM + 8;
  conversionFrameB.overlap = 1;
  conversionUnit.extendedBottom = FRAME_BOTTOM + 40;
  conversionUnit.overlap = 1;
  conversionDirection.choice = 0;
  conversionFrameA.choice = 0;
  conversionFrameB.choice = 0;
  conversionUnit.choice = 0;
  currentFrameChoice = conversionUnit.choice;

  dynamicScroll = 0;
  prev_dynamicScroll = -1;
  lastTouch = -1;
  newState = true;
  doFull = true;
  disableTimeout = false;
  setTXTFont(&FreeMonoBold9pt7b);
  currentLine = "";
}
// KB HANDLER
void processKB_CALC() {
  if (OLEDPowerSave) {
    u8g2.setPowerSave(0);
    OLEDPowerSave = false;
  }
  disableTimeout = false;
  int currentMillis = millis();
  if (currentMillis - KBBounceMillis >= KB_COOLDOWN) {  
    char inchar = updateKeypress();

    setTXTFont(currentFont);
    // update scroll (calc specific function,could be abstracted to processSB_CALC())
    updateScrollFromTouch_Frame();
    if (frameSelection){
      if (CurrentFrameState == &conversionUnit) {
        selectUnitType(conversionUnit.choice);
      } else {
        String sym = frameChoiceString(*CurrentFrameState);
        CurrentFrameState->unit = (Unit*)getUnit(sym);
      }
      frameSelection = 0;
    }
    switch (CurrentCALCState) {

      // CONVERSIONS MODE 
      case CALC3:

        // HANDLE INPUTS
        // no char recieved
        if (inchar == 0);  
        else if (inchar == 12) {
          if (currentLine.length() > 0) {
            currentLine.remove(currentLine.length() - 1);
          }
        }
        else if (inchar == '.') {
          currentLine += ".";
          CurrentKBState = FUNC;
        }
        else if (inchar == ','){
          if (currentLine.endsWith(",")){
            //Serial.println("Performed character replacement for ',,' ");
            currentLine[currentLine.length()-1] = '.';
          } else {
            currentLine += ",";
          }
        }
        // TAB Recieved (switch unit types aka length to area)
        else if (inchar == 9 || inchar == 14) {    
          conversionDirection.choice == 1 ? conversionDirection.choice = 0 :conversionDirection.choice= 1;
          newLineAdded = true;
          //Serial.println("adjusting convDir to : " + String(conversionDirection.choice));
        }                                      
        // SHIFT Recieved
        else if (inchar == 17) {                                  
          if (CurrentKBState == SHIFT) CurrentKBState = NORMAL;
          else CurrentKBState = SHIFT;
        }
        // FN Recieved
        else if (inchar == 18) {                                  
          if (CurrentKBState == FUNC) CurrentKBState = NORMAL;
          else CurrentKBState = FUNC;
        }
        // Space Recieved
        else if (inchar == 32) {                                  
          if (CurrentFrameState != &conversionScreen){
            CurrentFrameState->bottom = CurrentFrameState->originalBottom;
            CurrentFrameState = &conversionScreen;
            dynamicScroll = CurrentFrameState->scroll;
            prev_dynamicScroll = CurrentFrameState->prevScroll;
            newLineAdded = true;
          } 
        }
        // CR Recieved
        else if (inchar == 13) {    
          //Serial.println(" current direction is " + String(conversionDirection.choice));
          calcCRInput(); // existing behavior                      
        }
        // ESC / CLEAR Recieved
        else if (inchar == 20) {   
          if (CurrentFrameState == &conversionScreen){                               
            calcClear();
            currentLine = "";
            oledWord("Clearing...");
            drawCalc();
            //Serial.println("In CALC3 Mode calling frames");
            einkTextFramesDynamic(frames,false,false);
            display.refresh();
            delay(300);
          }
        }
        // LEFT (scroll up)
        else if (inchar == 19 || inchar == 5) {
          if (CurrentFrameState != &conversionFrameA){
            CurrentFrameState->bottom = CurrentFrameState->originalBottom;
            CurrentFrameState = &conversionFrameA;
            dynamicScroll = CurrentFrameState->scroll;
            prev_dynamicScroll = CurrentFrameState->prevScroll;

            CurrentFrameState->bottom = CurrentFrameState->extendedBottom;

            newLineAdded = true;
          }           
        }
        // RIGHT (switch to conversion a tab or scroll down in tab)
        else if (inchar == 21 || inchar == 6) { 
          if (CurrentFrameState != &conversionFrameB){
            CurrentFrameState->bottom = CurrentFrameState->originalBottom;
            CurrentFrameState = &conversionFrameB;
            dynamicScroll = CurrentFrameState->scroll;
            prev_dynamicScroll = CurrentFrameState->prevScroll;

            CurrentFrameState->bottom = CurrentFrameState->extendedBottom;
            newLineAdded = true;
          } 
        }
        //BKSP Recieved
        else if (inchar == 8) {    
          if (currentLine.length() > 0) {
            currentLine.remove(currentLine.length() - 1);
          }                       
        }
        //SAVE Recieved (no plan to save calculations)
        else if (inchar == 6) {}
        //LOAD Recieved (no plan to load calculations)
        else if (inchar == 5) {}
        //FILE Recieved (switch to unit types tab)
        else if (inchar == 7) {
          if (CurrentFrameState != &conversionUnit){
            CurrentFrameState->bottom = CurrentFrameState->originalBottom;
            CurrentFrameState = &conversionUnit;
            CurrentFrameState->bottom = CurrentFrameState->extendedBottom;
            dynamicScroll = CurrentFrameState->scroll;
            prev_dynamicScroll = CurrentFrameState->prevScroll;
            newLineAdded = true;
          } 
        }
        // Font Switcher (regular tab also starts the font switcher)
        else if (inchar == 14) {                                  
        }
        // add non-special characters to line
        else {
          currentLine += inchar;
          // No to return FN for ease of input
          // SHIFT will return to FUNC
          if (CurrentKBState == SHIFT) CurrentKBState = FUNC;
        }
        currentMillis = millis();
        //Make sure oled only updates at 60fps
        if (currentMillis - OLEDFPSMillis >= 16) {
          OLEDFPSMillis = currentMillis;
          // ONLY SHOW OLEDLINE WHEN NOT IN SCROLL MODE
          if (lastTouch == -1) {
            oledLine(currentLine);
            if (prev_dynamicScroll != dynamicScroll) prev_dynamicScroll = dynamicScroll;
            updateScroll(CurrentFrameState,prev_dynamicScroll,dynamicScroll);     
          }
          else oledScrollFrame();
        }
        break;
      // HELP MODE (no need inputs)
      case CALC4:
          // CR Recieved
          if (inchar == 13) {
            currentLine = "/0";    
            calcCRInput();   
            break;
          }   
      // PROGRAMMING MODE (not implemented)
      case CALC1:

      // SCIENTIFIC MODE 
      case CALC2:


      // standard mode
      case CALC0:
        // HANDLE INPUTS
        // no char recieved
        if (inchar == 0);  
        else if (inchar == 12) {
          if (currentLine.length() > 0) {
            currentLine.remove(currentLine.length() - 1);
          }
        }
        else if (inchar == '.') {
          currentLine += ".";
          CurrentKBState = FUNC;
        }
        else if (inchar == ','){
          if (currentLine.endsWith(",")){
            //Serial.println("Performed character replacement for ',,' ");
            currentLine[currentLine.length()-1] = '.';
          } else {
            currentLine += ",";
          }
        }
        // TAB Recieved (This starts the font switcher now)
        else if (inchar == 9) {    
          CurrentCALCState = CALCFONT;
          CurrentKBState = FUNC;
          newState = true;                              
        }                                      
        // SHIFT Recieved
        else if (inchar == 17) {                                  
          if (CurrentKBState == SHIFT) CurrentKBState = NORMAL;
          else CurrentKBState = SHIFT;
        }
        // FN Recieved
        else if (inchar == 18) {                                  
          if (CurrentKBState == FUNC) CurrentKBState = NORMAL;
          else CurrentKBState = FUNC;
        }
        // Space Recieved
        else if (inchar == 32) {                                  
          currentLine += " ";
        }
        // CR Recieved
        else if (inchar == 13) {    
          calcCRInput();                      
        }
        // ESC / CLEAR Recieved
        else if (inchar == 20) {                                  
          calcClear();
          currentLine = "";
          oledWord("Clearing...");
          drawCalc();
          einkTextFramesDynamic(frames,false,false);
          display.refresh();
          delay(300);
        }
        // LEFT (scroll up)
        else if (inchar == 19 || inchar == 5) {
          if (dynamicScroll < CurrentFrameState->source->size() - (9 + SCROLL_MAX)){
             dynamicScroll += SCROLL_MAX;
          } else if (dynamicScroll < CurrentFrameState->source->size() - (9 + SCROLL_MED)){
             dynamicScroll += SCROLL_MED;
          } else if (dynamicScroll < CurrentFrameState->source->size() - (9 + SCROLL_SML)){
             dynamicScroll += SCROLL_SML;
          } else if (dynamicScroll < CurrentFrameState->source->size() - 10) {
            dynamicScroll++;
          }
          newLineAdded = true; 
          updateScroll(CurrentFrameState,prev_dynamicScroll,dynamicScroll);                            
        }
        // RIGHT (scroll down)
        else if (inchar == 21 || inchar == 6) { 

          if (dynamicScroll > (SCROLL_MAX +1)){
            dynamicScroll -= SCROLL_MAX;
          }else if (dynamicScroll > (SCROLL_MED +1)){
            dynamicScroll -= SCROLL_MED;
          } else if (dynamicScroll > (SCROLL_SML +1)){
            dynamicScroll -= SCROLL_SML;
          } else if (dynamicScroll > 1){
            dynamicScroll--;
          } 
          updateScroll(CurrentFrameState,prev_dynamicScroll,dynamicScroll);     
          newLineAdded = true;
        }
        //BKSP Recieved
        else if (inchar == 8) {    
          if (currentLine.length() > 0) {
            currentLine.remove(currentLine.length() - 1);
          }                       
        }
        //SAVE Recieved (no plan to save calculations)
        else if (inchar == 6) {}
        //LOAD Recieved (no plan to load calculations)
        else if (inchar == 5) {}
        //FILE Recieved 
        else if (inchar == 7) {
          calcClear();
          currentLine = "";
          oledWord("Clearing...");
          drawCalc();
          einkTextFramesDynamic(frames,false,false);
          delay(300);
        }
        // Font Switcher (regular tab also starts the font switcher)
        else if (inchar == 14) {                                  
          CurrentCALCState = CALCFONT;
          CurrentKBState = FUNC;
          newState = true;         
        }
        // add non-special characters to line
        else {
          currentLine += inchar;
          // No to return FN for ease of input
          // SHIFT will return to FUNC
          if (CurrentKBState == SHIFT) CurrentKBState = FUNC;
        }
        currentMillis = millis();
        //Make sure oled only updates at 60fps
        if (currentMillis - OLEDFPSMillis >= 16) {
          OLEDFPSMillis = currentMillis;
          // ONLY SHOW OLEDLINE WHEN NOT IN SCROLL MODE
          if (lastTouch == -1) {
            oledLine(currentLine);
            if (prev_dynamicScroll != dynamicScroll) prev_dynamicScroll = dynamicScroll;
            updateScroll(CurrentFrameState,prev_dynamicScroll,dynamicScroll);     
          }
          else oledScrollFrame();
        }
        break;

      // FONT SWITCHER
      case CALCFONT:
        //No char recieved
        if (inchar == 0);
        //BKSP Recieved
        else if (inchar == 12 || inchar == 8) {                  
          CurrentCALCState = CALC0;
          CurrentKBState = NORMAL;
          newLineAdded = false;
          editingFile = "";
          currentLine = "";
          drawCalc();
          einkTextFramesDynamic(frames,true,false);
        }
        else if (inchar >= '0' && inchar <= '9') {
          int fontIndex = (inchar == '0') ? 10 : (inchar - '0');
          switch (fontIndex) {
            case 1:
              currentFont = &FreeMonoBold9pt7b;
              break;
            case 2:
              currentFont = &FreeSans9pt7b;
              break;
            case 3:
              currentFont = &FreeSerif9pt7b;
              break;
            case 4:
              currentFont = &FreeSerifBold9pt7b;
              break;
            case 5:
              currentFont = &FreeMono12pt7b;
              break;
            case 6:
              currentFont = &FreeSans12pt7b;
              break;
            case 7:
              currentFont = &FreeSerif12pt7b;
              break;
            default:
              currentFont = &FreeMonoBold9pt7b;
              break;
          }
          // SET THE FONT
          setTXTFont(currentFont);

          // UPDATE THE ARRAY TO MATCH NEW FONT SIZE
          String fullTextStr = vectorToString();
          stringToVector(fullTextStr);

          CurrentCALCState = CALC0;
          CurrentKBState = NORMAL;
          newLineAdded = true;
          currentWord = "";
          currentLine = "";
      
          calcClear();

          // REFRESH SCREEN

          refresh();
          drawCalc();
          einkTextFramesDynamic(frames,true,false);

        }
        currentMillis = millis();
        //Make sure oled only updates at 60fps
        if (currentMillis - OLEDFPSMillis >= 16) {
          OLEDFPSMillis = currentMillis;
          oledLine(currentWord, false);
        }

        break;
    }
    KBBounceMillis = currentMillis;
  }
}
// Eink handler
// NOTE: duplicated code for some calc modes in handler
void einkHandler_CALC() {
  if (newLineAdded || newState) {
    // reset eink screen if returning from a new mode
    if (calcSwitchedStates == 1){
      calcSwitchedStates = 0;
      doFull = 1;
      frames.clear();
      if (CurrentCALCState == CALC2)  {   
        // to-do: convert to helper function to reset all frame tabs
        conversionFrameA.bottom = conversionFrameA.originalBottom;
        conversionFrameB.bottom = conversionFrameB.originalBottom;
        conversionUnit.bottom = conversionUnit.originalBottom;
        frames.push_back(&calcScreen);
        CurrentFrameState = &calcScreen;
      } else {
        doFull = 1;
        frames.clear();
        if (CurrentCALCState == CALC1){
          frames.push_back(&programmingScreen);
          frames.push_back(&numberSizeFrame);
          frames.push_back(&hexadecimalFrame);
          frames.push_back(&decimalFrame);
          decimalFrame.invert = 1;
          frames.push_back(&octalFrame);
          frames.push_back(&binaryFrame);
          CurrentFrameState = &programmingScreen; // placeholder
          CurrentFrameState->scroll = 0;
          CurrentFrameState->prevScroll = -1;
        } else if (CurrentCALCState == CALC3){
          frames.push_back(&conversionScreen);
          frames.push_back(&conversionFrameA);
          frames.push_back(&conversionFrameB);
          frames.push_back(&conversionDirection);
          frames.push_back(&conversionUnit);
          CurrentFrameState = &conversionScreen;
          CurrentFrameState->scroll = 0;
          CurrentFrameState->prevScroll = -1;
        }else {  // CALC0 and CALC4
          frames.clear();
 
          currentFont = &FreeMonoBold9pt7b;
          setTXTFont(currentFont);
          frames.push_back(&helpScreen);
          CurrentFrameState = &helpScreen;
          dynamicScroll = helpScreen.scroll;
          updateScroll(CurrentFrameState, prev_dynamicScroll, dynamicScroll);
          prev_dynamicScroll = -1;
        }
      }
      drawCalc();
      newLineAdded = true;
      einkTextFramesDynamic(frames,true,false);
    } else {
      switch (CurrentCALCState) {
        case CALC0:
          //standard mode
          if (newState && doFull) { 
            drawCalc();
            einkTextFramesDynamic(frames,false,false);
            //refresh();
            doFull = false;
          } else if (newLineAdded && !newState) {
            refresh_count++;
            if (refresh_count > REFRESH_MAX_CALC){
              drawCalc(); 
              setFastFullRefresh(false);
              einkTextFramesDynamic(frames,true,false);
              refresh_count = 0;
            } else {
              einkTextFramesDynamic(frames,true,false);
            }
            setFastFullRefresh(true);
          } else if (newState && !newLineAdded) {
            drawCalc();
          }
          break;
        case CALC1:
          //programming mode
          if (newState && doFull) { 
            drawCalc();
            einkTextFramesDynamic(frames,true,false);
            //refresh();
            doFull = false;
          } else if (newLineAdded && !newState) {
            refresh_count++;
            if (refresh_count > REFRESH_MAX_CALC){
              drawCalc(); 
              setFastFullRefresh(false);
              einkTextFramesDynamic(frames,true,false);
              refresh_count = 0;
            } else {
              einkTextFramesDynamic(frames,true,false);
            }
            setFastFullRefresh(true);
          } else if (newState && !newLineAdded) {
            drawCalc();
          }
          break;
        case CALC2:
          //scientific mode
          if (newState && doFull) { 
            drawCalc();
            einkTextFramesDynamic(frames,true,false);
            //refresh();
            doFull = false;
          } else if (newLineAdded && !newState) {
            refresh_count++;
            if (refresh_count > REFRESH_MAX_CALC){
              drawCalc(); 
              setFastFullRefresh(false);
              einkTextFramesDynamic(frames,true,false);
              refresh_count = 0;
            } else {
              einkTextFramesDynamic(frames,true,false);
            }
            setFastFullRefresh(true);
          } else if (newState && !newLineAdded) {
            drawCalc();
          }
          break;
        case CALC3:
          //conversions 
          if (newState && doFull) { 
            drawCalc();
            //Serial.println("In CALC3 Mode calling frames");
            einkTextFramesDynamic(frames,true,false);
            //refresh();
            doFull = false;
          } else if (newLineAdded && !newState) {
            refresh_count++;
            if (refresh_count > REFRESH_MAX_CALC){
              drawCalc(); 
              setFastFullRefresh(false);
              //Serial.println("In CALC3 Mode calling frames");
              einkTextFramesDynamic(frames,true,false);
              refresh_count = 0;
            } else {
              //Serial.println("In CALC3 Mode calling frames");
              einkTextFramesDynamic(frames,true,false);
            }
            setFastFullRefresh(true);
          } else if (newState && !newLineAdded) {
            drawCalc();
          }
          break;
        case CALC4:
          // help mode
          if (newState && doFull) { 
            drawCalc();
            einkTextFramesDynamic(frames,true,false);
            //refresh();
            doFull = false;
          } else if (newLineAdded && !newState) {
            refresh_count++;
            if (refresh_count > REFRESH_MAX_CALC){
              drawCalc(); 
              setFastFullRefresh(false);
              einkTextFramesDynamic(frames,true,false);
              refresh_count = 0;
            } else {
              einkTextFramesDynamic(frames,true,false);
            }
            setFastFullRefresh(true);
          } else if (newState && !newLineAdded) {
            drawCalc();
          }
          break;
        case CALCFONT:
          display.firstPage();
          do {
            // false avoids full refresh
            einkTextDynamic(true, false);      
            display.setPartialWindow(60, 0, 200, 218);
            drawStatusBar("Select a Font (0-9)");
            display.fillRect(60, 0, 200, 218, GxEPD_WHITE);
            display.drawBitmap(60, 0, fontfont0, 200, 218, GxEPD_BLACK);
            for (int i = 0; i < 7; i++) {
              display.setCursor(88, 54 + (17 * i));
              switch (i) {
                case 0: display.setFont(&FreeMonoBold9pt7b); break;
                case 1: display.setFont(&FreeSans9pt7b); break;
                case 2: display.setFont(&FreeSerif9pt7b); break;
                case 3: display.setFont(&FreeSerifBold9pt7b); break;
                case 4: display.setFont(&FreeMono12pt7b); break;
                case 5: display.setFont(&FreeSans12pt7b); break;
                case 6: display.setFont(&FreeSerif12pt7b); break;
              }
              display.print("Font Number " + String(i + 1));
            }
          } while (display.nextPage());
          CurrentKBState = FUNC;
          newState = false;
          newLineAdded = false;
          break;
        }
    }
    newState = false;
    newLineAdded = false;
  }
}

///////////////////////////// CALC EINK FUNCTIONS
// CLOSE CALC AND UPDATE
void closeCalc(AppState newAppState){
  frames.clear();
  frames.push_back(&calcScreen);
  if (CurrentCALCState == CALC3){
    CurrentCALCState == CALC0;
  }
  // essential to display next app correctly 
  display.setFullWindow();

  display.fillScreen(GxEPD_WHITE);
  u8g2.clearBuffer();
  dynamicScroll = 0;
  prev_dynamicScroll = -1;
  updateScroll(CurrentFrameState,prev_dynamicScroll,dynamicScroll);     
  if (newAppState == TXT) {
    TXT_INIT();
  }  else {
    CurrentAppState = HOME;
    currentLine     = "";
    newState        = true;
    CurrentKBState  = NORMAL; 
    disableTimeout = false;
    refresh(); 
  }
}

///////////////////////////// ALGORITHMS
// FORMAT INTO RPN,EVALUATE,SAVE
int calculate(const String& cleanedInput,String &result,const Unit *convA,const Unit *convB){
  // convert to reverse polish notation for easy calculation
  std::deque<String> inputRPN = convertToRPN(cleanedInput);
  // calculate the RPN expression
  result = evaluateRPN(inputRPN,convA,convB);
  return 0;
}
// STRING INPUT TO RPN
std::deque<String> convertToRPN(String expression) {
    std::deque<String> outputQueue;
    std::stack<String> operatorStack;
    std::vector<String> tokens = tokenize(expression);
    //Serial.println("Converting to RPN: " + expression);
    // Parenthesis validation
    /*
    for (auto it = tokens.begin(); it != tokens.end(); it++) {
      Serial.println("RPN Token: " + *it);
    }
    */
    int paren_balance = 0;
    for (char c : expression) {
        if (c == '(') paren_balance++;
        else if (c == ')') paren_balance--;
    }
    if (paren_balance != 0) return {}; 
    
    // Shunting Yard Algorithm
    for (size_t i = 0; i < tokens.size(); ++i) {
        const String& token = tokens[i];
        if (isNumberToken(token) || isConstantToken(token)) {
            outputQueue.push_back(token);
        } 
        else if (isFunctionToken(token)) {
            operatorStack.push(token);
        }
        // add argument 1 to output queue for functions with multiple arguments
        else if (token == ",") {
          while (!operatorStack.empty() && operatorStack.top() != "(") {
              outputQueue.push_back(operatorStack.top());
              operatorStack.pop();
          }
        } 
        else if (isAlpha(token[0]) && token != "E") {
          if (i + 1 < tokens.size() && tokens[i+1] == ":") {
              // For assignment, push variable name marker to output
              //Serial.println("pushed ~var~ + variable!");
              outputQueue.push_back("~var~" + token);
          } else {
              //Serial.println("pushed variable!");
              outputQueue.push_back(token);
          }
        }
        else if (token == "(") {
            operatorStack.push(token);
        } 
        // add all items in operator stack to output until first "(", remove "(", and push function to output
        else if (token == ")") {
          while (!operatorStack.empty() && operatorStack.top() != "(") {
              outputQueue.push_back(operatorStack.top());
              operatorStack.pop();
          }
          if (!operatorStack.empty()) operatorStack.pop(); // pop "("
          if (!operatorStack.empty() && isFunctionToken(operatorStack.top())) {
              outputQueue.push_back(operatorStack.top());
              operatorStack.pop();
          }
        } 
        /*
        if ((token == "~neg~")) {
          if ((i+1) < tokens.size()){
            //Serial.println("pushed unary negation! RPN");
            operatorStack.push(String(-1*variables[tokens[i+1]]));
          }

          i++;
          continue;
        }
        */
        // sort operators by precedence, push all operators of greater precedence to the output queue and place new operator in operator stack
        else if (isOperatorToken(token)) {
          // assignment guard stays the same...
          if (token == ":") {
            if (outputQueue.empty()) return {};
            const String& prev = outputQueue.back();
            // after the refactor, the variable is emitted as "~var~name"
            if (!prev.startsWith("~var~")) return {};
          }

          const bool rightA = isRightAssociative(token);
          const uint8_t p1 = precedenceOfToken(token);

          while (!operatorStack.empty()
                && operatorStack.top() != "(")
          {
            const String& o2 = operatorStack.top();
            if (!isOperatorToken(o2)) break;

            const uint8_t p2 = precedenceOfToken(o2);
            const bool popIt =
              (!rightA && (p1 <= p2)) ||   // left-assoc: pop while prec <=
              ( rightA && (p1 <  p2));     // right-assoc: pop while prec <
            if (!popIt) break;

            outputQueue.push_back(o2);
            operatorStack.pop();
          }

          operatorStack.push(token);
        }
    }
    // empty rest of operator stack
    while (!operatorStack.empty()) {
        outputQueue.push_back(operatorStack.top());
        operatorStack.pop();
    }

    return outputQueue;
}
// SPLIT STRING INTO TOKENS
// can make tokenize add proper operators instead of replacement operators so that convertToRPN is able to handle standard notation inputs from other apps
// i.e. " : " -> " = ", " ' " -> " * ", " " " -> "^"
std::vector<String> tokenize(const String& expression) {
    std::vector<String> tokens;
    String currentToken = "";
    bool usedRepeatFunction = false;
    //Serial.println("Tokenizing expression: " + expression);
    for (int i = 0; i < expression.length(); ++i) {
      char c = expression[i];
      //Serial.println("handling character: " + String(c));   
      // Handle assignment '='
      if (c == ':') {
          // Single '=' for assignment
          tokens.push_back(":");  // Keep as : for assignment
          continue;
      }
      // handle - and unary negation
      bool prevIsOperatorOrStart =
          (i == 0) || 
          isOperatorToken(String(expression[i - 1])) || 
          expression[i - 1] == '(' || 
          expression[i - 1] == ',' || 
          expression[i - 1] == ':';

      if (c == '-' && prevIsOperatorOrStart) {
          if (i + 1 < expression.length() && isDigit(expression[i + 1])) {
            //Serial.println("pushed back negative number");
            currentToken += c;
            while (i + 1 < expression.length() &&
                  (isDigit(expression[i + 1]) || expression[i + 1] == '.')) {
                currentToken += expression[++i];
            }
            tokens.push_back(currentToken);
            currentToken = "";
            continue;
          } else {
            //Serial.println("pushed back unary minus operator");
            tokens.push_back("~neg~");
            continue;
          }
      }
      // handle !! macro
      if (!usedRepeatFunction && c == '!' && i + 1 < expression.length() && expression[i + 1] == '!') {
          if (prevTokens.empty()) {
            oledWord("E: no previous expression");
            delay(1000);
            return {};
          }

          usedRepeatFunction = true;
          // Add in previous expression tokens
          for (auto it = prevTokens.begin(); it != prevTokens.end(); ++it) {
              //Serial.println("pushing token: " + *it + " from previous expression");
              tokens.push_back(*it);
          }

          i++;
          continue;
      }
            
      // Handle numbers
      if (isDigit(c) || (c == '.' && i + 1 < expression.length() && isDigit(expression[i + 1]))) {
          //Serial.println("handling" + currentToken);
          currentToken += c;
          while (i + 1 < expression.length()) {
              char peek = expression[i + 1];
              if (isDigit(peek) || peek == '.') {
                  currentToken += expression[++i];
              } else {
                  break;
              }
          }

          //Serial.println("Pushing number " + String(currentToken));
          tokens.push_back(currentToken);
          currentToken = "";
          continue;
      }

      // Handle alphabetic tokens
      if (isAlpha(c) && c != 'E') {
          currentToken += c;
          while (i + 1 < expression.length()) {
              if (isAlphaNumeric(expression[i + 1]) && expression[i + 1] != 'E') {
                  currentToken += expression[++i];
              } else {
                  break;  // Stop at anything like '('
              }
          }

          tokens.push_back(currentToken);
          currentToken = "";
          continue;
      }

      // Handle parentheses with implied multiplication
      if (c == '(') {
          if (!tokens.empty()) {
              const String& prev = tokens.back();
              bool insertMultiply = false;
              // If the previous token is a number, constant, or closing parentheses
              if (isNumberToken(prev) || prev == ")" || prev == "pi" || prev == "e" || prev == "ans") {
                  insertMultiply = true;
              }
              // If it's an alphanumeric and not a known function, assume variable * (
              if (isAlpha(prev[0]) && !isFunctionToken(prev) && prev != "E") {
                  insertMultiply = true;
              }
              if (insertMultiply) {
                  tokens.push_back("'");
              }
          }

          tokens.push_back("(");
          continue;
      }
      // Handle closing parens
      if (c == ')') {
          tokens.push_back(")");
          continue;
      }
    // Handle operators and commas
    if (isOperatorToken(String(c)) || c == ',') {
        tokens.push_back(String(c));
        continue;
    }

      // Unknown token,error
      oledWord("E: malformed expression");
      delay(1000);
      return {}; 
    }
    prevTokens = tokens;
    return tokens;
}
// EVALUATE RPN
String evaluateRPN(std::deque<String> rpnQueue, const Unit *convA, const Unit *convB) {
    std::stack<double> evalStack;
    std::stack<String> varStack;
    // print queue
    //Serial.println("Handling evaluating RPN");
    //for (auto it = rpnQueue.begin(); it != rpnQueue.end(); it++) {
    //  Serial.println("eval Token: " + *it);
    //}
    
    while (!rpnQueue.empty()) {
        String token = rpnQueue.front();
        rpnQueue.pop_front();

        //can declare these if else blocks as inline functions 
        // Handle assignment variable marker first
        if (token.startsWith("~var~")) {
          String varName = token.substring(5);
          varStack.push(varName);
          continue;
        }
        else if (isNumberToken(token)) {
            evalStack.push(token.toDouble());
        }
        // Handle previous answer
        else if (token == "ans") {
            evalStack.push(variables["ans"]);
        }
        // Handle constants
        else if (token == "pi") {
            evalStack.push(PI);
        }
        else if (token == "e") {
            evalStack.push(EULER);
        // Handle unary negation
        } else if (token == "~neg~") {
          if (evalStack.empty()) return "E: ~neg~";
          double a = evalStack.top(); evalStack.pop();
          evalStack.push(-a);
        } else if (isAlpha(token[0]) && !isFunctionToken(token) && token != "E") {
          if (variables.find(token) != variables.end()) {
              evalStack.push(variables[token]);
          } else {
              oledWord("undefined variable");
              delay(1000);
              return "E: undefined variable";
          }
        }

        // Handle binary operators
        else if (token == "+") {
            if (evalStack.size() < 2) return "E: +";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(a + b);
        }
        else if (token == "-") {
            //Serial.println("subtracting!");
            if (evalStack.size() < 2) return "E: -";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(a - b);
        }
        else if (token == "'") {
            if (evalStack.size() < 2) return "E: *";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(a * b);
        }
        else if (token == "/") {
            if (evalStack.size() < 2) return "E: /";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            if (b == 0) return "Div by 0";
            evalStack.push(a / b);
        }
        else if (token == "\"") {
            if (evalStack.size() < 2) return "E: ^";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(pow(a, b));
        }
        else if (token == "%") {
            if (evalStack.size() < 2) return "E: %";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            // Handle modulo with floating-point numbers using fmod
            if (b == 0) return "E: Div by 0 %";
            evalStack.push(fmod(a, b));
        }
        else if (token == "E") {
            if (evalStack.size() < 2) return "E: E";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            if (abs(b) > 300) return "E: large exponent";
            String temp = String(a, 8) + "e" + String(b,8);
            
            if (round(b) == 0){
              evalStack.push(1.0);
            } else {
              evalStack.push(a * pow(10, b));
            }

        }
        else if (token == ":") {
          // Needs exactly 1 value and 1 variable
          if (evalStack.empty() || varStack.empty()) {
              return "E: assignment needs 1 value and 1 variable";
          }
          String varName = varStack.top(); varStack.pop();
          double value = evalStack.top(); evalStack.pop();
          variables[varName] = value;
          evalStack.push(value);
        }


        // Handle unary operators
        else if (token == "!") {
            if (evalStack.empty()) return "E: !";
            double a = evalStack.top(); evalStack.pop();
            if (a < 0) return "E: ! input < 0 ";
            evalStack.push(tgamma(a + 1));
        }
        
        // Handle trig functions
        // Trig functions too numerous and messy, need to do something about repeat code 
        else if (token == "sin") {
            if (evalStack.empty()) return "E: sin";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            evalStack.push(sin(a));
        }
        else if (token == "asin") {
            if (evalStack.empty()) return "E: asin";
            double a = evalStack.top(); evalStack.pop();
            if (a < -1 || a > 1) return "E: domain of asin";
            a = convertTrig(asin(a),trigType,true);
            evalStack.push(a);
        }
        else if (token == "sinh") {
            if (evalStack.empty()) return "E: sinh";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            evalStack.push(sinh(a));
        }
        else if (token == "csc") {
            if (evalStack.empty()) return "E: csc";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            if (sin(a) == 0) return "E: Div by 0 csc";
            evalStack.push(1/sin(a));
        }
        else if (token == "acsc") {
            if (evalStack.empty()) return "E: acsc";
            double a = evalStack.top(); evalStack.pop();
            if (a==0) return "E: Div by 0 acsc";
            double inv = 1/a;
            if (inv < -1 || inv > 1) return "E: domain of acsc";
            a = convertTrig(asin(inv),trigType,true);
            evalStack.push(a);
        }
        else if (token == "csch") {
            if (evalStack.empty()) return "E: csch";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            if (sinh(a) == 0) return "E: Div by 0 csch";
            evalStack.push(1/sinh(a));
        }
        else if (token == "cos") {
            if (evalStack.empty()) return "E: cos";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            evalStack.push(cos(a));
        }
        else if (token == "acos") {
            if (evalStack.empty()) return "E: acos";
            double a = evalStack.top(); evalStack.pop();
            if (a < -1 || a > 1) return "E: domain of acos";
            if (a == 0) a = PI/2;
            a = convertTrig(acos(a),trigType,true);
            evalStack.push(a);
        }
        else if (token == "cosh") {
            if (evalStack.empty()) return "E: cosh";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            evalStack.push(cosh(a));
        }
        else if (token == "sec") {
            if (evalStack.empty()) return "E: sec";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            if (cos(a) == 0) return "E: Div by 0 sec";
            evalStack.push(1/cos(a));
        }
        // could add logic to handle a = +inf == PI/2
        else if (token == "asec") {
            if (evalStack.empty()) return "E: asec";
            double a = evalStack.top(); evalStack.pop();
            if (a==0) return "E: Div by 0 acsc";
            double inv = 1/a;
            if (inv < -1 || inv > 1) return "E: domain of acsc";
            a = convertTrig(acos(inv),trigType,true);
            evalStack.push(a);
        }
        else if (token == "sech") {
            if (evalStack.empty()) return "E: sech";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            if (cosh(a) == 0) return "E: Div by 0 sech";
            evalStack.push(1/cosh(a));
        }
        else if (token == "tan") {
            if (evalStack.empty()) return "E: tan";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            evalStack.push(tan(a));
        }
        else if (token == "atan") {
            if (evalStack.empty()) return "E: atan";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(atan(a),trigType,true);
            evalStack.push(a);
        }
        else if (token == "tanh") {
            if (evalStack.empty()) return "E: tanh";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            evalStack.push(tanh(a));
        }
        else if (token == "cot") {
            if (evalStack.empty()) return "E: cot";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            if (tan(a) == 0) return "E: Div by 0 cot";
            evalStack.push(1/tan(a));
        }
        else if (token == "acot") {
            if (evalStack.empty()) return "E: acot";
            double a = evalStack.top(); evalStack.pop();
            double result;
            if (a == 0) {
              // acot(0) = π/2
              result = PI / 2; 
            } else {
              // returns value in (−π/2, π/2)
              result = atan(1/a);
              // shift to principal range (0, π) 
              if (a<0) result += PI; 
            }
            evalStack.push(convertTrig(a,trigType,true));
        }
        else if (token == "coth") {
            if (evalStack.empty()) return "E: coth";
            double a = evalStack.top(); evalStack.pop();
            a = convertTrig(a,trigType);
            if (tanh(a) == 0) return "E: Div by 0 coth";
            evalStack.push(1/tanh(a));
        }

        // handle single input functions
        else if (token == "sqrt") {
            if (evalStack.empty()) return "E: sqrt";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(sqrt(a));
        }
        else if (token == "cbrt") {
            if (evalStack.empty()) return "E: cbrt";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(cbrt(a));
        }
        else if (token == "exp") {
            if (evalStack.empty()) return "E: exp";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(exp(a));
        }
        else if (token == "round") {
            if (evalStack.empty()) return "E: round";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(round(a));
        }
        else if (token == "ln") {
            if (evalStack.empty()) return "E: ln";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(log(a));
        }
        else if (token == "floor") {
            if (evalStack.empty()) return "E: floor";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(floor(a));
        }
        else if (token == "ceil") {
            if (evalStack.empty()) return "E: ceil";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(ceil(a));
        }
        else if (token == "abs") {
            if (evalStack.empty()) return "E: abs";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(fabsf(a));
        }

        // handle multiple input functions
        // log(a,b) = log(a)/log(b) base b log of a
        else if (token == "log") {
            if (evalStack.size() < 2) return "E: log";
            double a = evalStack.top(); evalStack.pop();
            // base of log, order changed for a more natural input
            double b = evalStack.top(); evalStack.pop();
            evalStack.push(log(a)/log(b));
        }
        else if (token == "max") {
            if (evalStack.size() < 2) return "E: max";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(max(a, b));
        }
        else if (token == "min") {
            if (evalStack.size() < 2) return "E: min";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(min(a, b));
        }
        else if (token == "pow") {
            if (evalStack.size() < 2) return "E: pow";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(pow(a, b));
        }
        else if (token == "rand") {
            if (evalStack.size() < 2) return "E: rand";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(random(a, b));
        }
        else if (token == "dice") {
            if (evalStack.size() < 2) return "E: dice";
            int b = int(evalStack.top()); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            if (b < 2) return "E: dice sides";
            int roll = 0;

            for (int i = 0; i < a; i++){           
              roll+= (esp_random() % b) + 1;
              //Serial.println("current roll: " + String(roll));
            }
            evalStack.push(roll);
        }
        else if (token == "pick"){
            if (evalStack.size() < 1) return "E: pick no n arg";
            int a = static_cast<int>(evalStack.top()); evalStack.pop();
            int choices = a;
            if (evalStack.size() < choices) return "E: pick invalid choices";
            int pickedValue = random(1, choices+1);
            double valueToPush = 0;

            for (int i = 0; i < choices; i++){
              double poppedValue = evalStack.top(); evalStack.pop();
              if (i == (choices - pickedValue)) {
                valueToPush = poppedValue;
              }
            }
            evalStack.push(valueToPush);
        }
        else {
            return "E: Unknown token: " + token;
        }
    }

    if (evalStack.size() != 1) {
      return "E: malformed exp";
    }
    if (varStack.size() != 0){
      String varName = varStack.top(); varStack.pop();
      double value = evalStack.top();
      variables[varName] = value;
    }
    //Serial.println("exited evaluation RPN function");
    variables["ans"] = evalStack.top();

    if ((convA != nullptr) && strcmp(convA->name, convB->name) != 0){
      //Serial.println("converting input!");
      cleanExpression = cleanExpression + convA->symbol;
      return formatNumber(convert(evalStack.top(), convA, convB));
    }
    return formatNumber(evalStack.top());
}

///////////////////////////// INPUT/OUTPUT FUNCTIONS
// ENTER (CR) INPUT
void calcCRInput(){
  // trim spaces
  currentLine.replace(" ", ""); 
  // parse commands
  if (currentLine != ""){
    if (currentLine == "/0"){
        // standard mode
        CurrentCALCState = CALC0;
        calcSwitchedStates = 1;
    }  
    else if (currentLine == "/1"){
        // programming mode
        CurrentCALCState = CALC1;
        calcSwitchedStates = 1;
        delay(1000);
    }
    else if (currentLine == "/2"){
        // scientific mode
        CurrentCALCState = CALC2;
        calcSwitchedStates = 1;
    }
    else if (currentLine == "/3"){
        // conversion
        CurrentCALCState = CALC3;
        calcSwitchedStates = 1;
        newLineAdded = true;
    }
    else if (currentLine == "/4"){
        // help mode
        CurrentCALCState = CALC4;
        //Serial.println("helpScreen scroll = " + String(helpScreen.scroll));
        calcSwitchedStates = true;

    }
    else if (currentLine == "/5") {
        // write current file to text
        oledWord("Exporting Data to TXT!");

        // CHANGED: copy from source into a vector
        allLines = sourceToVector(CurrentFrameState->source);

        // remove '~R~' or '~C~' formatting (not used by txt app)
        for (int i = 0; i < allLines.size(); i++) {
          if (!(i % 2 == 0)) {
            String temp = allLines[i - 1] + " = " + allLines[i].substring(3);
            allLines[i - 1] = temp;
            allLines[i] = "";
          }
        }
        delay(200);
        closeCalc(TXT);
    }
    else if (currentLine == "/6"){
      closeCalc(HOME);
    }
    else if (currentLine == "/gon"){
      trigType = 2;
      drawCalc();
    }
    else if (currentLine == "/rad"){
      trigType = 1;
      drawCalc();
    }
    else if (currentLine == "/deg"){
      trigType = 0;
      drawCalc();      
    }
    else {
      // no command, calculate answer
      dynamicScroll = 0;
      updateScroll(CurrentFrameState,prev_dynamicScroll,dynamicScroll);
      // handle only !!
      currentLine == "!!" ? prevLine : currentLine;
      //Serial.println("PrevLine updated to : " + currentLine);
      if (CurrentCALCState == CALC3){
        conversionFrameA.bottom = conversionFrameA.originalBottom;
        conversionFrameB.bottom = conversionFrameA.originalBottom;
        CurrentFrameState = &conversionScreen;
        String conversionA = frameChoiceString(conversionFrameA);
        String conversionB = frameChoiceString(conversionFrameB);

        const Unit* convA = getUnit(conversionA);
        const Unit* convB = getUnit(conversionB);

        conversionDirection.choice == 0 ? printAnswer(currentLine,convA,convB):printAnswer(currentLine,convB,convA);
        

        
      } else {
        printAnswer(currentLine, &emptyUnit, &emptyUnit);
      }
      prevLine = currentLine;
    }
  }
  calculatedResult = "";
  currentLine = "";
  newLineAdded = true;
}
// CONVERT NUMBER TO FLOAT STRING OR INT STRING !!
String formatNumber(double value) {
    //Serial.println("formating number " + String(value));
    // NaN/Inf guards
    if (isnan(value)) return "nan";
    if (!isfinite(value)) return value < 0 ? "-inf" : "inf";
    String result;
    char buffer[32];
    //Serial.println("formatting number!");
    if (CurrentCALCState == CALC2){
      return formatScientific(value);
    } 
    else {
      if ((value > INT_MAX || ((fabs(value - round(value)) <= 1e-9)&&(fabs(value - round(value)) > 1e-300)))) {
        //Serial.println("sending scientifc instead for conversion!");
        return formatScientific(value);
      }
      // handle integer test case
      if (fabs(value - round(value)) < 1e-9) {
        snprintf(buffer, sizeof(buffer), "%lld", static_cast<long long>(round(value)));
        return String(buffer);
      }
      result = trimValue(value);
    }
    //Serial.println("Returning standard number!");
    return result;
}
// CONVERT DOUBLE TO SCIENTIFIC NOTATION !!
String formatScientific(double value) {
    // handle overflow and underflow edge cases
    if (String(value) == "inf" || String(value) == "-inf") {
      //Serial.println("overflow error");
      return String(value);
    }
    if (abs(value) < 1e-300) return "0e0";
    int exponent = 0;
    double mantissa = value;
    bool negative = mantissa < 0;
    String result;
    // normalize
    if (negative) mantissa = -mantissa;
    while (mantissa >= 10.0) {
        mantissa /= 10.0;
        exponent++;
    }
    while (mantissa < 1.0) {
        mantissa *= 10.0;
        exponent--;
    }
    if (negative) mantissa = -mantissa;
    result = trimValue(mantissa);
    result += "e" + String(exponent);
    return result;
}
// TRIM EXCESS ZEROS !!
String trimValue(double value){
    char buffer[32];
    // Format mantissa with 10 decimals initially
    snprintf(buffer, sizeof(buffer), "%.8f", value);
    String valueStr(buffer);
    int dotPos = valueStr.indexOf('.');
    if (dotPos != -1) {
        int lastNonZero = valueStr.length() - 1;
        while (lastNonZero > dotPos && valueStr[lastNonZero] == '0') {
            lastNonZero--;
        }
        if (lastNonZero == dotPos) {
            valueStr.remove(dotPos + 2);
        } else {
            valueStr.remove(lastNonZero + 1);
        }
    }
    return valueStr;
}
// CALC DISPLAY ANSWER !!
void printAnswer(String cleanExpression,const Unit *convA,const Unit *convB) {
  int16_t x1, y1;
  uint16_t exprWidth, resultWidth, charHeight;
  String resultOutput = "";
  int maxWidth = display.width();
  int result = calculate(cleanExpression, resultOutput,convA,convB);

  // Set font before measuring text
  display.setFont(currentFont);
  if (CurrentCALCState == CALC3 && convA && convB){
    // Non-breaking space keeps value and unit together during wrapping
    const char* SP = " ";
    cleanExpression += String(SP) + convA->symbol;
    resultOutput    += String(SP) + convB->symbol;
  }
  calcAppend(cleanExpression);
  calcAppend("~R~" + resultOutput);
  newLineAdded = true;
}

