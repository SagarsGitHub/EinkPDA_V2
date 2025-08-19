#ifndef GLOBALS_H
#define GLOBALS_H

// LIBRARIES
#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Adafruit_TCA8418.h>
#include <pgmspace.h>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include <Buzzer.h>
#include <USB.h>
#include <USBMSC.h>
#include <SD_MMC.h>
#include <Preferences.h>
#include "Adafruit_MPR121.h"
#include "esp_cpu.h"
#include "RTClib.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "FS.h"
#include "SPIFFS.h"
#include <sys/stat.h>
#include <iostream>

#include "assets.h"
#include "config.h"
#include <stdint.h>
#include <stddef.h>

// FONTS
// 9x7
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>
// 12x7
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
// U8G2 FONTS
//U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;
//u8g2_font_4x6_mf
//u8g2_font_5x7_mf
//u8g2_font_spleen5x8_mf
//u8g2_font_boutique_bitmap_9x9_tf
//u8g2_font_courR08_tf.h

// Display
extern GxEPD2_BW<GxEPD2_310_GDEQ031T10, GxEPD2_310_GDEQ031T10::HEIGHT> display;
//extern volatile bool useFastFullUpdate;
//extern volatile bool GxEPD2_310_GDEQ031T10::useFastFullUpdate;
//extern U8G2_SSD1326_ER_256X32_F_4W_HW_SPI u8g2;           // 256x32 SPI OLED
extern U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2;       // 128x32 I2C OLED
//extern U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
// Keypad
extern Adafruit_TCA8418 keypad;
extern char keysArray[4][10];
extern char keysArraySHFT[4][10];
extern char keysArrayFN[4][10];

// Buzzer
//extern Buzzer buzzer;

// Touch slider
extern Adafruit_MPR121 cap;

// RTC
//extern RTC_PCF8563 rtc;
extern const char daysOfTheWeek[7][12];

// USB
extern USBMSC msc;

// GENERAL

// Settings editable on-device
extern Preferences prefs;
extern int TIMEOUT;            // Time until automatic sleep (Seconds)
extern bool DEBUG_VERBOSE;     // Spit out some extra information
extern bool SYSTEM_CLOCK;      // Enable a small clock on the bottom of the screen.
extern bool SHOW_YEAR;         // Show the year on the clock
extern bool SAVE_POWER;        // Enable a slower CPU clock speed to save battery with little cost to performance
extern bool ALLOW_NO_MICROSD;  // Allow the device to operate with no SD card

extern volatile int einkRefresh;
extern int OLEDFPSMillis;
extern int KBBounceMillis;
extern volatile int timeoutMillis;
extern volatile int prevTimeMillis;
extern volatile bool TCA8418_event;
extern volatile bool PWR_BTN_event;
extern volatile bool SHFT;
extern volatile bool FN;
extern volatile bool newState;
extern bool noTimeout;
extern volatile bool OLEDPowerSave;
extern volatile bool disableTimeout;
extern volatile int battState;
extern volatile int prevBattState;
extern unsigned int flashMillis;
extern int prevTime;
extern uint8_t prevSec;
extern TaskHandle_t einkHandlerTaskHandle;
extern char currentKB[4][10];
extern volatile bool SDCARD_INSERT;
extern bool noSD;

enum KBState { NORMAL, SHIFT, FUNC };
extern KBState CurrentKBState;

extern uint8_t partialCounter;
extern volatile bool forceSlowFullUpdate;

enum AppState { HOME, TXT, FILEWIZ, USB_APP, BT, SETTINGS, TASKS, CALC };
extern const String appStateNames[];
extern const unsigned char *appIcons[7];
extern AppState CurrentAppState;


// <TXT.ino>
extern String currentWord;
extern String allText;
extern String prevAllText;
extern String prevLastLine;
extern bool prevBKSP;
extern int scroll;
extern int lines;
extern String outLines[13];
extern String lines_prev[13];
extern String filesList[MAX_FILES];
extern uint8_t fileIndex;
extern String editingFile;
extern String prevEditingFile;
extern String excludedFiles[3];

enum TXTState { TXT_, WIZ0, WIZ1, WIZ2, WIZ3, FONT };
extern TXTState CurrentTXTState;

