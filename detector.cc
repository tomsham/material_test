#include "detector.hh"

MySensentiveDetector::MySensentiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensentiveDetector::~MySensentiveDetector()
{}

/*	
In Geant4, 1 run contains number of events, 1 event contains number of tracks and steps.
The track stores current information of particle and updated after each step, the step stores changing information of particle.
Vertex that store the initial information of a particle.
New vertex can be created by existed track from physical processes.
*/

G4bool MySensentiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
	G4Track* track = aStep->GetTrack();
	//track->SetTrackStatus(fStopAndKill);

	//ReadOut(aStep, track);
	//SaveToDataFile(aStep, ROhist, track);
	/*
	G4String Self_Name = track->GetTouchable()->GetVolume()->GetName();

	G4double mom_z = track->GetMomentum().z();
	if (Self_Name == "PlaneDetector") {
		if (mom_z > 0)
			SaveToDataFile(aStep, ROhist, track);
		else
			track->SetTrackStatus(fStopAndKill);
	}
	*/
	return 0;
}

// This function store information to a Ntuple then it can be saved in run.cc
void MySensentiveDetector::SaveToDataFile(G4Step* aStep, G4TouchableHistory* ROhist, G4Track* track){
    G4AnalysisManager *man = G4AnalysisManager::Instance();
	G4String particle_type = track->GetParticleDefinition()->GetParticleType();						//Get the particle type
	G4String particle_name = track->GetDefinition()->GetParticleName();								//Get the particle name
	G4String detector_Name = track->GetTouchable()->GetVolume()->GetName();
	G4String creator_process_name = "NULL";
	if (track->GetCreatorProcess())
		creator_process_name = track->GetCreatorProcess()->GetProcessName();				//Get the process name of the vertex of track 

    std::string partical_name_list[] = {"e+", "e-", "gamma", "nu_e" };
    int length_partical_name_list = sizeof(partical_name_list)/sizeof(std::string);					// length_str = 7

    for (int i = 0; i < length_partical_name_list; i++)
    {
        if (particle_name == partical_name_list[i]) {
			G4double ekin = track->GetKineticEnergy();												//Get Kinetic Energy when it hit the detector
			G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
            G4ThreeVector prePosition = preStepPoint->GetPosition();								//Position when it pass the detector
            G4double x = prePosition.x(), y = prePosition.y(), z = prePosition.z();
            G4ThreeVector direction = preStepPoint->GetMomentumDirection();
            G4double theta = direction.theta(), phi = direction.phi(), costheta = direction.cosTheta();
            G4ThreeVector momentum = preStepPoint->GetMomentum();
            G4double px = momentum.x(); G4double py = momentum.y(); G4double pz = momentum.z();		//The momentum when it pass the detector
            man->FillNtupleDColumn(i+1, 0, ekin/MeV);	//MeV
            man->FillNtupleDColumn(i+1, 1, x/mm);		//mm
            man->FillNtupleDColumn(i+1, 2, y/mm);		//mm
            man->FillNtupleDColumn(i+1, 3, z/mm);		//mm
            man->FillNtupleDColumn(i+1, 4, px/MeV);		//MeV/c
            man->FillNtupleDColumn(i+1, 5, py/MeV);		//MeV/c
            man->FillNtupleDColumn(i+1, 6, pz/MeV);		//MeV/c
            man->FillNtupleDColumn(i+1, 7, theta);
			man->FillNtupleDColumn(i+1, 8, costheta);
            man->FillNtupleDColumn(i+1, 9, phi);
            man->FillNtupleSColumn(i+1, 10, creator_process_name);
            man->FillNtupleSColumn(i+1, 11, detector_Name);
            man->AddNtupleRow(i+1);
        }
    }
	man->FillNtupleSColumn(0, 0, particle_name);
    man->FillNtupleSColumn(0, 1, particle_type);
    man->FillNtupleSColumn(0, 2, creator_process_name);
    man->AddNtupleRow(0);
}

// Output Information just touch the detector
void MySensentiveDetector::ReadOut(G4Step* aStep, G4Track* track) {
    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
	G4String Particle_Name = track->GetDefinition()->GetParticleName();
	G4String creator_process_name = "NULL";
	if (track->GetCreatorProcess())
		creator_process_name = track->GetCreatorProcess()->GetProcessName();				//Get the process name of the vertex of track 
	G4String physVol_name = touchable->GetVolume()->GetName();
	G4String logicVol_name = touchable->GetVolume()->GetLogicalVolume()->GetName();
	G4String motherlogicVol_name = touchable->GetVolume()->GetMotherLogical()->GetName();
	G4ThreeVector posReal = aStep->GetPreStepPoint()->GetPosition();
	G4ThreeVector posDetector = touchable->GetVolume()->GetTranslation();
	G4double ekin = track->GetKineticEnergy();
	G4double theta = track->GetMomentumDirection().theta(), phi = track->GetMomentumDirection().phi();

	G4cout << "Particle Name is:" << Particle_Name << G4endl;
	G4cout << "CreatorProcess is:" << creator_process_name << G4endl;
	G4cout << "Kinetic Energy is:" << ekin/MeV << " MeV" << G4endl;
	G4cout << "Theta is:" << theta/degree << " degree" << G4endl;
	G4cout << "Phi is:" << phi/degree << " degree" << G4endl;
	G4cout << "Real position is:" << posReal/cm << " cm" << G4endl;
	G4cout << "Detector name is:" << physVol_name << G4endl;
	G4cout << "Mother Name is:" << track->GetTouchable()->GetVolume(1)->GetName() << G4endl;
	G4cout << "Mother LogicVol is:" << motherlogicVol_name << G4endl;
	G4cout << "Mother Material is:" << touchable->GetVolume()->GetMotherLogical()->GetMaterial()->GetName() << G4endl;
	G4cout << "PreStepPoint Mom is:" << aStep->GetPreStepPoint()->GetMomentum() << "MeV/c" << G4endl;
	G4cout << G4endl;
}