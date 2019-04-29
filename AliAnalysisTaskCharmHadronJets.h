/// \class AliAnalysisTaskCharmHadronJets
/// \brief Analysis task for generated charmed hadrons in jets
///
/// This task runs the charmed hadron in jets for use with 
/// On-the-fly simulation.
///
/// adapted from AliAnalysisTaskDmesonJets by Salvatore Aiola

#ifndef ALIANALYSISTASKCHARMHADRONJETS_H
#define ALIANALYSISTASKCHARMHADRONJETS_H

/**************************************************************************
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

class TClonesArray;
class AliRDHFCuts;
class AliAODEvent;
class AliAODRecoDecay;
class AliAODRecoDecayHF2Prong;
class AliAODRecoCascadeHF;
class AliVParticle;
class AliAODMCParticle;
class AliHFAODMCParticleContainer;
class AliHFTrackContainer;
class AliParticleContainer;
class AliClusterContainer;
class THnSparse;
class AliFJWrapper;
class THashList;
class TTree;
class AliEMCALGeometry;
class TRandom;
class AliRhoParameter;
class AliAnalysisTaskSE;

//C++
#include <exception>
#include <list>
#include <vector>
#include <map>

#include <AliTLorentzVector.h>
#include <THistManager.h>

#include <AliAnalysisTaskEmcalJetLight.h>
#include <AliJetContainer.h>

class AliAnalysisTaskCharmHadronJets : public AliAnalysisTaskEmcalJetLight
{
 public:

  typedef AliJetContainer::EJetType_t EJetType_t;
  typedef AliJetContainer::EJetAlgo_t EJetAlgo_t;
  typedef AliJetContainer::ERecoScheme_t ERecoScheme_t;

  enum EOutputType_t { kNoOutput, kTreeOutput, kTHnOutput, kOnlyQAOutput, kTreeExtendedOutput };
  enum EMCMode_t { kNoMC, kSignalOnly, kBackgroundOnly, kMCTruth, kD0Reflection, kOnlyWrongPIDAccepted };
  enum EMesonOrigin_t {
    kUnknownQuark = BIT(0),
    kFromDown     = BIT(1),
    kFromUp       = BIT(2),
    kFromStrange  = BIT(3),
    kFromCharm    = BIT(4),
    kFromBottom   = BIT(5),
    kFromTop      = BIT(6),
    kFromGluon    = BIT(7),
    kAnyOrigin    = kUnknownQuark | kFromDown | kFromUp | kFromStrange | kFromCharm | kFromBottom | kFromTop | kFromGluon
  };

  AliAnalysisTaskCharmHadronJets();
  AliAnalysisTaskCharmHadronJets(const char* name, Int_t nOutputTrees=2);
  virtual ~AliAnalysisTaskCharmHadronJets();

  void SetApplyKinematicCuts(Bool_t b)            { fApplyKinematicCuts = b ; }
//  void SetOutputType(EOutputType_t b)             { SetOutputTypeInternal(b); }
  void SetRejectISR(Bool_t b)                     { fRejectISR          = b ; }
  void SetJetArea(Int_t type,
      Double_t garea = 0.005)                     { fJetAreaType        = type; fJetGhostArea = garea; }

  void SetJetType(EJetType_t jetType) { fJetType = jetType;}
  void SetJetAlgorithm(EJetAlgo_t jetAlgo) { fJetAlgo = jetAlgo;}
  void SetJetRecoScheme(ERecoScheme_t recoScheme) { fJetRecoScheme = recoScheme;}
  void SetJetR(Double_t jetR ) { fJetR = jetR;} 

  void SetCandidatePDG(UInt_t pdg) {fCandidatePDG = pdg;}
  void SetRejectedOrigin(UInt_t origin) {fRejectedOrigin = origin;}
  void SetAcceptedDecay(UInt_t decay) {fAcceptedDecay = decay;}

  virtual void         UserCreateOutputObjects();
  virtual void         ExecOnce();
  virtual Bool_t       Run();
  virtual Bool_t       FillHistograms();

  void                AddInputVectors(AliEmcalContainer* cont, Int_t offset, TH2* rejectHist=0, Double_t eff=0.);

  void                FillPartonLevelHistograms();
  void                RunParticleLevelAnalysis();

  static AliAnalysisTaskCharmHadronJets* AddTaskCharmHadronJets(
      TString nMCpart = "usedefault", 
      AliJetContainer::EJetType_t jetType = AliJetContainer::kChargedJet,
      AliJetContainer::EJetAlgo_t jetAlgo = AliJetContainer::antikt_algorithm,
      AliJetContainer::ERecoScheme_t recoScheme = AliJetContainer::pt_scheme,
      Double_t jetR = 0.4,
      Int_t nMaxTrees = 2, 
      TString suffix = "");

//  virtual void SetOutputTypeInternal(EOutputType_t b)             { fOutputType         = b; }

 protected:


  EOutputType_t        fOutputType                ; ///<  Output type: none, TTree or THnSparse
  THistManager         fHistManager               ; ///<  Histogram manager
  Bool_t               fApplyKinematicCuts        ; ///<  Apply jet kinematic cuts
  Int_t                fNOutputTrees              ; ///<  Maximum number of output trees
  Bool_t               fRejectISR                 ; ///<  Reject initial state radiation
  Int_t                fJetAreaType               ; ///<  Jet area type
  Double_t             fJetGhostArea              ; ///<  Area of the ghost particles
  EJetType_t           fJetType ; ///<  jet type
  EJetAlgo_t             fJetAlgo ; ///< jet algorithm            
  ERecoScheme_t        fJetRecoScheme ; ///< jet recombination scheme
  Double_t             fJetR ; ///< jet radius
  AliHFAODMCParticleContainer* fMCContainer       ; //!<! MC particle container
  AliAODEvent         *fAodEvent                  ; //!<! AOD event
  AliFJWrapper        *fFastJetWrapper            ; //!<! Fastjet wrapper

  UInt_t                             fCandidatePDG          ; ///<  Candidate PDG
  UInt_t                             fRejectedOrigin        ; ///<  Bit mask with D meson origins that are rejected (used for MC analysis, i.e. signal-only, background-only and particle-level)
  UInt_t                             fAcceptedDecay         ; ///<  Bit mask with D meson decays that are accepted (only used for particle-level analysis)


 private:



  AliAnalysisTaskCharmHadronJets(const AliAnalysisTaskCharmHadronJets&);
  AliAnalysisTaskCharmHadronJets& operator=(const AliAnalysisTaskCharmHadronJets&);

  /// \cond CLASSIMP
  ClassDef(AliAnalysisTaskCharmHadronJets, 1);
  /// \endcond
};

#endif
