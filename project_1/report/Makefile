FILENAME=report
LATEX_COMPILER=xelatex

all: pdf clean-intermediate

pdf:
	$(LATEX_COMPILER) $(FILENAME)
	#bibtex $(FILENAME)
	#$(LATEX_COMPILER) $(FILENAME)
	$(LATEX_COMPILER) $(FILENAME)

clean-intermediate:
	rm -f $(FILENAME).{aux,bbl,blg,fff,log,ttt,out}

clean:clean-intermediate
	rm -f $(FILENAME).pdf
