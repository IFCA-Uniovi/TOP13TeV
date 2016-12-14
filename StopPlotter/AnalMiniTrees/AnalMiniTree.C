#include "AnalMiniTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void AnalMiniTree::Loop(TString plot){

	TString htitle; Int_t nbins; Float_t binmin = 0; Float_t binmax = 0;
	if (fChain == 0) return;
	Long64_t nentries = fChain->GetEntriesFast();
  //############ SRs ############
  if(plot == "SRMT23D"){
    htitle = "SR_MT2_3D";
    nbins = 3*3*3;
    histo = new Histo(theSample + "_" + htitle, theSample + "_" + htitle, 27, -0.5, 27-0.5);
    histo->SetTag(theSample, "SRs", "SR (MT2 3D)", "");
  }
  if(plot == "SR"){
    htitle = "SR";
    nbins = 37;
    histo = new Histo(theSample + "_" + htitle, theSample + "_" + htitle, 37, -0.5, 37-0.5);
    histo->SetTag(theSample, "SRs", "SR", "");
  }
  //############ Variables ############
 else if (plot == "MET"){
    htitle = "MET";
    nbins = 17;
    float xbins[] = {0, 50, 80, 110, 140, 170, 200, 230, 260, 290, 320, 350, 400, 450, 500, 550, 600}; 
    histo = new Histo(theSample + "_" + htitle, theSample + "_" + htitle, nbins-1, xbins);
    histo->SetTag(theSample, "MET", "MET [GeV]", "e#mu, M_{ll} #geq 20 GeV, #geq 2jets, #geq 1btag");
  }
 else if (plot == "HT"){
    htitle = "HT";
    //nbins = 17;
    //float xbins[] = {0, 60, 100, , 140, 170, 200, 230, 260, 290, 320, 350, 400, 450, 500, 550, 600}; 
    //histo = new Histo(theSample + "_" + htitle, theSample + "_" + htitle, nbins-1, xbins);
    histo = new Histo(theSample + "_" + htitle, theSample + "_" + htitle, 23, 0, 1380);
    histo->SetTag(theSample, "HT", "HT [GeV]", "e#mu, M_{ll} #geq 20 GeV, #geq 2jets, #geq 1btag");
  }
	else if(plot == "MT2"){ 
    htitle = "MT2";
    nbins = 9;
    float xbins[] = {0, 20, 40, 50, 100, 140, 200, 300, 400}; 
    histo = new Histo(theSample + "_" + htitle, theSample + "_" + htitle, nbins-1, xbins);
  }
	else if(plot == "METSR"){ 
    htitle = "MET";
    nbins = 9;
    float xbins[] = {0, 20, 40, 50, 100, 140, 200, 300, 400}; 
    histo = new Histo(theSample + "_" + htitle, theSample + "_" + htitle, nbins-1, xbins);
  }
  else if(plot == "DeltaPhi"){
    htitle = "DeltaPhi";
    nbins = 20;
    histo = new Histo(theSample + "_" + htitle, theSample + "_" + htitle, 20, -3.2, 3.2);
  }
  else if(plot == "DeltaEta"){
    htitle = "DeltaEta";
    histo = new Histo(theSample + "_" + htitle, theSample + "_" + htitle, 15, 0, 3.2);
  }
  else if(plot == "DeltaR"){
    htitle = "DeltaR";
    histo = new Histo(theSample + "_" + htitle, theSample + "_" + htitle, 15, 0, 8);
  }
  else               { std::cout << " -------> Wrong plot name!! " << endl; return;}
  TLorentzVector l0;
  TLorentzVector l1;
  TLorentzVector jet0;
  TLorentzVector jet1;


	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		// if (Cut(ientry) < 0) continue;

    Float_t lb = TMT2lblb; Float_t bb = TMT2bb; Float_t ll = TMT2ll;
    l0.SetPxPyPzE(TLep1_Px, TLep1_Py, TLep1_Pz, TLep1_E);
    l1.SetPxPyPzE(TLep2_Px, TLep2_Py, TLep2_Pz, TLep2_E);
    jet0.SetPxPyPzE(TJet_Px[0], TJet_Py[0], TJet_Pz[0], TJet_E[0]);
    jet1.SetPxPyPzE(TJet_Px[1], TJet_Py[1], TJet_Pz[1], TJet_E[1]);
    Float_t dEta = TMath::Abs(l0.Eta() - l1.Eta());
    Float_t dPhi = TMath::Abs(l0.DeltaPhi(l1));

		if(chan == "ElMu" && !TIsElMu) continue;
		if(chan == "Elec" && !TIsDoubleElec) continue;
		if(chan == "Muon" && !TIsDoubleMuon) continue;
		if( (chan == "SF" || chan == "sameF" || chan == "SameF") && (!TIsDoubleMuon || !TIsDoubleElec)) continue;

		//############ SRs ############
		if     (plot == "SRMT23D"){
			if(ll < 100){
          if     (lb<100 && bb<170)               histo->Fill(1, TWeight);
          else if(lb<100 && bb>170 && bb<270)     histo->Fill(2, TWeight);
          else if(lb<100 && bb>270)               histo->Fill(3, TWeight);
          else if(lb<200 && lb>100 && bb<170)     histo->Fill(4, TWeight);
					else if(lb<200 && lb>100 && bb<270)     histo->Fill(5, TWeight);
					else if(lb<200 && lb>100 && bb>270)     histo->Fill(6, TWeight);
					else if(lb>200 && bb<170)               histo->Fill(7, TWeight);
					else if(lb>200 && bb>170 && bb<270)     histo->Fill(8, TWeight);
					else if(lb>200 && bb>270)               histo->Fill(9, TWeight);
			}

			else if(ll > 100 && ll < 200){ 
          if     (lb<100 && bb<170)               histo->Fill(10, TWeight);
          else if(lb<100 && bb>170 && bb<270)     histo->Fill(11, TWeight);
          else if(lb<100 && bb>270)               histo->Fill(12, TWeight);
          else if(lb<200 && lb>100 && bb<170)     histo->Fill(13, TWeight);
					else if(lb<200 && lb>100 && bb<270)     histo->Fill(14, TWeight);
					else if(lb<200 && lb>100 && bb>270)     histo->Fill(15, TWeight);
					else if(lb>200 && bb<170)               histo->Fill(16, TWeight);
					else if(lb>200 && bb>170 && bb<270)     histo->Fill(17, TWeight);
					else if(lb>200 && bb>270)               histo->Fill(18, TWeight);
			}
			else if(ll  > 200){ 
          if     (lb<100 && bb<170)               histo->Fill(19, TWeight);
          else if(lb<100 && bb>170 && bb<270)     histo->Fill(20, TWeight);
          else if(lb<100 && bb>270)               histo->Fill(21, TWeight);
          else if(lb<200 && lb>100 && bb<170)     histo->Fill(22, TWeight);
					else if(lb<200 && lb>100 && bb<270)     histo->Fill(23, TWeight);
					else if(lb<200 && lb>100 && bb>270)     histo->Fill(24, TWeight);
					else if(lb>200 && bb<170)               histo->Fill(25, TWeight);
					else if(lb>200 && bb>170 && bb<270)     histo->Fill(26, TWeight);
					else if(lb>200 && bb>270)               histo->Fill(27, TWeight);
			}
		}

		else if(plot == "SR"){ 
			if(TMET > 50 && TNJetsBtag > 0 && ((chan == "ElMu") || ( (TMET/TMath::Sqrt(THT)) > 5 && TMinDPhiMetJets > 0.25 ) )){


				if     (dEta < 0.5 && dPhi < 1 && TMET < 100) histo->Fill(1,TWeight);
				if     (dEta < 0.5 && dPhi < 1 && TMET > 100 && TMET < 150) histo->Fill(2,TWeight); 
				if     (dEta < 0.5 && dPhi < 1 && TMET > 150 && TMET < 200) histo->Fill(3,TWeight); 
				if     (dEta < 0.5 && dPhi < 1 && TMET > 200 && TMET < 300) histo->Fill(4,TWeight); 
				if     (dEta < 0.5 && dPhi < 1 && TMET > 300) histo->Fill(5,TWeight); 

				else if(dEta < 0.5 && dPhi > 1 && dPhi < 2.5 && TMET < 100) histo->Fill(6,TWeight); 
				else if(dEta < 0.5 && dPhi > 1 && dPhi < 2.5 && TMET > 100 && TMET < 150) histo->Fill(7,TWeight); 
				else if(dEta < 0.5 && dPhi > 1 && dPhi < 2.5 && TMET > 150 && TMET < 200) histo->Fill(8,TWeight); 
				else if(dEta < 0.5 && dPhi > 1 && dPhi < 2.5 && TMET > 200 && TMET < 300) histo->Fill(9,TWeight); 
				else if(dEta < 0.5 && dPhi > 1 && dPhi < 2.5 && TMET > 300) histo->Fill(10,TWeight); 

				else if(dEta < 0.5 && dPhi > 2.5 && TMET < 100) histo->Fill(11,TWeight); 
				else if(dEta < 0.5 && dPhi > 2.5 && TMET > 100 && TMET < 150) histo->Fill(12,TWeight); 
				else if(dEta < 0.5 && dPhi > 2.5 && TMET > 150 && TMET < 200) histo->Fill(13,TWeight); 
				else if(dEta < 0.5 && dPhi > 2.5 && TMET > 200 && TMET < 300) histo->Fill(14,TWeight); 
				else if(dEta < 0.5 && dPhi > 2.5 && TMET > 300) histo->Fill(15,TWeight); 

				else if(dEta > 0.5 && dEta<1 && dPhi < 1 && TMET < 100) histo->Fill(16,TWeight); 
				else if(dEta > 0.5 && dEta<1 && dPhi < 1 && TMET > 100 && TMET < 150) histo->Fill(17,TWeight); 
				else if(dEta > 0.5 && dEta<1 && dPhi < 1 && TMET > 150 && TMET < 200) histo->Fill(18,TWeight); 
				else if(dEta > 0.5 && dEta<1 && dPhi < 1 && TMET > 200 && TMET < 300) histo->Fill(19,TWeight); 
				else if(dEta > 0.5 && dEta<1 && dPhi < 1 && TMET > 300) histo->Fill(20,TWeight); 

				else if(dEta > 0.5 && dEta<1 && dPhi > 1 && dPhi < 2.5 && TMET < 100) histo->Fill(21,TWeight); 
				else if(dEta > 0.5 && dEta<1 && dPhi > 1 && dPhi < 2.5 && TMET > 100 && TMET < 150) histo->Fill(22,TWeight); 
				else if(dEta > 0.5 && dEta<1 && dPhi > 1 && dPhi < 2.5 && TMET > 150 && TMET < 200) histo->Fill(23,TWeight); 
				else if(dEta > 0.5 && dEta<1 && dPhi > 1 && dPhi < 2.5 && TMET > 200 && TMET < 300) histo->Fill(24,TWeight); 
				else if(dEta > 0.5 && dEta<1 && dPhi > 1 && dPhi < 2.5 && TMET > 300) histo->Fill(25,TWeight); 

				else if(dEta > 0.5 && dEta<1 && dPhi > 2.5 && TMET < 100) histo->Fill(26,TWeight); 
				else if(dEta > 0.5 && dEta<1 && dPhi > 2.5 && TMET > 100 && TMET < 150) histo->Fill(27,TWeight); 
				else if(dEta > 0.5 && dEta<1 && dPhi > 2.5 && TMET > 150 && TMET < 200) histo->Fill(28,TWeight); 
				else if(dEta > 0.5 && dEta<1 && dPhi > 2.5 && TMET > 200 && TMET < 300) histo->Fill(29,TWeight); 
				else if(dEta > 0.5 && dEta<1 && dPhi > 2.5 && TMET > 300) histo->Fill(30,TWeight); 

        // MET > 100, MET < 100
				else if(dEta > 1 && dEta<2 && dPhi < 1 && TMET < 100)  histo->Fill(31,TWeight); 
				else if(dEta > 1 && dEta<2 && dPhi < 1 && TMET > 100)  histo->Fill(32,TWeight); 
				else if(dEta > 1 && dEta<2 && dPhi > 1 && dPhi < 2.5 && TMET < 100)  histo->Fill(33,TWeight); 
				else if(dEta > 1 && dEta<2 && dPhi > 1 && dPhi > 2.5 && TMET > 100)  histo->Fill(34,TWeight); 
				else if(dEta > 1 && dEta<2 && dPhi > 2.5 && TMET < 100) histo->Fill(35,TWeight); 
				else if(dEta > 1 && dEta<2 && dPhi > 2.5 && TMET > 100) histo->Fill(36,TWeight); 
				else if(dEta > 2) histo->Fill(37,TWeight); 
			}
		}


		//############ Variables ############
		else if(plot == "MET"){
			if(TMET > 50 && TNJetsBtag > 0 && ((chan == "ElMu") || ( (TMET/TMath::Sqrt(THT)) > 5 && TMinDPhiMetJets > 0.25 ) ))
				histo->Fill(TMET, TWeight);
		}
		else if(plot == "MT2"){
      if(TMET > 50 && TNJetsBtag > 0 && ((chan == "ElMu") || ( (TMET/TMath::Sqrt(THT)) > 5 && TMinDPhiMetJets > 0.25 ) ))
				histo->Fill(TMT2ll, TWeight);
		}
    else if(plot == "DeltaPhi"){
      if(TMET > 50 && TNJetsBtag > 0 && ((chan == "ElMu") || ( (TMET/TMath::Sqrt(THT)) > 5 && TMinDPhiMetJets > 0.25 ) ))
        histo->Fill(l0.DeltaPhi(l1), TWeight);
    }
    else if(plot == "DeltaEta"){
      if(TMET > 50 && TNJetsBtag > 0 && ((chan == "ElMu") || ( (TMET/TMath::Sqrt(THT)) > 5 && TMinDPhiMetJets > 0.25 ) ))
        histo->Fill(TMath::Abs(l0.Eta() - l1.Eta()), TWeight);
    }
    else if(plot == "DeltaR"){
      if(TMET > 50 && TNJetsBtag > 0 && ((chan == "ElMu") || ( (TMET/TMath::Sqrt(THT)) > 5 && TMinDPhiMetJets > 0.25 ) ))
        histo->Fill(l0.DeltaR(l1), TWeight);
    }
    else if(plot == "HT"){
      if(TMET > 50 && TNJetsBtag > 0 && ((chan == "ElMu") || ( (TMET/TMath::Sqrt(THT)) > 5 && TMinDPhiMetJets > 0.25 ) ))
        histo->Fill(THT, TWeight);
    }
	}
  histo->SetStyle();
}