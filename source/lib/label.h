#ifndef LABEL_H
#define LABEL_H

#include <stdbool.h>
#include "../SDL_include.h"

typedef struct Label *Label;

Label createLabel();
void label_setText(Label lbl, char *text);
void label_setAppearance(SDL_Renderer *rend, Label lbl, int x, int y, SDL_Color fg, TTF_Font *font);
void label_setColor(Label lbl, SDL_Color fg);
void label_setFont(Label lbl, TTF_Font *font);
void label_refreshTextures(SDL_Renderer *rend, Label lbl);
void label_setPosition(Label lbl, int x, int y);

SDL_Rect label_getRect(Label lbl);

void label_render(SDL_Renderer *rend, Label lbl);
int destroyLabel(Label lbl);

#endif