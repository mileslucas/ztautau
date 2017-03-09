#include <string>

using namespace std;

void cutFinder(){

  /* Import files */
  TFile *ztt_f = new TFile("emu_zttllcxxx_is722.root");
  TFile *gamma_f = new TFile("emu_dyemxmg25_py560.root");
  TFile *ww_f = new TFile("emu_wwllcxxxx_py057.root");
  TFile *tt_f = new TFile("emu_ttll170xs_hw05.root");

  /* Get Trees */
  TTree *ztt = (TTree *) ztt_f->Get("h10");
  TTree *gamma = (TTree *) gamma_f->Get("h10");
  TTree *ww = (TTree *) ww_f->Get("h10");
  TTree *tt = (TTree *) tt_f->Get("h10");

  /* Choose histogram variables */
  const char* varnames[] = {
    "ptm1", // transverse momentum of muon 1
    "ete1", // transverse E of electron 1
    "ete2", // transverse E of electron 2
    //"met4", // calorimeter missing Et
    "met5", // total missing Et
    "nnej", // number of jets
    "etj1", // transverse energy of jet 1
    "etj2", // transverse energy of jet 2
    "memu",
    "mej1",
    "mej2",
  };
  int numvars = sizeof(varnames)/sizeof(varnames[0]);/* Define Cuts */

  int nztt, ngamma, nww, ntt;
  nztt = ngamma = nww = ntt = 0;

  double Nztt = 71393; double sztt = 13.3;
  double Ngamma = 199990; double sgamma = 5.51;
  double Nww = 101792; double sww = 0.69;
  double Ntt = 101339; double stt = 0.4;
  
  double L = 108.3;
  
  /* For each variable, go through a number of cuts
     and find the max sig/bkg ratio */
  int varRatioMax[numvars];
  int varRatioMin[numvars];
  
  for(int i = 0; i < numvars; i++) {
    
    int ratioMax = 0;
    for(int j = 0; j < 250; j++) {
      const char *cut = (string(varnames[i])+"<"+to_string(j)).c_str();
      int cutZtt = ztt->GetEntries(cut);
      int cutGamma = gamma->GetEntries(cut);
      int cutWW = ww->GetEntries(cut);
      int cutTT = tt->GetEntries(cut);
      
      double eff_ztt = cutZtt / Nztt;
      double eff_gamma = cutGamma / Ngamma;
      double eff_ww = cutWW / Nww;
      double eff_tt = cutTT / Ntt;

      double exp_ztt = eff_ztt * sztt * L;
      double exp_gamma = eff_gamma * sgamma * L;
      double exp_ww = eff_ww * sww * L;
      double exp_tt = eff_tt * stt * L;

      double ratio = exp_ztt / (exp_gamma + exp_ww + exp_tt);

      

      if (ratio > ratioMax){
	varRatioMax[i] = j;
	ratioMax = ratio;
      }
      
    }
    ratioMax = 0;
    for(int j = 250; j >= 0; j--) {
      const char *cut = (string(varnames[i])+">"+to_string(j)).c_str();
      int cutZtt = ztt->GetEntries(cut);
      int cutGamma = gamma->GetEntries(cut);
      int cutWW = ww->GetEntries(cut);
      int cutTT = tt->GetEntries(cut);
      
      double eff_ztt = cutZtt / Nztt;
      double eff_gamma = cutGamma / Ngamma;
      double eff_ww = cutWW / Nww;
      double eff_tt = cutTT / Ntt;

      double exp_ztt = eff_ztt * sztt * L;
      double exp_gamma = eff_gamma * sgamma * L;
      double exp_ww = eff_ww * sww * L;
      double exp_tt = eff_tt * stt * L;

      double ratio = exp_ztt / (exp_gamma + exp_ww + exp_tt);

      

      if (ratio > ratioMax){
	varRatioMin[i] = j;
	ratioMax = ratio;
      }
      
    }

    printf("%d < %s < %d\n",varRatioMin[i], varnames[i], varRatioMax[i]);
  
  }

  TCut cut = "(ptm1<29)&&(ete1<32)&&(ete2<11)&& \
              (met5<22)&&(nnej<2)&&(etj1<43)&&  \
              (etj2<14)&&(memu>11)&&(memu<86)&& \
              (mej1<68)&&(mej2<34)";

  int cutZtt = ztt->GetEntries(cut);
  int cutGamma = gamma->GetEntries(cut);
  int cutWW = ww->GetEntries(cut);
  int cutTT = tt->GetEntries(cut);
      
  double eff_ztt = cutZtt / Nztt;
  double eff_gamma = cutGamma / Ngamma;
  double eff_ww = cutWW / Nww;
  double eff_tt = cutTT / Ntt;

  double exp_ztt = eff_ztt * sztt * L;
  double exp_gamma = eff_gamma * sgamma * L;
  double exp_ww = eff_ww * sww * L;
  double exp_tt = eff_tt * stt * L;

  double ratio = exp_ztt / (exp_gamma + exp_ww + exp_tt);

  printf("Eff: %f\n", eff_ztt);
  printf("ZTT: %f Gamma: %f WW: %f TT: %f\n", exp_ztt, exp_gamma, exp_ww, exp_tt);
  printf("Final Ratio: %f\n", ratio);
  
}
