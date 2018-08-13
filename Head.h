#ifndef HEAD_H
#define HEAD_H

#include "Data.h"

/** Get file length from url
  If there were some problem -1 will be returned */
extern long head_get_file_length(struct url_info* url);

#endif