extern String currentLine;
extern const GFXfont *currentFont;
extern uint8_t maxCharsPerLine;
extern uint8_t maxLines;
extern uint8_t fontHeight;
extern uint8_t lineSpacing;
extern volatile bool newLineAdded;
extern volatile bool doFull;
extern std::vector<String> allLines;
extern volatile long int dynamicScroll;
extern volatile long int prev_dynamicScroll;
extern int lastTouch;
extern unsigned long lastTouchTime;

// <CALC.cpp>
// max refreshes before a full refresh is forced (change to 5 for eink longevity)
#define REFRESH_MAX_CALC 10
#define SCROLL_MAX 8
#define SCROLL_MED 4
#define SCROLL_SML 2
#define FRAME_TOP 32                                  // top margin for large calc frame
#define FRAME_TOP_FOR_BOTTOM_TAB FRAME_TOP + 88       // top margin for 1/2 tab pinned to bottom
#define FRAME_LEFT 10                                 // left margin for large calc frame
#define FRAME_LEFT_FOR_RIGHT_TAB FRAME_LEFT + 152     // left margin for 1/2 tab pinned to right
#define FRAME_RIGHT 10                                // right margin for large calc frame
#define FRAME_RIGHT_FOR_LEFT_TAB FRAME_RIGHT + 152    // right margin for 1/2 tab pinned to left
#define FRAME_BOTTOM 32                               // bottom margin for large calc frame
#define FRAME_BOTTOM_FOR_TOP_TAB FRAME_BOTTOM + 88       // bottom margin for 1/2 tab pinned to top
enum CALCState { CALC0, CALC1, CALC2, CALC3, CALC4, CALCFONT };
struct Unit {
    const char* name;  
    const char* symbol;  
    // factor * (value + offset) = convert from in to basis unit
    // (inBase / to.factor) - to.offset  = convert from basis unit to output unit
    double factor;         
    double offset;         
};
struct UnitSet {
  const char* category;
  const Unit* data;
  size_t      size;
};
extern Unit emptyUnit;
extern CALCState CurrentCALCState;
extern int refresh_count;
extern String cleanExpression;
extern String calculatedResult;
extern int calcSwitchedStates;
extern String prevLine;
extern std::map<String, double> variables;
extern const char* operatorsCalc[];
extern const size_t operatorsCalcCount;
struct OpEntry { const char* token; uint8_t prec; bool rightAssoc; };
extern const OpEntry OPS[];
extern const size_t OPS_N;
extern const char* functionsCalc[];
extern const size_t functionsCalcCount;
extern const char* constantsCalc[];
extern const size_t constantsCalcCount;
extern std::vector<String> prevTokens;
extern int trigType;


// <TASKS.ino>
extern std::vector<std::vector<String>> tasks;
extern uint8_t selectedTask;
enum TasksState { TASKS0, TASKS0_NEWTASK, TASKS1, TASKS1_EDITTASK };
extern TasksState CurrentTasksState;
extern uint8_t newTaskState;
extern uint8_t editTaskState;
extern String newTaskName;
extern String newTaskDueDate;

// <HOME.ino>
enum HOMEState { HOME_HOME, NOWLATER };
extern HOMEState CurrentHOMEState;

// <FILEWIZ.ino>
enum FileWizState { WIZ0_, WIZ1_, WIZ1_YN, WIZ2_R, WIZ2_C, WIZ3_ };
extern FileWizState CurrentFileWizState;
extern String workingFile;

// <frameFunc.cpp>
# define MAX_FRAMES 100
# define X_OFFSET 4
#pragma region textSource
// bit flags for alignment or future options
enum LineFlags : uint8_t { LF_NONE=0, LF_RIGHT= 1<<0, LF_CENTER= 1<<1 };
struct LineView {
  const char* ptr;   // points to NUL-terminated string in RAM or PROGMEM
  uint16_t    len;   // byte length (no need to include '\0')
  uint8_t     flags; // LineFlags // To-Do switch from ~C~ and ~R~ to flags
};
// read-only interface for any line list (PROGMEM table, arena, etc.)
struct TextSource {
  virtual ~TextSource() {}
  virtual size_t   size() const = 0;
  virtual LineView line(size_t i) const = 0;
};
template<size_t MAX_LINES, size_t BUF_BYTES>
struct FixedArenaSource : TextSource {
  char     buf[BUF_BYTES];
  uint16_t off[MAX_LINES];
  uint16_t len_[MAX_LINES];
  uint8_t  flags_[MAX_LINES];
  size_t   nLines = 0;
  size_t   used   = 0;

