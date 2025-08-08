//  oooooooooooo         ooooo ooooo      ooo oooo    oooo  //
//  `888'     `8         `888' `888b.     `8' `888   .8P'   //
//   888                  888   8 `88b.    8   888  d8'     //
//   888oooo8    8888888  888   8   `88b.  8   88888[       //
//   888    "             888   8     `88b.8   888`88b.     //
//   888       o          888   8       `888   888  `88b.   //
//  o888ooooood8         o888o o8o        `8  o888o  o888o  //
#include "globals.h"

void refresh() {
  // USE A SLOW FULL UPDATE EVERY N FAST UPDATES OR WHEN SPECIFIED
  if ((partialCounter >= FULL_REFRESH_AFTER) || forceSlowFullUpdate) {
    forceSlowFullUpdate = false;
    partialCounter = 0;
    setFastFullRefresh(false);
  }
  // OTHERWISE USE A FAST FULL UPDATE
  else {
    setFastFullRefresh(true);
    partialCounter++;
  }

  display.display(false);

  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  display.hibernate();
}

void setFastFullRefresh(bool setting) {
  GxEPD2_310_GDEQ031T10::useFastFullUpdate = setting;
  /*if (GxEPD2_310_GDEQ031T10::useFastFullUpdate != setting) {
    GxEPD2_310_GDEQ031T10::useFastFullUpdate = setting;
  }*/
}

void einkHandler(void* parameter) {
  delay(250);
  /*display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  display.display(false);
  display.hibernate();*/

  while (true) {
    applicationEinkHandler();

    vTaskDelay(50 / portTICK_PERIOD_MS);
    yield();
  }
}

void statusBar(String input, bool fullWindow) {
  display.setFont(&FreeMonoBold9pt7b);
  if (!fullWindow) display.setPartialWindow(0, display.height() - 20, display.width(), 20);
  display.fillRect(0, display.height() - 26, display.width(), 26, GxEPD_WHITE);
  display.drawRect(0, display.height() - 20, display.width(), 20, GxEPD_BLACK);
  display.setCursor(4, display.height() - 6);
  display.print(input);

  /*switch (CurrentKBState) {
    case NORMAL:
      //Display battery level
      display.drawBitmap(display.width()-30,display.height()-20, KBStatusallArray[6], 30, 20, GxEPD_BLACK);
      break;
    case SHIFT:
      display.drawBitmap(display.width()-30,display.height()-20, KBStatusallArray[0], 30, 20, GxEPD_BLACK);
      break;
    case FUNC:
      display.drawBitmap(display.width()-30,display.height()-20, KBStatusallArray[1], 30, 20, GxEPD_BLACK);
      break;
  }*/
  display.drawRect(display.width() - 30, display.height() - 20, 30, 20, GxEPD_BLACK);
}

void drawStatusBar(String input) {
  display.fillRect(0, display.height() - 26, display.width(), 26, GxEPD_WHITE);
  display.drawRect(0, display.height() - 20, display.width(), 20, GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);
  display.setCursor(4, display.height() - 6);
  display.print(input);

  // BATTERY INDICATOR
  //display.drawBitmap(display.width() - 30, display.height() - 20, KBStatusallArray[battState], 30, 20, GxEPD_BLACK);
  //display.drawRect(display.width() - 30, display.height() - 20, 30, 20, GxEPD_BLACK);
}

uint8_t getMaxCharsPerLine() {
  int16_t x1, y1;
  uint16_t charWidth, charHeight;
  // GET AVERAGE CHAR WIDTH
  display.getTextBounds("abcdefghijklmnopqrstuvwxyz", 0, 0, &x1, &y1, &charWidth, &charHeight);
  charWidth = charWidth / 52;

  return (display.width() / (charWidth));
}

uint8_t getMaxLines() {
  int16_t x1, y1;
  uint16_t charWidth, charHeight;
  // GET MAX CHAR HEIGHT
  display.getTextBounds("H", 0, 0, &x1, &y1, &charWidth, &charHeight);
  fontHeight = charHeight;

  return ((display.height() - 26) / (charHeight + lineSpacing));
}

void setTXTFont(const GFXfont* font) {
  // SET THE FONT
  display.setFont(font);
  currentFont = (GFXfont*)font;

  // UPDATE maxCharsPerLine & maxLines
  maxCharsPerLine = getMaxCharsPerLine();
  maxLines = getMaxLines();
}

