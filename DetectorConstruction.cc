#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4VSolid.hh"
#include "G4UnionSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4AssemblyVolume.hh"
#include "stdio.h"
//#include "CADMesh.hh"
#include "G4GDMLParser.hh"
#include <iostream>
#include "G4MaterialTable.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4VisAttributes.hh"
#include "G4IsotopeProperty.hh"

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{
}


DetectorConstruction::~DetectorConstruction()
{
}


G4VPhysicalVolume* DetectorConstruction::Construct(){

	G4NistManager* nist = G4NistManager::Instance();
 // physWorld->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::Invisible);
 
G4Material* world_mat = nist -> FindOrBuildMaterial("G4_AIR");


  G4double atomicNumber = 1.;
  G4double massOfMole = 1.008*g/mole;
  G4double density = 1.e-25*g/cm3;
  G4double temperature = 2.73*kelvin;
  G4double pressure = 3.e-18*pascal;
  G4Material* Vacuum = new G4Material("interGalactic",
                  atomicNumber, massOfMole, density,
                  kStateGas,temperature, pressure);
 // G4Material *world_mat = Vacuum;
//==============world definition==================================


  G4double world_size = 2000*mm;

  G4Box* solidWorld =
    new G4Box("World",                       // its name
              0.5*world_size,                // half x
              0.5*world_size,                // half y
              0.5*world_size);               // half z

  G4LogicalVolume* lv_wo=
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name


  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      lv_wo,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      true);                 //overlaps checking


//====================================================================


/*
 G4LogicalVolume* logicDetector =
    new G4LogicalVolume(solidDetector,detector_mat,"Detector");
    new G4PVPlacement(0,G4ThreeVector(0,0,-113.6),logicDetector,"Detector1",logicWorld,false,1,true);
    */

  G4Material* detector_mat = nist -> FindOrBuildMaterial("G4_Al");
  G4Material* Si_mat= nist -> FindOrBuildMaterial("G4_Si");
  G4Material* Pb_mat= nist -> FindOrBuildMaterial("G4_Pb");
  G4Element* element_La = new G4Element("lanthanum", "La",57., 138.90547*g/mole);
  G4Element* element_Br = new G4Element("bromine", "Br",35., 79.904*g/mole);
  G4Element* element_Ce = new G4Element("cerium", "Ce",58., 140.116*g/mole);
  G4Material* laBr3 = new G4Material("lanthanumbromide",5.06*g/cm3,2,kStateSolid,300*kelvin);
  G4Material* scin_mat = new G4Material("scintill",5.08*g/cm3,2,kStateSolid,300*kelvin);
  laBr3 -> AddElement(element_La,1);
  laBr3 -> AddElement(element_Br,3);
  scin_mat -> AddMaterial(laBr3,95*perCent);
  scin_mat -> AddElement(element_Ce, 5*perCent);

//==============================Si =============================//
/*
  G4Box* La=
    new G4Box("Si",10*0.5*mm,10*0.5*mm,10* 0.5*mm);               
  G4LogicalVolume* lv_La=
    new G4LogicalVolume(La,scin_mat,"lv_La");            
  G4VPhysicalVolume* pv_La=
    new G4PVPlacement(0,G4ThreeVector(0,0,0),lv_La,"pv_La",lv_wo,false,213455,true);                 


  G4Box* Si=
    new G4Box("Si",50*0.5*mm,50*0.5*mm,1* 0.5*mm);               
  G4LogicalVolume* lv_Si=
    new G4LogicalVolume(Si,Si_mat,"lv_Si");            
  G4VPhysicalVolume* pv_Si=
    new G4PVPlacement(0,G4ThreeVector(0,0,40),lv_Si,"pv_Si",lv_wo,false,117,true);                 

  G4Box* Al=
    new G4Box("Al",50*0.5*mm,50*0.5*mm,3* 0.5*mm);               
  G4LogicalVolume* lv_Al=
    new G4LogicalVolume(Al,detector_mat,"lv_Al");            
  G4VPhysicalVolume* pv_Al=
    new G4PVPlacement(0,G4ThreeVector(0,0,100),lv_Al,"pv_Al",lv_wo,false,130,true);                 

  G4Box* Si2=
    new G4Box("Si2",50*0.5*mm,50*0.5*mm,1* 0.5*mm);               
  G4LogicalVolume* lv_Si2=
    new G4LogicalVolume(Si2,Si_mat,"lv_Si2");            
  G4VPhysicalVolume* pv_Si2=
    new G4PVPlacement(0,G4ThreeVector(0,0,110),lv_Si2,"pv_Si2",lv_wo,false,127,true);                 


  G4Box* Si3=
    new G4Box("Si3",1*0.5*mm,50*0.5*mm,50* 0.5*mm);               
  G4LogicalVolume* lv_Si3=
    new G4LogicalVolume(Si3,Si_mat,"lv_Si3");            
  G4VPhysicalVolume* pv_Si3=
    new G4PVPlacement(0,G4ThreeVector(40,0,0),lv_Si3,"pv_Si3",lv_wo,false,217,true);                 

  G4Box* Al2=
    new G4Box("Al",3*0.5*mm,50*0.5*mm,50* 0.5*mm);               
  G4LogicalVolume* lv_Al2=
    new G4LogicalVolume(Al2,detector_mat,"lv_Al2");            
  G4VPhysicalVolume* pv_Al2=
    new G4PVPlacement(0,G4ThreeVector(100,0,0),lv_Al2,"pv_Al2",lv_wo,false,230,true);                 

  G4Box* Si4=
    new G4Box("Si2",1*0.5*mm,50*0.5*mm,50* 0.5*mm);               
  G4LogicalVolume* lv_Si4=
    new G4LogicalVolume(Si4,Si_mat,"lv_Si4");            
  G4VPhysicalVolume* pv_Si4=
    new G4PVPlacement(0,G4ThreeVector(110,0,0),lv_Si4,"pv_Si4",lv_wo,false,227,true);                 

  G4Box* Pb=
    new G4Box("Pb",0.5*0.5*mm,50*0.5*mm,50* 0.5*mm);               
  G4LogicalVolume* lv_Pb=
    new G4LogicalVolume(Pb,Pb_mat,"lv_Pb");            
  G4VPhysicalVolume* pv_Pb=
    new G4PVPlacement(0,G4ThreeVector(90,0,0),lv_Pb,"pv_Pb",lv_wo,false,200,true);                 


  G4Box* Al3=
    new G4Box("Al3",50*0.5*mm,50*0.5*mm,3* 0.5*mm);               
  G4LogicalVolume* lv_Al3=
    new G4LogicalVolume(Al3,detector_mat,"lv_Al3");            
  G4VPhysicalVolume* pv_Al3=
    new G4PVPlacement(0,G4ThreeVector(0,0,-100),lv_Al3,"pv_Al3",lv_wo,false,330,true);                 

  G4Box* Si5=
    new G4Box("Si5",50*0.5*mm,50*0.5*mm,1* 0.5*mm);               
  G4LogicalVolume* lv_Si5=
    new G4LogicalVolume(Si5,Si_mat,"lv_Si5");            
  G4VPhysicalVolume* pv_Si5=
    new G4PVPlacement(0,G4ThreeVector(0,0,-110),lv_Si5,"pv_Si5",lv_wo,false,317,true);                 
    
  G4VisAttributes* Att2= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  lv_Si->SetVisAttributes(Att2);
  lv_Si2->SetVisAttributes(Att2);
  lv_Si3->SetVisAttributes(Att2);
  lv_Si4->SetVisAttributes(Att2);
  lv_Si5->SetVisAttributes(Att2);

  */
//==============================================================================

//---------------------------------gdml------------------------------
/*
  G4VPhysicalVolume* physWorld;

  G4GDMLParser parser;
  //parser.Read("SuperClover_geo.gdml",false);
  //parser.Read("LaBr3_Array_ss.gdml",false);
  //parser.Read("LaBr3_Array_ss2.gdml",false);
  //parser.Read("LaBr3_detector.gdml",false);
  //parser.Read("/home/jang77893/LaBr3_bg/build/GDMLfile/LaBrArray_ss_final.gdml",false);
  parser.Read("LaBrArray_ss_final.gdml",false);
  physWorld = parser.GetWorldVolume();

  auto lv_wo= physWorld -> GetLogicalVolume();
  auto NofDau = lv_wo -> GetNoDaughters();



  //lv_wo->SetMaterial(world_mat);
  lv_wo->SetMaterial(world_mat);
  std::cout<<"world material : "<< lv_wo->GetMaterial()->GetName()<< std::endl;
  std::cout<<lv_wo->GetName()<<std::endl;
  std::cout<<"number of daughter : "<< NofDau<<std::endl;
  std::cout<<"name of daughter : " << lv_wo->GetDaughter(0)->GetName()<<std::endl; 
  auto lv_dau=lv_wo->GetDaughter(0)->GetLogicalVolume();
  lv_dau->SetMaterial(detector_mat);
  std::cout<<"supporting structure material : "<< lv_dau->GetMaterial()<<std::endl; 
  
  G4VisAttributes* Att2= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  lv_dau->SetVisAttributes(Att2);
*/
/*
  for(int i=0;i<25;i++)
  {

	  auto dau= lv_wo -> GetDaughter(i);
	  auto dauName= dau-> GetName();
	  int copyNo = dau-> GetCopyNo();
	  auto lv_dau = dau->GetLogicalVolume(); 
	  if(dauName=="LaBr3_24detector_Al_PV")
	  {
		  lv_dau->SetMaterial(detector_mat);
	  }
	  else
	  {
		  lv_dau->SetMaterial(scin_mat);
	  }
	  std::cout<<dauName<<" : " << lv_dau->GetMaterial()->GetName()<< std::endl;
 } 
  */

//----------------------------gdml--------------------------- 



  //G4LogicalVolume *Logic_World=physWorld->GetLogicalVolume();
  //Logic_World->SetMaterial(Vacuum);

