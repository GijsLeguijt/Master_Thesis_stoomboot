#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Cons.hh>
#include <G4Sphere.hh>
#include <G4Torus.hh>
#include <G4Orb.hh>
#include <G4Polyhedra.hh>
#include <G4Polycone.hh>
#include <G4Ellipsoid.hh>
#include <G4ExtrudedSolid.hh>  // SERENA
//#include <G4Trd.hh>
#include <G4UnionSolid.hh>
//#include <G4IntersectionSolid.hh>
#include <G4SubtractionSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4PVParameterised.hh>
#include <G4OpBoundaryProcess.hh>
#include <G4SDManager.hh>
#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <globals.hh>

#include <vector>
#include <numeric>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cassert>

using std::vector;
using std::stringstream;
using std::max;

#include "DetectorMessenger.hh"

#include "SensitiveDetector.hh"
#include "DetectorConstruction.hh"

#include "G4PhysicalVolumeStore.hh"

#include "TFile.h"
#include "TParameter.h"

using namespace CLHEP;

map<G4String, G4double> DetectorConstruction::m_hGeometryParameters;

DetectorConstruction::DetectorConstruction(G4String fName)
{
    m_pDetectorMessenger = new DetectorMessenger(this);
    detRootFile = fName;
    
    m_hSourcePosition = 0*cm;
    m_hNaIPosition    = 41.175*mm;

}

DetectorConstruction::~DetectorConstruction()
{
    delete m_pDetectorMessenger;
}


G4VPhysicalVolume*
DetectorConstruction::Construct()
{
    
    DefineMaterials();
    
    DefineGeometryParameters();
    
    // construction of the geometry
    ConstructLaboratory();
    // NaI detectors + source
    ConstructCollimatorSystem();
    // make the appropriate components active
    ConstructSD();
    
    PrintGeometryInformation();
    
    MakeDetectorPlots();
    
    return m_pLabPhysicalVolume;
}

void
DetectorConstruction::DefineMaterials()
{
    G4NistManager* pNistManager = G4NistManager::Instance();
    
    //================================== elements ===================================
    pNistManager->FindOrBuildElement("U");
//    G4Element *Xe = new G4Element("Xenon",     "Xe", 54., 131.293*g/mole);
//    G4Element *H  = new G4Element("Hydrogen",  "H",  1.,  1.0079*g/mole);
    G4Element *C  = new G4Element("Carbon",    "C",  6.,  12.011*g/mole);
    G4Element *N  = new G4Element("Nitrogen",  "N",  7.,  14.007*g/mole);
    G4Element *O  = new G4Element("Oxygen",    "O",  8.,  15.999*g/mole);
    G4Element *F  = new G4Element("Fluorine",  "F",  9.,  18.998*g/mole);
    //	G4Element *Al = new G4Element("Aluminium", "Al", 13., 26.982*g/mole);
    G4Element *Si = new G4Element("Silicon",   "Si", 14., 28.086*g/mole);
//    G4Element *Cr = new G4Element("Chromium",  "Cr", 24., 51.996*g/mole);
//    G4Element *Mn = new G4Element("Manganese", "Mn", 25., 54.938*g/mole);
    G4Element *Fe = new G4Element("Iron",      "Fe", 26., 55.85*g/mole);
//    G4Element *Ni = new G4Element("Nickel",    "Ni", 28., 58.693*g/mole);
    
    G4Element *Na = pNistManager->FindOrBuildElement("Na");
    G4Element *I  = pNistManager->FindOrBuildElement("I");
    //================================== materials ==================================
    
    //------------------------------------- air -------------------------------------
    //G4Material *Air = pNistManager->FindOrBuildMaterial("G4_AIR");
    //G4Material *Air = G4Material::GetMaterial("G4_AIR");
    
    
    //----------------------------------- vacuum ------------------------------------
    G4Material *Vacuum = new G4Material("Vacuum", 1.e-20*g/cm3, 2, kStateGas);
    Vacuum->AddElement(N, 0.755);
    Vacuum->AddElement(O, 0.245);
    
    //------------------------------------ water ------------------------
//    G4Material *Water = new G4Material("Water", 1.*g/cm3, 2, kStateLiquid);
//    Water->AddElement(H, 2);
//    Water->AddElement(O, 1);
    
    //------------------------------------ NaI   ------------------------
    G4Material *NaI = new G4Material("NaI", 3.67*g/cm3, 2, kStateSolid);
    NaI->AddElement(Na, 1);
    NaI->AddElement( I, 1);
    
    //------------------------------------ plastic -----------------------------------
//    G4Material* PE = new G4Material("PE", 1.0*g/cm3, 2, kStateSolid);
//    PE->AddElement(C, 2);
//    PE->AddElement(H, 4);
    //------------------------------------ lead --------------------------------------
//    G4Material *Pb = pNistManager->FindOrBuildMaterial("G4_Pb");
    //------------------------------- stainless steel -------------------------------
//    G4Material *SS304LSteel = new G4Material("SS304LSteel", 8.00*g/cm3, 5, kStateSolid);
//    SS304LSteel->AddElement(Fe, 0.65);
//    SS304LSteel->AddElement(Cr, 0.20);
//    SS304LSteel->AddElement(Ni, 0.12);
//    SS304LSteel->AddElement(Mn, 0.02);
//    SS304LSteel->AddElement(Si, 0.01);
    //------------------------------------ teflon -----------------------------------
    G4Material* Teflon = new G4Material("Teflon", 2.2*g/cm3, 2, kStateSolid);
    Teflon->AddElement(C, 0.240183);
    Teflon->AddElement(F, 0.759817);
    //------------------------------------ acrylic -----------------------------------
//    G4Material *Acrylic = new G4Material("Acrylic", 1.18*g/cm3, 3, kStateSolid, 168.15*kelvin, 1.5*atmosphere);
//    Acrylic->AddElement(C,5);
//    Acrylic->AddElement(H,8);
//    Acrylic->AddElement(O,2);
    
}

