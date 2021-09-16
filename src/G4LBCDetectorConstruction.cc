// User Defined Classes
#include "G4LBCDetectorConstruction.hh"
#include "G4LBCSensitiveDetector.hh"

// Geometry Related Libraries
#include "G4SDManager.hh"
#include "G4NistManager.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

// CGS Geometry Classes
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"

// Visualization Libraries
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4LBCDetectorConstruction::G4LBCDetectorConstruction()
: G4VUserDetectorConstruction(){}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

G4LBCDetectorConstruction::~G4LBCDetectorConstruction(){}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

G4VPhysicalVolume* G4LBCDetectorConstruction::Construct()
{

  /////////////////////////////////////////////////////////////////////////////
  // Materials Definition
  /////////////////////////////////////////////////////////////////////////////
  G4NistManager * nist = G4NistManager::Instance();

  G4Material * materialAir          = nist->FindOrBuildMaterial("G4_AIR");
  G4Material * materialAluminum     = nist->FindOrBuildMaterial("G4_Al");
  G4Material * materialTeflon       = nist->FindOrBuildMaterial("G4_TEFLON");
  G4Material * materialQuartz       = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

  G4Element * elLa = new G4Element("Lanthanum", "La", 57, 138.905*g/mole);
  G4Element * elBr = new G4Element("Bromine", "Br", 35, 79.904*g/mole);
  G4Element * elCl = new G4Element("Chlorine", "Cl", 17, 35.453*g/mole);

  G4double density     = 4.90 * g / cm3;
  G4int    ncomponents = 3;

  G4Material * materialLBC = new G4Material("LBC", density, ncomponents);
  materialLBC->AddElement(elLa, 25*perCent);
  materialLBC->AddElement(elBr, 71.25*perCent);
  materialLBC->AddElement(elCl, 3.75*perCent);

  /////////////////////////////////////////////////////////////////////////////
  // World Volume
  /////////////////////////////////////////////////////////////////////////////

  G4double world_half_Z  = 10*cm;
  G4double world_half_XY = 5*cm;

  G4Box * solidWorld = new G4Box("solidWorld", world_half_XY, world_half_XY, world_half_Z);
  G4LogicalVolume* logicalWorld = new G4LogicalVolume(solidWorld, materialAir, "logicalWorld", 0, 0, 0, true);

  G4VPhysicalVolume * physicalWorld = new G4PVPlacement(0,
                                                        G4ThreeVector(),
                                                        logicalWorld,
                                                        "physicalWorld",
                                                        0,
                                                        false,
                                                        0,
                                                        true);

  G4double spanning_angle = 360*deg;

  /////////////////////////////////////////////////////////////////////////////
  // Hermetic Sealing
  /////////////////////////////////////////////////////////////////////////////
  G4double pRmin = 0    * mm;
  G4double pRmax = 16.5 * mm;
  G4double halfZ = 15.0 * mm;

  G4Tubs * solid_HermeticSeal_side = new G4Tubs("solid_HermeticSeal_side",
                                                pRmin,
                                                pRmax,
                                                halfZ,
                                                0*deg,
                                                spanning_angle);

  pRmin = 0     * mm;
  pRmax = 15.25 * mm;
  halfZ = 3.2    * mm;

  G4Tubs * solid_HermeticSeal_window = new G4Tubs("solid_HermeticSeal_window",
                                                  pRmin,
                                                  pRmax,
                                                  halfZ,
                                                  0*deg,
                                                  spanning_angle);

  G4SubtractionSolid * solid_HermeticSeal =
    new G4SubtractionSolid("solid_HermeticSeal", solid_HermeticSeal_side, solid_HermeticSeal_window, 0, G4ThreeVector(0,0,-15*mm));

  G4LogicalVolume * logical_HermeticSeal =
    new G4LogicalVolume(solid_HermeticSeal, materialAluminum, "logicalHermeticSeal", 0, 0, 0, true);

  new G4PVPlacement(0,
                    G4ThreeVector(),
                    logical_HermeticSeal,
                    "physicalHermeticSeal",
                    logicalWorld,
                    false,
                    0,
                    true);

  /////////////////////////////////////////////////////////////////////////////
  // Reflector
  /////////////////////////////////////////////////////////////////////////////
  pRmin = 0     * mm;
  pRmax = 15.25 * mm;
  halfZ = 12.8  * mm;

  G4Tubs * solid_Reflector = new G4Tubs("solid_Reflector",
                                        pRmin,
                                        pRmax,
                                        halfZ,
                                        0*deg,
                                        spanning_angle);

  G4LogicalVolume * logical_Reflector =
    new G4LogicalVolume(solid_Reflector, materialTeflon, "logicalReflector", 0, 0, 0, true);

  new G4PVPlacement(0,
                    G4ThreeVector(0,0,1.4*mm),
                    logical_Reflector,
                    "physicalReflector",
                    logical_HermeticSeal,
                    false,
                    0,
                    true);

  /////////////////////////////////////////////////////////////////////////////
  // LBC Crystal
  /////////////////////////////////////////////////////////////////////////////
  pRmin = 0     * mm;
  pRmax = 12.5  * mm;
  halfZ = 12.5  * mm;

  G4Tubs * solid_LBC = new G4Tubs("solid_LBC",
                                        pRmin,
                                        pRmax,
                                        halfZ,
                                        0*deg,
                                        spanning_angle);

  G4LogicalVolume * logical_LBC =
    new G4LogicalVolume(solid_LBC, materialLBC, "logicalLBC", 0, 0, 0, true);

  new G4PVPlacement(0,
                    G4ThreeVector(0,0,0.3-0.6*mm),
                    logical_LBC,
                    "physicalLBC",
                    logical_Reflector,
                    false,
                    0,
                    true);

  /////////////////////////////////////////////////////////////////////////////
  // Optical Window
  /////////////////////////////////////////////////////////////////////////////
  pRmin = 0     * mm;
  pRmax = 15.25 * mm;
  halfZ = 0.2   * mm;

  G4Tubs * solid_OpticalWindow = new G4Tubs("solid_OpticalWindow",
                                        pRmin,
                                        pRmax,
                                        halfZ,
                                        0*deg,
                                        spanning_angle);

  G4LogicalVolume * logical_OpticalWindow =
    new G4LogicalVolume(solid_OpticalWindow, materialQuartz, "logicalOpticalWindow", 0, 0, 0, true);

  new G4PVPlacement(0,
                    G4ThreeVector(0,0,-15+3.4*mm),
                    logical_OpticalWindow,
                    "physicalOpticalWindow",
                    logical_HermeticSeal,
                    false,
                    0,
                    true);

  /////////////////////////////////////////////////////////////////////////////
  // Visualization Attributes
  /////////////////////////////////////////////////////////////////////////////
  logicalWorld->SetVisAttributes(G4VisAttributes::Invisible);
  logical_HermeticSeal->SetVisAttributes(new G4VisAttributes(G4Colour(102.0/255, 103.0/255, 105.0/255, 0.75)));
  logical_Reflector->SetVisAttributes(new G4VisAttributes(G4Colour(194.0/255, 194.0/255, 194.0/255, 0.2)));
  logical_LBC->SetVisAttributes(new G4VisAttributes(G4Colour(50.0/255, 103.0/255, 105.0/255, 0.4)));
  logical_OpticalWindow->SetVisAttributes(new G4VisAttributes(G4Colour(145.0/255, 135.0/255, 106/255, 0.4)));

  return physicalWorld;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void G4LBCDetectorConstruction::ConstructSDandField()
{
  G4LBCSensitiveDetector * sd = new G4LBCSensitiveDetector("LBC SD","LBCHitsCollection");

  G4SDManager::GetSDMpointer()->AddNewDetector(sd);
  SetSensitiveDetector("logicalLBC", sd);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
