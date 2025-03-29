#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include "../SDL_include.h"

typedef struct Button *Button;

enum{BUTTON_NONE, BUTTON_HOVERED};

Button createButton();
void button_setText(Button b, char *text);
void button_setAppearance(SDL_Renderer *rend, Button b, SDL_Rect rect, SDL_Color bg, SDL_Color fg, TTF_Font *font);
int button_setColorsHovered(SDL_Renderer *rend, Button b, SDL_Color bg, SDL_Color fg);
void button_setBGColor(Button b, SDL_Color bg);
void button_setFGColor(Button b, SDL_Color fg);
void button_setFont(Button b, TTF_Font *font);
void button_refreshTextures(SDL_Renderer *rend, Button b);

void button_setRect(Button b, SDL_Rect rect);
SDL_Rect button_getRect(Button b);

int button_event(Button b, int mouse_x, int mouse_y); // 0: NONE , 1: HOVERED
void button_render(SDL_Renderer *rend, Button b);
int destroyButton(Button b);

#endif