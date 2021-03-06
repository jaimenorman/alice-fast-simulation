//runJetSimulationGrid.C

#include <iostream>

#include <Rtypes.h>
#include <TSystem.h>
#include <TInterpreter.h>
#include <TROOT.h>
#include <TString.h>

#include "PythiaProcess_dev.h"
#include <AliLog.h>

#include "OnTheFlySimulationGenerator.h"

void runJetSimulation(TString name, Int_t pythiaEvents, TString procStr, TString gen, UInt_t seed, TString lhe, TString hep,
    TString beamType, Double_t ebeam1, Double_t ebeam2, Bool_t always_d_mesons, Bool_t extended_event_info, Double_t minPtHard = -1, Double_t maxPtHard = -1,
    UInt_t debug_level = 0)
{
  //gSystem->SetFPEMask(kInvalid | kDivByZero | kOverflow | kUnderflow);
  gSystem->SetFPEMask(kNoneMask);
  //gSystem->SetFPEMask(kDivByZero);

  TString trainName;
  if (!name.IsNull()) {
    trainName = Form("FastSim_%s", name.Data());
  }
  else {
    if (minPtHard >= 0 && maxPtHard >= 0) {
      trainName = Form("FastSim_%s_%s", gen.Data(), procStr.Data());
    }
    else {
      trainName = Form("FastSim_%s_%s_%.0f_%.0f", gen.Data(), procStr.Data(), minPtHard, maxPtHard);
    }
  }

  ProcessMy_t proc = kPyMbDefault;
  //ProcessMy_t proc = kPyCharmColorSoft;
  //ProcessMy_t proc = kPyCharmColorHard;

  OnTheFlySimulationGenerator::ESpecialParticle_t specialPart = OnTheFlySimulationGenerator::kNoSpecialParticle;
  OnTheFlySimulationGenerator::EGenerator_t partonEvent = OnTheFlySimulationGenerator::kPythia6;
  OnTheFlySimulationGenerator::EGenerator_t hadronization = OnTheFlySimulationGenerator::kPythia6;
  OnTheFlySimulationGenerator::EGenerator_t decayer = OnTheFlySimulationGenerator::kPythia6;

  // Parsing generator string into parton event, hadronizer, decayer
  TString partonEvent_str, hadronization_str, decayer_str;
  TObjArray* generators = gen.Tokenize("+");
  UInt_t n = generators->GetEntries();
  if (n == 1) {
    partonEvent_str = generators->At(0)->GetName();
    hadronization_str = generators->At(0)->GetName();
    decayer_str = generators->At(0)->GetName();
  }
  else if (n == 2) {
    partonEvent_str = generators->At(0)->GetName();
    decayer_str = generators->At(1)->GetName();
    if (decayer_str == "pythia6" || decayer_str == "pythia8") {
      hadronization_str = decayer_str;
    }
    else {
      hadronization_str = partonEvent_str;
    }
  }
  else if (n == 3) {
    partonEvent_str = generators->At(0)->GetName();
    hadronization_str = generators->At(1)->GetName();
    decayer_str = generators->At(2)->GetName();
  }

  AliInfoGeneralStream("") << "Parton event: " << partonEvent_str.Data() << std::endl;
  AliInfoGeneralStream("") << "Hadron event: " << hadronization_str.Data() << std::endl;
  AliInfoGeneralStream("") << "Decayer: " << decayer_str.Data() << std::endl;

  // Parton event generator selection
  if (partonEvent_str == "powheg") {
    partonEvent = OnTheFlySimulationGenerator::kPowheg;

    if (hadronization_str == "pythia6" || hadronization_str == "pythia8") {
      // POWHEG selection needs an LHE file (POWHEG runs standalone before the execution of this program)
      if (lhe.IsNull()) {
        AliErrorGeneralStream("") << "Must provide an LHE file if POWHEG is selected as event generator! Aborting." << std::endl;
        return;
      }

      // Select the PYTHIA process
      // Trigger on HF particle if required
      if (procStr == "dijet" || procStr == "dijet_lo") {
        proc = kPyJetsPWHG;
        specialPart = OnTheFlySimulationGenerator::kNoSpecialParticle;
      }
      else if (procStr == "charm" || procStr == "charm_lo") {
        proc = kPyCharmPWHG;
        specialPart = OnTheFlySimulationGenerator::kNoSpecialParticle;
      }
      else if (procStr == "beauty" || procStr == "beauty_lo") {
        proc = kPyBeautyPWHG;
        specialPart = OnTheFlySimulationGenerator::kNoSpecialParticle;
      }
      else {
        AliErrorGeneralStream("") << "You choose '" << procStr.Data() << "'. Not clear what process you want to simulate. Aborting." << std::endl;
        return;
      }
    }
    else if (hadronization_str == "herwig") {
      AliErrorGeneralStream("") << "POWHEG+HERWIG not implemented (yet). Aborting." << std::endl;
      return;
    }
    else {
      AliErrorGeneralStream("") << "Cannot couple POWHEG with '" << hadronization_str.Data() << "'. Aborting." << std::endl;
      return;
    }
  }
  else if (partonEvent_str == "pythia6") {
    partonEvent = OnTheFlySimulationGenerator::kPythia6;

    // No LHE file needed in this case
    if (!lhe.IsNull()) {
      AliWarningGeneralStream("") << "An LHE file was provided ("<< lhe.Data() << ") but PYTHIA6 was selected as generator. The LHE file will be ignored" << std::endl;
      lhe = "";
    }

    // Select the PYTHIA process
    // Trigger on HF particle if required
    if (procStr == "dijet") {
      proc = kPyJets;
      specialPart = OnTheFlySimulationGenerator::kNoSpecialParticle;
    }
    else if (procStr == "mb") {
      proc = kPyMbNonDiffr;
      specialPart = OnTheFlySimulationGenerator::kNoSpecialParticle;
    }
    else if (procStr == "charm_tot") {
      proc = kPyMbDefault;
      specialPart = OnTheFlySimulationGenerator::kccbar;
    }
    else if (procStr == "beauty_tot") {
      proc = kPyMbDefault;
      specialPart = OnTheFlySimulationGenerator::kbbbar;
    }
    else if (procStr == "charm") {
      proc = kPyMbNonDiffr;
      specialPart = OnTheFlySimulationGenerator::kccbar;
    }
    else if (procStr == "beauty") {
      proc = kPyMbNonDiffr;
      specialPart = OnTheFlySimulationGenerator::kbbbar;
    }
    else if (procStr == "charm_lo") {
      proc = kPyCharm;
      specialPart = OnTheFlySimulationGenerator::kccbar;
    }
    else if (procStr == "beauty_lo") {
      proc = kPyBeauty;
      specialPart = OnTheFlySimulationGenerator::kbbbar;
    }
    else {
      AliErrorGeneralStream("") << "You choose '" << procStr.Data() << "'. Not clear what process you want to simulate. Aborting." << std::endl;
      return;
    }
  }
  else if (partonEvent_str == "pythia8") {
    partonEvent = OnTheFlySimulationGenerator::kPythia8;

    // No LHE file needed in this case
    if (!lhe.IsNull()) {
      AliWarningGeneralStream("") << "An LHE file was provided ("<< lhe.Data() << ") but PYTHIA8 was selected as generator. The LHE file will be ignored" << std::endl;
      lhe = "";
    }

    // Select the PYTHIA process
    // Trigger on HF particle if required
    if (procStr == "dijet") {
      proc = kPyJets;
      specialPart = OnTheFlySimulationGenerator::kNoSpecialParticle;
    }
    else if (procStr == "mb") {
      proc = kPyMbNonDiffr;
      specialPart = OnTheFlySimulationGenerator::kNoSpecialParticle;
    }
    else if (procStr == "charm_tot") {
      proc = kPyMbDefault;
      specialPart = OnTheFlySimulationGenerator::kccbar;
    }
    else if (procStr == "beauty_tot") {
      proc = kPyMbDefault;
      specialPart = OnTheFlySimulationGenerator::kbbbar;
    }
    else if (procStr == "charm") {
      proc = kPyMbNonDiffr;
      specialPart = OnTheFlySimulationGenerator::kccbar;
    }
    else if(procStr == "charm_ColorHard" ) {
      proc = kPyCharmColorHard;
      specialPart = OnTheFlySimulationGenerator::kccbar;
    }
    else if(procStr == "charm_ColorSoft" ) {
      proc = kPyCharmColorSoft;
      specialPart = OnTheFlySimulationGenerator::kccbar;
    }
    else if (procStr == "beauty") {
      proc = kPyMbNonDiffr;
      specialPart = OnTheFlySimulationGenerator::kbbbar;
    }
    else if (procStr == "charm_lo") {
      proc = kPyCharm;
      specialPart = OnTheFlySimulationGenerator::kccbar;
    }
    else if (procStr == "beauty_lo") {
      proc = kPyBeauty;
      specialPart = OnTheFlySimulationGenerator::kbbbar;
    }
    else {
      AliErrorGeneralStream("") << "You choose '" << procStr.Data() << "'. Not clear what process you want to simualte. Aborting." << std::endl;
      return;
    }
  }
  else if (partonEvent_str == "herwig") {
    partonEvent = OnTheFlySimulationGenerator::kHerwig7;
  }
  else {
    AliErrorGeneralStream("") << "Parton event generator selection '" << partonEvent_str.Data() << "' is not valid. Aborting." << std::endl;
    return;
  }

  // Hadronization must be PYTHIA6, PYTHIA8 or HERWIG
  if (hadronization_str == "pythia6") {
    hadronization = OnTheFlySimulationGenerator::kPythia6;

    if (partonEvent != OnTheFlySimulationGenerator::kPythia6 && partonEvent != OnTheFlySimulationGenerator::kPowheg) {
      AliErrorGeneralStream("") << "Hadronization '" << hadronization_str.Data() << "' not compatible with parton event from '" << partonEvent_str.Data() << "'. Aborting." << std::endl;
      return;
    }
  }
  else if (hadronization_str == "pythia8") {
    hadronization = OnTheFlySimulationGenerator::kPythia8;

    if (partonEvent != OnTheFlySimulationGenerator::kPythia8 && partonEvent != OnTheFlySimulationGenerator::kPowheg) {
      AliErrorGeneralStream("") << "Hadronization '" << hadronization_str.Data() << "' not compatible with parton event from '" << partonEvent_str.Data() << "'. Aborting." << std::endl;
      return;
    }
  }
  else if (hadronization_str == "herwig") {
    hadronization = OnTheFlySimulationGenerator::kHerwig7;

    // HERWIG selection needs an LHE file (POWHEG runs standalone before the execution of this program)
    if (hep.IsNull()) {
      AliErrorGeneralStream("") << "Must provide an HepMC file if HERWIG is selected as event generator! Aborting." << std::endl;
      return;
    }

    if (partonEvent != OnTheFlySimulationGenerator::kHerwig7 && partonEvent != OnTheFlySimulationGenerator::kPowheg) {
      AliErrorGeneralStream("") << "Hadronization '" << hadronization_str.Data() << "' not compatible with parton event from '" << partonEvent_str.Data() << "'. Aborting." << std::endl;
      return;
    }
  }
  else {
    AliErrorGeneralStream("") << "Hadronization generator selection '" << hadronization_str.Data() << "' is not valid. Aborting." << std::endl;
    return;
  }

  // Decayer selection
  if (decayer_str == "pythia6") {
    decayer = OnTheFlySimulationGenerator::kPythia6;

    if (hadronization != OnTheFlySimulationGenerator::kPythia6) {
      AliErrorGeneralStream("") << "Decayer '" << decayer_str.Data() << "' not compatible with hadronization from '" << hadronization_str.Data() << "'. Aborting." << std::endl;
      return;
    }
  }
  else if (decayer_str == "pythia8") {
    decayer = OnTheFlySimulationGenerator::kPythia8;

    if (hadronization != OnTheFlySimulationGenerator::kPythia8) {
      AliErrorGeneralStream("") << "Decayer '" << decayer_str.Data() << "' not compatible with hadronization from '" << hadronization_str.Data() << "'. Aborting." << std::endl;
      return;
    }
  }
  else if (decayer_str == "evtgen") {
    decayer = OnTheFlySimulationGenerator::kEvtGen;

    if (hadronization == OnTheFlySimulationGenerator::kHerwig7) {
      AliErrorGeneralStream("") << "Decayer '" << decayer_str.Data() << "' not compatible with hadronization from '" << hadronization_str.Data() << "'. Aborting." << std::endl;
      return;
    }
  }
  else if (decayer_str == "herwig") {
    decayer = OnTheFlySimulationGenerator::kHerwig7;
  }
  else {
    AliErrorGeneralStream("") << "Decayer generator selection '" << decayer_str.Data() << "' is not valid. Aborting." << std::endl;
    return;
  }

  if (hadronization == OnTheFlySimulationGenerator::kPythia6 || hadronization == OnTheFlySimulationGenerator::kPythia8) {
    AliInfoGeneralStream("") << "Setting PYTHIA process to " << proc << " " << std::endl;
  }

  OnTheFlySimulationGenerator* sim = new OnTheFlySimulationGenerator(trainName);
  sim->EnableJetQA(kFALSE);
  sim->SetNumberOfEvents(pythiaEvents);
  sim->SetProcess(proc);
  sim->SetSpecialParticle(specialPart);
  sim->SetPartonEventGenerator(partonEvent);
  sim->SetHadronization(hadronization);
  sim->SetDecayer(decayer);
  sim->SetSeed(seed);
  sim->SetLHEFile(lhe);
  sim->SetHepMCFile(hep);
  sim->SetEnergyBeam1(ebeam1);
  sim->SetEnergyBeam2(ebeam2);
  sim->SetPtHardRange(minPtHard, maxPtHard);
  sim->SetExtendedEventInfo(extended_event_info);
//  if (procStr == "dijet" || procStr == "dijet_lo" || procStr == "mb" || procStr == "soft") {
//    sim->EnableJetTree();

// }
    if (always_d_mesons || procStr.Contains("charm") || procStr.Contains("beauty") || procStr == "mb") {

//  }
 // if (always_d_mesons || procStr.Contains("charm") || procStr.Contains("beauty")) {

    sim->EnableDMesonJets();
  }
  if (beamType == "pPb") {
    sim->SetBeamType(OnTheFlySimulationGenerator::kpPb);
  }
  else if (beamType == "pp") {
    sim->SetBeamType(OnTheFlySimulationGenerator::kpp);
  }
  else {
    AliErrorGeneralStream("") << "ERROR: Beam type " << beamType.Data() << " not recognized!! Not running..." << std::endl;
    return;
  }
  sim->Start(debug_level);
}
