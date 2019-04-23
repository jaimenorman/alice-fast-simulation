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


void SetStyle();
void SetStyleHisto(TH1 *h);

//R
void drawresults(TString fname = "")

{

  SetStyle();

  // get file
  TFile *f = new TFile(fname.Data());

  // get lists containing histograms
  TList	*l = (TList*)f->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
  THashList	*l2 = (THashList*)l->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");

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


  TCanvas *c1 = new TCanvas("c1","c1",1000,600);
  TCanvas *c2 = new TCanvas("c2","c2",1000,600);
  TCanvas *c3 = new TCanvas("c3","c3",1000,600);
  TCanvas *c4 = new TCanvas("c4","c4",1000,600);
  TCanvas *c5 = new TCanvas("c5","c5",1000,600);

  c1->Divide(1,1);
  c2->Divide(1,1);
  c3->Divide(2,1);
  c4->Divide(2,1);
  c5->Divide(2,1);

  gStyle->SetOptStat(0);


auto legend = new TLegend(0.35,0.58,0.85,0.83);
auto legend1 = new TLegend(0.35,0.65,0.85,0.9);
auto legend2 = new TLegend(0.35,0.65,0.85,0.9);
auto legend3 = new TLegend(0.35,0.65,0.85,0.9);

//  c1->cd(1);
  //fMothLcPt->Divide(fDaughLcPt);
  //fMothLcPt->Draw();
  //c1->cd(2);
  //fMothD0Pt->Divide(fDaughD0Pt);
  //fMothD0Pt->Draw();
  TH1D* Proj_R_Cut1 = fRPt->ProjectionX("fRPt",0,1,"");

  c1->cd(1);
  SetStyleHisto(Proj_R_Cut1);
  Proj_R_Cut1->Draw();

  c2->cd(1);
  SetStyleHisto(fhLc_pt);
  fhLc_pt->Divide(fhD0_pt);
  fhLc_pt->Draw();

  c3->cd(1);

  //fRPt->Sumw2();
  //fRPt->Draw();
  //fRPt->SetLineWidth(2);
  //fRPt->SetLineColor(6);

  //KaonC1->Sumw2();
  SetStyleHisto(KaonC1);
  KaonC1->GetXaxis()->SetTitle("R between D^{0} and Kaon daughter");
  KaonC1->SetLineWidth(2);
  KaonC1->SetLineColor(4);
  KaonC1->Scale(1. / KaonC1->Integral("width"));
  KaonC1->GetYaxis()->SetRangeUser(0 , 6.8);
  KaonC1->Draw("L");

  double_t meankaonc1, sigkaonc1;
  meankaonc1=KaonC1->GetMean();
  sigkaonc1=KaonC1->GetRMS();

  //KaonC2->Sumw2();
  KaonC2->Draw("SAMEL");
  KaonC2->SetLineWidth(2);
  KaonC2->SetLineColor(6);
  KaonC2->Scale(1. / KaonC2->Integral("width"));

  double_t meankaonc2, sigkaonc2;
  meankaonc2=KaonC2->GetMean();
  sigkaonc2=KaonC2->GetRMS();

  //KaonC3->Sumw2();
  KaonC3->Draw("SAMEL");
  KaonC3->SetLineWidth(2);
  KaonC3->SetLineColor(30);
  KaonC3->Scale(1. / KaonC3->Integral("width"));

  double_t meankaonc3, sigkaonc3;
  meankaonc3=KaonC3->GetMean();
  sigkaonc3=KaonC3->GetRMS();

  legend->AddEntry(KaonC1,TString::Format("#splitline{0 < p_{T} < 3 GeV/c}{Mean = %.2f RMS = %.2f} ",meankaonc1,sigkaonc1),"l");
  legend->AddEntry(KaonC2,TString::Format("#splitline{3 < p_{T} < 7 GeV/c}{Mean = %.2f RMS = %.2f} ",meankaonc2,sigkaonc2),"l");
  legend->AddEntry(KaonC3,TString::Format("#splitline{7 < p_{T} < 12 GeV/c}{Mean = %.2f RMS = %.2f} ",meankaonc3,sigkaonc3),"l");
  legend->Draw();

  TLatex info; info.SetNDC(); info.SetTextFont(43); info.SetTextSize(18);
  info.DrawLatex(0.40, 0.86, "PYTHIA8, |#eta_{D0}| < 1");

  c3->cd(2);

  //fRPt1->Sumw2();
  //fRPt1->Draw();
  //fRPt1->SetLineWidth(2);
  //fRPt1->SetLineColor(40);

  //PionC1->Sumw2();
  SetStyleHisto(PionC1);
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

  legend1->AddEntry(PionC1,TString::Format("#splitline{0 < p_{T} < 3 GeV/c}{Mean = %.2f RMS = %.2f} ",meanpionc1,sigpionc1),"l");
  legend1->AddEntry(PionC2,TString::Format("#splitline{3 < p_{T} < 7 GeV/c}{Mean = %.2f RMS = %.2f} ",meanpionc2,sigpionc2),"l");
  legend1->AddEntry(PionC3,TString::Format("#splitline{7 < p_{T} < 12 GeV/c}{Mean = %.2f RMS = %.2f} ",meanpionc3,sigpionc3),"l");
  legend1->Draw();

  c3->SaveAs("D0DaughterAngles.eps");

//legend->AddEntry("f1","Function abs(#frac{sin(x)}{x})","l");
//  TList	*l2 = (TList*)AliAnalysisTaskCharmHadronJets_histos

c4->cd(1);
//LcKaonC1->Sumw2();
  SetStyleHisto(LcKaonC1);
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

  legend2->AddEntry(LcKaonC1,TString::Format("#splitline{0 < p_{T} < 3 GeV/c}{Mean = %.2f RMS = %.2f} ",meanLckaonc1,sigLckaonc1),"l");
  legend2->AddEntry(LcKaonC2,TString::Format("#splitline{3 < p_{T} < 7 GeV/c}{Mean = %.2f RMS = %.2f} ",meanLckaonc2,sigLckaonc2),"l");
  legend2->AddEntry(LcKaonC3,TString::Format("#splitline{7 < p_{T} < 12 GeV/c}{Mean = %.2f RMS = %.2f} ",meanLckaonc3,sigLckaonc3),"l");
  legend2->Draw();

c4->cd(2);

//LcPionC1->Sumw2();
SetStyleHisto(LcPionC1);
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
SetStyleHisto(zD0);
zD0->Draw();
zD0->Scale(1. / zD0->Integral("width"));

c5->cd(2);
SetStyleHisto(zLc);
zLc->Draw();
zLc->Scale(1./ zLc->Integral("width"));

}

