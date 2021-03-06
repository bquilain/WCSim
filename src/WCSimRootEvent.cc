// Based on Root test Event.cxx
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include "TObject.h"
#include "TDirectory.h"
#include "TProcessID.h"
#include <string>
#include <vector>

#include <TStopwatch.h>
#include "WCSimRootEvent.hh"

#ifndef REFLEX_DICTIONARY
ClassImp(WCSimRootCherenkovDigiHit)
ClassImp(WCSimRootCherenkovHit)
ClassImp(WCSimRootCherenkovHitTime)
ClassImp(WCSimRootTrack)
ClassImp(WCSimRootPi0)
ClassImp(WCSimRootEventHeader)
ClassImp(WCSimRootTrigger)
ClassImp(WCSimRootEvent)
#endif
//#define DEBUG

//TClonesArray* WCSimRootTrigger::fgTracks = 0;
//
//TClonesArray* WCSimRootTrigger::fgCherenkovHits = 0;
//TClonesArray* WCSimRootTrigger::fgCherenkovHitTimes = 0;
//TClonesArray* WCSimRootTrigger::fgCherenkovDigiHits = 0;
//
//TClonesArray* WCSimRootTrigger::fgFVCherenkovHits = 0;
//TClonesArray* WCSimRootTrigger::fgFVCherenkovHitTimes = 0;
//TClonesArray* WCSimRootTrigger::fgFVCherenkovDigiHits = 0;
//
// 
//_____________________________________________________________________________

WCSimRootTrigger::WCSimRootTrigger()
{
  // Create an WCSimRootTrigger object.


  // WARNING : default constructor for ROOT : do not allocate memory
  // inside it or upon re-reading the object there will be a memory leak
  // TClonesArray of WCSimRootTracks
  fTracks = 0;
  fNtrack = 0;

  // TClonesArray of WCSimRootCherenkovHits
  fCherenkovHits = 0;
  fCherenkovHitTimes = 0;
  fNcherenkovhits = 0;
  fNcherenkovhittimes = 0;

  // TClonesArray of WCSimRootCherenkovDigiHits
  fCherenkovDigiHits = 0;
  fNcherenkovdigihits = 0;
  fSumQ = 0;

  fCaptures = 0;

  fTriggerType = kTriggerUndefined;
  fTriggerInfo.clear();
  
  IsZombie = true;
  
}

WCSimRootTrigger::WCSimRootTrigger(int Number,int Subevt)
{
  this->Initialize();
  fEvtHdr.Set(Number,0,0,Subevt);
}

//copy constructor --> only shallow copy of preallocated objects ??


void WCSimRootTrigger::Initialize() //actually allocate memory for things in here
{
  // Create an WCSimRootTrigger object.
  // When the constructor is invoked for the first time, the class static
  // variable fgTracks is 0 and the TClonesArray fgTracks is created.
  // Sim. for the other TClonesArray
  TStopwatch* mystopw = new TStopwatch();

  // TClonesArray of WCSimRootTracks
  fTracks = new TClonesArray("WCSimRootTrack", 10000);
  fNtrack = 0;

  // TClonesArray of WCSimRootCherenkovHits
  fCherenkovHits = new TClonesArray("WCSimRootCherenkovHit", 
				    10000);
  fCherenkovHitTimes = new TClonesArray("WCSimRootCherenkovHitTime", 
					10000);
  fNcherenkovhits = 0;
  fNcherenkovhittimes = 0;

  // TClonesArray of WCSimRootCherenkovDigiHits
  fCherenkovDigiHits = new TClonesArray("WCSimRootCherenkovDigiHit", 
				       10000);
  fNcherenkovdigihits = 0;
  fSumQ = 0;

  // TClonesArray of WCSimRootCaptures
  fCaptures = new TClonesArray("WCSimRootCapture", 100);
  fNcaptures = 0;

  fTriggerType = kTriggerUndefined;
  fTriggerInfo.clear();
  
  //  G4cout << " Time to allocate the TCAs :  Real = " << mystopw->RealTime() 
  //	    << " ; CPU = " << mystopw->CpuTime() << "\n";
  delete mystopw;
  

  IsZombie = false; // the memory has been allocated
}