void
DetectorConstruction::DefineGeometryParameters()
{
    //================================== Laboratory =================================
    m_hGeometryParameters["LabSize"] = 0.5*m;
    //================================== NaI crystal ================================
    m_hGeometryParameters["NaI_crystal_R"] = 76*mm/2;
    m_hGeometryParameters["NaI_crystal_Z"] = 76*mm;
    //================================== Disk source ================================
    m_hGeometryParameters["SourceDisk_R"] = 76*mm/2;
    m_hGeometryParameters["SourceDisk_Z"] = 6*mm;
    // little sphere in middle of disk
    m_hGeometryParameters["SourceCore_R"] = 1.0*mm;
}

G4double
DetectorConstruction::GetGeometryParameter(const char *szParameter)
{
    return m_hGeometryParameters[szParameter];
}

void
DetectorConstruction::ConstructLaboratory()
{
    //================================== Laboratory =================================
    const G4double dLabHalfSize = 0.5*GetGeometryParameter("LabSize");
    
    //G4Material *Air = G4Material::GetMaterial("G4_AIR");
    G4NistManager* pNistManager = G4NistManager::Instance();

    G4Material *Air = pNistManager->FindOrBuildMaterial("G4_AIR");

    
    //G4Material *Air = G4Material::GetMaterial("G4_AIR");
    
    G4Box *pLabBox         = new G4Box("LabBox", dLabHalfSize, dLabHalfSize, dLabHalfSize);
    m_pLabLogicalVolume    = new G4LogicalVolume(pLabBox, Air, "LabVolume", 0, 0, 0);
    m_pLabPhysicalVolume   = new G4PVPlacement(0, G4ThreeVector(), m_pLabLogicalVolume, "Lab", 0, false, 0);
    m_pMotherLogicalVolume = m_pLabLogicalVolume;
    
    m_pLabLogicalVolume->SetVisAttributes(G4VisAttributes::Invisible);

}

void
DetectorConstruction::ConstructSD()
{
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4String SDname;
    
    // make NaI sensitive
    G4VSensitiveDetector* NaI_SD = new SensitiveDetector(SDname="/NaI");
    SDman->AddNewDetector(NaI_SD);
    m_pNaI_crystal1_LogicalVolume->SetSensitiveDetector(NaI_SD);
   // m_pNaI_crystal2_LogicalVolume->SetSensitiveDetector(NaI_SD);

}


