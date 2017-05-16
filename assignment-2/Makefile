# $Id: Makefile,v 1.23 2016-10-12 16:59:53-07 - - $

EXECBIN   = oc
OBJECTS   = ${ALLCSRC:.cpp=.o}
CPP       = g++ -g -O0 -Wall -Wextra -std=gnu++14
MKDEPS    = g++ -MM -std=gnu++14
GRIND	  = valgrind --leak-check=full --show-reachable=yes
DEPSFILE  = Makefile.deps
CPPSRC    = main.cpp auxlib.cpp string_set.cpp cppstrtok.cpp
HEADSRC   = auxlib.h string_set.h cppstrtok.h 
ALLGENS   = ${PARSEHDR} ${CGENS}
REPORTS   = ${LEXOUT} ${PARSEOUT}
TESTINS   = ${wildcard test*.in}
ALLCSRC   = ${CPPSRC} 
SUBSRC    = README Makefile ${CPPSRC} ${HEADSRC}

USER       = melilocacr
HTTPS      = https://
GITHUB     = github.com
GITUSER    = ${USER}@github.com
REPO       = /melilocacr/cs104.git

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${CPP} -o${EXECBIN} ${OBJECTS}

%.o : %.cpp
	${CPP} -c $<

ci :
	- git add -A
	- git commit -a
	- git push --all ${HTTPS}${GITHUB}${REPO}

clean :
	- rm ${OBJECTS} ${REPORTS} ${DEPSFILE}
	- rm ${foreach test, ${TESTINS:.in=}, \
		${patsubst %, ${test}.%, out err log}}
	- rm *.out *.err *.log *.oc *.str

spotless : clean
	- rm ${EXECBIN} List.*.ps List.*.pdf

deps : ${ALLCSRC}
	@ echo "# ${DEPSFILE} created `date` by ${MAKE}" >${DEPSFILE}
	${MKDEPS} ${ALLCSRC} >>${DEPSFILE}

${DEPSFILE} :
	@ touch ${DEPSFILE}
	${MAKE} --no-print-directory deps

submit :
	-mkdir -p submission
	-rm submission/*.*
	-cp ${SUBSRC} submission/
	-submit cmps104a-wm.s17 late2 ${SUBSRC}

ifeq "${NEEDINCL}" ""
include ${DEPSFILE}
endif
