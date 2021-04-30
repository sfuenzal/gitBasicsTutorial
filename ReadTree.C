#include <iostream>
#include <TTree.h>
#include <TFile.h>
#include <TLorentzVector.h>

void ReadTree()
{
	TFile *f=new TFile("/user/e/edson/public/exp_phys/events.root","READ");
    	TTree *t=(TTree*)f->Get("LHEF");
    	//t->Print();
        
	//Need this to correctly read TTree branches
    	t->SetMakeClass(1);
    	t->SetBranchStatus("*", 0);

    	//Define local variables to store tree branches 
    	Int_t Particle_;
    	t->SetBranchStatus("Particle", 1);
    	t->SetBranchAddress("Particle",&Particle_);
    	const Int_t MaxNParticles=50;
    	Double_t E[MaxNParticles];
    	Double_t Pt[MaxNParticles];
   	Double_t Eta[MaxNParticles];
    	Double_t Phi[MaxNParticles];
    	Int_t PID[MaxNParticles];
        Int_t Mother1[MaxNParticles];         

    	//Read TTree Branches
    	t->SetBranchStatus("Particle.E",1);
    	t->SetBranchAddress("Particle.E",&E);
    	t->SetBranchStatus("Particle.PT",1);
    	t->SetBranchAddress("Particle.PT",&Pt);
    	t->SetBranchStatus("Particle.Eta",1);
    	t->SetBranchAddress("Particle.Eta",&Eta);
    	t->SetBranchStatus("Particle.Phi",1);
    	t->SetBranchAddress("Particle.Phi",&Phi);
    	t->SetBranchStatus("Particle.PID",1);
    	t->SetBranchAddress("Particle.PID",&PID);
        t->SetBranchStatus("Particle.Mother1",1);
        t->SetBranchAddress("Particle.Mother1",&Mother1);        
         
        //A invariant mass Histogram                                                                                                               
    	TH1F *h    = new TH1F("h","Invariant Mass; M [GeV]; Events",50,80,100);
    	//A canvas to draw it                                                                                                                      
    	TCanvas *c = new TCanvas("c","",800,600);

    	//Loop over events
    	for(Long64_t i = 0; i < t->GetEntries(); i++) 
	{
      		t->GetEntry(i);
      		std::cout << "This is event: " << i << std::endl;
		std::cout << i << " : " << Particle_ << " :";
      		TLorentzVector lepton (0,0,0,0); 
		//Loop over particles
      		for (Int_t j = 0; j < Particle_; j++) 
		{
         		//std::cout << "E: " << E[j];
         		//std::cout << "Pt: " << Pt[j];
         		//std::cout << "Eta: " << Eta[j];
         		//std::cout << "Phi: " << Phi[j];
			//std::cout << "Pid: " << PID[j];
			
			if (Mother1[j] != -1){
			std::cout << "Mother: " << Mother1[j];}
			
			 //Particles are named according to their Monte Carlo particle identification codes, defined in the Les Houches Accord (LHA)
        		//Electron = 11
        		//Muon = 13
        		//Tau = 13 
        		//Boson Z = 23
        		if(std::abs(PID[j])==11 || std::abs(PID[j])==13 || std::abs(PID[j])==15){
          		//Select which leptons comes from a Z
          			if(PID[Mother1[j]]==23){
            				std::cout << "Mother is: " << Mother1[j] << " Mother PID is: " << PID[Mother1[j]] << std::endl;
            				//Fill and sum their 4-momenta
            				TLorentzVector temp;
            				temp.SetPtEtaPhiE(Pt[j],Eta[j],Phi[j],E[j]);
            				lepton+=temp;
          		}
        		}      		

		}
		std::cout << "M()= " << lepton.M() << std::endl;
                h->Fill(lepton.M());
      		std::cout << std::endl;
	}
        
	  c->Draw();
    h->SetMarkerColor(8);
    h->SetMarkerStyle(4);
    h->Draw("PH");
    c->SaveAs("InvMass.png");

        
}