void drawThickLine(int x0, int y0, int x1, int y1, int thickness) {
  float dx = x1 - x0;
  float dy = y1 - y0;
  float length = sqrt(dx * dx + dy * dy);
  float stepX = dx / length;
  float stepY = dy / length;

  for (float i = 0; i <= length; i += thickness / 2.0) {
    int cx = round(x0 + i * stepX);
    int cy = round(y0 + i * stepY);
    display.fillCircle(cx, cy, thickness / 2, GxEPD_BLACK);
  }
}

void einkTextPartial(String text, bool noRefresh) {
  bool doFullRefresh = false;

  einkRefresh++;
  if (einkRefresh > FULL_REFRESH_AFTER) {
    doFullRefresh = true;
    einkRefresh = 0;
    display.setFullWindow();
    display.fillScreen(GxEPD_WHITE);
  }

  display.setFont(&FreeMonoBold9pt7b);

  if (splitIntoLines(text.c_str(), scroll)) doFullRefresh = true;

  for (int i = 0; i < 13; i++) {
    if (outLines[i] != "") {  // Print only non-empty lines
      if (doFullRefresh) {
        display.fillRect(0, 16 * i, display.width(), 16, GxEPD_WHITE);
        display.setCursor(0, 10 + (16 * i));
        display.print(outLines[i]);
      } else if (outLines[i] != lines_prev[i]) {  //If the line has changed
        display.setPartialWindow(0, 16 * i, display.width(), 16);
        display.fillRect(0, 16 * i, display.width(), 16, GxEPD_WHITE);
        display.setCursor(0, 10 + (16 * i));
        display.print(outLines[i]);
        if (!noRefresh) refresh();
      }
    }
  }

  if (doFullRefresh && !noRefresh) {
    display.nextPage();
    display.hibernate();
  }

  for (int i = 0; i < 13; i++) {
    lines_prev[i] = outLines[i];  // Copy each line
  }
}

void einkTextDynamic(bool doFull_, bool noRefresh) {
  // SET FONT
  setTXTFont(currentFont);

  // ITERATE AND DISPLAY
  uint8_t size = allLines.size();
  uint8_t displayLines = maxLines;

  if (displayLines > size) displayLines = size;  // PREVENT OUT OF BOUNDS

  // Apply dynamic scroll offset (make sure it's within the bounds)
  int scrollOffset = dynamicScroll;
  if (scrollOffset < 0) scrollOffset = 0;
  if (scrollOffset > size - displayLines) scrollOffset = size - displayLines;

  // FULL REFRESH OPERATION
  if (doFull_) {
    for (uint8_t i = size - displayLines - scrollOffset; i < size - scrollOffset; i++) {
      if ((allLines[i]).length() > 0) {
        display.setFullWindow();
        display.fillRect(0, (fontHeight + lineSpacing) * (i - (size - displayLines - scrollOffset)), display.width(), (fontHeight + lineSpacing), GxEPD_WHITE);
        display.setCursor(0, fontHeight + ((fontHeight + lineSpacing) * (i - (size - displayLines - scrollOffset))));
        display.print(allLines[i]);
        Serial.println(allLines[i]);
      }
    }
  }
  // PARTIAL REFRESH, ONLY SEND LAST LINE
  else {
    if ((allLines[size - displayLines - scrollOffset]).length() > 0) {
      display.setPartialWindow(0, (fontHeight + lineSpacing) * (size - displayLines - scrollOffset), display.width(), (fontHeight + lineSpacing));
      display.fillRect(0, (fontHeight + lineSpacing) * (size - displayLines - scrollOffset), display.width(), (fontHeight + lineSpacing), GxEPD_WHITE);
      display.setCursor(0, fontHeight + ((fontHeight + lineSpacing) * (size - displayLines - scrollOffset)));
      display.print(allLines[size - displayLines - scrollOffset]);
    }
  }

  drawStatusBar("L:" + String(allLines.size()) + " " + editingFile);
}

int countLines(String input, size_t maxLineLength) {
  size_t inputLength = input.length();
  uint8_t charCounter = 0;
  uint16_t lineCounter = 1;

  for (size_t c = 0; c < inputLength; c++) {
    if (input[c] == '\n') {
      charCounter = 0;
      lineCounter++;
      continue;
    } else if (charCounter > (maxLineLength - 1)) {
      charCounter = 0;
      lineCounter++;
    }
    charCounter++;
  }

  return lineCounter;
}

