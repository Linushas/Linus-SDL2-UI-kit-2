#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include "load_page.h"
#include "lib/tomlc99/toml.h"

SDL_Color getColorFromName(const char *name, const UIRes ui_res);
TTF_Font* getFontFromName(const char *font_name, const UIRes ui_res);

void parseXML(WM *wm, Panel panel, const char *filename) {    
        xmlDoc *doc = xmlReadFile(filename, NULL, 0);
        if (doc == NULL) {
            fprintf(stderr, "Failed to parse %s\n", filename);
            return;
        }
    
        xmlNode *root = xmlDocGetRootElement(doc);
        if (root == NULL) {
            fprintf(stderr, "Empty document\n");
            xmlFreeDoc(doc);
            return;
        }
    
        int padding = 10;
        int component_height = 35;
        int y_offset = padding;
    
        xmlNode *panel_node = NULL;
        for (panel_node = root->children; panel_node; panel_node = panel_node->next) {
            if (panel_node->type == XML_ELEMENT_NODE && xmlStrcmp(panel_node->name, BAD_CAST "panel") == 0) {
    
                xmlNode *component_node = NULL;
                for (component_node = panel_node->children; component_node; component_node = component_node->next) {
                    if (component_node->type == XML_ELEMENT_NODE) {
    
                        char *type = (char *)component_node->name;
                        xmlChar *key = xmlGetProp(component_node, BAD_CAST "key");
                        xmlChar *content = xmlNodeGetContent(component_node);
    
                        if (!key || !content) {
                            fprintf(stderr, "Error: Missing key or content in component node.\n");
                            if (key) xmlFree(key);
                            if (content) xmlFree(content);
                            continue;
                        }
    
                        int component_type = -1;
                        if (strcmp(type, "label") == 0) {
                            component_type = COMPONENT_LABEL;
                        } else if (strcmp(type, "button") == 0) {
                            component_type = COMPONENT_BUTTON;
                        } else {
                            fprintf(stderr, "Unknown component type: %s\n", type);
                            xmlFree(key);
                            xmlFree(content);
                            continue;
                        }
    
                        char *key_copy = strdup((const char *)key);
                        char *content_copy = strdup((const char *)content);
    
                        if (!key_copy || !content_copy) {
                            fprintf(stderr, "Memory allocation failed for key or content.\n");
                            free(key_copy);
                            free(content_copy);
                            xmlFree(key);
                            xmlFree(content);
                            continue;
                        }
    
                        int x = padding;
                        int y = y_offset;
                        panel_newLazyComponent(wm->rend, panel, content_copy, x, y, component_type, key_copy);
    
                        free(key_copy);
                        free(content_copy);
    
                        y_offset += component_height;
    
                        xmlFree(key);
                        xmlFree(content);
                    }
                }
            }
        }
    
        xmlFreeDoc(doc);
}

