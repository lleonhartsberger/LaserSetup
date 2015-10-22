#include "analyze_00.h"
using namespace std;
void SINGLEMEAS::SingleMeas::Initialise()
{
	std::cout<<"objects at Initialisation"<<std::endl;
	// 	gDirectory->ls("-m");
	read_str_file=0;
	// Create readfile/filenames outfile
	TString tmpstr = fFilename;
	fOutFilename=tmpstr+"_analysis.txt";
	tmpstr.ReplaceAll(".txt",".root");
	fROOTFilename=tmpstr;
	
	// 	Graphs
	fGraphMap.insert(make_pair("I_Diode",TGraphErrors()));
	fGraphMap.insert(make_pair("I_Diode_Abs_Time",TGraphErrors()));
	fGraphMap.insert(make_pair("I_Diode_Diff_Time",TGraphErrors()));
	fGraphMap.insert(make_pair("I_Diode_AutoCorr",TGraphErrors()));
	fGraphMap.insert(make_pair("I_Diode_Diff_Time",TGraphErrors()));
	fGraphMap.insert(make_pair("I_Diode_Diff_Points",TGraphErrors()));
	fGraphMap.insert(make_pair("Meas_Rate",TGraphErrors()));
	fGraphMap.insert(make_pair("Smallest_Distance",TGraphErrors()));
	
	for(auto graphs : fGraphMap)
	{
		graphs.second.SetName(graphs.first);
		graphs.second.SetTitle(graphs.first);
		fTH1Map[graphs.first]= new TH1D();
	}
//  	const int ADC_bits=14; 
	int ADC_bits[]={11,12,13,14,15,16};
	const double range[]={5E-8,5E-7, 5E-6, 5E-5, 5E-4, 5E-3};
	const double range_up_coeff=1.1;
	const double range_low_coeff=-0.1;
	for( int i=0; i<6; i++)
	{
		const int no_of_bins=TMath::Power(2,ADC_bits[i]);
		string name("I_Diode_ADC_binned_");
		char buffer [100];
		int n; 
		n=sprintf (buffer, "%1.0EA", range[i]);
// 		printf("%s", buffer, n);
		name+=string(buffer);
		double binsize=(range[0]*(range_up_coeff-range_low_coeff))/no_of_bins;
 		double upper=range_up_coeff*range[0];
 		double lower=range_low_coeff*range[0];
// 		double upper=range_up_coeff*range[0];
// 		double lower=range_low_coeff*range[0];
		Printf("%E %E %E %d %s", binsize, lower, upper, no_of_bins, name.c_str());
		fTH1Map[name]= new TH1D(name.c_str(),name.c_str(),no_of_bins-1,lower,upper);
	}

	
// 	fTH1Map["I_Diode_ADC_binned_5E-3A"]= new TH1D("I_Diode_ADC_binned_5E-3A","I_Diode_ADC_binned_5E-3A",no_of_bins,-5e-4,5.5e-3);
// 	fTH1Map["I_Diode_ADC_binned_5E-4A"]= new TH1D("I_Diode_ADC_binned_5E-4A","I_Diode_ADC_binned_5E-4A",no_of_bins,-5e-5,5.5e-4);
// 	fTH1Map["I_Diode_ADC_binned_5E-5A"]= new TH1D("I_Diode_ADC_binned_5E-5A","I_Diode_ADC_binned_5E-5A",no_of_bins,-5e-6,5.5e-5);
// 	fTH1Map["I_Diode_ADC_binned_5E-6A"]= new TH1D("I_Diode_ADC_binned_5E-6A","I_Diode_ADC_binned_5E-6A",no_of_bins,-5e-7,5.5e-6);
// 	fTH1Map["I_Diode_ADC_binned_5E-7A"]= new TH1D("I_Diode_ADC_binned_5E-7A","I_Diode_ADC_binned_5E-7A",no_of_bins,-5e-8,5.5e-7);
// 	fTH1Map["I_Diode_ADC_binned_5E-8A"]= new TH1D("I_Diode_ADC_binned_5E-8A","I_Diode_ADC_binned_5E-8A",no_of_bins,-5e-9,5.5e-8);
	
	
// 		fProfileMap.insert(make_pair(graphs.first, TProfile2D (graphs.first,graphs.first, 48, -0.5,47.5, 6, -0.5, 5.5, 0, 10)));
// 		fTH1Map.insert(make_pair(graphs.first, TH1F()));
		
// 	fTMultiGraphMap["R_punchthrough_MG"]=new TMultiGraph("R_punchthrough","R_punchthrough");
		
	polymarker.SetMarkerStyle(20);
	polymarker.SetMarkerColor(1);
	polymarker.SetMarkerSize(2);
	
	polymarker_no_contact.SetMarkerStyle(20);
	polymarker_no_contact.SetMarkerColor(3);
	polymarker_no_contact.SetMarkerSize(2);
	
	polymarker_compliance.SetMarkerStyle(20);
	polymarker_compliance.SetMarkerColor(2);
	polymarker_compliance.SetMarkerSize(2);
	
	paves = new TPavesText(0.1,0.1,0.9,0.9);
	
	std::cout<<"initialized "<<std::endl;
}


