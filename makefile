SDIR=./src
IDIR=./inc
TDIR=./test
ODIR=./obj
BDIR=./bin

cc=gcc
CFLAGS=-I$(IDIR) -Wall -g -c -o
OPTION=-lfl -o

LEX_OBJECTS=lex-test.o lexical.o

PARSE_OBJECTS=parse-test.o lexical.o syntax.o

LEX_OBJS=$(patsubst %, $(ODIR)/%, $(LEX_OBJECTS))

PARSE_OBJS=$(patsubst %, $(ODIR)/%, $(PARSE_OBJECTS))

LEX_PROG=$(BDIR)/lextest

PARSE_PROG=$(BDIR)/parsetest

lextest: $(LEX_OBJS) | $(BDIR)
	$(cc) $(OPTION) $(LEX_PROG) $^

parsetest: $(PARSE_OBJS) | $(BDIR)
	$(cc) $(OPTION) $(PARSE_PROG) $^

$(SDIR)/syntax.c: $(SDIR)/syntax.y
	bison -vdo $@ $<

$(SDIR)/lexical.c: $(SDIR)/lexical.l $(SDIR)/syntax.c
	flex -o $@ $<

$(ODIR)/%.o: $(SDIR)/%.c | $(ODIR)
	$(cc) $(CFLAGS) $@ $<

$(ODIR)/%.o: $(TDIR)/%.c | $(ODIR)
	$(cc) $(CFLAGS) $@ $<

$(BDIR):
	mkdir $(BDIR)

$(ODIR):
	mkdir $(ODIR)

clean:
	rm -rf $(ODIR) $(BDIR) $(SDIR)/syntax.[ch] $(SDIR)/syntax.output $(SDIR)/lexical.c