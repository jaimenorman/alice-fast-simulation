#include "TFile.h"
#include "TList.h"
#include "TH1F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include <stdio.h>
#include <TH2.h>
#include <TStyle.h>
#include "Riostream.h"
#include <iostream>
#include <string>
#include <TH1.h>


using namespace std;

//R
void drawresults(TString fname = "")

{

  // get file
  TFile *f = new TFile(fname.Data());

  // get lists containing histograms
  TList	*l = (TList*)f->Get("AliAnalysisTaskCharmHadronJets_histos");
  THashList	*l2 = (THashList*)l->FindObject("histosAliAnalysisTaskCharmHadronJets");

  // get histograms

  TH1F	*fhLc_pt = (TH1F*)l2->FindObject("fLcPt");
  TH1F	*fhD0_pt = (TH1F*)l2->FindObject("fD0Pt");

  TH2F *fRPt = (TH2F*)l2->FindObject("fRKaon_Pt");
  TH1F *KaonC1=(TH1F*)l2->FindObject("ProjKaon_R_Cut1");
  TH1F *KaonC2=(TH1F*)l2->FindObject("ProjKaon_R_Cut2");
  TH1F *KaonC3=(TH1F*)l2->FindObject("ProjKaon_R_Cut3");

  TH1F *fRPt1=(TH1F*)l2->FindObject("fRPion_Pt");
  TH1F *PionC1=(TH1F*)l2->FindObject("ProjPion_R_Cut1");
  TH1F *PionC2=(TH1F*)l2->FindObject("ProjPion_R_Cut2");
  TH1F *PionC3=(TH1F*)l2->FindObject("ProjPion_R_Cut3");

  TH2F *LcfRPt = (TH2F*)l2->FindObject("LcfRKaon_Pt");
  TH1F *LcKaonC1=(TH1F*)l2->FindObject("LcProjKaon_R_Cut1");
  TH1F *LcKaonC2=(TH1F*)l2->FindObject("LcProjKaon_R_Cut2");
  TH1F *LcKaonC3=(TH1F*)l2->FindObject("LcProjKaon_R_Cut3");

  TH1F *LcfRPt1=(TH1F*)l2->FindObject("LcfRPion_Pt");
  TH1F *LcPionC1=(TH1F*)l2->FindObject("LcProjPion_R_Cut1");
  TH1F *LcPionC2=(TH1F*)l2->FindObject("LcProjPion_R_Cut2");
  TH1F *LcPionC3=(TH1F*)l2->FindObject("LcProjPion_R_Cut3");

  TH1F *zD0=(TH1F*)l2->FindObject("fZ_D0");
  TH1F *zLc=(TH1F*)l2->FindObject("fZ_Lc");


  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  TCanvas *c2 = new TCanvas("c2","c2",800,600);
  TCanvas *c3 = new TCanvas("c3","c3",800,600);
  TCanvas *c4 = new TCanvas("c4","c4",800,600);
  TCanvas *c5 = new TCanvas("c5","c5",800,600);

  c1->Divide(1,1);
  c2->Divide(1,1);
  c3->Divide(2,1);
  c4->Divide(2,1);
  c5->Divide(2,1);

  gStyle->SetOptStat(0);


auto legend = new TLegend(0.1,0.7,0.48,0.9);
auto legend1 = new TLegend(0.1,0.7,0.48,0.9);
auto legend2 = new TLegend(0.1,0.7,0.48,0.9);
auto legend3 = new TLegend(0.1,0.7,0.48,0.9);

//  c1->cd(1);
  //fMothLcPt->Divide(fDaughLcPt);
  //fMothLcPt->Draw();
  //c1->cd(2);
  //fMothD0Pt->Divide(fDaughD0Pt);
  //fMothD0Pt->Draw();
  TH1D* Proj_R_Cut1 = fRPt->ProjectionX("fRPt",0,1,"");

  c1->cd(1);
  Proj_R_Cut1->Draw();

  c2->cd(1);
  fhLc_pt->Divide(fhD0_pt);
  fhLc_pt->Draw();

  c3->cd(1);

  //fRPt->Sumw2();
  //fRPt->Draw();
  //fRPt->SetLineWidth(2);
  //fRPt->SetLineColor(6);

  //KaonC1->Sumw2();
  KaonC1->Draw("SAME");
  KaonC1->SetLineWidth(2);
  KaonC1->SetLineColor(4);
  KaonC1->Scale(1. / KaonC1->Integral("width"));

  double_t meankaonc1, sigkaonc1;
  meankaonc1=KaonC1->GetMean();
  sigkaonc1=KaonC1->GetRMS();

  //KaonC2->Sumw2();
  KaonC2->Draw("SAME");
  KaonC2->SetLineWidth(2);
  KaonC2->SetLineColor(6);
  KaonC2->Scale(1. / KaonC2->Integral("width"));

  double_t meankaonc2, sigkaonc2;
  meankaonc2=KaonC2->GetMean();
  sigkaonc2=KaonC2->GetRMS();

  //KaonC3->Sumw2();
  KaonC3->Draw("SAME");
  KaonC3->SetLineWidth(2);
  KaonC3->SetLineColor(30);
  KaonC3->Scale(1. / KaonC3->Integral("width"));

  double_t meankaonc3, sigkaonc3;
  meankaonc3=KaonC3->GetMean();
  sigkaonc3=KaonC3->GetRMS();

  legend->AddEntry(KaonC1,TString::Format("#splitline{0<Pt<3}{Mean = %f RMS = %f} ",meankaonc1,sigkaonc1),"l");
  legend->AddEntry(KaonC2,TString::Format("#splitline{3<Pt<7}{Mean = %f RMS = %f} ",meankaonc2,sigkaonc2),"l");
  legend->AddEntry(KaonC3,TString::Format("#splitline{7<Pt<12}{Mean = %f RMS = %f} ",meankaonc3,sigkaonc3),"l");
  legend->Draw();


  c3->cd(2);

  //fRPt1->Sumw2();
  //fRPt1->Draw();
  //fRPt1->SetLineWidth(2);
  //fRPt1->SetLineColor(40);

  //PionC1->Sumw2();
  PionC1->Draw("SAME");
  PionC1->SetLineWidth(2);
  PionC1->SetLineColor(4);
  PionC1->Scale(1. / PionC1->Integral("width"));

  double_t meanpionc1, sigpionc1;
  meanpionc1=PionC1->GetMean();
  sigpionc1=PionC1->GetRMS();

  //PionC2->Sumw2();
  PionC2->Draw("SAME");
  PionC2->SetLineWidth(2);
  PionC2->SetLineColor(6);
  PionC2->Scale(1. / PionC2->Integral("width"));

  double_t meanpionc2, sigpionc2;
  meanpionc2=PionC2->GetMean();
  sigpionc2=PionC2->GetRMS();

  //PionC3->Sumw2();
  PionC3->Draw("SAME");
  PionC3->SetLineWidth(2);
  PionC3->SetLineColor(30);
  PionC3->Scale(1. / PionC3->Integral("width"));

  double_t meanpionc3, sigpionc3;
  meanpionc3=PionC3->GetMean();
  sigpionc3=PionC3->GetRMS();

  legend1->AddEntry(PionC1,TString::Format("#splitline{0<Pt<3}{Mean = %f RMS = %f} ",meanpionc1,sigpionc1),"l");
  legend1->AddEntry(PionC2,TString::Format("#splitline{3<Pt<7}{Mean = %f RMS = %f} ",meanpionc2,sigpionc2),"l");
  legend1->AddEntry(PionC3,TString::Format("#splitline{7<Pt<12}{Mean = %f RMS = %f} ",meanpionc3,sigpionc3),"l");
  legend1->Draw();

//legend->AddEntry("f1","Function abs(#frac{sin(x)}{x})","l");
//  TList	*l2 = (TList*)AliAnalysisTaskCharmHadronJets_histos

c4->cd(1);
//LcKaonC1->Sumw2();
LcKaonC1->Draw();
LcKaonC1->SetLineWidth(2);
LcKaonC1->SetLineColor(4);
LcKaonC1->Scale(1. / LcKaonC1->Integral("width"));

double_t meanLckaonc1, sigLckaonc1;
meanLckaonc1=LcKaonC1->GetMean();
sigLckaonc1=LcKaonC1->GetRMS();

//LcKaonC2->Sumw2();
LcKaonC2->Draw("SAME");
LcKaonC2->SetLineWidth(2);
LcKaonC2->SetLineColor(6);
LcKaonC2->Scale(1. / LcKaonC2->Integral("width"));

double_t meanLckaonc2, sigLckaonc2;
meanLckaonc2=LcKaonC2->GetMean();
sigLckaonc2=LcKaonC2->GetRMS();

//LcKaonC3->Sumw2();
LcKaonC3->Draw("SAME");
LcKaonC3->SetLineWidth(2);
LcKaonC3->SetLineColor(30);
LcKaonC3->Scale(1. / LcKaonC3->Integral("width"));

double_t meanLckaonc3, sigLckaonc3;
meanLckaonc3=LcKaonC3->GetMean();
sigLckaonc3=LcKaonC3->GetRMS();

legend2->AddEntry(LcKaonC1,TString::Format("#splitline{0<Pt<3}{Mean = %f RMS = %f} ",meanLckaonc1,sigLckaonc1),"l");
legend2->AddEntry(LcKaonC2,TString::Format("#splitline{3<Pt<7}{Mean = %f RMS = %f} ",meanLckaonc2,sigLckaonc2),"l");
legend2->AddEntry(LcKaonC3,TString::Format("#splitline{7<Pt<12}{Mean = %f RMS = %f} ",meanLckaonc3,sigLckaonc3),"l");
legend2->Draw();


c4->cd(2);

//LcPionC1->Sumw2();
LcPionC1->Draw("SAME");
LcPionC1->SetLineWidth(2);
LcPionC1->SetLineColor(4);
LcPionC1->Scale(1. / LcPionC1->Integral("width"));
double_t meanLcpionc1, sigLcpionc1;
meanLcpionc1=LcPionC1->GetMean();
sigLcpionc1=LcPionC1->GetRMS();
//LcPionC2->Sumw2();
LcPionC2->Draw("SAME");
LcPionC2->SetLineWidth(2);
LcPionC2->SetLineColor(6);
LcPionC2->Scale(1. / LcPionC2->Integral("width"));
double_t meanLcpionc2, sigLcpionc2;
meanLcpionc2=LcPionC2->GetMean();
sigLcpionc2=LcPionC2->GetRMS();
//LcPionC3->Sumw2();
LcPionC3->Draw("SAME");
LcPionC3->SetLineWidth(2);
LcPionC3->SetLineColor(30);
LcPionC3->Scale(1. / LcPionC3->Integral("width"));
double_t meanLcpionc3, sigLcpionc3;
meanLcpionc3=LcPionC3->GetMean();
sigLcpionc3=LcPionC3->GetRMS();

legend3->AddEntry(LcPionC1,TString::Format("#splitline{0<Pt<3}{Mean = %f RMS = %f} ",meanLcpionc1,sigLcpionc1),"l");
legend3->AddEntry(LcPionC2,TString::Format("#splitline{3<Pt<7}{Mean = %f RMS = %f} ",meanLcpionc2,sigLcpionc2),"l");
legend3->AddEntry(LcPionC3,TString::Format("#splitline{7<Pt<12}{Mean = %f RMS = %f} ",meanLcpionc3,sigLcpionc3),"l");
legend3->Draw();


c5->cd(1);
zD0->Draw();
zD0->Scale(1. / zD0->Integral("width"));

c5->cd(2);
zLc->Draw();
zLc->Scale(1./ zLc->Integral("width"));

}

