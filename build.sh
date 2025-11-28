#!/bin/sh

gcc -O1 -fPIC -rdynamic -shared -o libdll0.so dll0.c
strip libdll0.so
# now make 4 copies of this file...
cp -v -f --remove-destination libdll0.so libdll0_0.so
cp -v -f --remove-destination libdll0.so libdll0_1.so
cp -v -f --remove-destination libdll0.so libdll0_2.so
cp -v -f --remove-destination libdll0.so libdll0_3.so

gcc -O1 -fPIC -rdynamic -shared -o libdll1.so dll1.c
strip libdll1.so

gcc -O1 -o SO_test main.cpp
strip SO_test

