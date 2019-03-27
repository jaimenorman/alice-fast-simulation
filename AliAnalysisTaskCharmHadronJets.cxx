/*************************************************************************
* Copyright(c) 1998-2016, ALICE Experiment at CERN, All rights reserved. *
*                                                                        *
* Author: The ALICE Off-line Project.                                    *
* Contributors are mentioned in the code where appropriate.              *
*                                                                        *
* Permission to use, copy, modify and distribute this software and its   *
* documentation strictly for non-commercial purposes is hereby granted   *
* without fee, provided that the above copyright notice appears in all   *
* copies and that both the copyright notice and this permission notice   *
* appear in the supporting documentation. The authors make no claims     *
* about the suitability of this software for any purpose. It is          *
* provided "as is" without express or implied warranty.                  *
**************************************************************************/

//C++
#include <sstream>
#include <array>

// Root
#include <TClonesArray.h>
#include <TDatabasePDG.h>
#include <TParticlePDG.h>
#include <TVector3.h>
#include <THnSparse.h>
#include <TParticle.h>
#include <TMath.h>
#include <THashList.h>
#include <TFile.h>
#include <TRandom3.h>

// Aliroot general
#include "AliLog.h"
#include "AliEMCALGeometry.h"
#include "AliAnalysisManager.h"
#include "AliVEventHandler.h"
#include <THistManager.h>

// Aliroot HF
#include "AliAODRecoCascadeHF.h"
#include "AliAODRecoDecayHF2Prong.h"
#include "AliRDHFCutsD0toKpi.h"
#include "AliRDHFCutsDStartoKpipi.h"
#include "AliHFAODMCParticleContainer.h"
#include "AliHFTrackContainer.h"
#include "AliAnalysisVertexingHF.h"

// Aliroot EMCal jet framework
#include "AliEmcalJetTask.h"
#include "AliEmcalJet.h"
#include "AliJetContainer.h"
#include "AliParticleContainer.h"
#include "AliEmcalParticle.h"
#include "AliFJWrapper.h"
#include "AliRhoParameter.h"

#include "AliAnalysisTaskCharmHadronJets.h"

// Definitions of class AliAnalysisTaskCharmHadronJets

/// \cond CLASSIMP
ClassImp(AliAnalysisTaskCharmHadronJets);
/// \endcond

/// This is the default constructor, used for ROOT I/O purposes.
AliAnalysisTaskCharmHadronJets::AliAnalysisTaskCharmHadronJets() :
  AliAnalysisTaskEmcalJetLight(),
  fOutputType(kTreeOutput),
  fHistManager(),
  fApplyKinematicCuts(kTRUE),
  fNOutputTrees(0),
  fRejectISR(kFALSE),
  fJetAreaType(fastjet::active_area),
  fJetGhostArea(0.005),
  fJetType(AliJetContainer::kChargedJet),
  fJetAlgo (AliJetContainer::antikt_algorithm),
  fJetRecoScheme(AliJetContainer::pt_scheme),
  fJetR(0.4),
  fMCContainer(0),
  fAodEvent(0),
  fFastJetWrapper(0)
{
  SetMakeGeneralHistograms(kTRUE);
}

/// This is the standard named constructor.
///
/// \param name Name of the task
AliAnalysisTaskCharmHadronJets::AliAnalysisTaskCharmHadronJets(const char* name, Int_t nOutputTrees) :
  AliAnalysisTaskEmcalJetLight(name, kTRUE),
  fOutputType(kTreeOutput),
  fHistManager(name),
  fApplyKinematicCuts(kTRUE),
  fNOutputTrees(nOutputTrees),
  fRejectISR(kFALSE),
  fJetAreaType(fastjet::active_area),
  fJetGhostArea(0.005),
  fJetType(AliJetContainer::kChargedJet),
  fJetAlgo (AliJetContainer::antikt_algorithm),
  fJetRecoScheme(AliJetContainer::pt_scheme),
  fJetR(0.4),
  fMCContainer(0),
  fAodEvent(0),
  fFastJetWrapper(0)
{
  SetMakeGeneralHistograms(kTRUE);
  for (Int_t i = 0; i < nOutputTrees; i++){
    DefineOutput(2+i, TTree::Class());
  }
}

///// This is the standard destructor.
AliAnalysisTaskCharmHadronJets::~AliAnalysisTaskCharmHadronJets()
{
  if (fFastJetWrapper) delete fFastJetWrapper;
}