void DrawResults1(TString fname1 = "",TString fname2 = "") {
  // get file
  TFile *f1 = new TFile(fname1.Data());
  TFile *f2 = new TFile(fname2.Data());
  // get lists containing histograms
  TList	*l1 = (TList*)f1->Get("AliAnalysisTaskCharmHadronJets_histos");
  THashList	*l2_1 = (THashList*)l1->FindObject("histosAliAnalysisTaskCharmHadronJets");

  TList	*l2 = (TList*)f2->Get("AliAnalysisTaskCharmHadronJets_histos");
  THashList	*l2_2 = (THashList*)l2->FindObject("histosAliAnalysisTaskCharmHadronJets");
  // get histograms
  TH1F	*fhLc_pt1 = (TH1F*)l2_1->FindObject("fLcPt");
  TH1F	*fhD0_pt1 = (TH1F*)l2_1->FindObject("fD0Pt");

  TH1F	*fhLc_pt2 = (TH1F*)l2_2->FindObject("fLcPt");
  TH1F	*fhD0_pt2 = (TH1F*)l2_2->FindObject("fD0Pt");

  TFile *f3 = new TFile("lcd0_pythia8_all.root");
  TH1F *lcd0 = (TH1F*)f3->Get("h_lcd0_monash");

  TCanvas *c2 = new TCanvas("c2","c2",800,600);
  c2->Divide(1,1);
  gStyle->SetOptStat(0);
  auto legend = new TLegend(0.1,0.7,0.48,0.9);

  c2->cd(1);
  fhLc_pt1->Divide(fhD0_pt1);
  fhLc_pt1->Draw("C");
  //fhLc_pt1->Sumw2();
  fhLc_pt2->Divide(fhD0_pt2);
  fhLc_pt2->Draw("SAME,C");
  //fhLc_pt2->Sumw2();
  fhLc_pt2->SetLineColor(6);
  lcd0->Draw("SAME");

  legend->AddEntry(fhLc_pt1," Pythia6 ","l");
  legend->AddEntry(fhLc_pt2," Pythia8 ","l");
  legend->AddEntry(lcd0," Ref: Pythia8 ","l");
  legend->Draw();

}

