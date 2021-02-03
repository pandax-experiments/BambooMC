#include "BambooControl.hh"
#include "BambooDetectorConstruction.hh"
#include "BambooFactory.hh"
#include "BambooUtils.hh"
#include "analysis/BambooAnalysis.hh"

#include <algorithm>
#include <iostream>
#include <memory>
#include <unistd.h>

#include <QFile>
#include <QXmlStreamReader>

#include <G4PhysListFactory.hh>
#include <G4VUserPhysicsList.hh>

bool BambooParameters::addParameter(const std::string &name,
                                    const std::string &value) {
    bool overwrite{false};
    if (parameters.find(name) != parameters.end())
        overwrite = true;
    parameters[name] = value;
    return overwrite;
}

const std::string
BambooParameters::getParameter(const std::string &name) const {
    if (parameters.find(name) != parameters.end())
        return parameters.at(name);
    return "";
}

double BambooParameters::evaluateParameter(const std::string &name) const {
    if (parameters.find(name) != parameters.end())
        return BambooUtils::evaluate(parameters.at(name));
    return 0;
}

void BambooControl::setup(int argc, char *argv[]) {
    extern char *optarg;
    std::stringstream ss;
    while (true) {
        const int option = getopt(argc, argv, "m:n:o:c:x:i");
        if (option == -1)
            break;
        switch (option) {
        case 'm':
            mac_file_name = optarg;
            break;
        case 'n':
            ss.clear();
            ss.str(optarg);
            ss >> num_events;
            break;
        case 'o':
            data_file_name = optarg;
            break;
        case 'c':
        case 'x':
            config_file_name = optarg;
            //            if (!bgv.loadXMLFile(config_file_name))
            if (!loadConfig(config_file_name))
                exit(1);
            break;
        case 'i':
            interactive = true;
            break;
        }
    }

    if (config_file_name.empty()) {
        std::cerr << "A config file must be provided! Try '-x xml_file'."
                  << std::endl;
        exit(1);
    }

    if (optind <= argc - 1) {
        std::cerr << optind << " vs " << argc - 1 << std::endl;
        std::cerr << "Unrecognized arguments!" << std::endl;
        exit(1);
    }
}

bool BambooControl::loadConfig(const std::string &config_name) {
    auto n = config_name.find_last_of('.');
    if (n == std::string::npos)
        return false;
    auto extension_name = config_name.substr(n + 1);
    if (extension_name == "xml") {
        std::cout << "load xml config" << std::endl;
        return loadXmlConfig(config_name);
    } else if (extension_name == "json") {
        std::cout << "load json config" << std::endl;
    } else if (extension_name == "yml" || extension_name == "yaml") {
        std::cout << "load yaml config" << std::endl;
    } else {
        std::cout << "not supported!" << std::endl;
        return false;
    }
    return false;
}

auto extract_attributes(QXmlStreamReader &reader, const std::string &name) {
    return reader.attributes().value(name.c_str()).toString().toStdString();
}

auto read_parameter(QXmlStreamReader &reader) {
    auto parameter = std::make_pair(extract_attributes(reader, "name"),
                                    extract_attributes(reader, "value"));
    return parameter;
}

void read_entry(QXmlStreamReader &reader, BambooParameters &pars) {
    while (reader.readNextStartElement()) {
        if (reader.name() == "parameter") {
            auto par = read_parameter(reader);
            pars.addParameter(par.first, par.second);
            reader.skipCurrentElement();
        }
    }
}

DetectorInfoTuple BambooControl::read_detector_xml(QXmlStreamReader &reader) {
    auto name = extract_attributes(reader, "name");
    auto type = extract_attributes(reader, "type");
    auto parent = extract_attributes(reader, "parent");
    if (name == "") {
        name = type;
    } else if (type == "") {
        type = name;
    }
    detectorParameterMaps.emplace(name, BambooParameters{});
    while (reader.readNextStartElement()) {
        auto par = read_parameter(reader);
        detectorParameterMaps[name].addParameter(par.first, par.second);
        reader.skipCurrentElement();
    }
    return std::make_tuple(name, type, parent);
}

void BambooControl::read_geometry_xml(QXmlStreamReader &reader) {
    std::set<DetectorInfoTuple> det_set;
    while (reader.readNextStartElement()) {
        if (reader.name() == "parameter") {
            auto par = read_parameter(reader);
            geometryParameters.addParameter(par.first, par.second);
            reader.skipCurrentElement();
        } else if (reader.name() == "material") {
            materialName = extract_attributes(reader, "name");
            read_entry(reader, materialParameters);
        } else if (reader.name() == "detector") {
            det_set.emplace(std::move(read_detector_xml(reader)));
        }
    }
    if (!sortDetectors(det_set)) {
        std::cerr << "invalid detector.." << std::endl;
        exit(1);
    }
}

