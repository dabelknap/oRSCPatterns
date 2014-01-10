/**
 * Filename: OrscLinks.cc
 *
 * Description: Abstracts the JSC output bits for a single RCT crate to be
 *              arranged for use with the oRSC optical links.
 *
 * Author: D. Austin Belknap, UW-Madison
 */

#include "L1Trigger/UCT2015/interface/OrscLinks.h"


/**
 * Add a L1CaloRegion, and parse the bits
 */
void
OrscLinks::addRegion(const L1CaloRegion &reg) {
  unsigned card = reg.rctCard();
  unsigned region = reg.rctRegionIndex();
  unsigned et = reg.et();
  bool tau = reg.tauVeto();
  bool mip = reg.mip();
  bool overflow = reg.overFlow();

  // separate the 10 region bits
  for (int i = 0; i < 10; ++i) {
    if (overflow) {
      RC[card][region][i] = 0x1;
    }
    else {
      RC[card][region][i] = et & 0x1;
      et >>= 1;
    }
  }

  RCEtId[card][region] = (!mip && !tau) & 0x1;
}

/**
 * Add a L1CaloEmCand and parse the bits
 */
void
OrscLinks::addEM(const L1CaloEmCand &cand) {
  unsigned index = cand.index();
  unsigned card = cand.rctCard();
  unsigned region = cand.rctRegion();
  unsigned rank = cand.rank();
  bool iso = cand.isolated();

  // Separate and store the 6 rank bits, 3 card bits, region bit, for iso and
  // non-iso EM candidates.
  if (iso) {
    for (int i = 0; i < 6; ++i) {
      NE[index][i] = rank & 0x1;
      rank >>= 1;
    }
    for (int i = 0; i < 3; ++i) {
      NECard[index][i] = card & 0x1;
      card >>= 1;
    }
    NEReg[index] = region & 0x1;
  }
  else {
    for (int i = 0; i < 6; ++i) {
      IE[index][i] = rank & 0x1;
      rank >>= 1;
    }
    for (int i = 0; i < 3; ++i) {
      IECard[index][i] = card & 0x1;
      card >>= 1;
    }
    IEReg[index] = region & 0x1;
  }
}


/**
 * Output the link values as four 32-bit integers.
 */
std::vector<uint32_t>
OrscLinks::link_values(int link_number) {
  std::vector<uint32_t> link;

  uint32_t val = 0;

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 8; j++) {
      val <<= 1;

      if (i == 0) {
        continue;
      }

      if (link_number == 1) {
        val += Link1[i][j] & 0x1;
      }
      else if (link_number == 2) {
        val += Link2[i][j] & 0x1;
      }
      else {
        perror("Invalid link number given");
      }

      if (i % 4 == 3) {
        link.push_back(val);
        val = 0;
      }
    }
  }
  return link;
}


/**
 * Arrange the JSC output bits into the bitfields for the 2 oRSC optical links.
 */