double_t FindingK (double_t k,TString fname = "")
{
   TFile *f = new TFile(fname.Data());
   TList	*l1 = (TList*)f->Get("AliAnalysisTaskCharmHadronJets_histos");

   TH1F *Nevts=(TH1F*)l1->FindObject("fHistEventCount");
   TH1F *CrosSectMB=(TH1F*)l1->FindObject("fHistXsection");

double_t CrosSecMB= CrosSectMB->GetMean(2);
double_t nbins = Nevts->GetNbinsX();
double_t Nevt= Nevts->GetBinContent(nbins/2);

printf("The value of CS_MB is %f and the number of evts is %f \n",CrosSecMB,Nevt);
   k=CrosSecMB/Nevt;

  return k;
}

//inclusive jet cross section
void CrossSection1(TString fname1 = "", TString fname2 = "") {

  // get file
  TFile *f1 = new TFile(fname1.Data());
  TFile *f2 = new TFile(fname2.Data());
  // get lists containing histograms
  TList	*l1 = (TList*)f1->Get("AliAnalysisTaskCharmHadronJets_histos");
  THashList	*l2_1 = (THashList*)l1->FindObject("histosAliAnalysisTaskCharmHadronJets");

  TList	*l2 = (TList*)f2->Get("AliAnalysisTaskCharmHadronJets_histos");
  THashList	*l2_2 = (THashList*)l2->FindObject("histosAliAnalysisTaskCharmHadronJets");
  // get histograms

  TH1F	*fJetPt1 = (TH1F*)l2_1->FindObject("fHistPtJet");
  TH1F	*fJetPt2 = (TH1F*)l2_2->FindObject("fHistPtJet");

  TFile *f3 = new TFile("HEPData-ins1693308-v1-Table_1.root");
  TGraphAsymmErrors	*fRefCrossSection = (TGraphAsymmErrors*)f3->Get("Table 1/Graph1D_y1");


  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  c1->Divide(1,1);
  gStyle->SetOptStat(0);
  auto legend = new TLegend(0.1,0.7,0.48,0.9);

  TCanvas *c5 = new TCanvas("c5","c5",800,600);
  c5->Divide(1,1);

  double_t k1;
  double_t k2;

  k1 = FindingK(k1,fname1);
  k2 = FindingK(k2,fname2);

  fJetPt1->Scale(k1);
  fJetPt2->Scale(k2);
//1/2

  TH1F *fJetPt1new = (TH1F*) fJetPt1->Clone();
  TH1F *fJetPt2new = (TH1F*) fJetPt2->Clone();

  for (int i = 1; i<=fJetPt1->GetXaxis()->GetNbins(); i++) {
     double_t y1 = fJetPt1->GetBinWidth(i);
     double_t y2= fJetPt1->GetBinContent(i);
     fJetPt1new->SetBinContent(i, y2/(y1));
   }

  for (int i = 1; i<=fJetPt2->GetXaxis()->GetNbins(); i++) {
     double_t y1 = fJetPt2->GetBinWidth(i);
     double_t y2= fJetPt2->GetBinContent(i);
     fJetPt2new->SetBinContent(i, y2/(y1));
     printf("Number of bin is: %i bin width is %f bin content is %f \n",i,y1,y2);
   }

  c1->cd(1);

  fJetPt1new->Draw("");
  fJetPt2new->Draw("SAME");
  fRefCrossSection->Draw("SAME");
  fJetPt2new->SetLineColor(6);
  //fJetPt1new->Scale(1. / fJetPt1new->Integral("width"));
  //fJetPt2new->Scale(1. / fJetPt2new->Integral("width"));

  legend->AddEntry(fJetPt1new," Pythia6 ","l");
  legend->AddEntry(fJetPt2new," Pythia8 ","l");
  legend->Draw();

//TH1F *fRefCrossSection1 = (TH1F*) fRefCrossSection->Clone();
//fRefCrossSection1->Reset();
//fRefCrossSection1->SetBins(95,5.00,100.00);
//double_t y = fRefCrossSection1->GetXaxis()->GetNbins();
//printf("Number of bins is %f \n",y);

Int_t n = fRefCrossSection->GetN();
double_t x1[n],yy1[n];

x1[n] = {0};
yy1[n] = {0};

for (int i = 1; i<=fRefCrossSection->GetN(); i++) {
  double_t x, y;
  fRefCrossSection->GetPoint(i,x,y);
  Double_t histx = fJetPt2new->GetXaxis()->FindBin(x);
  double_t y1 = fJetPt2new->GetBinContent(histx);
  if (y1==0) continue;
  x1[i]=x;
  yy1[i]=y1/y;
  //fRatioCS->SetPoint(i,x,y/y1);
}

for (Int_t i =1 ; i<=n; i++)
{printf("x1= %f and y1 = %f ",x1[i],yy1[i]);}

TGraph* gr = new TGraph(n,x1,yy1);

//TH1F *gr1 = new TH1F("gr1","MC/data ",n-1,x1);
//for (int i = 1; i<=fRefCrossSection->GetN(); i++) { gr1->Fill(yy1[i]);}

  c5->cd(1);
  gr->Draw("A*");
  //gr->Draw("AC*");
}