/// Creates the output containers.
void AliAnalysisTaskCharmHadronJets::UserCreateOutputObjects()
{
  ::Info("UserCreateOutputObjects", "CreateOutputObjects of task %s", GetName());

  AliAnalysisTaskEmcalJetLight::UserCreateOutputObjects();

  // Define histograms
  // the TList fOutput is already defined in  AliAnalysisTaskEmcalJetLight::UserCreateOutputObjects()

  TString hname;
  TString htitle;


  hname = "fHistCharmPt";
  htitle = hname + ";#it{p}_{T,charm} (GeV/#it{c});counts";
  fHistManager.CreateTH1(hname, htitle, 500, 0, 1000);

  hname = "fHistCharmEta";
  htitle = hname + ";#eta_{charm};counts";
  fHistManager.CreateTH1(hname, htitle, 400, -10, 10);

  hname = "fHistCharmPhi";
  htitle = hname + ";#phi_{charm};counts";
  fHistManager.CreateTH1(hname, htitle, 125, 0, TMath::TwoPi());

  hname = "fHistBottomPt";
  htitle = hname + ";#it{p}_{T,bottom} (GeV/#it{c});counts";
  fHistManager.CreateTH1(hname, htitle, 500, 0, 1000);

  hname = "fHistBottomEta";
  htitle = hname + ";#eta_{bottom};counts";
  fHistManager.CreateTH1(hname, htitle, 400, -10, 10);

  hname = "fHistBottomPhi";
  htitle = hname + ";#phi_{bottom};counts";
  fHistManager.CreateTH1(hname, htitle, 125, 0, TMath::TwoPi());

  hname = "fHistHighestPartonPt";
  htitle = hname + ";#it{p}_{T,bottom} (GeV/#it{c});counts";
  fHistManager.CreateTH1(hname, htitle, 500, 0, 1000);

  hname = "fHistHighestPartonType";
  htitle = hname + ";type;counts";
  fHistManager.CreateTH1(hname, htitle, 10, 0, 10);

  hname = "fHistNHeavyQuarks";
  htitle = hname + ";number of heavy-quarks;counts";
  fHistManager.CreateTH1(hname, htitle, 21, -0.5, 20.5);

  hname = "fhParticles_pt";
  htitle = hname + ";p_{T} (GeV/c);counts";
  fHistManager.CreateTH1(hname, htitle, 100, 0, 20);

  hname="fD0Pt";
  htitle= hname + ";#it{p}_{T,D0} (GeV/#it{c});counts";
  fHistManager.CreateTH1(hname,htitle,100,0,12);

  hname="fD0PtCut1";
  htitle= hname + ";#it{p}_{T,D0} (GeV/#it{c});counts";
  fHistManager.CreateTH1(hname,htitle,100,0,12);

  hname="fD0PtCut2";
  htitle= hname + ";#it{p}_{T,D0} (GeV/#it{c});counts";
  fHistManager.CreateTH1(hname,htitle,100,0,12);

  hname="fD0Phi";
  htitle= hname + ";#phi_{D0};counts";
  fHistManager.CreateTH1(hname,htitle,125,0,TMath::TwoPi());

  hname="fD0Eta";
  htitle= hname + ";#eta_{D0};counts";
  fHistManager.CreateTH1(hname,htitle,50,-10,10);


  hname="fLcPt";
  htitle= hname + ";#it{p}_{T,#Lambda_{C}} (GeV/#it{c});counts";
  fHistManager.CreateTH1(hname,htitle,100,0,12);

  hname="fLcPtCut1";
  htitle= hname + ";#it{p}_{T,#Lambda_{C}} (GeV/#it{c});counts";
  fHistManager.CreateTH1(hname,htitle,100,0,12);

  hname="fLcPtCut2";
  htitle= hname + ";#it{p}_{T,#Lambda_{C}} (GeV/#it{c});counts";
  fHistManager.CreateTH1(hname,htitle,100,0,12);

  hname="fLcPhi";
  htitle= hname + ";#phi_{#Lambda_{C}};counts";
  fHistManager.CreateTH1(hname,htitle,125,0,TMath::TwoPi());

  hname="fLcEta";
  htitle= hname + ";#eta_{#Lambda_{C}};counts";
  fHistManager.CreateTH1(hname,htitle,50,-10,10);

  hname = "fMotherLc_pt";
  htitle = hname + ";p_{T,#Lambda_{C}} (GeV/c);counts";
  fHistManager.CreateTH1(hname, htitle, 100, 0, 12);

  hname = "fDaughterLc_pt";
  htitle = hname + ";p_{T,#Lambda_{C}} (GeV/c);counts";
  fHistManager.CreateTH1(hname, htitle, 100, 0, 12);

  hname = "fMotherD0_pt";
  htitle = hname + ";p_{T,D0} (GeV/c);counts";
  fHistManager.CreateTH1(hname, htitle, 100, 0, 12);

  hname = "fDaughterD0_pt";
  htitle = hname + ";p_{T,D0} (GeV/c);counts";
  fHistManager.CreateTH1(hname, htitle, 100, 0, 12);

  hname = "fZ_D0";
  htitle = hname + ";Z_{D0} ;counts";
  fHistManager.CreateTH1(hname, htitle, 100, 0, 2.5);

  hname = "fZ_Lc";
  htitle = hname + ";Z_{Lc} ;counts";
  fHistManager.CreateTH1(hname, htitle, 100, 0, 2.5);

  hname = "fMomentZ_D0";
  htitle = hname + ";Z_{D0} ;counts";
  fHistManager.CreateTH1(hname, htitle, 100, 0, 2.5);

  hname = "fMomentZ_Lc";
  htitle = hname + ";Z_{Lc} ;counts";
  fHistManager.CreateTH1(hname, htitle, 100, 0, 2.5);

  hname = "fRKaon_Pt";
  htitle = hname + ";R ;p_{T,D0}";
  fHistManager.CreateTH2(hname,htitle,100,0,3,100,0,15);

  hname = "ProjKaon_R_Cut0";
  htitle = hname + ";R ;counts";
  fHistManager.CreateTH1(hname,htitle,100,0,4);

  hname = "ProjKaon_R_Cut1";
  htitle = hname + ";R ;counts";
  fHistManager.CreateTH1(hname,htitle,100,0,4);

  hname = "ProjKaon_R_Cut2";
  htitle = hname + ";R ;counts";
  fHistManager.CreateTH1(hname,htitle,100,0,4);

  hname = "ProjKaon_R_Cut3";
  htitle = hname + ";R ;counts";
  fHistManager.CreateTH1(hname,htitle,100,0,4);

  hname = "fRPion_Pt";
  htitle = hname + ";R ;p_{T,D0}";
  fHistManager.CreateTH2(hname,htitle,100,0,3,100,0,15);

  hname = "ProjPion_R_Cut0";
  htitle = hname + ";R ;counts";
  fHistManager.CreateTH1(hname,htitle,100,0,4);

  hname = "ProjPion_R_Cut1";
  htitle = hname + ";R ;counts";
  fHistManager.CreateTH1(hname,htitle,100,0,4);

  hname = "ProjPion_R_Cut2";
  htitle = hname + ";R ;counts";
  fHistManager.CreateTH1(hname,htitle,100,0,4);

  hname = "ProjPion_R_Cut3";
  htitle = hname + ";R ;counts";
  fHistManager.CreateTH1(hname,htitle,100,0,4);

  hname = "LcfRKaon_Pt";
  htitle = hname + ";R ;p_{T,D0}";
  fHistManager.CreateTH2(hname,htitle,100,0,3,100,0,15);

  hname = "LcProjKaon_R_Cut0";
  htitle = hname + ";R ;counts";
  fHistManager.CreateTH1(hname,htitle,100,0,4);

  hname = "LcProjKaon_R_Cut1";
  htitle = hname + ";R ;counts";
  fHistManager.CreateTH1(hname,htitle,100,0,4);

  hname = "LcProjKaon_R_Cut2";
  htitle = hname + ";R ;counts";
  fHistManager.CreateTH1(hname,htitle,100,0,4);

  hname = "LcProjKaon_R_Cut3";
  htitle = hname + ";R ;counts";
  fHistManager.CreateTH1(hname,htitle,100,0,4);

  hname = "LcfRPion_Pt";
  htitle = hname + ";R ;p_{T,D0}";
  fHistManager.CreateTH2(hname,htitle,100,0,3,100,0,15);

  hname = "LcProjPion_R_Cut0";
  htitle = hname + ";R ;counts";
  fHistManager.CreateTH1(hname,htitle,100,0,4);

  hname = "LcProjPion_R_Cut1";
  htitle = hname + ";R ;counts";
  fHistManager.CreateTH1(hname,htitle,100,0,4);

  hname ="LcProjPion_R_Cut2";
  htitle = hname + ";R ;counts";
  fHistManager.CreateTH1(hname,htitle,100,0,4);

  hname = "LcProjPion_R_Cut3";
  htitle = hname + ";R ;counts";
  fHistManager.CreateTH1(hname,htitle,100,0,4);

  // jet level histograms

  hname = "fHistConstJet_DR";
  htitle = hname + ";#Delta R;counts";
  fHistManager.CreateTH1(hname, htitle, 50, 0, 1.0);

  hname = "fHistConstJet_Z";
  htitle = hname + ";Z_{const.-jet};counts";
  fHistManager.CreateTH1(hname, htitle, 50, 0, 1.5);

  hname = "fHistConstJet_ptConst_ptJet";
  htitle = hname + ";p_{T,const,};p_{T,jet};counts";
  fHistManager.CreateTH2(hname, htitle, 60, 0, 30, 60, 0, 30 );

  hname = "fHistConstJet_ptConst_ptJet_Z";
  htitle = hname + ";p_{T,const,};p_{T,jet};Z_{const.-jet}";
  fHistManager.CreateTH3(hname, htitle, 60, 0, 30, 60, 0, 30, 50, 0, 1.5 );

  hname = "fHistConstJet_ptConst_ptJet_DR";
  htitle = hname + ";p_{T,const,};p_{T,jet};#Delta R_{const.-jet}";
  fHistManager.CreateTH3(hname, htitle, 60, 0, 30, 60, 0, 30, 50, 0, 1.5 );

  hname = "fHistDJet_Z";
  htitle = hname + ";Z_{D-jet} ;counts";
  fHistManager.CreateTH1(hname, htitle, 50, 0, 1.5);

  hname = "fHistDJet_DR";
  htitle = hname + ";#Delta R;counts";
  fHistManager.CreateTH1(hname, htitle, 50, 0, 1.0);

  hname = "fHistDJet_ptD_ptJet";
  htitle = hname + ";p_{T,const,};p_{T,jet};counts";
  fHistManager.CreateTH2(hname, htitle, 60, 0, 30, 60, 0, 30 );

  hname = "fHistDJet_ptD_ptJet_Z";
  htitle = hname + ";p_{T,const,};p_{T,jet};Z_{const.-jet}";
  fHistManager.CreateTH3(hname, htitle, 60, 0, 30, 60, 0, 30, 50, 0, 1.5 );

  hname = "fHistDJet_ptD_ptJet_DR";
  htitle = hname + ";p_{T,const,};p_{T,jet};#Delta R_{const.-jet}";
  fHistManager.CreateTH3(hname, htitle, 60, 0, 30, 60, 0, 30, 50, 0, 1.5 );

  hname = "fHistNConstInJet";
  htitle = hname + ";N_{const.};counts";
  fHistManager.CreateTH1(hname, htitle, 50, 0.5, 50.5);

  hname = "fHistNConstInDJet";
  htitle = hname + ";N_{const.};counts";
  fHistManager.CreateTH1(hname, htitle, 50, 0.5, 50.5);

  hname = "fHistPtJet";
  htitle = hname + ";p_{T,Jet} (GeV/c) ;counts";
  fHistManager.CreateTH1(hname,htitle,45,5,50);

  hname = "fHistPhiJet";
  htitle = hname + ";#phi_{Jet} ;counts ";
  fHistManager.CreateTH1(hname,htitle,125,0,TMath::TwoPi());

  hname = "fHistEtaJet";
  htitle = hname +";#eta_{Jet} ;counts ";
  fHistManager.CreateTH1(hname,htitle,50,-10,10);

  hname = "fHistJetPt_D0";
  htitle = hname + ";p_{T,D0Jet} (GeV/c) ;counts";
  fHistManager.CreateTH1(hname,htitle,99,1,100);

  hname = "fHistLcJet_DR";
  htitle = hname + ";#Delta R;counts";
  fHistManager.CreateTH1(hname, htitle, 50, 0, 1.0);

  hname = "fHistLcJet_ptLc_ptJet";
  htitle = hname + ";p_{T,const,};p_{T,jet};counts";
  fHistManager.CreateTH2(hname, htitle, 60, 0, 30, 60, 0, 30 );

  hname = "fHistLCJet_ptLc_ptJet_Z";
  htitle = hname + ";p_{T,const,};p_{T,jet};Z_{const.-jet}";
  fHistManager.CreateTH3(hname, htitle, 60, 0, 30, 60, 0, 30, 50, 0, 1.5 );

  hname = "fHistLcJet_ptLc_ptJet_DR";
  htitle = hname + ";p_{T,const,};p_{T,jet};#Delta R_{const.-jet}";
  fHistManager.CreateTH3(hname, htitle, 60, 0, 30, 60, 0, 30, 50, 0, 1.5 );

  hname = "fHistLcJet_Z";
  htitle = hname + ";Z_{#Lambda_{c}-jet} ;counts";
  fHistManager.CreateTH1(hname, htitle, 50, 0, 1.5);

  hname = "fHistNConstInLcJet";
  htitle = hname + ";N_{const.};counts";
  fHistManager.CreateTH1(hname, htitle, 50, 0.5, 50.5);

  hname = "fHistJetPt_Lc";
  htitle = hname + ";p_{T,#Lambda_{c} Jet} (GeV/c) ;counts";
  fHistManager.CreateTH1(hname,htitle,99,1,100);


  // TO DO - set other histograms

  fOutput->Add(fHistManager.GetListOfHistograms());

  PostData(1, fOutput);
}