int vol_count=0; 
/*

  G4Tubs* solidDetector =
    new G4Tubs("Detector",43.5/2*mm,44.5/2*mm,(148.5-38.7)/2*mm,0.*deg,360.*deg);
  G4Tubs* solidDetector1 =
    new G4Tubs("Detector1",42.1/2*mm,43.1/2*mm,38.7/2*mm,0.*deg,360.*deg);
  G4Tubs* solidDetector3 =
    new G4Tubs("Detector3",0*mm,42.1/2*mm,0.5/2*mm,0*deg,360.*deg);
  G4Tubs* solidDetector2 =
    new G4Tubs("Detector2",0*mm,38.1/2*mm,38.1/2*mm,0.*deg,360.*deg);
  G4Tubs* solidDetector4 =
    new G4Tubs("Detector4",0*mm,38.1/2*mm,1./2*mm,0.*deg,360.*deg);
  G4Tubs* solidDetector5 =
    new G4Tubs("Detector5",38.1/2*mm,42.1/2*mm,38.2/2*mm,0.*deg,360.*deg);
  G4Tubs* solidDetector6 =
    new G4Tubs("Detector6",43.1/2*mm,43.5/2*mm,0.5/2*mm,0.*deg,360.*deg);
  G4Tubs* solidDetector7 =
    new G4Tubs("Detector7",38.1/2*mm,43.5/2*mm,0.9/2*mm,0.*deg,360.*deg);
  G4Tubs* solidDetector8 =
    new G4Tubs("Detector8",0*mm,43.5/2*mm,(148.5-38.7-0.4)/2*mm,0.*deg,360.*deg);
//  G4Box* Al_out =

//  G4Box* Al_out2 =
//    new G4Box("Al_out2",43.1/2,5/2,(148.5*2+40)/2);

 G4LogicalVolume* logicDetector =
    new G4LogicalVolume(solidDetector,detector_mat,"Detector");
    new G4PVPlacement(0,G4ThreeVector(0,0,-113.6),logicDetector,"Detector1",logicWorld,false,1,true);

 G4LogicalVolume* logicDetector1 =
    new G4LogicalVolume(solidDetector1,detector_mat,"Detector1");
    new G4PVPlacement(0,G4ThreeVector(0,0,(74.25-113.6)),logicDetector1,"Detector1",logicWorld,false,1,true);

 G4LogicalVolume* logicDetector2 =
    new G4LogicalVolume(solidDetector2,scin_mat,"Detector2");
    new G4PVPlacement(0,G4ThreeVector(0,0,(73.05-113.6)),logicDetector2,"Detector2",logicWorld,false,100,true);

 G4LogicalVolume* logicDetector3 =
    new G4LogicalVolume(solidDetector3,detector_mat,"Detector3");
    new G4PVPlacement(0,G4ThreeVector(0,0,(93.6-0.25-113.6)),logicDetector3,"Detector3",logicWorld,false,1,true);

 G4LogicalVolume* logicDetector4 =
    new G4LogicalVolume(solidDetector4,Vacuum,"Detector4");
    new G4PVPlacement(0,G4ThreeVector(0,0,(92.6-113.6)),logicDetector4,"Detector4",logicWorld,false,1,true);
 G4LogicalVolume* logicDetector5 =
   new G4LogicalVolume(solidDetector5,Vacuum,"Detector5");
    new G4PVPlacement(0,G4ThreeVector(0,0,(54.9+19.1-113.6)),logicDetector5,"Detector5",logicWorld,false,1,true);
 G4LogicalVolume* logicDetector6 =
    new G4LogicalVolume(solidDetector6,detector_mat,"Detector6");
    new G4PVPlacement(0,G4ThreeVector(0,0,(54.9-0.25-113.6)),logicDetector6,"Detector6",logicWorld,false,1,true);
 G4LogicalVolume* logicDetector7 =
    new G4LogicalVolume(solidDetector7,Vacuum,"Detector7");
    new G4PVPlacement(0,G4ThreeVector(0,0,(54.9-0.9+0.2-113.6)),logicDetector7,"Detector7",logicWorld,false,1,true);
 G4LogicalVolume* logicDetector8 =
    new G4LogicalVolume(solidDetector8,Vacuum,"Detector8");
    new G4PVPlacement(0,G4ThreeVector(0,0,(-0.45-113.6)),logicDetector8,"Detector8",logicWorld,false,1,true);

 //G4LogicalVolume* logicAl_out =
 //   new G4LogicalVolume(Al_out,detector_mat,"logicAl_out");
 //   new G4PVPlacement(0,G4ThreeVector(0,43.1/2+2.5,0),logicAl_out,"Al_out",logicWorld,false,333,true);
 //G4LogicalVolume* logicAl_out2 =
 //   new G4LogicalVolume(Al_out2,detector_mat,"logicAl_out2");
 //   new G4PVPlacement(0,G4ThreeVector(0,-43.1/2-2.5,0),logicAl_out2,"Al_out2",logicWorld,false,333,true);






  G4Tubs* FsolidDetector =
    new G4Tubs("FDetector",43.5/2*mm,44.5/2*mm,(148.5-38.7)/2*mm,0.*deg,360.*deg);
  G4Tubs* FsolidDetector1 =
    new G4Tubs("FDetector1",42.1/2*mm,43.1/2*mm,38.7/2*mm,0.*deg,360.*deg);
  G4Tubs* FsolidDetector3 =
    new G4Tubs("FDetector3",0*mm,42.1/2*mm,0.5/2*mm,0*deg,360.*deg);
  G4Tubs* FsolidDetector2 =
  new G4Tubs("FDetector2",0*mm,38.1/2*mm,38.1/2*mm,0.*deg,360.*deg);
  G4Tubs* FsolidDetector4 =
    new G4Tubs("FDetector4",0*mm,38.1/2*mm,1./2*mm,0.*deg,360.*deg);
  G4Tubs* FsolidDetector5 =
    new G4Tubs("FDetector5",38.1/2*mm,42.1/2*mm,38.2/2*mm,0.*deg,360.*deg);
  G4Tubs* FsolidDetector6 =
    new G4Tubs("FDetector6",43.1/2*mm,43.5/2*mm,0.5/2*mm,0.*deg,360.*deg);
  G4Tubs* FsolidDetector7 =
    new G4Tubs("FDetector7",38.1/2*mm,43.5/2*mm,0.9/2*mm,0.*deg,360.*deg);
  G4Tubs* FsolidDetector8 =
    new G4Tubs("FDetector8",0*mm,43.5/2*mm,(148.5-38.7-0.4)/2*mm,0.*deg,360.*deg);

 G4LogicalVolume* FlogicDetector =
    new G4LogicalVolume(FsolidDetector,detector_mat,"FDetector");
    new G4PVPlacement(0,G4ThreeVector(0,0,113.6),FlogicDetector,"FDetector1",logicWorld,false,1,true);

 G4LogicalVolume* FlogicDetector1 =
    new G4LogicalVolume(FsolidDetector1,detector_mat,"FDetector1");
    new G4PVPlacement(0,G4ThreeVector(0,0,-(74.25-113.6)),FlogicDetector1,"FDetector1",logicWorld,false,1,true);

 G4LogicalVolume* FlogicDetector2 =
    new G4LogicalVolume(FsolidDetector2,scin_mat,"FDetector2");
    new G4PVPlacement(0,G4ThreeVector(0,0,-(73.05-113.6)),FlogicDetector2,"FDetector2",logicWorld,false,200,true);

 G4LogicalVolume* FlogicDetector3 =
    new G4LogicalVolume(FsolidDetector3,detector_mat,"FDetector3");
    new G4PVPlacement(0,G4ThreeVector(0,0,-(93.6-0.25-113.6)),FlogicDetector3,"FDetector3",logicWorld,false,1,true);

 G4LogicalVolume* FlogicDetector4 =
    new G4LogicalVolume(FsolidDetector4,Vacuum,"FDetector4");
    new G4PVPlacement(0,G4ThreeVector(0,0,-(92.6-113.6)),FlogicDetector4,"FDetector4",logicWorld,false,1,true);
 G4LogicalVolume* FlogicDetector5 =
    new G4LogicalVolume(FsolidDetector5,Vacuum,"FDetector5");
    new G4PVPlacement(0,G4ThreeVector(0,0,-(54.9+19.1-113.6)),FlogicDetector5,"FDetector5",logicWorld,false,1,true);
 G4LogicalVolume* FlogicDetector6 =
    new G4LogicalVolume(FsolidDetector6,detector_mat,"FDetector6");
    new G4PVPlacement(0,G4ThreeVector(0,0,-(54.9-0.25-113.6)),FlogicDetector6,"FDetector6",logicWorld,false,1,true);
 G4LogicalVolume* FlogicDetector7 =
    new G4LogicalVolume(FsolidDetector7,Vacuum,"FDetector7");
    new G4PVPlacement(0,G4ThreeVector(0,0,-(54.9-0.9+0.2-113.6)),FlogicDetector7,"FDetector7",logicWorld,false,1,true);
 G4LogicalVolume* FlogicDetector8 =
    new G4LogicalVolume(FsolidDetector8,Vacuum,"FDetector8");
    new G4PVPlacement(0,G4ThreeVector(0,0,-(-0.45-113.6)),FlogicDetector8,"FDetector8",logicWorld,false,1,true);



*/


