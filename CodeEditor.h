#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <SFML/Graphics.hpp>
#include "Strings.h"

class CodeEditor 
{
private:
    sf::RectangleShape box;
    sf::RectangleShape lineNumbersBg;
    sf::Font& font;
    
    float posX, posY, width, height;
    
    String* lines;
    int lineCount;
    int lineCapacity;
    
    int scrollOffset;
    int visibleLines;
    float lineHeight;
    
    int cursorLine;
    int cursorCol;
    bool cursorVisible;
    sf::Clock cursorBlink;
    
    String templateContent;
    bool hasContent;
    
    bool selected;
    sf::Color colorNormal;
    sf::Color colorFocused;
    sf::Color outlineColor;
    
    static const int LINE_NUM_WIDTH = 40;
    static const int PADDING = 4;
    
    void expandLines();
    void ensureLineExists(int line);
    void insertChar(int line, int col, char c);
    void deleteChar(int line, int col);
    void splitLine(int line, int col);
    void mergeLines(int line);
    void updateScroll();
    int getIndentationLevel(const String& s) const;
    float colToPixelX(int line, int col) const;
    int pixelXToCol(int line, float pixX) const;

public:
    CodeEditor(float x, float y, float w, float h, const char* templateText, sf::Font& f);
    ~CodeEditor();
    
    CodeEditor(const CodeEditor&) = delete;
    CodeEditor& operator=(const CodeEditor&) = delete;
    
    void handleClick(float mouseX, float mouseY);
    void handleTypedChar(char c);
    void handleBackspace();
    void handleEnter();
    void handleTab();
    void handleArrowUp();
    void handleArrowDown();
    void handleArrowLeft();
    void handleArrowRight();
    
    void update(float mouseX, float mouseY);
    void render(sf::RenderWindow& window);
    
    String getText() const;
    void setText(const String& text);
    void clear();
    bool isSelected() const { return selected; }
    void unselect();
    bool isEmpty() const;
};

#endif