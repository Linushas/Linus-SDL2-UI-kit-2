#ifndef CARD_H
#define CARD_H

#include <stdbool.h>
#include "../SDL_include.h"

typedef struct Card *Card;

Card createCard();
void card_setAppearance(Card c, SDL_Rect rect, SDL_Color bg, SDL_Color border_color);
void card_setBGColor(Card c, SDL_Color bg);
void card_setBorderColor(Card c, SDL_Color border_color);

void card_setRect(Card c, SDL_Rect rect);
SDL_Rect card_getRect(Card c);

void card_render(SDL_Renderer *rend, Card c);
int destroyCard(Card c);

#endif