/*
double a;

for(int i=0;i<8;i++)
{
for(int j=-1;j<2;j++)

{
  G4Tubs* FsolidDetector =
    new G4Tubs("FDetector",43.5/2*mm,44.5/2*mm,(148.5-38.7)/2*mm,0.*deg,360.*deg);
  G4Tubs* FsolidDetector1 =
    new G4Tubs("FDetector1",42.1/2*mm,43.1/2*mm,38.7/2*mm,0.*deg,360.*deg);
  G4Tubs* FsolidDetector3 =
    new G4Tubs("FDetector3",0*mm,42.1/2*mm,0.5/2*mm,0*deg,360.*deg);
  G4Tubs* FsolidDetector2 =
  new G4Tubs("FDetector2",0*mm,38.1/2*mm,38.1/2*mm,0.*deg,360.*deg);
  G4Tubs* FsolidDetector4 =
    new G4Tubs("FDetector4",0*mm,38.1/2*mm,1./2*mm,0.*deg,360.*deg);
  G4Tubs* FsolidDetector5 =
    new G4Tubs("FDetector5",38.1/2*mm,42.1/2*mm,38.2/2*mm,0.*deg,360.*deg);
  G4Tubs* FsolidDetector6 =
    new G4Tubs("FDetector6",43.1/2*mm,43.5/2*mm,0.5/2*mm,0.*deg,360.*deg);
  G4Tubs* FsolidDetector7 =
    new G4Tubs("FDetector7",38.1/2*mm,43.5/2*mm,0.9/2*mm,0.*deg,360.*deg);
  G4Tubs* FsolidDetector8 =
    new G4Tubs("FDetector8",0*mm,43.5/2*mm,(148.5-38.7-0.4)/2*mm,0.*deg,360.*deg);
 G4RotationMatrix* boxRotation = new G4RotationMatrix;
 boxRotation->rotateX(45*deg*i);
 boxRotation->rotateY(25*deg*j);

a=((113.6+70));
 G4LogicalVolume* FlogicDetector =
    new G4LogicalVolume(FsolidDetector,detector_mat,"FDetector");
    new G4PVPlacement(boxRotation,G4ThreeVector(-sin(25*j*deg)*a*1,(113.6+70)*sin(45*i*deg)*cos(24*j*deg),(113.6+70)*cos(45*i*deg)*cos(25*deg*j)),FlogicDetector,"FDetector",logicWorld,false,1,true);

a=-((74.25-(113.6+70)));
 G4LogicalVolume* FlogicDetector1 =
    new G4LogicalVolume(FsolidDetector1,detector_mat,"FDetector1");
    new G4PVPlacement(boxRotation,G4ThreeVector(-sin(25*j*deg)*a*1,-(74.25-(113.6+70))*sin(45*i*deg)*cos(24*j*deg),-(74.25-(113.6+70))*cos(45*i*deg)*cos(25*deg*j)),FlogicDetector1,"FDetector1",logicWorld,false,1,true);

a=-(((73.05-(113.6+70))));
 G4LogicalVolume* FlogicDetector2 =
    new G4LogicalVolume(FsolidDetector2,scin_mat,"FDetector2");
    new G4PVPlacement(boxRotation,G4ThreeVector(-sin(25*j*deg)*a*1,-(73.05-(113.6+70))*sin(45*i*deg)*cos(24*j*deg),-(73.05-(113.6+70))*cos(45*i*deg)*cos(25*deg*j)),FlogicDetector2,"FDetector2",logicWorld,false,200,true);

a=-(((93.6-0.25-(113.6+70))));
 G4LogicalVolume* FlogicDetector3 =
    new G4LogicalVolume(FsolidDetector3,detector_mat,"FDetector3");
    new G4PVPlacement(boxRotation,G4ThreeVector(-sin(25*j*deg)*a*1,-(93.6-0.25-(113.6+70))*sin(45*i*deg)*cos(24*j*deg),-(93.6-0.25-(113.6+70))*cos(45*i*deg)*cos(25*deg*j)),FlogicDetector3,"FDetector3",logicWorld,false,1,true);

a=-(((92.6-(113.6+70))));
 G4LogicalVolume* FlogicDetector4 =
    new G4LogicalVolume(FsolidDetector4,Vacuum,"FDetector4");
    new G4PVPlacement(boxRotation,G4ThreeVector(-sin(25*j*deg)*a*1,-(92.6-(113.6+70))*sin(45*i*deg)*cos(24*j*deg),-(92.6-(113.6+70))*cos(45*i*deg)*cos(25*deg*j)),FlogicDetector4,"FDetector4",logicWorld,false,1,true);
a=-(((54.9+19.1-(113.6+70))));
 G4LogicalVolume* FlogicDetector5 =
    new G4LogicalVolume(FsolidDetector5,Vacuum,"FDetector5");
    new G4PVPlacement(boxRotation,G4ThreeVector(-sin(25*j*deg)*a*1,-(54.9+19.1-(113.6+70))*sin(45*i*deg)*cos(24*j*deg),-(54.9+19.1-(113.6+70))*cos(45*i*deg)*cos(25*deg*j)),FlogicDetector5,"FDetector5",logicWorld,false,1,true);
a=-(((54.9-0.25-(113.6+70))));
 G4LogicalVolume* FlogicDetector6 =
    new G4LogicalVolume(FsolidDetector6,detector_mat,"FDetector6");
    new G4PVPlacement(boxRotation,G4ThreeVector(-sin(25*j*deg)*a*1,-(54.9-0.25-(113.6+70))*sin(45*i*deg)*cos(24*j*deg),-(54.9-0.25-(113.6+70))*cos(45*i*deg)*cos(25*deg*j)),FlogicDetector6,"FDetector6",logicWorld,false,1,true);
a=-(((54.9-0.9+0.2-(113.6+70))));
 G4LogicalVolume* FlogicDetector7 =
    new G4LogicalVolume(FsolidDetector7,Vacuum,"FDetector7");
    new G4PVPlacement(boxRotation,G4ThreeVector(-sin(25*j*deg)*a*1,-(54.9-0.9+0.2-(113.6+70))*sin(45*i*deg)*cos(24*j*deg),-(54.9-0.9+0.2-(113.6+70))*cos(45*i*deg)*cos(25*deg*j)),FlogicDetector7,"FDetector7",logicWorld,false,1,true);
a=-(((-0.45-(113.6+70))));
 G4LogicalVolume* FlogicDetector8 =
    new G4LogicalVolume(FsolidDetector8,Vacuum,"FDetector8");
    new G4PVPlacement(boxRotation,G4ThreeVector(-sin(25*j*deg)*a*1,-(-0.45-(113.6+70))*sin(45*i*deg)*cos(24*j*deg),-(-0.45-(113.6+70))*cos(45*i*deg)*cos(25*deg*j)),FlogicDetector8,"FDetector8",logicWorld,false,1,true);
}



}


*/

/*
//-----------------------------double ------------------------------
G4Tubs* Detector1 =
    new G4Tubs("Detector1",0*mm,43.1/2*mm,38.7/2*mm,0.*deg,360.*deg);
G4Tubs* Detector2 =
  new G4Tubs("Detector2",0*mm,38.1/2*mm,(38.7-1)/2*mm,0.*deg,360.*deg);

G4Tubs* scin =
    new G4Tubs("scin",0*mm,38.1/2*mm,38.1/2*mm,0.*deg,360.*deg);
G4SubtractionSolid* Detector_scin = 
    new G4SubtractionSolid("Detector_scin",Detector1,Detector2);
G4Tubs* Detector_pmt =
    new G4Tubs("Detectr_pmt",0*mm,44.5/2*mm,(148.5-38.7)/2*mm,0.*deg,360.*deg);

double a;
double distance=25;

		a=(distance+38.7/2+0.2);
		G4LogicalVolume* logic_scin =
			new G4LogicalVolume(scin,scin_mat,"scin");
			new G4PVPlacement(0,G4ThreeVector(0,0,a),logic_scin,"scin",logicWorld,false,200,true);
		a=(distance+38.7/2);
		G4LogicalVolume* logicDetector_scin =
			new G4LogicalVolume(Detector_scin,detector_mat,"Detector_scin");
			new G4PVPlacement(0,G4ThreeVector(0,0,a),logicDetector_scin,"Detector_scin",logicWorld,false,1,true);
		a=(distance+(148.5-38.7)/2+38.7);
		G4LogicalVolume* logicDetector_pmt =
			new G4LogicalVolume(Detector_pmt,detector_mat,"Detector_pmt");
			new G4PVPlacement(0,G4ThreeVector(0,0,a),logicDetector_pmt,"Detector_pmt",logicWorld,false,1,true);

		a=-(distance+38.7/2+0.2);
		G4LogicalVolume* logic_scin1 =
			new G4LogicalVolume(scin,scin_mat,"scin");
			new G4PVPlacement(0,G4ThreeVector(0,0,a),logic_scin,"scin",logicWorld,false,400,true);
		a=-(distance+38.7/2);
		G4LogicalVolume* logicDetector_scin1 =
			new G4LogicalVolume(Detector_scin,detector_mat,"Detector_scin");
			new G4PVPlacement(0,G4ThreeVector(0,0,a),logicDetector_scin,"Detector_scin",logicWorld,false,1,true);
		a=-(distance+(148.5-38.7)/2+38.7);
		G4LogicalVolume* logicDetector_pmt1 =
			new G4LogicalVolume(Detector_pmt,detector_mat,"Detector_pmt");
			new G4PVPlacement(0,G4ThreeVector(0,0,a),logicDetector_pmt,"Detector_pmt",logicWorld,false,1,true);
//-----------------------double ----
*/


{
/*
//  -----------ring type-------------
	G4Tubs* Detector1 =
		new G4Tubs("Detector1",0*mm,43.1/2*mm,38.7/2*mm,0.*deg,360.*deg);
	G4Tubs* Detector2 =
		new G4Tubs("Detector2",0*mm,38.1/2*mm,(38.7-1)/2*mm,0.*deg,360.*deg);

	G4Tubs* scin =
		new G4Tubs("scin",0*mm,38.1/2*mm,38.1/2*mm,0.*deg,360.*deg);
	G4SubtractionSolid* Detector_scin = 
		new G4SubtractionSolid("Detector_scin",Detector1,Detector2);
	G4Tubs* Detector_pmt =
		new G4Tubs("Detectr_pmt",0*mm,44.5/2*mm,(148.5-38.7)/2*mm,0.*deg,360.*deg);

	double a;
	double distance=68;
	for(int i=0;i<8;i++)
	{
		for(int j=-1;j<2;j++)

		{
			vol_count=vol_count+1;
			G4RotationMatrix* boxRotation = new G4RotationMatrix;
			boxRotation->rotateX(45*deg*i);
			boxRotation->rotateY(25*deg*j);

			a=(distance+38.7/2+0.2);
			G4LogicalVolume* logic_scin =
				new G4LogicalVolume(scin,scin_mat,"scin");
			new G4PVPlacement(boxRotation,G4ThreeVector(-sin(25*j*deg)*a,a*sin(45*i*deg)*cos(24*j*deg),a*cos(45*i*deg)*cos(25*deg*j)),logic_scin,"scin",logicWorld,false,(200+vol_count),true);
			a=(distance+38.7/2);
			G4LogicalVolume* logicDetector_scin =
				new G4LogicalVolume(Detector_scin,detector_mat,"Detector_scin");
			new G4PVPlacement(boxRotation,G4ThreeVector(-sin(25*j*deg)*a,a*sin(45*i*deg)*cos(24*j*deg),a*cos(45*i*deg)*cos(25*deg*j)),logicDetector_scin,"Detector_scin",logicWorld,false,1,true);
			a=(distance+(148.5-38.7)/2+38.7);
			G4LogicalVolume* logicDetector_pmt =
				new G4LogicalVolume(Detector_pmt,detector_mat,"Detector_pmt");
			new G4PVPlacement(boxRotation,G4ThreeVector(-sin(25*j*deg)*a,a*sin(45*i*deg)*cos(24*j*deg),a*cos(45*i*deg)*cos(25*deg*j)),logicDetector_pmt,"Detector_pmt",logicWorld,false,1,true);
		}
	}
//---------------------------------------//
*/
}


