#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TCut.h>
#include <omp.h>

using namespace std;

ofstream outf;

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
double eff_id = 0.755;
double branch_ratio = 0.0620;

double getRatio(int, int, int, int, bool);
double getCS(int, int, int, int, int);


int main(int argc, char* argv[]) {

        /* Open up output file */
        outf.open("cut.log");

        /* open up data file*/
        TFile *data_f = new TFile("data/emu_data.root");
        TFile *ztt_f = new TFile("data/emu_zttllcxxx_is722.root");
        TFile *gamma_f = new TFile("data/emu_dyemxmg25_py560.root");
        TFile *ww_f = new TFile("data/emu_wwllcxxxx_py057.root");
        TFile *tt_f = new TFile("data/emu_ttll170xs_hw05.root");


        /* Get Trees */
        TTree *ztt = (TTree *)ztt_f->Get("h10");
        TTree *gamma = (TTree *)gamma_f->Get("h10");
        TTree *ww = (TTree *)ww_f->Get("h10");
        TTree *tt = (TTree *)tt_f->Get("h10");

        /* Choose histogram variables */
        const char *varnames[] = {
                "met5",
                "ete1",
                "ptm1",
                "memu",
        };

        const char *overrides[] = {
                "(met4>20)&&(met4<31)",
                "(nnej<3)"
        };


        int numvars = sizeof(varnames) / sizeof(varnames[0]);
        int numoverrides = sizeof(overrides) / sizeof(overrides[0]);

        /* Initializing variables */
        int n_ztt;
        int n_gamma;
        int n_ww;
        int n_tt;
        double ratio;
        double max_ratio;
        string cut_str;
        string varCut[numvars];

        stringstream cutbuffer;

        outf << " ----- Grid Search Cut Finder -----" << endl;
        outf <<  "### INDIVIDUAL CYCLES ###" << endl;
        cout <<  "### INDIVIDUAL CYCLES ###" << endl;

        //#pragma omp parallel for
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
                                        varCut[i] = cut_str;
                                        max_ratio = ratio;
                                }
                        }
                }

                //cout << varCut[i] << endl;
                if (varCut[i] != "") {
                        cutbuffer << varCut[i] << "&&";
                        outf << varCut[i] << endl;
                }
                cout << "Finished: " << varnames[i] << "   [" << double(i+1)*100/numvars << "%]" << endl;
        }

        /* Take the combination of cuts and put them together */
        string s = cutbuffer.str();
        for(int i = 0; i < numoverrides; i++) {
          s += overrides[i];
          s += "&&";
          outf << overrides[i] << endl;
        }
        s = s.substr(0, s.size() - 2);
        // cout<< s << endl;
        TCut cut = s.c_str();

        n_ztt = ztt->GetEntries(cut);
        n_gamma = gamma->GetEntries(cut);
        n_ww = ww->GetEntries(cut);
        n_tt = tt->GetEntries(cut);

        getRatio(n_ztt, n_gamma, n_ww, n_tt, 1);

        /* Now we calculate the cross section */
        TTree *data = (TTree *) data_f->Get("h10");
        int count = data->GetEntries(cut);
        outf << "Actual count: " << count << endl;
        double cs = getCS(count, n_ztt, n_gamma, n_ww, n_tt);
        outf << "emu cross section: " << cs << endl;
        outf << "Z->TT cross section: " << cs / branch_ratio << endl;

        return 0;
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
                outf << endl <<
                "Entries\n\tZTT: " << n_ztt << " Gamma: " << n_gamma <<
                " WW: " << n_ww << " TT: " << n_tt << endl;
                outf << "Efficiencies\n\tZTT: " << eff_ztt << " Gamma: " << eff_gamma <<
                " WW: " << eff_ww << " TT: " << eff_tt << endl;
                outf << "Expected\n\tZTT: " << exp_ztt << " Gamma: " << exp_gamma <<
                " WW: " << exp_ww << " TT: " << exp_tt << endl;
                outf << "Final Ratio\n\t" << ratio << endl;
        }
        return ratio;
}

/* Returns the cross section of an experimental cut. Do not use willy-nilly */
double getCS(int count, int n_ztt, int n_gamma, int n_ww, int n_tt) {
        //correct count for identification efficiency
        count /= eff_id;
        // Get the efficiency (sig/total)
        double eff_ztt = n_ztt / N_ztt;
        double eff_gamma = n_gamma / N_gamma;
        double eff_ww = n_ww / N_ww;
        double eff_tt = n_tt / N_tt;

        // Get the expected values (luminosity * cross section * efficiency)
        double exp_gamma = eff_gamma * cs_gamma * L;
        double exp_ww = eff_ww * cs_ww * L;
        double exp_tt = eff_tt * cs_tt * L;

        return (double(count) - (exp_gamma + exp_ww + exp_tt)) / (eff_ztt * L);
}
