void DrawResults(TString fname = "") {

  // get file
  TFile *f = new TFile(fname.Data());

  // get lists containing histograms
  TList	*l = (TList*)f->Get("AliAnalysisTaskCharmHadronJets_histos");
  THashList	*l2 = (THashList*)l->FindObject("histosAliAnalysisTaskCharmHadronJets");

  // get histograms
  TH1D	*fhLc_pt = (TH1D*)l2->FindObject("fhLc_pt");
  TH1D	*fhD0_pt = (TH1D*)l2->FindObject("fhD0_pt");
  fhLc_pt->Divide(fhD0_pt);
  fhLc_pt->Draw();
//  TList	*l2 = (TList*)AliAnalysisTaskCharmHadronJets_histos
}