//  -----------ring type half sphere-------------
  

{
	


	G4Tubs* Detector1 =
		new G4Tubs("Detector1",0*mm,43./2*mm,40/2*mm,0.*deg,360.*deg);
	G4Tubs* Detector2 =
		new G4Tubs("Detector2",0*mm,38.1/2*mm,39./2*mm,0.*deg,360.*deg);

	G4Tubs* scin =
		new G4Tubs("scin",0*mm,38.1/2*mm,38.1/2*mm,0.*deg,360.*deg);
	G4SubtractionSolid* Detector_scin = 
		new G4SubtractionSolid("Detector_scin",Detector1,Detector2);
	G4Tubs* Detector_pmt =
		new G4Tubs("Detectr_pmt",0*mm,45./2*mm,110/2*mm,0.*deg,360.*deg);
	G4Tubs* Detector_pmt2 =
		new G4Tubs("Detectr_pmt2",0*mm,48/2*mm,(70-0.001)/2*mm,0.*deg,360.*deg);

	
	G4Tubs* Pb_shield=
		new G4Tubs("Pb_shield1",43./2*mm,45./2*mm,40./2*mm,0.*deg,360.*deg);
		
		
	double a;
	double distance=100;
	double focus_po=500;
	int deg_arr[2]={0,35};
	int j;
	int numofcry=0;
	double pos_cry[24][3];

	for(int jk=0;jk<2;jk++)
	//for(int jk=0;jk<1;jk++)
	{
		for(int i=0;i<12;i++)
		{
			j=deg_arr[jk];
			vol_count=vol_count+1;
			G4RotationMatrix* boxRotation = new G4RotationMatrix;
			boxRotation->rotateX((30*i+15)*deg);
			boxRotation->rotateY(1*deg*j);

			a=(distance+38.1/2+0.5);
			//a=(distance+40/2);
			//a=(distance+(38.1)/2+1);
			G4LogicalVolume* logic_scin =
				new G4LogicalVolume(scin,scin_mat,"scin");
			new G4PVPlacement(boxRotation,G4ThreeVector(-sin(1*j*deg)*a+focus_po,a*sin((30*i+15)*deg)*cos(1*j*deg)+0+focus_po,a*cos((30*i+15)*deg)*cos(1*deg*j)+0+focus_po),logic_scin,"scin",lv_wo,false,(200+vol_count),true);

			pos_cry[numofcry][0]=-sin(1*j*deg)*a;
			pos_cry[numofcry][1]=a*sin((30*i+15)*deg)*cos(1*j*deg)+0;
			pos_cry[numofcry][2]=a*cos((30*i+15)*deg)*cos(1*deg*j)+0;

			a=(distance+40/2);
			G4LogicalVolume* logicDetector_scin =
				new G4LogicalVolume(Detector_scin,detector_mat,"Detector_scin");
			new G4PVPlacement(boxRotation,G4ThreeVector(-sin(1*j*deg)*a+focus_po,a*sin((30*i+15)*deg)*cos(1*j*deg)+0+focus_po,a*cos((30*i+15)*deg)*cos(1*deg*j)+0+focus_po),logicDetector_scin,"Detector_scin",lv_wo,false,1,true);
			a=(distance+(110)/2+40);
			G4LogicalVolume* logicDetector_pmt =
				new G4LogicalVolume(Detector_pmt,detector_mat,"Detector_pmt");
			new G4PVPlacement(boxRotation,G4ThreeVector(-sin(1*j*deg)*a+focus_po,a*sin((30*i+15)*deg)*cos(1*j*deg)+0+focus_po,a*cos((30*i+15)*deg)*cos(1*deg*j)+0+focus_po),logicDetector_pmt,"Detector_pmt",lv_wo,false,1,true);
			
			a=(distance+(70)/2+40+110);
			G4LogicalVolume* logicDetector_pmt2 =
				new G4LogicalVolume(Detector_pmt2,detector_mat,"Detector_pmt2");
			new G4PVPlacement(boxRotation,G4ThreeVector(-sin(1*j*deg)*a+focus_po,a*sin((30*i+15)*deg)*cos(1*j*deg)+0+focus_po,a*cos((30*i+15)*deg)*cos(1*deg*j)+0+focus_po),logicDetector_pmt2,"Detector_pmt2",lv_wo,false,1,true);

		/*//--------Pb shielding----------------------//	
			a=(distance+20);
			G4LogicalVolume* logic_Pb=
				new G4LogicalVolume(Pb_shield,Pb_mat,"Pb_shield");
			new G4PVPlacement(boxRotation,G4ThreeVector(-sin(1*j*deg)*a+focus_po,a*sin((30*i+15)*deg)*cos(1*j*deg)+0+focus_po,a*cos((30*i+15)*deg)*cos(1*deg*j)+0+focus_po),logic_Pb,"Pb_shield",lv_wo,false,99,true);
			*/
	
		numofcry=numofcry+1;
		}
	}


	for(int i=0;i<24;i++)
	{
			std::cout <<"The position of center of crystal"<<i<<" : "<<"{"<<pos_cry[i][0]<<","<<pos_cry[i][1]<<","<<pos_cry[i][2]<<"}" <<std::endl;
	}
}





//  -----------ring type half sphere-------------





/*
//---------------------4cluster------------------------

G4Tubs* Detector1 =
new G4Tubs("Detector1",0*mm,43.1/2*mm,38.7/2*mm,0.*deg,360.*deg);
G4Tubs* Detector2 =
new G4Tubs("Detector2",0*mm,38.1/2*mm,(38.7-1)/2*mm,0.*deg,360.*deg);

G4Tubs* scin =
new G4Tubs("scin",0*mm,38.1/2*mm,38.1/2*mm,0.*deg,360.*deg);
G4SubtractionSolid* Detector_scin =
new G4SubtractionSolid("Detector_scin",Detector1,Detector2);
G4Tubs* Detector_pmt =
new G4Tubs("Detectr_pmt",0*mm,44.5/2*mm,(148.5-38.7)/2*mm,0.*deg,360.*deg);

double a;
int j=0;
int i=0;
G4RotationMatrix* boxRotation = new G4RotationMatrix;
boxRotation->rotateX(45*deg*i);
boxRotation->rotateY(25*deg*j);

for(i=0;i<4;i++){


double num;
if(i%2==0)
{
	num=i-1;
}
else
{
	num=i-2;
}

a=(50+38.7/2+0.2);
G4LogicalVolume* logic_scin =
new G4LogicalVolume(scin,scin_mat,"scin");
new G4PVPlacement(0,G4ThreeVector((-44.5/2*(i-num-2)*num),44.5*sqrt(3)*(i-num-1)/2*(-num),a),logic_scin,"scin",logicWorld,false,200*(i+1),true);

a=(50+38.7/2);
G4LogicalVolume* logicDetector_scin =
new G4LogicalVolume(Detector_scin,detector_mat,"Detector_scin");
new G4PVPlacement(0,G4ThreeVector((-44.5/2*(i-num-2)*num),44.5*sqrt(3)*(i-num-1)/2*(-num),a),logicDetector_scin,"Detector_scin",logicWorld,false,1,true);

a=(50+(148.5-38.7)/2+38.7);

G4LogicalVolume* logicDetector_pmt =
new G4LogicalVolume(Detector_pmt,detector_mat,"Detector_pmt");
new G4PVPlacement(0,G4ThreeVector((-44.5/2*(i-num-2)*num),44.5*sqrt(3)*(i-num-1)/2*(-num),a),logicDetector_pmt,"Detector_pmt",logicWorld,false,1,true);


}
//------------------------------------------
*/


//----------6 detector---------------
/* 
G4Tubs* Detector1 =
new G4Tubs("Detector1",0*mm,43.1/2*mm,38.7/2*mm,0.*deg,360.*deg);
G4Tubs* Detector2 =
new G4Tubs("Detector2",0*mm,38.1/2*mm,(38.7-1)/2*mm,0.*deg,360.*deg);

G4Tubs* scin =
new G4Tubs("scin",0*mm,38.1/2*mm,38.1/2*mm,0.*deg,360.*deg);
G4SubtractionSolid* Detector_scin = 
new G4SubtractionSolid("Detector_scin",Detector1,Detector2);
G4Tubs* Detector_pmt =
new G4Tubs("Detectr_pmt",0*mm,44.5/2*mm,(148.5-38.7)/2*mm,0.*deg,360.*deg);

double a;

for(int k=0;k<2;k++)
{
//for(int i=0;i<3;i++)
int i=0;
{
	for(int j=-1;j<2;j++)
	{
		G4RotationMatrix* boxRotation = new G4RotationMatrix;
		boxRotation->rotateZ(120*deg*i*(1-2*k));
		boxRotation->rotateY(-25*deg*j*(1-2*k));
		boxRotation->rotateX(-25*deg*(abs(j)*2-1));

		a=(70+38.7/2+0.2);
		G4LogicalVolume* logic_scin =
		new G4LogicalVolume(scin,scin_mat,"scin");
		new G4PVPlacement(boxRotation,G4ThreeVector(sin(25*deg*j)*cos(25*deg)*a,sin(-25*deg*(abs(j)*2-1))*a*(1-2*k),a*(cos(-25*deg*(abs(j)*2-1)))*cos(25*deg*j)*(1-2*k)),logic_scin,"scin",logicWorld,false,200,true);
		
		a=(70+38.7/2);
		G4LogicalVolume* logicDetector_scin =
		new G4LogicalVolume(Detector_scin,detector_mat,"Detector_scin");
i	new G4PVPlacement(boxRotation,G4ThreeVector(sin(25*j*deg)*cos(25*deg)*a,sin(-25*deg*(abs(j)*2-1))*a*(1-2*k),a*cos(-25*deg*(abs(j)*2-1))*cos(25*deg*j)*(1-2*k)),logicDetector_scin,"Detector_scin",logicWorld,false,1,true);
		
		a=(70+(148.5-38.7)/2+38.7);
		G4LogicalVolume* logicDetector_pmt =
		new G4LogicalVolume(Detector_pmt,detector_mat,"Detector_pmt");
		new G4PVPlacement(boxRotation,G4ThreeVector(sin(25*deg*j)*cos(25*deg)*a,sin(-25*deg*(abs(j)*2-1))*a*(1-2*k),a*cos(-25*deg*(abs(j)*2-1))*cos(25*deg*j)*(1-2*k)),logicDetector_pmt,"Detector_pmt",logicWorld,false,1,true);
	}
}

}
*/
//---------------------------------