void
DetectorConstruction::ConstructCollimatorSystem()
{
    //================================== NaI crystal =================================
    const G4double dCrystalRadius =     GetGeometryParameter("NaI_crystal_R");
    const G4double dCrystalHalfZ  = 0.5*GetGeometryParameter("NaI_crystal_Z");
    

    //================================== Materials ===================================
    G4Material *NaI = G4Material::GetMaterial("NaI");
//    G4Material *PE = G4Material::GetMaterial("PE");
    G4Material *Teflon = G4Material::GetMaterial("Teflon");
//    G4Material *Pb = G4Material::GetMaterial("G4_Pb");
    G4Material *Air = G4Material::GetMaterial("G4_AIR");

    // rotation matrix for the NaI crystal
    G4RotationMatrix *pRot = new G4RotationMatrix();
    pRot->rotateY(90.*deg);
    
    //================================== Construction =================================
    // make and place one NaI crystal
    G4Tubs *pNaI_crystal1			= new G4Tubs("NaI_crystal1",
													0.*cm,
													dCrystalRadius,
													dCrystalHalfZ,
													0.*deg, 360.*deg);
    m_pNaI_crystal1_LogicalVolume	= new G4LogicalVolume(pNaI_crystal1, NaI, "NaI_crystalTUBS", 0, 0, 0);
    m_pNaI_crystal1_PhysicalVolume 	= new G4PVPlacement(pRot, G4ThreeVector(m_hNaIPosition,0,0),
        m_pNaI_crystal1_LogicalVolume, "NaI_crystal1", m_pMotherLogicalVolume, false, 0);
    
    G4Tubs *pNaI_crystal2			= new G4Tubs("NaI_crystal2",
                                                 0.*cm,
                                                 dCrystalRadius,
                                                 dCrystalHalfZ,
                                                 0.*deg, 360.*deg);
    m_pNaI_crystal2_LogicalVolume	= new G4LogicalVolume(pNaI_crystal2, NaI, "NaI_crystalTUBS", 0, 0, 0);
    m_pNaI_crystal2_PhysicalVolume 	= new G4PVPlacement(pRot, G4ThreeVector(-m_hNaIPosition,0,0), m_pNaI_crystal2_LogicalVolume, "NaI_crystal2", m_pMotherLogicalVolume, false, 0);
    
    
    // make and place the source
    G4cout <<"DetectorConstruction::ConstructCollimatorSystem:: Source at "<<m_hSourcePosition/cm<<" cm" <<G4endl;
    G4Tubs *pSourceDisk         	 = new G4Tubs("SourceDisk",
    											0.*cm,
    											GetGeometryParameter("SourceDisk_R"),
    											GetGeometryParameter("SourceDisk_Z")/2,
    											0.*deg, 360.*deg);
    m_pSourceDisk_LogicalVolume 	 = new G4LogicalVolume(pSourceDisk, Teflon, "SourceDisk", 0, 0, 0);
    m_pSourceDisk_PhysicalVolume	 = new G4PVPlacement(pRot, G4ThreeVector(0,0,0), m_pSourceDisk_LogicalVolume,
                                                     "SourceDisk", m_pMotherLogicalVolume, false, 0);
    

    G4Orb *pSourceCore				 = new G4Orb("SourceCore",
    										GetGeometryParameter("SourceCore_R"));
    m_pSourceCore_LogicalVolume 	 = new G4LogicalVolume(pSourceCore, Teflon, "SourceCore", 0, 0, 0);
    m_pSourceCore_PhysicalVolume 	 = new G4PVPlacement(pRot, G4ThreeVector(0,0,0), m_pSourceCore_LogicalVolume,
                                                     "SourceCore", m_pSourceDisk_LogicalVolume, false, 0);

    // visibility
    G4Colour hTitaniumColor(0.600, 0.600, 0.600, 0.4);
    G4VisAttributes *pTitaniumVisAtt = new G4VisAttributes(hTitaniumColor);
    pTitaniumVisAtt->SetVisibility(true);
    m_pNaI_crystal1_LogicalVolume->SetVisAttributes(pTitaniumVisAtt);
    m_pNaI_crystal2_LogicalVolume->SetVisAttributes(pTitaniumVisAtt);
    m_pSourceDisk_LogicalVolume -> SetVisAttributes(pTitaniumVisAtt);

    G4cout <<"end sysdef"<<G4endl;
}


