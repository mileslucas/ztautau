#include <string>

using namespace std;

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

double getRatio(int, int, int, int, bool);

void cutFinder() {

        /* Import files */
        TFile *f = new TFile("emu_data.root");

        /* Get Trees */
        TTree *ztt = (TTree *)f->Get("ztt");
        TTree *gamma = (TTree *)f->Get("gamma");
        TTree *ww = (TTree *)f->Get("ww");
        TTree *tt = (TTree *)f->Get("tt");

        /* Choose histogram variables */
        const char *varnames[] = {"ptm1", // transverse momentum of muon 1
                                  "ete1", // transverse E of electron 1
                                  "met4",
                                  "met5", // total missing Et
                                  "nnej", // number of jets
                                  "etj1", // transverse energy of jet 1
                                  "memu", "mej1"};
        int numvars = sizeof(varnames) / sizeof(varnames[0]);

        /* For each variable, go through a number of cuts
           and find the max sig/bkg ratio */
        int bestFloorCut[numvars];
        int bestCeilingCut[numvars];

        /* Initializing variables */
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
        string varCut[numvars];

        stringstream cutbuffer;

        string cut_str;
        cout << "### INDIVIDUAL CYCLES ###" << endl;

        for (int i = 0; i < numvars; i++) {
                max_ratio = 0;
                for (int ceil = 0; ceil < 100; ceil++) {
                        for (int floor = 0; floor < ceil; floor++) {
                                // ignore cuts that are within 1 of each other due to clobbering
                                // also ignore cuts that are not real (floor higher than ceiling)
                                if ((ceil - floor) < 2)
                                        continue;

                                // Define cut using some c++ string mangling then getting its c string
                                cut_str = "(" + string(varnames[i]) + ">" + to_string(floor) + ")&&(" +
                                          string(varnames[i]) + "<" + to_string(ceil) + ")";
                                const char *cut = cut_str.c_str();

                                // Get the number of entries after the cut
                                n_ztt = ztt->GetEntries(cut);
                                n_gamma = gamma->GetEntries(cut);
                                n_ww = ww->GetEntries(cut);
                                n_tt = tt->GetEntries(cut);

                                // Get the ratio for the cuts
                                ratio = getRatio(n_ztt, n_gamma, n_ww, n_tt, 0);

                                if (ratio > max_ratio) {
                                        bestCeilingCut[i] = ceil;
                                        bestFloorCut[i] = floor;
                                        varCut[i] = cut_str;
                                        max_ratio = ratio;
                                }
                        }
                }

                //cout << varCut[i] << endl;
                if (varCut[i] != "") {
                        cutbuffer << varCut[i] << "&&";
                }
        }

        /* Take the combination of cuts and put them together */

        string s = cutbuffer.str();
        s = s.substr(0, s.size() - 2);
        cout << s << endl;
        TCut cut = s.c_str();

        n_ztt = ztt->GetEntries(cut);
        n_gamma = gamma->GetEntries(cut);
        n_ww = ww->GetEntries(cut);
        n_tt = tt->GetEntries(cut);

        ratio = getRatio(n_ztt, n_gamma, n_ww, n_tt, 1);

        cutbuffer.str(string());
        stringstream tmpCutBuffer;


        cout << endl << "### COMBINED CYCLES ###" << endl;
        /* Now we must take this cut and tweak the values while considering all the
           other
           cuts */
        for (int i = 0; i < numvars; i++) {
                max_ratio = 0;
                for (int ceil = 0; ceil < 100; ceil++) {
                        for (int floor = 0; floor < ceil; floor++) {
                                tmpCutBuffer.str(string());
                                // ignore cuts that are within 1 of each other due to clobbering
                                // also ignore cuts that are not real (floor higher than ceiling)
                                if ((ceil - floor) < 2)
                                        continue;

                                // Define cut using some c++ string mangling then getting its c string
                                cut_str = "(" + string(varnames[i]) + ">" + to_string(floor) + ")&&(" +
                                          string(varnames[i]) + "<" + to_string(ceil) + ")";
                                // Now  we have to replace it within the origin cut
                                for(int q = 0; q < numvars; q++) {
                                        if (q == i) tmpCutBuffer << cut_str << "&&";
                                        else tmpCutBuffer << varCut[q] << "&&";
                                }
                                string s = tmpCutBuffer.str();
                                s = s.substr(0, s.size() - 2);
                                //cout << s << endl;
                                TCut cut = s.c_str();

                                // Get the number of entries after the cut
                                n_ztt = ztt->GetEntries(cut);
                                n_gamma = gamma->GetEntries(cut);
                                n_ww = ww->GetEntries(cut);
                                n_tt = tt->GetEntries(cut);

                                ratio = getRatio(n_ztt, n_gamma, n_ww, n_tt, 0);
                                if (ratio > max_ratio) {
                                        bestCeilingCut[i] = ceil;
                                        bestFloorCut[i] = floor;
                                        varCut[i] = cut_str;
                                        max_ratio = ratio;
                                }
                        }
                }

                //cout << varCut[i] << endl;
                if (varCut[i] != "") {
                        cutbuffer << varCut[i] << "&&";
                }
        }

        string new_s = cutbuffer.str();
        new_s = new_s.substr(0, new_s.size() - 2);
        cout << new_s << endl;
        TCut cut_revision = new_s.c_str();

        n_ztt = ztt->GetEntries(cut_revision);
        n_gamma = gamma->GetEntries(cut_revision);
        n_ww = ww->GetEntries(cut_revision);
        n_tt = tt->GetEntries(cut_revision);

        ratio = getRatio(n_ztt, n_gamma, n_ww, n_tt, 1);

        /* Now we calculate the cross section */
}

/* This returns the ratio of s / sqrt(s+q) for the given number of entries */
double getRatio(int n_ztt, int n_gamma, int n_ww, int n_tt, bool verbose) {
        // Get the efficiency (sig/total)
        double eff_ztt = n_ztt / N_ztt;
        double eff_gamma = n_gamma / N_gamma;
        double eff_ww = n_ww / N_ww;
        double eff_tt = n_tt / N_tt;

        // Get the expected values (luminosity * cross section * efficiency)
        double exp_ztt = eff_ztt * cs_ztt * L;
        double exp_gamma = eff_gamma * cs_gamma * L;
        double exp_ww = eff_ww * cs_ww * L;
        double exp_tt = eff_tt * cs_tt * L;

        double ratio = exp_ztt / TMath::Sqrt(exp_tt + exp_gamma + exp_ww + exp_tt);
        if (verbose) {
                printf("Entries-\n\tZTT: %9d Gamma: %9d WW: %9d TT: %9d\n", n_ztt, n_gamma,
                       n_ww, n_tt);
                printf("Efficiencies-\n\tZTT: %9f Gamma: %9f WW: %9f TT: %9f\n", eff_ztt,
                       eff_gamma, eff_ww, eff_tt);
                printf("Expected-\n\tZTT: %9f Gamma: %9f WW: %9f TT: %9f\n", exp_ztt,
                       exp_gamma, exp_ww, exp_tt);
                printf("Final Ratio: %9f\n", ratio);
        }
        return ratio;
}
