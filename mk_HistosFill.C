// Purpose: Fill jet physics analysis histograms
// Author:    hannu.siikonen@cern.ch
// Created: October 27, 2021

// Only load macros from settings.h
#define STNGS
#include "settings.h"
#undef STNGS
{
  gROOT->ProcessLine(".L CondFormats/JetMETObjects/src/JetCorrectorParameters.cc+");
  gROOT->ProcessLine(".L CondFormats/JetMETObjects/src/SimpleJetCorrector.cc+");
  gROOT->ProcessLine(".L CondFormats/JetMETObjects/src/FactorizedJetCorrector.cc+");
  gROOT->ProcessLine(".L CondFormats/JetMETObjects/src/SimpleJetCorrectionUncertainty.cc+");
  gROOT->ProcessLine(".L CondFormats/JetMETObjects/src/JetCorrectionUncertainty.cc+");
  #if JETRESO == 1
  gROOT->ProcessLine(".L CondFormats/JetMETObjects/src/JetResolutionObject.cc+");
  gROOT->ProcessLine(".L JetMETCorrections/Modules/src/JetResolution.cc+");
  #endif
  #ifdef USEASSERT
  gROOT->ProcessLine(".L tools.C+g");
  gROOT->ProcessLine(".L HistosRun.C+g");
  gROOT->ProcessLine(".L HistosBasic.C+g");
  gROOT->ProcessLine(".L HistosEta.C+g");
  gROOT->ProcessLine(".L HistosMC.C+g");
  gROOT->ProcessLine(".L HistosAll.C+g");
  gROOT->ProcessLine(".L HistosFill.C+g");
  #else
  gROOT->ProcessLine(".L tools.C+");
  gROOT->ProcessLine(".L HistosRun.C+");
  gROOT->ProcessLine(".L HistosBasic.C+");
  gROOT->ProcessLine(".L HistosEta.C+");
  gROOT->ProcessLine(".L HistosMC.C+");
  gROOT->ProcessLine(".L HistosAll.C+");
  gROOT->ProcessLine(".L HistosFill.C+");
  #endif // USEASSERT or not?
  gROOT->ProcessLine(".x mk2_HistosFill.C");
}
