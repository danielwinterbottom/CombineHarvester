#ifndef HTTFits2016_HttSystematics_h
#define HTTFits2016_HttSystematics_h
#include "CombineHarvester/CombineTools/interface/CombineHarvester.h"

namespace ch {
// Implemented in src/HttSystematics.cc
void AddSystematics(CombineHarvester& cb, int control_region, bool inc_btag, bool inc_jes, bool inc_met);
}

#endif
