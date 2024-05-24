#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>

#include "prompt.h"
#include "warp.h"
#include "peek.h"
#include "seek.h"
#include "pastevents.h"
#include "sleep.h"
#include "proclore.h"
#include "foreback.h"

#define COLOR_RESET "\x1B[0m"
#define COLOR_GREEN "\x1B[32m" // Green for executables
#define COLOR_WHITE "\x1B[37m" // White for files
#define COLOR_BLUE "\x1B[34m"  // Blue for directories
#define COLOR_YELLOW "\x1B[0;33m"

#endif