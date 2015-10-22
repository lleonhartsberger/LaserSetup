#include <bitset>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <istream>
#include "Riostream.h"
#include <sstream>
#include <stdlib.h>
#include <string>
#include <stdio.h>

#include <TApplication.h>
#include "TAttLine.h"
#include "TAttMarker.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TClass.h"
#include "TColor.h"
#include "TDirectory.h"
#include "TExec.h"
#include "TF1.h"
#include "TF2.h"
#include "TFile.h"
#include "TFormula.h"
#include "TGaxis.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include <time.h>
#include "TKey.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TList.h"
#include "TMath.h"
#include "TMultiGraph.h"
#include "TObject.h"
#include "TPad.h"
#include "TPostScript.h"
#include "TRandom.h"
#include "TROOT.h"
#include "TSpectrum.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"
#include <vector>

#include "../tools/analyze_00.h"
using namespace SINGLEMEAS;
int main( int argc, char* argv[] )
{
	bool batchMode =0;
	TApplication cApp( "Root Application", &argc, argv );
	if ( batchMode ) gROOT->SetBatch( true );
	TQObject::Connect( "TCanvas", "Closed()", "TApplication", &cApp, "Terminate()" );
	TCanvas *d = new TCanvas("h","h",1);
	
	std::vector<std::string> filevec;
	std::vector<int> color_filevec;
	std::ifstream infile;
	std::ofstream outfile;
	
	infile.open("/home/johannes_grossmann/dissertation/2015/laser_setup/Messungen/PowerMeterData/files.csv");// file containing numbers in 3 columns 
	outfile.open("/home/johannes_grossmann/dissertation/2015/laser_setup/Messungen/PowerMeterData/results.csv");// file containing numbers in 3 columns 
	
	if(infile.fail()) // checks to see if file opended 
	{ 
		std::cout << "error" << std::endl; 
		return 1; // no point continuing if the file didn't open...
	} 
	if(outfile.fail()) // checks to see if file opended 
	{ 
		std::cout << "error" << std::endl; 
		return 1; // no point continuing if the file didn't open...
	}
	outfile<<"Name\tType\tOperator\tOverall percentage of:\tDefective strips\tNo Contact\tOverflow\tSum Current\tMean Current\tDifference\tLeakyStrips\tNullStrips\tOverflow"<<std::endl;
	
	
	while(!infile.eof()) // reads file to end of *file*, not line
	{ 
		std::string a, b, c;
		int color;
		
		infile >> a >> b>> c>>color; // read first column number
		filevec.push_back(a);
		color_filevec.push_back(color);
		std::printf("%s\t%s\t%s\t%d\n", a.c_str(), b.c_str(), c.c_str(), color);
	}
	infile.close(); 
	gStyle->SetPalette(55);
	
	for(int i=0; i<filevec.size()-1; i++)
		//  	for(int i=0; i<5; i++)
	{
		std::cout<<"processing"<<filevec.at(i) <<std::endl;
		SINGLEMEAS::SingleMeas meas(filevec.at(i));
		meas.Initialise();
		meas.ReadFile();
		meas.Fill();
		meas.Analysis();
 		meas.SaveResults(*d,outfile);
		meas.Clean();
// 		
		TString tmp = filevec.at(i)+".pdf";
		TString tmp1 = filevec.at(i)+".eps";
		d->SaveAs(tmp);
		d->SaveAs(tmp1);
// 		d->Clear();
// 		f.Clear();
// 		h.Clear();
// 		h01.Clear();
		// 		g.Clear();
// 		meas.Clean();
		std::cout<<"finished processing file "<<filevec.at(i)<<std::endl;
	}
	for(int i=0; i<filevec.size(); i++)
	{
		std::cout<<filevec.at(i)<<std::endl;
	}
	outfile.close();
// 	d->Close();
	std::cout<<"the end"<<std::endl;
	cApp.Run();
	std::cout<<"ende root"<<std::endl;
	return 0;
	
}