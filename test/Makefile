CC := "gcc"
LD := "gcc"

BUILDDIR := build

# Files from the application to be compiled for testing
APP_SRC := \
	cleaner.c

TEST_SRC := ${sort ${wildcard src/*.c}}
TEST_OBJECTS := ${addprefix ${BUILDDIR}/, ${TEST_SRC:.c=.o}}
TESTS := ${patsubst src/%.c,build/src/%,${TEST_SRC}}

APP_OBJECTS := ${addprefix ${BUILDDIR}/app/, ${APP_SRC:.c=.o}}

DEPENDENCY_OBJECTS := ${TEST_OBJECTS:.o=.d} ${APP_OBJECTS:.o=.d}

INCLUDE_DIRS := \
	-I../src

DBG_FLAGS := -g -gdwarf-2 -O0 -fstack-usage# Debug

CFLAGS = \
	-std=gnu11 \
	${INCLUDE_DIRS} \
	${DBG_FLAGS}

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

LDFLAGS = \
	-lc

.PHONY: test clean

test: ${TESTS} | ${BUILDDIR}
	${subst ${} ${}, && ,${^:%=./%}}

clean:
	@rm -rf ${BUILDDIR}/

${TESTS}: ${APP_OBJECTS} ${TEST_OBJECTS}
	@mkdir -p ${dir $@}
	${LD} -o $@ $@.o ${APP_OBJECTS} ${LDFLAGS}

${BUILDDIR}/%.o: %.c Makefile | ${BUILDDIR}
	@mkdir -p ${dir $@}
	${CC} ${CFLAGS} -c $< -o $@

${BUILDDIR}/app/%.o: ../src/%.c Makefile | ${BUILDDIR}
	@mkdir -p ${dir $@}
	${CC} ${CFLAGS} -c $< -o $@

${BUILDDIR}:
	@mkdir $@

-include ${DEPENDENCY_OBJECTS}
