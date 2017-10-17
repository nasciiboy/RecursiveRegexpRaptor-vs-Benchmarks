#! /bin/bash

loops=3

stdBins=$(ls -1 bin.std.*)
raptorBins=$(ls -1 bin.raptor.*)

rm -f result.*

for bin in $stdBins
do
    echo $bin
    ./$bin data.txt $loops \
           $'.|\n' \
           $'\\d' \
           $'\\S' \
           $'\\S+' \
           $'\\w' \
           $'\\w' \
           $'[a-zA-Z0-9_]' \
           $'[a-zA-Z]+' \
           $'[.\\s]+' \
           $'([^\n]+)' \
           $'e' \
           $'(((((e)))))' \
           $'((((((((((e))))))))))' \
           $'Twain' \
           $'(Twain)' \
           $'(?i)Twain' \
           $'(?i:Tw)(?:(?:a?A?[i_I]{0,2})?[nN])' \
           $'[a-z]shing' \
           $'Huck[a-zA-Z]+|Saw[a-zA-Z]+' \
           $'[a-q][^u-z]{13}x' \
           $'Tom|Sawyer|Huckleberry|Finn' \
           $'(Tom|Sawyer|Huckleberry|Finn)' \
           $'[hHeELlOo][hHeELlOo][hHeELlOo][hHeELlOo][hHeELlOo]' \
           $'Tom.{10,25}river|river.{10,25}Tom' \
           $'ing[^a-zA-Z]' \
           $'[a-zA-Z]ing[^a-zA-Z]' \
           $'([a-zA-Z]+ing)'
done


for rexp in $raptorBins
do
    echo $rexp
    ./$rexp data.txt $loops \
           $'.' \
           $':d' \
           $':S' \
           $':S+' \
           $':w' \
           $'[:w_]' \
           $'[a-zA-Z0-9_]' \
           $'[a-zA-Z]+' \
           $'[.:s]+' \
           $'<[^\n]+>' \
           $'e' \
           $'<<<<<e>>>>>' \
           $'<<<<<<<<<<e>>>>>>>>>>' \
           $'Twain' \
           $'<Twain>' \
           $'#*Twain' \
           $'(Tw)#*((a?A?[i_I]{0,2})?[nN])' \
           $'[a-z]shing' \
           $'Huck[a-zA-Z]+|Saw[a-zA-Z]+' \
           $'[a-q][^u-z]{13}x' \
           $'Tom|Sawyer|Huckleberry|Finn' \
           $'<Tom|Sawyer|Huckleberry|Finn>' \
           $'[hHeELlOo][hHeELlOo][hHeELlOo][hHeELlOo][hHeELlOo]' \
           $'---' \
           $'ing[^a-zA-Z]' \
           $'[a-zA-Z]ing[^a-zA-Z]' \
           $'---'
done

./maketable $(ls -r1 result.*)
