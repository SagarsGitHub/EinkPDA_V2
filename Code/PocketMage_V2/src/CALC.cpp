//   .oooooo.         .o.       ooooo          .oooooo.  //
//  d8P'  `Y8b       .888.      `888'         d8P'  `Y8b //
// 888              .8"888.      888         888         //
// 888             .8' `888.     888         888         //
// 888            .88ooo8888.    888         888         //
// `88b    ooo   .8'     `888.   888       o `88b    ooo //
//  `Y8bood8P'  o88o     o8888o o888ooooood8  `Y8bood8P' // 
#include "globals.h"



// !! Commented for code-review

///////////////////////////// MAIN FUNCTIONS
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
  
    switch (CurrentCALCState) {
      // standard mode
      case CALC0:
        
        setTXTFont(currentFont);
        // update scroll (calc specific function,could be abstracted to processSB_CALC())
        updateScrollFromTouch_Calc();
        
        // HANDLE INPUTS
        //No char recieved
        if (inchar == 0);  
        else if (inchar == 12) {
          if (currentLine.length() > 0) {
            currentLine.remove(currentLine.length() - 1);
          }

        }
        //TAB Recieved (This starts the font switcher now)
        else if (inchar == 9) {    
          CurrentCALCState = CALCFONT;
          CurrentKBState = FUNC;
          newState = true;                              
        }                                      
        //SHIFT Recieved
        else if (inchar == 17) {                                  
          if (CurrentKBState == SHIFT) CurrentKBState = NORMAL;
          else CurrentKBState = SHIFT;
        }
        //FN Recieved
        else if (inchar == 18) {                                  
          if (CurrentKBState == FUNC) CurrentKBState = NORMAL;
          else CurrentKBState = FUNC;
        }
        //Space Recieved
        else if (inchar == 32) {                                  
          currentLine += " ";
        }
        //CR Recieved
        else if (inchar == 13) {    
          calcCRInput();                      
        }
        //ESC / CLEAR Recieved
        else if (inchar == 20) {                                  
          allLinesCalc.clear();
          currentLine = "";
          oledWord("Clearing...");
          drawCalc();
          einkCalcDynamic(false,false);
          display.refresh();
          delay(300);
        }
        // LEFT (scroll up)
        else if (inchar == 19 || inchar == 5) {
          if (dynamicScroll < allLinesCalc.size() - 8){
             dynamicScroll += 8;
          } else if (dynamicScroll < allLinesCalc.size() - 4){
             dynamicScroll += 4;
          } else if (dynamicScroll < allLinesCalc.size() - 2){
             dynamicScroll += 2;
          } else if (dynamicScroll < allLinesCalc.size() - 1) {
            dynamicScroll++;
          }
          newLineAdded = true;
                             
          
        }
        // RIGHT (scroll down)
        else if (inchar == 21 || inchar == 6) { 
          if (dynamicScroll > 19){
            dynamicScroll -= 8;
          }else if (dynamicScroll > 15){
            dynamicScroll -= 4;
          } else if (dynamicScroll > 13){
            dynamicScroll -= 2;
          } else if (dynamicScroll > 12){
            dynamicScroll--;
          } 
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
          allLinesCalc.clear();
          currentLine = "";
          oledWord("Clearing...");

          drawCalc();
          einkCalcDynamic(true);
          delay(300);
        }

        // Font Switcher (regular tab also starts the font switcher)
        else if (inchar == 14) {                                  
          CurrentCALCState = CALCFONT;
          CurrentKBState = FUNC;
          newState = true;         
        }
        // ADD NON-SPECIAL CHARACTERS TO CURRENTLINE
        else {
          currentLine += inchar;
          // NO LOGIC TO SWITCH FN BACK TO NORMAL FOR EASE OF INPUT
        }

        currentMillis = millis();
        //Make sure oled only updates at 60fps
        if (currentMillis - OLEDFPSMillis >= 16) {
          OLEDFPSMillis = currentMillis;
          // ONLY SHOW OLEDLINE WHEN NOT IN SCROLL MODE
          if (lastTouch == -1) {
            oledLine(currentLine);
            if (prev_dynamicScroll != dynamicScroll) prev_dynamicScroll = dynamicScroll;
          }
          else oledScrollCalc();
        }

        

        break;
      // PROGRAMMING MODE (not implemented)
      case CALC1:
        break;
      // SCIENTIFIC MODE (not implemented)
      case CALC2:
        break;
      // CONVERSIONS MODE (not implemented)
      case CALC3:
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
          // refresh screen (new function?)
          //display.fillScreen(GxEPD_WHITE);
          //refresh();
          drawCalc();
          einkCalcDynamic(true);
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
          std::vector<String> tempLines = allLinesCalc;
          allLinesCalc.clear();

          // REFRESH SCREEN
          refresh();
          drawCalc();
          allLinesCalc = tempLines;
          einkCalcDynamic(true);
          
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
    switch (CurrentCALCState) {
      case CALC0:
        
        //standard mode
        if (newState && doFull) {

          
          drawCalc();
          einkCalcDynamic(true);
          //refresh();
          doFull = false;
        } else if (newLineAdded && !newState) {
          
          refresh_count++;
          if (refresh_count > 10){
            drawCalc(); 
            setFastFullRefresh(false);
            einkCalcDynamic(true);
            
            refresh_count = 0;
          } else {
            einkCalcDynamic(true);
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
        break;
      case CALC3:
        //conversions 
        break;
      case CALC4:
        //help mode
        
        currentFont = &FreeMonoBold9pt7b;
        setTXTFont(currentFont);
        // print out everything needed to understand basics of program, might be memory inefficient, remove or rector
        allLinesCalc.clear();
        // potentially store helpText in SD card
        allLinesCalc.insert(allLinesCalc.end(), helpText.begin(), helpText.end());
        dynamicScroll = (allLinesCalc.size() - 11);
        drawCalc(); 
        
        einkCalcDynamic(true);
        calcSwitchedSates = true;
        setFastFullRefresh(false);

        break;
      case CALCFONT:
        

        display.firstPage();
        do {
          // false avoids full refresh
          einkCalcDynamic(true, false);      
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
    newState = false;
    newLineAdded = false;
    if ( CurrentCALCState == CALC4) {
      calcSwitchedSates = 1;
      CurrentCALCState = CALC0;
    }
  }
}


///////////////////////////// SCROLL FUNCTIONS
// could be abstracted to a processSB_Calc function with an interface set up for every app
void updateScrollFromTouch_Calc() {
  //oledWord("checking for touch!");

  uint16_t touched = cap.touched();  // Read touch state
  if (touched){
    //oledWord("touched!");
  }
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
        int maxScroll = max(0, (int)allLinesCalc.size() - maxLines + 1);  // Ensure a valid scroll range
        if (newTouch > lastTouch) {
          dynamicScroll = min((int)(dynamicScroll + 1), maxScroll);

        } else if (newTouch < lastTouch) {
          dynamicScroll = max((int)(dynamicScroll - 1), 0);
        }
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
        }
    }
  }
    //Serial.println("update scroll has finished!");
}


