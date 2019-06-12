#include "BambooGlobalVariables.hh"

#include <G4SystemOfUnits.hh>
#include <QFile>
#include <QXmlStreamReader>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

BambooGlobalVariables *BambooGlobalVariables::instance_ = 0;

DetectorParameters::~DetectorParameters() { parameters.clear(); }

int DetectorParameters::getParameterAsInt(const string &parameter) const {
    map<string, string>::const_iterator res = parameters.find(parameter);
    if (res != parameters.end()) {
        return QString(res->second.c_str()).toInt();
    }
    return 0;
}

double DetectorParameters::getParameterAsDouble(const string &parameter) const {
    map<string, string>::const_iterator res = parameters.find(parameter);
    if (res != parameters.end()) {
        return QString(res->second.c_str()).toDouble();
    }
    return 0;
}

string DetectorParameters::getParameterAsString(const string &parameter) const {
    map<string, string>::const_iterator res = parameters.find(parameter);
    if (res != parameters.end()) {
        return res->second;
    }
    return string{};
}

map<string, string> &DetectorParameters::getParametersMap() {
    return parameters;
}

BambooGlobalVariables::~BambooGlobalVariables() {
    detectorParametersList_.clear();
}

BambooGlobalVariables *BambooGlobalVariables::Instance() {
    if (instance_ == 0) {
        instance_ = new BambooGlobalVariables;
    }
    return instance_;
}

BambooGlobalVariables::BambooGlobalVariables()
    : runNumber_(0), physicsName_("SimpleUnderGroundPhysics"),
      generatorName_("SimpleGPSGenerator"), outDataName_("pandaxout.root"),
      readGeometry_(false), readDetector_(false), readMaterial_(false),
      readPhysics_(false), readAnalysis_(false) {}

bool BambooGlobalVariables::loadXMLFile(const G4String &filename) {
    QFile file(QString(filename.c_str()));
    if (!file.exists()) {
        cerr << "file " << filename << " does not exist!" << endl;
        return false;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cerr << "can't open " << filename << endl;
        return false;
    }
    QXmlStreamReader xs(&file);
    if (xs.readNextStartElement()) {
        if (xs.name() != "BambooMC") {
            cerr << "No entry for BambooMC!" << endl;
            return false;
        }
    } else {
        if (xs.hasError()) {
            cerr << "Error Code = " << xs.error() << endl;
        }
        cerr << "Can not read from file " << filename << endl;
        return false;
    }
    int nPhysics = 0;
    int nGenerator = 0;
    int nAnalysis = 0;
    readDetector_ = false;
    while (!xs.atEnd()) {
        xs.readNext();
        if (xs.isStartElement()) {
            if (xs.name() == "run") {
                runNumber_ = xs.attributes().value("number").toString().toInt();
            }
            if (xs.name() == "geometry") {
                readGeometry_ = true;
            }
            if (xs.name() == "detector") {
                if (!readGeometry_)
                    return false;
                readDetector_ = true;
                if (!loadDetectorPart(xs))
                    return false;
            }
            if (xs.name() == "parameter") {
                if (readDetector_) {
                    if (!loadDetectorParameter(xs))
                        return false;
                } else if (readGeometry_ && !readMaterial_) {
                    if (!loadGeometryParameter(xs)) {
                        return false;
                    }
                } else if (readMaterial_) {
                    if (!loadMaterialParameter(xs)) {
                        return false;
                    }
                } else if (readPhysics_) {
                    if (!loadPhysicsParameter(xs)) {
                        return false;
                    }
                } else if (readAnalysis_) {
                    if (!loadAnalysisParameter(xs)) {
                        return false;
                    }
                } else if (readGenerator_) {
                    if (!loadGeneratorParameter(xs)) {
                        return false;
                    }
                } else {
                    return false;
                }
            }
            if (xs.name() == "material") {
                if (!readGeometry_)
                    return false;
                if (!loadMaterial(xs)) {
                    return false;
                }
                readMaterial_ = true;
            }
            if (xs.name() == "physics") {
                if (!loadPhysics(xs))
                    return false;
                readPhysics_ = true;
                nPhysics++;
            }
            if (xs.name() == "generator") {
                if (!loadGenerator(xs))
                    return false;
                readGenerator_ = true;
                nGenerator++;
            }
            if (xs.name() == "analysis") {
                if (!loadAnalysis(xs))
                    return false;
                readAnalysis_ = true;
                nAnalysis++;
            }
        } else if (xs.isEndElement()) {
            if (xs.name() == "geometry") {
                readGeometry_ = false;
                cout << "finished loading geometry from xml file." << endl
                     << endl;
            }
            if (xs.name() == "detector") {
                readDetector_ = false;
            }
            if (xs.name() == "material") {
                readMaterial_ = false;
            }
            if (xs.name() == "physics") {
                readPhysics_ = false;
            }
            if (xs.name() == "analysis") {
                readAnalysis_ = false;
            }
            if (xs.name() == "generator") {
                readGenerator_ = false;
            }
        }
    }
    if (!validateDetector()) {
        return false;
    }
    if (nPhysics != 1) {
        cerr << "1 (and ONLY 1) physics class should be specified!" << endl;
        return false;
    }
    if (nGenerator != 1) {
        cerr << "1 (and ONLY 1) generator class should be specified!" << endl;
        return false;
    }
    if (nAnalysis != 1) {
        cerr << "1 (and ONLY 1) analysis class should be specified!" << endl;
        return false;
    }
    if (materialName_.empty()) {
        materialName_ = "SampleMaterial";
    }
    return true;
}

