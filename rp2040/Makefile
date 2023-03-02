.ONESHELL:
VPATH = _build


.PHONY: all
all: about.pdf about.html


.PHONY: clean
clean:
	rm -rf _build


%.pdf : %.adoc
	mkdir -p _build
	asciidoctor-pdf $< -D _build


%.html : %.adoc
	mkdir -p _build
	asciidoc -b slidy -o _build/$@ -a data-uri -a max-width=30cm $<