void SINGLEMEAS::SingleMeas::ReadFile()
{
	std::cout<<"read file"<<std::endl;
	this->pureRead((std::string)fFilename, fMeasMatrix);
}
void SINGLEMEAS::SingleMeas::SaveResults(TCanvas &canvas,  std::ofstream &resultfile)
{
	cout<<"Save Results to file "<<fROOTFilename<<endl;
	TFile fResultFile(fROOTFilename,"RECREATE");
	//  	fResultFile.cd();
	fResultFile.mkdir("Graphs");
	fResultFile.cd("Graphs");
	for(GraphMap::iterator it=fGraphMap.begin(); it!=fGraphMap.end(); ++it)
	{
		it->second.TObject::Write("Gr"+it->first);
	}
	fResultFile.cd();
	fResultFile.mkdir("TH2");
	fResultFile.cd("TH2");
	for(ProfileMap::iterator it=fProfileMap.begin(); it!=fProfileMap.end(); ++it)
	{
		it->second.TObject::Write("TP2D_"+it->first);
	}
	fResultFile.cd();
	fResultFile.mkdir("TMG");
	fResultFile.cd("TMG");
	for(TMultiGraphMap::iterator it=fTMultiGraphMap.begin(); it!=fTMultiGraphMap.end(); ++it)
	{
		it->second->TObject::Write("TMG_"+it->first);
	}
	fResultFile.cd();
	fResultFile.mkdir("TH1");
	fResultFile.cd("TH1");
	for(TH1Map::iterator it=fTH1Map.begin(); it!=fTH1Map.end(); it++)
	{
		it->second->TObject::Write("TH1_"+it->first,TObject::kOverwrite);
		//  		delete it->second;
	}
	// 	fit->Write("gaussian");
	fResultFile.cd();
	canvas.Write("Sum_Measurements");
	
	fResultFile.Write();
	fResultFile.Close();
	gDirectory->ls("-m");
// 	resultfile<<fGenData["Name"]<<fGenData["Type"]<<fGenData["Operator"]<<"\t";
	cout<<"Saved ..."<<endl;
}
void SINGLEMEAS::SingleMeas::Fill()
{
	printf("Filling the histograms\n");
	int i =0;
	int frequency_counter=0;
	double i_strip_cum=0;
	if(fMeasMatrix.empty())
		return;
	for(MeasMatrix::const_iterator it = fMeasMatrix.begin(); it != fMeasMatrix.end(); ++ it)
// 	for(MeasMatrix::const_iterator it = fMeasMatrix.begin(); it != fMeasMatrix.begin()+10; ++ it)
	{
		std::vector<double> tvec;
		int pointno = it-fMeasMatrix.begin();
		for(std::vector<double>::const_iterator itit = it->begin(); itit != it->end(); ++itit)
		{
			tvec.push_back((double)*itit);
// 			cout<<*itit<<"\t";
		}
// 		cout<<endl;
		fGraphMap["I_Diode"].TGraph::SetPoint(pointno,tvec[1],tvec[0] );
		fGraphMap["I_Diode_Abs_Time"].TGraph::SetPoint(pointno,tvec[1],tvec[0]);
		fGraphMap["I_Diode_Diff_Time"].TGraph::SetPoint(pointno,tvec[2],tvec[0]);
		fGraphMap["I_Diode_Diff_Points"].TGraph::SetPoint(pointno,pointno,tvec[0]);
// 		Fill Th1
		fTH1Map["I_Diode_ADC_binned_5E-03A"]->Fill(tvec[0]);
		fTH1Map["I_Diode_ADC_binned_5E-04A"]->Fill(tvec[0]);
		fTH1Map["I_Diode_ADC_binned_5E-05A"]->Fill(tvec[0]);
		fTH1Map["I_Diode_ADC_binned_5E-06A"]->Fill(tvec[0]);
		fTH1Map["I_Diode_ADC_binned_5E-07A"]->Fill(tvec[0]);
		fTH1Map["I_Diode_ADC_binned_5E-08A"]->Fill(tvec[0]);
		
	}
	
	// 		Calc mean measurement frequency
	double *time_ptr = fGraphMap["I_Diode_Diff_Time"].GetX();
	int pointno1=0;
	for(int i=0; i<fGraphMap["I_Diode_Diff_Time"].GetN(); i++)
	{
		if(i>0)
		{
			double freq=	1./(time_ptr[i+1]-time_ptr[i]);
			double time= time_ptr[i];
			if(freq<10000)
			{
				fGraphMap["Meas_Rate"].TGraph::SetPoint(pointno1,time,  freq);
				pointno1++;
			}
		}
	}
	double *i_ptr = fGraphMap["I_Diode_Diff_Time"].GetY();
	const int index = fGraphMap["I_Diode_Diff_Time"].GetN();
	int int_arr[index];
	TMath::Sort(index, i_ptr, int_arr, kFALSE);
	cout<<"jetzt die sortierten zahen"<<endl;
	for(int i=0; i<index-1; i++)
	{
		double difference = i_ptr[int_arr[i+1]]-i_ptr[int_arr[i]];
		fGraphMap["Smallest_Distance"].SetPoint(i,i,difference);
		if(difference>2e-12 && difference < 7E-12)
			printf("%.6e %.3e %d\n", difference,i_ptr[int_arr[i]],int_arr[i]);
	}


}

