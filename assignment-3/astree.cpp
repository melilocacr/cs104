// $Id: astree.cpp,v 1.14 2016-08-18 15:05:42-07 - - $

#include <assert.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "astree.h"
#include "string_set.h"
#include "lyutils.h"

astree::astree (int symbol_, const location& lloc_, const char* info) {
   symbol = symbol_;
   lloc = lloc_;
   lexinfo = string_set::intern (info);
   // vector defaults to empty -- no children
}

astree::~astree() {
   while (not children.empty()) {
      astree* child = children.back();
      children.pop_back();
      delete child;
   }
   if (yydebug) {
      fprintf (stderr, "Deleting astree (");
      astree::dump (stderr, this);
      fprintf (stderr, ")\n");
   }
}

astree* astree::adopt (astree* child1, astree* child2) {
   if (child1 != nullptr) children.push_back (child1);
   if (child2 != nullptr) children.push_back (child2);
   return this;
}

astree* astree::adopt3 (astree* child1,
   astree* child2, astree* child3) {
   if (child1 != nullptr) children.push_back (child1);
   if (child2 != nullptr) children.push_back (child2);
   if (child3 != nullptr) children.push_back (child3);
   return this;
}

astree* astree::adopt_sym (astree* child, int symbol_) {
   symbol = symbol_;
   return adopt (child);
}


void astree::dump_node (FILE* outfile) {
   fprintf (outfile, "%p->{%s %zd.%zd.%zd \"%s\":",
            this, get_tname (symbol),
            lloc.filenr, lloc.linenr, lloc.offset,
            lexinfo->c_str());
   for (size_t child = 0; child < children.size(); ++child) {
      fprintf (outfile, " %p", children.at(child));
   }
}

void astree::dump_tree (FILE* outfile, int depth) {
   fprintf (outfile, "%*s", depth * 3, "");
   dump_node (outfile);
   fprintf (outfile, "\n");
   for (astree* child : children) child->dump_tree (outfile, depth + 1);
   fflush (NULL);
}

void astree::dump (FILE* outfile, astree* tree) {
   if (tree == nullptr) fprintf (outfile, "nullptr");
   else tree->dump_node (outfile);
}

void print_ast (FILE* outfile, astree* tree, int depth) {
   string indent = "";

   for(int i = 0; i < depth; i++) {
      indent += "|   ";
   }

   fprintf (outfile, "%s", indent.c_str());

   char* tname = (char*) get_tname(tree->symbol);
   if (strstr(tname, "TOK_") == tname) {
      tname += 4;
   }
   
   fprintf (outfile, "%s \"%s\" (%zd.%zd.%zd)\n",
            tname, tree->lexinfo->c_str(),
            tree->lloc.filenr, tree->lloc.linenr, tree->lloc.offset);
   for (astree* child : tree->children) {
      print_ast (outfile, child, depth + 1);
   }
}

astree* new_function
   (int tp, int tf, astree* identdecl, astree* paramlist, astree* block)
   { astree* new_func;
   if(!string(";").compare(*block->lexinfo)) { 
      new_func = new astree(tp, identdecl->lloc, ""); 
      new_func->adopt3(identdecl, paramlist, block);
   }
   else {
      new_func = new astree(tf, identdecl->lloc, "");
      new_func->adopt3(identdecl, paramlist, block);
   }

   return new_func;
}

void destroy (astree* tree1, astree* tree2) {
   if (tree1 != nullptr) delete tree1;
   if (tree2 != nullptr) delete tree2;
}

void errllocprintf (const location& lloc, const char* format,
                    const char* arg) {
   static char buffer[0x1000];
   assert (sizeof buffer > strlen (format) + strlen (arg));
   snprintf (buffer, sizeof buffer, format, arg);
   errprintf ("%s:%zd.%zd: %s",
              lexer::filename (lloc.filenr), lloc.linenr, lloc.offset,
              buffer);
}
