#ifndef _ORSCLINKS_h
#define _ORSCLINKS_h

#include <iostream>
#include <stdint.h>

#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegion.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloEmCand.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"


class OrscLinks {
  private:
    uint8_t ElecBC0, JetBC0;

    // Index: RC number, region, bit number
    uint8_t RC[7][2][10];

    // Index: RC number, region
    uint8_t RCEtId[7][2];

    // Index: iso EG rank, bit number
    uint8_t IECard[4][3];

    // Index: iso EG rank
    uint8_t IEReg[4];

    // Index: iso EG rank, bit number
    uint8_t IE[4][6];

    // Index: non-iso EG rank, bit number
    uint8_t NECard[4][3];

    // Index: non-iso EG rank
    uint8_t NEReg[4];

    // Index: non-iso EG rank, bit number
    uint8_t NE[4][6];

    uint8_t Link1 [16][8];
    uint8_t Link2 [16][8];


  public:
    OrscLinks() {};
    ~OrscLinks() {};

    void addRegion(const L1CaloRegion &reg);

    void addEM(const L1CaloEmCand &cand);

    void populate_link_tables();
};

#endif