{
 //-------------------------6 x cluster -----------------------------------------------------
/*
G4Tubs* Detector1 =
new G4Tubs("Detector1",0*mm,43.1/2*mm,38.7/2*mm,0.*deg,360.*deg);
G4Tubs* Detector2 =
new G4Tubs("Detector2",0*mm,38.1/2*mm,(38.7-1)/2*mm,0.*deg,360.*deg);

G4Tubs* scin =
new G4Tubs("scin",0*mm,38.1/2*mm,38.1/2*mm,0.*deg,360.*deg);
G4SubtractionSolid* Detector_scin =
new G4SubtractionSolid("Detector_scin",Detector1,Detector2);
G4Tubs* Detector_pmt =
new G4Tubs("Detectr_pmt",0*mm,44.5/2*mm,(148.5-38.7)/2*mm,0.*deg,360.*deg);

double a;
int k=0;
int j;
double angle=55;
double distance=120;

for(k=0;k<2;k++)
{

for(j=-1;j<2;j++)
{
 
G4ThreeVector *Rotaxis=new G4ThreeVector(-sin(30*deg)*(abs(j)*2-1),cos(30*deg)*j,0);
  G4RotationMatrix* boxRotation = new G4RotationMatrix;
  boxRotation->rotate(-angle*deg,Rotaxis);
  //G4RotationMatrix* boxRotation2 = new G4RotationMatrix;
  //boxRotation2->rotate(-30*deg,Rotaxis);
  //boxRotation2->rotateZ(-60*deg);
  //G4RotationMatrix* boxRotation3 = new G4RotationMatrix;
  //boxRotation3->rotate(-30*deg,Rotaxis);
  //boxRotation3->rotateZ(60*deg);



for(int i=0;i<4;i++){


double num;
if(i%2==0)
{
        num=i-1;
}
else
{
        num=i-2;
}

vol_count=vol_count+1;
a=(distance+38.7/2+0.2);
G4LogicalVolume* logic_scin =
new G4LogicalVolume(scin,scin_mat,"scin");
new G4PVPlacement(boxRotation,G4ThreeVector((-44.5/2*(i-num-2)*num)*(1-2*k),44.5*sqrt(3)*(i-num-1)/2*(-num)*(1-2*k),a*(1-2*k)).rotate(angle*deg,G4ThreeVector(-sin(30*deg)*(abs(j)*2-1),cos(30*deg)*j,0)),logic_scin,"scin",logicWorld,false,(200+vol_count),true);

a=(distance+38.7/2);
G4LogicalVolume* logicDetector_scin =
new G4LogicalVolume(Detector_scin,detector_mat,"Detector_scin");
new G4PVPlacement(boxRotation,G4ThreeVector((-44.5/2*(i-num-2)*num)*(1-2*k),44.5*sqrt(3)*(i-num-1)/2*(-num)*(1-2*k),a*(1-2*k)).rotate(angle*deg,G4ThreeVector(-sin(30*deg)*(abs(j)*2-1),cos(30*deg)*j,0)),logicDetector_scin,"Detector_scin",logicWorld,false,1,true);

a=(distance+(148.5-38.7)/2+38.7);

G4LogicalVolume* logicDetector_pmt =
new G4LogicalVolume(Detector_pmt,detector_mat,"Detector_pmt");
new G4PVPlacement(boxRotation,G4ThreeVector((-44.5/2*(i-num-2)*num)*(1-2*k),44.5*sqrt(3)*(i-num-1)/2*(-num)*(1-2*k),a*(1-2*k)).rotate(angle*deg,G4ThreeVector(-sin(30*deg)*(abs(j)*2-1),cos(30*deg)*j,0)),logicDetector_pmt,"Detector_pmt",logicWorld,false,1,true);



}


}
}
//-------------------------------------------------------------------
*/
}







{
/*
	//-------------------------8 x cluster  -----------------------------------------------------
	G4Tubs* Detector1 =
		new G4Tubs("Detector1",0*mm,43.1/2*mm,38.7/2*mm,0.*deg,360.*deg);
	G4Tubs* Detector2 =
		new G4Tubs("Detector2",0*mm,38.1/2*mm,(38.7-1)/2*mm,0.*deg,360.*deg);

	G4Tubs* scin =
		new G4Tubs("scin",0*mm,38.1/2*mm,38.1/2*mm,0.*deg,360.*deg);
	G4SubtractionSolid* Detector_scin =
		new G4SubtractionSolid("Detector_scin",Detector1,Detector2);
	G4Tubs* Detector_pmt =
		new G4Tubs("Detectr_pmt",0*mm,44.5/2*mm,(148.5-38.7)/2*mm,0.*deg,360.*deg);

	double a;
	double angle=45;

	double distance=200;


			double num;
			double x;
			double y;
			double z;
			double rotx;
			double rotz;
			int opp;
			for(int k=0;k<2;k++)
			{
				//opp=1-2*k;
				opp=1;
				if(k==1)
				{
					for(int j=0;j<4;j++)
					{
						//G4RotationMatrix* boxRotation2 = new G4RotationMatrix;
						//boxRotation2->rotate(-30*deg,Rotaxis);
						//boxRotation2->rotateZ(-60*deg);
						//G4RotationMatrix* boxRotation3 = new G4RotationMatrix;
						//boxRotation3->rotate(-30*deg,Rotaxis);
						//boxRotation3->rotateZ(60*deg);

						G4RotationMatrix* boxRotation = new G4RotationMatrix;
						//boxRotation->rotateZ(30.*deg);
						if(j<2)	
						{
							rotx=cos(30*(2*j+1)*deg);
							rotz=-sin(30*(2*j+1)*deg);
							G4ThreeVector *Rotaxis=new G4ThreeVector(rotx,0,rotz);
							boxRotation->rotate(-30*opp*deg,Rotaxis);
						}
						else 
						{
							rotx=cos(30*(2*j-3)*deg);
							rotz=sin(30*(1-4*j)*deg);
							G4ThreeVector *Rotaxis=new G4ThreeVector(rotx,0,rotz);
							boxRotation->rotate(-30*opp*deg,Rotaxis);
						}
						if(j<2)	
						{
							boxRotation->rotateY(-30*(2*j+1)*deg);
							//		boxRotation->rotateZ(30.*deg);
							angle=opp*30*(2*j+1);
							rotx=cos(30*(2*j+1)*deg);
							rotz=-sin(30*(2*j+1)*deg);
							G4ThreeVector *Rotaxis=new G4ThreeVector(rotx,0,rotz);
							//boxRotation->rotateY(-30*(2*j+1)*deg).rotate(-30*deg,Rotaxis);
						}
						else 
						{
							boxRotation->rotateY(30*(2*(j-2)+1)*deg);
							//boxRotation->rotateZ(30.*deg);
							angle=-opp*30*(2*(j-2)+1);
							rotx=cos(30*(2*j-3)*deg);
							rotz=sin(30*(1-4*j)*deg);
							G4ThreeVector *Rotaxis=new G4ThreeVector(rotx,0,rotz);
							//boxRotation->rotateY(-30*(2*j+1)*deg).rotate(-30*deg,Rotaxis);
						}



						for(int i=0;i<3;i++)
						{ if(i<2)
							{
								y=-opp*44.5*0.5/tan(60*deg);
								x=opp*44.5*0.5*(1-2*i);
							}
							else
							{
								y=opp*44.5*0.5/cos(30*deg);
								x=opp*0;

							}
							//vol_count=vol_count+1;

							a=(distance+38.7/2+0.2);
							//boxRotation->rotateZ(30.*deg);

							G4LogicalVolume* logic_scin =
								new G4LogicalVolume(scin,scin_mat,"scin");
							//new G4PVPlacement(boxRotation,G4ThreeVector(a,y,z).rotate(angle*deg,G4ThreeVector(-sin(30*deg)*(abs(j)*2-1),cos(30*deg)*j,0)),logic_scin,"scin",logicWorld,false,(200+vol_count),true);
							new G4PVPlacement(boxRotation,G4ThreeVector(x,y,a).rotate(angle*deg,G4ThreeVector(0,opp*1,0)).rotate(opp*30*deg,G4ThreeVector(rotx,0,rotz)),logic_scin,"scin",logicWorld,false,(200+vol_count),true);

							a=(distance+38.7/2);
							G4LogicalVolume* logicDetector_scin =
								new G4LogicalVolume(Detector_scin,detector_mat,"Detector_scin");
							//new G4PVPlacement(boxRotation,G4ThreeVector(a,y,z).rotate(angle*deg,G4ThreeVector(-sin(30*deg)*(abs(j)*2-1),cos(30*deg)*j,0)),logicDetector_scin,"Detector_scin",logicWorld,false,1,true);
							new G4PVPlacement(boxRotation,G4ThreeVector(x,y,a).rotate(angle*deg,G4ThreeVector(0,opp*1,0)).rotate(opp*30*deg,G4ThreeVector(rotx,0,rotz)),logicDetector_scin,"Detector_scin",logicWorld,false,1,true);

							a=(distance+(148.5-38.7)/2+38.7);

							G4LogicalVolume* logicDetector_pmt =
								new G4LogicalVolume(Detector_pmt,detector_mat,"Detector_pmt");
							//new G4PVPlacement(boxRotation,G4ThreeVector(a,y,z).rotate(angle*deg,G4ThreeVector(-sin(30*deg)*(abs(j)*2-1),cos(30*deg)*j,0)),logicDetector_pmt,"Detector_pmt",logicWorld,false,1,true);
							new G4PVPlacement(boxRotation,G4ThreeVector(x,y,a).rotate(angle*deg,G4ThreeVector(0,opp*1,0)).rotate(opp*30*deg,G4ThreeVector(rotx,0,rotz)),logicDetector_pmt,"Detector_pmt",logicWorld,false,1,true);

						}
					}
				}
				else
				{
					for(int j=0;j<4;j++)
					{
						//G4RotationMatrix* boxRotation2 = new G4RotationMatrix;
						//boxRotation2->rotate(-30*deg,Rotaxis);
						//boxRotation2->rotateZ(-60*deg);
						//G4RotationMatrix* boxRotation3 = new G4RotationMatrix;
						//boxRotation3->rotate(-30*deg,Rotaxis);
						//boxRotation3->rotateZ(60*deg);

						G4RotationMatrix* boxRotation = new G4RotationMatrix;
						//boxRotation->rotateZ(30.*deg);
						if(j<2)	
						{
							rotx=cos(30*(2*j+1)*deg);
							rotz=-sin(30*(2*j+1)*deg);
							G4ThreeVector *Rotaxis=new G4ThreeVector(rotx,0,rotz);
							boxRotation->rotate(-60*opp*deg,Rotaxis);
						}
						else 
						{
							rotx=cos(30*(2*j-3)*deg);
							rotz=sin(30*(1-4*j)*deg);
							G4ThreeVector *Rotaxis=new G4ThreeVector(rotx,0,rotz);
							boxRotation->rotate(-60*opp*deg,Rotaxis);
						}
						if(j<2)	
						{
							boxRotation->rotateY(-30*(2*j+1)*deg);
							angle=opp*30*(2*j+1);
							rotx=cos(30*(2*j+1)*deg);
							rotz=-sin(30*(2*j+1)*deg);
							G4ThreeVector *Rotaxis=new G4ThreeVector(rotx,0,rotz);
						}
						else 
						{
							boxRotation->rotateY(30*(2*(j-2)+1)*deg);
							angle=-opp*30*(2*(j-2)+1);
							rotx=cos(30*(2*j-3)*deg);
							rotz=sin(30*(1-4*j)*deg);
							G4ThreeVector *Rotaxis=new G4ThreeVector(rotx,0,rotz);
							//boxRotation->rotateY(-30*(2*j+1)*deg).rotate(-30*deg,Rotaxis);
						}



						for(int i=0;i<3;i++)
						{
							if(i<2)
							{
								y=-opp*44.5*0.5/tan(60*deg);
								x=opp*44.5*0.5*(1-2*i);
							}
							else
							{
								y=opp*44.5*0.5/cos(30*deg);
								x=opp*0;

							}
							//vol_count=vol_count+1;

							a=(distance+38.7/2+0.2);

							G4LogicalVolume* logic_scin =
								new G4LogicalVolume(scin,scin_mat,"scin");
							new G4PVPlacement(boxRotation,G4ThreeVector(x,y,a).rotate(angle*deg,G4ThreeVector(0,opp*1,0)).rotate(opp*60*deg,G4ThreeVector(rotx,0,rotz)),logic_scin,"scin",logicWorld,false,(200+vol_count),true);

							a=(distance+38.7/2);
							G4LogicalVolume* logicDetector_scin =
								new G4LogicalVolume(Detector_scin,detector_mat,"Detector_scin");
							new G4PVPlacement(boxRotation,G4ThreeVector(x,y,a).rotate(angle*deg,G4ThreeVector(0,opp*1,0)).rotate(opp*60*deg,G4ThreeVector(rotx,0,rotz)),logicDetector_scin,"Detector_scin",logicWorld,false,1,true);

							a=(distance+(148.5-38.7)/2+38.7);

							G4LogicalVolume* logicDetector_pmt =
								new G4LogicalVolume(Detector_pmt,detector_mat,"Detector_pmt");
							new G4PVPlacement(boxRotation,G4ThreeVector(x,y,a).rotate(angle*deg,G4ThreeVector(0,opp*1,0)).rotate(opp*60*deg,G4ThreeVector(rotx,0,rotz)),logicDetector_pmt,"Detector_pmt",logicWorld,false,1,true);

						}
					}
				}
			}

*/
}
//-------------------------------------------------------------------

