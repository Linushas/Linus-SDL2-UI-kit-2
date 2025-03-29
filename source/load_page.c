#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include "load_page.h"

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
    
                        // Create the component
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
    
        printf("Finished parsing XML.\n");
        xmlFreeDoc(doc);
}