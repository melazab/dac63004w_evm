#include "utils/signal_utils.h"
#include <signal.h>
#include <stdio.h>

volatile bool running = true;

void signal_handler(int signum) {
  (void)signum; // Unused parameter
  running = false;
  printf("\nReceived shutdown signal. Cleaning up...\n");
}

void setup_signal_handler(void) {
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);
}
