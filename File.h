#ifndef FILE_H
#define FILE_H

#include "Defs.h"
#include "Data.h"
#include "Misc.h"
#include "Head.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

extern BOOL download_file(struct url_info*,int noofthreads);

#endif
