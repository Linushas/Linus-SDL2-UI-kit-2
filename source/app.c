#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "app.h"
#include "load_page.h"

int eventHandler(WM *wm) {
        static SDL_Event event;
        wm->resized = false;

        while(SDL_PollEvent(&event)) {
                switch(event.type) {
                        case SDL_QUIT:
                                wm->is_running = false;
                                break;

                        case SDL_MOUSEBUTTONDOWN:
                                if(event.button.button == SDL_BUTTON_LEFT)
                                        wm->is_mouse_down = true;
                                break;
                        case SDL_MOUSEBUTTONUP:
                                if(event.button.button == SDL_BUTTON_LEFT)
                                        wm->is_mouse_down = false;
                                break;

                        case SDL_WINDOWEVENT:
                                if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                                        wm->resized = true;
                                        SDL_GetWindowSize(wm->win, &wm->w, &wm->h);
                                }
                                break;
                }
        }
}

int UI_Init(UIRes *res) {
        res->russo[2] = TTF_OpenFont("res/RussoOne-Regular.ttf", 24);
        res->russo[1] = TTF_OpenFont("res/RussoOne-Regular.ttf", 16);
        res->russo[0] = TTF_OpenFont("res/RussoOne-Regular.ttf", 12);

        res->montserrat[2] = TTF_OpenFont("res/Montserrat-Regular.ttf", 24);
        res->montserrat[1] = TTF_OpenFont("res/Montserrat-Regular.ttf", 16);
        res->montserrat[0] = TTF_OpenFont("res/Montserrat-Regular.ttf", 12);

        res->color[BLACK] =             createColor(0, 0, 0, 255);
        res->color[WHITE] =             createColor(255, 255, 255, 255);
        res->color[RED] =               createColor(255, 0, 0, 255);
        res->color[GREEN] =             createColor(0, 255, 0, 255);
        res->color[BLUE] =              createColor(0, 0, 255, 255);
        res->color[SPANISH_BLUE] =      createColor(0, 112, 184, 255);
        res->color[GREY] =              createColor(160, 160, 160, 255);
        res->color[CHARCOAL] =          createColor(51, 51, 51, 255);
        res->color[DIM_GREY] =          createColor(105, 105, 105, 255);
        res->color[RAISIN_BLACK] =      createColor(36, 33, 36, 255);
        res->color[SLATE_GREY]=         createColor(112, 128, 144, 255);
        return 1;
}

void applyStyling(const WM wm, Panel panel, const UIRes ui_res) {
        parseStyle(wm, panel, ui_res, "page/style.toml");
        Button b = panel_getComponent(panel, "path_bar");
        button_setAppearance(wm.rend, b, createRect(0, 0, wm.w, 20), ui_res.color[WHITE], ui_res.color[BLACK], ui_res.montserrat[0]);
        button_setColorsHovered(wm.rend, b, ui_res.color[WHITE], ui_res.color[BLACK]);
        button_refreshTextures(wm.rend, b);
}

void render(SDL_Renderer *rend, Panel panel) {
        SDL_SetRenderDrawColor(rend, 50,50,100,0);
        SDL_RenderClear(rend);
        panel_render(rend, panel);
        SDL_RenderPresent(rend);
}

void switchPage(WM *wm, Panel panel, UIRes *ui_res, char *filename) {
        panel_destroyAllComponents(panel);
        parseXML(wm, panel, filename);
        panel_newLazyComponent(wm->rend, panel, filename, 0, 0, COMPONENT_BUTTON, "path_bar");
        applyStyling(*wm, panel, *ui_res);
}

void UI_eventHandler(WM *wm, Panel panel, UI_Event *ui_event, UIRes *ui_res) {
        if(wm->resized) {
                applyStyling(*wm, panel, *ui_res);
        }

        panel_update(wm->rend, panel, ui_event, wm->is_mouse_down);
        switch(ui_event->event_type) {
                case BUTTON_CLICKED:
                        Button b = panel_getComponent(panel, ui_event->component_key);
                        if(button_hasLink(b)) {
                                char link[256];
                                button_getLink(b, link);
                                printf("link: %s\n", link);
                                switchPage(wm, panel, ui_res, link);
                        }
                        break;
        }
}

SDL_Rect createRect(int x, int y, int w, int h) {
        SDL_Rect r = {x,y,w,h};
        return r;
}

SDL_Color createColor(int r, int g, int b, int a) {
        SDL_Color c = {r,g,b,a};
        return c;
}