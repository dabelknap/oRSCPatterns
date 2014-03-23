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
#include <stdexcept>
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
    // RC Et, Index: RC number, region, bit number
    uint8_t RCEt[7][2][10];

    // RC Tau Veto, Index: RC number, region
    uint8_t RCTau[7][2];

    // RC Overflow, Index: RC number, region
    uint8_t RCOf[7][2];

    // RC HAD Veto (MIP), Index: RC number, region
    uint8_t RCHad[7][2];

    // HF Finegrain, Index: Region
    uint8_t HFFg[8];

    // HF Et, Index: Region, bit number
    uint8_t HFEt[8][8];


    // Non-Iso EG position (0 Region, 1-3 card)
    // Index: candidate index, bit-number
    uint8_t NEPos[4][4];

    // Non-Iso EG Et, Index: cand. index, bit number
    uint8_t NEEt[4][6];

    // Iso EG position (0 Region, 1-3 card)
    // Index: candidate index, bit-number
    uint8_t IEPos[4][4];

    // Iso EG Et, Index: cand. index, bit number
    uint8_t IEEt[4][6];


    uint8_t Link1 [24][8];
    uint8_t Link2 [24][8];

    bool table_filled;


  public:
    OrscLinks() { table_filled = false; };
    ~OrscLinks() {};

    void populate_link_tables();

    void addRegion(const L1CaloRegion &reg);

    void addEM(const L1CaloEmCand &cand);

    void print_table(int link);

    std::vector<uint32_t> link_values(int link);
};

#endif
