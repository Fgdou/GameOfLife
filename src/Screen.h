//
// Created by fgdou on 7/11/21.
//

#ifndef GAMEOFLIFE_SCREEN_H
#define GAMEOFLIFE_SCREEN_H

#define ESC "\033["


#include <ostream>

/**
 * This class is used to navigate around the terminal screen and change the cursor position
 */
class Screen {
public:

    Screen(FILE* stream);

    void render();
    void leftStart();
    void left(int n);
    void right(int n);
    void up(int n);
    void down(int n);
    void set(int x, int y);
    void clear();
    void home();

    void setCursorVisible(bool visible);

    void print(const char* c);
    void newline();

    void reset();

private:

    FILE* stream;
};


#endif //GAMEOFLIFE_SCREEN_H
