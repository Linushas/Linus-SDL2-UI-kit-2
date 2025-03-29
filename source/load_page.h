#ifndef LOAD_PAGE_H
#define LOAD_PAGE_H
#include "app.h"

void parseXML(WM *wm, Panel panel, const char *filename);
void parseStyle(WM wm, Panel panel, const UIRes ui_res, const char *filename);

#endif