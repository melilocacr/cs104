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
#include "lyutils.h"
#include "astree.h"
#include "string_set.h"
#include "cppstrtok.h"

using namespace std;

extern string_set strSet;
extern lexer lexSet;
const string cpp_name = "/usr/bin/cpp";
string cpp_command;
int cppdebug = 0;

// Open a pipe from the C preprocessor.
// Exit failure if can't.
// Assigns opened pipe to FILE* yyin.
void cpp_popen (const char* filename) {
  cpp_command = cpp_name + " " + filename;
  yyin = popen (cpp_command.c_str(), "r");
  if (yyin == NULL) {
    syserrprintf (cpp_command.c_str());
    exit (exec::exit_status);
  } else {
    if (yy_flex_debug) {
      fprintf (stderr, "-- popen (%s), fileno(yyin) = %d\n",
               cpp_command.c_str(), fileno (yyin));
    }
    lexer::newfilename (cpp_command);
  }
}

void cpp_pclose() {
  int pclose_rc = pclose (yyin);
  eprint_status (cpp_command.c_str(), pclose_rc);
  if (pclose_rc != 0) exec::exit_status = EXIT_FAILURE;
}

const char* scan_opts (int argc, char** argv) {
  yy_flex_debug = 0;
  yydebug = 0;
  lexer::interactive = isatty (fileno (stdin))
                       and isatty (fileno (stdout));
  for (;;) {
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
  const char* filename = optind == argc ? "-" : argv[optind];
  cpp_popen (filename);
  return filename;
}

int main(int argc, char ** argv) {
  exec::execname = basename (argv[0]);
  if (yydebug or yy_flex_debug) {
    fprintf (stderr, "Command:");
    for (char** arg = &argv[0]; arg < &argv[argc]; ++arg) {
      fprintf (stderr, " %s", *arg);
    }
    fprintf (stderr, "\n");
  }

  // Flag management
  char* filename = const_cast<char*>(scan_opts(argc, argv));

  // file checks
  FILE *ocSource = fopen(filename, "r");
  if (!ocSource) {
    fprintf (stderr, "%s: %s\n", exec::execname, strerror (errno));
    exit(EXIT_FAILURE);
  }

  char* extPtr = strstr(filename, ".oc");
  if (!extPtr) {
    syserrprintf ("input file must be .oc extension");
    exit(EXIT_FAILURE);
  }

  // eliminate oc file extension
  *extPtr = 0;

  // str file Stream
  char* strFile = strcat(filename, ".str");
  FILE* strFileStream = fopen(strFile, "w");
  if (!strFileStream) {
    fprintf (stderr, "%s: %s\n", exec::execname, strerror (errno));
  }

  // tok file Stream
  *extPtr = 0;
  char* tokFileName = strcat(filename, ".tok");
  FILE* tokFileStream = fopen(tokFileName, "w");
  if (!tokFileStream) {
    fprintf (stderr, "%s: %s\n", exec::execname, strerror (errno));
  }
  lexSet.outFile(tokFileStream);

  // ast file Stream
  *extPtr = 0;
  char* astFileName = strcat(filename, ".ast");
  FILE* astFileStream = fopen(astFileName, "w");
  if (!astFileStream) {
    fprintf (stderr, "%s: %s\n", exec::execname, strerror (errno));
  }

  int parseint = yyparse();

  // dump ast
  print_ast (astFileStream, yyparse_astree, 0);

  // dump stringset
  strSet.dump(strFileStream); 

  cpp_pclose();
  fclose(tokFileStream);
  fclose(strFileStream);
  fclose(astFileStream);

  yylex_destroy();

  return 0;
}