//_____________________________________________________________________________

WCSimRootTrigger::~WCSimRootTrigger()
{
  // now we must do a bunch a deleting stuff...
  //Destroys all the TClonesArray.. Let's see if Ren'e Brun is right...

  TStopwatch* mystopw = new TStopwatch();

  mystopw->Start();

  if (!IsZombie) {

    fTracks->Delete();            
    fCherenkovHits->Delete();      
    fCherenkovHitTimes->Delete();   
    fCherenkovDigiHits->Delete();
    fCaptures->Delete();
    
    delete   fTracks;            
    delete   fCherenkovHits;      
    delete   fCherenkovHitTimes;   
    delete   fCherenkovDigiHits;
    delete   fCaptures;
  }
  mystopw->Stop();

  //  G4cout << " Time to delete the TCAs :  Real = " << mystopw->RealTime() 
  //    << " ; CPU = " << mystopw->CpuTime() << "\n";

  delete mystopw;
  //Clear("C");
}


//_____________________________________________________________________________

void WCSimRootTrigger::Clear(Option_t */*option*/)
{
  // To be filled in 
  // Filled in, by MF, 31/08/06  -> Keep all the alloc'ed memory but reset all
  // the indices to 0 in the TCAs.
  fNtrack = 0;

  // TClonesArray of WCSimRootCherenkovHits
  fNcherenkovhits = 0;
  fNcherenkovhittimes = 0;

  // TClonesArray of WCSimRootCherenkovDigiHits
  fNcherenkovdigihits = 0;
  fSumQ = 0;

  // TClonesArray of WCSimRootCaptures
  fNcaptures = 0;

  // remove whatever's in the arrays
  // but don't deallocate the arrays themselves

  fTracks->Delete();            
  fCherenkovHits->Delete();      
  fCherenkovHitTimes->Delete();   
  fCherenkovDigiHits->Delete();
  fCaptures->Delete();

  fTriggerType = kTriggerUndefined;
  fTriggerInfo.clear();

  IsZombie = false ; // we DO NOT deallocate the memory
}

//_____________________________________________________________________________

void WCSimRootTrigger::Reset(Option_t */*option*/)
{
// Static function to reset all static objects for this event
// To be filled in
}

//_____________________________________________________________________________

void WCSimRootTrigger::SetHeader(Int_t i, 
				  Int_t run, 
				  Int_t date,Int_t subevent)
{
  // Set the header values
  fEvtHdr.Set(i, run, date,subevent);
}

//_____________________________________________________________________________

void WCSimRootTrigger::SetTriggerInfo(TriggerType_t trigger_type,
				      std::vector<Double_t> trigger_info)
{
  fTriggerType = trigger_type;
  fTriggerInfo = trigger_info;
}

//_____________________________________________________________________________

void WCSimRootTrigger::SetPi0Info(Double_t pi0Vtx[3],
                                  Int_t   gammaID[2],
                                  Double_t gammaE[2],
                                  Double_t gammaVtx[2][3])
{
    fPi0.Set(pi0Vtx,
             gammaID,
             gammaE,
             gammaVtx);
}

//_____________________________________________________________________________

void WCSimRootTrigger::SetCaptureParticle(Int_t parent,
                                          Int_t ipnu,
                                          Double_t time,
                                          Double_t vtx[3],
                                          Double_t dir[3],
                                          Double_t energy,
                                          Int_t id)
{
    WCSimRootCapture * capture = 0;
    for(int i = 0; i<fCaptures->GetEntriesFast(); i++){
        if(((WCSimRootCapture*)fCaptures->At(i))->GetCaptureParent() == parent) {
            capture = (WCSimRootCapture *) fCaptures->At(i);
            break;
        }
    }
    if(capture == 0) {
        TClonesArray &captures = *fCaptures;
        capture = new(captures[fNcaptures++]) WCSimRootCapture(parent);
    }
    if(ipnu==22) capture->AddGamma(id, energy, dir);
    else capture->SetInfo(vtx, time, ipnu);
}

