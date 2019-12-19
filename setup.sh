#!/bin/bash


if (test "$1" = "build"); then
	gcc -I. screener.c -shared -o libscreener.so
elif (test "$1" = "install"); then
	cp -f screener.h /usr/include/ || exit 1
	cp -f libscreener.so /lib/ || exit 1
	cp -f screener.pc /lib/pkgconfig/ || exit 1
fi