  size_t size() const override { return nLines; }

  LineView line(size_t i) const override {
    return { buf + off[i], len_[i], flags_[i] };
  }

  void clear() { nLines = 0; used = 0; }

  // Returns false if out of capacity
  bool pushLine(const char* s, uint16_t L, uint8_t flags = LF_NONE) {
    if (nLines >= MAX_LINES || used + L + 1 > BUF_BYTES) return false;
    memcpy(buf + used, s, L);
    buf[used + L] = '\0';
    off[nLines]   = (uint16_t)used;
    len_[nLines]  = L;
    flags_[nLines]= flags;
    used         += L + 1;
    nLines++;
    return true;
  }
};
struct ProgmemTableSource : TextSource {
  // table is a PROGMEM array of PROGMEM pointers to '\0'-terminated strings
  const char* const* table; // PROGMEM
  size_t count;

  ProgmemTableSource(const char* const* t, size_t n) : table(t), count(n) {}

  size_t size() const override { return count; }

  LineView line(size_t i) const override {
    const char* p = (const char*)pgm_read_ptr(&table[i]);
    // length from PROGMEM
    uint16_t L = (uint16_t)strlen_P(p);
    return { p, L, LF_NONE };
  }
};

extern const char* const HELP_LINES[] PROGMEM;
extern const size_t HELP_COUNT;
extern const char* const UNIT_TYPES_LINES[] PROGMEM;
extern const size_t UNIT_TYPES_COUNT;
extern const char* const CONV_DIR_LINES[] PROGMEM;
extern const size_t CONV_DIR_COUNT;
extern const char* const CONV_LENGTH_LINES[] PROGMEM;
extern const size_t CONV_LENGTH_COUNT;
extern const char* const CONV_AREA_LINES[] PROGMEM;
extern const size_t CONV_AREA_COUNT;
extern const char* const CONV_VOLUME_LINES[] PROGMEM;
extern const size_t CONV_VOLUME_COUNT;
extern const char* const CONV_MASS_LINES[] PROGMEM;
extern const size_t CONV_MASS_COUNT;
extern const char* const CONV_TEMPERATURE_LINES[] PROGMEM;
extern const size_t CONV_TEMPERATURE_COUNT;
extern const char* const CONV_ENERGY_LINES[] PROGMEM;
extern const size_t CONV_ENERGY_COUNT;
extern const char* const CONV_SPEED_LINES[] PROGMEM;
extern const size_t CONV_SPEED_COUNT;
extern const char* const CONV_PRESSURE_LINES[] PROGMEM;
extern const size_t CONV_PRESSURE_COUNT;
extern const char* const CONV_DATA_LINES[] PROGMEM;
extern const size_t CONV_DATA_COUNT;
extern const char* const CONV_ANGLE_LINES[] PROGMEM;
extern const size_t CONV_ANGLE_COUNT;
extern const char* const CONV_TIME_LINES[] PROGMEM;
extern const size_t CONV_TIME_COUNT;
extern const char* const CONV_POWER_LINES[] PROGMEM;
extern const size_t CONV_POWER_COUNT;
extern const char* const CONV_FORCE_LINES[] PROGMEM;
extern const size_t CONV_FORCE_COUNT;
extern const char* const CONV_FREQUENCY_LINES[] PROGMEM;
extern const size_t CONV_FREQUENCY_COUNT;



extern const UnitSet UnitCatalog[];
extern const size_t  UnitCatalogCount;

extern const ProgmemTableSource* const allUnitLists[];
extern const size_t              AllUnitListsCount;

extern const UnitSet* CurrentUnitSet;
extern const ProgmemTableSource* CurrentUnitListSrc;

extern FixedArenaSource<512, 16384> calcLines;
extern ProgmemTableSource helpSrc;
extern ProgmemTableSource unitTypesSrc;
extern ProgmemTableSource convDirSrc;
extern ProgmemTableSource convLengthSrc;
extern ProgmemTableSource convAreaSrc;
extern ProgmemTableSource convVolumeSrc;
extern ProgmemTableSource convMassSrc;
extern ProgmemTableSource convTemperatureSrc;
extern ProgmemTableSource convEnergySrc;
extern ProgmemTableSource convSpeedSrc;
extern ProgmemTableSource convPressureSrc;
extern ProgmemTableSource convDataSrc;
extern ProgmemTableSource convAngleSrc;
extern ProgmemTableSource convTimeSrc;
extern ProgmemTableSource convPowerSrc;
extern ProgmemTableSource convForceSrc;
extern ProgmemTableSource convFrequencySrc;
extern const UnitSet* CurrentUnitSet; 

