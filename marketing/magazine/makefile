# Makefile for buttcom magazine submission

# The first rule seen by make is the default

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
		./figs/command_array.eps\
		./figs/recbuffer.eps\
		./figs/prsbuffer.eps \
		./figs/parse_cmd_flow.eps\
		./tikz/recv_cmd_flow.eps
	# ------------- Running latex on the document ---------------------
	latex main.tex


# Make the code snippet figure
./figs/command_array.eps: ./figs/command_array.fig
	# ------------ Making the code snippet figure ---------------------
	fig2dev -L eps -m 1 ./figs/command_array.fig ./figs/command_array.eps
	# Fix the bounding box
	./figs/bboxfix.sh ./figs/command_array.eps temp
	mv temp ./figs/command_array.eps
	
# Make the received character buffer figure
./figs/recbuffer.eps: ./figs/recbuffer.fig
	# --------- Making the received character buffer figure -----------
	fig2dev -L eps -m 1 ./figs/recbuffer.fig ./figs/recbuffer.eps
	


# Make the command processing flow figure
./figs/parse_cmd_flow.eps: ./figs/parse_cmd_flow.fig
	# ---------- Making the command processing flow figure ------------
	fig2dev -L eps -m 1 ./figs/parse_cmd_flow.fig ./figs/parse_cmd_flow.eps

# Make the other received character flow diagram
./tikz/recv_cmd_flow.eps: ./tikz/recv_cmd_flow.tex\
							./doctools/latex/tikzstuff.tex
	# -------- Making the tikz received character flow figure ----------
	cd tikz; pdflatex recv_cmd_flow.tex
	pdftops -eps ./tikz/recv_cmd_flow.pdf

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
