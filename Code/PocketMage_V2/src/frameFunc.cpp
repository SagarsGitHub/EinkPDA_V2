//  88888888b  888888ba   .d888888  8888ba.88ba   88888888b .d88888b      //
//  88         88    `8b d8'    88  88  `8b  `8b  88        88.    "'     //
// a88aaaa    a88aaaa8P' 88aaaaa88a 88   88   88 a88aaaa    `Y88888b.     //
//  88         88   `8b. 88     88  88   88   88  88              `8b     //
//  88         88     88 88     88  88   88   88  88        d8'   .8P     //
//  dP         dP     dP 88     88  dP   dP   dP  88888888P  Y88888P      //

#include "globals.h"
//
void einkTextFrameDynamic(Frame &frame, bool doFull_, bool noRefresh, bool drawBox) {
    const int maxTextWidth = display.width() - frame.left - frame.right;
    setTXTFont(currentFont);
    // Wrap and format text (could be cached in Frame if needed)
    std::vector<String> wrappedLines = formatText(frame, maxTextWidth);

    // Calculate visible range
    long size = wrappedLines.size();
    frame.maxLines = (display.height() - frame.top - frame.bottom - 8) / (fontHeight + lineSpacing);
    long startLine, endLine;
    getVisibleRange(&frame, size, startLine, endLine);

    // Precompute common variables
    int usableX = frame.left;
    int usableWidth = display.width() - frame.left - frame.right;
    int usableY = alignDown8(frame.top);

    if (doFull_) {
        // Full refresh — clear whole area once
        int usableHeight = alignUp8(display.height() - frame.top - frame.bottom);
        display.setPartialWindow(usableX, usableY, usableWidth, usableHeight);
        display.firstPage();
        do {
            display.fillRect(usableX, usableY, usableWidth, usableHeight, GxEPD_WHITE);
            if (drawBox) drawFrameBox(usableX, usableY, usableWidth, usableHeight);

            for (int i = startLine; i < endLine; i++) {
                drawLineInFrame(wrappedLines[i], i - startLine, frame, usableY, false, false);
            }
        } while (display.nextPage());
    } 
    else {
        // Partial refresh — only clear/draw per line
        int heightAligned = alignUp8((fontHeight + lineSpacing) * (endLine - startLine));
        display.setPartialWindow(usableX, usableY, usableWidth, heightAligned);
        display.firstPage();
        do {
            for (int i = startLine; i < endLine; i++) {
                drawLineInFrame(wrappedLines[i], i - startLine, frame, usableY, true, true);
            }
        } while (display.nextPage());
    }
}

// FORMAT TEXT BASED ON FRAME'S DIMENSIONS
std::vector<String> formatText(Frame &frame,int maxTextWidth){
  std::vector<String> wrappedLines = {};
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
    bool centerAlign = originalLine.startsWith("~C~");
    // clip right align marker
    String line = (rightAlign || centerAlign) ? originalLine.substring(3) : originalLine; 
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
            if (rightAlign){
              wrappedLines.push_back("~R~" + currentLine);
            } else if (centerAlign){
              wrappedLines.push_back("~C~" + currentLine);
            } else {
              wrappedLines.push_back(currentLine);
            }
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
            if (rightAlign){
              wrappedLines.push_back("~R~" + currentWord.substring(0, splitPos));
            } else if (centerAlign){
              wrappedLines.push_back("~C~" + currentWord.substring(0, splitPos));
            } else {
              wrappedLines.push_back(currentWord.substring(0, splitPos));
            }
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
      if (rightAlign){
        wrappedLines.push_back("~R~" + currentLine);
      } else if (centerAlign){
        wrappedLines.push_back("~C~" + currentLine);
      } else {
        wrappedLines.push_back(currentLine);
      }
    }
  }

  return wrappedLines;

}

// DRAW SINGLE LINE IN FRAME
void drawLineInFrame(const String &srcLine, int lineIndex, const Frame &frame,
                     int usableY, bool clearLine, bool isPartial) {
    if (srcLine.length() == 0) return;

    String line = srcLine;
    bool rightAlign  = line.startsWith("~R~");
    bool centerAlign = line.startsWith("~C~");
    if (rightAlign || centerAlign) line.remove(0, 3);

    int16_t x1, y1;
    uint16_t lineWidth, lineHeight;
    display.getTextBounds(line, 0, 0, &x1, &y1, &lineWidth, &lineHeight);

    int cursorX = computeCursorX(frame, rightAlign, centerAlign, x1, lineWidth);

    // Y position - modified calculation
    int yRaw = frame.top + lineIndex * (fontHeight + lineSpacing);
    int yDraw = yRaw + fontHeight - y1; // Accounts for text baseline and descenders

    if (clearLine) {
        int yClear = alignDown8(yRaw);
        int clearHeight = alignUp8(fontHeight + lineSpacing + abs(y1)); // Ensure full text height is cleared
        display.fillRect(frame.left, yClear,
                         display.width() - frame.left - frame.right,
                         clearHeight,
                         GxEPD_WHITE);
    }

    display.setCursor(cursorX, yDraw);
    display.print(line);
}

// DRAW BOX AROUND FRAME
void drawFrameBox(int usableX, int usableY, int usableWidth, int usableHeight) {
    display.drawFastHLine(usableX, usableY, usableWidth, GxEPD_BLACK); // Top
    display.drawFastHLine(usableX, usableY + usableHeight - 1, usableWidth, GxEPD_BLACK); // Bottom
    display.drawFastVLine(usableX, usableY, usableHeight, GxEPD_BLACK); // Left
    display.drawFastVLine(usableX + usableWidth - 1, usableY, usableHeight, GxEPD_BLACK); // Right
}


// FIND START AND END LINES OF FRAME
void getVisibleRange(const Frame *f, long totalLines, long &startLine, long &endLine) {
    if (totalLines <= 0) {
        startLine = endLine = 0;
        return;
    }

    long displayLines = constrain((long)f->maxLines, 0L, totalLines);
    long scrollOffset = constrain((long)f->scroll, 0L, max(0L, totalLines - displayLines));

    // bottom scroll = 0
    startLine = max(0L, totalLines - displayLines - scrollOffset);
    endLine   = min(totalLines, startLine + displayLines);
}

// COMPUTE X POS IN FRAME
int computeCursorX(const Frame &frame, bool rightAlign, bool centerAlign, int16_t x1, uint16_t lineWidth) {
    const int paddingFix = 16;
    int usableWidth = display.width() - frame.left - frame.right;
    int base;

    if (rightAlign) {
        base = frame.left + usableWidth - lineWidth - paddingFix;
    } 
    else if (centerAlign) {
        base = frame.left + (usableWidth - lineWidth) / 2 - paddingFix / 2;
    } 
    else {
        base = frame.left;
    }

    // base - left margin + offset
    return base - x1 + X_OFFSET;
}

int alignDown8(int v) { return v - (v % 8); }
int alignUp8(int v)   { return (v % 8) ? v + (8 - (v % 8)) : v; }
