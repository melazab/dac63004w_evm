#ifndef SIGNAL_UTILS_H
#define SIGNAL_UTILS_H

#include <stdbool.h>

extern volatile bool running;

void setup_signal_handler(void);

#endif // SIGNAL_UTILS_H
