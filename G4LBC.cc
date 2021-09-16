/*
 Tissue Equivalent Proportional Counter - Geant4 implementation
 --------------------------------------------------------------
 Based on :
"The microdosimetric extension in TOPAS: Development and
 comparison with published data" Zhu et al. 2019

*/

// Geant4 core libs
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "Randomize.hh"

// User defined classes
#include "G4LBCDetectorConstruction.hh"
#include "G4LBCPhysicsList.hh"
#include "G4LBCPrimaryGeneratorAction.hh"
#include "G4LBCRunAction.hh"
#include "G4LBCEventAction.hh"

// STL libs
#include <random>
#include <ctime>
#include <chrono>

int main(int argc, char** argv)
{

  G4UIExecutive * ui = 0;
  if(argc == 1)
  {
    ui = new G4UIExecutive(argc, argv);
  }

  // Choose the random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  // Initialize SEED using current time since epoch
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  // Set HepRandom with time dependent seed --> Needed for Cloud Parallelization
  CLHEP::HepRandom::setTheSeed(seed);
  G4Random::setTheSeed(seed);

  // Construct the run manager
  G4RunManager * runManager = new G4RunManager;

  // Set Mandatory initialization classes
  // ---------------------------------------------------------------------------
  G4LBCDetectorConstruction * det  = new G4LBCDetectorConstruction();
  G4LBCPhysicsList          * phys = new G4LBCPhysicsList();

  runManager->SetUserInitialization(det);
  runManager->SetUserInitialization(phys);

  // Set Mandatory action classes
  // ---------------------------------------------------------------------------
  G4LBCPrimaryGeneratorAction * gen   = new G4LBCPrimaryGeneratorAction();
  G4LBCRunAction              * run   = new G4LBCRunAction();
  G4LBCEventAction            * event = new G4LBCEventAction();

  runManager->SetUserAction(gen);
  runManager->SetUserAction(run);
  runManager->SetUserAction(event);

  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // get the pointer to the User Interface Manager
  G4UImanager * uiManager = G4UImanager::GetUIpointer();

  if(!ui)
  {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    uiManager->ApplyCommand(command+fileName);
  }
  else
  {
    uiManager->ApplyCommand("/control/execute macro/vis/initialize_visualization.mac");
    ui->SessionStart();
    delete ui;
  }

  // Terminate application
  // All user action and initialization classes are freed by the runManager
  // so they should not be deleted in the main() program
  delete visManager;
  delete runManager;

  return 0;
}
