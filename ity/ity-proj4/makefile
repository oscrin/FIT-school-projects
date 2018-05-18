#
# Projekt: ITY-proj4
# Autor:   Jan Kubica (xkubic39)
# E-mail:  xkubic39@stud.fit.vutbr.cz
# Datum:   18.4.2016
# 

# nazev souboru
NAME=proj4

# preklad zdrojoveho souboru *.tex --> *.pdf + *.ps
do: $(NAME).tex
	latex $(NAME).tex
	bibtex $(NAME)
	latex $(NAME).tex
	latex $(NAME).tex
	dvips -t a4 $(NAME).dvi
	ps2pdf $(NAME).ps

# odstraneni souboru *.ps *.pdf *.aux *.log *.dvi
clean:
	rm $(NAME).pdf
	rm $(NAME).ps
	rm $(NAME).aux
	rm $(NAME).log
	rm $(NAME).dvi
	rm $(NAME).blg
	rm $(NAME).bbl

