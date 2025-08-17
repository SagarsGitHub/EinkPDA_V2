//  88888888b  888888ba   .d888888  8888ba.88ba   88888888b .d88888b      //
//  88         88    `8b d8'    88  88  `8b  `8b  88        88.    "'     //
// a88aaaa    a88aaaa8P' 88aaaaa88a 88   88   88 a88aaaa    `Y88888b.     //
//  88         88   `8b. 88     88  88   88   88  88              `8b     //
//  88         88     88 88     88  88   88   88  88        d8'   .8P     //
//  dP         dP     dP 88     88  dP   dP   dP  88888888P  Y88888P      //

#include "globals.h"

// !! Commented for code-review
// to-do 
// ensure max memory efficiency
// einkBitmapFrameDynamic?

///////////////////////////// HELPER FUNCTIONS
// align numbers to be divisible by 8, useful for partial windows (y & h must be divisible by 8 with GxEPD2 with rotation == 3)
int alignDown8(int v) { return v - (v % 8); }
int alignUp8(int v)   { return (v % 8) ? v + (8 - (v % 8)) : v; }

static size_t sliceThatFits(const char* s, size_t n, int maxTextWidth) {
  if (!s || n == 0) return 0;

  int16_t x1, y1; uint16_t w, h;
  static char buf[256]; 
  const size_t cap = sizeof(buf) - 1;

  size_t best = 0, lastSpace = SIZE_MAX;
  size_t i = 0;
  size_t len = 0;

  while (i < n && len < cap) {
    char c = s[i];

    // newline: either end before it, or consume 1 char if it's first
    if (c == '\n' || c == '\r') return (best > 0) ? best : 1;

    if (c == ' ') lastSpace = i;

    buf[len++] = c;
    buf[len] = '\0';

    display.getTextBounds(buf, 0, 0, &x1, &y1, &w, &h);
    if ((int)w > maxTextWidth) break;

    best = i + 1;
    ++i;
  }

  const bool overflowed = (i < n) || (len >= cap);
  if (best == 0) return (n ? 1 : 0);

  if (overflowed && lastSpace != SIZE_MAX && lastSpace + 1 <= best) {
    return lastSpace + 1;
  }
  return best;
}

static inline long totalLines(const Frame& f) {
  return f.source ? (long)f.source->size() : 0L;
}

static inline long maxFirstVisible(const Frame& f) {
  long tl = totalLines(f);
  long m  = (long)f.maxLines;
  if (m <= 0) return 0;
  return tl > m ? (tl - m) : 0L;
}

static inline void clampScroll(Frame& f) {
  long maxStart = maxFirstVisible(f);
  if (f.scroll < 0)       f.scroll = 0;
  if (f.scroll > maxStart) f.scroll = maxStart;
}

static inline void visibleRange(const Frame& f, long& start, long& end) {
  // requires f.maxLines to be set for the current font/area
  long tl = totalLines(f);
  if (tl <= 0) { start = end = 0; return; }
  start = f.scroll;
  if (start < 0) start = 0;
  long ml = (long)f.maxLines;
  if (ml <= 0) { end = start; return; }
  end = start + ml;
  if (end > tl) end = tl;
}

static inline long maxScroll(const Frame& f) {
  long tl = totalLines(f);
  long ml = (long)f.maxLines;
  if (ml <= 0) return 0;
  return (tl > ml) ? (tl - ml) : 0; 
}

static inline size_t trimCRLF(const char* s, size_t n) {
  while (n && (s[n-1] == '\n' || s[n-1] == '\r')) --n;
  return n;
}

static inline long bottomIdx(const Frame& f) {
  long tl = totalLines(f);
  return (tl > 0) ? (tl - 1 - f.scroll) : -1;
}