void SINGLEMEAS::SingleMeas::Analysis()
{
	if(fMeasMatrix.empty())
		return;
  	fill_1d(fGraphMap["I_Diode"]);
	
//   	fill_1d(fGraphMap["Meas_Rate"]);
// 	fill_1d(fGraphMap["I_Diode"]);

// 	double *x_ptr = fGraphMap["I_Diode_Diff_Time"].GetX();
// 	double *y_ptr = fGraphMap["I_Diode_Diff_Time"].GetY();
// 		double * z_prt_re;
// 		double * z_prt_im;
// 		double re2, im_2;
// 		int n_size = fGraphMap["I_Diode_Diff_Time"].GetN()+1;
// 		TVirtualFFT *fft_own = TVirtualFFT::FFT(1, &n_size, "R2C ES K");
// 		if (!fft_own) return;
// 		fft_own->SetPoints(y_ptr);
// 		fft_own->Transform();
//  		fft_own->GetPoints(z_prt_re);
// 		TH1 *hr = 0;
// 		hr = TH1::TransformHisto(fft_own, hr, "RE");
// 		hr->SetTitle("Real part of the 3rd (array) tranfsorm");
// // 		hr->Draw();
// 		TH1 *him = 0;
// 		him = TH1::TransformHisto(fft_own, him, "RE");
// 		him->SetTitle("Real part of the 3rd (array) tranfsorm");
// // 		him->Draw();
// 
// 		fTH1Map["TransformRe"]=hr;
// 		fTH1Map["TransformIm"]=him;
// 		for(int i=0; i<fGraphMap["I_Diode_Diff_Time"].GetN(); i++)
// 		{
// // 			printf("%f %e\n", x_ptr[i],y_ptr[i]);
// 		}
}
double SINGLEMEAS::SingleMeas::Freedman_Diaconis(double* data, const int datasize)
{
	cout<<"friedman diaconis"<<endl;
	const int nprob=2;
	double quantiles[nprob];
	double prob[nprob]={0.25,0.75};
// 	1. Compute quartiles
	TMath::Quantiles(datasize, nprob, data, quantiles, prob,kFALSE,0,7);
	cout<<"friedman quantiles"<<endl;
	double width= 2.*(quantiles[1]-quantiles[0])*TMath::Power(datasize,(-1./3.));
// 	Printf("quantiles %.10e %.10e %.10e\n", quantiles[0], quantiles[1],quantiles[1]-quantiles[0]);
	return width;
}
void SINGLEMEAS::SingleMeas::fill_1d(TGraphErrors& graph)
{
	cout<<"fill_1d"<<endl;
	int no_of_bins;
	double binwidth, x_min, y_min, x_max, y_max;
	double *data = graph.GetY();
	int datasize = graph.GetN();
	graph.ComputeRange(x_min, y_min,x_max, y_max);
 	binwidth= Freedman_Diaconis(data, datasize);
	
// 	binwidth=1;

	Printf("x_min %e, y_min %e,x_max %e, y_max %e, binwidth %e\n", x_min, y_min,x_max, y_max, binwidth);
	std::string graphname(graph.GetName());
	no_of_bins = TMath::Nint((y_max-y_min)/binwidth);
	if(no_of_bins==0)
		no_of_bins=1;
	if(abs(y_min-y_max)<1.E-16)
		y_max=y_min+1;
	Printf("no_of_bins %d max %e min %e graphname %s\n",no_of_bins,y_max,y_min, graphname.c_str() );

	fTH1Map[graphname]=new TH1D(graphname.c_str(),graphname.c_str(),no_of_bins,y_min, y_max );
	for(int i=0; i<datasize; i++)
	{
		fTH1Map[graphname]->Fill(data[i]);
	}
}

