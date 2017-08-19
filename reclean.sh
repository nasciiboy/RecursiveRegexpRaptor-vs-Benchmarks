#! /bin/bash

makes=$(ls -1 makefile-*)

for m in $makes
do
    make -f $m clean
done

rm bin.*
rm result.*
rm table
rm maketable