static inline void ensureChoiceVisible(Frame& f) {
  long T = f.source ? (long)f.source->size() : 0L;
  int  D = max(1, f.maxLines);
  if (T == 0 || f.choice < 0 || f.choice >= T) return;

  // choose a start index so that choice is within [start, start + D)
  long startWanted = f.choice - (D - 1); 
  if (startWanted < 0) startWanted = 0;
  long maxStart = (T > D) ? (T - D) : 0;
  if (startWanted > maxStart) startWanted = maxStart;
  f.scroll = T - D - startWanted;
}

inline void resetScroll(Frame& f) {
  long tl = (long)(f.source ? f.source->size() : 0);
  long ml = (long)f.maxLines;
  f.scroll = tl > ml ? (tl - ml) : 0;
  f.prevScroll = -1;
}
// UPDATE CURRENT FRAME SCROLL
inline void updateScroll(Frame *currentFrameState,int prevScroll,int currentScroll, bool reset){
  int scroll, prev;
  if (reset){
    resetScroll(*currentFrameState);
  } else {
    currentFrameState->scroll = currentScroll;
    currentFrameState->prevScroll = prevScroll;
  }
  return;
}

// GET CLEANED STRING FROM FRAME CHOICE
String frameChoiceString(const Frame& f) {
  LineView lv = f.source->line(f.choice);
  String s(lv.ptr, lv.len);
  if (s.startsWith("~C~") || s.startsWith("~R~")) s.remove(0, 3);
  s.trim();
  return s;   
}
//COPY TEXTSOURCE TO STD::VECTOER<STRING> MEMORY INEFFICIENT
std::vector<String> sourceToVector(const TextSource* src) {
  std::vector<String> result;
  if (!src) return result;

  result.reserve(src->size());
  for (size_t i = 0; i < src->size(); ++i) {
    LineView lv = src->line(i);
    // copy the chars into an Arduino String
    result.push_back(String(lv.ptr).substring(0, lv.len));
  }
  return result;
}


