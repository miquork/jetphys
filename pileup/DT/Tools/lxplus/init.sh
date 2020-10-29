#!/bin/bash

FOLDERS=(B C D E Fe Fl G H)
SETS=(/JetHT/Run2016B-21Feb2020_ver2_UL2016_HIPM-v1/MINIAOD /JetHT/Run2016C-21Feb2020_UL2016_HIPM-v1/MINIAOD /JetHT/Run2016D-21Feb2020_UL2016_HIPM-v1/MINIAOD /JetHT/Run2016E-21Feb2020_UL2016_HIPM-v1/MINIAOD /JetHT/Run2016F-21Feb2020_UL2016_HIPM-v1/MINIAOD /JetHT/Run2016F-21Feb2020_UL2016-v1/MINIAOD /JetHT/Run2016G-21Feb2020_UL2016-v1/MINIAOD /JetHT/Run2016H-21Feb2020_UL2016-v1/MINIAOD)
SETSZB=(/ZeroBias/Run2016B-21Feb2020_ver2_UL2016_HIPM-v1/MINIAOD /ZeroBias/Run2016C-21Feb2020_UL2016_HIPM-v1/MINIAOD /ZeroBias/Run2016D-21Feb2020_UL2016_HIPM-v1/MINIAOD /ZeroBias/Run2016E-21Feb2020_UL2016_HIPM-v1/MINIAOD /ZeroBias/Run2016F-21Feb2020_UL2016_HIPM-v1/MINIAOD /ZeroBias/Run2016F-21Feb2020_UL2016-v1/MINIAOD /ZeroBias/Run2016G-21Feb2020_UL2016-v1/MINIAOD /ZeroBias/Run2016H-21Feb2020_UL2016-v1/MINIAOD)
CERTIFICATE=/afs/cern.ch/user/h/hsiikone/work/certificates/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt

for index in ${!FOLDERS[*]}; do 
    loc=${FOLDERS[$index]}
    ds=${SETS[$index]}
    dszb=${SETSZB[$index]}

    mkdir -p $loc
    mkdir -p ${loc}z
    wait
    das_client --query="run,lumi dataset=${ds}" > ${loc}/lumis.txt 
    wait
    das_client --query="run,lumi dataset=${dszb}" > ${loc}z/lumis.txt 
    wait

    python txt2json.py ${loc}/ || echo "lumis.txt not found, trying to use lumis.json"
    wait
    cd $loc
    compareJSON.py --and lumis.json ../fullrun.json clumis.json
    wait
    cd ..
    python txt2json.py ${loc}z/ || echo "lumis.txt not found, trying to use lumis.json"
    wait
    cd ${loc}z
    compareJSON.py --and lumis.json ../fullrun.json clumis.json
    wait
    cd ..
    compareJSON.py --or ${loc}/clumis.json ${loc}z/clumis.json tmp.json
    if [ "$index" -eq "0" ]; then
       mv tmp.json fullrun.json
    else
       mv fullrun.json tmptmp.json
       compareJSON.py --or tmptmp.json tmp.json fullrun.json
       wait
       rm tmp*.json
    fi

    echo "Finished! folder ${loc}(z) with dataset ${ds} and ${dszb}"
done

exit 0
