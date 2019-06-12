#ifndef BAMBOOGLOBALVARIABLES_H
#define BAMBOOGLOBALVARIABLES_H

#include <exception>
#include <map>
#include <string>
#include <vector>

#include <G4String.hh>
#include <globals.hh>

#include "detector/BambooDetectorPart.hh"

using std::vector;
using std::map;
using std::string;

class BambooGlobalVariables;

class DetectorParameters {
    friend class BambooGlobalVariables;

  public:
    DetectorParameters() : isWorld{false} {}
    ~DetectorParameters();
    const string &getType() const;
    const string &getDetectorPartName() const;
    const string &getParentName() const;
    int getParameterAsInt(const string &parameter) const;
    double getParameterAsDouble(const string &parameter) const;
    string getParameterAsString(const string &parameter) const;
    map<string, string> &getParametersMap();
    bool isWorldPart() const { return isWorld; }
    void setWorld(bool t) { isWorld = t; }

  private:
    string type;
    string name;
    string parentName;
    bool isWorld;
    map<string, string> parameters;
};

inline const string &DetectorParameters::getType() const { return type; }

inline const string &DetectorParameters::getDetectorPartName() const {
    return name;
}

inline const string &DetectorParameters::getParentName() const {
    return parentName;
}

class QXmlStreamReader;

class BambooGlobalVariables {

  public:
    static BambooGlobalVariables *Instance();

    ~BambooGlobalVariables();

    bool loadXMLFile(const G4String &filename);

    const vector<DetectorParameters> &getDetectorParametersList();

    DetectorParameters &
    findDetectorPartParameters(const string &name);

    BambooDetectorPart *findDetectorPart(const string &name);

    void addDetectorPart(BambooDetectorPart *part);

    int getRunNumber() const;

    const string &getMaterialName();

    const string &getPhysicsName();

    const string &getGeneratorName();

    const string &getAnalysisName();

    int getGeometryParameterAsInt(const string &parameter) const;

    double getGeometryParameterAsDouble(const string &parameter) const;

    string getGeometryParameterAsString(const string &parameter) const;

    int getMaterialParameterAsInt(const string &parameter) const;

    double getMaterialParameterAsDouble(const string &parameter) const;

    string getMaterialParameterAsString(const string &parameter) const;

    int getPhysicsParameterAsInt(const string &parameter) const;

    double getPhysicsParameterAsDouble(const string &parameter) const;

    string getPhysicsParameterAsString(const string &parameter) const;

    int getAnalysisParameterAsInt(const string &parameter) const;

    double getAnalysisParameterAsDouble(const string &parameter) const;

    string getAnalysisParameterAsString(const string &parameter) const;

    int getGeneratorParameterAsInt(const string &parameter) const;

    double getGeneratorParameterAsDouble(const string &parameter) const;

    string getGeneratorParameterAsString(const string &parameter) const;

    const string &getOutDataName() const;

    void setOutDataName(const string &name);

    void setOutDataName(const char *name);

    double getUnitByName(const string &name);

  private:
    BambooGlobalVariables();

    bool loadDetectorPart(QXmlStreamReader &xs);

    bool loadDetectorParameter(QXmlStreamReader &xs);

    bool loadGeometryParameter(QXmlStreamReader &xs);

    bool loadMaterial(QXmlStreamReader &xs);

    bool loadPhysics(QXmlStreamReader &xs);

    bool loadMaterialParameter(QXmlStreamReader &xs);

    bool loadPhysicsParameter(QXmlStreamReader &xs);

    bool loadAnalysisParameter(QXmlStreamReader &xs);

    bool loadGeneratorParameter(QXmlStreamReader &xs);

    bool loadGenerator(QXmlStreamReader &xs);

    bool loadAnalysis(QXmlStreamReader &xs);

    bool validateDetector();

    int runNumber_;

    vector<DetectorParameters> detectorParametersList_;

    vector<BambooDetectorPart *> detectorPartList_;

    map<string, string> geometryParameters_;

    map<string, string> materialParameters_;

    map<string, string> physicsParameters_;

    map<string, string> analysisParameters_;

    map<string, string> generatorParameters_;

    static BambooGlobalVariables *instance_;

    string materialName_;

    string physicsName_;

    string generatorName_;

    string analysisName_;

    string outDataName_;

    bool readGeometry_;

    bool readDetector_;

    bool readMaterial_;

    bool readPhysics_;

    bool readAnalysis_;

    bool readGenerator_;
};

#endif