///////////////////////////// ALGORITHMS
// FORMAT INTO RPN,EVALUATE,SAVE
int calculate(const String& cleanedInput,String &result){
  int error = 0;
  //convert to reverse polish notation for easy calculation
  std::queue<String> inputRPN = convertToRPN(cleanedInput,error);
  
  // if not an error, calculate the RPN expression
  result = evaluateRPN(inputRPN);
  prevAns = result;
  return 0;
}

// STRING INPUT TO RPN
std::queue<String> convertToRPN(String expression, int& error) {
    std::queue<String> outputQueue;
    std::stack<String> operatorStack;
    std::vector<String> tokens = tokenize(expression);

    Serial.println("Tokens found:");
    for (const auto& t : tokens) {
        Serial.println(t);
    }
    error = false;

    // Parenthesis validation
    int paren_balance = 0;
    for (char c : expression) {
        if (c == '(') paren_balance++;
        else if (c == ')') paren_balance--;
    }
    if (paren_balance != 0) {
        error = true;
        return outputQueue;
    }

    

    for (size_t i = 0; i < tokens.size(); ++i) {
        const String& token = tokens[i];
        Serial.println("current token = " + token);
        if (isNumberToken(token) || isConstantToken(token)) {
            outputQueue.push(token);
        } 
        else if (isFunctionToken(token)) {
            operatorStack.push(token);
        }
        else if (token == ",") {
          while (!operatorStack.empty() && operatorStack.top() != "(") {
              outputQueue.push(operatorStack.top());
              operatorStack.pop();
          }
        } 
        else if (isAlpha(token[0])) {

          if (i + 1 < tokens.size() && tokens[i+1] == "&") {
              // For assignment, push variable name and & operator
              Serial.println("pushed =var= + variable!");
              outputQueue.push("=var=" + token);  // Special marker for assignment variable
              operatorStack.push("&");  // Then push assignment operator
              i++;  // Skip the &
          } else {
              Serial.println("pushed variable!");
              outputQueue.push(token);
          }
        }
        else if (token == "(") {
            operatorStack.push(token);
        } 
        else if (token == ")") {
          while (!operatorStack.empty() && operatorStack.top() != "(") {
              outputQueue.push(operatorStack.top());
              operatorStack.pop();
          }
          if (!operatorStack.empty()) operatorStack.pop(); // pop "("
          if (!operatorStack.empty() && isFunctionToken(operatorStack.top())) {
              outputQueue.push(operatorStack.top());
              operatorStack.pop();
          }
        } 
        else if (isOperatorToken(token)) {
          // Handle assignment '='
          if (token == "&") {
              // The variable should be the previous token in the original expression
              if (outputQueue.empty()) {
                error = true;
                return outputQueue;
              }
              String prev = outputQueue.back();
              if (!isVariableToken(prev)) {
                error = true;
                return outputQueue;
              }
          }
          while (!operatorStack.empty() &&
                operatorStack.top() != "(" &&
                precedenceCalc[operatorStack.top()] >= precedenceCalc[token]) {
              outputQueue.push(operatorStack.top());
              operatorStack.pop();
          }
          operatorStack.push(token);
      }
    }

    while (!operatorStack.empty()) {
        outputQueue.push(operatorStack.top());
        operatorStack.pop();
    }

    return outputQueue;
}

