#!/bin/bash

FOLDERS=(B C D E Fe Fl G H)
DO16=1 # 1 or 0

for index in ${!FOLDERS[*]}; do
    loc=${FOLDERS[$index]}
    cd ${loc}
    root -l -b -q ../combine.C\(\"${loc}\",${DO16}\)
    wait
    git add -f pileup_DT.root
    cd ..
done

exit 0
