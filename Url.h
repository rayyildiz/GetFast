#ifndef URL_H
#define URL_H

#include "Defs.h"
#include "Data.h"
#include "Misc.h"
#include "Head.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

extern BOOL download_url(struct url_info*);
extern BOOL parse_file(struct url_info*);

#endif
