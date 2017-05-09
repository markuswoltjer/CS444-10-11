export PATH := /usr/local/apps/tex_live/current/bin/x86_64-linux:$(PATH)
DOCUMENT_NAME = Latex2

writeup:
	latex $(DOCUMENT_NAME).tex	
	latex $(DOCUMENT_NAME).tex
	dvips $(DOCUMENT_NAME).dvi
	ps2pdf $(DOCUMENT_NAME).ps
	gcc concurrency2.c -o concurrency2 -fopenmp

clean:
	rm -f *.out *.log *.dvi *.aux *.ps *.pdf *.toc
	
