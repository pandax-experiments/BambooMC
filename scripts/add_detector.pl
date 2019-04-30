#!/usr/bin/perl -w

use strict;
use 5.010;
use File::Path qw(make_path);
say "Usage: $0 set_name detector_name" and die unless $#ARGV==1;

my $set_name = $ARGV[0];
my $detector_name = $ARGV[1];

say "creating new detector \"$detector_name\" in set \"$set_name\"";


# create the directory of set
make_path("detectors/$set_name", {mode => 0755}) unless -d "detectors/$set_name";

my $uc_name = $detector_name;
$uc_name =~ tr/a-z/A-Z/;

my $sample_header = <<"EOF";
#ifndef ${uc_name}_H
#define ${uc_name}_H

#include "detector/BambooDetectorPart.hh"
#include "detector/BambooDetectorFactory.hh"

class $detector_name : public BambooDetectorPart
{

public:

    $detector_name(const G4String &name);

    virtual G4bool construct();

    static DetectorRegister<$detector_name> reg;

private:

    // define additional parameters here
};

#endif // ${uc_name}_H
EOF

open my $header, ">detectors/${set_name}/${detector_name}.hh"
  or die "Can't open file: $!";

print $header $sample_header;
close $header;

my $sample_source = <<"EOF";
#include "$detector_name.hh"
#include "BambooGlobalVariables.hh"
#include "BambooUtils.hh"

#include <G4Material.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>

DetectorRegister<$detector_name> ${detector_name}::reg("$detector_name");

$detector_name::$detector_name (const G4String & name)
  : BambooDetectorPart(name)
{
    // read parameters from the input xml file
    DetectorParameters dp = BambooGlobalVariables::Instance()
        ->findDetectorPartParameters("$detector_name");
    G4cout << "$detector_name found..." << G4endl;
}

G4bool ${detector_name}::construct ()
{
    // add construction code here
    return true;
}

EOF

open my $source, ">detectors/${set_name}/${detector_name}.cc"
  or die "Can't open file: $!";

print $source $sample_source;
close $source;

# create the sources.cmake
my $cmake_file = "detectors/${set_name}/sources.cmake";
if ( -f $cmake_file ) {
  open my $fh_cmake, ">>$cmake_file" or die "Cannot open the cmake file: $!\n";
  say $fh_cmake qq(set(Detector \$\{Detector\} \$\{source_path\}/${detector_name}.cc));
  close $fh_cmake;
} else {
  open my $fh_cmake, ">$cmake_file" or die "Cannot open the cmake file: $!\n";
  say $fh_cmake qq(message("enable $set_name"));
  say $fh_cmake qq(set(source_path \$\{PROJECT_SOURCE_DIR\}/detectors/${set_name}));
  say $fh_cmake qq(set(Detector \$\{Detector\} \$\{source_path\}/${detector_name}.cc));
  close $fh_cmake;
}
