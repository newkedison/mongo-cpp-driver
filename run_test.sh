#!/bin/bash

cd test
rm *.o a.out
make linux
./a.out
