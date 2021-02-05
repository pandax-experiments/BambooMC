#!/usr/bin/perl -w

use strict;
use 5.010;
use File::Path qw(make_path);
say "Usage: $0 set_name detector_type" and die unless $#ARGV==1;

my $set_name = $ARGV[0];
my $detector_type = $ARGV[1];

say "creating new detector \"$detector_type\" in set \"$set_name\"";


# create the directory of set
make_path("user/detectors/$set_name", {mode => 0755}) unless -d "user/detectors/$set_name";

my $uc_name = $detector_type;
$uc_name =~ tr/a-z/A-Z/;

my $sample_header = <<"EOF";
#pragma once

#include "BambooControl.hh"
#include "BambooDetector.hh"
#include "BambooFactory.hh"

class $detector_type : public BambooDetector {

  public:
    $detector_type(const std::string &n, const BambooParameters &pars);

    virtual bool construct(const BambooParameters &global_pars, BambooDetector *parent);

    static DetectorRegister<$detector_type, std::string, BambooParameters> reg;

  private:
    // define additional parameters here
};

EOF

open my $header, ">user/detectors/${set_name}/${detector_type}.hh"
  or die "Can't open file: $!";

print $header $sample_header;
close $header;

my $sample_source = <<"EOF";
#include "${detector_type}.hh"

#include <G4Material.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>

DetectorRegister<$detector_type, std::string, BambooParameters> ${detector_type}::reg("$detector_type");

${detector_type}::${detector_type} (const std::string &n, const BambooParameters &pars)
  : BambooDetector(n, pars) {
    G4cout << "create detector $detector_type..." << G4endl;
}

bool ${detector_type}::construct (const BambooParameters &global_pars, BambooDetector *parent) {
    // add construction code here
    using namespace CLHEP;
    return true;
}

EOF

open my $source, ">user/detectors/${set_name}/${detector_type}.cc"
  or die "Can't open file: $!";

print $source $sample_source;
close $source;

# create the sources.cmake
my $cmake_file = "user/detectors/${set_name}/sources.cmake";
if ( -f $cmake_file ) {
  open my $fh_cmake, ">>$cmake_file" or die "Cannot open the cmake file: $!\n";
  say $fh_cmake qq(set(Detector \$\{Detector\} \$\{source_path\}/${detector_type}.cc));
  close $fh_cmake;
} else {
  open my $fh_cmake, ">$cmake_file" or die "Cannot open the cmake file: $!\n";
  say $fh_cmake qq(message("enable $set_name"));
  say $fh_cmake qq(set(source_path \$\{PROJECT_SOURCE_DIR\}/user/detectors/${set_name}));
  say $fh_cmake qq(set(Detector \$\{Detector\} \$\{source_path\}/${detector_type}.cc));
  close $fh_cmake;
}