///////////////////////////// CALC EINK FUNCTIONS
// CALC FRAME
void drawCalc(){
  // SET FONT
  setTXTFont(currentFont);
  display.setFullWindow();
  display.firstPage();
  do {
    // print status bar
    if (CurrentCALCState == CALC4){
      drawStatusBar("<- -> scroll | enter -> EXIT");
    } else {
      drawStatusBar("\"/4\" -> info | \"/6\" -> EXIT");
    }
    // draw calc bitmap
    display.drawBitmap(0, 0, calcAllArray[0], 320, 218, GxEPD_BLACK);
    // print current calc mode
    display.setCursor(25, 20);
    switch (CurrentCALCState) {
      case CALC0:      
        //standard mode
        display.print("Calc: Standard");
        break;
      case CALC1:
        //programming mode
        display.print("Calc: Programming");
        break;
      case CALC2:
        //scientific mode
        Serial.println("printing Calc: Scientific!");
        display.print("Calc: Scientific");
        break;
      case CALC3:
        //conversions
        display.print("Calc: Conversions");
        break;
      case CALC4:
        //help mode
        display.print("Calc: Help");
        break;  
    }
    // print current trig mode
    display.setCursor(240, 20);
    switch (trigType){
      // 0 = degree mode
      case (0):
        display.print("deg");
      break;
      // 1 = radian mode
      case (1):
        display.print("rad");
      break;
      // 2 = grad mode
      case (2):
        display.print("grad");
      break;
    }
  } while (display.nextPage());
  
}

