/**
 * Filename: OrscLinkPatterns.cc
 *
 * Description: This producer creates 18 "OrscLinks" objects (one for each RCT
 *              crate). It then loops over the Regions and EM Candidates from
 *              the RCT emulator and loads them into the OrscLink objects. The
 *              optical link layout of the bits are extracted from the OrscLink
 *              objects and loaded into a single text file.
 *
 * Author: D. Austin Belknap, UW-Madison
 */

// system include files
#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "stdint.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegion.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloEmCand.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"

#include "L1Trigger/CaloLinks/include/CaloLinks.h"


class OrscLinkPatterns : public edm::EDAnalyzer {
  public :
    explicit OrscLinkPatterns(const edm::ParameterSet&);
    ~OrscLinkPatterns();


  private:
    virtual void beginJob();
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob();

    edm::Handle<L1CaloRegionCollection> newRegions;
    edm::Handle<L1CaloEmCollection> newEMCands;

    std::ofstream outfile;

    void addRegion(CrateLinks& crateLink, const L1CaloRegion &reg);
    void addEM(CrateLinks& crateLink, const L1CaloEmCand &cand);
};


OrscLinkPatterns::OrscLinkPatterns(const edm::ParameterSet& iConfig) {
}


OrscLinkPatterns::~OrscLinkPatterns() {
}


void
OrscLinkPatterns::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using std::cout;
  using std::endl;

  using namespace edm;

  // Grab Region and EM collections
  iEvent.getByLabel("uctDigis", newRegions);
  iEvent.getByLabel("uctDigis", newEMCands);

  CaloLinks calolinks(iEvent.id().event(), iEvent.id().luminosityBlock(), iEvent.id().run());

  // Load each region into the OrscLink object of its crate
  for (L1CaloRegionCollection::const_iterator newRegion = newRegions->begin();
      newRegion != newRegions->end(); newRegion++) {
    addRegion(calolinks.get_crate(newRegion->rctCrate()), *newRegion);
  }

  // Load each EM Candidate into the OrscLink object of its crate
  for (L1CaloEmCollection::const_iterator egtCand = newEMCands->begin();
      egtCand != newEMCands->end(); egtCand++) {
    addEM(calolinks.get_crate(egtCand->rctCrate()), *egtCand);
  }

  // For each OrscLink crate object, print the optical link layouts into a text
  // file.
  calolinks.write_to_file(outfile);
}


void
OrscLinkPatterns::beginJob() {
  outfile.open("output.txt", std::ofstream::out);
}


void
OrscLinkPatterns::endJob() {
  outfile.close();
}


void
OrscLinkPatterns::addRegion(CrateLinks& crateLink, const L1CaloRegion &reg) {
  unsigned card   = reg.rctCard();
  unsigned region = reg.rctRegionIndex();
  unsigned et     = reg.et();

  bool tau        = reg.tauVeto();
  bool mip        = reg.mip();
  bool overflow   = reg.overFlow();
  bool finegrain  = reg.fineGrain();

  bool isHF       = reg.isHf();


  if (isHF) {
    crateLink.set_HFFg(region, finegrain & 0x1);

    for (int i = 0; i < 8; ++i) {
      crateLink.set_HFEt(region, i, et & 0x1);
      et >>= 1;
    }
  }
  else {
    crateLink.set_RCOf(card, region, overflow & 0x1);
    crateLink.set_RCTau(card, region, tau & 0x1);
    crateLink.set_RCHad(card, region, mip & 0x1);

    // separate the 10 region bits
    for (int i = 0; i < 10; ++i) {
      crateLink.set_RCEt(card, region, i, et & 0x1);
      et >>= 1;
    }
  }
}

/**
 * Add a L1CaloEmCand and parse the bits
 */
void
OrscLinkPatterns::addEM(CrateLinks& crateLink, const L1CaloEmCand &cand) {
  unsigned index = cand.index();
  unsigned card = cand.rctCard();
  unsigned region = cand.rctRegion();
  unsigned rank = cand.rank();
  bool iso = cand.isolated();

  // Separate and store the 6 rank bits, 3 card bits, region bit, for iso and
  // non-iso EM candidates.
  if (iso) {
    for (int i = 0; i < 6; ++i) {
      crateLink.set_NEEt(index, i, rank & 0x1);
      rank >>= 1;
    }

    crateLink.set_NEPos(index, 0, region & 0x1);
    for (int i = 1; i < 4; ++i) {
      crateLink.set_NEPos(index, i, card & 0x1);
      card >>= 1;
    }
  }
  else {
    for (int i = 0; i < 6; ++i) {
      crateLink.set_IEEt(index, i, rank & 0x1);
      rank >>= 1;
    }

    crateLink.set_IEPos(index, 0, region & 0x1);
    for (int i = 1; i < 4; ++i) {
      crateLink.set_IEPos(index, i, card & 0x1);
      card >>= 1;
    }
  }
}


DEFINE_FWK_MODULE(OrscLinkPatterns);
