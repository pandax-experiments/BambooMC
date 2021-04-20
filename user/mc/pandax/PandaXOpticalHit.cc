#include "PandaXOpticalHit.hh"

G4ThreadLocal G4Allocator<PandaXOpticalHit> *pandaXOpticalHitAllocator = nullptr;

const PandaXOpticalHit &PandaXOpticalHit::
operator=(const PandaXOpticalHit &right) {
    pmt_number = right.pmt_number;
    pos = right.pos;
    energy = right.energy;
    creatorProcess = right.creatorProcess;
    t_global = right.t_global;
    t_local = right.t_local;
    t_proper = right.t_proper;
    velocity = right.velocity;
    return *this;
}

G4int PandaXOpticalHit::operator==(const PandaXOpticalHit &right) const {
    return ((this == &right) ? 1 : 0);
}