void DrawResults1(TString fname1 = "",TString fname2 = "") {
  SetStyle();
  // get file
  //TFile *f1 = new TFile("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia6_charm_1553872828/stage_1/output/001/AnalysisResults_FastSim_pythia6_charm.root");

  TFile *f2 = new TFile("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia8_charm_1554121250/stage_1/output/001/AnalysisResults_FastSim_pythia8_charm.root");
  TFile *f1 = new TFile("/Users/sadek/Analysis/alice-fast-simulation/AnalysisResults_FastSim_pythia8_charm_ColorHard_1554883740.root");

  //TFile *f2 = new TFile("/Users/sadek/Analysis/alice-fast-simulation/AnalysisResults_FastSim_pythia8_charm_ColorHard_1554409872.root");
  //TFile *f1 = new TFile(fname1.Data());
  //TFile *f2 = new TFile(fname2.Data());

  // get lists containing histograms
  TList	*l1 = (TList*)f1->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
  THashList	*l1_1 = (THashList*)l1->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");

  TList	*l2 = (TList*)f1->Get("AliAnalysisTaskCharmHadronJets_Lc_histos");
  THashList	*l2_1 = (THashList*)l2->FindObject("histosAliAnalysisTaskCharmHadronJets_Lc");

  TList	*l3 = (TList*)f2->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
  THashList	*l3_1 = (THashList*)l3->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");

  TList	*l4 = (TList*)f2->Get("AliAnalysisTaskCharmHadronJets_Lc_histos");
  THashList	*l4_1 = (THashList*)l4->FindObject("histosAliAnalysisTaskCharmHadronJets_Lc");


  // get histograms
  //TH1F	*fhLc_pt1 = (TH1F*)l2_1->FindObject("fLcPt");
  //TH1F	*fhD0_pt1 = (TH1F*)l2_1->FindObject("fD0Pt");

  //TH1F	*fhLc_pt2 = (TH1F*)l2_2->FindObject("fLcPt");
  //TH1F	*fhD0_pt2 = (TH1F*)l2_2->FindObject("fD0Pt");

//pythia6
  TH1F	*fhD0_pt1 = (TH1F*)l1_1->FindObject("SpCand_UE_Pt"); //fHistJetPt_SpeCandi
  TH1F	*fhLc_pt1 = (TH1F*)l2_1->FindObject("SpCand_UE_Pt"); //fHistJetPt_SpeCandi

//PYTHIA8
TH1F	*fhD0_pt2 = (TH1F*)l3_1->FindObject("fHistJetPt_SpeCandi");
TH1F	*fhLc_pt2 = (TH1F*)l4_1->FindObject("fHistJetPt_SpeCandi");


  TFile *f3 = new TFile("lcd0_pythia8_all.root");
  TH1F *lcd0 = (TH1F*)f3->Get("h_lcd0_monash");

  TCanvas *c2 = new TCanvas("c2","c2",800,600);
  c2->Divide(1,1);
  gStyle->SetOptStat(0);
  auto legend = new TLegend(0.1,0.7,0.48,0.9);

  c2->cd(1);
  fhLc_pt1->Divide(fhD0_pt1);
  fhLc_pt1->Draw("");//C

  fhLc_pt2->Divide(fhD0_pt2);
  fhLc_pt2->Draw("SAME");


  fhLc_pt2->SetLineColor(6);

  legend->AddEntry(fhLc_pt1," Pythia8_charm_ColorHard mode2 ","l");
  legend->AddEntry(fhLc_pt2," Pythia8_charm ","l");
  //legend->AddEntry(lcd0," Ref: Pythia8 ","l");
  legend->Draw();

}

double_t FindingK (double_t k,TString fname = "")
{
  TFile *f = new TFile("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia6_mb_1553871816/stage_1/output/001/AnalysisResults_FastSim_pythia6_mb.root");

  //TFile *f = new TFile("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia8_charm_1554121250/stage_1/output/001/AnalysisResults_FastSim_pythia8_charm.root");

   //TFile *f = new TFile(fname.Data());
   TList	*l1 = (TList*)f->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
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
  SetStyle();

//  TFile *f2 = TFile::Open(Form("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia6_mb_1553871816/stage_1/output/001/%s",fname2.Data()));

  //TFile *f1 = new TFile("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia6_mb_1553871816/stage_1/output/001/AnalysisResults_FastSim_pythia6_mb.root");
  //TFile *f2 = new TFile("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia6_mb_1553871816/stage_1/output/001/AnalysisResults_FastSim_pythia6_mb.root");

  // get file
  TFile *f1 = new TFile(fname1.Data());
  TFile *f2 = new TFile(fname2.Data());
  // get lists containing histograms
  TList	*l1 = (TList*)f1->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
  THashList	*l2_1 = (THashList*)l1->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");

  TList	*l2 = (TList*)f2->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
  THashList	*l2_2 = (THashList*)l2->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");
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
  //fJetPt2new->Draw("SAME");
  fRefCrossSection->Draw("SAME");
  fJetPt2new->SetLineColor(6);
  //fJetPt1new->Scale(1. / fJetPt1new->Integral("width"));
  //fJetPt2new->Scale(1. / fJetPt2new->Integral("width"));

  legend->AddEntry(fJetPt1new," Pythia6 ","l");
  //legend->AddEntry(fJetPt2new," Pythia8 ","l");
  legend->AddEntry(fRefCrossSection,"HEPData.86229.v1/t1","l");

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
  Double_t histx = fJetPt1new->GetXaxis()->FindBin(x);
  double_t y1 = fJetPt1new->GetBinContent(histx); //2
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
  SetStyle();
  // get file
  TFile *f1 = new TFile(fname1.Data());
  TFile *f2 = new TFile(fname2.Data());
    //TFile *f1 = new TFile("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia6_charm_1553872828/stage_1/output/001/AnalysisResults_FastSim_pythia6_charm.root");
    //TFile *f2 = new TFile("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia6_charm_1553872828/stage_1/output/001/AnalysisResults_FastSim_pythia6_charm.root");

  // get lists containing histograms
  TList	*l1 = (TList*)f1->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
  THashList	*l2_1 = (THashList*)l1->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");

  TList	*l2 = (TList*)f2->Get("AliAnalysisTaskCharmHadronJets_Lc_histos");
  THashList	*l2_2 = (THashList*)l2->FindObject("histosAliAnalysisTaskCharmHadronJets_Lc");
  // get histograms

  TH1F	*fJetPt1 = (TH1F*)l2_1->FindObject("fHistJetPt_SpeCandi"); // try to use the eta cond! is not it is -1.5 to 1.5
  TH1F	*fJetPt2 = (TH1F*)l2_2->FindObject("fHistJetPt_SpeCandi");

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
  fJetPt2->Scale(k2/3); // the eta window is between -1.5 and 1.5 !!!

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

  legend->AddEntry(fJetPt1new," D0 ","l");
  legend->AddEntry(fJetPt2new," Lc ","l");
  legend->Draw();

//  c4->cd(1);
//  fLcJetPt1new->Draw("");
//  fLcJetPt2new->Draw("SAME");
//  fLcJetPt2new->SetLineColor(6);
//  fLcJetPt1new->Scale(1. / fLcJetPt1new->Integral("width")); //normalise
//  fLcJetPt2new->Scale(1. / fLcJetPt2new->Integral("width"));

//  legend->Draw();


}

