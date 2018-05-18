#
# Projekt: ITY-proj1
# Autor:   Jan Kubica (xkubic39)
# E-mail:  xkubic39@stud.fit.vutbr.cz
# Datum:   25.2.2016
# 

# nazev souboru
NAME=proj1

# preklad zdrojoveho souboru *.tex --> *.pdf + *.ps
$(NAME).pdf: $(NAME).ps
	ps2pdf $(NAME).ps
	     
$(NAME).ps: $(NAME).dvi 
	dvips $(NAME).dvi
	
$(NAME).dvi: $(NAME).tex
	latex $(NAME).tex

# odstraneni souboru *.ps *.pdf
clean:
	rm $(NAME).aux
	rm $(NAME).log
	rm $(NAME).dvi