const vector<DetectorParameters> &
BambooGlobalVariables::getDetectorParametersList() {
    return detectorParametersList_;
}

DetectorParameters &BambooGlobalVariables::findDetectorPartParameters(
    const string &name) {
    for (size_t i = 0; i < detectorParametersList_.size(); ++i) {
        if (detectorParametersList_[i].getDetectorPartName() == name) {
            return detectorParametersList_[i];
        }
    }
    throw string("Unknown detector part name: ") + name;
}

BambooDetectorPart *
BambooGlobalVariables::findDetectorPart(const string &name) {
    for (size_t i = 0; i < detectorPartList_.size(); ++i) {
        if (detectorPartList_[i]->getName() == name.c_str()) {
            return detectorPartList_[i];
        }
    }
    return 0;
}

void BambooGlobalVariables::addDetectorPart(BambooDetectorPart *part) {
    detectorPartList_.push_back(part);
}

int BambooGlobalVariables::getRunNumber() const { return runNumber_; }

const string &BambooGlobalVariables::getMaterialName() { return materialName_; }

const string &BambooGlobalVariables::getPhysicsName() { return physicsName_; }

const string &BambooGlobalVariables::getGeneratorName() {
    return generatorName_;
}

const string &BambooGlobalVariables::getAnalysisName() { return analysisName_; }

int BambooGlobalVariables::getGeometryParameterAsInt(
    const string &parameter) const {
    map<string, string>::const_iterator res =
        geometryParameters_.find(parameter);
    if (res != geometryParameters_.end()) {
        return QString(res->second.c_str()).toInt();
    }
    return 0;
}

double BambooGlobalVariables::getGeometryParameterAsDouble(
    const string &parameter) const {
    map<string, string>::const_iterator res =
        geometryParameters_.find(parameter);
    if (res != geometryParameters_.end()) {
        return QString(res->second.c_str()).toDouble();
    }
    return 0;
}

string BambooGlobalVariables::getGeometryParameterAsString(
    const string &parameter) const {
    map<string, string>::const_iterator res =
        geometryParameters_.find(parameter);
    if (res != geometryParameters_.end()) {
        return res->second;
    }
    return string("");
}

int BambooGlobalVariables::getMaterialParameterAsInt(
    const string &parameter) const {
    map<string, string>::const_iterator res =
        materialParameters_.find(parameter);
    if (res != materialParameters_.end()) {
        return QString(res->second.c_str()).toInt();
    }
    return 0;
}

double BambooGlobalVariables::getMaterialParameterAsDouble(
    const string &parameter) const {
    map<string, string>::const_iterator res =
        materialParameters_.find(parameter);
    if (res != materialParameters_.end()) {
        return QString(res->second.c_str()).toDouble();
    }
    return 0;
}

string BambooGlobalVariables::getMaterialParameterAsString(
    const string &parameter) const {
    map<string, string>::const_iterator res =
        materialParameters_.find(parameter);
    if (res != materialParameters_.end()) {
        return res->second;
    }
    return string("");
}

