#!/bin/bash

NORMTAG=/afs/cern.ch/user/h/hsiikone/work/Normtags/normtag_PHYSICS.json

FOLDERS=(B C D E Fe Fl G H)

# The --byls setting is sensitive to minBiasXsec; this has an effect on the avgpu value
echo "Processing luminosity info for the whole run!"
brilcalc lumi -i fullrun.json --byls --normtag $NORMTAG -b "STABLE BEAMS" --minBiasXsec 80000 -o lumifile80.csv
brilcalc lumi -i fullrun.json --byls --normtag $NORMTAG -b "STABLE BEAMS" --minBiasXsec 69200 -o lumifile69.csv
wait

for loc in ${FOLDERS[@]}; do 
    cd $loc

    HLTNAME=HLT_PFJet*
    if [[ $loc == *z ]]; then
      HLTNAME=HLT_ZeroBias_v*
    fi
    wait
    echo "Running folder $loc with HLT path $HLTNAME!"

    brilcalc lumi -i clumis.json --normtag $NORMTAG -b "STABLE BEAMS" --hltpath "$HLTNAME" -o lumifile.csv
    wait

    echo "Finished! folder "$loc
    cd ..
done

exit 0
