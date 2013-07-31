#!/bin/bash

cd test
rm *.o a.out
make linux
valgrind --leak-check=full --track-origins=yes --error-exitcode=1 --show-reachable=yes -v ./a.out
