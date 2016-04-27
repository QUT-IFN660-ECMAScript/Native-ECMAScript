//
// Created by Freeman on 21/03/2016.
//

#include <stdio.h>
#include "y.tab.h"
#include <string.h>
#include <stdlib.h>


void yyerror(const char *s) {
    fprintf(stderr, "Parse Error:\n%s\n", s);
}

char* dynamic_strcat(char* str, char* s2) {
    int len;
    char *s;
    if (str != NULL)
        len = strlen(str);
    len += strlen(s2) + 1 * sizeof(*s2);
    s = (char*) realloc(str, len);
    strcat(s, s2);
    return s;
}
