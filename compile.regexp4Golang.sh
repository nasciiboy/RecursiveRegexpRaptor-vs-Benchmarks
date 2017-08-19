#! /bin/bash

cd regexp4Golang

if go build main.go
then
    echo "Build regexp4Golang!"
    mv main ../bin.raptor.regexp4Golang
else
    echo "regexp4Golang: no Build"
fi