bool BambooControl::loadXmlConfig(const std::string &config_name) {
    QFile file(QString(config_name.c_str()));
    if (!file.exists()) {
        std::cerr << "File " << config_name << " does not exist!" << std::endl;
        return false;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "can't open " << config_name << std::endl;
        return false;
    }
    QXmlStreamReader reader(&file);
    if (reader.readNextStartElement()) {
        if (reader.name() != "BambooMC") {
            std::cerr << "No entry for BambooMC!" << std::endl;
            return false;
        }
    } else {
        if (reader.hasError()) {
            std::cerr << "Error Code = " << reader.error() << std::endl;
        }
        std::cerr << "Can not read from file " << config_name << std::endl;
        return false;
    }
    while (reader.readNextStartElement()) {
        if (reader.name() == "run") {
            run_number = std::stoi(extract_attributes(reader, "number"));
            reader.skipCurrentElement();
        } else if (reader.name() == "geometry") {
            read_geometry_xml(reader);
        } else if (reader.name() == "physics") {
            physicsName = extract_attributes(reader, "name");
            read_entry(reader, physicsParameters);
        } else if (reader.name() == "generator") {
            generatorName = extract_attributes(reader, "name");
            read_entry(reader, generatorParameters);
        } else if (reader.name() == "analysis") {
            analysisName = extract_attributes(reader, "name");
            read_entry(reader, analysisParameters);
        } else {
            reader.skipCurrentElement();
        }
    }
    return true;
}

bool BambooControl::sortDetectors(std::set<DetectorInfoTuple> &ds) {
    std::vector<std::string> names{{""}};
    using factory = DetectorFactory<std::string, BambooParameters>;
    auto detectorExist{true};
    while (!ds.empty()) {
        auto it = std::find_if(ds.cbegin(), ds.cend(),
                               [&names](const DetectorInfoTuple &t) {
                                   return std::get<2>(t) == names.back();
                               });
        if (it == ds.cend()) {
            if (names.back() == "")
                break;
            names.pop_back();
            continue;
        }
        names.emplace_back(std::get<0>(*it));
        if (detectorExist && !factory::exists(std::get<1>(*it))) {
            std::cerr << "detector type " << std::get<1>(*it) << " does not exist! " << std::endl;
            detectorExist = false;
        }
        detectorInfo.emplace_back(std::move(*it));
        ds.erase(it);
    }
    if (!ds.empty())
        return false;
    return detectorExist;
}

void BambooControl::print() const {
    std::cout << "run: " << run_number << std::endl;

    if (materialName != "") {
        std::cout << "material: " << materialName << std::endl;
    }
    for (const auto &di : detectorInfo) {
        const auto &name = std::get<0>(di);
        const auto &type = std::get<1>(di);
        const auto &parent = std::get<2>(di);
        std::cout << "detector: " << name << " (" << type << ")";
        if (parent != "") {
            std::cout << " (in " << parent << ")";
        }
        std::cout << std::endl;
        if (detectorParameterMaps.find(name) != detectorParameterMaps.end()) {
            const auto &pars = detectorParameterMaps.at(name);
            for (const auto &par : pars.getParameters()) {
                std::cout << " -- parameter: " << par.first << " = "
                          << par.second << std::endl;
            }
        }
    }

    std::cout << "physics: " << physicsName << std::endl;
    for (const auto &par : physicsParameters.getParameters()) {
        std::cout << "physics parameter: " << par.first << " = " << par.second
                  << std::endl;
    }
    std::cout << "generator: " << generatorName << std::endl;
    std::cout << "analysis: " << analysisName << std::endl;
    for (const auto &par : analysisParameters.getParameters()) {
        std::cout << "analysis parameter: " << par.first << " = " << par.second
                  << std::endl;
    }
}

G4VUserDetectorConstruction *BambooControl::createDetector() {
    return new BambooDetectorConstruction{materialName, materialParameters,
                                          geometryParameters, detectorInfo,
                                          detectorParameterMaps};
}

G4VUserPhysicsList *BambooControl::createPhysics() {
    if (physicsName.find("Physics") == std::string::npos) {
        return (new G4PhysListFactory())->GetReferencePhysList(physicsName);
    } else {
        if (!PhysicsFactory::exists(physicsName)) {
            std::cout << physicsName << " does not exist!" << std::endl;
        }
        auto physicsList =
            PhysicsFactory::create(physicsName, physicsParameters);
        if (physicsList.get() == nullptr) {
            return nullptr;
        }
        return static_cast<G4VUserPhysicsList *>(physicsList.release());
    }
}

G4VUserPrimaryGeneratorAction *BambooControl::createGenerator() {
    auto generator =
        GeneratorFactory::create(generatorName, generatorParameters);
    if (generator.get() == nullptr) {
        return nullptr;
    }
    return static_cast<G4VUserPrimaryGeneratorAction *>(generator.release());
}

std::unique_ptr<BambooAnalysis> BambooControl::createAnalysis() {
    return AnalysisFactory::create(analysisName, analysisParameters);
}
