#include "CodeEditor.h"
#include <cstring>
#include <cstdio>

static void intToCStr(int n, char* buf, int bufSize) 
{
    snprintf(buf, bufSize, "%d", n);
}

CodeEditor::CodeEditor(float x, float y, float w, float h, const char* templateText, sf::Font& f)
    : font(f), posX(x), posY(y), width(w), height(h), lineCount(0), lineCapacity(64), scrollOffset(0), lineHeight(20.f), cursorLine(0), cursorCol(0), cursorVisible(true), hasContent(false), selected(false)
{
    lines = new String[lineCapacity];
    lines[0] = String("");
    lineCount = 1;
    visibleLines = static_cast<int>(h / lineHeight);
    templateContent = String(templateText ? templateText : "");
    
    colorNormal = sf::Color(255, 180, 200);
    colorFocused = sf::Color(28, 31, 45);
    outlineColor = colorNormal;
    
    box.setSize(sf::Vector2f(w, h));
    box.setPosition(x, y);
    box.setFillColor(sf::Color(255, 200, 220));
    box.setOutlineColor(outlineColor);
    box.setOutlineThickness(2.f);
    
    lineNumbersBg.setSize(sf::Vector2f(40, h));
    lineNumbersBg.setPosition(x, y);
    lineNumbersBg.setFillColor(sf::Color(255, 180, 200));
}

CodeEditor::~CodeEditor() {
    delete[] lines;
}

void CodeEditor::expandLines() {
    int newCap = lineCapacity * 2;
    String* newLines = new String[newCap];
    for (int i = 0; i < lineCount; ++i)
        newLines[i] = lines[i];
    delete[] lines;
    lines = newLines;
    lineCapacity = newCap;
}

void CodeEditor::ensureLineExists(int line) {
    while (line >= lineCount) {
        if (lineCount >= lineCapacity)
            expandLines();
        lines[lineCount] = String("");
        ++lineCount;
    }
}

void CodeEditor::insertChar(int line, int col, char c) {
    ensureLineExists(line);
    const String& old = lines[line];
    int len = old.getlen();
    
    if (col < 0) col = 0;
    if (col > len) col = len;
    
    String left = (col > 0) ? old.substring(0, col) : String("");
    String right = (col < len) ? old.substring(col, len - col) : String("");
    
    char temp[2] = {c, '\0'};
    String charStr(temp);
    
    lines[line] = left + charStr + right;
}

void CodeEditor::deleteChar(int line, int col) {
    if (line < 0 || line >= lineCount) return;
    if (col <= 0) return;
    
    const String& old = lines[line];
    int len = old.getlen();
    if (col > len) col = len;
    
    String left = (col > 1) ? old.substring(0, col - 1) : String("");
    String right = (col < len) ? old.substring(col, len - col) : String("");
    lines[line] = left + right;
}

void CodeEditor::splitLine(int line, int col) {
    ensureLineExists(line);
    const String& old = lines[line];
    int len = old.getlen();
    if (col < 0) col = 0;
    if (col > len) col = len;
    
    String before = (col > 0) ? old.substring(0, col) : String("");
    String after = (col < len) ? old.substring(col, len - col) : String("");
    
    if (lineCount >= lineCapacity)
        expandLines();
    
    for (int i = lineCount; i > line + 1; --i)
        lines[i] = lines[i - 1];
    
    lines[line] = before;
    lines[line + 1] = after;
    ++lineCount;
}

void CodeEditor::mergeLines(int line) {
    if (line < 0 || line + 1 >= lineCount) return;
    
    lines[line] = lines[line] + lines[line + 1];
    
    for (int i = line + 1; i < lineCount - 1; ++i)
        lines[i] = lines[i + 1];
    
    lines[lineCount - 1] = String("");
    --lineCount;
    if (lineCount < 1) {
        lines[0] = String("");
        lineCount = 1;
    }
}

void CodeEditor::updateScroll() {
    if (cursorLine < scrollOffset)
        scrollOffset = cursorLine;
    if (cursorLine >= scrollOffset + visibleLines)
        scrollOffset = cursorLine - visibleLines + 1;
    if (scrollOffset < 0)
        scrollOffset = 0;
}

int CodeEditor::getIndentationLevel(const String& s) const {
    int count = 0;
    int len = s.getlen();
    while (count < len && s[count] == ' ')
        ++count;
    return count;
}

float CodeEditor::colToPixelX(int line, int col) const {
    if (line < 0 || line >= lineCount) return 0.f;
    const char* src = lines[line].getstr();
    int len = lines[line].getlen();
    if (col > len) col = len;
    
    sf::Text probe;
    probe.setFont(font);
    probe.setCharacterSize(15);
    
    char tmp[512];
    int copyLen = col < 511 ? col : 511;
    for (int i = 0; i < copyLen; ++i)
        tmp[i] = src[i];
    tmp[copyLen] = '\0';
    
    probe.setString(tmp);
    return probe.getLocalBounds().width;
}