#pragma endregion
#pragma region frameSetup
class Frame {
public:
  // what kind of content this frame holds
  enum class Kind : uint8_t { none, text, bitmap };

  // geometry
  int left, right, top, bottom;
  int extendedBottom, originalBottom;

  // flags/state switch to uint8_t flag
  bool cursor   = false;
  bool box      = false;
  bool invert   = false;
  bool overlap  = false;

  int   choice     = -1;
  long  scroll     = 0;
  long  prevScroll = -1;
  int   maxLines   = 0;
  long  lastTotal  = -1;

  // content (only one valid at a time)
  Kind kind = Kind::none;
  const TextSource* source = nullptr;  // for text frames
  const uint8_t* bitmap    = nullptr;  // for bitmap frames
  int bitmapW = 0;
  int bitmapH = 0;
  
  const Unit *unit = nullptr;
  // base constructor for common fields
  Frame(int left, int right, int top, int bottom, bool cursor=false, bool box=false)
  : left(left), right(right), top(top), bottom(bottom),
    extendedBottom(bottom), originalBottom(bottom),
    cursor(cursor), box(box) {}

  // constructor for text frames
  Frame(int left, int right, int top, int bottom,
        const TextSource* linesPtr,
        bool cur=false, bool bx=false)
  : Frame(left, right, top, bottom, cur, bx) {
    kind   = Kind::text;
    source = linesPtr;
  }

  // constructor for bitmap frames
  Frame(int left, int right, int top, int bottom,
        const uint8_t* bitmapPtr, int width, int height,
        bool cursor=false, bool box=false)
  : Frame(left, right, top, bottom, cursor, box) {
    kind      = Kind::bitmap;
    bitmap    = bitmapPtr;
    bitmapW      = width;
    bitmapH      = height;
  }

  bool hasText()   const { return kind == Kind::text   && source; }
  bool hasBitmap() const { return kind == Kind::bitmap && bitmap; }
};
extern Frame calcScreen;
extern Frame conversionScreen;
extern Frame helpScreen;
extern Frame conversionUnit;
extern Frame conversionDirection;
extern Frame conversionFrameA;
extern Frame conversionFrameB;
extern Frame conversionTypes;
extern Frame programmingScreen;
extern Frame numberSizeFrame;
extern Frame binaryFrame;
extern Frame decimalFrame;
extern Frame hexadecimalFrame;
extern Frame octalFrame;
extern Frame testBitmapScreen;
extern Frame testTextScreen;
extern Frame *CurrentFrameState;
extern int currentFrameChoice;
extern int frameSelection;
extern std::vector<Frame*> frames;
#pragma endregion

// FUNCTION PROTOTYPES
// <sysFunc.ino>
// SYSTEM
void checkTimeout();
void PWR_BTN_irq();
void TCA8418_irq();
char updateKeypress();
void printDebug();
String vectorToString();
void stringToVector(String inputText);
void saveFile();
void writeMetadata(fs::FS &fs, const String& path);
void loadFile();
void delFile( String fileName);
void deleteMetadata(fs::FS &fs, String path);
void renFile(String oldFile, String newFile);
void renMetadata(fs::FS &fs, String oldPath, String newPath);
void copyFile(String oldFile, String newFile);
void updateBattState();
String removeChar(String str, char character);
void appendToFile(String path, String inText);
void setCpuSpeed(int newFreq);
void playJingle(String jingle);
void deepSleep(bool alternateScreenSaver = false);

// microSD
void listDir(fs::FS &fs, const char *dirname);
void readFile(fs::FS &fs, const char *path);
String readFileToString(fs::FS &fs, const char *path);
void writeFile(fs::FS &fs, const char *path, const char *message);
void appendFile(fs::FS &fs, const char *path, const char *message);
void renameFile(fs::FS &fs, const char *path1, const char *path2);
void deleteFile(fs::FS &fs, const char *path);
void setTimeFromString(String timeStr);

// <OLEDFunc.ino>
void oledWord(String word);
void oledLine(String line, bool doProgressBar = true);
void oledScroll();
void infoBar();

