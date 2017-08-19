#! /bin/bash

cd re2Golang

if go build main.go
then
    echo "Build re2Golang!"
    mv main ../bin.std.re2Golang
else
    echo "re2Golang: no Build"
fi