int CodeEditor::pixelXToCol(int line, float pixX) const {
    if (line < 0 || line >= lineCount) return 0;
    const char* src = lines[line].getstr();
    int len = lines[line].getlen();
    if (len == 0) return 0;
    
    sf::Text probe;
    probe.setFont(font);
    probe.setCharacterSize(15);
    
    char tmp[512];
    for (int c = 0; c <= len; ++c) {
        int copyLen = c < 511 ? c : 511;
        for (int i = 0; i < copyLen; ++i)
            tmp[i] = src[i];
        tmp[copyLen] = '\0';
        probe.setString(tmp);
        float w = probe.getLocalBounds().width;
        if (w >= pixX)
            return c;
    }
    return len;
}

void CodeEditor::handleClick(float mouseX, float mouseY) {
    if (mouseX < posX || mouseX > posX + width || mouseY < posY || mouseY > posY + height) {
        selected = false;
        outlineColor = colorNormal;
        box.setOutlineColor(outlineColor);
        return;
    }
    
    selected = true;
    outlineColor = colorFocused;
    box.setOutlineColor(outlineColor);
    
    float relY = mouseY - posY;
    int clickedLine = scrollOffset + static_cast<int>(relY / lineHeight);
    if (clickedLine < 0) clickedLine = 0;
    if (clickedLine >= lineCount) clickedLine = lineCount - 1;
    cursorLine = clickedLine;
    
    float relX = mouseX - posX - 40 - 4;
    if (relX < 0.f) relX = 0.f;
    cursorCol = pixelXToCol(cursorLine, relX);
    
    int lineLen = lines[cursorLine].getlen();
    if (cursorCol > lineLen) cursorCol = lineLen;
    
    cursorVisible = true;
    cursorBlink.restart();
}

void CodeEditor::handleTypedChar(char c) {
    if (!selected) return;
    if (c < 32 || c == 127) return;
    
    ensureLineExists(cursorLine);
    insertChar(cursorLine, cursorCol, c);
    ++cursorCol;
    hasContent = true;
    
    updateScroll();
    cursorVisible = true;
    cursorBlink.restart();
}

void CodeEditor::handleBackspace() {
    if (!selected) return;
    ensureLineExists(cursorLine);
    
    if (cursorCol > 0) {
        deleteChar(cursorLine, cursorCol);
        --cursorCol;
    } else if (cursorLine > 0) {
        int prevLineLen = lines[cursorLine - 1].getlen();
        mergeLines(cursorLine - 1);
        --cursorLine;
        cursorCol = prevLineLen;
    }
    
    hasContent = !isEmpty();
    updateScroll();
    cursorVisible = true;
    cursorBlink.restart();
}

void CodeEditor::handleEnter() {
    if (!selected) return;
    ensureLineExists(cursorLine);
    
    int indent = getIndentationLevel(lines[cursorLine]);
    
    splitLine(cursorLine, cursorCol);
    ++cursorLine;
    cursorCol = 0;
    
    for (int i = 0; i < indent; ++i) {
        insertChar(cursorLine, cursorCol, ' ');
        ++cursorCol;
    }
    
    hasContent = true;
    updateScroll();
    cursorVisible = true;
    cursorBlink.restart();
}

void CodeEditor::handleTab() {
    if (!selected) return;
    ensureLineExists(cursorLine);
    
    for (int i = 0; i < 4; ++i) {
        insertChar(cursorLine, cursorCol, ' ');
        ++cursorCol;
    }
    
    hasContent = true;
    updateScroll();
    cursorVisible = true;
    cursorBlink.restart();
}

void CodeEditor::handleArrowUp() {
    if (!selected) return;
    if (cursorLine > 0) {
        --cursorLine;
        int lineLen = lines[cursorLine].getlen();
        if (cursorCol > lineLen) cursorCol = lineLen;
    }
    updateScroll();
    cursorVisible = true;
    cursorBlink.restart();
}

void CodeEditor::handleArrowDown() {
    if (!selected) return;
    if (cursorLine < lineCount - 1) {
        ++cursorLine;
        int lineLen = lines[cursorLine].getlen();
        if (cursorCol > lineLen) cursorCol = lineLen;
    }
    updateScroll();
    cursorVisible = true;
    cursorBlink.restart();
}

void CodeEditor::handleArrowLeft() {
    if (!selected) return;
    if (cursorCol > 0) {
        --cursorCol;
    } else if (cursorLine > 0) {
        --cursorLine;
        cursorCol = lines[cursorLine].getlen();
    }
    updateScroll();
    cursorVisible = true;
    cursorBlink.restart();
}