/// Does some specific initializations for the analysis engines,
/// then calls the base class ExecOnce() method.
void AliAnalysisTaskCharmHadronJets::ExecOnce()
{
  AliAnalysisTaskEmcalJetLight::ExecOnce();

  // Load the event
  fAodEvent = dynamic_cast<AliAODEvent*>(fInputEvent);

  fFastJetWrapper = new AliFJWrapper(fName, fTitle);

  fFastJetWrapper->SetAreaType((fastjet::AreaType)fJetAreaType);
  fFastJetWrapper->SetGhostArea(fJetGhostArea);

  if (!fAodEvent) {
     AliError(Form("This task need an AOD event (Task '%s'). Expect troubles...", GetName()));
     //return;
  }

  for (auto cont_it : fParticleCollArray) {
    AliHFAODMCParticleContainer* part_cont = dynamic_cast<AliHFAODMCParticleContainer*>(cont_it.second);
    if (part_cont) fMCContainer = part_cont;
    else {
      Printf("Error - cannot find MC container");
      return;
    }
  }

  for (auto cont_it : fJetCollArray) {
    AliJetContainer* jet_cont = dynamic_cast<AliJetContainer*>(cont_it.second);
    Printf (" jet container found, with name %s",jet_cont->GetName());
  }

}

/// Run the analysis
///
/// \return kTRUE on success
Bool_t AliAnalysisTaskCharmHadronJets::Run()
{

  RunParticleLevelAnalysis();

  return kTRUE;
}

