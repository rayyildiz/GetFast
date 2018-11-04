#ifndef URL_H
#define URL_H

#include "defs.h"
#include "data.h"
#include "misc.h"
#include "head.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

extern BOOL download_url(struct url_info*);
extern BOOL parse_file(struct url_info*);

#endif