int BambooGlobalVariables::getPhysicsParameterAsInt(
    const string &parameter) const {
    map<string, string>::const_iterator res =
        physicsParameters_.find(parameter);
    if (res != physicsParameters_.end()) {
        return QString(res->second.c_str()).toInt();
    }
    return 0;
}

double BambooGlobalVariables::getPhysicsParameterAsDouble(
    const string &parameter) const {
    map<string, string>::const_iterator res =
        physicsParameters_.find(parameter);
    if (res != physicsParameters_.end()) {
        return QString(res->second.c_str()).toDouble();
    }
    return 0;
}

string BambooGlobalVariables::getPhysicsParameterAsString(
    const string &parameter) const {
    map<string, string>::const_iterator res =
        physicsParameters_.find(parameter);
    if (res != physicsParameters_.end()) {
        return res->second;
    }
    return string("");
}

int BambooGlobalVariables::getAnalysisParameterAsInt(
    const string &parameter) const {
    map<string, string>::const_iterator res =
        analysisParameters_.find(parameter);
    if (res != analysisParameters_.end()) {
        return QString(res->second.c_str()).toInt();
    }
    return 0;
}

double BambooGlobalVariables::getAnalysisParameterAsDouble(
    const string &parameter) const {
    map<string, string>::const_iterator res =
        analysisParameters_.find(parameter);
    if (res != analysisParameters_.end()) {
        return QString(res->second.c_str()).toDouble();
    }
    return 0;
}

string BambooGlobalVariables::getAnalysisParameterAsString(
    const string &parameter) const {
    map<string, string>::const_iterator res =
        analysisParameters_.find(parameter);
    if (res != analysisParameters_.end()) {
        return res->second;
    }
    return string("");
}

int BambooGlobalVariables::getGeneratorParameterAsInt(
    const string &parameter) const {
    map<string, string>::const_iterator res =
        generatorParameters_.find(parameter);
    if (res != generatorParameters_.end()) {
        return QString(res->second.c_str()).toInt();
    }
    return 0;
}

double BambooGlobalVariables::getGeneratorParameterAsDouble(
    const string &parameter) const {
    map<string, string>::const_iterator res =
        generatorParameters_.find(parameter);
    if (res != generatorParameters_.end()) {
        return QString(res->second.c_str()).toDouble();
    }
    return 0;
}

string BambooGlobalVariables::getGeneratorParameterAsString(
    const string &parameter) const {
    map<string, string>::const_iterator res =
        generatorParameters_.find(parameter);
    if (res != generatorParameters_.end()) {
        return res->second;
    }
    return string("");
}

const string &BambooGlobalVariables::getOutDataName() const {
    return outDataName_;
}

void BambooGlobalVariables::setOutDataName(const string &name) {
    outDataName_ = name;
}

void BambooGlobalVariables::setOutDataName(const char *name) {
    outDataName_ = name;
}

double BambooGlobalVariables::getUnitByName(const string &name) {
    if (name.compare("m") == 0) {
        return m;
    }
    if (name.compare("mm") == 0) {
        return mm;
    }
    if (name.compare("cm") == 0) {
        return cm;
    }
    return 0;
}

bool BambooGlobalVariables::loadDetectorPart(QXmlStreamReader &xs) {
    Q_ASSERT(xs.isStartElement() && xs.name() == "detector");
    DetectorParameters dp;
    dp.type = xs.attributes().value("type").toString().toStdString();
    if (xs.attributes().hasAttribute("name")) {
        dp.name = xs.attributes().value("name").toString().toStdString();
    } else {
        dp.name = dp.type;
    }
    if (!xs.attributes().value("parent").isEmpty()) {
        dp.parentName =
            xs.attributes().value("parent").toString().toStdString();
    }
    cout << "detector -- " << dp.name;
    if (!dp.parentName.empty())
        cout << " in " << dp.parentName;
    cout << endl;
    detectorParametersList_.push_back(dp);
    return true;
}

