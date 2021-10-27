// Purpose: Fill jet physics analysis histograms
// Author:    mikko.voutilainen@cern.ch
// Co-author: hannu.siikonen@cern.ch
// Created: March 20, 2010

#include "CondFormats/JetMETObjects/src/Utilities.cc"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/SimpleJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
// For JEC uncertainty
#include "CondFormats/JetMETObjects/interface/SimpleJetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "settings.h"
#include "tools.h"
#include "HistosRun.h"
#include "HistosBasic.h"
#include "HistosEta.h"
#include "HistosMC.h"
#include "HistosAll.h"
#include "HistosFill.h"
#include "TROOT.h"

#ifdef __CLING__
R__LOAD_LIBRARY(CondFormats/JetMETObjects/src/JetCorrectorParameters_cc)
R__LOAD_LIBRARY(CondFormats/JetMETObjects/src/SimpleJetCorrector_cc)
R__LOAD_LIBRARY(CondFormats/JetMETObjects/src/FactorizedJetCorrector_cc)
R__LOAD_LIBRARY(CondFormats/JetMETObjects/src/SimpleJetCorrectionUncertainty_cc)
R__LOAD_LIBRARY(CondFormats/JetMETObjects/src/JetCorrectionUncertainty_cc)
#if JETRESO == 1
R__LOAD_LIBRARY(CondFormats/JetMETObjects/src/JetResolutionObject_cc)
R__LOAD_LIBRARY(JetMETCorrections/Modules/src/JetResolution_cc)
#endif
#ifdef USEASSERT
R__LOAD_LIBRARY(tools_C)
R__LOAD_LIBRARY(HistosRun_C)
R__LOAD_LIBRARY(HistosBasic_C)
R__LOAD_LIBRARY(HistosEta_C)
R__LOAD_LIBRARY(HistosMC_C)
R__LOAD_LIBRARY(HistosAll_C)
R__LOAD_LIBRARY(HistosFill_C)
#else
R__LOAD_LIBRARY(tools_C)
R__LOAD_LIBRARY(HistosRun_C)
R__LOAD_LIBRARY(HistosBasic_C)
R__LOAD_LIBRARY(HistosEta_C)
R__LOAD_LIBRARY(HistosMC_C)
R__LOAD_LIBRARY(HistosAll_C)
R__LOAD_LIBRARY(HistosFill_C)
#endif // USEASSERT or not?
#endif // CLING

