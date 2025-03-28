#ifndef UI_EXTRA_H
#define UI_EXTRA_H

#include "SDL_include.h"
#include "lib/LinUI.h"

enum Colors{GREY, BLACK, WHITE, RED, GREEN, BLUE, DIM_GREY, SLATE_GREY, RAISIN_BLACK, CHARCOAL, SPANISH_BLUE};

typedef struct windowModel {
        SDL_Window *win;
        SDL_Renderer *rend;
        bool is_running;
        int w, h;
        bool resized;

        int mouse_x, mouse_y;
        bool is_mouse_down;
} WM;

typedef struct UIResources {
        TTF_Font *montserrat[3];
        TTF_Font *russo[3];
        SDL_Color color[12];
} UIRes;

int UI_Init(UIRes *res);
void UI_eventHandler(WM *wm, Panel panel, UI_Event *ui_event, UIRes *ui_res);
int eventHandler(WM *wm);

void render(SDL_Renderer *rend, Panel panel);

void applyStyling(const WM wm, Panel panel, const UIRes ui_res);

SDL_Rect createRect(int x, int y, int w, int h);
SDL_Color createColor(int r, int g, int b, int a);

#endif