/// Adds all the particles contained in the container into the fastjet wrapper
///
/// \param cont Pointer to a valid AliEmcalContainer object
void AliAnalysisTaskCharmHadronJets::AddInputVectors(AliEmcalContainer* cont, Int_t offset, TH2* rejectHist, Double_t eff)
{
  auto itcont = cont->all_momentum();
  for (AliEmcalIterableMomentumContainer::iterator it = itcont.begin(); it != itcont.end(); it++) {
    UInt_t rejectionReason = 0;
    if (!cont->AcceptObject(it.current_index(), rejectionReason)) {
      if (rejectHist) rejectHist->Fill(AliEmcalContainer::GetRejectionReasonBitPosition(rejectionReason), it->first.Pt());
      continue;
    }
//    if (fRandomGen && eff > 0 && eff < 1) {
//      Double_t rnd = fRandomGen->Rndm();
//      if (eff < rnd) {
//        if (rejectHist) rejectHist->Fill(6, it->first.Pt());
//        continue;
//      }
//    }
    Int_t uid = offset >= 0 ? it.current_index() + offset: -it.current_index() - offset;
    fFastJetWrapper->AddInputVector(it->first.Px(), it->first.Py(), it->first.Pz(), it->first.E(), uid);

  }
}

///
/// Our main particle level analysis
///
///

