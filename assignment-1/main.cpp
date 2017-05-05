// This program was coded by Melissa Jiménez Araya
// It is a main program for an OC language compiler
// Spring 2017

#include <string>
using namespace std;

#include <assert.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

#include "auxlib.h"
#include "string_set.h"
#include "cppstrtok.h"

using namespace std;

extern string_set tokenSet;

int yy_flex_debug;
int yydebug;
int cppdebug = 0;

void scan_opts (int argc, char** argv) {
  yy_flex_debug = 0;
  yydebug = 0;
  for(;;) {
    int opt = getopt (argc, argv, "@:D:ly");
    if (opt == EOF) break;
    switch (opt) {
      case '@': set_debugflags (optarg);   break;
      case 'D': cppdebug = 1;              break;
      case 'l': yy_flex_debug = 1;         break;
      case 'y': yydebug = 1;               break;
      default:  errprintf ("bad option (%c)\n", optopt); break;
    }
  }
  if (optind > argc) {
    errprintf ("Usage: oc [-ly] [filename]\n");
    exit (EXIT_FAILURE);
  }
}

int main(int argc, char ** argv) {
  // Flag management
  scan_opts(argc, argv);

  const char* execname = basename (argv[0]);
  char* filename = argv[optind];
  string filenameStr = filename;
  string command;

  // file checks
  FILE *ocSource = fopen(filename, "r");
  if(!ocSource) {
    fprintf (stderr, "%s: %s\n", execname, strerror (errno));
    exit(EXIT_FAILURE);
  }


  char* extPtr = strstr(filename, ".oc");
  if (!extPtr) {
    syserrprintf ("input file must be .oc extension");
    exit(EXIT_FAILURE);
  }

  // call CPP preprocessor
  if(cppdebug) {
    string CPP = "/usr/bin/cpp -D";
  }
  else {
    string CPP = "/usr/bin/cpp";
  }
  
  command = CPP + " " + filenameStr;

  FILE* pipe = popen (command.c_str(), "r");
  // DEBUGF('c', "cpp command=\"%s\"\n", command.c_str());
  if (!pipe) {
    syserrprintf (command.c_str());
    exit(EXIT_FAILURE);
  }
  else {
    // tokenize
    cpplines (pipe, filename);
    int pclose_rc = pclose (pipe);
    eprint_status (command.c_str(), pclose_rc);
  }

  // eliminate oc file extension
  *extPtr = 0;

  // store resulting string set into an external file
  char* strFile = strcat(filename, ".str");
  FILE* strFileStream = fopen(strFile, "w");
  if (!strFileStream) {
     fprintf (stderr, "%s: %s\n", execname, strerror (errno));
  }
  tokenSet.dump(strFileStream);
  fclose(strFileStream);

  return 0;
}
