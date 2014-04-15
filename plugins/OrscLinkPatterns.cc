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

#include "L1Trigger/oRSCPatterns/interface/OrscLinks.h"


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

  // Print event ID info
  outfile << iEvent.id() << endl;

  // Grab Region and EM collections
  iEvent.getByLabel("uctDigis", newRegions);
  iEvent.getByLabel("uctDigis", newEMCands);

  // Create OrscLink objects for each crate
  OrscLinks link_crate[18];

  // Load each region into the OrscLink object of its crate
  for (L1CaloRegionCollection::const_iterator newRegion = newRegions->begin();
      newRegion != newRegions->end(); newRegion++) {
    link_crate[newRegion->rctCrate()].addRegion(*newRegion);
  }

  // Load each EM Candidate into the OrscLink object of its crate
  for (L1CaloEmCollection::const_iterator egtCand = newEMCands->begin();
      egtCand != newEMCands->end(); egtCand++) {
    link_crate[egtCand->rctCrate()].addEM(*egtCand);
  }

  // For each OrscLink crate object, print the optical link layouts into a text
  // file.
  for (int i = 0; i < 18; ++i) {
    link_crate[i].populate_link_tables();

    std::vector<uint32_t> link1 = link_crate[i].link_values(1);
    std::vector<uint32_t> link2 = link_crate[i].link_values(2);

    // Print the contents of each link into the text file
    outfile << std::setw(2) << std::uppercase << std::setfill('0');
    outfile << "Crate " << std::setw(2) << i << " Link 1 ";

    for (int j = 0; j < 24; ++j) {
        outfile << " " << std::setw(2) << std::hex << int(link1.at(j)) << std::dec;
    }

    outfile << std::endl;
    outfile << "Crate " << std::setw(2) << i << " Link 2 ";

    for (int j = 0; j < 24; ++j) {
        outfile << " " << std::setw(2) << std::hex << int(link2.at(j)) << std::dec;
    }
    outfile << std::endl;
  }
}


void
OrscLinkPatterns::beginJob() {
  outfile.open("output.txt", std::ofstream::out);
}


void
OrscLinkPatterns::endJob() {
  outfile.close();
}


DEFINE_FWK_MODULE(OrscLinkPatterns);