void Z() {
  SetStyle();

  TFile *f = new TFile("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia8_charm_1554121250/stage_1/output/001/AnalysisResults_FastSim_pythia8_charm.root");

  TList	*l1 = (TList*)f->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
  THashList	*l1_1 = (THashList*)l1->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");

  TList	*l2 = (TList*)f->Get("AliAnalysisTaskCharmHadronJets_Lc_histos");
  THashList	*l2_1 = (THashList*)l2->FindObject("histosAliAnalysisTaskCharmHadronJets_Lc");

  TH3F	*fhD0 = (TH3F*)l1_1->FindObject("fHistSpeCandiJet_ptSpeCandi_ptJet_Z");
  TH3F	*fhLc = (TH3F*)l2_1->FindObject("fHistSpeCandiJet_ptSpeCandi_ptJet_Z");

  TH2D* fhD0_1 = (TH2D*)fhD0->Project3DProfile("zy");
  fhD0_1->SetName("fhD0_1_zx");
  TH2D* fhLc_1 = (TH2D*)fhLc->Project3DProfile("zy");

  TH1D* Proj_ZD0 = (TH1D*)fhD0_1->ProjectionY("fhD0_1",0,3,"");

  TCanvas *c6 = new TCanvas("c6","c6",800,600);
  TCanvas *c7 = new TCanvas("c7","c7",800,600);
  c6->Divide(3,1);
  c7->Divide(2,1);

  c6->cd(1);
  fhD0->Draw("");

  c6->cd(2);
  fhD0_1->Draw("colz");

  c6->cd(3);
  Proj_ZD0->Draw("");

  c7->cd(1);
  fhLc->Draw("");
  c7->cd(2);
  fhLc_1->Draw("colz");

}

void DrawResults(TString fname1 = "", TString fname2 = "") {

  DrawResults1(fname1,fname2);
  CrossSection1(fname1,fname2);
  CrossSection2(fname1,fname2);
}

// for inclusive particles
void ModeComparison(TString fname1 = "",TString fname2 = "")
{

SetStyle();

  TFile *f1 = new TFile(fname1.Data()); //mode1
  TFile *f2 = new TFile(fname2.Data()); //mode2
  TFile *f3 = new TFile("lcd0_pythia8_all.root");//pythia8 reference
  TFile *f4 = new TFile("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia6_charm_1553872828/stage_1/output/001/AnalysisResults_FastSim_pythia6_charm.root");


  TList	*l1 = (TList*)f1->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
  THashList	*l1_1 = (THashList*)l1->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");

  TList	*l2 = (TList*)f2->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
  THashList	*l2_1 = (THashList*)l2->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");

  TList	*l4 = (TList*)f4->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
  THashList	*l4_1 = (THashList*)l4->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");

//mode 1
  TH1F *fD0_pt1 = (TH1F*)l1_1->FindObject("fD0Pt");
  TH1F *fLc_pt1 = (TH1F*)l1_1->FindObject("fLcPt");
//mode 2
  TH1F *fD0_pt2 = (TH1F*)l2_1->FindObject("fD0Pt");
  TH1F *fLc_pt2 = (TH1F*)l2_1->FindObject("fLcPt");
// ref without mode
  TH1F *lcd0 = (TH1F*)f3->Get("h_lcd0_monash");
  //ref for Pythia6
  TH1F *fD0_pt4 = (TH1F*)l4_1->FindObject("fD0Pt");
  TH1F *fLc_pt4 = (TH1F*)l4_1->FindObject("fLcPt");

  TCanvas *c2 = new TCanvas("c2","c2",800,600);
  c2->Divide(1,1);
  gStyle->SetOptStat(0);
  auto legend = new TLegend(0.1,0.7,0.48,0.9);

  c2->cd(1);
    fLc_pt1->Divide(fD0_pt1);
    fLc_pt1->Draw("");//C

    fLc_pt2->Divide(fD0_pt2);
    fLc_pt2->Draw("SAME");
    fLc_pt2->SetLineColor(6);

    lcd0->Draw("SAME");

    fLc_pt4->Divide(fD0_pt4);
    fLc_pt4->Draw("SAME");
    fLc_pt4->SetLineColor(30);

    legend->AddEntry(fLc_pt1," Pythia8_charm_ColorSoft mode1","l");
    legend->AddEntry(fLc_pt2," Pythia8_charm_ColorHard mode1","l");
    legend->AddEntry(lcd0," Ref: Pythia8 ","l");
    legend->AddEntry(fLc_pt4," Pythia6 charm ","l");
    legend->Draw();
}
//AnalysisResults_FastSim_pythia8_charm_ColorHard_1554883740.root
//AnalysisResults_FastSim_pythia8_charm_ColorSoft_1554562536.root

