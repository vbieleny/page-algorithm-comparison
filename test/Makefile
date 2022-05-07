PATHB		:= build
PATHR		:= build/results
PATHI		:= ../include
PATHT		:= .
PATHS		:= ../src
EXCS		:= kernel.c

SRCT		:= $(wildcard $(PATHT)/*_test.c)
OUTS		:= $(SRCT:$(PATHT)/%.c=%.out)
OUTS		:= $(addprefix $(PATHB)/, $(OUTS))

EXCS		:= $(addprefix $(PATHS)/, $(EXCS))
SRCS		:= $(wildcard $(PATHS)/*.c)
SRCS		:= $(filter-out $(EXCS), $(SRCS))
OBJS		:= $(SRCS:$(PATHS)/%.c=%.o)
OBJS		:= $(addprefix $(PATHB)/, $(OBJS))

CC			?= /usr/bin/gcc
CFLAGS 		:= $(CFLAGS) -Wall -Wno-format -std=gnu11 -masm=intel -m32 -g -I$(PATHI)

RESULTS 	:= $(patsubst $(PATHT)/%_test.c, $(PATHR)/%_test.txt, $(SRCT))

.EXTRA_PREREQS := Makefile
.PHONY: test clean

.PRECIOUS: $(PATHB)/%_test.out
.PRECIOUS: $(PATHB)/%.d
.PRECIOUS: $(PATHB)/%.o
.PRECIOUS: $(PATHR)/%.txt

all: $(OUTS)

test: $(RESULTS)
	@echo "-----------------------\nPASSES:\n-----------------------"
	@grep -h -s PASS $(PATHR)/*.txt || true
	@echo "-----------------------\nFAILURES:\n-----------------------"
	@grep -h -s FAIL $(PATHR)/*.txt || true
	@echo "-----------------------\nASSERTS:\n-----------------------"
	@grep -h -s assert: $(PATHR)/*.txt || true
	@echo "\nDONE"

$(PATHR)/%.txt: $(PATHB)/%.out | $(PATHR)
	./$< > $@ || (rm -rf $(PATHR) && false)

$(PATHB)/%_test.out: $(PATHB)/%_test.o $(OBJS) | $(PATHB)
	$(CC) $(CFLAGS) -o $@ $^

$(PATHB)/%.o:: $(PATHS)/%.c | $(PATHB)
	$(CC) $(CFLAGS) -c $< -o $@

$(PATHB)/%.o:: $(PATHT)/%.c | $(PATHB)
	$(CC) $(CFLAGS) -c $< -o $@

$(PATHB):
	mkdir -p $@

$(PATHR):
	mkdir -p $@

clean:
	-rm -rf $(PATHB) $(PATHR)