///////////////////////////// DRAWING FUNCTIONS
// DRAW ALL FRAMES STORED WITHIN TOTAL FRAME BOUNDING BOX
void einkTextFramesDynamic(std::vector<Frame*> &frames, bool doFull_, bool noRefresh) {
  if (frames.empty()) return;

  // compute union window
  int minX =  32767, minY =  32767;
  int maxX = -32768, maxY = -32768;
  for (Frame* f : frames) {
    if (!f) continue;
    const int w = display.width()  - f->left  - f->right;
    const int h = display.height() - f->top   - f->bottom;
    if (w <= 0 || h <= 0) continue;
    if (f->left < minX) minX = f->left;
    if (f->top  < minY) minY = f->top;
    if (f->left + w > maxX) maxX = f->left + w;
    if (f->top  + h > maxY) maxY = f->top  + h;
  }
  if (minX > maxX || minY > maxY) return;

  const int winX = minX;
  const int winY = alignUp8(minY);
  const int winW = alignUp8(maxX - minX);
  const int winH = alignUp8(maxY - winY);

  setTXTFont(currentFont);

  display.setPartialWindow(winX, winY, winW, winH);
  display.firstPage();
  do {
    if (doFull_) {
      display.fillRect(winX, winY, winW, winH, GxEPD_WHITE);
    }
    //Serial.println("Drawing Text!");
    for (Frame* frame : frames) {
      if (!frame || !frame->source) continue;

      const int frameW = display.width()  - frame->left - frame->right;
      const int frameH = display.height() - frame->top  - frame->bottom;
      if (frameW <= 0 || frameH <= 0) continue;

      display.fillRect(frame->left + 2, frame->top, frameW - 2, frameH, GxEPD_WHITE);

      if (frame->box) {
        //Serial.println("drawing box!");
        if (frameW > 2 && frameH > 2) {
          drawFrameBox(frame->left + 1, frame->top + 1, frameW - 2, frameH - 2);
        }
      }

      const int lineStride = fontHeight + lineSpacing;

      frame->maxLines = (lineStride > 1) ? (frameH / lineStride) - 1 : 0;
      if (frame->maxLines <= 0) continue;

      const long total  = frame->source ? (long)frame->source->size() : 0L;
      const long maxScroll = (total > frame->maxLines) ? (total - frame->maxLines) : 0L;
      if (frame->scroll > maxScroll) frame->scroll = maxScroll;
      if (frame->scroll < 0)  frame->scroll = 0;
      if (frame == CurrentFrameState && frame->choice >= 0) {
        ensureChoiceVisible(*frame);
      }
      // initialize lastTotal on first draw
      if (frame->lastTotal < 0) frame->lastTotal = total;

      // remember if user was pinned to bottom before we adjust
      const bool wasPinnedToBottom = (frame->scroll == 0);

      // if maxLines shrank or list shrank, cap scroll
      if (frame->scroll > maxScroll) frame->scroll = maxScroll;
      if (frame->scroll < 0)  frame->scroll = 0;

      // if user is at bottom, keep them there when lines grow
      if (wasPinnedToBottom) frame->scroll = 0;

      // update last seen count
      frame->lastTotal = total;
      long startLine = 0, endLine = 0;
      // now get the visible range with the reconciled values


      const int maxTextWidth = frameW;
      int outLine = 0;
      getVisibleRange(frame, total, startLine, endLine); 
      // Force the visible window to the selected line when only one line fits
      if (frame->maxLines <= 1 && frame->choice >= 0 && frame->choice < total) {
        startLine = frame->choice;
        endLine   = frame->choice + 1;       // draw exactly one line
        // (bounds already checked; endLine won't exceed total)
      }
      for (long li = startLine; li < endLine; ++li) {
        LineView lv = frame->source->line(li);
        size_t effLen = trimCRLF(lv.ptr, lv.len);
        if (effLen == 0) { ++outLine; continue; }

        const bool right  = (lv.flags & LF_RIGHT)  != 0;
        const bool center = (lv.flags & LF_CENTER) != 0;

        const bool isSelectedLine = (frame == CurrentFrameState) && (frame->choice == li);
        bool firstSlice = true;

        size_t pos = 0;
        while (pos < effLen) {
          size_t take = sliceThatFits(lv.ptr + pos, effLen - pos, maxTextWidth);
          if (take == 0) break;

          String toPrint;
          if (right)       toPrint = "~R~";
          else if (center) toPrint = "~C~";



          toPrint.concat(String(lv.ptr + pos).substring(0, take));

          if (isSelectedLine && firstSlice) {
            toPrint = toPrint + "<";
          }
          // draw with the current visual row index
          drawLineInFrame(toPrint, outLine++, *frame, /*unused*/0, /*clearLine*/false, /*isPartial*/!doFull_);

          pos += take;
          firstSlice = false;
        }
      }
   
      }
    //Serial.println("Stopped Drawing Text!");
  } while (display.nextPage());
    //Serial.println("Done drawing frames!");
  (void)noRefresh;
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
    display.print(line);
  
    

}

///////////////////////////// FRAME SCROLL FUNCTIONS
void updateScrollFromTouch_Frame() {
  uint16_t touched = cap.touched();
  int newTouch = -1;
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
  if (newTouch != -1) {
    if (lastTouch != -1) { 
      int touchDelta = abs(newTouch - lastTouch);
      if (touchDelta < 2) { 
        long T = CurrentFrameState->source ? (long)CurrentFrameState->source->size() - 1: 0L;
        if (CurrentFrameState->choice == -1){
          Serial.println("Adjusting scroll to clamp non-choice frames");
          T -= CurrentFrameState->maxLines + 1;
        }
        const long maxScroll = max(0L, T);
        if (newTouch > lastTouch) {
          dynamicScroll = min((long)(dynamicScroll + 1), maxScroll);
        } else if (newTouch < lastTouch) {
          dynamicScroll = max((long)(dynamicScroll - 1), 0L);
        }
        //Serial.println("updating scroll to: " + String(dynamicScroll));
        //Serial.println("max scroll is = " + String((int)T));
        if (CurrentFrameState->choice != -1){
          CurrentFrameState->choice = CurrentFrameState->scroll;
        }
        updateScroll(CurrentFrameState, prev_dynamicScroll, dynamicScroll);
      }
    }
    lastTouch = newTouch;
    lastTouchTime = currentTime;
  } 
  else if (lastTouch != -1) {
    if (currentTime - lastTouchTime > TOUCH_TIMEOUT_MS) {
        lastTouch = -1;
        if (prev_dynamicScroll != dynamicScroll) {
            newLineAdded = true;
            prev_dynamicScroll = dynamicScroll; 
            updateScroll(CurrentFrameState, prev_dynamicScroll, dynamicScroll);
            if (CurrentFrameState->choice != -1){
              if (CurrentFrameState == &conversionUnit) {
                Serial.println("changed conversionUnit type!");
                selectUnitType(conversionUnit.choice);
              } else {
                String sym = frameChoiceString(*CurrentFrameState);
                Serial.println("Setting Unit to " + sym);
                CurrentFrameState->unitA = (Unit*)getUnit(sym);
              }
           }
        }
    }
  }

  //Serial.println("update scroll has finished!");
}

