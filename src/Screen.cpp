//
// Created by fgdou on 7/11/21.
//

#include "Screen.h"

Screen::Screen(FILE *stream)
    : stream(stream)
{
    fprintf(stream, ESC "=7l");
}

void Screen::render() {
    fflush(stream);
}

void Screen::leftStart() {
    fprintf(stream, "\r");
}

void Screen::left(int n) {
    fprintf(stream, ESC "%dD", n);
}
void Screen::up(int n) {
    fprintf(stream, ESC "%dA", n);
}
void Screen::down(int n) {
    fprintf(stream, ESC "%dB", n);
}
void Screen::right(int n) {
    fprintf(stream, ESC "%dC", n);
}

void Screen::set(int x, int y) {
    fprintf(stream, ESC "%d;%dH", y, x);
}

void Screen::clear() {
    fprintf(stream, ESC "2J");
    home();
}

void Screen::home() {
    fprintf(stream, ESC "H");
}

void Screen::print(const char* c) {
    fprintf(stream, "%s", c);
}

void Screen::setCursorVisible(bool visible) {
    if(visible)
        fprintf(stream, ESC "?25h");
    else
        fprintf(stream, ESC "?25l");
}

void Screen::newline() {
    down(1);
    leftStart();
}

void Screen::reset() {
    setCursorVisible(true);
}

