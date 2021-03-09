
all: out/results.html

out/ :
	mkdir -p out

out/results.html : results.adoc out/
	asciidoctor -v $< -o $@


