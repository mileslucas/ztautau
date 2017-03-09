/*
 * Miles Lucas
 * Phys 322L Spring 2017
 * mdlucas@iastate.edu
 */

void data() {

  /* Make hists directory if it does not already exist */
  system("mkdir hists");
  
  /* Open file and get the tree */
  TFile *f1 = new TFile("emu_data.root");
  TTree *t1 = (TTree *) f1->Get("data");

  /* Set up the canvas */
  TCanvas *c1 = new TCanvas("c", "Data Hists", 1000, 1000);
  gStyle->SetOptStat(111111);

  /* Take cuts and plot data */
  TCut cut = "(ptm1<29)&&(ete1<32)&&(ete2<11)&& \
              (met5<22)&&(nnej<2)&&(etj1<43)&&  \
              (etj2<14)&&(memu>30)&&(memu<50)&& \
              (mej1<68)&&(mej2<34)";

  c1->cd(1);
  t1->Draw("ete1", cut);

  /* Save the data as an eps */
  //c1->Print("hists/data.eps");
}
