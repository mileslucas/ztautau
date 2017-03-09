/*
 * Miles Lucas
 * Phys 322L Spring 2017
 * mdlucas@iastate.edu
 */

#include <string>

using namespace std;

void simulation(){

  /* Create the directory to store the histograms in if it doesn't already exist */
  system("rm -r hists/*"); // clear histogram folder
  system("mkdir hists");
  
  /* Import file */
  TFile *f1 = new TFile("emu_data.root");;

  /* Get Trees */
  TTree *ztt = (TTree *) f1->Get("ztt");
  TTree *gamma = (TTree *) f1->Get("gamma");
  TTree *ww = (TTree *) f1->Get("ww");
  TTree *tt = (TTree *) f1->Get("tt");

  /* Set up canvas */
  TCanvas *c1 = new TCanvas("c", "Simulation Hists", 1000, 1000);
  gStyle->SetOptStat(111111);
  c1->Divide(2,2); // two columns, two rows
  // Set up the labels for the histograms
  TPaveText *ztt_l = new TPaveText();
  ztt_l->AddText("Z->TT");
  TPaveText *gamma_l = new TPaveText();
  gamma_l->AddText("Gamma");
  TPaveText *ww_l = new TPaveText();
  ww_l->AddText("WW");
  TPaveText *tt_l = new TPaveText();
  tt_l->AddText("tt");
  
 
  
  /* Choose histogram variables */
  const char* varnames[] = {
    "ptm1", // transverse momentum of muon 1
    "ete1", // transverse E of electron 1
    //"ete2", // transverse E of electron 2
    "met4", // calorimeter missing Et
    "met5", // total missing Et
    "nnej", // number of jets
    "etj1", // transverse energy of jet 1
    "etj2", // transverse energy of jet 2
    "memu"
  };
  int numvars = sizeof(varnames)/sizeof(varnames[0]);

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
  for(int i = 1; i < 2; i++) { 
    const char* var = varnames[i];
    const char* filename = ("hists/" + string(var) + ".eps").c_str();

    // Each iteration will navigate to each pad, draw the histogram, and add a label for the dataset
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

    // This will print each set into a directory called hists
    c1->Print(filename);
  }
  // This will close the window after all the iterations have been made
  //c1->Close();  
}
