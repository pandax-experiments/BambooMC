#include "analysis/PandaXEnergyDepositionHit.hh"

G4Allocator<PandaXEnergyDepositionHit> *pandaXEnergyDepositionHitAllocator =
    new G4Allocator<PandaXEnergyDepositionHit>;

PandaXEnergyDepositionHit::PandaXEnergyDepositionHit()
    : _trackId(-1), _parentId(-1), _energy(0), _x(0), _y(0), _z(0),
      _type("unknown"), _creatorProcess("unknown"),
      _depositionProcess("unknown"), _parent("none") {}

PandaXEnergyDepositionHit::~PandaXEnergyDepositionHit() {}

const PandaXEnergyDepositionHit &PandaXEnergyDepositionHit::
operator=(const PandaXEnergyDepositionHit &right) {
    _trackId = right._trackId;
    _parentId = right._parentId;
    _energy = right._energy;
    _x = right._x;
    _y = right._y;
    _z = right._z;
    _t = right._t;
    _type = right._type;
    _creatorProcess = right._creatorProcess;
    _depositionProcess = right._depositionProcess;
    _parent = right._parent;
    return *this;
}

int PandaXEnergyDepositionHit::
operator==(const PandaXEnergyDepositionHit &right) const {
    return ((this == &right) ? 1 : 0);
}

int PandaXEnergyDepositionHit::getTrackId() const { return _trackId; }

int PandaXEnergyDepositionHit::getParentId() const { return _parentId; }

double PandaXEnergyDepositionHit::getEnergy() const { return _energy; }

double PandaXEnergyDepositionHit::getX() const { return _x; }

double PandaXEnergyDepositionHit::getY() const { return _y; }

double PandaXEnergyDepositionHit::getZ() const { return _z; }

double PandaXEnergyDepositionHit::getT() const { return _t; }

const string &PandaXEnergyDepositionHit::getType() const { return _type; }

const string &PandaXEnergyDepositionHit::getCreatorProcess() const {
    return _creatorProcess;
}

const string &PandaXEnergyDepositionHit::getDepositionProcess() const {
    return _depositionProcess;
}

const string &PandaXEnergyDepositionHit::getParent() const { return _parent; }

void PandaXEnergyDepositionHit::setTrackId(int id) { _trackId = id; }

void PandaXEnergyDepositionHit::setParentId(int id) { _parentId = id; }

void PandaXEnergyDepositionHit::setEnergy(double energy) { _energy = energy; }

void PandaXEnergyDepositionHit::setX(double x) { _x = x; }

void PandaXEnergyDepositionHit::setY(double y) { _y = y; }

void PandaXEnergyDepositionHit::setZ(double z) { _z = z; }

void PandaXEnergyDepositionHit::setT(double t) { _t = t; }

void PandaXEnergyDepositionHit::setType(const string &type) { _type = type; }

void PandaXEnergyDepositionHit::setCreatorProcess(const string &process) {
    _creatorProcess = process;
}

void PandaXEnergyDepositionHit::setDepositionProcess(const string &process) {
    _depositionProcess = process;
}

void PandaXEnergyDepositionHit::setParent(const string &parent) {
    _parent = parent;
}