//_____________________________________________________________________________

void WCSimRootPi0::Set(Double_t pi0Vtx[3],
                       Int_t   gammaID[2],
                       Double_t gammaE[2],
                       Double_t gammaVtx[2][3])
{
    for (int i=0;i<2;i++)
    {
        fGammaID[i] = gammaID[i];
        fGammaE[i]  = gammaE[i];
    }

    for (int j=0;j<3;j++)
    {
        fPi0Vtx[j]      = pi0Vtx[j];
        fGammaVtx[0][j] = gammaVtx[0][j];
        fGammaVtx[1][j] = gammaVtx[1][j];
    }
}

//_____________________________________________________________________________

WCSimRootCapture::WCSimRootCapture(Int_t captureParent)
{
    fCaptureParent = captureParent;
    fNGamma = 0;
    fTotalGammaE = 0;
    fGammas = new TClonesArray("WCSimRootCaptureGamma", 10);
    IsZombie=false;
}

//_____________________________________________________________________________

WCSimRootCapture::~WCSimRootCapture()
{
    if(!IsZombie) {
        fGammas->Delete();
        delete fGammas;
    }
}

//_____________________________________________________________________________

void WCSimRootCapture::SetInfo(Double_t captureVtx[3],
                               Double_t captureT,
                               Int_t   captureNucleus)
{
    for (int i=0;i<3;i++) fCaptureVtx[i] = captureVtx[i];
    fCaptureT = captureT;
    fCaptureNucleus = captureNucleus;
}

//_____________________________________________________________________________

void WCSimRootCapture::AddGamma(Int_t   gammaID,
                                Double_t gammaE,
                                Double_t gammaDir[3])
{
    TClonesArray &gammas = *fGammas;
    new(gammas[fNGamma]) WCSimRootCaptureGamma(gammaID, gammaE, gammaDir);
    fTotalGammaE += gammaE;
    fNGamma++;
}

//_____________________________________________________________________________

WCSimRootCaptureGamma::WCSimRootCaptureGamma(Int_t id,
                                             Double_t energy,
                                             Double_t *dir) {
    fID = id;
    fEnergy = energy;
    for(int i=0;i<3;i++) fDir[i] = dir[i];
}

//_____________________________________________________________________________

WCSimRootTrack *WCSimRootTrigger::AddTrack(Int_t ipnu,
					   Int_t flag, 
					   Double_t m, 
					   Double_t p, 
					   Double_t E, 
					   Int_t startvol, 
					   Int_t stopvol, 
					   Double_t dir[3], 
					   Double_t pdir[3], 
					   Double_t stop[3], 
					   Double_t start[3],
					   Int_t parenttype,
					   Double_t time,
					   Int_t id)
{
  // Add a new WCSimRootTrack to the list of tracks for this event.
  // To avoid calling the very time consuming operator new for each track,
  // the standard but not well know C++ operator "new with placement"
  // is called. If tracks[i] is 0, a new Track object will be created
  // otherwise the previous Track[i] will be overwritten.

  TClonesArray &tracks = *fTracks;
  WCSimRootTrack *track = 
    new(tracks[fNtrack++]) WCSimRootTrack(ipnu,
					   flag,
					   m,
					   p,
					   E,
					   startvol,
					   stopvol,
					   dir,
					   pdir,
					   stop,
					   start,
					   parenttype,
					  time,id);

  return track;
}


//_____________________________________________________________________________

