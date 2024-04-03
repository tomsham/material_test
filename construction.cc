#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction() {
	// Define required materials
	DefineMaterials();

	// The A is a placeholder for the user defined commands (fMessenger)
	G4int placeHolder = 0;

	// Material Test
	isMaterialTest = true;

	// Set the material for each logical volume
	matWorld = Vacuum; // Air or Vacuum;
	matTestBlock = Pb;

	logicPlaneDetector = NULL;

	// Parameters for adjusting size of the shape of predefined objects
	block_sideLength_half = 50/2*cm;				//All variables of container are made up.
	block_thickness_half = 75/2*cm;
	gap_distance = 1.*mm;					//Distance between two nearest plane detectors, also is the spacing of two plane detectors
	
	// These are user defined commands for use in User-Interface(UI) mode and batch mode(using macro file)
	fMessenger = new G4GenericMessenger(this, "/@MyDetector/", "Macros");
	fMessenger->DeclareProperty("control/execute region_setup.mac", placeHolder, "Set the active region (cylinder locate at origin, radius = 9.53/2*mm, half height = 0.0001*mm)");
	fMessenger->DeclareProperty("control/execute rebuild.mac",placeHolder,"Rebuild Selected Physical Volume inside a 1.5*1.5*1.5 m^3 Cubic World contains Air, its center is the origin");

	fMessenger = new G4GenericMessenger(this, "/@MyDetector/Container/", "Container control");
	fMessenger->DeclareProperty("isMaterialTest", isMaterialTest, "Construct the TestBlock and Plane Detectors.");
	fMessenger->DeclarePropertyWithUnit("gap_distance", "mm", gap_distance, "Set the spacing of Plane Detectors, default is = 0.02 mm");
}
MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials() {
	G4NistManager* nist = G4NistManager::Instance();
	G4double hc = 1239.841939 * eV;

	Air = nist->FindOrBuildMaterial("G4_AIR");
	Vacuum = nist->FindOrBuildMaterial("G4_Galactic");

	// Aerogel
	SiO2 = new G4Material("SiO2", 0.125*g/cm3, 2);
	SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
	SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

	H2O = new G4Material("H2O", 1.000*g/cm3, 2);
	H2O->AddElement(nist->FindOrBuildElement("H"), 2);
	H2O->AddElement(nist->FindOrBuildElement("O"), 1);

	C = nist->FindOrBuildElement("C");

	Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
	Aerogel->AddMaterial(SiO2, 62.5*perCent);
	Aerogel->AddMaterial(H2O, 37.4*perCent);
	Aerogel->AddElement(C, 0.1*perCent);

	G4double energyAerogel[2] = { hc/900, hc/200 };		// 900nm, 200nm
	G4double rindexAerogel[2] = { 1.1, 1.1 };
	G4MaterialPropertiesTable* mptAerogel = new G4MaterialPropertiesTable();
	mptAerogel->AddProperty("RINDEX", energyAerogel, rindexAerogel, 2);
	Aerogel->SetMaterialPropertiesTable(mptAerogel);
	// End Aerogel

	G4double rindexWorld[2] = {1.0, 1.0};
	G4MaterialPropertiesTable* mptAir = new G4MaterialPropertiesTable();
	mptAir->AddProperty("RINDEX", energyAerogel, rindexWorld, 2);
	Air->SetMaterialPropertiesTable(mptAir);

	//NaCl
	NaCl = new G4Material("NaCl", 2.16*g/cm3, 2);
	NaCl->AddElement(nist->FindOrBuildElement("Na"), 1);
	NaCl->AddElement(nist->FindOrBuildElement("Cl"), 4);
	// End NaCl

	// CsI
	CsI = nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");
	G4double energyCsI[1] = { hc/310 };					//310nm
	G4double rindexCsI[1] = { 1.95 };
	G4MaterialPropertiesTable* mptCsI = new G4MaterialPropertiesTable();
	mptCsI->AddProperty("RINDEX", energyCsI, rindexCsI, 1);

	/*
	//Scintillation Test (test data for CsI)
	G4double scintCsI[1] = { 0.1 };
	G4double fractionCsI[1] = { 1.0 };						//Intensity fraction of different wavelengths which are the component of scintillation intensity eg 30./keV
	mptCsI->AddConstProperty("RESOLUTIONSCALE", 1.);				// AddConstProperty for a number
	//mptCsI->AddConstProperty("SCINTILLATIONYIELD", 30./keV);
	mptCsI->AddProperty("SCINTILLATIONCOMPONENT1", energyCsI, fractionCsI, 1);	// AddProperty for a array
	mptCsI->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 1500.0*ns);	// Time constant is the decay time	
	mptCsI->AddConstProperty("SCINTILLATIONYIELD1", 30./keV);
	//End Scintillation Test
	*/

	CsI->SetMaterialPropertiesTable(mptCsI);

	Pb = nist->FindOrBuildMaterial("G4_Pb");
	/*Pb_ = nist->FindOrBuildElement("Pb");
	Pb = new G4Material("Pb_", 11.35*g/cm3, 1);
	Pb->AddElement(Pb_, 1.);*/
	// End CsI

	// Mylar
	Mylar = nist->FindOrBuildMaterial("G4_MYLAR");
	// End Mylar
	
	// Kapton
	Kapton = nist->FindOrBuildMaterial("G4_KAPTON");
	// End Kapton

	// Ti_
	Ti = nist->FindOrBuildElement("Ti");
	Ti_ = new G4Material("Ti_", 4.507*g/cm3, 1);	//The density of G4_Ti is 4.54*g/cm3
	Ti_->AddElement(Ti, 1.);
	// End Ti_

	// LAB
	C16H26 = new G4Material("C16H26", 0.856*g/cm3, 2);
	C16H26 -> AddElement(nist->FindOrBuildElement("C"), 16);
	C16H26 -> AddElement(nist->FindOrBuildElement("H"), 26);

	C17H28 = new G4Material("C17H28", 0.855*g/cm3, 2);
	C17H28 -> AddElement(nist->FindOrBuildElement("C"), 17);
	C17H28 -> AddElement(nist->FindOrBuildElement("H"), 28);

	C18H30 = new G4Material("C18H28", 0.856*g/cm3, 2);
	C18H30 -> AddElement(nist->FindOrBuildElement("C"), 18);
	C18H30 -> AddElement(nist->FindOrBuildElement("H"), 30);

	C19H32 = new G4Material("C19H32", 0.881*g/cm3, 2);
	C19H32 -> AddElement(nist->FindOrBuildElement("C"), 19);
	C19H32 -> AddElement(nist->FindOrBuildElement("H"), 32);

	LAB = new G4Material("LAB",  0.86*g/cm3, 4);
	LAB -> AddMaterial(C16H26, 6.98*perCent);
	LAB -> AddMaterial(C17H28, 30.6*perCent);
	LAB -> AddMaterial(C18H30, 45.0*perCent);
	LAB -> AddMaterial(C19H32, 17.4*perCent);

	G4double energyLAB[6] = { hc/627.3, hc/579.8, hc/547.7, hc/493.9, hc/438.5, hc/405.1};		// hc/wavelength(in nm)
	G4double rindexLAB[6] = { 1.4786, 1.48118, 1.48329, 1.48786, 1.49505, 1.50061 };
	G4MaterialPropertiesTable* mptLAB = new G4MaterialPropertiesTable();
	mptLAB->AddProperty("RINDEX", energyLAB, rindexLAB, 2);
	LAB->SetMaterialPropertiesTable(mptLAB);
	// End LAB

	// Acrylic, from examples>advanced>air_shower>UltraDetectorConstruction.cc
	Acrylic = new G4Material("Acrylic", 1.19*g/cm3, 3);
	Acrylic->AddElement(C, 5);
	Acrylic->AddElement(nist->FindOrBuildElement("H"), 8);
	Acrylic->AddElement(nist->FindOrBuildElement("O"), 2);
	// End Acrylic
}

