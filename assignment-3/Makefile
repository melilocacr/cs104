# $Id: Makefile,v 1.23 2016-10-12 16:59:53-07 - - $

EXECBIN   = oc
OBJECTS   = ${ALLCSRC:.cpp=.o}
CPP       = g++ -g -O0 -Wall -Wextra -std=gnu++14
MKDEPS    = g++ -MM -std=gnu++14
GRIND	  = valgrind --leak-check=full --show-reachable=yes
DEPSFILE  = Makefile.deps
FLEX      = flex --outfile=${LEXCPP}
BISON     = bison --defines=${PARSEHDR} --output=${PARSECPP} --xml
XML2HTML  = xsltproc /usr/share/bison/xslt/xml2xhtml.xsl

MODULES   = astree auxlib lyutils string_set cppstrtok 
CPPSRC    = ${MODULES:=.cpp} main.cpp
FLEXSRC   = scanner.l
BISONSRC  = parser.y
PARSEHDR  = yyparse.h
LEXCPP    = yylex.cpp
PARSECPP  = yyparse.cpp
CGENS     = ${LEXCPP} ${PARSECPP}
HEADSRC   = ${MODULES:=.h} 
ALLGENS   = ${PARSEHDR} ${CGENS}
REPORTS   = ${LEXOUT} ${PARSEOUT}
TESTINS   = ${wildcard test*.in}
ALLCSRC   = ${CPPSRC} ${CGENS}
SUBSRC    = README Makefile ${CPPSRC} ${HEADSRC} ${FLEXSRC} ${BISONSRC}

USER       = melilocacr
HTTPS      = https://
GITHUB     = github.com
GITUSER    = ${USER}@github.com
REPO       = /melilocacr/cs104.git

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${CPP} -o${EXECBIN} ${OBJECTS}

yylex.o : yylex.cpp
	@ # Suppress warning message from flex compilation.
	${CPP} -Wno-sign-compare -c $<

%.o : %.cpp
	${CPP} -c $<

${LEXCPP} : ${FLEXSRC}
	${FLEX} ${FLEXSRC}

${PARSECPP} ${PARSEHDR} : ${BISONSRC}
	${BISON} ${BISONSRC}
	${XML2HTML} yyparse.xml >yyparse.html

ci :
	- git add -A
	- git commit -am
	- git push --all ${HTTPS}${GITHUB}${REPO}

clean :
	- rm ${OBJECTS} ${REPORTS} ${DEPSFILE}
	- rm ${foreach test, ${TESTINS:.in=}, \
		${patsubst %, ${test}.%, out err log}}
	- rm *.out *.err *.log *.oc *.str *.tok

spotless : clean
	- rm ${EXECBIN} List.*.ps List.*.pdf

deps : ${ALLCSRC}
	@ echo "# ${DEPSFILE} created `date` by ${MAKE}" >${DEPSFILE}
	${MKDEPS} ${ALLCSRC} >>${DEPSFILE}

${DEPSFILE} :
	@ touch ${DEPSFILE}
	${MAKE} --no-print-directory deps

checksource :
	- checksource ${CPPSRC} ${HEADSRC} 

submit :
	- mkdir -p submission
	- rm submission/*.*
	- cp ${SUBSRC} submission/
	- submit cmps104a-wm.s17 late2 ${SUBSRC}

ifeq "${NEEDINCL}" ""
include ${DEPSFILE}
endif
