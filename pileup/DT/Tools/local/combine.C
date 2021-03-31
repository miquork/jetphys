#include "../../../settings.h"

const vector<string> tags = {"500", "450", "400", "320", "260", "200", "140", "80", "60", "40", "0"};
const vector<string> tags16 = {"450", "400", "320", "260", "200", "140", "80", "60", "40", "0"};

void combine(const char* Era = "B") {
  TFile f("pileup_DTu.root","RECREATE");
  TFile g("pileup_DT.root","RECREATE");
  int eraNo = 0;
  for (auto &eraMatch : jp::eras) {
    if (Era==eraMatch) break;
    ++eraNo;
  }
  cout << "Found " << Era << " at the index " << eraNo << endl;

  const auto &tgs = jp::yid==0 ? tags16 : tags;
  double scale = 1.0;
  for (unsigned i = 0; i < tgs.size(); ++i) {
    TString file, name;
    file.Form("HLT_PFJet%s.root", tgs[i].c_str());
    name.Form("jt%s", tgs[i].c_str());

    TFile h(file.Data(),"READ");
    h.cd();
    TH1D* handle = dynamic_cast<TH1D*>(gROOT->FindObject("pileup"));
    if (handle) {
      handle->Scale(jp::triglumiera[eraNo][tgs.size()-1]/jp::triglumiera[eraNo][tgs.size()-1-i]);
      f.cd();
      handle->SetName(name.Data());
      handle->SetTitle(name.Data());
      handle->Write();
      g.cd();
      if (i==0) scale = 1.0/handle->GetMaximum(); // The reference tag needs to be first so that the weights line up
      handle->Scale(scale);
      handle->Write();
      h.Close();
    }
  }

  f.Close();
  g.Close();
}
