#include "G4LBCPhysicsList.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4RadioactiveDecayPhysics.hh"

G4LBCPhysicsList::G4LBCPhysicsList() : G4VModularPhysicsList()
{
  RegisterPhysics(new G4RadioactiveDecayPhysics());
  RegisterPhysics(new G4EmStandardPhysics_option3());
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

G4LBCPhysicsList::~G4LBCPhysicsList(){}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void G4LBCPhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();

  return ;
}
