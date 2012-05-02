NAME= 		main
TEXSRCS= 	$(wildcard *.tex) \
			doctools/latex/magazine_template.tex \
			doctools/latex/mycommands.tex
XDVI_FLAGS= -paper letter -interpreter /usr/bin/X11/gs
DVIPS_FLAGS= -t letter
XFIGSRCS=	figs/usart_pinout.fig \
			figs/recv_cmd_flow.fig \
			figs/parse_cmd_flow.fig \
			figs/recbuffer.fig
include /usr/share/latex-mk/latex.gmk