// CALC EINK TEXT 
// partial refresh that doesn't write over text outside of the defined frame
void einkTextFrameDynamic(Frame &frame,bool doFull_, bool noRefresh, bool drawBox) {
  // calculate max text width
  const int maxTextWidth = display.width() - frame.left - frame.right - 8;
  // set font
  setTXTFont(currentFont);
  std::vector<String> wrappedLines;

  // for each line in all lines
  for (const String& originalLine : (*frame.lines)) {
    // empty line handling
    if (originalLine.length() == 0) {
      wrappedLines.push_back("");
      continue;
    }      

    // Check if line needs wrapping
    String currentLine = "";
    String currentWord = "";
    int16_t x1, y1;
    uint16_t lineWidth, lineHeight;

    // cut original marker to add it to cut partial text
    bool rightAlign = originalLine.startsWith("~R~");
    // clip right align marker
    String line = rightAlign ? originalLine.substring(3) : originalLine; 
    display.getTextBounds(line, 0, 0, &x1, &y1, &lineWidth, &lineHeight);

    // simple line smaller than screen size
    if (lineWidth <= maxTextWidth) {
      wrappedLines.push_back(originalLine);
      continue;
    }
    
    // for each character in the line
    for (int i = 0; i < line.length(); i++) {

      char c = line[i];
      // update running word
      currentWord += c;

      // Check word boundaries (space or end of string)
      // if you have the end of the word/line
      if (c == ' ' || i == line.length() - 1) {
        // create a test line with the line + word
        String testLine = currentLine + currentWord;
        // check boundries
        display.getTextBounds(testLine, 0, 0, &x1, &y1, &lineWidth, &lineHeight);

        // if the test linewidth overruns the frame, push the line we are building, adding right align if needed (consider adding center align check)
        if (lineWidth > maxTextWidth) {
          if (currentLine.length() > 0) {
            wrappedLines.push_back(rightAlign ? "~R~" + currentLine : currentLine);
            currentLine = "";
          }

          // Handle very long words 
          while (currentWord.length() > 0) {
            // set initial split position
            int splitPos = 1;
            String testFragment = "";
            
            // keep adding current word to test fragment until the fragment reaches the max text width
            while (splitPos < currentWord.length()) {
              testFragment += currentWord[splitPos];
              display.getTextBounds(testFragment, 0, 0, &x1, &y1, &lineWidth, &lineHeight);
              if (lineWidth > maxTextWidth) break;
              splitPos++;
            }
            // push split line onto wrapped lines, adding right align if needed
            wrappedLines.push_back(rightAlign ? "~R~" + currentWord.substring(0, splitPos) : currentWord.substring(0, splitPos));
            currentWord = currentWord.substring(splitPos);
          }
        } else {
          // line + word is less than or equal to the max text width
          currentLine = testLine;
        }
        currentWord = "";
      }
    }
    // puah line to wrapped lines
    if (currentLine.length() > 0) {
      wrappedLines.push_back(rightAlign ? "~R~" + currentLine : currentLine);
    }
  }

  // wrap lines
  uint16_t size = wrappedLines.size();
  frame.maxLines = (display.height() - frame.top - frame.bottom) / (fontHeight + lineSpacing);
  Serial.println("max lines = " + String(maxLines));
  Serial.println("current size = " + String(size));
  // determine the amount of lines to display
  uint16_t displayLines = constrain(maxLines + 2, 0, size);
  Serial.println("current display lines = " + String(displayLines));
  // set scroll offset + start and end + total height based on font
  int scrollOffset = constrain(frame.scroll, 0, size - displayLines);
  Serial.println("current scrollOffset = " + String(scrollOffset));
  delay(100);
  int startLine = size - displayLines - scrollOffset;
  Serial.println("current startLine = " + String(startLine));
  int endLine = size - scrollOffset;
  int totalTextHeight = (fontHeight + lineSpacing) * displayLines;
  // for current GxEPD2 screen rotation, h and y need to be divisible by 8
  totalTextHeight -= totalTextHeight % 8;
  
  if (doFull_) {
    // set  partial window size of frame
    // Snap frame top and height to multiples of 8 (rounding OUT)
    int usableX      = frame.left;
    int usableY      = frame.top;
    usableY -= usableY % 8;
    int usableWidth  = display.width()  - frame.left - frame.right;
    int usableHeight = display.height() - frame.top - frame.bottom;
    if (usableHeight % 8 != 0) {
      usableHeight += (8 - (usableHeight % 8)); // round up, not down
    }
    display.setPartialWindow(usableX, usableY, usableWidth, usableHeight);
  
    display.firstPage();
    do {
      
      // fill frame
      display.fillRect(usableX, usableY, usableWidth, usableHeight, GxEPD_WHITE);
      
      if (drawBox){
        // Top border
        display.drawFastHLine(usableX, usableY, usableWidth, GxEPD_BLACK);

        // Bottom border
        display.drawFastHLine(usableX, usableY + usableHeight - 1, usableWidth, GxEPD_BLACK);

        // Left border
        display.drawFastVLine(usableX, usableY, usableHeight, GxEPD_BLACK);

        // Right border
        display.drawFastVLine(usableX + usableWidth - 1, usableY, usableHeight, GxEPD_BLACK);  
      }
      for (int i = startLine; i < endLine; i++) {
        //nothing in line edge case
        if (wrappedLines[i].length() == 0) continue;

        // for current GxEPD2 screen rotation, h and y need to be divisible by 8
        int lineIndex = i - startLine;
        int y = frame.top + (fontHeight + lineSpacing) * lineIndex;
        String line = wrappedLines[i];
        bool rightAlign = line.startsWith("~R~");
        if (rightAlign) line.remove(0, 3);
        int16_t x1, y1;
        uint16_t lineWidth, lineHeight;
        display.getTextBounds(line, 0, 0, &x1, &y1, &lineWidth, &lineHeight);
        const int paddingFix = 15;
        int xPos = rightAlign ? (display.width() - frame.right - lineWidth - paddingFix) : frame.left;
        display.setCursor(xPos + 8, y + fontHeight + 8);
        display.print(line);
      }
    } while (display.nextPage());
  } else {
    // set line index to the start based on scroll
    int i = startLine;
            // Top border

    if (i >= 0 && frame.lines->at(i).length() > 0) {
      // for current GxEPD2 screen rotation, h and y need to be divisible by 8 
      int h = fontHeight + lineSpacing;
      h -= h % 8;
      int y = frame.top;
      y -= y % 8;
      // set partial window to the size of the frame
      display.setPartialWindow(frame.left, y, maxTextWidth, h);
      display.firstPage();

      do {
        // fill with white and add all lines
        display.fillRect(frame.left, y, maxTextWidth, h, GxEPD_WHITE);



        String line = frame.lines->at(i);
        bool rightAlign = line.startsWith("~R~");
        if (rightAlign) line.remove(0, 3);  // Strip "~R~"
        int16_t x1, y1;
        uint16_t lineWidth, lineHeight;
        display.getTextBounds(line, 0, 0, &x1, &y1, &lineWidth, &lineHeight);
        // right justify logic, should add center justify logic as well
        int xPos = rightAlign ? (display.width() - frame.right - lineWidth) : frame.left;
        display.setCursor(xPos, y + fontHeight);
        display.print(line);

      } while (display.nextPage());
    }
  }
}
