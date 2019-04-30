#ifndef PANDAXDATAMANAGER_H
#define PANDAXDATAMANAGER_H

#include "analysis/PandaXRunAction.hh"

#include <map>
#include <string>
#include <vector>

class TFile;
class TTree;

class G4Event;

class TemporaryParticle {
  public:
    TemporaryParticle();
    ~TemporaryParticle();

    const std::string &getParticleType() const;
    void setParticleType(const std::string type);

    int getId() const;
    void setId(int i);

    double getEnergy() const;
    void setEnergy(double e);

    double getPx() const;
    void setPx(double v);

    double getPy() const;
    void setPy(double v);

    double getPz() const;
    void setPz(double v);

    double getX() const;
    void setX(double v);

    double getY() const;
    void setY(double v);

    double getZ() const;
    void setZ(double v);

  private:
    std::string _type;
    int _id;
    double _energy;
    double _px;
    double _py;
    double _pz;
    double _x;
    double _y;
    double _z;
};

class PandaXDataManager {

    friend class PandaXRunAction;
    friend class PandaXEventAction;
    friend class PandaXAnalysis;

  public:
    PandaXDataManager(bool enableEnergyDeposition = true,
                      bool enableFlatSurfaceFlux = false,
                      bool enablePrimaryParticle = false);

    ~PandaXDataManager();

    void book(const std::string &name = "pandaxout.root");

    void save();

    void saveNullEvent(bool t);

    void fillEvent(const G4Event *aEvent, bool partial = false);

    std::map<int, std::string> &getTrackMap();

    static PandaXDataManager *Instance();

    void addTemporaryParticle(const TemporaryParticle &tp);

  private:
    void resetData();

    //! reset the hits collection of the event
    void resetPartialEvent(const G4Event *aEvent);

    TFile *_rootFile;
    TTree *_mcTree;

    int _runId;
    int _eventId;
    int64_t seed_;

    // variables for the energy deposition.
    int _nHits;
    double _totalEnergy;
    std::vector<int> _trackId;
    std::vector<int> _parentId;
    std::vector<std::string> _type;
    std::vector<std::string> _parentType;
    std::vector<std::string> _creatorProcess;
    std::vector<std::string> _depositionProcess;
    std::vector<std::string> _volume;
    std::vector<double> _xd;
    std::vector<double> _yd;
    std::vector<double> _zd;
    std::vector<double> _td;
    std::vector<double> _energy;

    // variables for flux counts
    int _nTracks;
    std::vector<double> _trackEnergy;
    std::vector<std::string> _trackName;
    std::vector<std::string> _trackParent;
    std::vector<double> _px;
    std::vector<double> _py;
    std::vector<double> _pz;
    std::vector<double> track_time_;
    std::vector<double> track_x_;
    std::vector<double> track_y_;
    std::vector<double> track_z_;
    std::vector<int> _surface;

    // variables for primary particle
    int _nPrimaries;
    std::vector<std::string> _primaryType;
    std::vector<int> _primaryId;
    std::vector<double> _primaryEnergy;
    std::vector<double> _primaryPx;
    std::vector<double> _primaryPy;
    std::vector<double> _primaryPz;
    std::vector<double> _primaryX;
    std::vector<double> _primaryY;
    std::vector<double> _primaryZ;

    bool _recordEnergyDeposition;
    bool _recordFlatSurfaceFlux;
    bool _recordPrimaryParticle;
    bool _saveNullEvent;
    bool _partialSaved;

    std::vector<TemporaryParticle> _particles;
    std::vector<TemporaryParticle> _tparticles;

    std::map<int, std::string> _trackMap;

    static PandaXDataManager *_instance;
};
#endif
