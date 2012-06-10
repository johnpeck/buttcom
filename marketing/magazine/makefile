# Makefile for buttcom magazine submission

# The dvi target
dvi: main.dvi

main.dvi: main.tex ./figs/command_array.eps
	latex main.tex

# Make the code snippet figure
./figs/command_array.eps: ./figs/command_array.fig
	# ------------ Making the code snippet figure ---------------------
	fig2dev -L eps -m 1 ./figs/command_array.fig ./figs/command_array.eps
	# Fix the bounding box
	./figs/bboxfix.sh ./figs/command_array.eps temp
	mv temp ./figs/command_array.eps
