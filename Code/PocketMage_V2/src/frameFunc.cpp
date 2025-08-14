//  88888888b  888888ba   .d888888  8888ba.88ba   88888888b .d88888b      //
//  88         88    `8b d8'    88  88  `8b  `8b  88        88.    "'     //
// a88aaaa    a88aaaa8P' 88aaaaa88a 88   88   88 a88aaaa    `Y88888b.     //
//  88         88   `8b. 88     88  88   88   88  88              `8b     //
//  88         88     88 88     88  88   88   88  88        d8'   .8P     //
//  dP         dP     dP 88     88  dP   dP   dP  88888888P  Y88888P      //

#include "globals.h"
///////////////////////////// DRAWING FUNCTIONS
// DRAW ALL FRAMES STORED WITHIN TOTAL FRAME BOUNDING BOX
void einkTextFramesDynamic(std::vector<Frame*> &frames, bool doFull_, bool noRefresh) {
    if (frames.empty()) return;

    // calculate union bounding box of all frames
    int minX = INT_MAX;
    int minY = INT_MAX;
    int maxX = 0;
    int maxY = 0;
    for (auto frame : frames) {
        minX = std::min(minX, frame->left);
        minY = std::min(minY, frame->top);
        int frameRight = display.width() - frame->right;
        int frameBottom = display.height() - frame->bottom;
        maxX = std::max(maxX, frameRight);
        maxY = std::max(maxY, frameBottom);
    }
    minY = alignDown8(minY);
    int usableWidth = maxX - minX;
    int usableHeight = alignUp8(maxY - minY);

    // set partial window once
    display.setPartialWindow(minX, minY, usableWidth, usableHeight);

    display.firstPage();
    do {
        if (doFull_) {
            Serial.println("clearing rectangle size of current frame \n" );
            display.fillRect(minX, minY, usableWidth, usableHeight, GxEPD_WHITE);
        }
        // draw all frames relative to union bounding box
        for (auto frame : frames) {
            const int maxTextWidth = display.width() - frame->left - frame->right;
            setTXTFont(currentFont);

            // format wrapped lines for frame
            std::vector<String> wrappedLines = formatText(*frame, maxTextWidth);

            long size = wrappedLines.size();
            frame->maxLines = (display.height() - frame->top - frame->bottom - 8) / (fontHeight + lineSpacing);
            long startLine, endLine;
            getVisibleRange(frame, size, startLine, endLine);

            if (frame->box) {
                // draw box adjusted for union window origin

                int boxX = frame->left;
                int boxY = frame->top;
                int boxWidth = display.width() - frame->left - frame->right;
                int boxHeight = display.height() - frame->top - frame->bottom;
                display.fillRect(boxX, boxY, boxWidth, boxHeight, GxEPD_WHITE);
                drawFrameBox(boxX, boxY, boxWidth, boxHeight);
            }

            // calculate offset relative to union bounding box
            int offsetY = frame->top - minY;
            // draw lines for frame
            if (frame->choice != -1 && (endLine - startLine == 1)){
                drawLineInFrame(wrappedLines[frame->choice], 0, *frame, offsetY, false, !doFull_);
            } else {
                for (int i = startLine; i < endLine; i++) {

                // draw line with clearLine = false (avoid clearing overlapping lines) and isPartial = !doFull_
                drawLineInFrame(wrappedLines[i], i - startLine, *frame, offsetY, false, !doFull_);

            }

            }


        }
    } while (display.nextPage());
}
// DRAW BOX AROUND FRAME
void drawFrameBox(int usableX, int usableY, int usableWidth, int usableHeight) {
    // draw box around frame within the partial window
    display.drawFastHLine(usableX, usableY, usableWidth, GxEPD_BLACK); // Top
    display.drawFastHLine(usableX, usableY + usableHeight - 1, usableWidth, GxEPD_BLACK); // Bottom
    display.drawFastVLine(usableX, usableY, usableHeight, GxEPD_BLACK); // Left
    display.drawFastVLine(usableX + usableWidth - 1, usableY, usableHeight, GxEPD_BLACK); // Right
}
// DRAW SINGLE LINE IN FRAME
void drawLineInFrame(String &srcLine, int lineIndex, Frame &frame, int usableY, bool clearLine, bool isPartial) {
    if (srcLine.length() == 0) return;
    // get alignment and remove alignment marker
    String line = srcLine;
    bool rightAlign  = line.startsWith("~R~");
    bool centerAlign = line.startsWith("~C~");
    if (rightAlign || centerAlign) line.remove(0, 3);

    int16_t x1, y1;
    uint16_t lineWidth, lineHeight;
    display.getTextBounds(line, 0, 0, &x1, &y1, &lineWidth, &lineHeight);

    int cursorX = computeCursorX(frame, rightAlign, centerAlign, x1, lineWidth);
    // set yRaw to frame top + spaces taken by all previous lines
    int yRaw = frame.top + lineIndex * (fontHeight + lineSpacing);
    // set the cursor y so that the top of the font does not get cut off by the top of the frame
    int yDraw = yRaw + fontHeight - y1; 
    // if clear line, clear box the size of the frame at the current line
    if (clearLine) {
        int yClear = alignDown8(yRaw);
        int clearHeight = alignUp8(fontHeight + lineSpacing + abs(y1));
        display.fillRect(frame.left, yClear,
                         display.width() - frame.left - frame.right,
                         clearHeight,
                         GxEPD_WHITE);
    }
    display.setCursor(cursorX, yDraw);
    if (lineIndex == frame.choice &&  CurrentFrameState == &frame){
        display.print("- " +line);
    } else {
        display.print(line);
    }
    

}

///////////////////////////// TEXT POSITION FUNCTIONS
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
    // clip align marker
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

      // check word boundaries (space or end of string)
      // if you have the end of the word/line
      if (c == ' ' || i == line.length() - 1) {
        // create a test line with the line + word
        String testLine = currentLine + currentWord;
        // check boundaries
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

          // handle very long words 
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
    // push line to wrapped lines
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
// FIND START AND END LINES OF FRAME
void getVisibleRange(Frame *f, long totalLines, long &startLine, long &endLine) {
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
int computeCursorX(Frame &frame, bool rightAlign, bool centerAlign, int16_t x1, uint16_t lineWidth) {
    const int padding = 16;
    int usableWidth = display.width() - frame.left - frame.right;
    int base;
    // draw lines with alignment
    if (rightAlign) {
        base = frame.left + usableWidth - lineWidth - padding;
    } 
    else if (centerAlign) {
        base = frame.left + (usableWidth - lineWidth) / 2 - padding / 2;
    } 
    else {
        base = frame.left;
    }
    // base - left margin + offset
    return base - x1 + X_OFFSET;
}

///////////////////////////// HELPER FUNCTIONS
// align numbers to be divisible by 8, useful for partial windows (y & h must be divisible by 8 with GxEPD2 with rotation == 3)
int alignDown8(int v) { return v - (v % 8); }
int alignUp8(int v)   { return (v % 8) ? v + (8 - (v % 8)) : v; }
