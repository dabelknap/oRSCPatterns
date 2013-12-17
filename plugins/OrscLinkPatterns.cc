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

  iEvent.getByLabel("uctDigis", newRegions);
  iEvent.getByLabel("uctDigis", newEMCands);

  cout << "Declare links" << endl;

  OrscLinks links;

  cout << "Parse regions" << endl;

  for (L1CaloRegionCollection::const_iterator newRegion = newRegions->begin();
      newRegion != newRegions->end(); newRegion++) {
    links.addRegion(*newRegion);
  }

  cout << "Parse EM" << endl;

  for (L1CaloEmCollection::const_iterator egtCand = newEMCands->begin();
      egtCand != newEMCands->end(); egtCand++) {
    links.addEM(*egtCand);
  }

  cout << "Parse links to vectors" << endl;

  links.populate_link_tables();

  std::vector<uint32_t> link1 = links.link_values(1);
  std::vector<uint32_t> link2 = links.link_values(2);

  outfile << "Link0 4";
  for (int i = 0; i < 4; ++i) {
    outfile << " " << std::hex << link1.at(i);
  }
  outfile << std::endl;
  outfile << "Link1 4";
  for (int i = 0; i < 4; ++i) {
    outfile << " " << std::hex << link2.at(i);
  }
  outfile << std::endl;
}


void
OrscLinkPatterns::beginJob() {
  outfile.open("example.txt");
}


void
OrscLinkPatterns::endJob() {
  outfile.close();
}


DEFINE_FWK_MODULE(OrscLinkPatterns);
