#ifndef MISC_H
#define MISH_H

#include "Defs.h"
#include "Data.h"
#include <stdio.h>
#include <string.h>

extern void reverse_string(char *str);
extern BOOL parse_url(struct url_info* url,char *h);
extern BOOL parse_string(char* out,char* atr);
extern void usage();
extern void progress_bar(float cur,float tot);

#endif