TMultiGraph& SINGLEMEAS::SingleMeas::DrawTMultigraph(TString MapStr)
{
	gPad->SetGrid(1);
	return *fTMultiGraphMap[MapStr];
}

TGraphErrors& SINGLEMEAS::SingleMeas::Draw1DRet(TString MapStr)
{
	return (fGraphMap[MapStr]);
}
TProfile2D& SINGLEMEAS::SingleMeas::Draw2DRet(TString MapStr)
{
	gPad->SetGrid(1);
	gStyle->SetOptStat(0);
	gPad->SetLogz();
	
	fProfileMap[MapStr].GetZaxis()->SetTitleOffset(1.5);
	
	return fProfileMap[MapStr];
}
void SINGLEMEAS::SingleMeas::DrawMarker()
{
	polymarker.Draw("same");
	polymarker_compliance.Draw("same");
	polymarker_no_contact.Draw("same");
}
void SINGLEMEAS::SingleMeas::DrawGraphRet(TMultiGraph& mg, TCanvas &canvas, int i, TGraphErrors &tgr)
{
	tgr = fGraphMap["I_Strip_normalized"];
	tgr.SetTitle(fGenData["Name"]);
	tgr.GetYaxis()->SetRangeUser(0,1e-9);
	this->tgraph_style_gen(i,tgr);
}

void SINGLEMEAS::SingleMeas::DrawText()
{
	paves->AddText("Results");
	paves->AddText("Text formatting is automatic");
	paves->Draw();
}
void SINGLEMEAS::SingleMeas::DrawCanvas(TCanvas& canvas)
{
	canvas.Clear();
	// 	canvas.Divide(3,2);
	// 	canvas.cd(1);
	
	gPad->SetGrid(1);
	gPad->SetLogz();
	gPad->SetRightMargin(0.2);
	gStyle->SetOptStat(0);
	
	
	this->Draw1DRet("I_Diode").Draw("apl");
}

