#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH


#include "G4VUserDetectorConstruction.hh"

#include "globals.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Cons.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4GenericMessenger.hh"

#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "detector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	MyDetectorConstruction();
	~MyDetectorConstruction();

	virtual G4VPhysicalVolume* Construct();
	virtual void ConstructSDandField();
	void DefineMaterials();

	// TestBlock and Detector
	void ConstructMaterialTest();
	// End TestBlock and Detector

private:
	G4GenericMessenger* fMessenger;

	// Aerogel
	G4Material *Aerogel, *SiO2, *H2O;
	G4Element *C;

	// Materials
	G4Material *Ti_, *NaCl;;
	G4Element *Ti;
	
	G4Material *CsI, *Mylar;
	G4Material *Kapton, *Polystyrene;
	G4Material *C16H26, *C17H28, *C18H30, *C19H32, *LAB, *Acrylic;

	G4Material *Pb;
	G4Element *Pb_;
	// End Materials

	// World
	G4Material *Air, *Vacuum, *matWorld;
	G4LogicalVolume *logicWorld;
	G4VPhysicalVolume *physWorld;
	// End World
	
	// TestBlock and Detector
	G4Material *matTestBlock;
	G4bool isMaterialTest;
	G4double block_sideLength_half, block_thickness_half, gap_distance;
	G4LogicalVolume *logicTestBlock, *logicPlaneDetector;
	G4VPhysicalVolume *physTestBlock, *physPlaneDetector;
	// End TestBlock and Detector
};

#endif