{
/*
	//-------------------------8 x cluster  -----------------------------------------------------
	G4Tubs* Detector1 =
		new G4Tubs("Detector1",0*mm,43.1/2*mm,38.7/2*mm,0.*deg,360.*deg);
	G4Tubs* Detector2 =
		new G4Tubs("Detector2",0*mm,38.1/2*mm,(38.7-1)/2*mm,0.*deg,360.*deg);

	G4Tubs* scin =
		new G4Tubs("scin",0*mm,38.1/2*mm,38.1/2*mm,0.*deg,360.*deg);
	G4SubtractionSolid* Detector_scin =
		new G4SubtractionSolid("Detector_scin",Detector1,Detector2);
	G4Tubs* Detector_pmt =
		new G4Tubs("Detectr_pmt",0*mm,44.5/2*mm,(148.5-38.7)/2*mm,0.*deg,360.*deg);

	double a;
	double angle=45;

	double distance=110;


			double num;
			double x;
			double y;
			double z;
			double rotx;
			double roty;
			double rotz;
			int opp;
			for(int k=0;k<2;k++)
			{
				//opp=1-2*k;
				if(k==1)
				{
				opp=1+k;
					for(int j=0;j<4;j++)
					{

						G4RotationMatrix* boxRotation = new G4RotationMatrix;
						angle=30*opp;
						if(j==0||j==2)
						{
							boxRotation->rotateY(-30*opp*(1-j)*deg);
						}
						if(j==1||j==3)
						{
							boxRotation->rotateX(-30*opp*(j-2)*deg);
						}



						for(int i=0;i<3;i++)
						{ 
							if(i<2)
							{
								y=-44.5*0.5/tan(60*deg);
								x=44.5*0.5*(1-2*i);
							}
							else
							{
								y=44.5*0.5/cos(30*deg);
								x=0;

							}

							vol_count=vol_count+1;
							a=(distance+38.7/2+0.2);

							G4LogicalVolume* logic_scin =
								new G4LogicalVolume(scin,scin_mat,"scin");
							new G4PVPlacement(boxRotation,G4ThreeVector(x,y,a).rotate(angle*deg,G4ThreeVector(0,opp*1,0)).rotate(90*j*deg,G4ThreeVector(0,0,1)),logic_scin,"scin",logicWorld,false,(200+vol_count),true);

							a=(distance+38.7/2);
							G4LogicalVolume* logicDetector_scin =
								new G4LogicalVolume(Detector_scin,detector_mat,"Detector_scin");
							new G4PVPlacement(boxRotation,G4ThreeVector(x,y,a).rotate(angle*deg,G4ThreeVector(0,opp*1,0)).rotate(90*j*deg,G4ThreeVector(0,0,1)),logicDetector_scin,"Detector_scin",logicWorld,false,1,true);

							a=(distance+(148.5-38.7)/2+38.7);

							G4LogicalVolume* logicDetector_pmt =
								new G4LogicalVolume(Detector_pmt,detector_mat,"Detector_pmt");
							new G4PVPlacement(boxRotation,G4ThreeVector(x,y,a).rotate(angle*deg,G4ThreeVector(0,opp*1,0)).rotate(90*j*deg,G4ThreeVector(0,0,1)),logicDetector_pmt,"Detector_pmt",logicWorld,false,1,true);

						}
					}
				}
				else
				{
				opp=1+k;
					for(int j=0;j<4;j++)
					{

						G4RotationMatrix* boxRotation = new G4RotationMatrix;
						angle=60*opp;
						//
						//if(j==0||j==2)

						//{
						//	boxRotation->rotateY(-30*opp*(1-j)*deg);
						//	G4ThreeVector *Rotaxis=new G4ThreeVector(rotx,0,rotz);
						//	boxRotation->rotate(-60*opp*deg,Rotaxis);
						//}
						//if(j==1||j==3)
						//	{
						//	boxRotation->rotateX(-30*opp*(j-2)*deg);
						//}
						//

						if(j<2) rotx=-1;
						else rotx=1;	
						
						if(j==1||j==2) roty=-1;
						else roty=1;

						G4ThreeVector *Rotaxis=new G4ThreeVector(rotx,roty,0);
						boxRotation->rotate(-60*opp*deg,Rotaxis);


						for(int i=0;i<3;i++)
						{ 
							if(i<2)
							{
								y=-44.5*0.5/tan(60*deg);
								x=44.5*0.5*(1-2*i);
							}
							else
							{
								y=44.5*0.5/cos(30*deg);
								x=0;

							}

							vol_count=vol_count+1;
							a=(distance+38.7/2+0.2);

							G4LogicalVolume* logic_scin =
								new G4LogicalVolume(scin,scin_mat,"scin");
							new G4PVPlacement(boxRotation,G4ThreeVector(x,y,a).rotate(angle*deg,G4ThreeVector(0,opp*1,0)).rotate((90*j+45)*deg,G4ThreeVector(0,0,1)),logic_scin,"scin",logicWorld,false,(200+vol_count),true);

							a=(distance+38.7/2);
							G4LogicalVolume* logicDetector_scin =
								new G4LogicalVolume(Detector_scin,detector_mat,"Detector_scin");
							new G4PVPlacement(boxRotation,G4ThreeVector(x,y,a).rotate(angle*deg,G4ThreeVector(0,opp*1,0)).rotate((90*j+45)*deg,G4ThreeVector(0,0,1)),logicDetector_scin,"Detector_scin",logicWorld,false,1,true);

							a=(distance+(148.5-38.7)/2+38.7);

							G4LogicalVolume* logicDetector_pmt =
								new G4LogicalVolume(Detector_pmt,detector_mat,"Detector_pmt");
							new G4PVPlacement(boxRotation,G4ThreeVector(x,y,a).rotate(angle*deg,G4ThreeVector(0,opp*1,0)).rotate((90*j+45)*deg,G4ThreeVector(0,0,1)),logicDetector_pmt,"Detector_pmt",logicWorld,false,1,true);

						}
					}
				}
			}

*/
}