void mk2_HistosFill() {
  bool fail = false;
  // Performing generic checks always, as a safety measure
  if (jp::triggers.size()!=jp::notrigs) {
    cout << "jp::trigger size mismatch" << endl;
    fail = true;
  }
  if (jp::trigthr.size()!=jp::notrigs) {
    cout << "jp::trigthr size mismatch" << endl;
    fail = true;
  }
  if (jp::trigranges.size()!=jp::notrigs) {
    cout << "jp::trigranges size mismatch" << endl;
    fail = true;
  }
  if (jp::triglumi.size()!=jp::notrigs) {
    cout << "jp::triglumi size mismatch" << endl;
    fail = true;
  }
  if (jp::triglumi.size()!=jp::notrigs) {
    cout << "jp::triglumi size mismatch" << endl;
    fail = true;
  }
  if (jp::IOVnames.size()!=jp::IOVranges.size()) {
    cout << "IOV count mismatch in jp::IOVnames and jp::IOVranges" << endl;
    fail = true;
  }
  if (jp::triglumiera.size()!=jp::dtfiles.size()) {
    cout << "Mismatch between data files and lumi era" << endl;
    fail = true;
  }
  for (auto &lumis : jp::triglumiera) {
    if (lumis.size()!=jp::notrigs) {
      cout << "jp::triglumiera size mismatch with triggers!" << endl;
      fail = true;
    }
  }
  if (jp::eras.size()<jp::dtfiles.size()) {
    cout << "Amount of eras not sufficient! (jp::eras)" << endl;
    fail = true;
  }
  if (jp::eras.size()<jp::dtfiles.size()) {
    cout << "Amount of eras not sufficient! (jp::eras)" << endl;
    fail = true;
  }
  // Check that pthat file dimensions are ok
  if (jp::pthatfiles.size()<jp::npthatbins) {
    cout << "The pthat info dimensions don't match! jp::pthatfiles vs jp::npthatbins" << endl;
    fail = true;
  } else if (jp::pthatsigmas.size()!=jp::npthatbins) {
    cout << "The pthat info dimensions don't match! jp::pthatsigmas vs jp::npthatbins" << endl;
    fail = true;
  } else if (jp::pthatranges.size()!=jp::npthatbins+1) {
    cout << "The pthat info dimensions don't match! jp::pthatranges vs jp::npthatbins" << endl;
    fail = true;
  }
  // Check that ht file dimensions are ok
  if (jp::htfiles.size()<jp::nhtbins) {
    cout << "The ht info dimensions don't match! jp::htfiles vs jp::nhtbins" << endl;
    fail = true;
  } else if (jp::htsigmas.size()!=jp::nhtbins) {
    cout << "The ht info dimensions don't match! jp::htsigmas vs jp::nhtbins" << endl;
    fail = true;
  } else if (jp::htranges.size()!=jp::nhtbins+1) {
    cout << "The ht info dimensions don't match! jp::htranges vs jp::nhtbins" << endl;
    fail = true;
  }
  if (fail) return;

  // Find out era index
  int eraIdx = -1;
  int eraNo = 0;
  for (auto &eraMatch : jp::eras) {
    if (jp::run==eraMatch) {
      eraIdx = eraNo;
      break;
    }
    ++eraNo;
  }
  if (eraIdx<0) {
    cout << "Era " << jp::run << " not found! Aborting!" << endl;
    return;
  }

  string algo = "ak4";
  if (jp::strings_equal(jp::algo,"AK8")) algo = "ak8";

  // connect trees
  TChain *c = new TChain(Form("%s/ProcessedTree",algo.c_str()));
  const char* p = jp::filepath;

  vector<const char*> files;
  if (jp::isdt) {
    cout << "Running over DT" << endl;
    cout << "Load trees..." << endl;

    if (eraIdx>=jp::dtfiles.size()) {
      cout << "Era index for " << jp::run << " does not match to a data file! Please retry!" << endl;
      return;
    }
    const char* ps = jp::dtpath;

    for (auto &fname : jp::dtfiles[eraIdx])
      files.push_back(Form("%s%s%s",p,ps,fname.c_str()));
  } else if (jp::ishtb) {
    cout << "Running over ht binned files in pythia8" << endl;
    cout << "Load trees..." << endl;

    string path = jp::htpath;
    if (jp::doMCFix and jp::yid==0) path += "_APV";

    for (auto &fname : jp::htfiles) {
      const char *name1 = Form("%s%s%s.root",p,path.c_str(),fname);
      const char *name2 = Form("%s%s%s_ext.root",p,path.c_str(),fname);
      std::ifstream stream1(name1);
      std::ifstream stream2(name2);
      if (stream1.good()) files.push_back(name1);
      if (stream2.good()) files.push_back(name2);
    }
  } else if (jp::ispthb) {
    cout << "Running over pthat binned files in pythia8" << endl;
    cout << "Load trees..." << endl;

    string path = jp::pthatpath;
    if (jp::doMCFix and jp::yid==0) path += "_APV";

    for (auto &fname : jp::pthatfiles) {
      const char *name1 = Form("%s%s%s.root",p,path.c_str(),fname);
      const char *name2 = Form("%s%s%s_ext.root",p,path.c_str(),fname);
      std::ifstream stream1(name1);
      std::ifstream stream2(name2);
      if (stream1.good()) files.push_back(name1);
      if (stream2.good()) files.push_back(name2);
    }
  } else if (jp::ispy or jp::ishw or jp::isnu) {
    string ftag = "";
    if (jp::ispy) {
      ftag = jp::p8file;
      if (jp::doMCFix) {
        if (jp::yid==0) ftag += "APV";
        else if (jp::yid==3) ftag += "HEM";
      }
      cout << "Running over pythia flat sample" << endl;
    } else if (jp::ishw) {
      ftag = jp::hwfile;
      if (jp::doMCFix and jp::yid==0) ftag += "APV";
      cout << "Running over Herwig flat sample" << endl;
    } else {
      ftag = jp::nufile;
      if (jp::doMCFix and jp::yid==0) ftag += "APV";
      else if (jp::yid==3) ftag += "HEM";
      cout << "Running over Single Neutrino sample" << endl;
    }

    for (const auto &fname : jp::mcfiles.at(ftag))
      files.push_back(Form("%s%s",p,fname));
  } else if (jp::ishw) {

    for (const auto &fname : jp::mcfiles.at(jp::hwfile)) files.push_back(Form("%s%s",p,fname));
  } else if (jp::isnu) {

    for (const auto &fname : jp::mcfiles.at(jp::nufile)) files.push_back(Form("%s%s",p,fname));
  } else {
    cout << "Enter a proper type!!" << endl;
    return;
  }

  if (files.size()==0) {
    if (jp::isdt) {
      cout << "Enter a proper value for run!" << endl;
      cout << "Entered: " << jp::run << endl << "Options:";
      for (auto &fname : jp::eras) cout << " " << fname;
      cout << endl;
    } else if (jp::ishtb) {
      cout << "Problems with ht file logic!" << endl;
    } else if (jp::ispthb) {
      cout << "Problems with pthat file logic!" << endl;
    } else {
      cout << "Unknown error!" << endl;
    }
    return;
  } else {
    cout << "Loading the files:" << endl;
    for (auto &fname : files) {
      cout << fname << endl;
      c->AddFile(fname);
    }
  }

  int centries = c->GetEntries();
  cout << "Got " << centries << " entries" << endl;

  if (centries > 0) {
    try {
      HistosFill filler(c,eraIdx);
      filler.Loop();
    } catch (const exception& e) {
      cout << e.what() << endl;
    }
  } else {
    cout << "Please specify files to be looped over!" << endl << "Exiting ..." << endl;
  }
  gROOT->ProcessLine(".q");
}
