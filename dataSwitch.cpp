

void dataSwitch(){
  TFile *newFile = new TFile("emu_data_new.root", "RECREATE", "Tevatron Emu Data");

  /* Import files */
  TFile *data_f = new TFile("emu_data.root");
  TFile *ztt_f = new TFile("emu_zttllcxxx_is722.root");
  TFile *gamma_f = new TFile("emu_dyemxmg25_py560.root");
  TFile *ww_f = new TFile("emu_wwllcxxxx_py057.root");
  TFile *tt_f = new TFile("emu_ttll170xs_hw05.root");

  /* Get Trees */
  TTree *data = (TTree *) data_f->Get("h10");
  TTree *ztt = (TTree *) ztt_f->Get("h10");
  TTree *gamma = (TTree *) gamma_f->Get("h10");
  TTree *ww = (TTree *) ww_f->Get("h10");
  TTree *tt = (TTree *) tt_f->Get("h10");

  data->SetName("data"); data->SetTitle("Tevatron data set");
  ztt->SetName("ztt"); ztt->SetTitle("Monte carlo simulation Z->TT");
  gamma->SetName("gamma"); gamma->SetTitle("Monte carlo simulation PP->Gamma");
  ww->SetName("ww"); ww->SetTitle("Monte carlo simulation PP->WW");
  tt->SetName("tt"); tt->SetTitle("Monte carlo simulation PP->tt");
  
  newFile->cd();
  data->CloneTree()->Write("data");
  ztt->CloneTree()->Write("ztt");
  gamma->CloneTree()->Write("gamma");
  ww->CloneTree()->Write("ww");
  tt->CloneTree()->Write("tt");

  delete data_f;
  delete ztt_f;
  delete gamma_f;
  delete ww_f;
  delete tt_f;

}
