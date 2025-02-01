CC := "gcc"
LD := "gcc"

#### Project ####

PROJECT_NAME := src-cleaner

BUILDDIR := build

INCLUDE_DIRS = \
	-Isrc

DEFINES =

SOURCES := \
	src/main.c \
	src/cleaner.c

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

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

LDFLAGS = \
	-lc

.PHONY: all test clean

all: ${TARGET}

test:
	${MAKE} -C test test

clean:
	@rm -rf ${BUILDDIR}/

${TARGET}: ${OBJECTS} | ${BUILDDIR}
	${LD} ${LDFLAGS} -o $@ $^

${BUILDDIR}/%.o: %.c Makefile | ${BUILDDIR}
	@mkdir -p ${dir $@}
	${CC} ${CFLAGS} -c $< -o $@

${BUILDDIR}:
	@mkdir $@

-include ${DEPENDENCY_OBJECTS}
