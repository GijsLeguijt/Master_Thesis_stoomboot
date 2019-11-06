#ifndef __DETECTORCONSTRUCTION_H__
#define __DETECTORCONSTRUCTION_H__

#include <globals.hh>

#include <vector>
#include <map>
#include "TFile.h"
#include "TDirectory.h"

using std::vector;
using std::map;

class G4Sphere;
class G4Colour;
class G4LogicalVolume;
class G4VPhysicalVolume;
class DetectorMessenger;
class G4SubtractionSolid;

#include <G4UnionSolid.hh>
#include <G4UImanager.hh>

#include <G4VUserDetectorConstruction.hh>

class DetectorConstruction: public G4VUserDetectorConstruction
{
public:
    DetectorConstruction(G4String fName);
    ~DetectorConstruction();
    
    G4VPhysicalVolume* Construct();
    static G4double GetGeometryParameter(const char *szParameter);
        
    
private:
    void DefineMaterials();
    void DefineGeometryParameters();
    
    void ConstructLaboratory();
    void ConstructCollimatorSystem();
    // define the sensitive detectors
    void ConstructSD();
    void PrintGeometryInformation();
    
    void MakeDetectorPlots();
    void StoreMaterialParameters();
    void StoreGeometryParameters();
    
    
    // == LOGICAL VOLUMES ==
    G4LogicalVolume *m_pMotherLogicalVolume;
    G4LogicalVolume *m_pLabLogicalVolume;
    
    // detector setup
    G4LogicalVolume *m_pTPC_LogicalVolume;
    G4LogicalVolume *m_pLXe_LogicalVolume;
    G4LogicalVolume *m_pCollimator_LogicalVolume;
        
    // == PHYSICAL VOLUMES ==
    G4VPhysicalVolume *m_pLabPhysicalVolume;

    // Collimator setup
    G4VPhysicalVolume *m_pTPC_PhysicalVolume;
    G4VPhysicalVolume *m_pLXe_PhysicalVolume;
    G4VPhysicalVolume *	m_pCollimator_PhysicalVolume;
    
    static map<G4String, G4double> m_hGeometryParameters;
    
    DetectorMessenger *m_pDetectorMessenger;

    // ROOT stuff
    TFile      *_fGeom;
    G4String    detRootFile;
    TDirectory *_detector;
    
};

#endif // __DETECTORCONSTRUCTION_H__