//D0 meson (Lc Baryon) cross section
void CrossSection2(TString fname1 = "", TString fname2 = "") {
//cross section of d0
  // get file
  TFile *f1 = new TFile(fname1.Data());
  TFile *f2 = new TFile(fname2.Data());
  // get lists containing histograms
  TList	*l1 = (TList*)f1->Get("AliAnalysisTaskCharmHadronJets_histos");
  THashList	*l2_1 = (THashList*)l1->FindObject("histosAliAnalysisTaskCharmHadronJets");

  TList	*l2 = (TList*)f2->Get("AliAnalysisTaskCharmHadronJets_histos");
  THashList	*l2_2 = (THashList*)l2->FindObject("histosAliAnalysisTaskCharmHadronJets");
  // get histograms

  TH1F	*fJetPt1 = (TH1F*)l2_1->FindObject("fHistJetPt_D0");
  TH1F	*fJetPt2 = (TH1F*)l2_2->FindObject("fHistJetPt_D0");

  //TH1F	*fLcJetPt1 = (TH1F*)l2_1->FindObject("fHistJetPt_Lc");
  //TH1F	*fLcJetPt2 = (TH1F*)l2_2->FindObject("fHistJetPt_Lc");


  TCanvas *c3 = new TCanvas("c3","c3",800,600);
  c3->Divide(1,1);

//  TCanvas *c4 = new TCanvas("c4","c4",800,600);
  //c4->Divide(1,1);

  gStyle->SetOptStat(0);
  auto legend = new TLegend(0.1,0.7,0.48,0.9);

  double_t k1;
  double_t k2;

  k1 = FindingK(k1,fname1);
  k2 = FindingK(k2,fname2);

  fJetPt1->Scale(k1/3);
  fJetPt2->Scale(k2/3);

  //fLcJetPt1->Scale(k1);
  //fLcJetPt2->Scale(k2);

  TH1F *fJetPt1new = (TH1F*) fJetPt1->Clone();
  TH1F *fJetPt2new = (TH1F*) fJetPt2->Clone();

  for (int i = 1; i<=fJetPt1->GetXaxis()->GetNbins(); i++) {
     double_t y1 = fJetPt1->GetBinWidth(i);
     double_t y2= fJetPt1->GetBinContent(i);
     fJetPt1new->SetBinContent(i, y2/y1); }

  for (int i = 1; i<=fJetPt2->GetXaxis()->GetNbins(); i++) {
     double_t y1 = fJetPt2->GetBinWidth(i);
     double_t y2= fJetPt2->GetBinContent(i);
     fJetPt2new->SetBinContent(i, y2/y1); }


    // TH1F *fLcJetPt1new = (TH1F*) fLcJetPt1->Clone();
    // TH1F *fLcJetPt2new = (TH1F*) fLcJetPt2->Clone();

    // for (int i = 1; i<=fLcJetPt1->GetXaxis()->GetNbins(); i++) {
    //    double_t y1 = fLcJetPt1->GetBinWidth(i);
    //    double_t y2= fLcJetPt1->GetBinContent(i);
    //    fLcJetPt1new->SetBinContent(i, y2/y1); }
    // for (int i = 1; i<=fLcJetPt2->GetXaxis()->GetNbins(); i++) {
    //    double_t y1 = fLcJetPt2->GetBinWidth(i);
    //    double_t y2= fLcJetPt2->GetBinContent(i);
    //    fLcJetPt2new->SetBinContent(i, y2/y1); }


  c3->cd(1);
  fJetPt1new->Draw("");
  fJetPt2new->Draw("SAME");
  fJetPt2new->SetLineColor(6);
  //fJetPt1new->Scale(1. / fJetPt1new->Integral("width")); //normalise
  //fJetPt2new->Scale(1. / fJetPt2new->Integral("width"));
  legend->AddEntry(fJetPt1new," Pythia6 ","l");
  legend->AddEntry(fJetPt2new," Pythia8 ","l");
  legend->Draw();

//  c4->cd(1);
//  fLcJetPt1new->Draw("");
//  fLcJetPt2new->Draw("SAME");
//  fLcJetPt2new->SetLineColor(6);
//  fLcJetPt1new->Scale(1. / fLcJetPt1new->Integral("width")); //normalise
//  fLcJetPt2new->Scale(1. / fLcJetPt2new->Integral("width"));

//  legend->Draw();


}

void DrawResults() {

  TString fname1, fname2;

  cout << " fname1: ";
  cin >> fname1;
  cout << " fname2: ";
  cin >> fname2;

  DrawResults1(fname1,fname2);
  CrossSection1(fname1,fname2);
  CrossSection2(fname1,fname2);
}
