# Makefile for buttcom magazine submission

# My name
MYNAME = jpeck
# The publisher name (no spaces)
PUBLISHER = "circuit_cellar"
# The revision code.  This plus the publisher name makes a revision.
REVCODE = a

# Code sources
CODESRC = 	bc_main.c \
			bc_functions.c \
			bc_command.c \
			bc_usart.c \
			bc_logger.c \
			bc_numbers.c \
			bc_ascii.c \
			bc_clock.c \
			bc_adc.c
#------------------------- Done with configuration ---------------------

# Convert the revision code to lowercase
REVCODE := $(shell echo $(REVCODE) | tr A-Z a-z)
# Convert the publisher to lowercase
PUBLISHER := $(shell echo $(PUBLISHER) | tr A-Z a-z)
# Convert my name to lowercase
MYNAME := $(shell echo $(MYNAME) | tr A-Z a-z)

# The submit directory -- like jpeck_circuit_cellar_submit_a
SUBMITDIR := submitroot/$(MYNAME)"_"$(PUBLISHER)"_submit_"$(REVCODE)

# The zipfile name
ZIPFILE := $(MYNAME)"_buttcom_"$(REVCODE)


# The pdf target
.PHONY: pdf
pdf: main.pdf
main.pdf: main.ps
	ps2pdf main.ps main.pdf

# The postscript target
.PHONY: ps
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
		./figs/usart_pinout.eps \
		./tikz/parse_cmd_flow.eps\
		./tikz/recv_cmd_flow.eps
	# ------------- Running latex on the document ---------------------
	latex main.tex


# --------------------- Create the submission -------------------------
.PHONY: submit
submit: $(SUBMITDIR)/readme.txt \
		$(SUBMITDIR)/code \
		$(addprefix $(SUBMITDIR)/code/,$(CODESRC)) \
		$(addprefix $(SUBMITDIR)/code/,$(addsuffix .h,$(basename $(CODESRC)))) \
		$(SUBMITDIR)/code/makefile \
		$(SUBMITDIR)/figures/figure1_connection_drawing.eps \
		$(SUBMITDIR)/figures/figure2_rxchar_flow.eps \
		$(SUBMITDIR)/figures/figure3_rxchar_buffer.eps \
		$(SUBMITDIR)/figures/figure4_hello_trace.tif \
		$(SUBMITDIR)/figures/figure5_parse_flow.eps \
		$(SUBMITDIR)/figures/figure6_parse_buffer.eps \
		$(SUBMITDIR)/figures/listing1_command_array.eps \
		$(SUBMITDIR)/figures/figure7_cal_trace.tif \
		$(SUBMITDIR)/text/buttcom.pdf
	zip -r $(SUBMITDIR)/$(ZIPFILE) $(SUBMITDIR)


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
	
# The article
$(SUBMITDIR)/text/buttcom.pdf: $(SUBMITDIR)/text \
					           main.pdf
	cp main.pdf $@

# Figure 1 -- Connection drawing
$(SUBMITDIR)/figures/figure1_connection_drawing.eps: $(SUBMITDIR)/figures\
												     ./figs/usart_pinout.eps
	cp ./figs/usart_pinout.eps $@ 
	
# Figure 2 -- Received character flow
$(SUBMITDIR)/figures/figure2_rxchar_flow.eps: $(SUBMITDIR)/figures\
											  ./tikz/recv_cmd_flow.eps
	cp ./tikz/recv_cmd_flow.eps $@ 
	
# Figure 3 -- Received character buffer
$(SUBMITDIR)/figures/figure3_rxchar_buffer.eps: $(SUBMITDIR)/figures\
											    ./figs/recbuffer.eps
	cp ./figs/recbuffer.eps $@ 
	
# Figure 4 -- Hello command trace
$(SUBMITDIR)/figures/figure4_hello_trace.tif: $(SUBMITDIR)/figures\
			                                  ./pngs/hellotrace.tif
	cp ./pngs/hellotrace.tif $@
	
# Figure 5 -- Command parse flow
$(SUBMITDIR)/figures/figure5_parse_flow.eps: $(SUBMITDIR)/figures\
											 ./tikz/parse_cmd_flow.eps
	cp ./tikz/parse_cmd_flow.eps $@
	
# Figure 6 -- Parse buffer
$(SUBMITDIR)/figures/figure6_parse_buffer.eps: $(SUBMITDIR)/figures\
											   ./figs/prsbuffer.eps
	cp ./figs/prsbuffer.eps $@ 
	
# Listing 1 -- Command array
$(SUBMITDIR)/figures/listing1_command_array.eps: $(SUBMITDIR)/figures\
											     ./figs/command_snippet.eps
	cp ./figs/command_snippet.eps $@ 
	
# Figure 7 -- Calibration trace
$(SUBMITDIR)/figures/figure7_cal_trace.tif: $(SUBMITDIR)/figures\
			                                ./pngs/caltrace.tif
	cp ./pngs/caltrace.tif $@

# The submission figures directory
$(SUBMITDIR)/figures: $(SUBMITDIR)
	mkdir -p $(SUBMITDIR)/figures

# The submission text directory
$(SUBMITDIR)/text: $(SUBMITDIR)
	mkdir -p $(SUBMITDIR)/text

# Copying code to the submission directory
$(SUBMITDIR)/code/%: ../../implement/code/%
	cp $< $@

# The submission code directory
$(SUBMITDIR)/code: $(SUBMITDIR)
	mkdir -p $(SUBMITDIR)/code

# The submission directory
$(SUBMITDIR): submitroot
	mkdir -p $(SUBMITDIR)

submitroot:
	mkdir -p submitroot
	


# ----------------------- Create the figures ---------------------------

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
	
# Make the hardware connection drawing
./figs/usart_pinout.eps: ./figs/usart_pinout.fig
	# ------------- Making the hardware connection figure --------------
	fig2dev -L eps -m 1 ./figs/usart_pinout.fig ./figs/usart_pinout.eps
	
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