WCSimRootTrack::WCSimRootTrack(Int_t ipnu, 
				 Int_t flag, 
				 Double_t m, 
				 Double_t p, 
				 Double_t E, 
				 Int_t startvol, 
				 Int_t stopvol, 
				 Double_t dir[3], 
				 Double_t pdir[3], 
				 Double_t stop[3], 
				 Double_t start[3],
				 Int_t parenttype,
			       Double_t time,Int_t id)
{

  // Create a WCSimRootTrack object and fill it with stuff

  fIpnu = ipnu;
  fFlag = flag;
  fM = m;
  fP = p;
  fE = E;
  fStartvol = startvol;
  fStopvol = stopvol;
  int i;
  for (i=0;i<3;i++)
  {
    fDir[i] = dir[i]; 
    fPdir[i] = pdir[i]; 
    fStop[i] = stop[i]; 
    fStart[i] = start[i];
  }
  fParenttype = parenttype;
  fTime = time;
  fId = id;
}


//_____________________________________________________________________________

WCSimRootCherenkovHit *WCSimRootTrigger::AddCherenkovHit(Int_t tubeID,
							 Int_t mPMTID,
							 Int_t mPMT_PMTID,
							 std::vector<Double_t> truetime,
							 std::vector<Int_t> primParID,
							 std::vector<Float_t> photonStartTime,
							 std::vector<TVector3> photonStartPos,
							 std::vector<TVector3> photonEndPos,
							 std::vector<TVector3> photonStartDir,
							 std::vector<TVector3> photonEndDir)

{
  // Add a new Cherenkov hit to the list of Cherenkov hits
  TClonesArray &cherenkovhittimes = *fCherenkovHitTimes;

  for (unsigned int i =0;i<truetime.size();i++)
  {
    fCherenkovHitCounter++;
    Float_t startPos[3];
    Float_t endPos[3];
    Float_t startDir[3];
    Float_t endDir[3];
    for(int j=0; j<3; j++){
      startPos[j] = photonStartPos[i][j];
      endPos[j] = photonEndPos[i][j];
      startDir[j] = photonStartDir[i][j];
      endDir[j] = photonEndDir[i][j];
    }
    WCSimRootCherenkovHitTime *cherenkovhittime = 
      new(cherenkovhittimes[fNcherenkovhittimes++]) WCSimRootCherenkovHitTime(truetime[i],primParID[i],
                                                                              photonStartTime[i], startPos, endPos,
                                                                              startDir, endDir);
  }
  
#ifdef DEBUG
  std::cout << "Debug B.Q: A new Cherenkov hit was added for tube " << tubeID << ", corresponding to PMT = " << mPMTID << ", and to small PMT in mPMT = " << mPMT_PMTID << std::endl;
#endif

  Int_t WC_Index[2];
  WC_Index[0] = fNcherenkovhittimes-truetime.size(); //fCherenkovHitCounter-truetime.size();
  WC_Index[1] = truetime.size();

  TClonesArray &cherenkovhits = *fCherenkovHits;
 
  WCSimRootCherenkovHit *cherenkovhit
    = new(cherenkovhits[fNcherenkovhits++]) WCSimRootCherenkovHit(tubeID,
								  mPMTID,
								  mPMT_PMTID,
								  WC_Index);

  return cherenkovhit;
}
//_____________________________________________________________________________

WCSimRootCherenkovHit::WCSimRootCherenkovHit(Int_t tubeID,
					     Int_t totalPe[2])
{
  // Create a WCSimRootCherenkovHitIndex object and fill it with stuff

  fTubeID     = tubeID;
  fTotalPe[0] = totalPe[0];
  fTotalPe[1] = totalPe[1];
}

//_____________________________________________________________________________

WCSimRootCherenkovHit::WCSimRootCherenkovHit(Int_t tubeID,
					     Int_t mPMTID,
					     Int_t mPMT_PMTID,
					     Int_t totalPe[2])
{
  // Create a WCSimRootCherenkovHitIndex object and fill it with stuff

  fTubeID     = tubeID;
  fmPMTID     = mPMTID;
  fmPMT_PMTID = mPMT_PMTID;
  fTotalPe[0] = totalPe[0];
  fTotalPe[1] = totalPe[1];
}

