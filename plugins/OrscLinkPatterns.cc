// system include files
#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
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

#include "L1Trigger/UCT2015/interface/OrscLinks.h"


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

    std::ofstream outfile1;
    std::ofstream outfile2;
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

  iEvent.getByLabel("uctDigis", newRegions);
  iEvent.getByLabel("uctDigis", newEMCands);

  OrscLinks link_crate[18];

  for (L1CaloRegionCollection::const_iterator newRegion = newRegions->begin();
      newRegion != newRegions->end(); newRegion++) {
    link_crate[newRegion->rctCrate()].addRegion(*newRegion);
  }

  for (L1CaloEmCollection::const_iterator egtCand = newEMCands->begin();
      egtCand != newEMCands->end(); egtCand++) {
    link_crate[egtCand->rctCrate()].addEM(*egtCand);
  }

  for (int i = 0; i < 18; ++i) {
    link_crate[i].populate_link_tables();

    std::vector<uint32_t> link1 = link_crate[i].link_values(1);
    std::vector<uint32_t> link2 = link_crate[i].link_values(2);

    if (i < 9) {
      outfile1 << "Link" << 2*i << " 4";
      for (int j = 0; j < 4; ++j) {
        outfile1 << " " << std::hex << link1.at(j) << std::dec;
      }
      outfile1 << std::endl;
      outfile1 << "Link" << 2*i+1 << " 4";
      for (int j = 0; j < 4; ++j) {
        outfile1 << " " << std::hex << link2.at(j) << std::dec;
      }
      outfile1 << std::endl;
    }
    else {
      int I = i % 9;
      outfile2 << "Link" << 2*I << " 4";
      for (int j = 0; j < 4; ++j) {
        outfile2 << " " << std::hex << link1.at(j) << std::dec;
      }
      outfile2 << std::endl;
      outfile2 << "Link" << 2*I+1 << " 4";
      for (int j = 0; j < 4; ++j) {
        outfile2 << " " << std::hex << link2.at(j) << std::dec;
      }
      outfile2 << std::endl;
    }
  }
}


void
OrscLinkPatterns::beginJob() {
  outfile1.open("example1.txt");
  outfile2.open("example2.txt");
}


void
OrscLinkPatterns::endJob() {
  outfile1.close();
  outfile2.close();
}


DEFINE_FWK_MODULE(OrscLinkPatterns);
