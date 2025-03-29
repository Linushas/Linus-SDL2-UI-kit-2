#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "label.h"

typedef struct Label {
        TTF_Font *font;
        SDL_Texture *font_texture;
        SDL_Color fg;
        SDL_Rect rect;
        char text[1024];
} *Label;

Label createLabel() {
        Label lbl = malloc(sizeof(struct Label));
        if (lbl == NULL) {
            printf("Error: Failed to allocate memory for Label.\n");
            return NULL;
        }
    
        lbl->fg = (SDL_Color){255, 255, 255, 255};
        lbl->rect = (SDL_Rect){0, 0, 0, 0};
        strcpy(lbl->text, "label");
        lbl->font = NULL;
        lbl->font_texture = NULL;
    
        return lbl;
}
    

void label_setText(Label lbl, char *text) {
        strcpy(lbl->text, text);
}

void label_setAppearance(SDL_Renderer *rend, Label lbl, int x, int y, SDL_Color fg, TTF_Font *font) {
        lbl->rect.x = x;
        lbl->rect.y = y;
        lbl->fg = fg;
        lbl->font = font;
        label_refreshTextures(rend, lbl);
}

void label_setColor(Label lbl, SDL_Color fg) {
        lbl->fg = fg;
}

void label_setFont(Label lbl, TTF_Font *font) {
        lbl->font = font;
}

void label_refreshTextures(SDL_Renderer *rend, Label lbl) {
        if (lbl == NULL || rend == NULL) return;

        if (lbl->font_texture != NULL) SDL_DestroyTexture(lbl->font_texture);
    
        SDL_Surface *surf = TTF_RenderText_Blended(lbl->font, lbl->text, lbl->fg);
        if (surf == NULL) {
            printf("Error: Failed to create text surface for Label.\n");
            return;
        }
        
        lbl->font_texture = SDL_CreateTextureFromSurface(rend, surf);
        SDL_FreeSurface(surf);
    
        if (lbl->font_texture == NULL) {
            printf("Error: Failed to create texture from text surface.\n");
            return;
        }
    
        lbl->rect.w = surf->w;
        lbl->rect.h = surf->h;
}
    

void label_setPosition(Label lbl, int x, int y) {
        lbl->rect.x = x;
        lbl->rect.y = y;
}

SDL_Rect label_getRect(Label lbl) {
        return lbl->rect;
}

void label_render(SDL_Renderer *rend, Label lbl) {
        SDL_RenderCopy(rend, lbl->font_texture, NULL, &lbl->rect);
}

int destroyLabel(Label lbl) {
        if (lbl == NULL) {
            printf("Error: Attempting to destroy a NULL Label.\n");
            return false;
        }
    
        if (lbl->font_texture != NULL) {
            SDL_DestroyTexture(lbl->font_texture);
            lbl->font_texture = NULL;
        }
    
        free(lbl);
        return true;
}
    