// SPLIT STRING INTO TOKENS
std::vector<String> tokenize(const String& expression) {
    std::vector<String> tokens;
    String currentToken = "";
    Serial.println("tokenizing expression: " + expression);
    for (int i = 0; i < expression.length(); ++i) {
        char c = expression[i];
        Serial.println("character: " + String(c));
        // shouldn't encounter this in normal calc app functions, but might if called without cleaning string input
        if (c == ' ' || c == '\t') {
            Serial.println("encountered space in tokenizer: " + String(c));
            continue;
        }

        // Handle assignment '=' and equality '=='
        if (c == '&') {
            // Single '=' for assignment
            Serial.println("encountered equals sign (&) in tokenizer: " + String(c));
            tokens.push_back("&");  // Keep as & for assignment
            continue;
        }
        
        if (c == '-' && (i == 0 || expression[i-1] == '(' || isOperatorToken(String(expression[i-1])))) {
            // Treat as part of a number (unary negation)
            Serial.println("encountered unary negation in tokenizer: " + String(c));
            currentToken += c;
            continue;
        }

        // Handle numbers (including decimals)
        if (isDigit(c) || (c == '.' && i + 1 < expression.length() && isDigit(expression[i + 1]))) {
            Serial.println("encountered number in tokenizer: " + String(c));
            currentToken += c;
            while (i + 1 < expression.length() &&
                   (isDigit(expression[i + 1]) || expression[i + 1] == '.')) {
                currentToken += expression[++i];
            }
            tokens.push_back(currentToken);
            currentToken = "";
            continue;
        }

        // Handle alphabetic tokens (variables/functions/constants)
        if (isAlpha(c)) {
            Serial.println("encountered alphabetic expression in tokenizer: " + String(c));
            currentToken += c;
            while (i + 1 < expression.length() && isAlphaNumeric(expression[i + 1])) {
                currentToken += expression[++i];
            }
            tokens.push_back(currentToken);
            currentToken = "";
            continue;
        }

        // Handle parentheses with implied multiplication
        if (c == '(') {
            Serial.println("encountered left parentheses in tokenizer: " + String(c));
            if (!tokens.empty()) {

                const String& prev = tokens.back();
                bool insertMultiply = false;

                // If the previous token is a number, constant, or closing parentheses
                if (isNumberToken(prev) || prev == ")" || prev == "pi" || prev == "e" || prev == "ans") {
                    Serial.println("encountered implicit multiplication with a value: " + String(c));
                    insertMultiply = true;
                }

                // If it's an alphanumeric and not a known function, assume variable * (
                if (isAlpha(prev[0]) && !isFunctionToken(prev)) {
                    Serial.println("encountered implicit multiplication with a variable: " + String(c));
                    insertMultiply = true;
                }

                if (insertMultiply) {
                    Serial.println("added  * to expression: " + String(c));
                    tokens.push_back("'");
                }
            }

            tokens.push_back("(");
            continue;
        }

        // Handle closing parens
        if (c == ')') {
            Serial.println("encountered right parentheses in tokenizer: " + String(c));
            tokens.push_back(")");
            continue;
        }


      
      // Handle operators and commas
      if (isOperatorToken(String(c)) || c == ',') {
          Serial.println("encountered operator or ',': " + String(c));
          tokens.push_back(String(c));
          continue;
      }
      if (c == '!') {
        Serial.println("encountered factorial: " + String(c));
        tokens.push_back("!");
        continue;
      }


      // Unknown token,error
      oledWord("Error: malformed expression");
      delay(1000);
      return {}; 
    }

    return tokens;
}