void SINGLEMEAS::SingleMeas::DrawCanvas_Praesi(TCanvas& canvas)
{
	TGaxis::SetMaxDigits(3);
	double margin =0.18;
	canvas.Clear();
	canvas.Divide(3,2);
	
	canvas.cd(4);
	gStyle->SetOptTitle(kFALSE);
	gPad->SetGrid(1);
	gPad->SetLogz();
	gPad->SetMargin(margin,margin+0.1,margin,margin);
	// 	canvas.Clear();
	
	// 	gPad->SetRightMargin(0.2);
	// 	gPad->SetBottomMargin(margin);
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(kFALSE);
	
	fProfileMap["Current_density"].Draw("colz");
	fProfileMap["Current_density"].SetMaximum(0.01);
	fProfileMap["Current_density"].SetMinimum(0.0001);
	fProfileMap["Current_density"].GetZaxis()->SetTitle("<J> [Am^{-2}]");
	fProfileMap["Current_density"].GetXaxis()->SetTitle("Column");
	fProfileMap["Current_density"].GetYaxis()->SetTitle("Row");
	
	fProfileMap["Current_density"].GetXaxis()->SetTitleSize(0.08);
	fProfileMap["Current_density"].GetYaxis()->SetTitleSize(0.08);
	fProfileMap["Current_density"].GetZaxis()->SetTitleSize(0.08);
	
	fProfileMap["Current_density"].GetXaxis()->SetTitleOffset(1);
	fProfileMap["Current_density"].GetYaxis()->SetTitleOffset(1);
	fProfileMap["Current_density"].GetZaxis()->SetTitleOffset(1);
	
	fProfileMap["Current_density"].GetXaxis()->SetLabelSize(0.05);
	fProfileMap["Current_density"].GetYaxis()->SetLabelSize(0.06);
	fProfileMap["Current_density"].GetZaxis()->SetLabelSize(0.06);
	this->DrawMarker();
	
	canvas.cd(1);
	gStyle->SetOptTitle(kFALSE);
	
	gPad->SetMargin(margin,margin,margin,margin);
	// 	gPad->SetBottomMargin(margin);
	this->Draw1DRet("I_Diode").Draw("apl");
	fGraphMap["I_Diode"].GetXaxis()->SetTitleSize(0.08);
	fGraphMap["I_Diode"].GetYaxis()->SetTitleSize(0.08);
	
	fGraphMap["I_Diode"].GetXaxis()->SetLabelSize(0.06);
	fGraphMap["I_Diode"].GetYaxis()->SetLabelSize(0.06);
	
	fGraphMap["I_Diode"].GetXaxis()->SetTitleOffset(1);
	fGraphMap["I_Diode"].GetYaxis()->SetTitleOffset(1);
	
	fGraphMap["I_Diode"].GetYaxis()->SetTitleOffset(1);
	
	canvas.cd(5);
	gStyle->SetOptTitle(kFALSE);
	gPad->SetMargin(margin,margin,margin,margin);
	// 	gPad->SetTopMargin(margin);
	this->DrawTMultigraph("IV_punchthrough_MG").Draw("apl");
	fTMultiGraphMap["IV_punchthrough_MG"]->GetXaxis()->SetTitleSize(0.06);
	
	fTMultiGraphMap["IV_punchthrough_MG"]->GetXaxis()->SetTitleSize(0.08);
	fTMultiGraphMap["IV_punchthrough_MG"]->GetYaxis()->SetTitleSize(0.08);
	
	fTMultiGraphMap["IV_punchthrough_MG"]->GetXaxis()->SetLabelSize(0.06);
	fTMultiGraphMap["IV_punchthrough_MG"]->GetYaxis()->SetLabelSize(0.06);
	
	fTMultiGraphMap["IV_punchthrough_MG"]->GetXaxis()->SetTitleOffset(1);
	fTMultiGraphMap["IV_punchthrough_MG"]->GetYaxis()->SetTitleOffset(1);
	canvas.cd(2);
	gStyle->SetOptTitle(0);
	gPad->SetMargin(margin,margin,margin,margin);
	// 	gPad->SetTopMargin(margin);
	this->Draw1DRet("Current_density").Draw("apl");
	gStyle->SetOptTitle(0);
	fGraphMap["Current_density"].GetXaxis()->SetTitleSize(0.08);
	fGraphMap["Current_density"].GetYaxis()->SetTitleSize(0.08);
	
	fGraphMap["Current_density"].GetXaxis()->SetLabelSize(0.06);
	fGraphMap["Current_density"].GetYaxis()->SetLabelSize(0.035);
	fGraphMap["Current_density"].GetXaxis()->SetTitleOffset(1);
	fGraphMap["Current_density"].GetYaxis()->SetTitleOffset(1);
	
	canvas.cd(3);
	gStyle->SetOptTitle(0);
	gPad->SetMargin(margin,margin,margin,margin);
	// 	gPad->SetTopMargin(margin);
	legend = new TLegend (0.4,0.7,0.95,0.9,"");
	this->Draw1DRet("I_dark").Draw("apl");
	this->Draw1DRet("I_Diode_cumulative").Draw("plsame");	
	legend->AddEntry(&this->Draw1DRet("I_dark"),this->Draw1DRet("I_dark").GetTitle(),"pl");
	legend->AddEntry(&this->Draw1DRet("I_Diode_cumulative"),this->Draw1DRet("I_Diode_cumulative").GetTitle(),"pl");
	legend->Draw();
	gStyle->SetOptTitle(0);
	fGraphMap["I_dark"].GetXaxis()->SetTitleSize(0.08);
	fGraphMap["I_dark"].GetYaxis()->SetTitleSize(0.08);
	
	fGraphMap["I_dark"].GetXaxis()->SetLabelSize(0.06);
	fGraphMap["I_dark"].GetYaxis()->SetLabelSize(0.06);
	fGraphMap["I_dark"].GetXaxis()->SetTitleOffset(1);
	fGraphMap["I_dark"].GetYaxis()->SetTitleOffset(1);
	
	canvas.cd(6);
	gStyle->SetOptTitle(0);
	gPad->SetGrid(1);
	gPad->SetMargin(margin,margin,margin,margin);
	// 	gPad->SetTopMargin(margin);
	
	fGraphMap["Temp"].Draw("ap");
	fGraphMap["Temp"].GetYaxis()->SetRangeUser(0.,30);
	fGraphMap["Temp"].GetYaxis()->SetTitle("T [#circC]/ RHEL [%]");
	gStyle->SetOptTitle(0);
	fGraphMap["Temp"].GetXaxis()->SetTitleSize(0.08);
	fGraphMap["Temp"].GetYaxis()->SetTitleSize(0.08);
	
	fGraphMap["Temp"].GetXaxis()->SetLabelSize(0.06);
	fGraphMap["Temp"].GetYaxis()->SetLabelSize(0.06);
	fGraphMap["Temp"].GetXaxis()->SetTitleOffset(1);
	fGraphMap["Temp"].GetYaxis()->SetTitleOffset(1);
	
	// 		meas.Draw1DRet("I_Diode_cumulative")->Draw("apl");
	fGraphMap["Humidity"].Draw("psame");
	fGraphMap["Humidity"].SetMarkerColor(kBlue);
	fGraphMap["Humidity"].SetLineColor(kBlue);
	
	TLegend *legend1 = new TLegend (0.6,0.2,1.0,0.4,"");
	legend1->AddEntry(&this->Draw1DRet("Temp"),this->Draw1DRet("Temp").GetTitle(),"p");
	legend1->AddEntry(&this->Draw1DRet("Humidity"),this->Draw1DRet("Humidity").GetTitle(),"p");
	legend1->Draw();
	
}


