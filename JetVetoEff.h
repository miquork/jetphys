// Adapted from Laura Martikainen by Hannu Siikonen

#include "tdrstyle_mod22.C"
#include "settings.h"

void JetVetoEff(string outputdir = "Plots", string extra ="") {
  int itag = jp::yid == 0 ? 1 : (jp::yid == 3 ? 3 : 2);
  int year = 15 + itag;
  lumi_bonus = Form("20%d", year);

  TFile *fHotExcl = new TFile(jp::hotfile,"READ");

  TH1::AddDirectory(kFALSE);

  TDirectory *curdir = gDirectory;

  TH2D* h2HotExcl = (TH2D*)fHotExcl->Get(Form("h2hot_%s",jp::hottype));

  // UL2016 needs also the mc map
  // if (jp::UL16 and jp::yid == 0) {
  if (jp::yid == 0) {
    TH2D* h2HotExcl2 = (TH2D*)fHotExcl->Get("h2hot_mc");
    h2HotExcl->Add(h2HotExcl2);
  }
  h2HotExcl->GetXaxis()->SetTitleSize(0.05);
  h2HotExcl->GetYaxis()->SetTitleSize(0.05);

  int nbins = h2HotExcl->GetNbinsX();
  int ybins = h2HotExcl->GetNbinsY();

  // Find fraction of zones excluded

  TH1D *ratio = h2HotExcl->ProjectionX();
  ratio->SetTitle(";;Statistics Loss (%)");
  ratio->GetXaxis()->SetTitleSize(0.05);
  ratio->GetYaxis()->SetTitleSize(0.05);

  ratio->Reset();
  for (int i = 0; i < nbins; ++i) {
    double sum = 0;
    for (int j = 0; j < ybins; ++j) {
      if (h2HotExcl->GetBinContent(i,j) > 0) sum++;
    }
    ratio->SetBinContent(i, 100.*sum/ybins);
    ratio->SetBinError(i,0);
    //    ratio->SetFillColorAlpha(46,0.5);
  }

  // Plot
  ratio->SetMaximum(15.);
  // lumi_13TeV = "Excluded areas";
  extraText = " ";
  TCanvas *c1 = tdrDiCanvas("c1",h2HotExcl,ratio,itag,33); // Spectra + ratios

  c1->cd(1);

  Int_t palette[3];  palette[0]=0, palette[1]=0, palette[2] = 33;
  gStyle->SetPalette(3,palette);

  h2HotExcl->GetYaxis()->SetTitle("#phi");
  h2HotExcl->SetLineColor(kBlack);
  h2HotExcl->Draw("SAMEcol"); // draw in color that can be seen

  CMS_lumi( (TPad*) gPad, itag, 33);
  gPad->RedrawAxis();
  gPad->Update();

  c1->cd(2);

  ratio->SetFillColor(33);
  ratio->SetLineColor(kBlack);
  ratio->Draw();
  ratio->GetXaxis()->SetTitle("#eta");

  gPad->RedrawAxis();
  gPad->Update();

  c1->cd(0);
  gPad->Update();

  curdir->cd();

  c1->Print(Form("%s/hotzone_%d%s.pdf",outputdir.c_str(), year, extra.c_str()));
}