void
OrscLinks::populate_link_tables() {
  uint8_t L1 [16][8] = {
    {0,             0,            0,            0,            0,            0,            0,            0},
    {ElecBC0,       JetBC0,       RC[0][0][5],  RC[0][0][4],  RC[0][0][3],  RC[0][0][2],  RC[0][0][1],  RC[0][0][0]},
    {IECard[0][0],  IEReg[0],     IE[0][5],     IE[0][4],     IE[0][3],     IE[0][2],     IE[0][1],     IE[0][0]},
    {RC[1][0][1],   RC[1][0][0],  RC[0][0][9],  RC[0][0][8],  RC[0][0][7],  RC[0][0][6],  IECard[0][2], IECard[0][1]},
    {IECard[1][0],  IEReg[1],     IE[1][5],     IE[1][4],     IE[1][3],     IE[1][2],     IE[1][1],     IE[1][0]},
    {RC[1][0][7],   RC[1][0][6],  RC[1][0][5],  RC[1][0][4],  RC[1][0][3],  RC[1][0][2],  IECard[1][2], IECard[1][1]},
    {RC[2][0][5],   RC[2][0][4],  RC[2][0][3],  RC[2][0][2],  RC[2][0][1],  RC[2][0][0],  RC[1][0][9],  RC[1][0][8]},
    {RC[3][0][3],   RC[3][0][2],  RC[3][0][1],  RC[3][0][0],  RC[2][0][9],  RC[2][0][8],  RC[2][0][7],  RC[2][0][6]},
    {IECard[2][0],  IEReg[2],     IE[2][5],     IE[2][4],     IE[2][3],     IE[2][2],     IE[2][1],     IE[2][0]},
    {RC[3][0][9],   RC[3][0][8],  RC[3][0][7],  RC[3][0][6],  RC[3][0][5],  RC[3][0][4],  IECard[2][2], IECard[2][1]},
    {IECard[3][0],  IEReg[3],     IE[3][5],     IE[3][4],     IE[3][3],     IE[3][2],     IE[3][1],     IE[3][0]},
    {RC[0][1][5],   RC[0][1][4],  RC[0][1][3],  RC[0][1][2],  RC[0][1][1],  RC[0][1][0],  IECard[3][2], IECard[3][1]},
    {RC[1][1][3],   RC[1][1][2],  RC[1][1][1],  RC[1][1][0],  RC[0][1][9],  RC[0][1][8],  RC[0][1][7],  RC[0][1][6]},
    {RC[2][1][1],   RC[2][1][0],  RC[1][1][9],  RC[1][1][8],  RC[1][1][7],  RC[1][1][6],  RC[1][1][5],  RC[1][1][4]},
    {RC[2][1][9],   RC[2][1][8],  RC[2][1][7],  RC[2][1][6],  RC[2][1][5],  RC[2][1][4],  RC[2][1][3],  RC[2][1][2]},
    {0,             RCEtId[3][0], RCEtId[2][1], RCEtId[2][0], RCEtId[1][1], RCEtId[1][0], RCEtId[0][1], RCEtId[0][0]}
  };

  uint8_t L2 [16][8] = {
    {0,             0,            0,            0,            0,            0,            0,            0},
    {ElecBC0,       JetBC0,       RC[4][0][5],  RC[4][0][4],  RC[4][0][3],  RC[4][0][2],  RC[4][0][1],  RC[4][0][0]},
    {NECard[0][0],  NEReg[0],     NE[0][5],     NE[0][4],     NE[0][3],     NE[0][2],     NE[0][1],     NE[0][0]},
    {RC[5][0][1],   RC[5][0][0],  RC[4][0][9],  RC[4][0][8],  RC[4][0][7],  RC[4][0][6],  NECard[0][2], NECard[0][1]},
    {NECard[1][0],  NEReg[1],     NE[1][5],     NE[1][4],     NE[1][3],     NE[1][2],     NE[1][1],     NE[1][0]},
    {RC[5][0][7],   RC[5][0][6],  RC[5][0][5],  RC[5][0][4],  RC[5][0][3],  RC[5][0][2],  NECard[1][2], NECard[1][1]},
    {RC[6][0][5],   RC[6][0][4],  RC[6][0][3],  RC[6][0][2],  RC[6][0][1],  RC[6][0][0],  RC[5][0][9],  RC[5][0][8]},
    {RC[3][1][3],   RC[3][1][2],  RC[3][1][1],  RC[3][1][0],  RC[6][0][9],  RC[6][0][8],  RC[6][0][7],  RC[6][0][6]},
    {NECard[2][0],  NEReg[2],     NE[2][5],     NE[2][4],     NE[2][3],     NE[2][2],     NE[2][1],     NE[2][0]},
    {RC[3][1][9],   RC[3][1][8],  RC[3][1][7],  RC[3][1][6],  RC[3][1][5],  RC[3][1][4],  NECard[2][2], NECard[2][1]},
    {NECard[3][0],  NEReg[3],     NE[3][5],     NE[3][4],     NE[3][3],     NE[3][2],     NE[3][1],     NE[3][0]},
    {RC[4][1][5],   RC[4][1][4],  RC[4][1][3],  RC[4][1][2],  RC[4][1][1],  RC[4][1][0],  NECard[3][2], NECard[3][1]},
    {RC[5][1][3],   RC[5][1][2],  RC[5][1][1],  RC[5][1][0],  RC[4][1][9],  RC[4][1][8],  RC[4][1][7],  RC[4][1][6]},
    {RC[6][1][1],   RC[6][1][0],  RC[5][1][9],  RC[5][1][8],  RC[5][1][7],  RC[5][1][6],  RC[5][1][5],  RC[5][1][4]},
    {RC[6][1][9],   RC[6][1][8],  RC[6][1][7],  RC[6][1][6],  RC[6][1][5],  RC[6][1][4],  RC[6][1][3],  RC[6][1][2]},
    {0,             RCEtId[6][1], RCEtId[6][0], RCEtId[5][1], RCEtId[5][0], RCEtId[4][1], RCEtId[4][0], RCEtId[3][1]}
  };

  // copy values into the class member arrays
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      Link1[i][j] = L1[i][j];
      Link2[i][j] = L2[i][j];
    }
  }

  table_filled = true;
}
