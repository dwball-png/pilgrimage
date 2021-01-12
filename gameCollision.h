/*
  Daniel Ball
  gameCollision.h
  CPSC 4160
*/

#pragma once
#include <SDL2/SDL.h> 
#include <iostream>


class Collision {
    public:
        Collision();
        ~Collision();

        // checks for intersection between rectangles a and b
        static bool collideRectangle(SDL_Rect* a, SDL_Rect* b) {
            // aR < bL
            if ((a->x + a->w) < (b->x)) {
                return false;
            }
            // aL > bR
            if ((a->x) > (b->x + b->w)) {
                return false;
            }
            // aB < aT
            if ((a->y + a->h) < (b->y)) {
                return false;
            }
            // aT > yB
            if ((a->y) > (b->y + b->h)) {
                return false;
            }
            return true;
        }

        // checks for intersection between point (x, y) and rectangle b
        static bool collidePoint(int x, int y, SDL_Rect* b) {
            // aR < bL
            if ((x) < (b->x)) {return false;}
            // aL > bR
            if ((x) > (b->x + b->w)) {return false;}
            return true;
        }

        // checks for intersection between rectangles a, offset by x and y, and b
        static bool placeMeeting(SDL_Rect* a, SDL_Rect* b, int x_off, int y_off) {
            // aR < bL
            if ((a->x + x_off + a->w) < (b->x)) {
                return false;
            }
            // aL > bR
            if ((a->x + x_off) > (b->x + b->w)) {
                return false;
            }
            // aB < aT
            if ((a->y + y_off + a->h) < (b->y)) {
                return false;
            }
            // aT > yB
            if ((a->y + y_off) > (b->y + b->h)) {
                return false;
            }
            return true;
        }
        
};