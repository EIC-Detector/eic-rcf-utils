// Function Declaration
#include <vector>

double get_invariant_mass(double, double, double, double, double, double);

std::vector<std::vector<float> > get_event_cluster_data();
std::vector<std::vector<float> > get_event_track_data();

void openFile();
void closeFile();

// Global variables

TFile *f;
TTree *t;
std::vector<double> invariant_mass_cluster;
std::vector<double> invariant_mass_track;
TString file_name = "JP_C_10.root";

// Main Code

int jpsi_invariant_mass_analysis()
{
  std::vector<std::vector<float> > vect_1 = get_event_cluster_data(file_name);
  for(int i = 0; i<vect_1.at(0).size()/2; i++)
    {
      double theta_1 = (double)vect_1.at(0).at(2*i);
      double eta_1 = (double)vect_1.at(1).at(2*i);
      double phi_1 = (double)vect_1.at(2).at(2*i);
      double energy_1 = (double)vect_1.at(3).at(2*i);
      double pt_1 = (double)vect_1.at(4).at(2*i);
      
      double theta_2 = (double)vect_1.at(0).at(2*i+1);
      double eta_2 = (double)vect_1.at(1).at(2*i+1);
      double phi_2 = (double)vect_1.at(2).at(2*i+1);
      double energy_2 = (double)vect_1.at(3).at(2*i+1);
      double pt_2 = (double)vect_1.at(4).at(2*i+1);
      
      invariant_mass_cluster.push_back(get_invariant_mass(pt_1, pt_2, phi_1, phi_2, eta_1, eta_2));
    }
  
  std::vector<std::vector<float> > vect_2 = get_event_track_data(file_name);
  for(int i = 0; i<vect_2.at(0).size()/2; i++)
    {
      double theta_1 = (double)vect_2.at(0).at(2*i);
      double eta_1 = (double)vect_2.at(1).at(2*i);
      double phi_1 = (double)vect_2.at(2).at(2*i);
      double energy_1 = (double)vect_2.at(3).at(2*i);
      double pt_1 = (double)vect_2.at(4).at(2*i);
      
      double theta_2 = (double)vect_2.at(0).at(2*i+1);
      double eta_2 = (double)vect_2.at(1).at(2*i+1);
      double phi_2 = (double)vect_2.at(2).at(2*i+1);
      double energy_2 = (double)vect_2.at(3).at(2*i+1);
      double pt_2 = (double)vect_2.at(4).at(2*i+1);
      
      invariant_mass_track.push_back(get_invariant_mass(pt_1, pt_2, phi_1, phi_2, eta_1, eta_2));
    }
  
  return 0;
}

// Function Intialization

double get_invariant_mass(double pt1, double pt2, double phi1, double phi2, double eta1, double eta2)
{
  double Msquared = 2*pt1*pt2*(cosh(eta1-eta2)-cos(phi1-phi2));
  return (sqrt(Msquared));
}

std::vector<std::vector<float> > get_event_cluster_data(TString File)
{
  openFile(File);

  std::vector< vector<float> > return_vector;
  std::vector<float> return_theta;
  std::vector<float> return_eta;
  std::vector<float> return_phi;
  std::vector<float> return_energy;
  std::vector<float> return_pt;
  
  std::vector<float> theta;
  std::vector<float> eta;
  std::vector<float> phi;
  std::vector<float> energy;
  std::vector<float> pt;
  std::vector<float> ptotal;

  std::vector<float>* theta_pointer=&theta;
  std::vector<float>* eta_pointer=&eta;
  std::vector<float>* phi_pointer=&phi;
  std::vector<float>* energy_pointer=&energy;
  std::vector<float>* pt_pointer=&pt;
  std::vector<float>* ptotal_pointer=&ptotal;

  t->SetBranchAddress("em_cluster_theta",&theta_pointer);
  t->SetBranchAddress("em_cluster_eta",&eta_pointer);
  t->SetBranchAddress("em_cluster_phi",&phi_pointer);
  t->SetBranchAddress("em_cluster_e",&energy_pointer);
  t->SetBranchAddress("em_cluster_pt",&pt_pointer);
  t->SetBranchAddress("em_track_ptotal",&ptotal_pointer);
  
  const float energy_cut = 0.3;
  const float ep_cut = 0.8;
  
  Int_t nentries = Int_t(t->GetEntries());
  for(Int_t entryInChain=0; entryInChain<nentries; entryInChain++)
    {
      Int_t entryInTree = t->LoadTree(entryInChain);
      if (entryInTree < 0) break;
      t->GetEntry(entryInChain);
      int count=0;
      for(unsigned j = 0; j<theta.size(); j++)
	{
	  if(ptotal.at(j)!=0)
	    {
	      if(energy.at(j)>energy_cut && energy.at(j)/ptotal.at(j)>ep_cut) 
		count++;
	    }
	}
      if(count==2) //only two tracks, needs updating
	{
	  for(unsigned i = 0; i<theta.size(); i++)
	    {
	      if(energy.at(i)>energy_cut && energy.at(i)/ptotal.at(i)>ep_cut)
		{
		  return_theta.push_back(theta.at(i));
		  return_eta.push_back(eta.at(i));
		  return_phi.push_back(phi.at(i));
		  return_energy.push_back(energy.at(i));
		  return_pt.push_back(pt.at(i));
		}
	    }
	}
    }
  closeFile();

  return_vector.push_back(return_theta);
  return_vector.push_back(return_eta);
  return_vector.push_back(return_phi);
  return_vector.push_back(return_energy);
  return_vector.push_back(return_pt);
  return return_vector;
}

