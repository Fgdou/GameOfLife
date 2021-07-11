#include <iostream>
#include <unistd.h>
#include <cstring>
#include <csignal>

#include "Screen.h"

Screen sc(stdout);
int height = 0;
int width = 0;
bool running = true;
int radius = 10;

void initArray(bool *array, int percent) {

    for(int i=0; i<height*width; i++) {
        int x = i%width;
        int y = i/width;

        if(x < width/2 - radius || x > width / 2 + radius)
            array[i] = false;
        else if(y < height/2 - radius || y > height / 2 + radius)
            array[i] = false;
        else
            array[i] = (rand()%100) < percent;
    }
}

void repeat(const char* c, int n){
    for(int i=0; i<n; i++)
        sc.print(c);
}

void print(const bool* array, int changes, int moves){
    sc.home();

    //Top bar
    sc.print("\u2554");
    repeat("\u2550", width);
    sc.print("\u2557");
    sc.newline();
    sc.print("\u2551");

    //Content
    for(int i=0; i<height*width; i++){
        if(i%width == 0 && i != 0) {
            sc.print("\u2551");
            sc.newline();
            sc.print("\u2551");
        }
        if(array[i])
            sc.print("\u2588");
        else
            sc.print(" ");
    }

    //Bottom bar
    sc.print("\u2551");
    sc.newline();
    sc.print("\u2560");
    repeat("\u2550", width);
    sc.print("\u2563");
    sc.newline();
    sc.print("\u2551");
    repeat("#", width*changes/1000);
    repeat(" ", width - width*changes/1000);
    sc.print("\u2551");
    sc.leftStart();
    sc.right(width/2);
    std::cout << moves;
    sc.newline();
    sc.print("\u255A");
    repeat("\u2550", width);
    sc.print("\u255D");

    sc.render();
}

void errorExit(const char* name){
    std::cout << "Usage : " << name << " width height percent radius speed\n";
    exit(EXIT_FAILURE);
}

void sig_handler(int sig) {
    switch (sig) {
        case SIGINT:
            running = false;
            break;
        default:
            exit(EXIT_FAILURE);
    }
}

void setState(bool* array, int x, int y, bool state){
    array[y*width + x] = state;
}
bool isAlive(bool* array, int x, int y){
    //Wrape around
    if(x < 0) x += width;
    if(y < 0) y += height;

    x = x%width;
    y = y%height;

    return array[y*width + x];
}

int act(bool* array, bool* tmp){
    int changes = 0;
    memcpy(tmp, array, height*width);

    //For each cell
    for(int y=0; y<height; y++)
        for(int x=0; x<width; x++){
            int cnt = 0;

            //Count around
            for(int yy=-1; yy<=1; yy++)
                for(int xx=-1; xx<=1; xx++){
                    if(xx == 0 && yy == 0)
                        continue;
                    if(isAlive(tmp, x+xx, y+yy))
                        cnt++;
                }

            //Rules
            if(isAlive(tmp, x, y) && !(cnt == 2 || cnt == 3)){
                setState(array, x, y, false);
                changes++;
            }else if(!isAlive(tmp, x, y) && cnt == 3){
                setState(array, x, y, true);
                changes++;
            }
        }
    return changes;
}

int main(int argc, char** argv) {
    //Exit when ctrl-c
    signal(SIGINT, sig_handler);

    //Get values
    if(argc != 6)
        errorExit(argv[0]);

    width = std::stoi(argv[1]);
    height = std::stoi(argv[2]);
    int percent = std::stoi(argv[3]);
    radius = std::stoi(argv[4]);
    int speed = std::stoi(argv[5]);

    srand(time(NULL));

    sc.clear();
    sc.setCursorVisible(false);

    bool* array = new bool[height*width];
    bool* arrayCopy = new bool[height*width];

    initArray(array, percent);

    int changes[4] = {-1,-1,-1, -1};
    int max = 1;

    print(array, 0, 0);

    while(running){
        //Check the changes
        changes[3] = changes[2];
        changes[2] = changes[1];
        changes[1] = changes[0];
        changes[0] = act(array, arrayCopy);

        int moves = abs(changes[0]-changes[1]) + abs(changes[1]-changes[2]) + abs(changes[2]-changes[3]);
        if(changes[3] == -1)
            moves = 0;

        if(moves > max)
            max = moves;

        //Update
        print(array, moves*1000/max, moves);
        usleep(1000000/speed);

        if(changes[2] == changes[1] && changes[1] == changes[0] && changes[3] == changes[2])
            running = false;

    }

    delete[] array;
    sc.reset();
}