void parseStyle(WM wm, Panel panel, const UIRes ui_res, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Failed to open %s\n", filename);
        return;
    }

    char errbuf[200];
    toml_table_t *root = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);

    if (!root) {
        fprintf(stderr, "Error parsing TOML file: %s\n", errbuf);
        return;
    }

    toml_table_t *panel_table = toml_table_in(root, "main");
    if (panel_table) {
        const char *x = toml_raw_in(panel_table, "x");
        const char *y = toml_raw_in(panel_table, "y");
        const char *w = toml_raw_in(panel_table, "w");
        const char *h = toml_raw_in(panel_table, "h");
        const char *bg = toml_raw_in(panel_table, "bg");
        const char *border_color = toml_raw_in(panel_table, "border_color");

        if (x && y && w && h) {
            int width = strcmp(w, "\"WINDOW_WIDTH\"") == 0 ? 
                        wm.w : atoi(w);
            int height = strcmp(h, "\"WINDOW_HEIGHT\"") == 0 ? 
                         wm.h : atoi(h);
            panel_setRect(panel, (SDL_Rect){.x=atoi(x), .y=atoi(y), .w=width, .h=height});
        }

        if (bg) panel_setBGColor(panel, getColorFromName(bg, ui_res));
        if (border_color) panel_setBorderColor(panel, getColorFromName(border_color, ui_res));
    }

    for (int i = 0; ; i++) {
        const char *comp_key = toml_key_in(root, i);
        if (!comp_key) break;
        if (strcmp(comp_key, "main") == 0) continue;

        toml_table_t *comp = toml_table_in(root, comp_key);
        if (!comp) continue;

        const char *x = toml_raw_in(comp, "x");
        const char *y = toml_raw_in(comp, "y");
        const char *w = toml_raw_in(comp, "w");
        const char *h = toml_raw_in(comp, "h");
        const char *bg = toml_raw_in(comp, "bg");
        const char *fg = toml_raw_in(comp, "fg");
        const char *hov_bg = toml_raw_in(comp, "hov_bg");
        const char *hov_fg = toml_raw_in(comp, "hov_fg");
        const char *font = toml_raw_in(comp, "font");

        int type = panel_getComponentType(panel, (char *)comp_key);
        switch(type) {
            case COMPONENT_BUTTON: {
                Button b = panel_getComponent(panel, (char *)comp_key);
                if (!b) break;

                if (x && y && w && h) {
                    SDL_Rect rect = {
                        .x = atoi(x),
                        .y = atoi(y),
                        .w = atoi(w),
                        .h = atoi(h)
                    };
                    button_setRect(b, rect);
                }

                if (bg) button_setBGColor(b, getColorFromName(bg, ui_res));
                if (fg) button_setFGColor(b, getColorFromName(fg, ui_res));
                if (font) button_setFont(b, getFontFromName(font, ui_res));

                if(hov_bg && hov_fg) {
                    button_setColorsHovered(wm.rend, b, getColorFromName(hov_bg, ui_res), getColorFromName(hov_fg, ui_res));
                }

                button_refreshTextures(wm.rend, b);
                break;
            }

            case COMPONENT_LABEL:
                Label lbl = panel_getComponent(panel, (char *)comp_key);
                if (!lbl) break;

                if (x && y) {
                    label_setPosition(lbl, atoi(x), atoi(y));
                }

                if (fg) label_setColor(lbl, getColorFromName(fg, ui_res));
                if (font) label_setFont(lbl, getFontFromName(font, ui_res));

                label_refreshTextures(wm.rend, lbl);
                break;

            default:
                // fprintf(stderr, "Unknown component type: %s\n", comp_key);
                break;
        }
    }

    toml_free(root);
}

SDL_Color getColorFromName(const char *name, const UIRes ui_res) {
    char stripped_name[256];
    int j = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        if (name[i] == '"' || name[i] == ' ' || name[i] == '\t') {
            continue;
        }
        stripped_name[j++] = name[i];
    }
    stripped_name[j] = '\0';

    if (strcmp(stripped_name, "BLACK") == 0) {
        return ui_res.color[BLACK];
    } else if (strcmp(stripped_name, "WHITE") == 0) {
        return ui_res.color[WHITE];
    } else if (strcmp(stripped_name, "RED") == 0) {
        return ui_res.color[RED];
    } else if (strcmp(stripped_name, "BLUE") == 0) {
        return ui_res.color[BLUE];
    } else if (strcmp(stripped_name, "GREEN") == 0) {
        return ui_res.color[GREEN];
    } else if (strcmp(stripped_name, "GREY") == 0) {
        return ui_res.color[GREY];
    } else if (strcmp(stripped_name, "DIM_GREY") == 0) {
        return ui_res.color[DIM_GREY];
    } else if (strcmp(stripped_name, "SLATE_GREY") == 0) {
        return ui_res.color[SLATE_GREY];
    } else if (strcmp(stripped_name, "RAISIN_BLACK") == 0) {
        return ui_res.color[RAISIN_BLACK];
    } else if (strcmp(stripped_name, "CHARCOAL") == 0) {
        return ui_res.color[CHARCOAL];
    } else if (strcmp(stripped_name, "SPANISH_BLUE") == 0) {
        return ui_res.color[SPANISH_BLUE];
    }
    return ui_res.color[WHITE];
}

TTF_Font* getFontFromName(const char *font_name, const UIRes ui_res) {
    char stripped_name[256];
    int j = 0;
    for (int i = 0; font_name[i] != '\0'; i++) {
        if (font_name[i] == '"' || font_name[i] == ' ' || font_name[i] == '\t') {
            continue; 
        }
        stripped_name[j++] = font_name[i];
    }
    stripped_name[j] = '\0';

    if (strcmp(stripped_name, "montserrat,small") == 0) {
        return ui_res.montserrat[0];
    } else if (strcmp(stripped_name, "montserrat,medium") == 0) {
        return ui_res.montserrat[1];
    } else if (strcmp(stripped_name, "montserrat,big") == 0) {
        return ui_res.montserrat[2];
    }

    return ui_res.montserrat[0]; 
}