void AliAnalysisTaskCharmHadronJets::RunParticleLevelAnalysis()
{

  // Some settings (ignore for now - we may change this)

  fMCContainer->SetSpecialPDG(fCandidatePDG);
  fMCContainer->SetRejectedOriginMap(fRejectedOrigin);
  fMCContainer->SetAcceptedDecayMap(fAcceptedDecay);
  fMCContainer->SetRejectISR(fRejectISR);
  fMCContainer->SetParticlePtCut(0.15);

  // Jet reconstruction settings

  // whether to use full, charged or neutral jets
  switch (fJetType) {
    case AliJetContainer::kFullJet:
      fMCContainer->SetCharge(AliParticleContainer::EChargeCut_t::kNoChargeCut);
      break;
    case AliJetContainer::kChargedJet:
      fMCContainer->SetCharge(AliParticleContainer::EChargeCut_t::kCharged);
      break;
    case AliJetContainer::kNeutralJet:
      fMCContainer->SetCharge(AliParticleContainer::EChargeCut_t::kNeutral);
      break;
  }

  // Set the jet reconstruction settings
  fFastJetWrapper->Clear();
  fFastJetWrapper->SetR(fJetR);
  fFastJetWrapper->SetAlgorithm(AliEmcalJetTask::ConvertToFJAlgo(fJetAlgo));
  fFastJetWrapper->SetRecombScheme(AliEmcalJetTask::ConvertToFJRecoScheme(fJetRecoScheme));
  //fFastJetWrapper->SetMinJetPt(1);

  // add all input vectors in the MC container to the jet finder
  AddInputVectors(fMCContainer, 100, 0);

  // run the jet finder
  fFastJetWrapper->Run();

  //
  // particle loop - loop over all particles in MC container
  //
  auto itpart = fMCContainer->all_momentum();
  for(auto part : itpart) {
    // part is a type pair <AliTLorentzVector, AliAODMCParticle *>
    // where the first and second element can be accessed by
    // part.first and part.second
    fHistManager.FillTH1("fhParticles_pt", part.second->Pt());

    UInt_t pdg = part.second->PdgCode();
   double_t eta= part.second->Eta();

   // UInt_t idmother=part.second->GetMother();


   // AliAODMCParticle* part1 = (AliAODMCParticle*) fMCContainer->GetMCParticleWithLabel(idmother);
   // UInt_t moth=part1->GetPdgCode();

    //fHistManager.FillTH1("fMother_pt", part1->Pt());


   // UInt_t idmother=part.second->GetMother();
   // AliAODMCParticle* part1 = (AliAODMCParticle*) fMCContainer->GetMCParticleWithLabel(idmother);
  //  UInt_t moth=part1->GetPdgCode();

  //  while ( moth > 6 && moth < -6 ) {
//	    idmother = part1->GetMother();
//	    if (idmother<0){ break; }
//	    part1 = fMCContainer->GetMCParticleWithLabel(idmother);
//	    moth=part1->GetPdgCode();
//	    printf("The PDG mother = %f \n", moth);
  //  }

    //printf("The Final PDG of quark is:  %f \n",moth);

    //Condition for lambda c
    if ( pdg == 4122 || pdg == -4122 )
    { fHistManager.FillTH1("fLcPt",part.second->Pt());
	    fHistManager.FillTH1("fLcPhi",part.second->Phi());
	    fHistManager.FillTH1("fLcEta",part.second->Eta());
	    if ( eta < 1 && eta > -1 ) {fHistManager.FillTH1("fLcPtCut1",part.second->Pt());}
               UInt_t idmother= part.second->GetMother();
               UInt_t moth;
               UInt_t k = 1;
        AliAODMCParticle* moth1 = (AliAODMCParticle*) fMCContainer->GetMCParticleWithLabel(idmother);
        fHistManager.FillTH1("fMotherLc_pt", moth1->Pt());

        UInt_t iddaughter=part.second->GetFirstDaughter();
        AliAODMCParticle* part2 = (AliAODMCParticle*) fMCContainer->GetMCParticleWithLabel(iddaughter);
        UInt_t daught=part2->GetPdgCode();
        fHistManager.FillTH1("fDaughterLc_pt", part2->Pt());

        Int_t icount = 0;
	       while (k==1) {
           if ( idmother < 0 ) { break; }
		       AliAODMCParticle* part1 = (AliAODMCParticle*) fMCContainer->GetMCParticleWithLabel(idmother);
           if(!part1) {break;}
		       moth=part1->GetPdgCode();
           idmother = part1->GetMother();
		       printf("Lc - The PDG mother = %i \n", moth);
		       //if ( TMath::Abs(moth) <= 6 ) //found quark
           if (moth==-6||moth==-5||moth==-4||moth==-3||moth==-2||moth==-1||moth==0||moth==1||moth==2||moth==3||moth==4||moth==5||moth==6)
           {
             if ( moth != 5 && moth != -5 ) { fHistManager.FillTH1("fLcPtCut2",part.second->Pt()); }

             if ( moth == 4 || moth == -4) {
             fHistManager.FillTH1("fZ_Lc",part.second->Pt()/part1->Pt());
             Double_t pcharm = part1->P();
             Double_t pLc =part.second->P();
             Double_t ZLc = pLc / pcharm ;
             printf(" The fragmentation function is ZLc = %f \n",ZLc);
             //fHistManager.FillTH1("fMomentZ_Lc",TMath::Sqrt(part.first.Px()*part.first.Px()+part.first.Py()*part.first.Py()+part.first.Pz()*part.first.Pz())/TMath::Sqrt(part1->Px()*part1->Px()+part1->Py()*part1->Py()+part1->Pz()*part1->Pz()));
             fHistManager.FillTH1("fMomentZ_Lc",ZLc);}


             break;
           }
           if ( moth == 2212 ) break; //found proton!


           icount++;
           if(icount>10) break;
	       }
         printf("reached end of Lc mother loop");
         //Daughter
         UInt_t Ndaught = part.second->GetNDaughters();
        // if we want all the N daughters.
         for (Int_t j = 0; j <= Ndaught; j++){
           UInt_t iddaught = part.second->GetDaughter(j);
           AliAODMCParticle* daught1 = (AliAODMCParticle*) fMCContainer->GetMCParticleWithLabel(iddaught);
           UInt_t Pdgdaught1= daught1->GetPdgCode();
           printf("The daughter of generation %i is %i \n ",j,Pdgdaught1);

           // if Kaon daughter is detected:
              if (Pdgdaught1==311||Pdgdaught1==313||Pdgdaught1==321||Pdgdaught1==323||Pdgdaught1==-311||Pdgdaught1==-313||Pdgdaught1==-321||Pdgdaught1==-323)
           {
             double_t DEta = daught1->Eta();
             double_t DPhi = daught1->Phi();
             double_t DPt = daught1->Pt();
             double_t rD0 = TMath::Sqrt((DEta-part.second->Eta())*(DEta-part.second->Eta())+(DPhi-part.second->Phi())*(DPhi-part.second->Phi()));
             fHistManager.FillTH2("LcfRKaon_Pt",rD0,part.second->Pt());

             Double_t D0Pt = part.second->Pt();

               if (D0Pt<3 && D0Pt>0) { fHistManager.FillTH1("LcProjKaon_R_Cut1",rD0); }
               if (D0Pt<7 && D0Pt>3) { fHistManager.FillTH1("LcProjKaon_R_Cut2",rD0); }
               if (D0Pt<12 && D0Pt>7) { fHistManager.FillTH1("LcProjKaon_R_Cut3",rD0); }
               if (D0Pt<12 && D0Pt>0) { fHistManager.FillTH1("LcProjKaon_R_Cut0",rD0);}

           }
           // If PION detected
           if (Pdgdaught1==211||Pdgdaught1==-211||Pdgdaught1==111)
           {
             double_t DEta = daught1->Eta();
             double_t DPhi = daught1->Phi();
             double_t DPt = daught1->Pt();
             double_t rD0 = TMath::Sqrt((DEta-part.second->Eta())*(DEta-part.second->Eta())+(DPhi-part.second->Phi())*(DPhi-part.second->Phi()));
             fHistManager.FillTH2("LcfRPion_Pt",rD0,part.second->Pt());

             Double_t D0Pt = part.second->Pt();

               if (D0Pt<3 && D0Pt>0) { fHistManager.FillTH1("LcProjPion_R_Cut1",rD0); }
               if (D0Pt<7 && D0Pt>3) { fHistManager.FillTH1("LcProjPion_R_Cut2",rD0); }
               if (D0Pt<12 && D0Pt>7) { fHistManager.FillTH1("LcProjPion_R_Cut3",rD0); }
               if (D0Pt<12 && D0Pt>0) { fHistManager.FillTH1("LcProjPion_R_Cut0",rD0);}
           }

         }

    }


    //Condition for D0
   if ( pdg == 421 || pdg == -421)
   { fHistManager.FillTH1("fD0Pt",part.second->Pt());
     fHistManager.FillTH1("fD0Phi",part.second->Phi());
     fHistManager.FillTH1("fD0Eta",part.second->Eta());
         if ( eta < 1 && eta > -1 ) {fHistManager.FillTH1("fD0PtCut1",part.second->Pt());}
	 UInt_t idmother= part.second->GetMother();
	 UInt_t moth;
	 UInt_t k=1;

   AliAODMCParticle* moth1 = (AliAODMCParticle*) fMCContainer->GetMCParticleWithLabel(idmother);
   fHistManager.FillTH1("fMotherD0_pt", moth1->Pt());

   UInt_t iddaughter=part.second->GetFirstDaughter();
   AliAODMCParticle* part2 = (AliAODMCParticle*) fMCContainer->GetMCParticleWithLabel(iddaughter);
   UInt_t daught=part2->GetPdgCode();
   fHistManager.FillTH1("fDaughterD0_pt", part2->Pt());

   Int_t icount = 0;

	 while (k==1) {
     if ( idmother < 0 ) { break; }
		 AliAODMCParticle* part1 = (AliAODMCParticle*) fMCContainer->GetMCParticleWithLabel(idmother);
     if (!part1) {break;}
     moth=part1->GetPdgCode();
     printf("The Mother Pdg = %i \n", moth);

     if (moth==-6||moth==-5||moth==-4||moth==-3||moth==-2||moth==-1||moth==0||moth==1||moth==2||moth==3||moth==4||moth==5||moth==6)
     { //printf("INSIDE THE LOOOOOOOOOOP1 \n ");
       if ( moth != 5 && moth != -5 ) { fHistManager.FillTH1("fD0PtCut2",part.second->Pt()); }
       if ( moth == 4 || moth == -4)
       { //Double_t pcharm = part1->P();
         Double_t pcharm= TMath::Sqrt(part1->Px()*part1->Px()+part1->Py()*part1->Py()+part1->Pz()*part1->Pz());
         //Double_t pD0 =part.second->P();
         Double_t pD0 = TMath::Sqrt(part.first.Px()*part.first.Px()+part.first.Py()*part.first.Py()+part.first.Pz()*part.first.Pz());
         Double_t zD0 = pD0 / pcharm ;
         printf(" The fragmentation function is zD0 =   %f for %i = %f and %i = %f \n",zD0,moth,pcharm,pdg,pD0);
         fHistManager.FillTH1("fMomentZ_D0",zD0);
       //fHistManager.FillTH1("fMomentZ_D0",TMath::Sqrt(part.first.Px()*part.first.Px()+part.first.Py()*part.first.Py()+part.first.Pz()*part.first.Pz())/TMath::Sqrt(part1->Px()*part1->Px()+part1->Py()*part1->Py()+part1->Pz()*part1->Pz()));
       fHistManager.FillTH1("fZ_D0",part.second->Pt()/part1->Pt());}
       break; }

     if ( moth == 2212 ) {break;}

		 idmother = part1->GetMother();
		 printf("D0 - The PDG mother = %i \n", moth);

		 //if ( idmother < 0 ) { break; }
     icount++;
     if(icount>10) {break;}
	 }

   printf("reached end of D0 mother loop");

   //Daughter
   UInt_t Ndaught = part.second->GetNDaughters();
// if we want all the N daughters.
   for (Int_t j = 0; j <= Ndaught; j++){

     UInt_t iddaught = part.second->GetDaughter(j);
     AliAODMCParticle* daught1 = (AliAODMCParticle*) fMCContainer->GetMCParticleWithLabel(iddaught);
     UInt_t Pdgdaught1= daught1->GetPdgCode();
    // printf("The daughter of generation %i is %i \n ",j,Pdgdaught1);

// if Kaon daughter is detected:
   if (Pdgdaught1==311||Pdgdaught1==313||Pdgdaught1==321||Pdgdaught1==323||Pdgdaught1==-311||Pdgdaught1==-313||Pdgdaught1==-321||Pdgdaught1==-323)
{
  double_t DEta = daught1->Eta();
  double_t DPhi = daught1->Phi();
  double_t DPt = daught1->Pt();
  double_t rD0 = TMath::Sqrt((DEta-part.second->Eta())*(DEta-part.second->Eta())+(DPhi-part.second->Phi())*(DPhi-part.second->Phi()));
  fHistManager.FillTH2("fRKaon_Pt",rD0,part.second->Pt());

  Double_t D0Pt = part.second->Pt();

    if (D0Pt<3 && D0Pt>0) { fHistManager.FillTH1("ProjKaon_R_Cut1",rD0); }
    if (D0Pt<7 && D0Pt>3) { fHistManager.FillTH1("ProjKaon_R_Cut2",rD0); }
    if (D0Pt<12 && D0Pt>7) { fHistManager.FillTH1("ProjKaon_R_Cut3",rD0); }
    if (D0Pt<12 && D0Pt>0) { fHistManager.FillTH1("ProjKaon_R_Cut0",rD0);}

}
// If pion detected
if (Pdgdaught1==211||Pdgdaught1==-211||Pdgdaught1==111)
{
  double_t DEta = daught1->Eta();
  double_t DPhi = daught1->Phi();
  double_t DPt = daught1->Pt();
  double_t rD0 = TMath::Sqrt((DEta-part.second->Eta())*(DEta-part.second->Eta())+(DPhi-part.second->Phi())*(DPhi-part.second->Phi()));
  fHistManager.FillTH2("fRPion_Pt",rD0,part.second->Pt());

  Double_t D0Pt = part.second->Pt();

    if (D0Pt<3 && D0Pt>0) { fHistManager.FillTH1("ProjPion_R_Cut1",rD0); }
    if (D0Pt<7 && D0Pt>3) { fHistManager.FillTH1("ProjPion_R_Cut2",rD0); }
    if (D0Pt<12 && D0Pt>7) { fHistManager.FillTH1("ProjPion_R_Cut3",rD0); }
    if (D0Pt<12 && D0Pt>0) { fHistManager.FillTH1("ProjPion_R_Cut0",rD0);}
}

   }


 } }


  //
  // Jet loop - loop over all reconstructed jets
  //

  std::vector<fastjet::PseudoJet> jets_incl = fFastJetWrapper->GetInclusiveJets();
  for (auto jet : jets_incl) {
    Double_t ptJet = TMath::Sqrt(jet.px()*jet.px() + jet.py()*jet.py());
    Printf("jet pt = %f",ptJet);

    if (jet.pseudorapidity()<0.5 && jet.pseudorapidity()>-0.5) { fHistManager.FillTH1("fHistPtJet",ptJet); }
    fHistManager.FillTH1("fHistEtaJet",jet.pseudorapidity());
    fHistManager.FillTH1("fHistPhiJet",jet.phi());

    Int_t nDmesonsInJet = 0;
    Int_t nConstsInJet = 0;
    Int_t nLcBaryonsInJet=0;

    for (auto constituent : jet.constituents()) {
      Int_t iPart = constituent.user_index() - 100;
      if (constituent.perp() < 1e-6) continue; // reject ghost particles
      AliAODMCParticle* part = fMCContainer->GetMCParticle(iPart);
      if (!part) {
        ::Error("AliAnalysisTaskDmesonJets::AnalysisEngine::RunParticleLevelAnalysis", "Could not find jet constituent %d!", iPart);
        continue;
      }
      nConstsInJet++;
      Printf("jet constituent - pdg = %i",part->PdgCode());
      Double_t ptConstJet = part->Pt();
      Double_t dEta = (part->Eta() - jet.eta());
      Double_t dPhi = (part->Phi() - jet.phi());
      Double_t dR = TMath::Sqrt(dEta*dEta + dPhi*dPhi);
      TVector3 pConstvec = TVector3(part->Px(),part->Py(),part->Pz());
      TVector3 pjetvec = TVector3(jet.px(),jet.py(),jet.pz());
      Double_t zConstJet = pjetvec.Dot(pConstvec) / pjetvec.Dot(pjetvec);
      fHistManager.FillTH1("fHistConstJet_DR", dR);
      fHistManager.FillTH1("fHistConstJet_Z", zConstJet);
      fHistManager.FillTH2("fHistConstJet_ptConst_ptJet", ptConstJet, ptJet);
      fHistManager.FillTH3("fHistConstJet_ptConst_ptJet_DR", ptConstJet, ptJet, dR);
      fHistManager.FillTH3("fHistConstJet_ptConst_ptJet_Z", ptConstJet, ptJet, zConstJet);
//The special PDG candidate . (With MinimumBias pythia )
      Int_t SpecialCandidate= TMath::Abs(part->PdgCode());

      if ( SpecialCandidate == 421) {
        printf("HIiiiiiiiiiiiiiiiiiiiiiiiiiiii 1 \n");
        nDmesonsInJet++;
        fHistManager.FillTH1("fHistDJet_DR", dR);
        fHistManager.FillTH1("fHistDJet_Z", zConstJet);
        fHistManager.FillTH2("fHistDJet_ptD_ptJet", ptConstJet, ptJet);
        fHistManager.FillTH3("fHistDJet_ptD_ptJet_DR", ptConstJet, ptJet, dR);
        fHistManager.FillTH3("fHistDJet_ptD_ptJet_Z", ptConstJet, ptJet, zConstJet);
       printf("HIiiiiiiiiiiiiiiiiiiiiiiiiiiii 2 \n");}
}
      //if (SpecialCandidate == 4122) {
      //  printf("HOoooooooooooooooooooooooooooo 1 \n ");
      //  nLcBaryonsInJet++;
      //  fHistManager.FillTH1("fHistLcJet_DR", dR);
      //  fHistManager.FillTH1("fHistLcJet_Z", zConstJet);
      //  fHistManager.FillTH2("fHistLcJet_ptLc_ptJet", ptConstJet, ptJet);
      //  fHistManager.FillTH3("fHistLcJet_ptLc_ptJet_DR", ptConstJet, ptJet, dR);
      //  fHistManager.FillTH3("fHistLcJet_ptLc_ptJet_Z", ptConstJet, ptJet, zConstJet);
      //  printf("HOoooooooooooooooooooooooooooo 2 \n ");
      //}
//}
    fHistManager.FillTH1("fHistNConstInJet", nConstsInJet);

    if(nDmesonsInJet>0) fHistManager.FillTH1("fHistNConstInDJet", nConstsInJet);
    if(nDmesonsInJet>0) fHistManager.FillTH1("fHistJetPt_D0",ptJet);

    if(nLcBaryonsInJet>0) fHistManager.FillTH1("fHistNConstInLcJet", nConstsInJet);
    if(nLcBaryonsInJet>0) fHistManager.FillTH1("fHistJetPt_Lc",ptJet);


  }



}



