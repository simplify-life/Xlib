#VERSION 0.0.1
#build xlib container image

FROM gcc:4.9

MAINTAINER ximena "xiaominghe2014@gmail.com"

COPY . /usr/src/Xlib
WORKDIR /usr/src/Xlib

CMD sh -c 'make clean && make && ./xlib'