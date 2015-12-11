all: arvbin index busca encadeamento linear rehashing arvoreb trab2 doc

all-nodoc: arvbin index busca encadeamento linear rehashing arvoreb trab2

arvbin: arvbin.c
	gcc -Wall -c arvbin.c

index: index.c
	gcc -Wall -c index.c

busca: busca.c
	gcc -Wall -c busca.c

encadeamento: encadeamento.c
	gcc -Wall -c encadeamento.c -lm

linear: linear.c
	gcc -Wall -c linear.c

rehashing: rehashing.c
	gcc -Wall -c rehashing.c

arvoreb: arvoreb.c
	gcc -Wall -c arvoreb.c

trab2: main.c encadeamento.c linear.c rehashing.c arvoreb.c index.c arvbin.c busca.c
	gcc -Wall -o trab2 main.c encadeamento.o linear.o rehashing.o arvoreb.o index.o arvbin.o busca.o

doc: doc.tex
	latexmk -pdf -pdflatex="pdflatex -interaction=nonstopmode" -use-make doc.tex

clean-nodoc:
	rm *.o trab2

clean: 
	rm *.o trab2 doc.pdf doc.log doc.fls doc.aux doc.fdb_latexmk