//ModeComparison("AnalysisResults_FastSim_pythia8_charm_ColorSoft_1554562536.root","AnalysisResults_FastSim_pythia8_charm_ColorHard_1554883740.root")

//MCZ("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia8_charm_ColorSoft_1555678227/stage_1/output/001/AnalysisResults_FastSim_pythia8_charm_ColorSoft.root","/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia8_charm_ColorHard_1555665226/stage_1/output/001/AnalysisResults_FastSim_pythia8_charm_ColorHard.root")
//ModeComparison("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia8_charm_ColorSoft_1555678227/stage_1/output/001/AnalysisResults_FastSim_pythia8_charm_ColorSoft.root","/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia8_charm_ColorHard_1555665226/stage_1/output/001/AnalysisResults_FastSim_pythia8_charm_ColorHard.root")

void MCZ(TString fname1 = "",TString fname2 = "")
{
  SetStyle();
  TFile *f1 = new TFile(fname1.Data()); //mode1
  TFile *f2 = new TFile(fname2.Data()); //mode2
  TFile *f3 = new TFile("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia6_charm_1553872828/stage_1/output/001/AnalysisResults_FastSim_pythia6_charm.root");
  TFile *f4 = new TFile("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia8_charm_1554121250/stage_1/output/001/AnalysisResults_FastSim_pythia8_charm.root");


  TList	*l1 = (TList*)f1->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
  THashList	*l1_1 = (THashList*)l1->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");

  TList	*l2 = (TList*)f2->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
  THashList	*l2_1 = (THashList*)l2->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");

  TList	*l3 = (TList*)f3->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
  THashList	*l3_1 = (THashList*)l3->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");

  TList	*l4 = (TList*)f4->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
  THashList	*l4_1 = (THashList*)l4->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");

  //mode 1
    TH1F *fD0_Z1 = (TH1F*)l1_1->FindObject("fZ_D0");
    TH1F *fLc_Z1 = (TH1F*)l1_1->FindObject("fZ_Lc");
  //mode 2
    TH1F *fD0_Z2 = (TH1F*)l2_1->FindObject("fZ_D0");
    TH1F *fLc_Z2 = (TH1F*)l2_1->FindObject("fZ_Lc");
//pythia6 ref
    TH1F *fD0_Z3 = (TH1F*)l3_1->FindObject("fZ_D0");
    TH1F *fLc_Z3 = (TH1F*)l3_1->FindObject("fZ_Lc");
//pythia8 ref
    TH1F *fD0_Z4 = (TH1F*)l4_1->FindObject("fZ_D0");
    TH1F *fLc_Z4 = (TH1F*)l4_1->FindObject("fZ_Lc");


    TH1F *fD0Z1 = (TH1F*) fD0_Z1->Clone();
    TH1F *fLcZ1 = (TH1F*) fLc_Z1->Clone();
    TH1F *fD0Z2 = (TH1F*) fD0_Z2->Clone();
    TH1F *fLcZ2 = (TH1F*) fLc_Z2->Clone();


    TCanvas *c2 = new TCanvas("c2","c2",800,600);
    c2->Divide(1,1);
    TCanvas *c3 = new TCanvas("c3","c3",800,600);
    c3->Divide(2,1);


    gStyle->SetOptStat(0);
    auto legend = new TLegend(0.1,0.7,0.48,0.9);
    auto legend1 = new TLegend(0.1,0.7,0.48,0.9);
    auto legend2 = new TLegend(0.1,0.7,0.48,0.9);

    c3->cd(1);
      fLc_Z1->Draw("");
      fD0_Z1->Draw("SAME");
      fD0_Z1->SetLineColor(6);
      legend1->AddEntry(fLc_Z1," Pythia8_charm_ColorSoft mode1","l");
      legend1->Draw();

    c3->cd(2);
      fLc_Z2->Draw("");
      fD0_Z2->Draw("SAME");
      fD0_Z2->SetLineColor(6);
      legend2->AddEntry(fLc_Z2," Pythia8_charm_ColorHard mode1","l");
      legend2->Draw();

    c2->cd(1);
      fLcZ1->Divide(fD0Z1);
      fLcZ1->Draw("");//C

      fLcZ2->Divide(fD0Z2);
      fLcZ2->Draw("SAME");
      fLcZ2->SetLineColor(6);

      fLc_Z3->Divide(fD0_Z3);
      fLc_Z3->Draw("SAME");
      fLc_Z3->SetLineColor(30);

      fLc_Z4->Divide(fD0_Z4);
      fLc_Z4->Draw("SAME");
      fLc_Z4->SetLineColor(34);

      legend->AddEntry(fLcZ1," Pythia8_charm_ColorSoft mode1","l");
      legend->AddEntry(fLcZ2," Pythia8_charm_ColorHard mode1","l");
      legend->AddEntry(fLc_Z3," Pythia6 charm ","l");
      legend->AddEntry(fLc_Z4," Pythia8 charm ","l");
      legend->Draw();
}

//comparison pythia6 and pythia8 grid, for ratio of inclusive and in jets