// Construct All physical volumes
G4VPhysicalVolume* MyDetectorConstruction::Construct() {
	G4double xWorld = 0.75*m;
	G4double yWorld = 0.75*m;
	G4double zWorld = 0.75*m;

	// A cubic world with volume 1.5 m*1.5 m*1.5 m
	G4Box* solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);
	logicWorld = new G4LogicalVolume(solidWorld, matWorld, "logicWorld");
	physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

	if (isMaterialTest)
		ConstructMaterialTest();

	return physWorld;
}

// Set Sensitive Detector(SD) and Field
void MyDetectorConstruction::ConstructSDandField() {
	MySensentiveDetector* sensDet = new MySensentiveDetector("SensitiveDetector");

	// PlaneDetector
	if(logicPlaneDetector != NULL)
		logicPlaneDetector->SetSensitiveDetector(sensDet);
	// End PlaneDetector
}

// TestBlock and Detector
void MyDetectorConstruction::ConstructMaterialTest() {
	// TestBlock
	G4Box* solidTestBlock = new G4Box("solidTestBlock", block_sideLength_half, block_sideLength_half, block_thickness_half);
	logicTestBlock = new G4LogicalVolume(solidTestBlock, matTestBlock, "logicTestBlock");
	physTestBlock = new G4PVPlacement(0, G4ThreeVector(0.*m, 0.*m, block_thickness_half), logicTestBlock, "TestBlock", logicWorld, false, 0, true);
	// End TestBlock
	/*
	// PlaneDetector
	G4double plane_height_half = 1*nm/2;
	G4Box* solidPlaneDetector = new G4Box("solidPlaneDetector", block_sideLength_half, block_sideLength_half, plane_height_half);
	logicPlaneDetector = new G4LogicalVolume(solidPlaneDetector, matWorld, "logicPlaneDetector");
	
	// To generate planes (index = 0, 1, 2, ...,11) plane
	for (int i = 1; i < (int)(2*block_thickness_half/gap_distance); i++) {
		G4double pos_z = -block_thickness_half + gap_distance*(i);
		G4Translate3D transZ(G4ThreeVector(0.*m, 0.*m, pos_z));
		physPlaneDetector = new G4PVPlacement(transZ, logicPlaneDetector, "PlaneDetector", logicTestBlock, false, i, true);
	}
	// PlaneDetector
	*/
}
// End TestBlock and Detector