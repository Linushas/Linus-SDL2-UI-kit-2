#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "panel.h"
#include "button.h"
#include "label.h"
#include "card.h"

typedef struct Component {
        char *key;
        void *component;
        int type;
        bool hidden;
} Component;

typedef struct Panel {
        SDL_Rect rect;
        SDL_Color bg, border_color;
        bool hidden;

        TTF_Font *default_font;
        struct ChildComponents {
                int component_count;
                Component *component_list;
        };
} *Panel;

Panel createPanel() {
        Panel panel = malloc(sizeof(struct Panel));
        if (panel == NULL) {
                printf("Failed to allocate memory for Panel\n");
                return NULL;
        }

        panel->rect = (SDL_Rect){.x=0, .y=0, .w=1, .h=1};
        panel->bg = (SDL_Color){.r=0, .g=0, .b=0, .a=0};
        panel->border_color = (SDL_Color){.r=0, .g=0, .b=0, .a=0};;
        panel->component_count = 0;
        panel->component_list = NULL;
        panel->hidden = false;
        panel->default_font = TTF_OpenFont("res/Montserrat-Regular.ttf", 16);

        return panel;
}

void panel_newComponent(Panel p, int type, char *key) {
        if (p == NULL) {
                printf("Error: Attempting to add component to a NULL Panel.\n");
                return;
        }

        switch (type) {
                case COMPONENT_BUTTON:
                        Button b = createButton();
                        panel_addComponent(p, type, b, key);
                        break;
                case COMPONENT_LABEL:
                        Label lbl = createLabel();
                        panel_addComponent(p, type, lbl, key);
                        break;
                case COMPONENT_CARD:
                        Card c = createCard();
                        panel_addComponent(p, type, c, key);
                        break;
                default:
                        printf("Warning: Unknown component type (%d) in panel_newComponent.\n", type);
                        break;
        }
}

void panel_newLazyComponent(SDL_Renderer *rend, Panel p, char *text, int x, int y, int type, char *key) {
        if (p == NULL) {
                printf("Error: Attempting to add component to a NULL Panel.\n");
                return;
        }
        SDL_Color bg = {220, 220, 220};
        SDL_Color fg = {0,0,0};

        switch (type) {
                case COMPONENT_BUTTON:
                        Button b = createButton();
                        panel_addComponent(p, type, b, key);
                        button_setText(b, text);
                        button_setAppearance(rend, b, (SDL_Rect){.x=x, .y=y, .w=150, .h=30}, bg, fg, p->default_font);
                        break;
                case COMPONENT_LABEL:
                        Label lbl = createLabel();
                        panel_addComponent(p, type, lbl, key);
                        label_setText(lbl, text);
                        label_setAppearance(rend, lbl, x, y, fg, p->default_font);
                        break;
                case COMPONENT_CARD:
                        Card c = createCard();
                        panel_addComponent(p, type, c, key);
                        card_setAppearance(c, (SDL_Rect){.x=x, .y=y, .w=100, .h=100}, bg, fg);
                        break;
                default:
                        printf("Warning: Unknown component type (%d) in panel_newComponent.\n", type);
                        break;
        }
}

void panel_setAppearance(Panel p, SDL_Rect rect, SDL_Color bg, SDL_Color border_color) {
        p->rect = rect;
        p->bg = bg;
        p->border_color = border_color;
}

void panel_hide(Panel p, bool hide) {
        p->hidden = hide;
}

int panel_addComponent(Panel p, int type, void *component, char *key) {
        p->component_count++;

        p->component_list = realloc(p->component_list, p->component_count * sizeof(Component));
        if (p->component_list == NULL) {
                printf("Failed to reallocate memory for component list : panel\n");
                return false; 
        }

        p->component_list[p->component_count - 1].key = malloc(strlen(key) + 1);
        if (p->component_list[p->component_count - 1].key == NULL) {
                printf("Failed to allocate memory for component key\n");
                return false;
        }
        strcpy(p->component_list[p->component_count - 1].key, key);

        p->component_list[p->component_count - 1].component = component;
        p->component_list[p->component_count - 1].type = type;
        p->component_list[p->component_count - 1].hidden = false;

        return true;
}

void panel_setRect(Panel p, SDL_Rect rect) {
        p->rect = rect;
}

