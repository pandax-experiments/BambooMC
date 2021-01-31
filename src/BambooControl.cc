#include "BambooControl.hh"
#include "BambooUtils.hh"

#include <iostream>
#include <unistd.h>

#include <QFile>
#include <QXmlStreamReader>


bool BambooParameters::addParameter(const std::string &name, const std::string &value) {
    bool overwrite{false};
    if (parameters.find(name) != parameters.end())
        overwrite = true;
    parameters[name] = value;
    return overwrite;
}

const std::string BambooParameters::getParameter(const std::string &name) const {
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
            //                exit(1);
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

void read_detector(QXmlStreamReader &reader) {
    while (reader.readNextStartElement()) {
        std::cout << "detector parameter: "
                  << extract_attributes(reader, "name") << std::endl;
        reader.skipCurrentElement();
    }
}

void read_geometry(QXmlStreamReader &reader, BambooParameters &pars) {
    while(reader.readNextStartElement()) {
        if(reader.name() == "parameter") {
            auto par = read_parameter(reader);
            pars.addParameter(par.first, par.second);
            reader.skipCurrentElement();
        } else if (reader.name() == "material") {
            reader.skipCurrentElement();
        } else if (reader.name() == "detector") {
            read_detector(reader);
        }
    }
}

void read_entry(QXmlStreamReader &reader, BambooParameters &pars) {
    while(reader.readNextStartElement()) {
        if(reader.name() == "parameter") {
            auto par = read_parameter(reader);
            pars.addParameter(par.first, par.second);
            reader.skipCurrentElement();
        }
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
        } else if (reader.name() == "geometry") {
            read_geometry(reader, geometryParameters);  
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
