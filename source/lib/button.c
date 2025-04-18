#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "button.h"

typedef struct Button {
        TTF_Font *font;
        SDL_Texture *font_texture, *font_hov_texture;
        SDL_Color bg, fg, hov_bg, hov_fg;
        SDL_Rect box_rect, text_rect;
        bool is_hovered;
        // void (*clicked)(Button);
        char link[256];
        bool has_link;
        char text[256];
} *Button;

void button_setText(Button b, char *text) {
        strcpy(b->text, text);
}

void button_setAppearance(SDL_Renderer *rend, Button b, SDL_Rect rect, SDL_Color bg, SDL_Color fg, TTF_Font *font) {
        b->font = font;
        b->bg = bg;
        b->fg = fg;
        b->hov_bg = fg;
        b->hov_fg = bg;
        b->box_rect = rect;

        button_refreshTextures(rend, b);
}

Button createButton() {
        Button b = malloc(sizeof(struct Button));
        if (b == NULL) {
                printf("Error: Failed to allocate memory for Button.\n");
                return NULL;
        }

        strcpy(b->text, "button");
        b->is_hovered = false;
        b->font = NULL;
        b->font_texture = NULL;
        b->font_hov_texture = NULL;
        b->has_link = false;

        return b;
}

void button_setLink(Button b, char *link) {
        strcpy(b->link, link);
        b->has_link = true;
}

int button_setColorsHovered(SDL_Renderer *rend, Button b, SDL_Color bg, SDL_Color fg) {
        if(b->font_hov_texture != b->font_texture) {
                SDL_DestroyTexture(b->font_hov_texture);
        }
        b->hov_bg = bg;
        b->hov_fg = fg;

        button_refreshTextures(rend, b);

        return true;
}

int button_event(Button b, int mouse_x, int mouse_y) {
        if(mouse_x <= (b->box_rect.x + b->box_rect.w) && mouse_x >= b->box_rect.x &&
           mouse_y <= (b->box_rect.y + b->box_rect.h) && mouse_y >= b->box_rect.y)
        {
                return b->is_hovered = BUTTON_HOVERED;
        } else {
                return b->is_hovered = BUTTON_NONE;
        }
}

void button_render(SDL_Renderer *rend, Button b) {
        if (b == NULL) {
                printf("Error: Attempting to render a NULL Button.\n");
                return;
        }
        if(b->is_hovered) {
                SDL_SetRenderDrawColor(rend, b->hov_bg.r, b->hov_bg.g, b->hov_bg.b, b->hov_bg.a);
                SDL_RenderFillRect(rend, &b->box_rect);
                SDL_RenderCopy(rend, b->font_hov_texture, NULL, &b->text_rect);
        } else {
                SDL_SetRenderDrawColor(rend, b->bg.r, b->bg.g, b->bg.b, b->bg.a);
                SDL_RenderFillRect(rend, &b->box_rect);
                SDL_RenderCopy(rend, b->font_texture, NULL, &b->text_rect);
        }
        
}

int destroyButton(Button b) {
        if (b == NULL) {
            printf("Error: Attempting to destroy a NULL Button.\n");
            return false;
        }
    
        if (b->font_texture != NULL) {
            SDL_DestroyTexture(b->font_texture);
            b->font_texture = NULL;
        }
        
        if (b->font_hov_texture != NULL) {
            SDL_DestroyTexture(b->font_hov_texture);
            b->font_hov_texture = NULL;
        }
    
        free(b);
        return true;
}

bool button_hasLink(Button b) {
        return b->has_link;
}

void button_getLink(Button b, char *link) {
        strcpy(link, b->link);
}
    
void button_setBGColor(Button b, SDL_Color bg) {
        b->bg = bg;
}

void button_setFGColor(Button b, SDL_Color fg) {
        b->fg = fg;
}

void button_setFont(Button b, TTF_Font *font) {
        b->font = font;
}

void button_refreshTextures(SDL_Renderer *rend, Button b) {
        if (b == NULL || rend == NULL || b->font == NULL) {
            printf("Error: Invalid arguments to button_refreshTextures.\n");
            return;
        }
    
        if (b->font_texture != NULL) {
            SDL_DestroyTexture(b->font_texture);
            b->font_texture = NULL;
        }
        if (b->font_hov_texture != NULL) {
            SDL_DestroyTexture(b->font_hov_texture);
            b->font_hov_texture = NULL;
        }
    
        SDL_Surface *surf = TTF_RenderText_Blended(b->font, b->text, b->fg);
        if (surf == NULL) {
            printf("Error: Failed to create text surface: %s\n", TTF_GetError());
            return;
        }
        b->font_texture = SDL_CreateTextureFromSurface(rend, surf);
        if (b->font_texture == NULL) {
            printf("Error: Failed to create texture from surface: %s\n", SDL_GetError());
            SDL_FreeSurface(surf);
            return;
        }
    
        b->text_rect.w = surf->w;
        b->text_rect.h = surf->h;
        b->text_rect.x = b->box_rect.x + (b->box_rect.w - surf->w) / 2;
        b->text_rect.y = b->box_rect.y + (b->box_rect.h - surf->h) / 2;
        SDL_FreeSurface(surf);
    
        surf = TTF_RenderText_Blended(b->font, b->text, b->hov_fg);
        if (surf == NULL) {
            printf("Error: Failed to create hover text surface: %s\n", TTF_GetError());
            return;
        }
        b->font_hov_texture = SDL_CreateTextureFromSurface(rend, surf);
        if (b->font_hov_texture == NULL) {
            printf("Error: Failed to create hover texture from surface: %s\n", SDL_GetError());
            SDL_FreeSurface(surf);
            return;
        }
    
        SDL_FreeSurface(surf);
    }
    

SDL_Rect button_getRect(Button b) {
        return b->box_rect;
}

void button_setRect(Button b, SDL_Rect rect) {
        b->box_rect = rect;
}