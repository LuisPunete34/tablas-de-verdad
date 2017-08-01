ejecutable := tabla
carpeta_fuente := fnt
carpeta_objeto := obj

compilador_c := gcc
compilador_yacc := bison
compilador_lex := flex
compilador_latex := pdflatex

codigo_fuente := tabla.tab.c \
		 lex.yy.c \
	         arbol.c \
		 vector.c

objetos2 := $(codigo_fuente:.c=.o)
objetos := $(addprefix $(carpeta_objeto)/, $(objetos2))
VPATH := $(carpeta_fuente)

$(ejecutable): $(objetos)
	$(compilador_c) -lm $(objetos) -o $@

$(carpeta_objeto)/tabla.tab.o: tabla.tab.c
	$(compilador_c) -c $< -o $@

$(carpeta_objeto)/lex.yy.o: lex.yy.c
	$(compilador_c) -c $< -o $@

$(carpeta_fuente)/tabla.tab.c: tabla.y arbol.h
	$(compilador_yacc) -d $< -o $@

$(carpeta_fuente)/lex.yy.c: tabla.l tabla.tab.h arbol.h
	$(compilador_lex) -o $@ $<

$(carpeta_objeto)/arbol.o: arbol.c arbol.h vector.h
	$(compilador_c) -c $< -o $@

$(carpeta_objeto)/vector.o: vector.c vector.h
	$(compilador_c) -c $< -o $@

reporte.pdf: reporte.tex
	$(compilador_latex) -output-directory $(carpeta_objeto) -shell-escape $<
	mv $(carpeta_objeto)/$@ ./$@

run:
	./$(ejecutable)

clean:
	rm -Rfv $(ejecutable) $(objetos) hoc.tab.c _minted-reporte