void SINGLEMEAS::SingleMeas::pureRead(std::string filename, MeasMatrix &pMatrix)
{
	using namespace std;

	int no_of_header_lines=1;
	int no_of_meas=0 ;
	int line_no = 0;
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	
	MeasMatrix tempmatrix;
	TTimeStamp startstamp;
	
	FILE *fp;
	fp = fopen(filename.c_str(), "r");
	if (!fp)
		return ;
	char buf[1000];
	
	while (fgets(buf,1000, fp)!=NULL)
	{
// 		printf("%s",buf);
		line_no++;
		if(line_no<=no_of_header_lines)
		{
			printf("%s",buf);
			istringstream iss(buf);
			std::vector<string> tokens{std::istream_iterator<string>{iss}, std::istream_iterator<string>{}};
			if(!read_str_file)
			{
				fGenData["Device"]=tokens[0];
				fGenData["DeviceSerialNo"]=tokens[1];
				fGenData["FirmwareVersion"]=tokens[3];
				fGenData["SensorType"]=tokens[4];
				fGenData["SensorSerialNo"]=tokens[5];
				read_str_file=1;
			}
		}else
		{
			no_of_meas++;
			if(no_of_meas%10000==0)
			{ 
				printf("Events read %d\n", no_of_meas);
				fflush(stdout);
			}
			tempmatrix.push_back(vector<double>());
			TTimeStamp stamp;
			int day, month, year, hour, minute, second, millisecond;
			double value_dbl, time_val, time_diff_val;
			char unit[10];
			char value[10];

			std::string::size_type sz;     // alias of size_t
			std::locale::global(std::locale("de_DE.UTF8"));
			sscanf(buf,"%d.%d.%d %d:%d:%d,%d\t%s\t%s",&day,&month,&year,&hour,&minute,&second,&millisecond,value,unit);
// 			printf(buf);
			std::locale::global(std::locale("de_DE.UTF8"));
			value_dbl = atof(value);
 			timeinfo->tm_sec=second;
			timeinfo->tm_min=minute;
 			timeinfo->tm_hour=hour;
 			timeinfo->tm_mday=day;
 			timeinfo->tm_mon = month-1;
 			timeinfo->tm_year=year-1900;
 			stamp.SetSec(mktime(timeinfo));
 			stamp.SetNanoSec(millisecond*1000000.);
			time_val=stamp.AsDouble();
			if(no_of_meas==1)
			{
				startstamp=stamp;
				printf(buf);
			}
			time_diff_val=stamp.AsDouble()-startstamp.AsDouble();
			//   		std::locale::global(std::locale("en_GB.UTF8"));
// 			printf ("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%e\t%s\n",day,month,year,hour,minute,second,millisecond,value_dbl,unit);
//  			printf ("%s\n",stamp.AsString("l"));
			tempmatrix.back().push_back(value_dbl);
			tempmatrix.back().push_back(time_val);
			tempmatrix.back().push_back(time_diff_val);
		}
	}
// 	printf(buf);
if(no_of_meas==0)
	return;
	pMatrix=tempmatrix;
	fclose(fp);
	printf(buf);
	
}
void SINGLEMEAS::SingleMeas::initialise_channel()
{
	for(int i =1; i<n_strixel+1; i++)
	{
		char buff[100], buff1[100], buff2[100];
		sprintf(buff, "R_punchthrough_str%03d\n",i);
		sprintf(buff1,"IV_str_%03d\n",i);
		sprintf(buff2,"R_diff_pthr_str_%03d\n",i);
		
		fChannelGraphMap[buff]=TGraphErrors();
		fChannelGraphMap[buff1]= TGraphErrors();
		fChannelGraphMap[buff2]= TGraphErrors();
		
		fChannelGraphMap[buff].GetXaxis()->SetTitle("U [V]");
		fChannelGraphMap[buff1].GetXaxis()->SetTitle("U [V]");
		fChannelGraphMap[buff2].GetXaxis()->SetTitle("U [V]");
		
		fChannelGraphMap[buff].GetYaxis()->SetTitle("I [A]");
		fChannelGraphMap[buff1].GetYaxis()->SetTitle("R [#Omega]");
		fChannelGraphMap[buff2].GetYaxis()->SetTitle("R_{diff} [#Omega]");
		
		
		this->tgraph_style(i,fChannelGraphMap[buff]);
		this->tgraph_style(i,fChannelGraphMap[buff1]);
		this->tgraph_style(i,fChannelGraphMap[buff2]);
		
		
		fTMultiGraphMap["IV_punchthrough_MG"]->Add(&fChannelGraphMap[buff1],"pl");
		fTMultiGraphMap["R_punchthrough_MG"]->Add(&fChannelGraphMap[buff],"pl");
		fTMultiGraphMap["R_diff_punchthrough_MG"]->Add(&fChannelGraphMap[buff2],"pl");
	}
}

