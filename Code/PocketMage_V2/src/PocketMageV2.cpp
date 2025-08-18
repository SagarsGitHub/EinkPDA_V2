// PocketMage V3.0
// @Ashtf 2025

#include "globals.h"

//        .o.       ooooooooo.   ooooooooo.    .oooooo..o  //
//       .888.      `888   `Y88. `888   `Y88. d8P'    `Y8  //
//      .8"888.      888   .d88'  888   .d88' Y88bo.       //
//     .8' `888.     888ooo88P'   888ooo88P'   `"Y8888o.   //
//    .88ooo8888.    888          888              `"Y88b  //
//   .8'     `888.   888          888         oo     .d8P  //
//  o88o     o8888o o888o        o888o        8""88888P'   //

// ADD E-INK HANDLER APP SCRIPTS HERE
void applicationEinkHandler() {
  switch (CurrentAppState) {
    case HOME:
      einkHandler_HOME();
      break;
    case TXT:
      switch (TXT_APP_STYLE) {
        case 0:
          einkHandler_TXT();
          break;
        case 1:
          einkHandler_TXT_NEW();
          break;
      }
      break;
    case FILEWIZ:
      einkHandler_FILEWIZ();
      break;
    case TASKS:
      einkHandler_TASKS();
      break;
    // ADD APP CASES HERE
    case CALC:
      einkHandler_CALC();
      break;
    default:
      einkHandler_HOME();
      break;
  }
}

// ADD PROCESS/KEYBOARD APP SCRIPTS HERE
void processKB() {
  switch (CurrentAppState) {
    case HOME:
      processKB_HOME();
      break;
    case TXT:
      switch (TXT_APP_STYLE) {
        case 0:
          processKB_TXT();
          break;
        case 1:
          processKB_TXT_NEW();
          break;
      }
      break;
    case FILEWIZ:
      processKB_FILEWIZ();
      break;
    case TASKS:
      processKB_TASKS();
      break;
    // ADD APP CASES HERE
    case CALC:
      processKB_CALC();
      break;
    default:
      processKB_HOME();
      break;
  }
}

