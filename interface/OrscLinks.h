/**
 * Filename: OrscLinks.h
 *
 * Description: Abstracts the JSC output bits for a single RCT crate to be
 *              arranged for use with the oRSC optical links.
 *
 * Author: D. Austin Belknap, UW-Madison
 */

#ifndef _ORSCLINKS_h
#define _ORSCLINKS_h

#include <iostream>
#include <stdint.h>

#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegion.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloEmCand.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"


/**
 * This class abstracts the JSC bits for a single RCT crate. It parses Regions
 * and EM Candidates from the RCT emulator and extracts the relevant bits. It
 * can then arrange the bits into the bitfields used for the oRSC optical links.
 */
class OrscLinks {
  private:
    uint8_t ElecBC0, JetBC0;

    // Index: RC number, region, bit number
    uint8_t RC[7][2][10];

    // Index: RC number, region
    uint8_t RCEtId[7][2];

    // Index: iso EG rank, bit number
    uint8_t IECard[4][3];

    // Index: iso EG index
    uint8_t IEReg[4];

    // Index: iso EG index, bit number
    uint8_t IE[4][6];

    // Index: non-iso EG index, bit number
    uint8_t NECard[4][3];

    // Index: non-iso EG index
    uint8_t NEReg[4];

    // Index: non-iso EG index, bit number
    uint8_t NE[4][6];

    uint8_t Link1 [16][8];
    uint8_t Link2 [16][8];

    bool table_filled;


  public:
    OrscLinks() { table_filled = false; };
    ~OrscLinks() {};

    void populate_link_tables();

    void addRegion(const L1CaloRegion &reg);

    void addEM(const L1CaloEmCand &cand);

    std::vector<uint32_t> link_values(int link);
};

#endif