void SINGLEMEAS::SingleMeas::fill_channel(int chan, std::vector<double> &v, std::vector<double> &i)
{
	chan=i[0];
	char buff[100], buff1[100], buff2[100];
	sprintf(buff, "R_punchthrough_str%03d\n",chan);
	sprintf(buff1,"IV_str_%03d\n",chan);
	sprintf(buff2,"R_diff_pthr_str_%03d\n",chan);
	
	for(int j=1; j<i.size(); j++)
	{
		// 		printf("%03d\t%03f\t%03e\n",j-1, v[j],i[j]);
		fChannelGraphMap[buff1].SetPoint(j-1,v[j],i[j]);
		fChannelGraphMap[buff].SetPoint(j-1,v[j],v[j]/i[j]);
		fChannelGraphMap[buff2].SetPoint(j-1,v[j],i[j]);
		
	}
	// cout<<"chan  "<<chan<<endl;	
}
void SINGLEMEAS::SingleMeas::tgraph_style(int chan, TGraphErrors &graph)
{
	graph.SetMarkerColor(chan%50);
	graph.SetLineColor(chan%50);
	graph.SetMarkerStyle(7);
	graph.SetMarkerSize(3);
}
void SINGLEMEAS::SingleMeas::tgraph_style_gen(int chan, TGraphErrors &graph)
{
	if((chan+1)%10!=0){
		graph.SetMarkerColor(chan+1);
		graph.SetLineColor(chan+1);
	}
	else
	{
		graph.SetMarkerColor(50-(chan+1)%10);
		graph.SetLineColor(50-(chan+1)%10);
	}
	graph.SetMarkerStyle(7);
	graph.SetMarkerSize(3);
}

