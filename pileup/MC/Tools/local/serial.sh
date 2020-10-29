#!/bin/bash

ORIGIN=/work/jet_tuples/SIM/UL16
NAMES=(P8CP5 P8CP5APV NU NUAPV H7CH3 H7CH3APV)
LOCS=(P8CP5/FlatPthat_15to7000.root P8CP5/FlatPthat_15to7000_APV.root NuGun.root NuGun_APV.root H7CH3/FlatPthat_15to7000.root H7CH3/FlatPthat_15to7000_APV.root)
DO16=1 # 1 or 0

for index in ${!LOCS[*]}; do
    LOC=${ORIGIN}/${LOCS[$index]}
    NAME=${NAMES[$index]}
    root -l -b -q ./Pufromflat.C\(\"${LOC}\",\"${NAME}\"\)
    wait
    git add -f pileup_${NAME}.root
done

exit 0
