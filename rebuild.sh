#! /bin/bash

compiles=$(ls -1 compile*)

for compile in $compiles
do
    ./$compile
done

cd makeTable
if go build main.go
then
    echo "Build makeTable"
    mv main ../maketable
else
    echo "makeTable: no Build"
fi
