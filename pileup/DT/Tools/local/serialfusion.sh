#!/bin/bash

DO16=1 # 1 or 0

MAINDIR=""
LISTING=""
for loc in "$@"; do
    if [ "${LISTING}" == "" ]; then
       mkdir -p ${loc}
       chmod 755 ${loc}
       MAINDIR=${loc}
       LISTING=pileup_DTu.root
    else
       LISTING=`echo "${LISTING} ../${loc}/pileup_DTu.root"`
    fi
done

cd ${MAINDIR}
echo ${LISTING}
hadd -f ${LISTING}
wait
root -l -b -q ../normalize.C\(${DO16}\)
wait
git add -f pileup_DT.root
cd ..

exit 0
