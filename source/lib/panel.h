#ifndef PANEL_H
#define PANEL_H

#include <stdbool.h>
#include "../SDL_include.h"

enum ComponentType{COMPONENT_BUTTON, COMPONENT_SLIDER, COMPONENT_CHECKLIST, COMPONENT_TEXT_INPUT_FIELD, COMPONENT_DROPDOWN_MENU, COMPONENT_LABEL};
enum PanelEvent{BUTTON_CLICKED, CHECKLIST_UPDATED, SLIDER_UPDATED, TEXT_INPUT, DROPDOWN_ITEM_CLICKED};

typedef struct Panel *Panel;

typedef struct UI_Event {
        int event_type;
        char component_key[256];
        int item_idx;
} UI_Event;

Panel createPanel();
void panel_render(SDL_Renderer *rend, Panel p);
int panel_update(SDL_Renderer *rend, Panel p, UI_Event *ui_event, bool is_mouse_down);
int destroyPanel(Panel p);

void panel_hide(Panel p, bool hide);
void panel_setRect(Panel p, SDL_Rect rect);
void panel_setAppearance(Panel p, SDL_Rect rect, SDL_Color bg, SDL_Color border_color);
void panel_setBGColor(Panel p, SDL_Color bg);
void panel_setBorderColor(Panel p, SDL_Color border_color);

SDL_Rect panel_getRect(Panel p);
bool panel_isHidden(Panel p);
int panel_getComponentCount(Panel p);

void panel_newComponent(Panel p, int type, char *key);
void panel_newLazyComponent(SDL_Renderer *rend, Panel p, char *text, int x, int y, int type, char *key);
int panel_addComponent(Panel p, int type, void *component, char *key);
void *panel_getComponent(Panel p, char *key);
int panel_getComponentType(Panel p, char *key);
bool panel_isComponentHidden(Panel p, char *key);
void panel_destroyAllComponents(Panel p);
void panel_hideComponent(Panel p, char *key, bool hide);

#endif