int panel_update(SDL_Renderer *rend, Panel p, UI_Event *ui_event, bool is_mouse_down) {
        if(p->hidden) return ui_event->event_type = -1;
        ui_event->item_idx = -1;

        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);

        static bool was_mouse_down = false;
        
        if (p == NULL) {
                printf("Error: Attempting to render a NULL Panel.\n");
                return ui_event->event_type = -1;
        }

        for (int i = 0; i < p->component_count; i++) {
                Component comp = p->component_list[i];
                if(comp.hidden) continue;

                switch (comp.type) {
                        case COMPONENT_BUTTON:
                                if(button_event((Button)comp.component, mouse_x, mouse_y) && (was_mouse_down && !is_mouse_down)) {
                                        strcpy(ui_event->component_key, comp.key);
                                        if(button_hasLink((Button)comp.component)) {
                                                char link[256];
                                                button_getLink((Button)comp.component, link);
                                                strcpy(ui_event->link, link);
                                        }
                                        
                                        was_mouse_down = false;
                                        return ui_event->event_type = BUTTON_CLICKED;
                                }
                                break;
                        case COMPONENT_LABEL:
                                break;
                        case COMPONENT_CARD:
                                break;

                        default:
                                printf("Warning: Unknown component type (%d) in panel_update.\n", comp.type);
                                break;
                }
        }
        was_mouse_down = is_mouse_down;
        return ui_event->event_type = -1;
}

void *panel_getComponent(Panel p, char *key) {
        for(int i = 0; i < p->component_count; i++) {
                Component comp = p->component_list[i];
                if(strcmp(key, comp.key) == 0) {
                        return comp.component;
                }
        }
        return NULL;
}

void panel_render(SDL_Renderer *rend, Panel p) {
        if(p->hidden) return;
        if (p == NULL) {
            printf("Error: Attempting to render a NULL Panel.\n");
            return;
        }
    
        SDL_SetRenderDrawColor(rend, p->bg.r, p->bg.g, p->bg.b, p->bg.a);
        SDL_RenderFillRect(rend, &p->rect);
    
        SDL_SetRenderDrawColor(rend, p->border_color.r, p->border_color.g, p->border_color.b, p->border_color.a);
        SDL_RenderDrawRect(rend, &p->rect);
    
        for (int i = 0; i < p->component_count; i++) {
            Component comp = p->component_list[i];
            if(comp.hidden) continue;

            switch (comp.type) {
                        case COMPONENT_BUTTON:
                                button_render(rend, (Button)comp.component);
                                break;
                        case COMPONENT_LABEL:
                                label_render(rend, (Label)comp.component);
                                break;
                        case COMPONENT_CARD:
                                card_render(rend, (Card)comp.component);
                                break;

                        default:
                                printf("Warning: Unknown component type (%d) in panel_render.\n", comp.type);
                                break;
            }
        }
}

void panel_hideComponent(Panel p, char *key, bool hide) {
        for(int i = 0; i < p->component_count; i++) {
                if(strcmp(key, p->component_list[i].key) == 0) {
                        p->component_list[i].hidden = hide;
                        return;
                }
        }
}

int destroyPanel(Panel p) {
        if (p == NULL) return -1;

        panel_destroyAllComponents(p);

        if (p->default_font != NULL) {
                TTF_CloseFont(p->default_font);
                p->default_font = NULL;
        }

        if (p->component_list != NULL) {
                free(p->component_list);
                p->component_list = NULL;
        }

        free(p);
        p = NULL;

        return 0;
}

void panel_setBGColor(Panel p, SDL_Color bg) {
        p->bg = bg;
}

void panel_setBorderColor(Panel p, SDL_Color border_color) {
        p->border_color = border_color;
}

SDL_Rect panel_getRect(Panel p) {
        return p->rect;
}

bool panel_isHidden(Panel p) {
        return p->hidden;
}

int panel_getComponentCount(Panel p) {
        return p->component_count;
}

int panel_getComponentType(Panel p, char *key) {
        for(int i = 0; i < p->component_count; i++) {
                Component comp = p->component_list[i];
                if(strcmp(key, comp.key) == 0) {
                        return comp.type;
                }
        }
        return -1;
}

bool panel_isComponentHidden(Panel p, char *key) {
        for(int i = 0; i < p->component_count; i++) {
                Component comp = p->component_list[i];
                if(strcmp(key, comp.key) == 0) {
                        return comp.hidden;
                }
        }
        return false;
}

void panel_destroyAllComponents(Panel p) {
        for (int i = 0; i < p->component_count; i++) {
            if (p->component_list[i].key != NULL) {
                free(p->component_list[i].key);
                p->component_list[i].key = NULL;
            }
    
            switch (p->component_list[i].type) {
                case COMPONENT_BUTTON:
                    destroyButton((Button)p->component_list[i].component);
                    break;
                case COMPONENT_LABEL:
                    destroyLabel((Label)p->component_list[i].component);
                    break;
                case COMPONENT_CARD:
                    destroyCard((Card)p->component_list[i].component);
                    break;
                default:
                    printf("Warning: Unknown component type (%d) in panel_destroyAllComponents.\n", 
                           p->component_list[i].type);
                    break;
            }
        }
    
        free(p->component_list);
        p->component_list = NULL;
    
        p->component_count = 0;
    }