std::vector<std::vector<float> > get_event_track_data(TString File)
{
  openFile(File);

  std::vector< vector<float> > return_vector;
  std::vector<float> return_theta;
  std::vector<float> return_eta;
  std::vector<float> return_phi;
  std::vector<float> return_energy;
  std::vector<float> return_pt;
  
  std::vector<float> theta;
  std::vector<float> eta;
  std::vector<float> phi;
  std::vector<float> energy;
  std::vector<float> pt;
  std::vector<float> ptotal;

  std::vector<float>* theta_pointer=&theta;
  std::vector<float>* eta_pointer=&eta;
  std::vector<float>* phi_pointer=&phi;
  std::vector<float>* energy_pointer=&energy;
  std::vector<float>* pt_pointer=&pt;
  std::vector<float>* ptotal_pointer=&ptotal;

  t->SetBranchAddress("em_track_theta",&theta_pointer);
  t->SetBranchAddress("em_track_eta",&eta_pointer);
  t->SetBranchAddress("em_track_phi",&phi_pointer);
  t->SetBranchAddress("em_cluster_e",&energy_pointer);
  t->SetBranchAddress("em_track_ptrans",&pt_pointer);
  t->SetBranchAddress("em_track_ptotal",&ptotal_pointer);
  
  const float energy_cut = 0.3;
  const float ep_cut=0.8;

  Int_t nentries = Int_t(t->GetEntries());
  for(Int_t entryInChain=0; entryInChain<nentries; entryInChain++)
    {
      Int_t entryInTree = t->LoadTree(entryInChain);
      if (entryInTree < 0) break;
      t->GetEntry(entryInChain);
      int count=0;
      for(unsigned j = 0; j<theta.size(); j++)
	{
	  if(ptotal.at(j)!=0)
	    {
	      if(energy.at(j)>energy_cut && energy.at(j)/ptotal.at(j)>ep_cut) 
		count++;
	    }
	}
      if(count==2) //only two tracks, needs updating
	{
	  for(unsigned i = 0; i<theta.size(); i++)
	    {
	      if(energy.at(i)>energy_cut && energy.at(i)/ptotal.at(i)>ep_cut)
		{
		  return_theta.push_back(theta.at(i));
		  return_eta.push_back(eta.at(i));
		  return_phi.push_back(phi.at(i));
		  return_energy.push_back(energy.at(i));
		  return_pt.push_back(pt.at(i));
		}
	    }
	}
    }

  closeFile();

  return_vector.push_back(return_theta);
  return_vector.push_back(return_eta);
  return_vector.push_back(return_phi);
  return_vector.push_back(return_energy);
  return_vector.push_back(return_pt);
  return return_vector;
}

void openFile(TString FILE_NAME)
{
  f = new TFile("/sphenix/user/gregtom3/data/Summer2018/JPsi_reco_studies/" + FILE_NAME);
  t=(TTree*)f->Get("event_cluster");
}

void closeFile()
{
  delete t; delete f; f=NULL; t=NULL;
}
