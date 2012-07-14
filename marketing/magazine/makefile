# Makefile for buttcom magazine submission

# My name
MYNAME = jpeck
# The publisher name (no spaces)
PUBLISHER = "circuit_cellar"
# The revision code.  This plus the publisher name makes a revision.
REVCODE = a

#------------------------- Done with configuration ---------------------

# Convert the revision code to lowercase
REVCODE := $(shell echo $(REVCODE) | tr A-Z a-z)
# Convert the publisher to lowercase
PUBLISHER := $(shell echo $(PUBLISHER) | tr A-Z a-z)
# Convert my name to lowercase
MYNAME := $(shell echo $(MYNAME) | tr A-Z a-z)

# The submit directory -- like jpeck_circuit_cellar_submit_a
SUBMITDIR := submitroot/$(MYNAME)"_"$(PUBLISHER)"_submit_"$(REVCODE)

# The postscript target
ps: main.ps

main.ps: main.dvi
	dvips -t letter main.dvi -o $@

# The dvi target
dvi: main.dvi

# Make main.dvi a PHONY target so that latex can be run over an over again
# to resolve cross references.
.PHONY: main.dvi
main.dvi: main.tex\
		./figs/command_snippet.eps\
		./figs/recbuffer.eps\
		./figs/prsbuffer.eps \
		./tikz/parse_cmd_flow.eps\
		./tikz/recv_cmd_flow.eps
	# ------------- Running latex on the document ---------------------
	latex main.tex


# Make the code snippet figure
./figs/command_snippet.eps: ./figs/command_snippet.fig
	# ------------ Making the code snippet figure ---------------------
	fig2dev -L eps -m 1 ./figs/command_snippet.fig ./figs/command_snippet.eps
	# Fix the bounding box
	./figs/bboxfix.sh ./figs/command_snippet.eps temp
	mv temp ./figs/command_snippet.eps
	
# Make the received character buffer figure
./figs/recbuffer.eps: ./figs/recbuffer.fig
	# --------- Making the received character buffer figure -----------
	fig2dev -L eps -m 1 ./figs/recbuffer.fig ./figs/recbuffer.eps
	
# Make the command processing flow figure
./figs/parse_cmd_flow.eps: ./figs/parse_cmd_flow.fig
	# ---------- Making the command processing flow figure ------------
	fig2dev -L eps -m 1 ./figs/parse_cmd_flow.fig ./figs/parse_cmd_flow.eps

# Make the received character flow diagram
./tikz/recv_cmd_flow.eps: ./tikz/recv_cmd_flow.tex\
							./doctools/latex/tikzstuff.tex
	# -------- Making the tikz received character flow figure ----------
	cd tikz; pdflatex recv_cmd_flow.tex
	pdftops -eps ./tikz/recv_cmd_flow.pdf
	
	
# Make the parse buffer flow diagram
./tikz/parse_cmd_flow.eps: ./tikz/parse_cmd_flow.tex\
							./doctools/latex/tikzstuff.tex
	# ----------- Making the tikz parse buffer flow figure -------------
	cd tikz; pdflatex parse_cmd_flow.tex
	pdftops -eps ./tikz/parse_cmd_flow.pdf

# Make the parse buffer figure
./figs/prsbuffer.eps: ./figs/prsbuffer.fig
	# --------- Making the received character buffer figure -----------
	fig2dev -L eps -m 1 ./figs/prsbuffer.fig ./figs/prsbuffer.eps

.PHONY: submit
submit: $(SUBMITDIR)/readme.txt


# The README file
$(SUBMITDIR)/readme.txt: submitroot\
						 $(SUBMITDIR)\
						 changelog.txt
	echo "ASCII command interface for the AVR Butterfly" > $@
	echo "" >> $@
	echo "Submitted for publication in Circuit Cellar" >> $@
	echo "Submission revision "$(REVCODE) >> $@
	echo "Author: John Peck" >> $@
	echo "        3740 24th St" >> $@
	echo "        San Francisco, CA 94114" >> $@
	echo "        (608) 215-0041" >> $@
	echo "" >> $@
	echo "New in revision "$(REVCODE): >> $@
	cat changelog.txt >> $@


# The submission directory
$(SUBMITDIR): submitroot
	mkdir -p $(SUBMITDIR)

submitroot:
	mkdir -p submitroot

# Spell check
.PHONY: spell
spell:
	aspell check main.tex
	
# Word and figure count
.PHONY: count
count:
	texcount main.tex
	
# Bibliography
bib: main.aux
	bibtex main
	
# Clean
.PHONY: clean
clean:
	rm -f ./tikz/recv_cmd_flow.aux
	rm -f ./tikz/recv_cmd_flow.pdf
	rm -f ./tikz/recv_cmd_flow.log