//  ooo        ooooo       .o.       ooooo ooooo      ooo  //
//  `88.       .888'      .888.      `888' `888b.     `8'  //
//   888b     d'888      .8"888.      888   8 `88b.    8   //
//   8 Y88. .P  888     .8' `888.     888   8   `88b.  8   //
//   8  `888'   888    .88ooo8888.    888   8     `88b.8   //
//   8    Y     888   .8'     `888.   888   8       `888   //
//  o8o        o888o o88o     o8888o o888o o8o        `8   //

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////|
// SETUP
void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);
  //SPI.begin(SPI_SCK, -1, SPI_MOSI, -1);

  // OLED SETUP
  u8g2.begin();
  //u8g2.setBusClock(10000000);
  u8g2.setPowerSave(0);
  u8g2.clearBuffer();
  u8g2.sendBuffer();

  // SHOW "PocketMage" while DEVICE BOOTS
  oledWord("   PocketMage   ");

  // STARTUP JINGLE
  //playJingle("startup");

  // LOAD PREFERENCES
  prefs.begin("settings", true); // Open in read-only mode
  TIMEOUT           = prefs.getInt("TIMEOUT", 240);
  DEBUG_VERBOSE     = prefs.getBool("DEBUG_VERBOSE", true);
  SYSTEM_CLOCK      = prefs.getBool("SYSTEM_CLOCK", true);
  SHOW_YEAR         = prefs.getBool("SHOW_YEAR", true);
  SAVE_POWER        = prefs.getBool("SAVE_POWER", true);
  ALLOW_NO_MICROSD  = prefs.getBool("ALLOW_NO_SD", false);
  prefs.end();

  // WAKE INTERRUPT SETUP
  pinMode(KB_IRQ, INPUT);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_8, 0);

  // KEYBOARD SETUP
  if (!keypad.begin(TCA8418_DEFAULT_ADDR, &Wire)) {
    Serial.println("Error Initializing the Keyboard");
    while (1);
  }
  keypad.matrix(4, 10);
  attachInterrupt(digitalPinToInterrupt(KB_IRQ), TCA8418_irq, CHANGE);
  keypad.flush();
  keypad.enableInterrupts();


  // Create /sys folder if it does not exist
  if (!SPIFFS.exists("/sys")) {
    if (SPIFFS.mkdir("/sys")) Serial.println("Created /sys folder.");
    else                      Serial.println("Failed to create /sys folder.");
  }

  // EINK HANDLER SETUP
  display.init(115200);
  display.setRotation(3);
  display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();
  newState = true;

  xTaskCreatePinnedToCore(
    einkHandler,             // Function name (your user-defined function)
    "einkHandlerTask",       // Task name
    10000,                   // Stack size (in bytes)
    NULL,                    // Parameters (none in this case)
    1,                       // Priority (1 is low priority)
    &einkHandlerTaskHandle,  // Task handle
    0                        // Core ID (0 for core 0, 1 for core 1)
  );

  // POWER SETUP
  pinMode(PWR_BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PWR_BTN), PWR_BTN_irq, FALLING);
  pinMode(CHRG_SENS, INPUT);
  pinMode(BAT_SENS, INPUT);
  //WiFi.mode(WIFI_OFF);
  //btStop();

  // SET CPU CLOCK FOR POWER SAVE MODE
  if (SAVE_POWER) setCpuFrequencyMhz(40 );
  else            setCpuFrequencyMhz(240);

  // MPR121 / SLIDER
  if (!cap.begin(0x5A)) {
    Serial.println("TouchPad Failed");
    oledWord("TouchPad Failed");
  }
  // FRAMES SETUP
  frames.reserve(MAX_FRAMES);
  //oledWord("setting cap thershold to 5,1");
  
  // Step 1: Stop electrode operation
  cap.writeRegister(0x5E, 0x00); // ECR = 0x00 (stop mode)

  // Step 2: Set touch/release thresholds (ELE0–ELE2 example)
  cap.writeRegister(0x41, 0x03); // ELE0 Touch
  cap.writeRegister(0x4B, 0x01); // ELE0 Release
  cap.writeRegister(0x42, 0x03); // ELE1 Touch
  cap.writeRegister(0x4C, 0x01); // ELE1 Release
  cap.writeRegister(0x43, 0x01); // ELE2 Touch
  cap.writeRegister(0x4D, 0x00); // ELE2 Release
  
  // Step 3: Set filter & debounce settings (optional, but good idea)
  cap.writeRegister(0x2B, 0x01); // MHD rising
  cap.writeRegister(0x2C, 0x01); // NHD amount
  // 0x00 before
  cap.writeRegister(0x2D, 0x10); // NCL rising
  cap.writeRegister(0x2E, 0x00); // FDL rising

  cap.writeRegister(0x2F, 0x01); // MHD falling
  cap.writeRegister(0x30, 0x01); // NHD falling
  // 0xFF before
  cap.writeRegister(0x31, 0x64); // NCL falling
  cap.writeRegister(0x32, 0x30); // FDL falling
  
  cap.writeRegister(0x5B, 0x01); // Debounce: 1 touch, 1 release

  // Step 4: Configure baseline tracking control
  cap.writeRegister(0x7B, 0x00); // Baseline filter config (optional for tuning)
  //cap.writeRegister(0x7E, 90);  // Target level
  //cap.writeRegister(0x7C, 117); // USL = 90 * 1.3
  //cap.writeRegister(0x7D, 63);  // LSL = 90 * 0.7
  // Step 5: Enable 3 electrodes with baseline tracking
  cap.writeRegister(0x5E, 0b10000000 + 12); // ECR: 0b10000011 (baseline tracking + 3 electrodes)
  
  //cap.setThresholds(7,5);

  

  // SPIFFS SETUP
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    oledWord("SPIFFS Failed");
    delay(1000);
  }
  // RTC SETUP
  //pinMode(RTC_INT, INPUT);
  //if (!rtc.begin()) {
  //  Serial.println("Couldn't find RTC");
  //  Serial.flush();
  //   delay(1000);
  //}
  // SET CLOCK IF NEEDED
  //if (SET_CLOCK_ON_UPLOAD || rtc.lostPower()) rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //rtc.start();

  // Set "random" seed
  randomSeed(analogRead(BAT_SENS));
  // This would make any call to random use the esp hardware
  // randomSeed sets useRealRandomGenerator false, so this would need to replace randomSeed
  useRealRandomGenerator(true);


}

void loop() {
  if (!noTimeout)  checkTimeout();
  //if (DEBUG_VERBOSE) printDebug();

  updateBattState();
  processKB();
  
  // Yield to watchdog
  vTaskDelay(50 / portTICK_PERIOD_MS);
  yield();
}