/*
{
	//-------------------------8 x cluster(short code) -----------------------------------------------------
	G4Tubs* Detector1 =
		new G4Tubs("Detector1",0*mm,43.1/2*mm,38.7/2*mm,0.*deg,360.*deg);
	G4Tubs* Detector2 =
		new G4Tubs("Detector2",0*mm,38.1/2*mm,(38.7-1)/2*mm,0.*deg,360.*deg);

	G4Tubs* scin =
		new G4Tubs("scin",0*mm,38.1/2*mm,38.1/2*mm,0.*deg,360.*deg);
	G4SubtractionSolid* Detector_scin =
		new G4SubtractionSolid("Detector_scin",Detector1,Detector2);
	G4Tubs* Detector_pmt =
		new G4Tubs("Detectr_pmt",0*mm,44.5/2*mm,(148.5-38.7)/2*mm,0.*deg,360.*deg);

	double a;
	double angle=45;

	double distance=120;


			double num;
			double x;
			double y;
			double z;
			double rotx;
			double roty;
			double rotz;
			int opp;
			int angle_sign;
			int rot_axis[8][2]={{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},{1,1}};
			for(int k=0;k<2;k++)
			{
				//opp=1-2*k;
				if(k==1)
				{
				opp=1+k;
					for(int j=0;j<8;j++)
					{

						G4RotationMatrix* boxRotation = new G4RotationMatrix;
						angle=60;

						rotx=rot_axis[j][0];
						roty=rot_axis[j][1];

						G4ThreeVector *Rotaxis=new G4ThreeVector(rotx,roty,0);
						boxRotation->rotate(-angle*deg,Rotaxis);
						


						for(int i=0;i<3;i++)
						{ 
							if(i<2)
							{
								y=-44.5*0.5/tan(60*deg);
								x=44.5*0.5*(1-2*i);
							}
							else
							{
								y=44.5*0.5/cos(30*deg);
								x=0;

							}

							vol_count=vol_count+1;
							a=(distance+38.7/2+0.2);

							G4LogicalVolume* logic_scin =
								new G4LogicalVolume(scin,scin_mat,"scin");
							new G4PVPlacement(boxRotation,G4ThreeVector(x,y,a).rotate(angle*deg,G4ThreeVector(0,1,0)).rotate(45*j*deg,G4ThreeVector(0,0,1)),logic_scin,"scin",logicWorld,false,(200+vol_count),true);

							a=(distance+38.7/2);
							G4LogicalVolume* logicDetector_scin =
								new G4LogicalVolume(Detector_scin,detector_mat,"Detector_scin");
							new G4PVPlacement(boxRotation,G4ThreeVector(x,y,a).rotate(angle*deg,G4ThreeVector(0,1,0)).rotate(45*j*deg,G4ThreeVector(0,0,1)),logicDetector_scin,"Detector_scin",logicWorld,false,1,true);

							a=(distance+(148.5-38.7)/2+38.7);

							G4LogicalVolume* logicDetector_pmt =
								new G4LogicalVolume(Detector_pmt,detector_mat,"Detector_pmt");
							new G4PVPlacement(boxRotation,G4ThreeVector(x,y,a).rotate(angle*deg,G4ThreeVector(0,1,0)).rotate(45*j*deg,G4ThreeVector(0,0,1)),logicDetector_pmt,"Detector_pmt",logicWorld,false,1,true);

						}
					}
				}
			}
}
*/










G4Element* C = new G4Element("Carbon", "C", 6., 12.011*g/mole);
G4Element* H = new G4Element("Hydrogen", "H", 1., 1.00794*g/mole);

G4Material* plastic_mat = new G4Material("Plastic",  1.032*g/cm3, 2);
        plastic_mat->AddElement(C, 9);
        plastic_mat->AddElement(H, 10);


//-----------------source capsule----

/*
G4VSolid* source_encap = new G4Tubs("source_encap", 0.*mm, 0.5*25.5*mm, 0.5*6.3*mm, 0., 2*M_PI*rad);
G4LogicalVolume* log_source_encap = new G4LogicalVolume(source_encap, plastic_mat, "SourceCase");

//new G4PVPlacement(0, G4ThreeVector(0.*mm,1000.*mm, 1000.*mm),log_source_encap, "SourceCase", logicWorld, false, 77);
//new G4PVPlacement(0, G4ThreeVector(500.*mm,0.*mm, 0.*mm),log_source_encap, "SourceCase", logicWorld, false, 77);
new G4PVPlacement(0, G4ThreeVector(500.*mm,500*mm, 500*mm),log_source_encap, "SourceCase", lv_wo, false, 77);
*/


//----------------source capsule-----

//G4GDMLParser parser;
//parser.Write("myGDML.gdml",log_source_encap,false);

//----------------source capsule-----

//----------beam line cluster
/*
 G4Box* chamber =
   new G4Box("chamber",                       // its name
            800,                // half x
             35,                // half y
             50);               // half z
 G4LogicalVolume* log_cham =
   new G4LogicalVolume(chamber,          //its solid
                       plastic_mat,           //its material
                       "chamber");            //its name
   new G4PVPlacement(0,                     //no rotation
                     G4ThreeVector(),       //at (0,0,0)
                     log_cham,            //its logical volume
                     "chamber",               //its name
                     logicWorld,                     //its mother  volume
                     false,                 //no boolean operation
                     0,                     //copy number
                     true);                 //overlaps checking
*/
//----------beam line ring