void SINGLEMEAS::SingleMeas::axis_title()
{
	for(GraphMap::iterator it=fGraphMap.begin(); it!=fGraphMap.end(); ++it)
	{
		it->second.GetYaxis()->SetTitleOffset(1.5);
		it->second.GetXaxis()->SetTitle("Strip No");
		if(it->first.Contains("I_"))
		{
			it->second.GetYaxis()->SetTitle(it->first+" [A]");
			it->second.SetTitle(it->first);
		}
		if(it->first.Contains("Current_density"))
		{
			it->second.GetYaxis()->SetTitle("<J> [Am^{-2}]");
			it->second.SetTitle(it->first);
		}
		if(it->first.Contains("R_"))
		{
			it->second.GetYaxis()->SetTitle(it->first+" [#Omega]");
			it->second.SetTitle(it->first);
		}
		if(it->first.Contains("C_"))
		{
			it->second.GetYaxis()->SetTitle(it->first+" [F]");
			it->second.SetTitle(it->first);
		}
		if(it->first.Contains("Temp"))
		{
			it->second.GetYaxis()->SetTitle(it->first+" [#circ C]");
			it->second.SetTitle(it->first);
		}
		if(it->first.Contains("Humidity"))
		{
			it->second.GetYaxis()->SetTitle(it->first+" %RH");
			it->second.SetTitle(it->first);
		}
		it->second.SetMarkerStyle(6);
		it->second.SetMarkerSize(6);
		it->second.SetMarkerColor(kRed);
		it->second.SetLineColor(kRed);
		// 		std::cout << it->first << "\t"<<it->second->GetTitle()<<"\t"<<it->second->GetXaxis()->GetTitle()<<"\t"<<it->second->GetYaxis()->GetTitle()<<std::endl;
	}
	for(auto it: fProfileMap){
		TString tmpstr = it.first;
		// 		cout<<tmpstr<<endl;
		it.second.GetXaxis()->SetTitle("strip_no_x");
		it.second.GetYaxis()->SetTitle("strip_no_y");
		if(tmpstr.Contains("Current_density"))
		{
			it.second.SetTitle("<Mean Current_density>");
			it.second.GetZaxis()->SetTitle("<J> [Am^{-2}]");
		}
		if(tmpstr.Contains("I_"))
		{
			it.second.SetTitle("I_Diode");
			it.second.GetZaxis()->SetTitle("I [A]");
		}
	}
	for(auto it: fTMultiGraphMap){
		TString tmpstr = it.first;
		if(tmpstr.Contains("R_"))
		{
			it.second->SetTitle((TString)it.second->GetTitle()+";U [V];R [#Omega]");
		}
		if(tmpstr.Contains("IV_"))
		{
			it.second->SetTitle((TString)it.second->GetTitle()+";U [V];I [A]");
		}
	}
}
void SINGLEMEAS::SingleMeas::Clean()
{
	std::cout<<"clean"<<std::endl;
	for (auto it: fTH1Map)
	{
		if(it.second!=0)
		{
			std::cout<<"delete fth1"<<std::endl;
 			delete it.second;
//  			fTH1Map.erase(it);
			// 			it.second=nullptr;
			// 			delete it.second;
		}
	}
	if(fit!=0){
		// 		delete fit;
		// 		fit=nullptr;
	}
	
	for (auto it: fTMultiGraphMap)
	{
		if(it.second!=0){
			// 			std::cout<<"delete fTMultiGraphMap"<<std::endl;
			// 			delete it.second;
			// 			it.second=nullptr;
			// 			delete it.second;
		}
	}
	// 	for (auto it: fCanvasMap)
	// 	{
	// 		if(it.second!=0){
	// 			std::cout<<"delete fCanvasMap"<<std::endl;
	// 			delete it.second;
	// // 			it.second=nullptr;;
	// 		}
	// 	}
	if(paves!=0){
		// 		std::cout<<"delete paves"<<std::endl;
		// 		delete paves;
		// 		paves = nullptr;
		// 		delete paves;
	}
	if(legend!=0){
		// 		std::cout<<"delete legend"<<std::endl;
		// 		delete legend;
		// 		legend = nullptr; 
		// 		delete legend;
	}
	// 	gDirectory->GetList()->Delete();
	
	// sleep(1);
	// 	gDirectory->ls("-m");
	
	std::cout<<"deleted"<<std::endl;
}