void
DetectorConstruction::PrintGeometryInformation()
{
    //================================== Water =================================
    //	const G4double dWaterMass = m_pWaterLogicalVolume->GetMass(false, false)/(1000.*kg);
    //	G4cout << "Water Mass:                               " << dWaterMass << " ton" << G4endl << G4endl;
}

void DetectorConstruction::MakeDetectorPlots()
{
    _fGeom = new TFile(detRootFile,"RECREATE");
    _detector = _fGeom->mkdir("detector");
    
    // store properties of the materials that are used
    StoreMaterialParameters();
    // store geometry parameters
    StoreGeometryParameters();
    
    _fGeom->Write();
    _fGeom->Close();
    
}

void DetectorConstruction::StoreGeometryParameters()
{
    // TDirectory for storage of teh geometry parameters
    TDirectory *_geometry = _detector->mkdir("geometry");
    _geometry->cd();
    
    // NaI crystal
    TParameter<double> *NaI_R = new TParameter<double>("NaI_crystal_R",GetGeometryParameter("NaI_crystal_R")/mm);
    NaI_R->Write();
    TParameter<double> *NaI_Z = new TParameter<double>("NaI_crystal_Z",GetGeometryParameter("NaI_crystal_Z")/mm);
    NaI_Z->Write();
    TParameter<double> *NaI_pos = new TParameter<double>("NaI_crystal_Position",m_hNaIPosition/mm);
    NaI_pos->Write();

    // Source
    TParameter<double> *Source_R = new TParameter<double>("SourceDisk_R",GetGeometryParameter("SourceDisk_R")/mm);
    Source_R->Write();
    TParameter<double> *Source_Z = new TParameter<double>("SourceDisk_Z",GetGeometryParameter("SourceDisk_Z")/mm);
    Source_Z->Write();
    TParameter<double> *Source_pos = new TParameter<double>("SourceDisk_Position",m_hSourcePosition/mm);
    Source_pos->Write();
    
    _fGeom->cd();
}


void DetectorConstruction::StoreMaterialParameters()
{
    
    // make a list of materials for graphs
    G4int nmaterial = G4Material::GetNumberOfMaterials();
    G4cout <<"MakeDetectorPlots:: Number of materials = "<<nmaterial<<G4endl;
    
    TDirectory *_materials = _detector->mkdir("materials");
    _materials->cd();
    
    //  for(G4int imat=0; imat<(G4int)matNames.size(); imat++){
    vector<TDirectory*> matdirs;
    
    for(G4int imat=0; imat<nmaterial; imat++){
        G4Material *mat = G4NistManager::Instance()->GetMaterial(imat);
        G4String matname = mat->GetName();
        G4cout <<"MakeDetectorPlots:: "<<matname<<G4endl;
        G4double T   = mat->GetTemperature();
        G4double rho = mat->GetDensity();
        G4double P   = mat->GetPressure();
        
        matdirs.push_back(_materials->mkdir(matname));
        matdirs[imat]->cd();
        TParameter<double> *TemperaturePar = new TParameter<double>("Temperature",T);
        TemperaturePar->Write();
        TParameter<double> *DensityPar     = new TParameter<double>("Density",rho / (g/cm3));
        DensityPar->Write();
        TParameter<double> *PressurePar    = new TParameter<double>("Pressure",P / bar);
        PressurePar->Write();
        // disect the material
        size_t nele = mat->GetNumberOfElements();
        G4ElementVector *elems       = (G4ElementVector*)mat->GetElementVector();
        G4double        *fractionVec = (G4double*)mat->GetFractionVector();
        
        for(size_t iele=0; iele<nele; iele++){
            G4String elname = (*elems)[iele]->GetName();
            G4double frac   = fractionVec[iele];
            //      G4cout <<iele<<" elem = "<<(*elems)[iele]->GetName()<<" f = "<<fractionVec[iele]<<G4endl;
            char  elFrac[100];
            sprintf(elFrac,"f_%s",elname.c_str());
            TParameter<double> *_fracPar = new TParameter<double>((G4String)elFrac,frac);
            _fracPar->Write();
        }
        
        
        _materials->cd();
    }
    
    _fGeom->cd();
}