// EVALUATE RPN
String evaluateRPN(std::queue<String> rpnQueue) {
    std::stack<double> evalStack;
    std::stack<String> varStack;
    
    while (!rpnQueue.empty()) {
        String token = rpnQueue.front();
        Serial.println("varstack size = " + String(varStack.size()));
        rpnQueue.pop();

        // !! can declare these else if blocks as inline functions 
        if (isNumberToken(token)) {
            evalStack.push(token.toDouble());
        }
        // Handle previous anser
        else if (token == "ans") {
            evalStack.push(prevAns.toFloat());
        }
        // Handle constants
        else if (token == "pi") {
            evalStack.push(PI);
        }
        else if (token == "e") {
            evalStack.push(EULER);
        }
        else if (isAlpha(token[0]) && !isFunctionToken(token)) {
          Serial.println("variable check statement");
          // Check if next token is assignment operator
          if (rpnQueue.empty() ){
            evalStack.push(variables[token]);
            continue;
          } 
          Serial.println("after queue empty check");
          Serial.println("rpn que: " + rpnQueue.front());
          if (rpnQueue.front() == "&" && rpnQueue.size() > 1) {
              Serial.println("front &");
              // Push variable name to varStack
              varStack.push(token);
              Serial.println("Stored variable for assignment: " + token);
          } else {
              Serial.println("else state");
              // Regular variable usage
              if (variables.find(token) != variables.end()) {
                Serial.println("pushed regular variable");
                evalStack.push(variables[token]);
              } else {
                return "Error: undefined variable '" + token + "'";
              }
          }
        }
        
        // Handle binary operators
        else if (token == "+") {
            if (evalStack.size() < 2) return "Error with +";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(a + b);
        }
        else if (token == "-") {
            if (evalStack.size() < 2) return "Error with -";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(a - b);
        }
        else if (token == "'") {
            if (evalStack.size() < 2) return "Error with *";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(a * b);
        }
        else if (token == "/") {
            if (evalStack.size() < 2) return "Error with /";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            if (b == 0) return "Div by 0";
            evalStack.push(a / b);
        }
        else if (token == "\"") {
            if (evalStack.size() < 2) return "Error with ^";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(pow(a, b));
        }
        else if (token == "%") {
            if (evalStack.size() < 2) return "Error with %";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            
            // Handle modulo with floating-point numbers using fmod
            if (b == 0) return "Div by 0";
            evalStack.push(fmod(a, b));
        }
        else if (token == "&") {


          if (evalStack.size() < 1 && varStack.empty()) {

              return "Error: assignment";
          }
          String valueVar;
          double value;
          String varName = varStack.top(); varStack.pop();
          //if (!isVariableToken(varName) && !varStack.empty()) return "Error: invalid variable name '" + varName + "'";

            
          if (evalStack.empty()){
            if (varStack.empty()) return "Error: value variable missing";
            varStack.top(); varStack.pop();
            value = variables[valueVar];
            evalStack.push(value);
            variables[varName] = value; 
          } else {
            value = evalStack.top(); evalStack.pop();
            evalStack.push(value); 
            Serial.println("Set " + varName + " to " + String(value));
            variables[varName] = value; 
            
          }
          Serial.println("Assigned " + varName + " = " + String(value));
          
        }

        // Handle unary operators
        else if (token == "!") {
            if (evalStack.empty()) return "Error with !";
            double a = evalStack.top(); evalStack.pop();
            if (a < 0) return "Error with ! input ";
            evalStack.push(tgamma(a + 1));
        }
        
        // Handle functions
        // Trig functions too numerous and messy, need to do something about repeat code 
        else if (token == "sin") {
            if (evalStack.empty()) return "Error with sin";
            double a = evalStack.top(); evalStack.pop();
            // if in degree mode convert from degree to rad
            convertRad(a,isRad);
            evalStack.push(convertDeg(sin(a),isRad));
        }
        else if (token == "asin") {
            if (evalStack.empty()) return "Error with asin";
            double a = evalStack.top(); evalStack.pop();
            // if in degree mode convert from degree to rad
            convertRad(a,isRad);
            evalStack.push(convertDeg(asin(a),isRad));
        }
        else if (token == "sinh") {
            if (evalStack.empty()) return "Error with sinh";
            double a = evalStack.top(); evalStack.pop();
            // if in degree mode convert from degree to rad
            convertRad(a,isRad);
            evalStack.push(convertDeg(sinh(a),isRad));
        }
        else if (token == "csc") {
            if (evalStack.empty()) return "Error with csc";
            double a = evalStack.top(); evalStack.pop();
            // if in degree mode convert from degree to rad
            convertRad(a,isRad);
            if (sin(a) == 0) return "Error: divide by zero csc";
            evalStack.push(convertDeg(1/sin(a),isRad));
        }
        else if (token == "acsc") {
            if (evalStack.empty()) return "Error with acsc";
            double a = evalStack.top(); evalStack.pop();
            // if in degree mode convert from degree to rad
            convertRad(a,isRad);
            if (asin(a) == 0) return "Error: divide by zero acsc";
            evalStack.push(convertDeg(1/asin(a),isRad));
        }
        
        else if (token == "csch") {
            if (evalStack.empty()) return "Error with csch";
            double a = evalStack.top(); evalStack.pop();
            // if in degree mode convert from degree to rad
            convertRad(a,isRad);
            if (sinh(a) == 0) return "Error: divide by zero csch";
            evalStack.push(convertDeg(1/sinh(a),isRad));
        }
        
        else if (token == "cos") {
            if (evalStack.empty()) return "Error with cos";
            double a = evalStack.top(); evalStack.pop();
            // if in degree mode convert from degree to rad
            convertRad(a,isRad);
            evalStack.push(convertDeg(cos(a),isRad));
        }
        else if (token == "acos") {
            if (evalStack.empty()) return "Error with acos";
            double a = evalStack.top(); evalStack.pop();
            // if in degree mode convert from degree to rad
            convertRad(a,isRad);
            evalStack.push(convertDeg(acos(a),isRad));
        }
        else if (token == "cosh") {
            if (evalStack.empty()) return "Error with cosh";
            double a = evalStack.top(); evalStack.pop();
            // if in degree mode convert from degree to rad
            convertRad(a,isRad);
            evalStack.push(convertDeg(cosh(a),isRad));
        }
        else if (token == "sec") {
            if (evalStack.empty()) return "Error with sec";
            double a = evalStack.top(); evalStack.pop();
            // if in degree mode convert from degree to rad
            convertRad(a,isRad);
            if (cos(a) == 0) return "Error: divide by zero sec";
            evalStack.push(convertDeg(1/cos(a),isRad));
        }
        else if (token == "asec") {
            if (evalStack.empty()) return "Error with asec";
            double a = evalStack.top(); evalStack.pop();
            // if in degree mode convert from degree to rad
            convertRad(a,isRad);
            if (acos(a) == 0) return "Error: divide by zero asec";
            evalStack.push(convertDeg(1/acos(a),isRad));
        }
        else if (token == "sech") {
            if (evalStack.empty()) return "Error with sech";
            double a = evalStack.top(); evalStack.pop();
            // if in degree mode convert from degree to rad
            convertRad(a,isRad);
            if (cosh(a) == 0) return "Error: divide by zero sech";
            evalStack.push(convertDeg(1/cosh(a),isRad));
        }
        else if (token == "tan") {
            if (evalStack.empty()) return "Error with tan";
            double a = evalStack.top(); evalStack.pop();
            // if in degree mode convert from degree to rad
            convertRad(a,isRad);
            evalStack.push(convertDeg(tan(a),isRad));
        }
        else if (token == "atan") {
            if (evalStack.empty()) return "Error with atan";
            double a = evalStack.top(); evalStack.pop();
            // if in degree mode convert from degree to rad
            convertRad(a,isRad);
            evalStack.push(convertDeg(atan(a),isRad));
        }
        else if (token == "tanh") {
            if (evalStack.empty()) return "Error with tanh";
            double a = evalStack.top(); evalStack.pop();
            // if in degree mode convert from degree to rad
            convertRad(a,isRad);
            evalStack.push(convertDeg(tanh(a),isRad));
        }
        else if (token == "cot") {
            if (evalStack.empty()) return "Error with cot";
            double a = evalStack.top(); evalStack.pop();
            // if in degree mode convert from degree to rad
            convertRad(a,isRad);
            if (tan(a) == 0) return "Error: divide by zero cot";
            evalStack.push(convertDeg(1/tan(a),isRad));
        }
        else if (token == "acot") {
            if (evalStack.empty()) return "Error with acot";
            double a = evalStack.top(); evalStack.pop();
            // if in degree mode convert from degree to rad
            convertRad(a,isRad);
            if (atan(a) == 0) return "Error: divide by zero acot";
            evalStack.push(convertDeg(1/atan(a),isRad));
        }
        else if (token == "coth") {
            if (evalStack.empty()) return "Error with coth";
            double a = evalStack.top(); evalStack.pop();
            // if in degree mode convert from degree to rad
            convertRad(a,isRad);
            if (tanh(a) == 0) return "Error: divide by zero coth";
            evalStack.push(convertDeg(1/tanh(a),isRad));
        }
        else if (token == "sqrt") {
            if (evalStack.empty()) return "Error with sqrt";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(sqrt(a));
        }
        else if (token == "exp") {
            if (evalStack.empty()) return "Error with exp";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(exp(a));
        }
        else if (token == "round") {
            if (evalStack.empty()) return "Error with round";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(round(a));
        }
        else if (token == "log") {
            if (evalStack.empty()) return "Error with log";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(log(a));
        }
        else if (token == "log10") {
            if (evalStack.empty()) return "Error with log10";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(log10(a));
        }
        else if (token == "floor") {
            if (evalStack.empty()) return "Error with floor";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(floor(a));
        }
        else if (token == "ceil") {
            if (evalStack.empty()) return "Error with ceil";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(ceil(a));
        }
        else if (token == "abs") {
            if (evalStack.empty()) return "Error with abs";
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(fabsf(a));
        }


        // handle multiple input functions
        else if (token == "max") {
            if (evalStack.size() < 2) return "Error with max";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(max(a, b));
        }
        else if (token == "min") {
            if (evalStack.size() < 2) return "Error with min";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(min(a, b));
        }
        else if (token == "pow") {
            if (evalStack.size() < 2) return "Error with pow";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(pow(a, b));
        }
        else if (token == "rand") {
            if (evalStack.size() < 2) return "Error with rand";
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            evalStack.push(random(a, b));
        }
        else if (token.startsWith("=var=")) {
            // This is an assignment variable - push to varStack
            String varName = token.substring(5);
            varStack.push(varName);
            Serial.println("Stored assignment variable: " + varName);
        }
        else if (token == "&") {
          
          // Needs exactly 1 value and 1 variable
          if (evalStack.size() < 1 || varStack.empty()) {
              return "Error: assignment needs 1 value and 1 variable";
          }

          String varName = varStack.top(); varStack.pop();
          double value = evalStack.top(); evalStack.pop();
          
          
          variables[varName] = value;
          evalStack.push(value);  
          
          Serial.println("Assigned " + varName + " = " + String(value));
        }
        
        // Add other functions as needed...
        else {
            return "Unknown token: " + token;
        }
    }

    if (evalStack.size() != 1) return "Error converting RPN: stack empty";

    prevAns = String(evalStack.top());
    return formatNumber(evalStack.top());
}

