// A handy script for fetching the number of entries in a pthat sliced MC sample
// The user should replace fileformat and dirname according to present needs
// Aouthor Hannu Siikonen hannu.siikonen@cern.ch

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include "TMath.h"
#include "TStyle.h"
#include "TF1.h"
#include "TF2.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TStopwatch.h"
#include "TSystem.h"
#include "TLorentzVector.h"
#include "TRandom3.h"

#include <map>
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <regex>

const int maxpu = 80;

void Pufromflat(const char* filename, const char* tag = "MC") {
  TFile *output = new TFile(Form("pileup_%s.root",tag),"RECREATE");
  TFile *f = new TFile(filename);
  TTree *t = dynamic_cast<TTree*>(f->Get("ak4/ProcessedTree"));
  TH1D *hist = new TH1D("pileupmc","",maxpu,0,maxpu);
  t->Draw("EvtHdr_.mTrPu>>pileupmc","EvtHdr_.mWeight");
  output->cd();
  hist->Write();
  output->Close();
  output->Delete();
}
