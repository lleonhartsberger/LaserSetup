/*!
 * 
 * \file analyze_00.h
 * \brief data structure class for strixel scans
 * \author Grossmann Johannes
 * \date 16 / 05 / 15
 *
 * \Support : 
 *
 */
#ifndef analyze_00_h__
#define analyze_00_h__

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
#include "TVirtualFFT.h"
#include "TGraph.h"

typedef std::map<TString, TCanvas*> CanvasMap;
typedef std::map<TString, TGraphErrors> GraphMap;
typedef std::map<TString, TGraphErrors> ChannelGraphMap;
typedef std::map<TString, TF1> FitMap;
typedef std::map<TString, TObject> HistMap;
typedef std::map<TString, TProfile2D> ProfileMap;
typedef std::map<TString, TH1*> TH1Map;
typedef std::map<TString, TMultiGraph*> TMultiGraphMap;

typedef std::map<TString, TString> GenData;
typedef std::map<int,std::vector<double > > BadStripsMap;
typedef TPolyMarker BadStripsMarker;
typedef std::vector< TLine> BadStripsLines;
typedef std::vector< std::vector <double > > MeasMatrix;

namespace SINGLEMEAS
{
	class SingleMeas 
	{
	public:
		SingleMeas(std::string Filename):
		fDeviceString("PM100USB"),
		fDeviceSerialNoString("SN:P2004525"),
		fFirmwareVersionString("Firmware: 1.4.0"),
		fSensorTypeString("S150C"),
		fSensorSerialNoString("SN:15031631")
		{
			fFilename=Filename;
		}
		~SingleMeas() {
			if(fInFile.is_open()){
				fInFile.close();
			}
		}
		// methods
		void Initialise();
		void ReadFile();
		void SaveResults(TCanvas &canvas, std::ofstream &resultfile);
		void Fill();
		void Analysis();
		
		double Freedman_Diaconis(double* data, const int datasize);
		double Sturge(double* data, const int datasize);
		double Square_Root(double* data, const int datasize);
// 		double Freedman_Diaconis(double* data, const int datasize);
		
		
		TMultiGraph& DrawTMultigraph(TString MapStr);
		void DrawGraphRet(TMultiGraph &mg, TCanvas &canvas, int i, TGraphErrors &tgr);
		TGraphErrors& Draw1DRet(TString MapStr);
		TProfile2D& Draw2DRet(TString MapStr);
		void DrawText();
		void DrawMarker();
		void DrawCanvas(TCanvas &canvas);
		void DrawCanvas_Praesi(TCanvas &canvas);

		void Clean();
		
		
	private:
		std::string fDeviceString;
		std::string fDeviceSerialNoString;
		std::string fFirmwareVersionString;
		std::string fSensorTypeString;
		std::string fSensorSerialNoString;
		
		
		TString fFilename, fROOTFilename;
		TString fOutFilename;
		TFile fResultFile;
		TF1 *fit ;
		TPavesText *paves;		
		TLegend* legend;
		
// 		CanvasMap fCanvasMap;
		GraphMap fGraphMap;
		ChannelGraphMap fChannelGraphMap;
		FitMap fFitMap;
		HistMap fHistMap;
		ProfileMap fProfileMap;
		TH1Map fTH1Map;
		TMultiGraphMap fTMultiGraphMap;
		
		GenData fGenData;

		
		MeasMatrix fMeasMatrix;
		BadStripsMarker polymarker;
		BadStripsMarker polymarker_no_contact;
		BadStripsMarker polymarker_compliance;
		
		std::ifstream fInFile;
		std::vector<int> double_sized_strip_column;
		std::vector<double> area;

		int n_strixel=288;
		double I_sum, I_mean;
		bool read_str_file;

		
		
	protected:
		void pureRead(std::string filename, MeasMatrix &pMatrix);
		void fill_channel(int chan,  std::vector<double> &v, std::vector<double> &i);
		void initialise_channel();
		void tgraph_style(int chan, TGraphErrors &graph);
		void tgraph_style_gen(int chan, TGraphErrors &graph);
		void axis_title();
		void fill_1d(TGraphErrors &graph);
	};
}

#endif