///////////////////////////// INPUT FUNCTIONS
// ENTER (CR) INPUT
void calcCRInput(){
  // reset eink screen if returning from a new mode
  if (calcSwitchedSates == 1){
    calcSwitchedSates = 0;
    allLinesCalc.clear();
    doFull = 1;

    drawCalc();

    einkCalcDynamic(true);
  }

  // clean input (remove spaces)
  stripFunction(currentLine,cleanExpression);

  // calculate answer
  if (cleanExpression != ""){
    
    
    if (cleanExpression == "/4"){
        CurrentCALCState = CALC4; 
    
    }else if (cleanExpression == "/5") {
        // write current file to text
        oledWord("Exporting Data to TXT!");
        allLines = allLinesCalc;
        
        for (int i = 0; i < allLines.size();i++){
          if (!(i%2 == 0)){
            // remvove '~R~' formatting (not used by txt app)
            String temp = allLines[i - 1] + " = " + allLines[i].substring(3);
            allLines[i - 1] =  temp;
            allLines[i] = "";
          }
        }

        closeCalc(TXT);
        
    } else if (cleanExpression == "/6"){
      
      closeCalc(HOME);
            
    } else if (cleanExpression == "/rad"){
      
      isRad = 1;
      drawCalc();
    } else if (cleanExpression == "/deg"){
      
      isRad = 0;
      drawCalc();      
    } else {
      // no command, calculate answer
      dynamicScroll = 0;
      printAnswer(cleanExpression);
    }
  }
  
  cleanExpression = "";
  calculatedResult = "";
  currentLine = "";
  newLineAdded = true;
}
// CONVERT NUMBER TO FLOAT STRING OR INT STRING
String formatNumber(double value) {
    
    if (value > INT_MAX) return "inf";
    if (value < INT_MIN) return "-inf";
    Serial.println("formating " + String(value));
    char buffer[32];
    // handle integer test case
    if (value == floor(value)) {
      snprintf(buffer, sizeof(buffer), "%ld", static_cast<long>(value));
      return String(buffer);
    }

    
    // print up to 8 decimals
    snprintf(buffer, sizeof(buffer), "%.8f", value);

    String result(buffer);
    Serial.println("looking at zeros" + result);
    // trim excess zeros (implement different handling w/ scientific mode: variable for sig figs, and check calc state)
    int dotPos = result.indexOf('.');
    if (dotPos != -1) {
        // START FROM BACK AND ITERATE UNTIL FIRST NON-ZERO OR DOT
        int lastNonZero = result.length() - 1;
        while (lastNonZero > dotPos && result[lastNonZero] == '0') {
            lastNonZero--;
        }
        if (lastNonZero == 1) {
          result = result.substring(0,lastNonZero + 2);
        } else {
          result = result.substring(0,lastNonZero + 1);
        }
    }
    Serial.println("formated result " + result);
    return result;
}
// REMOVE SPACES
// time inefficient N -> 2N, ignorable
void stripFunction(const String& input, String &cleanedInput){
  cleanedInput = input;  
  cleanedInput.replace(" ", ""); 
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
    // check if float is valid (no trailing or ending '.' + no alpha)
    for (size_t i = start; i < token.length(); i++) {
        if (token[i] == '.') {
            if (hasDecimal) return false;
            hasDecimal = true;
            
            // CHECK FOR FRONT AND BACK '.'
            // note: currently doesn't allow for ".12"
            if (i == start || i == token.length() - 1) {
                return false; // "." at start or end
            }
        }
        else if (!isDigit(token[i])) {
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
// CONFIRM A TOKEN IS A VARIABLE ECLUDING CONSTANTS
bool isVariableToken(const String& token) {
    if (token.isEmpty()) return false;
    // EXCLUDE CONSTANTS
    if (isConstantToken(token)) return false;
    if (isFunctionToken(token)) return false;

    // CHECK EACH LETTER TO ENSURE ITS A VARIABLE
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

double convertDeg(double radInput,bool isRad){
  return isRad ? radInput : ((radInput*180.0)/PI);
}

double convertRad(double degInput,bool isRad){
  return !isRad ? degInput : ((degInput*PI)/180.0);
}