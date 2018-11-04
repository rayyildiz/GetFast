#ifndef FILE_H
#define FILE_H

#include "defs.h"
#include "data.h"
#include "misc.h"
#include "head.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

extern BOOL download_file(struct url_info*,int noofthreads);

#endif
