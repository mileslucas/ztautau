#include <string>

using namespace std;

void cutFinder(){

  /* Import files */
  TFile *f = new TFile("emu_data.root");

  /* Get Trees */
  TTree *ztt = (TTree *) f->Get("ztt");
  TTree *gamma = (TTree *) f->Get("gamma");
  TTree *ww = (TTree *) f->Get("ww");
  TTree *tt = (TTree *) f->Get("tt");

  /* Choose histogram variables */
  const char* varnames[] = {
    "ptm1", // transverse momentum of muon 1
    "ete1", // transverse E of electron 1
    "met5", // total missing Et
    "nnej", // number of jets
    "etj1", // transverse energy of jet 1
    "memu",
    "mej1",
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
  string minCut[numvars];
  string maxCut[numvars];
    
  stringstream cutbuffer;  

  double csErrorMin = 100;
    double bestBias = .1;

for(int i = 0; i < numvars; i++) {
    
    int ratioMax = 0;
    for(int a = 0; a < 250; a++) {
      const char *cut = (string(varnames[i])+"<"+to_string(a)).c_str();
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

      

      if (ratio > (ratioMax + bestBias)){
	varRatioMax[i] = a;
    maxCut[i]= string(cut);
	ratioMax = ratio;
      }
      
    }
    ratioMax = 0;
    for(int b = 0; b < varRatioMax[i]; b++) {
      const char *cut = (string(varnames[i])+">"+to_string(b)).c_str();
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

      

      if (ratio > (ratioMax + bestBias)){
	varRatioMin[i] = b;
    minCut[i] = string(cut);
	ratioMax = ratio;
      }
      
    }
    /*
    printf("%d < %s < %d\n",varRatioMin[i], varnames[i], varRatioMax[i]);
    printf("%s && %s\n", minCut[i].c_str(), maxCut[i].c_str());
    cout << "(" << minCut[i] << ") && (" << maxCut[i] << ")" << endl;
    */
    cutbuffer << "(" << minCut[i] << ")&&(" << maxCut[i] << ")" << "&&";
  
  }
    string s = cutbuffer.str();
    cutbuffer.str(string());
    s = s.substr(0, s.size() - 2); 
    //cout << s << endl;
    /*
  TCut cut = "(ptm1<29)&&(ete1<32)&&(ete2<11)&& \
              (met5<22)&&(nnej<2)&&(etj1<43)&&  \
              (etj2<14)&&(memu>30)&&(memu<86)&& \
              (mej1<68)&&(mej2<34)";
*/
    TCut cut = s.c_str();

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

  printf("Num: %d %d %d %d\n", cutZtt, cutGamma, cutWW, cutTT);
  printf("ZTT: %f Gamma: %f WW: %f TT: %f\n", exp_ztt, exp_gamma, exp_ww, exp_tt);
  printf("Final Ratio: %f\n", ratio);
  
// Get actual entries
    int entries = ((TTree *) f->Get("data"))->GetEntries(cut);
   printf("Actual Entries: %d\n", entries);

    // calculate cross section
    double cs = (entries - (exp_gamma + exp_ww + exp_tt)) / (eff_ztt * L);
    printf("Cross Section: %f\n", cs);
}