bool BambooGlobalVariables::loadDetectorParameter(QXmlStreamReader &xs) {
    Q_ASSERT(xs.isStartElement() && xs.name() == "parameter");
    if (detectorParametersList_.size() < 1)
        return false;
    DetectorParameters &dp =
        detectorParametersList_[detectorParametersList_.size() - 1];
    string name = xs.attributes().value("name").toString().toStdString();
    string value = xs.attributes().value("value").toString().toStdString();
    dp.parameters[name] = value;
    cout << " -- parameter: " << name << " => " << value << endl;
    return true;
}

bool BambooGlobalVariables::loadGeometryParameter(QXmlStreamReader &xs) {
    Q_ASSERT(xs.isStartElement() && xs.name() == "parameter");
    string name = xs.attributes().value("name").toString().toStdString();
    string value = xs.attributes().value("value").toString().toStdString();
    geometryParameters_[name] = value;
    cout << "geometry parameter: " << name << " => " << value << endl;
    return true;
}

bool BambooGlobalVariables::loadMaterial(QXmlStreamReader &xs) {
    Q_ASSERT(xs.isStartElement() && xs.name() == "material");
    materialName_ = xs.attributes().value("name").toString().toStdString();
    cout << endl << "material -- " << materialName_ << endl;
    return true;
}

bool BambooGlobalVariables::loadPhysics(QXmlStreamReader &xs) {
    Q_ASSERT(xs.isStartElement() && xs.name() == "physics");
    physicsName_ = xs.attributes().value("name").toString().toStdString();
    cout << endl << "physics -- " << physicsName_ << endl;
    return true;
}

bool BambooGlobalVariables::loadMaterialParameter(QXmlStreamReader &xs) {
    Q_ASSERT(xs.isStartElement() && xs.name() == "parameter");
    string name = xs.attributes().value("name").toString().toStdString();
    string value = xs.attributes().value("value").toString().toStdString();
    materialParameters_[name] = value;
    cout << "material parameter: " << name << " => " << value << endl;
    return true;
}

bool BambooGlobalVariables::loadPhysicsParameter(QXmlStreamReader &xs) {
    Q_ASSERT(xs.isStartElement() && xs.name() == "parameter");
    string name = xs.attributes().value("name").toString().toStdString();
    string value = xs.attributes().value("value").toString().toStdString();
    physicsParameters_[name] = value;
    cout << "physics parameter: " << name << " => " << value << endl;
    return true;
}

bool BambooGlobalVariables::loadAnalysisParameter(QXmlStreamReader &xs) {
    Q_ASSERT(xs.isStartElement() && xs.name() == "parameter");
    string name = xs.attributes().value("name").toString().toStdString();
    string value = xs.attributes().value("value").toString().toStdString();
    analysisParameters_[name] = value;
    cout << "analysis parameter: " << name << " => " << value << endl;
    return true;
}

bool BambooGlobalVariables::loadGeneratorParameter(QXmlStreamReader &xs) {
    Q_ASSERT(xs.isStartElement() && xs.name() == "parameter");
    string name = xs.attributes().value("name").toString().toStdString();
    string value = xs.attributes().value("value").toString().toStdString();
    generatorParameters_[name] = value;
    cout << "generator parameter: " << name << " => " << value << endl;
    return true;
}

bool BambooGlobalVariables::loadGenerator(QXmlStreamReader &xs) {
    Q_ASSERT(xs.isStartElement() && xs.name() == "generator");
    generatorName_ = xs.attributes().value("name").toString().toStdString();
    cout << endl << "generator -- " << generatorName_ << endl;
    return true;
}

bool BambooGlobalVariables::loadAnalysis(QXmlStreamReader &xs) {
    Q_ASSERT(xs.isStartElement() && xs.name() == "analysis");
    analysisName_ = xs.attributes().value("name").toString().toStdString();
    cout << endl << "analysis -- " << analysisName_ << endl;
    return true;
}

bool BambooGlobalVariables::validateDetector() {
    // loop over all detector parameters to see if the configuration is validate
    int nWorlds = 0;
    for (size_t i = 0; i < detectorParametersList_.size(); ++i) {
        if (detectorParametersList_[i].getParentName().empty()) {
            detectorParametersList_[i].setWorld(true);
            nWorlds++;
        }
    }
    if (nWorlds != 1) {
        cerr << "There are " << nWorlds << " worlds in the configuration."
             << endl;
        return false;
    }
    return true;
}