void CodeEditor::handleArrowRight() {
    if (!selected) return;
    int lineLen = lines[cursorLine].getlen();
    if (cursorCol < lineLen) {
        ++cursorCol;
    } else if (cursorLine < lineCount - 1) {
        ++cursorLine;
        cursorCol = 0;
    }
    updateScroll();
    cursorVisible = true;
    cursorBlink.restart();
}

void CodeEditor::update(float mouseX, float mouseY) {
    (void)mouseX;
    (void)mouseY;
    if (selected && cursorBlink.getElapsedTime().asSeconds() >= 0.5f) {
        cursorVisible = !cursorVisible;
        cursorBlink.restart();
    }
}

void CodeEditor::render(sf::RenderWindow& window) {
    window.draw(box);
    window.draw(lineNumbersBg);
    
    if (!hasContent && templateContent.getlen() > 0) {
        sf::Text tmpl;
        tmpl.setFont(font);
        tmpl.setCharacterSize(15);
        tmpl.setFillColor(sf::Color(150, 140, 160));
        tmpl.setString(templateContent.getstr());
        tmpl.setPosition(posX + 40 + 4, posY + 4);
        window.draw(tmpl);
    }
    
    int lastVisible = scrollOffset + visibleLines;
    if (lastVisible > lineCount) lastVisible = lineCount;
    
    sf::Text lineText;
    lineText.setFont(font);
    lineText.setCharacterSize(15);
    lineText.setFillColor(sf::Color(80, 70, 80));
    
    sf::Text numText;
    numText.setFont(font);
    numText.setCharacterSize(12);
    numText.setFillColor(sf::Color(80, 70, 80));
    
    for (int i = scrollOffset; i < lastVisible; ++i) {
        float rowY = posY + (i - scrollOffset) * lineHeight;
        
        char numBuf[16];
        intToCStr(i + 1, numBuf, sizeof(numBuf));
        numText.setString(numBuf);
        float numW = numText.getLocalBounds().width;
        numText.setPosition(posX + 40 - numW - 4, rowY + 3);
        window.draw(numText);
        
        if (i < lineCount && lines[i].getlen() > 0) {
            lineText.setString(lines[i].getstr());
            lineText.setPosition(posX + 40 + 4, rowY + 2);
            window.draw(lineText);
        }
    }
    
    if (selected && cursorVisible) {
        float cursorX = posX + 40 + 4 + colToPixelX(cursorLine, cursorCol);
        float cursorY = posY + (cursorLine - scrollOffset) * lineHeight + 2;
        
        if (cursorLine >= scrollOffset && cursorLine < scrollOffset + visibleLines) {
            sf::RectangleShape caret(sf::Vector2f(2.f, lineHeight - 4.f));
            caret.setPosition(cursorX, cursorY);
            caret.setFillColor(sf::Color(80, 70, 80));
            window.draw(caret);
        }
    }
}

String CodeEditor::getText() const {
    String result("");
    for (int i = 0; i < lineCount; ++i) {
        result = result + lines[i];
        if (i < lineCount - 1) {
            char newline[2] = {'\n', '\0'};
            result = result + String(newline);
        }
    }
    return result;
}

void CodeEditor::setText(const String& text) {
    delete[] lines;
    lineCapacity = 64;
    lines = new String[lineCapacity];
    lineCount = 0;
    scrollOffset = 0;
    cursorLine = 0;
    cursorCol = 0;
    
    const char* src = text.getstr();
    int len = text.getlen();
    
    int start = 0;
    for (int i = 0; i <= len; ++i) {
        if (i == len || src[i] == '\n') {
            if (lineCount >= lineCapacity)
                expandLines();
            
            int segLen = i - start;
            if (segLen > 0)
                lines[lineCount] = text.substring(start, segLen);
            else
                lines[lineCount] = String("");
            
            ++lineCount;
            start = i + 1;
        }
    }
    
    if (lineCount == 0) {
        lines[0] = String("");
        lineCount = 1;
    }
    
    hasContent = !isEmpty();
}

void CodeEditor::clear() {
    for (int i = 0; i < lineCount; ++i)
        lines[i] = String("");
    lineCount = 1;
    scrollOffset = 0;
    cursorLine = 0;
    cursorCol = 0;
    hasContent = false;
}

void CodeEditor::unselect() {
    selected = false;
    outlineColor = colorNormal;
    box.setOutlineColor(outlineColor);
}

bool CodeEditor::isEmpty() const {
    for (int i = 0; i < lineCount; ++i)
        if (lines[i].getlen() > 0)
            return false;
    return true;
}