/*
 G4Box* chamber =
   new G4Box("chamber",                       // its name
             800,                // half x
             35,                // half y
             50);               // half z
 G4LogicalVolume* log_cham =
   new G4LogicalVolume(chamber,          //its solid
                       plastic_mat,           //its material
                       "chamber");            //its name
   new G4PVPlacement(0,                     //no rotation
                     G4ThreeVector(),       //at (0,0,0)
                     log_cham,            //its logical volume
                     "chamber",               //its name
                     logicWorld,                     //its mother  volume
                     false,                 //no boolean operation
                     0,                     //copy number
                     true);                 //overlaps checking
*/
/*
{

	//----------------------------------------------------
	// Material definitions
	//----------------------------------------------------
	
	G4String name, symbol;             //a=mass of a mole;
	G4double a, z, density;            //z=mean number of protons;  
	
	G4int ncomponents, natoms;         
	
	G4double pressure    = 3.e-18*pascal;
	G4double temperature = 2.73*kelvin;
	density     = 1.e-25*g/cm3;
	
	G4Material* Vacuum = new G4Material(name="Galactic", z=1., a=1.01*g/mole, 
										density,kStateGas,temperature,pressure);
	
	
	//
	// define simple Elements
	//
	
	// O
	a = 15.999*g/mole;
	G4Element* O  = new G4Element(name="Oxygen" ,symbol="O" , z= 8., a);
		
	// Mg
	a = 24.305*g/mole;
	G4Element* Mg  = new G4Element(name="Magnesium"  ,symbol="Mg" , z= 12., a);
	
	// Si
	a = 28.0855*g/mole;
	G4Element* Si  = new G4Element(name="Silicon"  ,symbol="Si" , z= 14., a);
	
	// Cl
	a = 35.453*g/mole;
	G4Element* Cl  = new G4Element(name="Chlorine"  ,symbol="Cl" , z= 17., a);
	
	// K
	a = 39.0983*g/mole;
	G4Element* K  = new G4Element(name="Potassium"  ,symbol="K" , z= 19., a);
	
	// Br
	a = 79.904*g/mole;
	G4Element* Br  = new G4Element(name="Bromine"  ,symbol="Br" , z= 35., a);
	
	// Sb
	a = 121.76*g/mole;
	G4Element* Sb = new G4Element(name="Antimony",symbol="Sb" , z= 51., a);
	
	// Cs
	a = 132.905*g/mole;
	G4Element* Cs = new G4Element(name="Cesium",symbol="Cs" , z= 55., a);
	
	// La
	a = 138.905*g/mole;
	G4Element* La = new G4Element(name="Lanthanum",symbol="La" , z= 57., a);
	
	
	//
	// define simple materials
	//
	
	// Al reflector
	density = 2.700*g/cm3;
	a = 26.98*g/mole;
	G4Material* AluR = new G4Material(name="AluRef", z=13., a, density);
	
	// MgO reflector
	density = 2.0*g/cm3;
	G4Material* MgO = new G4Material(name="MgO", density, ncomponents=2);
	MgO->AddElement(Mg, natoms=1);
	MgO->AddElement(O, natoms=1);
		
	// LaCl3
	density = 3.85*g/cm3;
	G4Material* LaCl3 = new G4Material(name="LaCl3", density, ncomponents=2);
	LaCl3->AddElement(Cl, natoms=3);
	LaCl3->AddElement(La, natoms=1);
	
	// LaBr3
	density = 5.08*g/cm3;
	G4Material* LaBr3 = new G4Material(name="LaBr3", density, ncomponents=2);
	LaBr3->AddElement(Br, natoms=3);
	LaBr3->AddElement(La, natoms=1);
	
	// Quartz (fused silica)
	density = 2.20*g/cm3;
	G4Material* Quartz = new G4Material(name="Quartz", density, ncomponents=2);
	Quartz->AddElement(Si, natoms=1);
	Quartz->AddElement(O, natoms=2);
	
	// Photocathode Material 
	// (Bialkali K2CsSb,  Density=?, Thickness=25.*nm?)
	density = 2.00*g/cm3;
	G4Material* K2CsSb = new G4Material(name="K2CsSb", density, ncomponents=3);
	K2CsSb->AddElement(K, natoms=2);
	K2CsSb->AddElement(Cs, natoms=1);
	K2CsSb->AddElement(Sb, natoms=1);
	
	//------------------------------------------------------
	// Optical properties
	//------------------------------------------------------
	
	const G4int nEntries = 2;
	
	G4double PhotonEnergy[nEntries] = {1.0*eV,7.0*eV};
	
	// MgO reflector
	
	G4double MgORefractionIndex[nEntries] = {1.0,1.0};
	
	G4double MgOAbsorptionLength[nEntries] = {1.0E-9*m,1.0E-9*m};
	
	G4MaterialPropertiesTable* MgOMPT = new G4MaterialPropertiesTable();
	
	MgOMPT->AddProperty("RINDEX",PhotonEnergy,MgORefractionIndex,
						nEntries);
	MgOMPT->AddProperty("ABSLENGTH",PhotonEnergy,MgOAbsorptionLength,
						nEntries);
	
	MgO->SetMaterialPropertiesTable(MgOMPT);
	
	// LaBr3
	
	G4double LaBr3RefractionIndex[nEntries] = {1.9,1.9};
	
	G4double LaBr3AbsorptionLength[nEntries] = {50.*cm,50.*cm};
	
	G4MaterialPropertiesTable* LaBr3MPT = new G4MaterialPropertiesTable();
	
	LaBr3MPT->AddProperty("RINDEX",PhotonEnergy,LaBr3RefractionIndex,
						  nEntries);
	LaBr3MPT->AddProperty("ABSLENGTH",PhotonEnergy,LaBr3AbsorptionLength,
						  nEntries);
	
	G4double ScintEnergy[nEntries] = {3.26*eV,3.44*eV};
	G4double ScintFast[nEntries] = {1.0,1.0};
	
	LaBr3MPT->AddProperty("FASTCOMPONENT",ScintEnergy,ScintFast,nEntries);
	
	LaBr3MPT->AddConstProperty("SCINTILLATIONYIELD",63./keV); 
	LaBr3MPT->AddConstProperty("RESOLUTIONSCALE",1.);
	LaBr3MPT->AddConstProperty("FASTTIMECONSTANT",20.*ns);
	LaBr3MPT->AddConstProperty("YIELDRATIO",1.);
	
	LaBr3->SetMaterialPropertiesTable(LaBr3MPT);
		
	// Quartz
	
	G4double QuartzRefractionIndex[nEntries] = {1.47,1.47};
	
	G4double QuartzAbsorptionLength[nEntries] = {3.0*cm,3.0*cm};
	
	G4MaterialPropertiesTable* QuartzMPT = new G4MaterialPropertiesTable();
	
	QuartzMPT->AddProperty("RINDEX",PhotonEnergy,QuartzRefractionIndex,
						   nEntries);
	QuartzMPT->AddProperty("ABSLENGTH",PhotonEnergy,QuartzAbsorptionLength,
						   nEntries);
	
	Quartz->SetMaterialPropertiesTable(QuartzMPT);
	
	// K2CsSb (Bialcali Photocathode)
	
	G4double K2CsSbRefractionIndex[nEntries] = {1.47,1.47};
	
	G4double K2CsSbAbsorptionLength[nEntries] = {1.0E-9*m,1.0E-9*m};
	
	G4MaterialPropertiesTable* K2CsSbMPT = new G4MaterialPropertiesTable();
	
	K2CsSbMPT->AddProperty("RINDEX",PhotonEnergy,K2CsSbRefractionIndex,
						   nEntries);
	K2CsSbMPT->AddProperty("ABSLENGTH",PhotonEnergy,K2CsSbAbsorptionLength,
						   nEntries);
	
	K2CsSb->SetMaterialPropertiesTable(K2CsSbMPT);
	
	// Vacuum
	
	G4double vacRefractionIndex[nEntries] = {1.0,1.0};
	
	
	G4MaterialPropertiesTable* vacMPT = new G4MaterialPropertiesTable();
	vacMPT->AddProperty("RINDEX",PhotonEnergy,vacRefractionIndex,
						nEntries);
	
	
	Vacuum->SetMaterialPropertiesTable(vacMPT);
	
	//------------------------------------------------------
	// Detector geometry
	//------------------------------------------------------
	
	//     
	// World
	//
	
	G4double WorldSize= 30.*cm;
	
	G4Box* 
    solidWorld = new G4Box("World",		       	                  //its name
						   WorldSize/2,WorldSize/2,WorldSize/2);  //its size
	
	G4LogicalVolume* 
    logicWorld = new G4LogicalVolume(solidWorld,      	//its solid
									 Vacuum,	        //its material
									 "World");		    //its name
	
	G4VPhysicalVolume* 
    physiWorld = new G4PVPlacement(0,			    //no rotation
								   G4ThreeVector(),	//at (0,0,0)
								   "World",		    //its name
								   logicWorld,		//its logical volume
								   NULL,		    //its mother  volume
								   false,	       	//no boolean operation
								   0);			    //copy number
	
	
	//
	// Detector 
	//
	
	G4double ScintHalfLength =1.5*cm;
	G4double ScintRadius = 2.*cm;
	
    G4double ReflectorThickness = 0.5*mm;
	G4double ReflectorHalfLength = ScintHalfLength+ReflectorThickness;
	G4double ReflectorRadius = ScintRadius+ReflectorThickness;
	
	G4double PMTWindowHalfLength = 1.0*mm;
	G4double PMTWindowRadius = 1.8*cm;
	
	G4double CathodeHalfLength = 0.005*mm;
	G4double CathodeRadius =1.9*cm;
	
	G4double StartPhi = 0.*deg;
	G4double DeltaPhi = 360.*deg;
	
	
	// Reflector
	
	G4Tubs* solidReflector = new G4Tubs("Reflector",0.*cm,
										ReflectorRadius,ReflectorHalfLength,StartPhi,DeltaPhi);
	
	G4LogicalVolume* logicReflector = new G4LogicalVolume(solidReflector,MgO,
														  "Reflector");
	
	G4ThreeVector positionReflector = G4ThreeVector(0.*cm,0.*cm,0.*cm);
	
	G4VPhysicalVolume* physiReflector = new G4PVPlacement(0,positionReflector,
														  "Reflector",logicReflector,
														  physiWorld,false,0);
	
	//Crystal
	
	G4Tubs* solidCrystal = new G4Tubs("Crystal", 0.*cm, ScintRadius,
									  ScintHalfLength,StartPhi,DeltaPhi);
	
	G4LogicalVolume* logicCrystal = new G4LogicalVolume(solidCrystal,LaBr3,
														"Crystal");
	
	G4ThreeVector positionCrystal = G4ThreeVector(0.*cm,0.*cm,
                                                  ReflectorThickness);
	
	G4VPhysicalVolume* physiCrystal = new G4PVPlacement(0,positionCrystal,
														"Crystal",logicCrystal,
														physiWorld,false,100);
	
	
	// PMT window
	
	G4Tubs* solidPMTWindow = new G4Tubs("PMTWindow",0.*cm,PMTWindowRadius,
										PMTWindowHalfLength,StartPhi,DeltaPhi);
	
	G4LogicalVolume* logicPMTWindow = new G4LogicalVolume(solidPMTWindow,
														  Quartz,"PMTWindow");
	
	G4ThreeVector positionPMTWindow = G4ThreeVector(0.*cm,0.*cm,
													ReflectorHalfLength+PMTWindowHalfLength);
	
	G4VPhysicalVolume* physiPMTWindow = new G4PVPlacement(0,positionPMTWindow,
														  "PMTWindow",logicPMTWindow,
														  physiWorld,false,0);
	
	// Photocathode
	
	G4Tubs* solidCathode = new G4Tubs("Cathode",0.*cm,CathodeRadius,
									  CathodeHalfLength,StartPhi,DeltaPhi);
	
	G4LogicalVolume* logicCathode = new G4LogicalVolume(solidCathode,
														K2CsSb,"Cathode");
	
	G4ThreeVector positionCathode = G4ThreeVector(0.*cm,0.*cm,
												  ReflectorHalfLength+2.*PMTWindowHalfLength
												  +CathodeHalfLength);
	
	G4VPhysicalVolume* physiCathode = new G4PVPlacement(0,positionCathode,
														"Cathode",logicCathode,
														physiWorld,false,0);
	
	
	//------------------------------------------------------
	// Surfaces and boundary processes
	//------------------------------------------------------
	
	// Reflector - sintillator surface 
	
	G4OpticalSurface* OpRefCrySurface = 
	new G4OpticalSurface("RefCrySurface");
	
	OpRefCrySurface->SetType(dielectric_metal);
	OpRefCrySurface->SetModel(glisur);
	OpRefCrySurface->SetFinish(polished);
	
	G4LogicalBorderSurface* RefCrySurface = 
    new G4LogicalBorderSurface("RefCrySurface",physiCrystal,
							   physiReflector,OpRefCrySurface);
	
	
	// Scintillator - PMT window surface 
	
	G4OpticalSurface* OpCryPMTWinSurface = 
	new G4OpticalSurface("CryPMTWinSurface");
	
	OpCryPMTWinSurface->SetType(dielectric_dielectric);
	OpCryPMTWinSurface->SetModel(glisur);
	OpCryPMTWinSurface->SetFinish(polished);
	
	G4LogicalBorderSurface* CryPMTWinSurface = 
    new G4LogicalBorderSurface("CryPMTWinSurface",physiCrystal,physiPMTWindow,
							   OpCryPMTWinSurface);
	
	// PMT window - photocathode surface
	
	G4OpticalSurface* OpPMTWinCathSurface = new G4OpticalSurface("PMTWinCathSurface");
	
	OpPMTWinCathSurface->SetType(dielectric_dielectric);
	OpPMTWinCathSurface->SetModel(glisur);
	OpPMTWinCathSurface->SetFinish(polished);
	
	G4LogicalBorderSurface* PMTWinCathSurface = 
    new G4LogicalBorderSurface("CathodeSurface",physiPMTWindow,physiCathode,
							   OpPMTWinCathSurface);
	
	
	//------------------------------------------------------
	// visualization attributes
	//------------------------------------------------------
	
	logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
	
	//Green color for scintillator crystal
	G4VisAttributes* Att1= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
	logicCrystal->SetVisAttributes(Att1);
	
	//Yellow color for reflector
	G4VisAttributes* Att2= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
	logicReflector->SetVisAttributes(Att2);
	
	//Blue color for PMT window
	G4VisAttributes* Att3= new G4VisAttributes(G4Colour(0.0,0.0,1.0));
	logicPMTWindow->SetVisAttributes(Att3);
	
	//White color for the absorber photocathode
	G4VisAttributes* Att4= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
	logicCathode->SetVisAttributes(Att4);
    
	//
	// always return the physical World
	//
return physiWorld;
}
*/

return physWorld;
}


