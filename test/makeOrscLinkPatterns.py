"""
This cfg file uses the RCT emulator to produce RCT digis which are arranged into
the bitfields for the oRSC optical links, which are output to two text files to
be loaded on two oRSCs.

Author: D. Austin Belknap, UW-Madison
"""

import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')

options.inputFiles = (
    'file:/hdfs/store/user/tapas/2012-08-01-CRAB_ZEESkim/skim_10_1_wd2.root')
options.maxEvents = 2
options.parseArguments()

process = cms.Process("ORSCPATTERNS")

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'POSTLS161_V12::All'

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
    )

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(options.inputFiles)
    )

process.load("L1Trigger.UCT2015.emulation_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")

process.patterns = cms.EDAnalyzer('OrscLinkPatterns')

process.pattern_sequence = cms.Sequence(
    process.emulationSequence *
    process.patterns)

process.p = cms.Path(process.pattern_sequence)
