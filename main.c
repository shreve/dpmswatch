/*

  DPMS Watch
  Copyright 2017 Jacob Shreve

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Relevant X11 libraries
// Requires compiling with -lX11 and -lXext
#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/extensions/dpms.h>

void usage() {
  printf("Usage: dpmswatch [-n onCmd] [-f offcmd]\nYou must supply at least one command.\n");
  exit(1);
}

void runCommand(char *command) {
  int result = system(command);
  if (result != 0) {
    printf("Command `%s` exited with non-zero status: %i", command, result);
  }
}

// Fetch and validate the arguments supplied
void initArgs(int argc, char *argv[], char *on, char *off) {
  // You need to have at least one argument (other than progname)
  if (argc == 1) { usage(); }

  int c;

  // Save the strings passed in for -n and -f
  while ((c = getopt(argc, argv, "n:f:")) != -1) {
    switch(c) {
    case 'n': strcpy(on, optarg); break;
    case 'f': strcpy(off, optarg); break;
    default: usage();
    }
  }

  // If both commands are blank strings, it's not valid.
  if (on[0] == '\0' && off[0] == '\0') { usage(); }
}

int main(int argc, char *argv[]) {
  char onCmd[200] = "";
  char offCmd[200] = "";

  initArgs(argc, argv, onCmd, offCmd);

  // Initialize X11 variables.
  Display *dpy = XOpenDisplay(NULL);
  BOOL noop;
  CARD16 state;
  Bool onoff;
  Bool last;
  Bool firstLoop = true;

  for(;;) {
    // Fetch information for the current string
    DPMSInfo(dpy, &state, &noop);

    // The native onoff doesn't do what I want.
    // If the state is anything other than DPMSModeOn, consider the screen off.
    onoff = (state == DPMSModeOn);

    // If the last value isn't undefined and isn't this value, a change has
    // happened, and we should trigger the corresponding function.
    if (!firstLoop && last != onoff) {
      if (onoff) {
        runCommand(onCmd);
      } else {
        runCommand(offCmd);
      }
    }

    last = onoff;
    firstLoop = false;

    sleep(1);
  }

  return 0;
}
