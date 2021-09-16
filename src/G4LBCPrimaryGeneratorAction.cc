#include "G4LBCPrimaryGeneratorAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LBCPrimaryGeneratorAction::G4LBCPrimaryGeneratorAction()
{
    fParticleGPS = new G4GeneralParticleSource();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LBCPrimaryGeneratorAction::~G4LBCPrimaryGeneratorAction()
{
    delete fParticleGPS;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4LBCPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    fParticleGPS->GeneratePrimaryVertex(anEvent);
}
