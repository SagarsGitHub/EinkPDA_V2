#include "globals.h"

//   .oooooo..o oooooooooooo ooooooooooooo ooooo     ooo ooooooooo.    //
//  d8P'    `Y8 `888'     `8 8'   888   `8 `888'     `8' `888   `Y88.  //
//  Y88bo.       888              888       888       8   888   .d88'  //
//   `"Y8888o.   888oooo8         888       888       8   888ooo88P'   //
//       `"Y88b  888    "         888       888       8   888          //
//  oo     .d8P  888       o      888       `88.    .8'   888          //
//  8""88888P'  o888ooooood8     o888o        `YbodP'    o888o         //

// Display setup
GxEPD2_BW<GxEPD2_310_GDEQ031T10, GxEPD2_310_GDEQ031T10::HEIGHT> display(GxEPD2_310_GDEQ031T10(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));
//volatile bool useFastFullUpdate = true;
volatile bool GxEPD2_310_GDEQ031T10::useFastFullUpdate = true;
//U8G2_SSD1326_ER_256X32_F_4W_HW_SPI u8g2(U8G2_R2, OLED_CS, OLED_DC, OLED_RST); //256x32
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE); //128x32

// Keypad setup
Adafruit_TCA8418 keypad;

// Buzzer
//Buzzer buzzer(17);

