#ifndef HEAD_H
#define HEAD_H

#include "data.h"

/** Get file length from url
  If there is an error, then `-1` will be returned */
extern long head_get_file_length(struct url_info* url);

#endif
