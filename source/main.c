#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "app.h"
#include "load_page.h"

void startUp(WM *wm, UIRes *ui_res);
void close(WM *wm, Panel panel, UIRes *ui_res);

int main(int argc, char **argv) {
        WM wm = {.is_running = true, .w = 600*16/9, .h = 600};
        UIRes ui_res;
        UI_Event ui_event;
        startUp(&wm, &ui_res);

        Panel panel = createPanel();
        parseXML(&wm, panel, "page/page.xml");
        applyStyling(wm, panel, ui_res);
        
        while(wm.is_running) {
                eventHandler(&wm);
                UI_eventHandler(&wm, panel, &ui_event, &ui_res);
                render(wm.rend, panel);
        }

        close(&wm, panel, &ui_res);
        return 0;
}

void startUp(WM *wm, UIRes *ui_res) {
        SDL_Init(SDL_INIT_EVERYTHING);
        TTF_Init();
        
        wm->win = SDL_CreateWindow("UI test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wm->w, wm->h, SDL_WINDOW_RESIZABLE);
        wm->rend = SDL_CreateRenderer(wm->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        
        UI_Init(ui_res);
}

void close(WM *wm, Panel panel, UIRes *ui_res) {
        for(int i = 0; i < 3; i++) {
                TTF_CloseFont(ui_res->russo[i]);
                TTF_CloseFont(ui_res->montserrat[i]);
        }

        destroyPanel(panel);
        SDL_DestroyRenderer(wm->rend);
        SDL_DestroyWindow(wm->win);
        TTF_Quit();
        SDL_Quit();
}