/// Fill the histograms.
///
/// \return Always kTRUE
Bool_t AliAnalysisTaskCharmHadronJets::FillHistograms()
{

  if (fMCContainer) FillPartonLevelHistograms();

  return kTRUE;
}


// Fill histograms with parton-level information
void AliAnalysisTaskCharmHadronJets::FillPartonLevelHistograms()
{
  auto itcont = fMCContainer->all_momentum();
  Int_t nHQ = 0;
  Double_t highestPartonPt = 0;
  Int_t PdgHighParton = 0;
  for (auto it = itcont.begin(); it != itcont.end(); it++) {
    auto part = *it;
    if (part.first.Pt() == 0) continue;

    Int_t PdgCode = part.second->GetPdgCode();

    // Skip all particles that are not either quarks or gluons
    if ((PdgCode < -9 || PdgCode > 9) && PdgCode != 21  && PdgCode != -21) continue;

    AliDebugStream(5) << "Parton " << it.current_index() <<
        " with pdg=" << PdgCode <<
        ", px=" << part.first.Px() <<
        ", py=" << part.first.Py() <<
        ", pz=" << part.first.Pz() <<
        ", n daughters = " << part.second->GetNDaughters() <<
        std::endl;

    // Skip partons that do not have any children
    // Unclear how this can happen, it would seem that this parton were not fragmented by the generator
    if (part.second->GetNDaughters() == 0) continue;

    // Look for highest momentum parton
    if (highestPartonPt < part.first.Pt()) {
      highestPartonPt = part.first.Pt();
      PdgHighParton = PdgCode;
    }

    // Skip partons that are not HF
    if (PdgCode != 4 && PdgCode != 5 && PdgCode != -4 && PdgCode != -5) continue;

    Bool_t lastInPartonShower = kTRUE;
    Bool_t hadronDaughter = kFALSE;
    for (Int_t daughterIndex = part.second->GetFirstDaughter(); daughterIndex <= part.second->GetLastDaughter(); daughterIndex++){
      if (daughterIndex < 0) {
        AliDebugStream(5) << "Could not find daughter index!" << std::endl;
        continue;
      }
      AliAODMCParticle *daughter = fMCContainer->GetMCParticle(daughterIndex);
      if (!daughter) {
        AliDebugStream(5) << "Could not find particle with index " << daughterIndex << "!" << std::endl;
        continue;
      }
      Int_t daughterPdgCode = daughter->GetPdgCode();
      if (daughter->GetMother() != it.current_index()) {
        AliDebugStream(5) << "Particle " << daughterIndex << " with pdg=" << daughterPdgCode <<
            ", px=" << daughter->Px() <<
            ", py=" << daughter->Py() <<
            ", pz=" << daughter->Pz() <<
            ", is not a daughter of " << it.current_index() <<
            "!" << std::endl;
        continue;
      }

      AliDebugStream(5) << "Found daughter " << daughterIndex <<
        " with pdg=" << daughterPdgCode <<
        ", px=" << daughter->Px() <<
        ", py=" << daughter->Py() <<
        ", pz=" << daughter->Pz() <<
        std::endl;
      // Codes between 81 and 100 are for internal MC code use, they may be intermediate states used e.g. in hadronizaion models
      if (daughterPdgCode == PdgCode) lastInPartonShower = kFALSE; // this parton is not the last parton in the shower
      if (TMath::Abs(daughterPdgCode) >= 111 || (daughterPdgCode >= 81 && daughterPdgCode <= 100)) hadronDaughter = kTRUE;
    }
    if (hadronDaughter) {
      AliDebugStream(5) << "This particle has at least a hadron among its daughters!" << std::endl;
      if (!lastInPartonShower) AliDebugStream(2) << "Odly, quark " << it.current_index() << " with PDG " << PdgCode << " (pt = " << part.first.Pt() << ", eta = " <<  part.first.Eta() << ") is not the last in the parton shower but at least a hadron found among its daughters?!" << std::endl;
    }
    else {
      AliDebugStream(5) << "This particle does not have hadrons among its daughters!" << std::endl;
      if (lastInPartonShower) AliDebugStream(2) << "Odly, quark " << it.current_index() << " with PDG " << PdgCode << " (pt = " << part.first.Pt() << ", eta = " <<  part.first.Eta() << ") is the last in the parton shower but no hadron found among its daughters?!" << std::endl;
      continue;
    }

    if (PdgCode == 4 || PdgCode == -4) {
      fHistManager.FillTH1("fHistCharmPt", part.first.Pt());
      fHistManager.FillTH1("fHistCharmEta", part.first.Eta());
//      fHistManager.FillTH1("fHistCharmPhi", part.first.Phi_0_2pi());
    }
    else if (PdgCode == 5 || PdgCode == -5) {
      fHistManager.FillTH1("fHistBottomPt", part.first.Pt());
      fHistManager.FillTH1("fHistBottomEta", part.first.Eta());
 //     fHistManager.FillTH1("fHistBottomPhi", part.first.Phi_0_2pi());
    }
    nHQ++;
  }
  fHistManager.FillTH1("fHistNHeavyQuarks", nHQ);
  fHistManager.FillTH1("fHistHighestPartonPt",highestPartonPt);
  Int_t partonType = 0;
  Int_t absPdgHighParton = TMath::Abs(PdgHighParton);
  if (absPdgHighParton == 9 || absPdgHighParton == 21) {
    partonType = 7;
  }
  else {
    partonType = absPdgHighParton;
  }
  fHistManager.FillTH1("fHistHighestPartonType",partonType);
}


