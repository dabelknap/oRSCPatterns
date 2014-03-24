##oRSC Pattern Generator

###Installation

The current version of this code runs in `CMSSW_5_3_12`, which is an SLC5 CMSSW area. At Wisconsin, you must use `login02.hep.wisc.edu`.

```sh
scram project -n orsc5312 CMSSW CMSSW_5_3_12
cd orsc5312/src

cmsenv
git cms-init

git cms-addpkg DataFormats/L1CaloTrigger
git cms-addpkg L1TriggerConfig/L1ScalesProducers
git cms-cvs-history import  UCT2015_v4 L1Trigger/RegionalCaloTrigger

git clone https://github.com/uwcms/UCT2015.git L1Trigger/UCT2015

git clone https://github.com/uwcms/oRSCPatterns.git L1Trigger/oRSCPatterns

cd $CMSSW_BASE/src
scram b -j 4
```

###Running
This can run over multiple events and produce a single text file: `output.txt`. The file contains the data represeting all 36 links for the full RCT system.
```sh
cd L1Trigger/oRSCPatterns/test
cmsRun makeOrscLinkPatterns.py
```

###Output Format
The bit-fields that define the layout of the bits are defined in `src/OrscLinks.cc` as two tables: one for each link. The output text file contains 18 crates x 2 links = 36 lines per event. Each line contains 24 8-bit words. The first word printed corresponds to the top row of the bit-field in `src/OrscLinks.cc`, with the least-significant bit being the rightmost entry in the bit-field. An example output file is here: `doc/example_output.txt`. The file can contain multiple events, which are indicated by the event's event, run, and lumi number.