///////////////////////////// FRAME OLED FUNCTIONS
void oledScrollFrame() {
  // CLEAR DISPLAY
  u8g2.clearBuffer();
  if (CurrentCALCState != CALC3) {}
  // draw background
  if (CurrentFrameState->choice == -1) u8g2.drawXBMP(0, 0, 128, 32, scrolloled0);

  // draw lines preview
  long int count = CurrentFrameState->source->size();
  long startIndex, endIndex;
  getVisibleRange(CurrentFrameState, count, startIndex, endIndex);

  // decide how many preview lines to show
  long previewTop    = startIndex;
  long previewBottom = endIndex - 1;
  const int rowStep  = 4;
  const int baseY    = 28;

  for (long int i = previewBottom; i >= previewTop && i >= 0; --i) {
    if (i < 0 || i >= count) continue;

    int16_t x1, y1;
    uint16_t charWidth, charHeight;
    LineView lv = CurrentFrameState->source->line(i);
    String line = String(lv.ptr).substring(0, lv.len);

    if (line.startsWith("    ")) {
      display.getTextBounds(line.substring(4), 0, 0, &x1, &y1, &charWidth, &charHeight);
      int lineWidth = map(charWidth, 0, 320, 0, 49);
      lineWidth = constrain(lineWidth, 0, 49);

      // compute Y based on distance from newest visible line
      long posFromBottom = previewBottom - i;
      int boxY = baseY - (rowStep * posFromBottom);
      if (boxY >= 0) {
        // u8g2.drawBox(68, boxY, lineWidth, 2);
      }
    } else {
      display.getTextBounds(line, 0, 0, &x1, &y1, &charWidth, &charHeight);
      int lineWidth = map(charWidth, 0, 320, 0, 56);
      lineWidth = constrain(lineWidth, 0, 56);

      long posFromBottom = previewBottom - i;
      int boxY = baseY - (rowStep * posFromBottom);
      if (boxY >= 0 && CurrentFrameState->choice == -1) {
        u8g2.drawBox(61, boxY, lineWidth, 2);
      }
    }
  }

  long displayedLinesStart = startIndex + 1;
  long displayedLinesEnd   = endIndex;
  if (count == 0) {
    displayedLinesStart = 0;
    displayedLinesEnd   = 0;
  }

  if (CurrentCALCState == CALC3 && (CurrentFrameState->choice != -1)) {

    //  fetch line by scroll
    long idx = CurrentFrameState->scroll;
    if (idx >= 0 && idx < count) { 
      LineView plv = CurrentFrameState->source->line(idx); 
      String pLine = String(plv.ptr).substring(0, plv.len);

      if (pLine.length() > 0) {
        u8g2.setFont(u8g2_font_ncenB10_tr);
        u8g2.drawStr((u8g2.getWidth() - u8g2.getUTF8Width(pLine.substring(3).c_str())) / 2, 24, pLine.substring(3).c_str());
      }
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

///////////////////////////// TEXT POSITION FUNCTIONS
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
