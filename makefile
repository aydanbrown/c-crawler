CC=gcc

default: build

build:
	gcc -o /crawler/bin/verify-article lib/verify-article.c && \
	gcc -o /crawler/bin/url-extract lib/url-extract.c && \
	gcc -o /crawler/bin/meta-extract lib/meta-extract.c && \
	gcc -o /crawler/bin/content-extract lib/content-extract.c

install:
	mkdir /crawler && \
	mkdir /crawler/bin && \
	mkdir /crawler/downloads && \
	mkdir /crawler/process && \
	mkdir /crawler/process/meta && \
	mkdir /crawler/process/content && \
	make build

database:
	node create-database.js

clean:
	sudo rm /crawler/downloads/* && \
	sudo rm -r /crawler/process/urls && \
	sudo rm -r /crawler/process/meta/* && \
	sudo rm -r /crawler/process/content/*

reset:
	make database && \
	sudo rm -r /crawler && \
	make install

start:
	sudo node crawler.js
