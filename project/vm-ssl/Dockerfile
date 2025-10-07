FROM debian:stable-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
	build-essential \
	make \
	vim \
	valgrind \
	openssl \
	curl \
	ca-certificates \
	git \
	&& apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /work

WORKDIR /work