// /Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia8_charm_ColorHard_1555677760/stage_1/output/001/AnalysisResults_FastSim_pythia8_charm_ColorHard.root
// /Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia8_charm_ColorSoft_1554562536/stage_1/output/001/AnalysisResults_FastSim_pythia8_charm_ColorSoft.root
void Jet_Incl ()
{
  SetStyle();
  // get file
    TFile *f1 = new TFile("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia6_charm_1553872828/stage_1/output/001/AnalysisResults_FastSim_pythia6_charm.root");
    TFile *f2 = new TFile("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia8_charm_1554121250/stage_1/output/001/AnalysisResults_FastSim_pythia8_charm.root");
  // get lists containing histograms
  TList	*l1 = (TList*)f1->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
  THashList	*l1_1 = (THashList*)l1->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");

  TList	*l2 = (TList*)f1->Get("AliAnalysisTaskCharmHadronJets_Lc_histos");
  THashList	*l2_1 = (THashList*)l2->FindObject("histosAliAnalysisTaskCharmHadronJets_Lc");

  TList	*l3 = (TList*)f2->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
  THashList	*l3_1 = (THashList*)l3->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");

  TList	*l4 = (TList*)f2->Get("AliAnalysisTaskCharmHadronJets_Lc_histos");
  THashList	*l4_1 = (THashList*)l4->FindObject("histosAliAnalysisTaskCharmHadronJets_Lc");


  //p6 inc part
    TH1F *fD0_pt1 = (TH1F*)l1_1->FindObject("fD0Pt");
    TH1F *fLc_pt1 = (TH1F*)l1_1->FindObject("fLcPt");
  //jet
  TH1F	*fhD0JETpt1 = (TH1F*)l1_1->FindObject("fHistJetPt_SpeCandi");
  TH1F	*fhLcJETpt1 = (TH1F*)l2_1->FindObject("fHistJetPt_SpeCandi");


  //p8
    TH1F *fD0_pt2 = (TH1F*)l3_1->FindObject("fD0Pt");
    TH1F *fLc_pt2 = (TH1F*)l3_1->FindObject("fLcPt");
  //jet
  TH1F	*fhD0JETpt2 = (TH1F*)l3_1->FindObject("fHistJetPt_SpeCandi");
  TH1F	*fhLcJETpt2 = (TH1F*)l4_1->FindObject("fHistJetPt_SpeCandi");

  TCanvas *c2 = new TCanvas("c2","c2",800,600);
  c2->Divide(1,1);
  gStyle->SetOptStat(0);
  auto legend = new TLegend(0.1,0.7,0.48,0.9);

  c2->cd(1);
  fLc_pt1->Divide(fD0_pt1);
  fLc_pt1->Draw("");//C

  fLc_pt2->Divide(fD0_pt2);
  fLc_pt2->Draw("SAME");
  fLc_pt2->SetLineColor(6);

  fhLcJETpt1->Divide(fhD0JETpt1);
  fhLcJETpt1->Draw("SAME");
  fhLcJETpt1->SetLineColor(30);

  fhLcJETpt2->Divide(fhD0JETpt2);
  fhLcJETpt2->Draw("SAME");
  fhLcJETpt2->SetLineColor(36);

  legend->AddEntry(fLc_pt1," pythia 6 charm inclusive ratio ","l");
  legend->AddEntry(fLc_pt2," pythia 8 charm inclusive ratio ","l");
  legend->AddEntry(fhLcJETpt1," pythia 6 charm in jets ratio ","l");
  legend->AddEntry(fhLcJETpt2," pythia 8 charm in jets ratio ","l");

  legend->Draw();

}

void analysis1(TString fname1 = "",TString fname2 = "",TString fname3 = "",TString fname4 = "")

