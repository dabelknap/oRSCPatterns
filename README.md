##oRSC Pattern Generator

###Installation

The current version of this code runs in `CMSSW_5_3_12` and `CMSSW_6_2_4`.
`CMSSW_5_3_12` is an SLC5 CMSSW area; at Wisconsin, you must use
`login02.hep.wisc.edu`.

```sh
scram project -n orsc5312 CMSSW CMSSW_5_3_12
cd orsc5312/src

cmsenv
git cms-init

git cms-addpkg DataFormats/L1CaloTrigger
git cms-addpkg L1TriggerConfig/L1ScalesProducers
git cms-addpkg L1Trigger/RegionalCaloTrigger

git clone https://github.com/dabelknap/CaloLinks.git L1Trigger/CaloLinks

git clone https://github.com/uwcms/UCT2015.git L1Trigger/UCT2015

git clone https://github.com/uwcms/oRSCPatterns.git L1Trigger/oRSCPatterns

cd $CMSSW_BASE/src
scram b -j 4
```

###Running
This runs over multiple events and produces a single file: `output.txt`. The
file contains the data for 18 optical links, represeting all 36 links for the
full RCT system. The data for multiple events are contained in a single file.
```sh
cd L1Trigger/oRSCPatterns/test
cmsRun makeOrscLinkPatterns.py
```

###Output Format
The bit-fields that define the layout of the bits are defined in
`src/OrscLinks.cc` as two tables: one for each link. The output text file
contains 18 crates x 2 links = 36 lines per event. Each column contains 6 32-bit
words. The first word printed corresponds to the top four rows of the bit-field
in `src/OrscLinks.cc`, with the least-significant bit being the bottom rightmost
entry in the bit-field. The most-significant bit is the top left bit.  An
example output file is here: `doc/example_output.txt`. The file can contain
multiple events, which are indicated by the event's event, run, and lumi number.
