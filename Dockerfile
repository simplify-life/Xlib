#VERSION 0.0.1
#build xlib container image

FROM gcc:latest

LABEL org.opencontainers.image.authors="xiaominghe2014@gmail.com"

COPY . /usr/src/Xlib
WORKDIR /usr/src/Xlib

CMD sh -c 'make clean && make && ./xlib'