{

SetStyle();

// get files

TFile *f1 = new TFile(fname1.Data());
TFile *f2 = new TFile(fname2.Data());
TFile *f3 = new TFile(fname3.Data());
TFile *f4 = new TFile(fname4.Data());
//TFile *f1 = new TFile("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia6_charm_155"); //pythia6 charm
//TFile *f2 = new TFile("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia8_charm_155"); // pythia8 charm
//TFile *f3 = new TFile("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia8_charm_ColorHard_1555677760/stage_1/output/001/AnalysisResults_FastSim_pythia8_charm_ColorHard.root"); // pythia8 color reconnection Hard
//TFile *f4 = new TFile("/Users/sadek/Analysis/alice-fast-simulation/GridOutput/FastSim_pythia8_charm_ColorSoft_1555678227/stage_1/output/001/AnalysisResults_FastSim_pythia8_charm_ColorSoft.root"); // pythia8 color reconnection soft

// get list containing histograms

//f1
TList	*l1_1_ = (TList*)f1->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
THashList	*l1_1 = (THashList*)l1_1_->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");

TList	*l1_2_ = (TList*)f1->Get("AliAnalysisTaskCharmHadronJets_Lc_histos");
THashList	*l1_2 = (THashList*)l1_2_->FindObject("histosAliAnalysisTaskCharmHadronJets_Lc");
//f2
TList	*l2_1_ = (TList*)f2->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
THashList	*l2_1 = (THashList*)l2_1_->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");

TList	*l2_2_ = (TList*)f2->Get("AliAnalysisTaskCharmHadronJets_Lc_histos");
THashList	*l2_2 = (THashList*)l2_2_->FindObject("histosAliAnalysisTaskCharmHadronJets_Lc");
//f3
TList	*l3_1_ = (TList*)f3->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
THashList	*l3_1 = (THashList*)l3_1_->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");

TList	*l3_2_ = (TList*)f3->Get("AliAnalysisTaskCharmHadronJets_Lc_histos");
THashList	*l3_2 = (THashList*)l3_2_->FindObject("histosAliAnalysisTaskCharmHadronJets_Lc");
//f4
TList	*l4_1_ = (TList*)f4->Get("AliAnalysisTaskCharmHadronJets_D0_histos");
THashList	*l4_1 = (THashList*)l4_1_->FindObject("histosAliAnalysisTaskCharmHadronJets_D0");

TList	*l4_2_ = (TList*)f4->Get("AliAnalysisTaskCharmHadronJets_Lc_histos");
THashList	*l4_2 = (THashList*)l4_2_->FindObject("histosAliAnalysisTaskCharmHadronJets_Lc");

// get necessary histos for f1

//D0 related histos
  TH2F *LJ_D0_DPhEta_p6 = (TH2F*)l1_1->FindObject("fLeadingJet_SpCand_DEta_DPhi"); // the Deta and Dphi between LJ and sp candidate
  TH2F *LJ_D0_pt_jet_p6 = (TH2F*)l1_1->FindObject("LJ_SpCand_pt_inJet"); // sp particles found in jets
  TH2F *LJ_D0_pt_ue_p6 = (TH2F*)l1_1->FindObject("LJ_SpCand_pt_UE");  // sp particles found in U.E
//Lc related histos
  TH2F *LJ_Lc_DPhEta_p6 = (TH2F*)l1_2->FindObject("fLeadingJet_SpCand_DEta_DPhi");
  TH2F *LJ_Lc_pt_jet_p6 = (TH2F*)l1_2->FindObject("LJ_SpCand_pt_inJet");
  TH2F *LJ_Lc_pt_ue_p6 = (TH2F*)l1_2->FindObject("LJ_SpCand_pt_UE");

// get necessary histos for f2

//D0 related histos
  TH2F *LJ_D0_DPhEta_p8 = (TH2F*)l2_1->FindObject("fLeadingJet_SpCand_DEta_DPhi"); // the Deta and Dphi between LJ and sp candidate
  TH2F *LJ_D0_pt_jet_p8 = (TH2F*)l2_1->FindObject("LJ_SpCand_pt_inJet"); // sp particles found in jets
  TH2F *LJ_D0_pt_ue_p8 = (TH2F*)l2_1->FindObject("LJ_SpCand_pt_UE");  // sp particles found in U.E
//Lc related histos
  TH2F *LJ_Lc_DPhEta_p8 = (TH2F*)l2_2->FindObject("fLeadingJet_SpCand_DEta_DPhi");
  TH2F *LJ_Lc_pt_jet_p8 = (TH2F*)l2_2->FindObject("LJ_SpCand_pt_inJet");
  TH2F *LJ_Lc_pt_ue_p8 = (TH2F*)l2_2->FindObject("LJ_SpCand_pt_UE");

// get necessary histos for f3

//D0 related histos
  TH2F *LJ_D0_DPhEta_p8ColorHard = (TH2F*)l3_1->FindObject("fLeadingJet_SpCand_DEta_DPhi"); // the Deta and Dphi between LJ and sp candidate
  TH2F *LJ_D0_pt_jet_p8ColorHard = (TH2F*)l3_1->FindObject("LJ_SpCand_pt_inJet"); // sp particles found in jets
  TH2F *LJ_D0_pt_ue_p8ColorHard = (TH2F*)l3_1->FindObject("LJ_SpCand_pt_UE");  // sp particles found in U.E
//Lc related histos
  TH2F *LJ_Lc_DPhEta_p8ColorHard = (TH2F*)l3_2->FindObject("fLeadingJet_SpCand_DEta_DPhi");
  TH2F *LJ_Lc_pt_jet_p8ColorHard = (TH2F*)l3_2->FindObject("LJ_SpCand_pt_inJet");
  TH2F *LJ_Lc_pt_ue_p8ColorHard = (TH2F*)l3_2->FindObject("LJ_SpCand_pt_UE");

// get necessary histos for f4

//D0 related histos
  TH2F *LJ_D0_DPhEta_p8ColorSoft = (TH2F*)l4_1->FindObject("fLeadingJet_SpCand_DEta_DPhi"); // the Deta and Dphi between LJ and sp candidate
  TH2F *LJ_D0_pt_jet_p8ColorSoft = (TH2F*)l4_1->FindObject("LJ_SpCand_pt_inJet"); // sp particles found in jets
  TH2F *LJ_D0_pt_ue_p8ColorSoft = (TH2F*)l4_1->FindObject("LJ_SpCand_pt_UE");  // sp particles found in U.E
//Lc related histos
  TH2F *LJ_Lc_DPhEta_p8ColorSoft = (TH2F*)l4_2->FindObject("fLeadingJet_SpCand_DEta_DPhi");
  TH2F *LJ_Lc_pt_jet_p8ColorSoft = (TH2F*)l4_2->FindObject("LJ_SpCand_pt_inJet");
  TH2F *LJ_Lc_pt_ue_p8ColorSoft = (TH2F*)l4_2->FindObject("LJ_SpCand_pt_UE");


TCanvas *c1 = new TCanvas("c1","c1",800,600);
c1->Divide(2,2);
auto legend1 = new TLegend(0.1,0.7,0.48,0.9);
auto legend2 = new TLegend(0.1,0.7,0.48,0.9);
auto legend3 = new TLegend(0.1,0.7,0.48,0.9);
auto legend4 = new TLegend(0.1,0.7,0.48,0.9);

// first canvas is to plot the 2D (Deta and Dphi) Lc/D0 ratio for each f.
c1->cd(1);
LJ_Lc_DPhEta_p6->Divide(LJ_D0_DPhEta_p6);
LJ_Lc_DPhEta_p6->Draw("");
legend1->AddEntry(LJ_Lc_DPhEta_p6,"pythia 6","l");
legend1->Draw();
c1->cd(2);
LJ_Lc_DPhEta_p8->Divide(LJ_D0_DPhEta_p8);
LJ_Lc_DPhEta_p8->Draw("");
LJ_Lc_DPhEta_p8->SetLineColor(36);
legend2->AddEntry(LJ_Lc_DPhEta_p8,"pythia 8","l");
legend2->Draw();
c1->cd(3);
LJ_Lc_DPhEta_p8ColorHard->Divide(LJ_D0_DPhEta_p8ColorHard);
LJ_Lc_DPhEta_p8ColorHard->Draw("");
LJ_Lc_DPhEta_p8ColorHard->SetLineColor(30);
legend3->AddEntry(LJ_Lc_DPhEta_p8ColorHard,"pythia 8 color hard mode 1","l");
legend3->Draw();
c1->cd(4);
LJ_Lc_DPhEta_p8ColorSoft->Divide(LJ_D0_DPhEta_p8ColorSoft);
LJ_Lc_DPhEta_p8ColorSoft->Draw("");
LJ_Lc_DPhEta_p8ColorSoft->SetLineColor(6);
legend4->AddEntry(LJ_Lc_DPhEta_p8ColorSoft,"pythia 8 color soft mode 1","l");
legend4->Draw();


TCanvas *c2 = new TCanvas("c2","c2",800,600);
c2->Divide(2,1);
auto legend1_ = new TLegend(0.1,0.7,0.48,0.9);
auto legend2_ = new TLegend(0.1,0.7,0.48,0.9);

//Plot on the same 1canva the UE ratio for different modes (fs), general ( should put a cond of LJ_pt>10GeV/c)
//Plot on the same 2canva the JETS ratio for different modes (fs), general ( should also later put a cond of LJ_pt>10GeV/c)

//UE projections for D0, different modes
TH1D* Proj_D0Pt_UE_p6 = LJ_D0_pt_ue_p6->ProjectionY("_py",10,-1,"d");
TH1D* Proj_D0Pt_UE_p8 = LJ_D0_pt_ue_p8->ProjectionY("_py",10,-1,"d");
TH1D* Proj_D0Pt_UE_p8ColorHard = LJ_D0_pt_ue_p8ColorHard->ProjectionY("_py",10,-1,"d");
TH1D* Proj_D0Pt_UE_p8ColorSoft = LJ_D0_pt_ue_p8ColorSoft->ProjectionY("_py",10,-1,"d");
//UE Lc projections for diff modes
TH1D* Proj_LcPt_UE_p6 = LJ_Lc_pt_ue_p6->ProjectionY("_py",10,-1,"d");
TH1D* Proj_LcPt_UE_p8 = LJ_Lc_pt_ue_p8->ProjectionY("_py",10,-1,"d");
TH1D* Proj_LcPt_UE_p8ColorHard = LJ_Lc_pt_ue_p8ColorHard->ProjectionY("_py",10,-1,"d");
TH1D* Proj_LcPt_UE_p8ColorSoft = LJ_Lc_pt_ue_p8ColorSoft->ProjectionY("_py",10,-1,"d");

//in jets projections for D0, different modes
TH1D* Proj_D0Pt_jet_p6 = LJ_D0_pt_jet_p6->ProjectionY("_py",10,-1,"d");
TH1D* Proj_D0Pt_jet_p8 = LJ_D0_pt_jet_p8->ProjectionY("_py",10,-1,"d");
TH1D* Proj_D0Pt_jet_p8ColorHard = LJ_D0_pt_jet_p8ColorHard->ProjectionY("_py",10,-1,"d");
TH1D* Proj_D0Pt_jet_p8ColorSoft = LJ_D0_pt_jet_p8ColorSoft->ProjectionY("_py",10,-1,"d");
//in jets Lc projections for diff modes
TH1D* Proj_LcPt_jet_p6 = LJ_Lc_pt_jet_p6->ProjectionY("_py",10,-1,"d");
TH1D* Proj_LcPt_jet_p8 = LJ_Lc_pt_jet_p8->ProjectionY("_py",10,-1,"d");
TH1D* Proj_LcPt_jet_p8ColorHard = LJ_Lc_pt_jet_p8ColorHard->ProjectionY("_py",10,-1,"d");
TH1D* Proj_LcPt_jet_p8ColorSoft = LJ_Lc_pt_jet_p8ColorSoft->ProjectionY("_py",10,-1,"d");

//the UE ratio
c2->cd(1);
Proj_LcPt_UE_p6->Divide(Proj_D0Pt_UE_p6);
Proj_LcPt_UE_p6->Draw();

Proj_LcPt_UE_p8->Divide(Proj_D0Pt_UE_p8);
Proj_LcPt_UE_p8->Draw("SAME");
Proj_LcPt_UE_p8->SetLineColor(36);

Proj_LcPt_UE_p8ColorHard->Divide(Proj_D0Pt_UE_p8ColorHard);
Proj_LcPt_UE_p8ColorHard->Draw("SAME");
Proj_LcPt_UE_p8ColorHard->SetLineColor(30);

Proj_LcPt_UE_p8ColorSoft->Divide(Proj_D0Pt_UE_p8ColorSoft);
Proj_LcPt_UE_p8ColorSoft->Draw("SAME");
Proj_LcPt_UE_p8ColorSoft->SetLineColor(6);

legend1_->AddEntry(Proj_LcPt_UE_p6,"pythia 6","l");
Legend1_->AddEntry(Proj_LcPt_UE_p8,"pythia 8","l");
Legend1_->AddEntry(Proj_LcPt_UE_p8ColorHard,"pythia 8 color hard mode 1","l");
Legend1_->AddEntry(Proj_LcPt_UE_p8ColorSoft,"pythia 8 color soft mode 1","l");
legend1_->Draw();

//the in jets ratio
c2->cd(2);
Proj_LcPt_jet_p6->Divide(Proj_D0Pt_jet_p6);
Proj_LcPt_jet_p6->Draw();

Proj_LcPt_jet_p8->Divide(Proj_D0Pt_jet_p8);
Proj_LcPt_jet_p8->Draw("SAME");
Proj_LcPt_jet_p8->SetLineColor(36);

Proj_LcPt_jet_p8ColorHard->Divide(Proj_D0Pt_jet_p8ColorHard);
Proj_LcPt_jet_p8ColorHard->Draw("SAME");
Proj_LcPt_jet_p8ColorHard->SetLineColor(30);

Proj_LcPt_jet_p8ColorSoft->Divide(Proj_D0Pt_jet_p8ColorSoft);
Proj_LcPt_jet_p8ColorSoft->Draw("SAME");
Proj_LcPt_jet_p8ColorSoft->SetLineColor(6);

legend2_->AddEntry(Proj_LcPt_jet_p6,"pythia 6","l");
Legend2_->AddEntry(Proj_LcPt_jet_p8,"pythia 8","l");
Legend2_->AddEntry(Proj_LcPt_jet_p8ColorHard,"pythia 8 color hard mode 1","l");
Legend2_->AddEntry(Proj_LcPt_jet_p8ColorSoft,"pythia 8 color soft mode 1","l");
legend2_->Draw();


TCanvas *c3 = new TCanvas("c3","c3",1000,800);
c3->Divide(4,1);
auto legend_1 = new TLegend(0.1,0.7,0.48,0.9); //p6
auto legend_2 = new TLegend(0.1,0.7,0.48,0.9); //p8
auto legend_3 = new TLegend(0.1,0.7,0.48,0.9); //p8Color Hard
auto legend_4 = new TLegend(0.1,0.7,0.48,0.9); //p8 color soft

//p6 inc part
  TH1F *D0_pt_p6 = (TH1F*)l1_1->FindObject("fD0Pt");
  TH1F *Lc_pt_p6 = (TH1F*)l1_1->FindObject("fLcPt");
//p8 incl part
  TH1F *D0_pt_p8 = (TH1F*)l2_1->FindObject("fD0Pt");
  TH1F *Lc_pt_p8 = (TH1F*)l2_1->FindObject("fLcPt");
//p8 color hard incl part
  TH1F *D0_pt_p8ColorHard = (TH1F*)l3_1->FindObject("fD0Pt");
  TH1F *Lc_pt_p8ColorHard = (TH1F*)l3_1->FindObject("fLcPt");
//p8 color soft incl part
  TH1F *D0_pt_p8ColorSoft = (TH1F*)l4_1->FindObject("fD0Pt");
  TH1F *Lc_pt_p8ColorSoft = (TH1F*)l4_1->FindObject("fLcPt");

// plot UE,IN JETS and inclusive ratio on 1 canva, for 1 mode a canva. (general. Should add a cut too on LJ_pt )
c3->cd(1); //p6
//ue
Proj_LcPt_UE_p6->Divide(Proj_D0Pt_UE_p6);
Proj_LcPt_UE_p6->Draw();
//injet
Proj_LcPt_jet_p6->Divide(Proj_D0Pt_jet_p6);
Proj_LcPt_jet_p6->Draw("SAME");
Proj_LcPt_jet_p6->SetLineColor(6);
//incl part
Lc_pt_p6->Divide(D0_pt_p6);
Lc_pt_p6->Draw("SAME");
Lc_pt_p6->SetLineColor(36);

legend_1->AddEntry(Proj_LcPt_UE_p6,"UE","l");
legend_1->AddEntry(Proj_LcPt_jet_p6,"JE","l");
legend_1->AddEntry(Lc_pt_p6,"Incl","l");
legend_1->Draw();

c3->cd(2);//p8
//ue
Proj_LcPt_UE_p8->Divide(Proj_D0Pt_UE_p8);
Proj_LcPt_UE_p8->Draw();
//injet
Proj_LcPt_jet_p8->Divide(Proj_D0Pt_jet_p8);
Proj_LcPt_jet_p8->Draw("SAME");
Proj_LcPt_jet_p8->SetLineColor(6);
//incl part
Lc_pt_p8->Divide(D0_pt_p8);
Lc_pt_p8->Draw("SAME");
Lc_pt_p8->SetLineColor(36);

legend_2->AddEntry(Proj_LcPt_UE_p8,"UE","l");
legend_2->AddEntry(Proj_LcPt_jet_p8,"JE","l");
legend_2->AddEntry(Lc_pt_p8,"Incl","l");
legend_2->Draw();

c3->cd(3);//hard
//ue
Proj_LcPt_UE_p8ColorHard->Divide(Proj_D0Pt_UE_p8ColorHard);
Proj_LcPt_UE_p8ColorHard->Draw();
//injet
Proj_LcPt_jet_p8ColorHard->Divide(Proj_D0Pt_jet_p8ColorHard);
Proj_LcPt_jet_p8ColorHard->Draw("SAME");
Proj_LcPt_jet_p8ColorHard->SetLineColor(6);
//incl part
Lc_pt_p8ColorHard->Divide(D0_pt_p8ColorHard);
Lc_pt_p8ColorHard->Draw("SAME");
Lc_pt_p8ColorHard->SetLineColor(36);

legend_3->AddEntry(Proj_LcPt_UE_p8ColorHard,"UE","l");
legend_3->AddEntry(Proj_LcPt_jet_p8ColorHard,"JE","l");
legend_3->AddEntry(Lc_pt_p8ColorHard,"Incl","l");
legend_3->Draw();

c3->cd(4);//soft
//ue
Proj_LcPt_UE_p8ColorSoft->Divide(Proj_D0Pt_UE_p8ColorSoft);
Proj_LcPt_UE_p8ColorSoft->Draw();
//injet
Proj_LcPt_jet_p8ColorSoft->Divide(Proj_D0Pt_jet_p8ColorSoft);
Proj_LcPt_jet_p8ColorSoft->Draw("SAME");
Proj_LcPt_jet_p8ColorSoft->SetLineColor(6);
//incl part
Lc_pt_p8ColorSoft->Divide(D0_pt_p8ColorSoft);
Lc_pt_p8ColorSoft->Draw("SAME");
Lc_pt_p8ColorSoft->SetLineColor(36);

legend_4->AddEntry(Proj_LcPt_UE_p8ColorSoft,"UE","l");
legend_4->AddEntry(Proj_LcPt_jet_p8ColorSoft,"JE","l");
legend_4->AddEntry(Lc_pt_p8ColorSoft,"Incl","l");
legend_4->Draw();

}

