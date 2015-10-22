/*!
 * 
 * \file fourier.h
 * \brief data structure class for strixel scans
 * \author Grossmann Johannes
 * \date 16 / 05 / 15
 *
 * \Support : 
 *
 */
#ifndef fourier_h__
#define fourier_h__

#include <fstream>
#include <iostream>
#include <istream>
#include <iterator>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <time.h>

#include "Riostream.h"
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
#include "TGaxis.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TKey.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TList.h"
#include "TMath.h"
#include "TMultiGraph.h"
#include "TObject.h"
#include "TPad.h"
#include "TPavesText.h"
#include "TPolyMarker.h"
#include "TPostScript.h"
#include "TProfile2D.h"
#include "TRandom.h"
#include "TROOT.h"
#include "TSpectrum.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTimeStamp.h"
#include "TH1D.h"
#include "TVirtualFFT.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TMath.h"

namespace FOURIER
{
	class Fourier
	{
	public:
		Fourier(void)
		{
		}
		~Fourier() {
		}
		void FFT();
	};
}

#endif