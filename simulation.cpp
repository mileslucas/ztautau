/*
 * Miles Lucas
 * Phys 322L Spring 2017
 * mdlucas@iastate.edu
 */

#include <string>

using namespace std;

void simulation(){

        /* open up data file*/
        TFile *data_f = new TFile("emu_data.root");
        TFile *ztt_f = new TFile("emu_zttllcxxx_is722.root");
        TFile *gamma_f = new TFile("emu_dyemxmg25_py560.root");
        TFile *ww_f = new TFile("emu_wwllcxxxx_py057.root");
        TFile *tt_f = new TFile("emu_ttll170xs_hw05.root");
        TFile *qcd_f = new TFile("emu_qcd.root");

        /* Get Trees */
        TTree *ztt = (TTree *)ztt_f->Get("h10");
        TTree *gamma = (TTree *)gamma_f->Get("h10");
        TTree *ww = (TTree *)ww_f->Get("h10");
        TTree *tt = (TTree *)tt_f->Get("h10");
        TTree *qcd = (TTree *)qcd_f->Get("h10");

        /* Set up canvas */
        TCanvas *c1 = new TCanvas("c", "Simulation Hists", 1000, 1000);
        gStyle->SetOptStat(111111);
        c1->Divide(3,2); // two columns, two rows
        // Set up the labels for the histograms
        TPaveText *ztt_l = new TPaveText();
        ztt_l->AddText("Z->TT");
        TPaveText *gamma_l = new TPaveText();
        gamma_l->AddText("Gamma");
        TPaveText *ww_l = new TPaveText();
        ww_l->AddText("WW");
        TPaveText *tt_l = new TPaveText();
        tt_l->AddText("tt");
        TPaveText *qcd_l = new TPaveText();
        qcd_l->AddText("QCD");



        /* Choose histogram variables */
        const char* var = "ete1";

        /* Define Cuts */
        TCut cut = "(ptm1<29)&&(ete1<32)&&(ete2<11)&& \
              (met5<22)&&(nnej<2)&&(etj1<43)&&  \
              (etj2<14)&&(memu>11)&&(memu<86)&& \
              (mej1<68)&&(mej2<34)";
        TCut cut1 = "ete1<32";
        TCut cut2 = "ptm1<40";
        TCut cut3 = "met4<50";
        TCut cut4 = "met5<50";
        TCut cut5 = "nnej<=2";
        TCut cut6 = "etj1=0";
        TCut cut7 = "etj2=0";

        /* Draw histograms */
        c1->cd(1);
        ztt->Draw(var, cut);
        ztt_l->Draw();

        c1->cd(2);
        gamma->Draw(var, cut);
        gamma_l->Draw();

        c1->cd(3);
        ww->Draw(var, cut);
        ww_l->Draw();

        c1->cd(4);
        tt->Draw(var, cut);
        tt_l->Draw();

        c1->cd(5);
        qcd->Draw(var, cut);
        qcd_l->Draw();
}