WCSimRootCherenkovHitTime::WCSimRootCherenkovHitTime(Double_t truetime,
						     Int_t primParID,
						     Float_t photonStartTime,
						     Float_t photonStartPos[3],
						     Float_t photonEndPos[3],
						     Float_t photonStartDir[3],
						     Float_t photonEndDir[3])
{
  // Create a WCSimRootCherenkovHit object and fill it with stuff
    fTruetime        = truetime; 
    fPrimaryParentID = primParID;
    fPhotonStartTime = photonStartTime;
    for (int i=0;i<3;i++) {
        fPhotonStartPos[i] = photonStartPos[i];
        fPhotonEndPos[i] = photonEndPos[i];
        fPhotonStartDir[i] = photonStartDir[i];
        fPhotonEndDir[i] = photonEndDir[i];
    }
}

//_____________________________________________________________________________

WCSimRootCherenkovDigiHit *WCSimRootTrigger::AddCherenkovDigiHit(Double_t q, 
								 Double_t t, 
								 Int_t tubeid,
								 Int_t mpmtid,
								 Int_t mpmt_pmtid,
								 std::vector<int> photon_ids)
{
  // Add a new digitized hit to the list of digitized hits
  TClonesArray &cherenkovdigihits = *fCherenkovDigiHits;
  WCSimRootCherenkovDigiHit *cherenkovdigihit = 
    new(cherenkovdigihits[fNcherenkovdigihits++]) WCSimRootCherenkovDigiHit(q, 
									    t, 
									    tubeid,
									    mpmtid,
									    mpmt_pmtid,
									    photon_ids);
 
  return cherenkovdigihit;
}
//_____________________________________________________________________________

WCSimRootCherenkovDigiHit::WCSimRootCherenkovDigiHit(Double_t q, 
						     Double_t t, 
						     Int_t tubeid,
						     std::vector<int> photon_ids)
{
  // Create a WCSimRootCherenkovDigiHit object and fill it with stuff

  fQ = q;
  fT = t;
  fTubeId = tubeid;
  fPhotonIds = photon_ids;
}

WCSimRootCherenkovDigiHit::WCSimRootCherenkovDigiHit(Double_t q, 
						     Double_t t, 
						     Int_t tubeid,
						     Int_t mpmtid,
						     Int_t mpmt_pmtid,
						     std::vector<int> photon_ids)
{
  // Create a WCSimRootCherenkovDigiHit object and fill it with stuff

  fQ = q;
  fT = t;
  fTubeId = tubeid;
  fmPMTId = mpmtid;
  fmPMT_PMTId = mpmt_pmtid;
  fPhotonIds = photon_ids;
}

// M Fechner, august 2006

WCSimRootEvent::WCSimRootEvent()
{
  // default constructor : nothing happens
  //fEventList.push_back(new WCSimRootTrigger() ); //at least one event 
  // this is standard root practise for streaming ROOT objtecs : if memory is alloc'ed here,
  // it will be lost
  fEventList = 0;
  Current = 0;
}

void WCSimRootEvent::Initialize()
{
  fEventList = new TObjArray(10,0); // very rarely more than 10 subevents...
  fEventList->AddAt(new WCSimRootTrigger(0,0),0);
  Current = 0;
}


WCSimRootEvent::~WCSimRootEvent()
{
  if (fEventList != 0) {
    for (int i = 0 ; i < fEventList->GetEntriesFast() ; i++) {
      delete (*fEventList)[i];
    }
    delete fEventList;
  }
  //  std::vector<WCSimRootTrigger*>::iterator  iter = fEventList.begin();
  //for ( ; iter != fEventList.end() ; ++iter) delete (*iter);
  //Clear("");
}

void WCSimRootEvent::Clear(Option_t* /*o*/)
{
  //nothing for now
}

void WCSimRootEvent::Reset(Option_t* /*o*/)
{
  //nothing for now
}
