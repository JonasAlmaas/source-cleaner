CC := "gcc"

#### Project ####

PROJECT_NAME := src-cleaner

BUILDDIR := build

SOURCES := \
	src/main.c

OBJECTS := ${addprefix ${BUILDDIR}/, ${SOURCES:.c=.o}}
DEPENDENCY_OBJECTS := ${OBJECTS:.o=.d}

TARGET = ${BUILDDIR}/${PROJECT_NAME}

.PHONY: all clean

all: ${TARGET}

clean:
	@rm -rf ${BUILDDIR}/

${TARGET}: ${OBJECTS} | ${BUILDDIR}
	${CC} -o $@ $^

${BUILDDIR}/%.o: %.c Makefile | ${BUILDDIR}
	@mkdir -p ${dir $@}
	${CC} ${CFLAGS} -c $< -o $@

${BUILDDIR}:
	@mkdir $@

-include ${DEPENDENCY_OBJECTS}
