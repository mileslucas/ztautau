/*
 * Miles Lucas
 * Phys 322L Spring 2017
 * mdlucas@iastate.edu
 */

void simulation(){
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

  /* Set up canvas */
  TCanvas *c1 = new TCanvas("c", "Simulation Hists", 1000, 1000);
  gStyle->SetOptStat(111111);
  c1->Divide(2,2); // two columns, two rows
  
  /* Define Cuts */
  TCut cut1 = "ete1<50";
  TCut cut2 = "ptm1<50";
  
  /* Draw histograms */
  const char* var = "ptm1";
  
  c1->cd(1);
  ztt->Draw(var, cut2);
  
  c1->cd(2);
  gamma->Draw(var, cut2);

  c1->cd(3);
  ww->Draw(var, cut2);

  c1->cd(4);
  tt->Draw(var, cut2);

}
