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
// make convertToRPN && evaluateRPN able to handle standard notation inputs from other apps
// implement  complex numbers + matrix multiplication
// implement for loops and conditionals
// programming mode

///////////////////////////// MAIN FUNCTIONS
// CALC INITIALIZETION
void CALC_INIT() {
  // open calc
  CurrentAppState = CALC;
  CurrentCALCState = CALC0;
  CurrentKBState = FUNC;
  CurrentFrameState = &calcScreen;
  frames.push_back(&calcScreen);
  conversionFrameA.choice = 0;
  conversionFrameB.choice = 0;
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
    updateScrollFromTouch_Calc();
    
    switch (CurrentCALCState) {

      // CONVERSIONS MODE 
      case CALC3:
      /*
        //SHIFT Recieved
        else if (inchar == 17) {                                  
          CurrentFrameState = &conversionFrameA;
          dynamicScroll = CurrentFrameState->scroll;
          prev_dynamicScroll = -1;
          conversionFrameA.bottom = FRAME_BOTTOM + 8;
          conversionFrameB.bottom = FRAME_BOTTOM + 96;
          calcSwitchedStates = 1;
          newLineAdded = true;
        }
        //FN Recieved
        else if (inchar == 18) {                                  
          CurrentFrameState = &conversionFrameB;
          dynamicScroll = CurrentFrameState->scroll;
          prev_dynamicScroll = -1;
          conversionFrameA.bottom = FRAME_BOTTOM + 96;
          conversionFrameB.bottom = FRAME_BOTTOM + 8;
          calcSwitchedStates = 1;
          newLineAdded = true;
        }
      */
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
            Serial.println("Performed character replacement for ',,' ");
            currentLine[currentLine.length()-1] = '.';
          } else {
            currentLine += ",";
          }
        }
        // TAB Recieved (switch unit types aka length to area)
        else if (inchar == 9 || inchar == 14) {    
                        
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
            CurrentFrameState->bottom = FRAME_BOTTOM + 96;
            CurrentFrameState = &conversionScreen;
            dynamicScroll = CurrentFrameState->scroll;
            prev_dynamicScroll = -1;

            calcSwitchedStates = 1;
            newLineAdded = true;
          } 
        }
        // CR Recieved
        else if (inchar == 13) {    
          calcCRInput();                      
        }
        // ESC / CLEAR Recieved
        else if (inchar == 20) {   
          if (CurrentFrameState == &conversionScreen){                               
            CurrentFrameState->lines->clear();
            currentLine = "";
            oledWord("Clearing...");
            drawCalc();
            Serial.println("In CALC3 Mode calling frames");
            einkTextFramesDynamic(frames,false,false);
            display.refresh();
            delay(300);
          }
        }
        // LEFT (scroll up)
        else if (inchar == 19 || inchar == 5) {
          if (CurrentFrameState != &conversionFrameA){
            CurrentFrameState = &conversionFrameA;
            dynamicScroll = CurrentFrameState->scroll;
            prev_dynamicScroll = -1;
            conversionFrameA.bottom = FRAME_BOTTOM + 8;
            conversionFrameB.bottom = FRAME_BOTTOM + 96;

            calcSwitchedStates = 1;
            newLineAdded = true;
          }
          newLineAdded = true; 
          updateScroll(CurrentFrameState,prev_dynamicScroll,dynamicScroll);                         
        }
        // RIGHT (switch to conversion a tab or scroll down in tab)
        else if (inchar == 21 || inchar == 6) { 
          if (CurrentFrameState != &conversionFrameB){
            CurrentFrameState = &conversionFrameB;
            dynamicScroll = CurrentFrameState->scroll;
            prev_dynamicScroll = -1;
            conversionFrameA.bottom = FRAME_BOTTOM + 96;
            conversionFrameB.bottom = FRAME_BOTTOM + 8;
            calcSwitchedStates = 1;
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
          else oledScrollCalc();
        }
        break;

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
            Serial.println("Performed character replacement for ',,' ");
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
          CurrentFrameState->lines->clear();
          currentLine = "";
          oledWord("Clearing...");
          drawCalc();
          einkTextFramesDynamic(frames,false,false);
          display.refresh();
          delay(300);
        }
        // LEFT (scroll up)
        else if (inchar == 19 || inchar == 5) {
          if (dynamicScroll < CurrentFrameState->lines->size() - (9 + SCROLL_MAX)){
             dynamicScroll += SCROLL_MAX;
          } else if (dynamicScroll < CurrentFrameState->lines->size() - (9 + SCROLL_MED)){
             dynamicScroll += SCROLL_MED;
          } else if (dynamicScroll < CurrentFrameState->lines->size() - (9 + SCROLL_SML)){
             dynamicScroll += SCROLL_SML;
          } else if (dynamicScroll < CurrentFrameState->lines->size() - 10) {
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
          CurrentFrameState->lines->clear();
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
          else oledScrollCalc();
        }
        break;
      // HELP MODE (no need inputs)
      case CALC4:
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
          //STORE ALLLINES IN TEMP BEFORE REFRESHING SCREEN TO AVOID DRAWING MULTIPLE FONTS
          std::vector<String> tempLines = *(CurrentFrameState->lines);
          CurrentFrameState->lines->clear();

          // REFRESH SCREEN
          refresh();
          drawCalc();
          *(CurrentFrameState->lines) = tempLines;
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
void einkHandler_CALC() {
  if (newLineAdded || newState) {
    // reset eink screen if returning from a new mode
    if (calcSwitchedStates == 1){
      calcSwitchedStates = 0;
      if (CurrentCALCState != CALC3)  {
          CurrentFrameState->lines->clear();
          doFull = 1;
          drawCalc();
          frames.clear();
          frames.push_back(&calcScreen);
      }      
      doFull = 1;
      drawCalc();
      einkTextFramesDynamic(frames,true,false);
    } else {
      switch (CurrentCALCState) {
        case CALC0:
          //standard mode
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
        case CALC1:
          //programming mode
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
                      Serial.println("In CALC3 Mode calling frames");
            einkTextFramesDynamic(frames,true,false);
            //refresh();
            doFull = false;
          } else if (newLineAdded && !newState) {
            refresh_count++;
            if (refresh_count > REFRESH_MAX_CALC){
              drawCalc(); 
              setFastFullRefresh(false);
                        Serial.println("In CALC3 Mode calling frames");
              einkTextFramesDynamic(frames,true,false);
              refresh_count = 0;
            } else {
                        Serial.println("In CALC3 Mode calling frames");
              einkTextFramesDynamic(frames,true,false);
            }
            setFastFullRefresh(true);
          } else if (newState && !newLineAdded) {
            drawCalc();
          }
          break;
        case CALC4:
          //help mode
          currentFont = &FreeMonoBold9pt7b;
          setTXTFont(currentFont);
          // print out everything needed to understand basics of program, might be memory inefficient, remove or rector
          CurrentFrameState->lines->clear();
          // potentially store helpText in SD card
          CurrentFrameState->lines->insert(CurrentFrameState->lines->end(), helpText.begin(), helpText.end());
          dynamicScroll = (CurrentFrameState->lines->size() - 11);
          updateScroll(CurrentFrameState,prev_dynamicScroll,dynamicScroll);     
          drawCalc(); 
          einkTextFramesDynamic(frames,true,false);
          calcSwitchedStates = true;
          setFastFullRefresh(false);
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
    if ( CurrentCALCState == CALC4) {
      calcSwitchedStates = 1;
      CurrentCALCState = CALC0;
    }
  }
}

///////////////////////////// CALC SCROLL FUNCTIONS
// could be abstracted to a processSB_Calc function with an interface set up for every app
void updateScrollFromTouch_Calc() {
  uint16_t touched = cap.touched();  // Read touch state
  int newTouch = -1;
  // Find the first active touch point (lowest index first)
  for (int i = 0; i < 3; i++) {
    if (touched & (1 << i)) {
      newTouch = i;
      Serial.print("Prev pad: ");
      Serial.print(lastTouch);
      Serial.print("   Touched pad: ");
      Serial.println(newTouch);
      break;
    }
  }

  unsigned long currentTime = millis();
  
  if (newTouch != -1) {  // If a touch is detected
    if (lastTouch != -1) {  // Compare with previous touch
      int touchDelta = abs(newTouch - lastTouch);
      if (touchDelta < 2) {  // Ignore large jumps (adjust threshold if needed)
        int maxScroll = max(0, (int)CurrentFrameState->lines->size() - 1);  // Ensure a valid scroll range
        dynamicScroll = CurrentFrameState->scroll;
        if (newTouch > lastTouch) {
          dynamicScroll = min((int)(dynamicScroll + 1), maxScroll);

        } else if (newTouch < lastTouch) {
          dynamicScroll = max((int)(dynamicScroll - 1), 0);
        }
        Serial.println("updating scroll to: " + String(dynamicScroll));
        Serial.println("max scroll is = " + String((int)CurrentFrameState->lines->size()));
        if (CurrentFrameState->choice != -1){
          CurrentFrameState->choice = dynamicScroll;
          CurrentFrameState->unitA = getUnit((*CurrentFrameState->lines)[CurrentFrameState->choice]);
        }
        updateScroll(CurrentFrameState,prev_dynamicScroll,dynamicScroll);     
      }
    }
    lastTouch = newTouch;  // Always update lastTouch
    lastTouchTime = currentTime;  // Reset timeout timer
  } 
  else if (lastTouch != -1) {
    if (currentTime - lastTouchTime > TOUCH_TIMEOUT_MS) {
        lastTouch = -1;
        if (prev_dynamicScroll != dynamicScroll) {
            newLineAdded = true;
            prev_dynamicScroll = dynamicScroll; // Update comparison value
            updateScroll(CurrentFrameState,prev_dynamicScroll,dynamicScroll);     

        }
    }
  }

    //Serial.println("update scroll has finished!");
}

///////////////////////////// CALC EINK FUNCTIONS
// CLOSE CALC AND UPDATE
void closeCalc(AppState newAppState){
  // essential to display next app correctly 
  display.setFullWindow();

  display.fillScreen(GxEPD_WHITE);
  u8g2.clearBuffer();
  dynamicScroll = 0;
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

///////////////////////////// CALC OLED FUNCTIONS
// calc specific oled
// modified from oledScroll
// modifications:
//     removed tab drawing if line starts with "   " since all answers are right aligned
void oledScrollCalc() {
  // CLEAR DISPLAY
u8g2.clearBuffer();
if (CurrentCALCState != CALC3){}
// draw background
u8g2.drawXBMP(0, 0, 128, 32, scrolloled0);
  // draw lines preview
  long int count = CurrentFrameState->lines->size();
  long startIndex, endIndex;
  getVisibleRange(CurrentFrameState, count, startIndex, endIndex);

    // Decide how many preview lines to show
  long previewTop = max(0L, endIndex - CurrentFrameState->scroll - CurrentFrameState->maxLines);
  long previewBottom= endIndex - 1; // index of newest visible line (may be < 0)
  const int rowStep = 4;    
  const int baseY   = 28; 

  for (long int i = previewBottom; i >= previewTop && i >= 0;  --i) {
    if (i < 0 || i >= count) continue;  // Ensure i is within bounds

    int16_t x1, y1;
    uint16_t charWidth, charHeight;
    String line = (*(CurrentFrameState->lines))[i];

    // measure width (same as you did)
    if (line.startsWith("    ")) {
      display.getTextBounds(line.substring(4), 0, 0, &x1, &y1, &charWidth, &charHeight);
      int lineWidth = map(charWidth, 0, 320, 0, 49);
      lineWidth = constrain(lineWidth, 0, 49);

      // compute Y based on distance from newest visible line
      long posFromBottom = previewBottom - i; // 0 = newest
      int boxY = baseY - (rowStep * posFromBottom);
      if (boxY >= 0) {
        // you removed the tab box drawing originally; uncomment if you want it:
        // u8g2.drawBox(68, boxY, lineWidth, 2);
      }
    } else {
      display.getTextBounds(line, 0, 0, &x1, &y1, &charWidth, &charHeight);
      int lineWidth = map(charWidth, 0, 320, 0, 56);
      lineWidth = constrain(lineWidth, 0, 56);

      long posFromBottom = previewBottom - i; // 0 = newest
      int boxY = baseY - (rowStep * posFromBottom);
      if (boxY >= 0) {
        u8g2.drawBox(61, boxY, lineWidth, 2);
      }
    }
  }
  long displayedLinesStart = startIndex + 1;
  long displayedLinesEnd   = endIndex; // endLine is one-past-last, so this already matches 1-based inclusive end
  if (count == 0) {
    displayedLinesStart = 0;
    displayedLinesEnd  = 0;
  }
  if (CurrentCALCState == CALC3 && (CurrentFrameState->choice != -1)){
    // print preview of the newest visible line (bottom of visible window)
    String pLine = (*(CurrentFrameState->lines))[CurrentFrameState->scroll];
    if (pLine.length() > 0) {
    u8g2.setFont(u8g2_font_ncenB10_tr);
      // draw preview text on the right half of the OLED so it fits
    u8g2.drawStr(0, 24, pLine.substring(3).c_str());
    }

  } else {
    // print current line
    u8g2.setFont(u8g2_font_ncenB08_tr);
    String lineNumStr = String(count - CurrentFrameState->scroll) + "/" + String(count);
    u8g2.drawStr(0, 12, "Lines:");
    u8g2.drawStr(0, 24, lineNumStr.c_str());
  }





  // send buffer
  u8g2.sendBuffer();
}

///////////////////////////// ALGORITHMS
// FORMAT INTO RPN,EVALUATE,SAVE
int calculate(const String& cleanedInput,String &result,Unit *convA,Unit *convB){
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

    if (paren_balance != 0) {
        return {}; 
    }
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
              Serial.println("pushed ~var~ + variable!");
              outputQueue.push_back("~var~" + token);
          } else {
              Serial.println("pushed variable!");
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
        if ((token == "~neg~")) {
          if ((i+1) < tokens.size()){
            Serial.println("pushed unary negation! RPN");
            operatorStack.push(String(-1*variables[tokens[i+1]]));
          }

          i++;
          continue;
        }
        // sort operators by precedence, push all operators of greater precedence to the output queue and place new operator in operator stack
        else if (isOperatorToken(token)) {
          // Handle assignment '='
          if (token == ":") {
              // The variable should be the previous token in the original expression
              if (outputQueue.empty()) {
                return {}; 
              }
              String prev = outputQueue.back();
              if (!isVariableToken(prev)) {
                return {}; 
              }
          }
          while (!operatorStack.empty() &&
                operatorStack.top() != "(" &&
                precedenceCalc[operatorStack.top()] >= precedenceCalc[token]) {
              outputQueue.push_back(operatorStack.top());
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
    Serial.println("Tokenizing expression: " + expression);
    for (int i = 0; i < expression.length(); ++i) {
      char c = expression[i];
      Serial.println("handling character: " + String(c));   
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
              Serial.println("pushed back negative number");
              currentToken += c;
              while (i + 1 < expression.length() &&
                    (isDigit(expression[i + 1]) || expression[i + 1] == '.')) {
                  currentToken += expression[++i];
              }
              tokens.push_back(currentToken);
              currentToken = "";
              continue;
          } else {
              Serial.println("pushed back unary minus operator");
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
              Serial.println("pushing token: " + *it + " from previous expression");
              tokens.push_back(*it);
          }

          i++;
          continue;
      }
            
      // Handle numbers
      if (isDigit(c) || (c == '.' && i + 1 < expression.length() && isDigit(expression[i + 1]))) {
          Serial.println("handling" + currentToken);
          currentToken += c;
          while (i + 1 < expression.length()) {
              char peek = expression[i + 1];
              if (isDigit(peek) || peek == '.') {
                  currentToken += expression[++i];
              } else {
                  break;
              }
          }

          Serial.println("Pushing number " + String(currentToken));
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
String evaluateRPN(std::deque<String> rpnQueue,Unit *convA,Unit *convB) {
    std::stack<double> evalStack;
    std::stack<String> varStack;
    // print queue
    Serial.println("Handling evaluating RPN");
    for (auto it = rpnQueue.begin(); it != rpnQueue.end(); it++) {
      Serial.println("eval Token: " + *it);
    }
    
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
        } else if (token == "~neg~"){
            String varToNegate = rpnQueue.front(); rpnQueue.pop_front();
            rpnQueue.push_front(String(-1*variables[varToNegate]));
        } else if (isAlpha(token[0]) && !isFunctionToken(token) && token != "E") {
          if (variables.find(token) != variables.end()) {
              evalStack.push(variables[token]);
          } else {
              oledWord("undefined variable");
              delay(1000);
              return "E: undefined variable '" + token + "'";
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
            Serial.println("subtracting!");
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
            if (evalStack.size() < choices) return "E: pick not enough choices";
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
      oledWord("malformed expression");
      delay(1000);
      return "E: malformed exp";
    }
    if (varStack.size() != 0){
      String varName = varStack.top(); varStack.pop();
      double value = evalStack.top();
      variables[varName] = value;
    }
    Serial.println("exited evaluation RPN function");
    variables["ans"] = evalStack.top();

    if ((convA != nullptr) && strcmp(convA->name, convB->name) != 0){
      Serial.println("converting input!");
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
    if (currentLine == "!!"){
      dynamicScroll = 0;
      updateScroll(CurrentFrameState,prev_dynamicScroll,dynamicScroll);     
      printAnswer(prevLine,&emptyUnit, &emptyUnit);
    } 
    else if (currentLine == "/0"){
        // standard mode
        CurrentCALCState = CALC0;
        CurrentFrameState = &calcScreen;
        calcSwitchedStates = 1;
        newLineAdded = true;
    }  
    else if (currentLine == "/1"){
        // programming mode
        CurrentCALCState = CALC0;
        oledWord("Programming Mode not implemented"); 
        CurrentFrameState = &calcScreen;
        calcSwitchedStates = 1;
        newLineAdded = true;
        delay(1000);
    }
    else if (currentLine == "/2"){
        // scientific mode
        CurrentCALCState = CALC2;
        CurrentFrameState = &calcScreen;
        calcSwitchedStates = 1;
        newLineAdded = true;

    }
    else if (currentLine == "/3"){
        // conversion
        display.clearScreen();
        allLinesCalc = {};
        CurrentCALCState = CALC3;
        drawCalc();
        frames.clear();
        frames.push_back(&conversionScreen);

        conversionUnit.scroll = 9;
        frames.push_back(&conversionFrameA);
        frames.push_back(&conversionFrameB);
        frames.push_back(&conversionDirection);
        frames.push_back(&conversionUnit);
        CurrentFrameState = &conversionScreen;
        CurrentFrameState->scroll = 0;
        CurrentFrameState->prevScroll = -1;
        newLineAdded = true;
    }
    else if (currentLine == "/4"){
        // help mode
        CurrentCALCState = CALC4;
        calcSwitchedStates = 1;
    }
    else if (currentLine == "/5") {
        // write current file to text
        oledWord("Exporting Data to TXT!");
        allLines = *(CurrentFrameState->lines);
        // remvove '~R~' formatting (not used by txt app)
        for (int i = 0; i < allLines.size();i++){
          if (!(i%2 == 0)){
            String temp = allLines[i - 1] + " = " + allLines[i].substring(3);
            allLines[i - 1] =  temp;
            allLines[i] = "";
          }
        }
        delay(200);
        closeCalc(TXT);
    }
    else if (currentLine == "/6"){
      closeCalc(HOME);
    }
    else if (currentLine == "/grad"){
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
      if (CurrentCALCState == CALC3){
        if (CurrentFrameState == &conversionScreen){
          String conversionA = (*(conversionFrameA.lines))[conversionFrameA.scroll].substring(3);
          String conversionB = (*(conversionFrameB.lines))[conversionFrameB.scroll].substring(3);
          Serial.println("convA = " + conversionA + "   convB = " + conversionB);
          delay(200);
          printAnswer(currentLine,getUnit(conversionA),getUnit(conversionB));
        }
      } else {
        dynamicScroll = 0;
        updateScroll(CurrentFrameState,prev_dynamicScroll,dynamicScroll);
        prevLine = currentLine;
        printAnswer(currentLine, &emptyUnit, &emptyUnit);
      }
    }
  }
  calculatedResult = "";
  currentLine = "";
  newLineAdded = true;
}
// CONVERT NUMBER TO FLOAT STRING OR INT STRING
String formatNumber(double value) {
    //Serial.println("formating number " + String(value));
    String result;
    char buffer[32];

    if (CurrentCALCState == CALC2){
      return formatScientific(value);
    } else {
      // handle overflow
      if (value > INT_MAX) return "inf";
      if (value < INT_MIN) return "-inf";
      // handle integer test case
      if (fabs(value - round(value)) < 1e-9) {
        snprintf(buffer, sizeof(buffer), "%lld", static_cast<long long>(round(value)));
        return String(buffer);
      }
      result = trimValue(value);
    }
    return result;
}
// CONVERT DOUBLE TO SCIENTIFIC NOTATION
String formatScientific(double value) {
    // handle overflow and underflow edge cases
    if (String(value) == "inf" || String(value) == "-inf") {
      Serial.println("overflow error");
      return String(value);
    }
    if (abs(value) < 1e-150) return "0e0";

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
// TRIM EXCESS ZEROS
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
// CALC DISPLAY ANSWER
void printAnswer(String cleanExpression,Unit *convA,Unit *convB) {
  int16_t x1, y1;
  uint16_t exprWidth, resultWidth, charHeight;
  String resultOutput = "";
  int maxWidth = display.width() - 40;
  int result = calculate(cleanExpression, resultOutput,convA,convB);

  // Set font before measuring text
  display.setFont(currentFont);
  if (CurrentCALCState == CALC3){
    cleanExpression = cleanExpression + " " + convA->symbol;
    resultOutput = resultOutput + " " + convB->symbol;
  }
  // Measure widths
  display.getTextBounds(cleanExpression, 0, 0, &x1, &y1, &exprWidth, &charHeight);
  display.getTextBounds(resultOutput, 0, 0, &x1, &y1, &resultWidth, &charHeight);

  // Clip long expressions
  if (exprWidth > maxWidth) {
    int mid = cleanExpression.length() / 2;
    CurrentFrameState->lines->push_back(cleanExpression.substring(0, mid));
    CurrentFrameState->lines->push_back(cleanExpression.substring(mid));
    newLineAdded = true;
  } else {
    CurrentFrameState->lines->push_back(cleanExpression);
  }
  CurrentFrameState->lines->push_back("~R~" + resultOutput); // right-align marker 
}

///////////////////////////// HELPER FUNCTIONS
// CONFRIM TOKEN IS A NUMBER
bool isNumberToken(const String& token) {
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
// CONFRIM TOKEN IS A FUNCTION
bool isFunctionToken(const String& token) {
  if (token.isEmpty()) return false;
  return functionsCalc.count(token) > 0;
}
// CONFIRM A TOKEN IS A VARIABLE INCLUDING CONSTANTS
bool isVariableToken(const String& token) {
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
// COMPARE TO SET OF OPERATORS
bool isOperatorToken(const String& token) {
    if (token.isEmpty()) return false;
    return operatorsCalc.count(token) > 0;
}
// COMPARE TO SET OF CONSTANTS
bool isConstantToken(const String& token) {
      if (token.isEmpty()) return false;
    return constantsCalc.count(token) > 0;
}
// CONVERT TRIG INPUTS
double convertTrig(double input,int trigType,bool reverse){
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

double convert(double value, Unit* from, Unit* to) {
    // convert to 
    double inBase = (value + from->offset) * from->factor;
    return (inBase / to->factor) - to->offset;
}
// UPDATE CURRENT FRAME SCROLL
void updateScroll(Frame *currentFrameState,int prevScroll,int currentScroll, bool reset){
  int scroll, prev;
  if (reset){
    currentFrameState->scroll = currentFrameState->lines->size()  - 11;
    currentFrameState->prevScroll = -1;

  } else {
    currentFrameState->scroll = currentScroll;
    currentFrameState->prevScroll = prevScroll;
  }
  return;
}
// SEARCH UNITS FOR UNIT WITH MATCHING SYMBOL
Unit* getUnit(const String& key) {
    for (size_t i = 0; i < lengthUnits.size(); ++i) {
        Unit& u = (Unit&)lengthUnits[i];
        String trimmedKey = key;
        trimmedKey.trim();
        if (trimmedKey.equals(u.symbol)){
          Serial.println("match!");
          return &u;
        }
    }
    return nullptr;
}