#include <stdio.h>
#include <stdlib.h>
#include "card.h"

typedef struct Card {
        SDL_Color bg, border_color;
        SDL_Rect rect;
} *Card;

Card createCard() {
        Card c = malloc(sizeof(struct Card));
        if (c == NULL) {
                printf("Error: Failed to allocate memory for Card.\n");
                return NULL;
        }

        return c;
}

void card_setAppearance(Card c, SDL_Rect rect, SDL_Color bg, SDL_Color border_color) {
        c->bg = bg;
        c->border_color = border_color;
        c->rect = rect;
}

void card_setBGColor(Card c, SDL_Color bg) {
        c->bg = bg;
}

void card_setBorderColor(Card c, SDL_Color border_color) {
        c->border_color = border_color;
}

void card_setRect(Card c, SDL_Rect rect) {
        c->rect = rect;
}

SDL_Rect card_getRect(Card c) {
        return c->rect;
}

void card_render(SDL_Renderer *rend, Card c) {
        SDL_SetRenderDrawColor(rend, c->bg.r, c->bg.g, c->bg.b, c->bg.a);
        SDL_RenderFillRect(rend, &c->rect);
        SDL_SetRenderDrawColor(rend, c->border_color.r, c->border_color.g, c->border_color.b, c->border_color.a);
        SDL_RenderDrawRect(rend, &c->rect);
}

int destroyCard(Card c) {
        if (c == NULL) {
                printf("Error: Attempting to destroy a NULL Card.\n");
                return false;
        }

        free(c);
        return true;
}