// <einkFunc.ino>
void refresh();
void einkHandler(void *parameter);
void statusBar(String input, bool fullWindow = false);
void einkTextPartial(String text, bool noRefresh = false);
void drawThickLine(int x0, int y0, int x1, int y1, int thickness);
int  countLines(String input, size_t maxLineLength = 29);
void einkTextDynamic(bool doFull_, bool noRefresh = false);
void setTXTFont(const GFXfont *font);
void setFastFullRefresh(bool setting);
void drawStatusBar(String input);
void drawCalc(); // Calc

// <FILEWIZ.ino>
void processKB_FILEWIZ();
void einkHandler_FILEWIZ();

// SPIFFS
void listDir(fs::FS &fs, const char *dirname);
void readFile(fs::FS &fs, const char *path);
String readFileToString(fs::FS &fs, const char *path);
void writeFile(fs::FS &fs, const char *path, const char *message);
void appendFile(fs::FS &fs, const char *path, const char *message);
void renameFile(fs::FS &fs, const char *path1, const char *path2);
void deleteFile(fs::FS &fs, const char *path);

// <TXT.ino>
void TXT_INIT();
void processKB_TXT();
void einkHandler_TXT();
void processKB_TXT_NEW();
void einkHandler_TXT_NEW();
bool splitIntoLines(const char* input, int scroll_);
int countWords(String str);
int countVisibleChars(String input);
void updateScrollFromTouch();

// <CALC.cpp>
  // main functions
void einkHandler_CALC();
void processKB_CALC();
void CALC_INIT();
void closeCalc(AppState newAppState); // calc eink function
inline void calcClear() { calcLines.clear(); }
  // strings
void calcCRInput();
String formatNumber(double value);
String formatScientific(double value);
String trimValue(double value);
void printAnswer(String inputString,const Unit *convA,const Unit *convB); 
int calculate(const String& cleanedInput,String &resultOutput,const Unit *convA,const Unit *convB);
  // algorithms
std::deque<String> convertToRPN(String expression);
String evaluateRPN(std::deque<String> rpnQueue,const Unit *convA,const Unit *convB);
std::vector<String> tokenize(const String& expression);
  // helpers
void updateScroll(Frame *currentFrameState,int prevScroll,int currentScroll, bool reset = false);
bool isNumberToken(const String& token);
bool isVariableToken(const String& token);
bool isFunctionToken(const String& token);
bool isConstantToken(const String& token);
double convertTrig(double input, int trigType,bool reverse = false);
double convert(double value,const Unit *from,const Unit *to);
void calcAppend(const String& s, bool right=false, bool center=false);
const Unit* getUnit(const String& sym);
void selectUnitType(int idx);

// <HOME.ino>
void einkHandler_HOME();
void processKB_HOME();
void commandSelect(String command);
void drawHome();

// <TASKS.ino>
void sortTasksByDueDate(std::vector<std::vector<String>> &tasks);
void addTask(String taskName, String dueDate, String priority, String completed);
void updateTaskArray();
void updateTasksFile();
void deleteTask(int index);
String convertDateFormat(String yyyymmdd);
void einkHandler_TASKS();
void processKB_TASKS();

// <FRAMES.cpp>
  // main functions
void einkTextFramesDynamic(std::vector<Frame*> &frames, bool doFull_, bool noRefresh);
  // text boxes
std::vector<String> formatText(Frame &frame,int maxTextWidth);
void drawLineInFrame(String &srcLine, int lineIndex, Frame &frame, int usableY, bool clearLine, bool isPartial);
void drawFrameBox(int usableX, int usableY, int usableWidth, int usableHeight,bool invert);
  // String formatting
static size_t sliceThatFits(const char* s, size_t n, int maxTextWidth);
std::vector<String> sourceToVector(const TextSource* src);
String frameChoiceString(const Frame& f);  
  //scroll
void updateScrollFromTouch_Frame(); 
void oledScrollFrame(); 
void updateScroll(Frame *currentFrameState,int prevScroll,int currentScroll, bool reset);
  // helpers
void getVisibleRange(Frame *f, long totalLines, long &startLine, long &endLine);
int computeCursorX(Frame &frame, bool rightAlign, bool centerAlign, int16_t x1, uint16_t lineWidth);
int alignUp8(int v);
int alignDown8(int v);

// <PocketMage>
void applicationEinkHandler();
void processKB();

#endif // GLOBALS_H