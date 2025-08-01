#ifndef GLOBALS_H
#define GLOBALS_H

// LIBRARIES
#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Adafruit_TCA8418.h>
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
enum CALCState { CALC0, CALC1, CALC2, CALC3, CALC4, CALCFONT };
// max refreshes before a full refresh is forced (change to 5 for eink longevity)
#define REFRESH_MAX_CALC 10
#define SCROLL_MAX 8
#define SCROLL_MED 4
#define SCROLL_SML 2
extern CALCState CurrentCALCState;
extern int refresh_count;
extern std::vector<String> allLinesCalc;
extern String cleanExpression;
extern String calculatedResult;
extern int calcSwitchedStates;
extern String prevLine;
extern std::map<String, float> variables;
extern  std::set<String> operatorsCalc;
extern  std::set<String> functionsCalc;
extern std::set<String> constantsCalc;
extern std::map<String, int> precedenceCalc;
extern std::vector<String> helpText;
extern char bufferString[20];
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
void einkCalcDynamic(bool doFull_, bool noRefresh = false);  // Calc

// <FILEWIZ.ino>
void processKB_FILEWIZ();
void einkHandler_FILEWIZ();

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

// <CALC.ino>
void einkHandler_CALC();
void processKB_CALC();
void CALC_INIT();
void closeCalc(AppState newAppState); // calc eink function
void oledScrollCalc(); // calc oled function
void updateScrollFromTouch_Calc(); // new processSB_Calc?
int calculate(const String& cleanedInput,String &resultOutput);
std::deque<String> convertToRPN(String expression);
String evaluateRPN(std::deque<String> rpnQueue);
std::vector<String> tokenize(const String& expression);
void calcCRInput();
String formatNumber(double value);
void printAnswer(String resultOutput); 
bool isNumberToken(const String& token);
bool isVariableToken(const String& token);
bool isFunctionToken(const String& token);
bool isOperatorToken(const String& token);
bool isConstantToken(const String& token);
double convertTrig(double input, int trigType,bool reverse = false);



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


// <PocketMage>
void applicationEinkHandler();
void processKB();

#endif // GLOBALS_H