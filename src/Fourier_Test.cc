#include <bitset>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <istream>
#include "Riostream.h"
#include <sstream>
#include <stdlib.h>
#include <string>

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

#include "../tools/fourier.h"
using namespace FOURIER;
int main( int argc, char* argv[] )
{
	bool batchMode =0;
	TApplication cApp( "Root Application", &argc, argv );
	if ( batchMode ) gROOT->SetBatch( true );
	TQObject::Connect( "TCanvas", "Closed()", "TApplication", &cApp, "Terminate()" );
	TCanvas *d = new TCanvas("h","h",1);
	FOURIER::Fourier penner;
	penner.FFT();
	std::cout<<"the end"<<std::endl;
	cApp.Run();
	std::cout<<"ende root"<<std::endl;
	return 0;
	
}