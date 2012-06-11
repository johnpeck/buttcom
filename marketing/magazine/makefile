# Makefile for buttcom magazine submission

# The postscript target
ps: main.ps

main.ps: main.dvi
	dvips -t letter main.dvi -o main.ps

# The dvi target
dvi: main.dvi

main.dvi: main.tex\
		./figs/command_array.eps\
		./figs/recbuffer.eps\
		./figs/prsbuffer.eps
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

# Make the parse buffer figure
./figs/prsbuffer.eps: ./figs/prsbuffer.fig
	# --------- Making the received character buffer figure -----------
	fig2dev -L eps -m 1 ./figs/prsbuffer.fig ./figs/prsbuffer.eps