char keysArray[4][10] = {
  { 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p' },
  { 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 8 },  //8:BKSP
  { 9, 'z', 'x', 'c', 'v', 'b', 'n', 'm', '.', 13 },   //9:TAB, 13:CR
  { 0, 17, 18, ' ', ' ', ' ', 19, 20, 21, 0 }          //17:SHFT, 18:FN, 19:<-, 20:SEL, 21:-> 
};

char keysArraySHFT[4][10] = {
  { 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P' },
  { 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 8 },
  { 9, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', 13 },
  { 0, 17, 18, ' ', ' ', ' ', 19, 20, 21, 0 }
};

char keysArrayFN[4][10] = {
  { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' },
  { '#', '!', '$', ':', ';', '(', ')', '\'', '\"', 12 },
  { 14, '%', '-', '&', '+', '_', '/', '?', ',', 13 },
  { 0, 17, 18, ' ', ' ', ' ', 5, 7, 6, 0 }
};

// Touch slider setup
Adafruit_MPR121 cap = Adafruit_MPR121();


// RTC setup
RTC_PCF8563 rtc;
const char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

// USB
USBMSC msc;

// VARIABLES
// GENERAL
Preferences prefs;
int TIMEOUT;              // Time until automatic sleep (Seconds)
bool DEBUG_VERBOSE;       // Spit out some extra information
bool SYSTEM_CLOCK;        // Enable a small clock on the bottom of the screen.
bool SHOW_YEAR;           // Show the year on the clock
bool SAVE_POWER;          // Enable a slower CPU clock speed to save battery with little cost to performance
bool ALLOW_NO_MICROSD;    // Allow the device to operate with no SD card

volatile int einkRefresh = FULL_REFRESH_AFTER;
int OLEDFPSMillis = 0;
int KBBounceMillis = 0;
volatile int timeoutMillis = 0;
volatile int prevTimeMillis = 0;
volatile bool TCA8418_event = false;
volatile bool PWR_BTN_event = false;
volatile bool SHFT = false;
volatile bool FN = false;
volatile bool newState = false;
bool noTimeout = false;
volatile bool OLEDPowerSave = false;
volatile bool disableTimeout = false;
volatile int battState = 0;
volatile int prevBattState = 0;
unsigned int flashMillis = 0;
int prevTime = 0;
uint8_t prevSec = 0;
TaskHandle_t einkHandlerTaskHandle = NULL;
char currentKB[4][10];
KBState CurrentKBState = NORMAL;
uint8_t partialCounter = 0;
volatile bool forceSlowFullUpdate = true;
volatile bool SDCARD_INSERT = false;
bool noSD = false;

const String appStateNames[] = { "txt", "filewiz", "usb", "bt", "settings", "tasks", "calc" };
const unsigned char *appIcons[7] = { _homeIcons2, _homeIcons3, _homeIcons4, _homeIcons5, _homeIcons6, taskIconTasks0, _homeIcons10 };
AppState CurrentAppState = HOME;

// <TXT.ino>
String currentWord = "";
String allText = "";
String prevAllText = "";
String prevLastLine = "";
bool prevBKSP = false;
int scroll = 0;
int lines = 0;
String outLines[13];
String lines_prev[13];
String filesList[MAX_FILES];
uint8_t fileIndex = 0;
String editingFile = "";
String prevEditingFile = "";
String excludedFiles[3] = { "/temp.txt", "/settings.txt", "/tasks.txt" };
TXTState CurrentTXTState = TXT_;

String currentLine = "";
const GFXfont *currentFont = (GFXfont *)&FreeSerif9pt7b;
uint8_t maxCharsPerLine = 0;
uint8_t maxLines = 0;
uint8_t fontHeight = 0;
uint8_t lineSpacing = 6;  // LINE SPACING IN PIXELS
volatile bool newLineAdded = true;
volatile bool doFull = false;
std::vector<String> allLines;
volatile long int dynamicScroll = 0;
volatile long int prev_dynamicScroll = 0;
int lastTouch = -1;
unsigned long lastTouchTime = 0;



// <TASKS.ino>
std::vector<std::vector<String>> tasks;
uint8_t selectedTask = 0;
TasksState CurrentTasksState = TASKS0;
uint8_t newTaskState = 0;
uint8_t editTaskState = 0;
String newTaskName = "";
String newTaskDueDate = "";

// <HOME.ino>
HOMEState CurrentHOMEState = HOME_HOME;

// <FILEWIZ.ino> 
FileWizState CurrentFileWizState = WIZ0_;
String workingFile = "";


// <CALC.cpp>
#pragma region calcDataStructures
const char* operatorsCalc[] = { "+", "-", "'", "/", "E", "%", ":", "!", "\"" };
const size_t operatorsCalcCount = sizeof(operatorsCalc)/sizeof(operatorsCalc[0]);

const char* functionsCalc[] = {
    // trig
    "sin", "cos", "tan", "asin", "acos", "atan",
    "sinh", "cosh", "tanh", "sec", "csc", "cot", 
    "sech", "csch", "coth", "asec", "acsc", "acot",
    // scientific
    "ln", "log", "sqrt", "cbrt", "abs", "exp",
    "round", "min", "max", "pow", "rand",
    // fun
    "dice", "pick"
};
const size_t functionsCalcCount = sizeof(functionsCalc)/sizeof(functionsCalc[0]);

const char* constantsCalc[] = {
  "inf", "-inf", "pi", "e", "ans"
};
const size_t constantsCalcCount = sizeof(constantsCalc)/sizeof(constantsCalc[0]);

const OpEntry OPS[] = {
  {":",0,false}, {"+",1,false}, {"-",1,false},
  {"*",2,false}, {"/",2,false}, {"%",2,false},
  {"E",3,true }, // exponent right-associative
  {"\"",3,false},
  {"'",2,false}, // if you use single-quote as some operator
  {"!",4,true }, // postfix, treat as right-assoc for shunting-yard condition
  {"~neg~",4,true} // unary negation
};
const size_t OPS_N = sizeof(OPS)/sizeof(OPS[0]);

#pragma region units
static const Unit lengthUnits[] = {
    { "meter",         "m",    1.0,        0.0 }, //base
    { "kilometer",     "km",   1000.0,     0.0 },
    { "centimeter",    "cm",   0.01,       0.0 },
    { "millimeter",    "mm",   0.001,      0.0 },
    { "micrometer",    "um",   1e-6,       0.0 },
    { "nanometer",     "nm",   1e-9,       0.0 },
    { "inch",          "in",   0.0254,     0.0 },
    { "foot",          "ft",   0.3048,     0.0 },
    { "yard",          "yd",   0.9144,     0.0 },
    { "mile",          "mi",   1609.34,    0.0 },
    { "nautical mile", "M",    1852.0,     0.0 }
};

static const Unit areaUnits[] = {
    { "Hectare",          "ha",   10000.0,        0.0 },
    { "Acre",             "ac",   4046.8564224,   0.0 },
    { "Square meter",     "m^2",  1.0,            0.0 }, //base
    { "Square inch",      "in^2", 0.00064516,     0.0 },
    { "Square yard",      "yd^2", 0.83612736,     0.0 },
    { "Square foot",      "ft^2", 0.09290304,     0.0 },
    { "Square mile",      "mi^2", 2589988.110336, 0.0 },
    { "Square kilometer", "km^2", 1000000.0,      0.0 },
    { "Square centimeter","cm^2", 0.0001,         0.0 },
    { "Square millimeter","mm^2", 0.000001,       0.0 }
};

static const Unit volumeUnits[] = {
    { "Cubic meter",      "m^3",   1.0,           0.0 }, // base
    { "Cubic centimeter", "cm^3",  1e-6,          0.0 },
    { "Cubic millimeter", "mm^3",  1e-9,          0.0 },
    { "Cubic kilometer",  "km^3",  1e9,           0.0 },
    { "Liter",            "L",     0.001,         0.0 },
    { "Milliliter",       "mL",    1e-6,          0.0 },
    { "Cubic inch",       "in^3",  1.6387e-5,     0.0 },
    { "Cubic foot",       "ft^3",  0.0283168,     0.0 },
    { "Cubic yard",       "yd^3",  0.764555,      0.0 },
    { "Teaspoon",         "tsp",   4.92892159375e-6, 0.0 },
    { "Tablespoon",       "tbsp",  1.478676478125e-5, 0.0 },
    { "US gallon",        "gal",   0.00378541,    0.0 },
    { "US quart",         "qt",    0.000946353,   0.0 },
    { "US pint",          "pt",    0.000473176,   0.0 },
    { "US cup",           "cup",   0.000236588,   0.0 },
    { "US fluid ounce",   "fl oz", 2.9574e-5,     0.0 }
};

static const Unit massUnits[] = {
    { "Kilogram",     "kg",   1.0,         0.0 }, // base
    { "Gram",         "g",    0.001,       0.0 },
    { "Milligram",    "mg",   1e-6,        0.0 },
    { "Microgram",    "µg",   1e-9,        0.0 },
    { "Metric ton",   "t",    1000.0,      0.0 },
    { "Pound",        "lb",   0.45359237,  0.0 },
    { "Ounce",        "oz",   0.0283495,   0.0 },
    { "Stone",        "st",   6.35029,     0.0 },
    { "US ton",       "sh t", 907.18474,   0.0 },
    { "Imperial ton", "l t",  1016.0469088,0.0 }
};

static const Unit temperatureUnits[] = {
    { "kelvin",     "K",   1.0,       0.0 },     // base
    { "celsius",    "°C",  1.0,     273.15 },
    { "fahrenheit", "°F",  5.0/9.0, 459.67 }
};

static const Unit energyUnits[] = {
    { "Joule",         "J",      1.0,            0.0 }, // base
    { "Kilojoule",     "kJ",     1000.0,         0.0 },
    { "Calorie",       "cal",    4.184,          0.0 },
    { "Kilocalorie",   "kcal",   4184.0,         0.0 },
    { "Watt-hour",     "Wh",     3600.0,         0.0 },
    { "Kilowatt-hour", "kWh",    3600000.0,      0.0 },
    { "Electronvolt",  "eV",     1.602176634e-19,0.0 },
    { "BTU",           "BTU",    1055.06,        0.0 },
    { "Therm",         "therm",  1.055e8,        0.0 }
};

static const Unit speedUnits[] = {
    { "Meter per second",     "m/s",   1.0,              0.0 }, // base
    { "Kilometer per hour",   "km/h",  1000.0/3600.0,    0.0 },
    { "Mile per hour",        "mph",   1609.344/3600.0,  0.0 },
    { "Knot",                 "kn",    1852.0/3600.0,    0.0 },
    { "Foot per second",      "ft/s",  0.3048,           0.0 },
    { "Centimeter per second","cm/s",  0.01,             0.0 }
};

static const Unit pressureUnits[] = {
    { "Pascal",      "Pa",   1.0,            0.0 }, // base
    { "Kilopascal",  "kPa",  1000.0,         0.0 },
    { "Megapascal",  "MPa",  1e6,            0.0 },
    { "Bar",         "bar",  100000.0,       0.0 },
    { "Atmosphere",  "atm",  101325.0,       0.0 },
    { "Torr",        "Torr", 101325.0/760.0, 0.0 },
    { "Pound/in²",   "psi",  6894.757293168, 0.0 }
};

static const Unit dataUnits[] = {
    { "Bit",        "bit",  1.0/8.0,                 0.0 },
    { "Byte",       "B",    1.0,                     0.0 }, // base
    { "Kilobyte",   "kB",   1000.0,                  0.0 },
    { "Megabyte",   "MB",   1000.0*1000.0,           0.0 },
    { "Gigabyte",   "GB",   1e9,                     0.0 },
    { "Terabyte",   "TB",   1e12,                    0.0 },
    { "Kibibyte",   "KiB",  1024.0,                   0.0 },
    { "Mebibyte",   "MiB",  1024.0*1024.0,            0.0 },
    { "Gibibyte",   "GiB",  1024.0*1024.0*1024.0,     0.0 },
    { "Tebibyte",   "TiB",  1099511627776.0,          0.0 }
};

static const Unit angleUnits[] = {
    { "Radian",     "rad",   1.0,                            0.0 }, // base
    { "Degree",     "°",     3.14159265358979323846/180.0,   0.0 },
    { "Gradian",    "gon",   3.14159265358979323846/200.0,   0.0 },
    { "Arcminute",  "arcmin",3.14159265358979323846/10800.0, 0.0 },
    { "Arcsecond",  "arcsec",3.14159265358979323846/648000.0,0.0 },
    { "Revolution", "rev",   2.0*3.14159265358979323846,     0.0 },
    { "Milliradian","mrad",  0.001,                          0.0 }
};

static const Unit timeUnits[] = {
    { "Second",      "s",   1.0,      0.0 }, //base
    { "Millisecond", "ms",  0.001,    0.0 },
    { "Microsecond", "µs",  1e-6,     0.0 },
    { "Nanosecond",  "ns",  1e-9,     0.0 },
    { "Minute",      "min", 60.0,     0.0 },
    { "Hour",        "h",   3600.0,   0.0 },
    { "Day",         "d",   86400.0,  0.0 },
    { "Week",        "wk",  604800.0, 0.0 },
    { "Year",        "yr",  31557600.0, 0.0 } // 365.25 days
};

static const Unit powerUnits[] = {
    { "Watt",            "W",     1.0,    0.0 }, //base
    { "Kilowatt",        "kW",    1000.0, 0.0 },
    { "Megawatt",        "MW",    1e6,    0.0 },
    { "Horsepower",      "hp",    745.7,  0.0 },
    { "Erg per second",  "erg/s", 1e-7,   0.0 }
};

static const Unit forceUnits[] = {
    { "Newton",        "N",    1.0,    0.0 }, //base
    { "Dyne",          "dyn",  1e-5,   0.0 },
    { "Kilogram-force","kgf",  9.80665,0.0 },
    { "Pound-force",   "lbf",  4.44822,0.0 },
    { "Ounce-force",   "ozf",  0.278014,0.0 }
};

static const Unit frequencyUnits[] = {
    { "Hertz",                 "Hz",   1.0,     0.0 }, //base
    { "Kilohertz",             "kHz",  1000.0,  0.0 },
    { "Megahertz",             "MHz",  1e6,     0.0 },
    { "Gigahertz",             "GHz",  1e9,     0.0 },
    { "Revolutions per minute","rpm",  1.0/60.0,0.0 }
};
// Helper for array length
template <typename T, size_t N>
constexpr size_t countof(const T (&)[N]) { return N; }
// --- Catalog of views (all constexpr-friendly in C++11) ---
const UnitSet UnitCatalog[] = {
    { "Length",      lengthUnits,     countof(lengthUnits)     },
    { "Area",        areaUnits,       countof(areaUnits)       },
    { "Volume",      volumeUnits,     countof(volumeUnits)     },
    { "Mass",        massUnits,       countof(massUnits)       },
    { "Temperature", temperatureUnits,countof(temperatureUnits)},
    { "Energy",      energyUnits,     countof(energyUnits)     },
    { "Speed",       speedUnits,      countof(speedUnits)      },
    { "Pressure",    pressureUnits,   countof(pressureUnits)   },
    { "Data",        dataUnits,       countof(dataUnits)       },
    { "Angle",       angleUnits,      countof(angleUnits)      },
    { "Time",        timeUnits,       countof(timeUnits)       },
    { "Power",       powerUnits,      countof(powerUnits)      },
    { "Force",       forceUnits,      countof(forceUnits)      },
    { "Frequency",   frequencyUnits,  countof(frequencyUnits)  }
};
const size_t UnitCatalogCount = sizeof(UnitCatalog)/sizeof(UnitCatalog[0]);

// Map unit type choice index -> list source shown in A/B frames
const ProgmemTableSource* const allUnitLists[] = {
  &convLengthSrc,     
  &convAreaSrc,      
  &convVolumeSrc,
  &convMassSrc,
  &convTemperatureSrc,
  &convEnergySrc,
  &convSpeedSrc,
  &convPressureSrc,
  &convDataSrc,
  &convAngleSrc,
  &convTimeSrc,
  &convPowerSrc,
  &convForceSrc,
  &convFrequencySrc   
};

const size_t AllUnitListsCount = sizeof(allUnitLists)/sizeof(allUnitLists[0]);
const UnitSet* CurrentUnitSet = &UnitCatalog[0];          // default: Length
const ProgmemTableSource* CurrentUnitListSrc = &convLengthSrc;  // pairs with Length


FixedArenaSource<512, 16384> calcLines; 
ProgmemTableSource gHelpSrc(HELP_LINES, HELP_COUNT);
ProgmemTableSource unitTypesSrc(UNIT_TYPES_LINES, UNIT_TYPES_COUNT);
ProgmemTableSource convDirSrc(CONV_DIR_LINES, CONV_DIR_COUNT);
ProgmemTableSource convLengthSrc(CONV_LENGTH_LINES, CONV_LENGTH_COUNT);
ProgmemTableSource convAreaSrc(CONV_AREA_LINES, CONV_AREA_COUNT);
ProgmemTableSource convVolumeSrc(CONV_VOLUME_LINES, CONV_VOLUME_COUNT);
ProgmemTableSource convMassSrc(CONV_MASS_LINES, CONV_MASS_COUNT);
ProgmemTableSource convTemperatureSrc(CONV_TEMPERATURE_LINES, CONV_TEMPERATURE_COUNT);
ProgmemTableSource convEnergySrc(CONV_ENERGY_LINES, CONV_ENERGY_COUNT);
ProgmemTableSource convSpeedSrc(CONV_SPEED_LINES, CONV_SPEED_COUNT);
ProgmemTableSource convPressureSrc(CONV_PRESSURE_LINES, CONV_PRESSURE_COUNT);
ProgmemTableSource convDataSrc(CONV_DATA_LINES, CONV_DATA_COUNT);
ProgmemTableSource convAngleSrc(CONV_ANGLE_LINES, CONV_ANGLE_COUNT);
ProgmemTableSource convTimeSrc(CONV_TIME_LINES, CONV_TIME_COUNT);
ProgmemTableSource convPowerSrc(CONV_POWER_LINES, CONV_POWER_COUNT);
ProgmemTableSource convForceSrc(CONV_FORCE_LINES, CONV_FORCE_COUNT);
ProgmemTableSource convFrequencySrc(CONV_FREQUENCY_LINES, CONV_FREQUENCY_COUNT);
#pragma endregion

#pragma endregion
CALCState CurrentCALCState = CALC0;
Frame *CurrentFrameState = &calcScreen;
std::vector<String> prevTokens;
String cleanExpression = "";
String calculatedResult = "";
String prevLine = "";
std::map<String, float> variables = {};
int calcSwitchedStates = 0;
int trigType = 1;
int refresh_count = 0;
int currentFrameChoice = -1;

// <frame.cpp>
#pragma region frameSetup
Frame calcScreen(FRAME_LEFT,FRAME_RIGHT,FRAME_TOP,FRAME_BOTTOM,&calcLines,false,false);
Frame conversionScreen(FRAME_LEFT ,FRAME_RIGHT,FRAME_TOP + 80, FRAME_BOTTOM,&calcLines,false,true);
Frame helpScreen(FRAME_LEFT,FRAME_RIGHT,FRAME_TOP,FRAME_BOTTOM,&gHelpSrc,false,true);
Frame conversionUnit(FRAME_LEFT ,FRAME_RIGHT,FRAME_TOP, FRAME_BOTTOM + 136,&unitTypesSrc,false,true);
Frame conversionDirection(FRAME_LEFT + 120 ,FRAME_RIGHT + 120,FRAME_TOP + 40, FRAME_BOTTOM + 96,&convDirSrc,false,false);
Frame conversionFrameA(FRAME_LEFT + 24,FRAME_RIGHT + 184,FRAME_TOP + 40, FRAME_BOTTOM + 96,&convLengthSrc,true,true);
Frame conversionFrameB(FRAME_LEFT + 184,FRAME_RIGHT + 24,FRAME_TOP + 40, FRAME_BOTTOM + 96,&convLengthSrc,true,true);
std::vector<Frame*> frames = {};
#pragma endregion

Unit emptyUnit = {"","",NAN,NAN};
