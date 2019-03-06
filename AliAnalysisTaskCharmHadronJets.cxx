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

  }

  //
  // Jet loop - loop over all reconstructed jets
  //
  std::vector<fastjet::PseudoJet> jets_incl = fFastJetWrapper->GetInclusiveJets();
  for (auto jet : jets_incl) {
    Double_t ptJet = TMath::Sqrt(jet.px()*jet.px() + jet.py()*jet.py());
    Printf("jet pt = %f",ptJet);
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
