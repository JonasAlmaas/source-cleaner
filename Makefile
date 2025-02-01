CC := "gcc"

#### Project ####

PROJECT_NAME := src-cleaner

BUILDDIR := build

INCLUDE_DIRS = \
	-Isrc

DEFINES =

SOURCES := \
	src/main.c

OBJECTS := ${addprefix ${BUILDDIR}/, ${SOURCES:.c=.o}}
DEPENDENCY_OBJECTS := ${OBJECTS:.o=.d}

TARGET = ${BUILDDIR}/${PROJECT_NAME}

DBG_FLAGS := -g -gdwarf-2 -O0 -fstack-usage# Debug
# DBG_FLAGS := -Os -DNDEBUG# Release

CFLAGS = \
	-std=gnu11 \
	${DEFINES} \
	${INCLUDE_DIRS} \
	${DBG_FLAGS} \
	-Wall -Wextra -Wmissing-include-dirs \
	-Wswitch-default -Wpedantic \
	-Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

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