void SetStyle() {
  cout << "Setting style!" << endl;

  gStyle->Reset("Plain");
  gStyle->SetOptTitle(0);
  //  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gStyle->SetCanvasColor(10);
  //  gStyle->SetCanvasBorderMode(0);
  //  gStyle->SetFrameLineWidth(1);
  gStyle->SetFrameFillColor(kWhite);
  gStyle->SetPadColor(10);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadTopMargin(0.07);
  gStyle->SetPadRightMargin(0.10);
  gStyle->SetHistLineWidth(2);
  gStyle->SetHistLineColor(kRed);
  gStyle->SetFuncWidth(2);
  gStyle->SetFuncColor(kGreen);
  //  gStyle->SetLineWidth(2);
  gStyle->SetLabelSize(0.035,"xyz");
  gStyle->SetLabelOffset(0.01,"y");
  gStyle->SetLabelOffset(0.01,"x");
  gStyle->SetLabelColor(kBlack,"xyz");
  gStyle->SetTitleSize(0.055,"xyz");
  gStyle->SetTitleOffset(1.3,"y");
  gStyle->SetTitleOffset(1.15,"x");
  gStyle->SetTitleFillColor(kWhite);
  //  gStyle->SetTextSizePixels(30);
  gStyle->SetTextFont(42);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendFillColor(kWhite);
  gStyle->SetLegendFont(42);
  //gStyle->SetLineWidth(2);

  gROOT->ForceStyle();
  // gStyle->SetMarkerStyle(20);
  // gStyle->SetMarkerSize(1.2);
  // gStyle->SetMarkerColor(kBlack);
}


void SetStyleHisto( TH1 *h){

  //h->SetLineColor(kBlack);
  //h->SetLineWidth(2);
  //h->GetYaxis()->SetLabelFont(42);
  //h->GetYaxis()->SetTitleFont(42);
  h->GetYaxis()->SetTitleSize(0.055);
  h->GetYaxis()->SetTitleOffset(1.3);
  h->GetYaxis()->SetLabelSize(0.05);
  //h->GetYaxis()->SetNdivisions(507);
  h->GetXaxis()->SetTitleFont(42);
  h->GetXaxis()->SetLabelFont(42);
  h->GetXaxis()->SetTitleSize(0.055);
  h->GetXaxis()->SetTitleOffset(1.0);
  h->GetXaxis()->SetLabelSize(0.05);
  h->GetXaxis()->SetNdivisions(505);
}
