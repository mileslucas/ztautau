/*
 * Miles Lucas
 * Phys 322L Spring 2017
 * mdlucas@iastate.edu
 */

void data() {
  
  /* Open file and get the tree */
  TFile *f1 = new TFile("emu_data.root");
  TTree *t1 = (TTree *) f1->Get("h10");

  /* Set up the canvas */
  TCanvas *c1 = new TCanvas("c", "Data Hists", 1000, 1000);
  gStyle->SetOptStat(111111);

  /* Take cuts and plot data */
  TCut cut = "";

  c1->cd(1);
  t1->Draw("ete1", cut);

  /* Save the data as an eps */
  //c1->Print("data.eps");
}
