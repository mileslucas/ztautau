#include <string>

using namespace std;

void cutFinder() {

  /* Import files */
  TFile *ztt_f = new TFile("emu_zttllcxxx_is722.root");
  TFile *gamma_f = new TFile("emu_dyemxmg25_py560.root");
  TFile *ww_f = new TFile("emu_wwllcxxxx_py057.root");
  TFile *tt_f = new TFile("emu_ttll170xs_hw05.root");

  /* Get Trees */
  TTree *ztt = (TTree *)ztt_f->Get("h10");
  TTree *gamma = (TTree *)gamma_f->Get("h10");
  TTree *ww = (TTree *)ww_f->Get("h10");
  TTree *tt = (TTree *)tt_f->Get("h10");

  /* Choose histogram variables */
  const char *varnames[] = {
      "ptm1", // transverse momentum of muon 1
      "ete1", // transverse E of electron 1
      "ete2", // transverse E of electron 2
      "met4", // calorimeter missing Et
      "met5", // total missing Et
      "nnej", // number of jets
      "etj1", // transverse energy of jet 1
      "etj2", // transverse energy of jet 2
      "memu", "mej1", "mej2",
  };
  int numvars = sizeof(varnames) / sizeof(varnames[0]); /* Define Cuts */

  /* These are the variables from the monte carlo simulations */
  double N_ztt = 71393;
  double cs_ztt = 13.3;
  double N_gamma = 199990;
  double cs_gamma = 5.51;
  double N_ww = 101792;
  double cs_ww = 0.69;
  double N_tt = 101339;
  double cs_tt = 0.4;
  double L = 108.3;

  /* For each variable, go through a number of cuts
     and find the max sig/bkg ratio */
  int bestFloorCut[numvars];
  int bestCeilingCut[numvars];

  /* Initializing variables */
  const char *cut;
  int n_ztt;
  int n_gamma;
  int n_ww;
  int n_tt;
  double eff_ztt;
  double eff_gamma;
  double eff_ww;
  double eff_tt;
  double exp_ztt;
  double exp_gamma;
  double exp_ww;
  double exp_tt;
  double ratio;
  double max_ratio;

  /* Iterate through each variable */
  for (int i = 0; i < numvars; i++) {

    max_ratio = 0;
    for (int floor = 0; floor < 250; floor++) {
      for (int ceil = 0; ceil < 250; ceil++) {
        // Define cut using some c++ string mangling then getting its c string
        cut = ("(" + string(varnames[i]) + "<" + to_string(ceil) + ")&&(" +
               string(varnames[i]) + ">" + to_string(floor))
                  .c_str();

        // Get the number of entries after the cut
        n_ztt = ztt->GetEntries(cut);
        n_gamma = gamma->GetEntries(cut);
        n_ww = ww->GetEntries(cut);
        n_tt = tt->GetEntries(cut);

        // Find the efficiency (number after cut / number before cut)
        eff_ztt = n_ztt / N_ztt;
        eff_gamma = n_gamma / N_gamma;
        eff_ww = n_ww / N_ww;
        eff_tt = n_tt / N_tt;

        // Find the expected number of events (efficiency * cross section *
        // luminosity)
        exp_ztt = eff_ztt * cs_ztt * L;
        exp_gamma = eff_gamma * cs_gamma * L;
        exp_ww = eff_ww * cs_ww * L;
        exp_tt = eff_tt * cs_tt * L;

        // Get the ratio using s / Sqrt(s + q)
        ratio = exp_ztt / TMath::Sqrt(exp_ztt + exp_gamma + exp_ww + exp_tt);
        if (ratio > max_ratio) {
          bestCeilingCut[i] = ceil;
          bestFloorCut[i] = floor;
          max_ratio = ratio;
        }
      }
    }

    printf("%d < %s < %d\n", bestFloorCut[i], varnames[i], bestCeilingCut[i]);
  }

  n_ztt = ztt->GetEntries(cut);
  n_gamma = gamma->GetEntries(cut);
  n_ww = ww->GetEntries(cut);
  n_tt = tt->GetEntries(cut);

  eff_ztt = n_ztt / N_ztt;
  eff_gamma = n_gamma / N_gamma;
  eff_ww = n_ww / N_ww;
  eff_tt = n_tt / N_tt;

  exp_ztt = eff_ztt * cs_ztt * L;
  exp_gamma = eff_gamma * cs_gamma * L;
  exp_ww = eff_ww * cs_ww * L;
  exp_tt = eff_tt * cs_tt * L;

  ratio = exp_ztt / TMath::Sqrt(exp_ztt + exp_gamma + exp_ww + exp_tt);

  printf("ZTT: %f Gamma: %f WW: %f TT: %f\n", exp_ztt, exp_gamma, exp_ww,
         exp_tt);
  printf("Final Ratio: %f\n", ratio);
}