/// Create an instance of this class and add it to the analysis manager
///
/// \param ntracks name of the track collection
/// \param nclusters name of the calorimeter cluster collection
/// \param nMCpart name of the MC particle collection
/// \param nMaxTrees number of output trees
/// \param suffix additional suffix that can be added at the end of the task name
/// \return pointer to the new AliAnalysisTaskCharmHadronJets task
AliAnalysisTaskCharmHadronJets* AliAnalysisTaskCharmHadronJets::AddTaskCharmHadronJets(TString nMCpart,
    AliJetContainer::EJetType_t jetType,
    AliJetContainer::EJetAlgo_t jetAlgo,
    AliJetContainer::ERecoScheme_t recoScheme,
    Double_t jetR,
    Int_t nMaxTrees,
    TString suffix)
{

  // Get the pointer to the existing analysis manager via the static access method
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    ::Error("AddTaskCharmHadronJets", "No analysis manager to connect to.");
    return NULL;
  }

  // Check the analysis type using the event handlers connected to the analysis manager
  AliVEventHandler* handler = mgr->GetInputEventHandler();
  if (!handler) {
    ::Error("AddTaskCharmHadronJets", "This task requires an input event handler");
    return NULL;
  }

  EDataType_t dataType = kUnknownDataType;

  if (handler->InheritsFrom("AliESDInputHandler")) {
    dataType = kESD;
  }
  else if (handler->InheritsFrom("AliAODInputHandler")) {
    dataType = kAOD;
  }

  // Init the task and do settings

  if (nMCpart == "usedefault") {
    nMCpart = "mcparticles"; // Always needs AliAODMCParticle objects
  }

  TString name("AliAnalysisTaskCharmHadronJets");
  if (strcmp(suffix, "") != 0) {
    name += TString::Format("_%s", suffix.Data());
  }

  AliAnalysisTaskCharmHadronJets* jetTask = new AliAnalysisTaskCharmHadronJets(name, nMaxTrees);

  if (!nMCpart.IsNull()) {
    AliMCParticleContainer* partCont = new AliHFAODMCParticleContainer(nMCpart);
    partCont->SetEtaLimits(-1.5, 1.5);
    partCont->SetPtLimits(0, 1000);
    jetTask->AdoptParticleContainer(partCont);

    TString nameJet = AliJetContainer::GenerateJetName(jetType, jetAlgo, recoScheme, jetR,0,0,"Jet");
    UInt_t acceptance = AliJetContainer::kTPCfid;
//    AliJetContainer* jetContBase = jetTask->AddJetContainer(jetType, jetAlgo, recoScheme, jetR, acceptance, partCont, 0x0 );
    AliJetContainer* jetContBase = new AliJetContainer(nameJet);
    //UInt_t accType, std::string partContName, std::string clusContName, TString tag="Jet")
    //jetContBase->SetRhoName(nRho);
    jetContBase->ConnectParticleContainer(partCont);
    jetTask->AdoptJetContainer(jetContBase);
  }

  jetTask->SetJetType(jetType);
  jetTask->SetJetAlgorithm(jetAlgo);
  jetTask->SetJetRecoScheme(recoScheme);
  jetTask->SetJetR(jetR);

  // Final settings, pass to manager and set the containers
  mgr->AddTask(jetTask);

  // Create containers for input/output
  AliAnalysisDataContainer* cinput1 = mgr->GetCommonInputContainer();
  TString contname1(name);
  contname1 += "_histos";
  AliAnalysisDataContainer* coutput1 = mgr->CreateContainer(contname1.Data(),
      TList::Class(), AliAnalysisManager::kOutputContainer,
      Form("%s", AliAnalysisManager::GetCommonFileName()));

  mgr->ConnectInput(jetTask, 0, cinput1);
  mgr->ConnectOutput(jetTask, 1, coutput1);

  for (Int_t i = 0; i < nMaxTrees; i++) {
    TString contname = TString::Format("%s_tree_%d", name.Data(), i);
    AliAnalysisDataContainer *coutput = mgr->CreateContainer(contname.Data(),
        TTree::Class(),AliAnalysisManager::kOutputContainer,
        Form("%s", AliAnalysisManager::GetCommonFileName()));
    mgr->ConnectOutput(jetTask, 2+i, coutput);
  }
  return jetTask;
}
