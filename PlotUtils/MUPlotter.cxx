#ifndef MNV_MUPlotter_cxx
#define MNV_MUPlotter_cxx 1

#include "PlotUtils/MUPlotter.h"
#include "HistogramUtils.h" //for IsAutoAxisLimit

#include "TROOT.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TLegend.h"
#include "TLine.h"
#include "TArrow.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TGaxis.h"
#include "TList.h"

#include <iostream>
#include <vector>
#include <string>
#include <assert.h>
#include <math.h>


//set this to 1 to turn on garbage collection by deletion of tmpObjects
#define DO_GARBAGE_COLLECTION 1

using namespace std;
using namespace PlotUtils;

/*#####################################################

	Below is an example of how to use it :
	... to be updated ...
	}
########################################################*/ 

//=========================================================
// Constructors and Destructors
//=========================================================
MUPlotter::MUPlotter()
{
	gROOT->GetListOfCleanups()->Add( &fTmpObjects );
	ApplyStyle( kDefaultStyle );
}

MUPlotter::MUPlotter( PlotUtils::t_PlotStyle style )
{
	gROOT->GetListOfCleanups()->Add( &fTmpObjects );
	ApplyStyle( style );
}


MUPlotter::~MUPlotter()
{
  gROOT->GetListOfCleanups()->Remove( &fTmpObjects );
#if DO_GARBAGE_COLLECTION

	//! Delete all heap based objects in tmpObjects array
	CleanTmp();

#endif
}


void MUPlotter::CleanTmp()
{
#if DO_GARBAGE_COLLECTION

	//! Get rid of the NULL entries
	fTmpObjects.Compress();

	//Error("MUPlotter::CleanTmp", "Number of objects in tmp before clean: %d", fTmpObjects.GetEntries() );

	//! Call Delete to get rid of the tmp objects
	fTmpObjects.Delete();

	//Error("MUPlotter::CleanTmp", "Number of objects in tmp after clean: %d", fTmpObjects.GetEntries() );

#endif
}

void MUPlotter::AddToTmp( TObject* obj )
{
	//note: build this everytime to avoid obj unused compiler warning
	if( 0 == obj )
	{
		Warning( "MUPlotter::AddToTmp", "Attempting to add NULL object to garbage.");
		return;
	}

#if DO_GARBAGE_COLLECTION

	//Error("MUPlotter::AddToTmp", Form( "Adding object '%s' at index %d.", obj->GetName(), fTmpObjects.GetEntries() ) );

	//! Add to array if the object isn't in the array already
	if( ! fTmpObjects.FindObject( obj ) )
		fTmpObjects.AddLast( obj );
#endif
}

//=========================================================
// utility to set up basic root environment
//=========================================================
void MUPlotter::SetRootEnv()
{
	gStyle->SetPalette(palette_style);

	// Canvas Styles
	gStyle->SetCanvasDefW(900);
	gStyle->SetCanvasDefH(750);
	gStyle->SetOptStat(0000);
	gStyle->SetOptFit(0000);
	gStyle->SetOptTitle(0);
	gStyle->SetCanvasColor(0);
	gStyle->SetPadBorderMode(0);
	gStyle->SetFrameBorderMode(0);
	gStyle->SetCanvasBorderMode(0);
	gStyle->SetPadTopMargin(0.09);
	gStyle->SetPadBottomMargin(0.15);
	gStyle->SetPadLeftMargin(0.15);
	gStyle->SetPadRightMargin(0.15);
	gStyle->SetFrameLineWidth(2);
	gStyle->SetHistLineWidth(2);

	// Axis Styles
	gStyle->SetHistMinimumZero( hist_min_zero );
	gStyle->SetTitleOffset( axis_title_offset_x, "X" );
	gStyle->SetTitleSize( axis_title_size_x, "X" );
	gStyle->SetTitleFont( axis_title_font_x, "X" );
	gStyle->SetTitleOffset( axis_title_offset_y, "Y" );
	gStyle->SetTitleSize( axis_title_size_y, "Y" );
	gStyle->SetTitleFont( axis_title_font_y, "Y" );
	gStyle->SetTitleOffset( axis_title_offset_z, "Z" );
	gStyle->SetTitleSize( axis_title_size_z, "Z" );
	gStyle->SetTitleFont( axis_title_font_z, "Z" );
	gStyle->SetLabelFont( axis_label_font, "XYZ" );
	gStyle->SetLabelSize( axis_label_size, "XYZ" );
	TGaxis::SetMaxDigits(axis_max_digits);
	gStyle->SetPadGridX( axis_draw_grid_x );
	gStyle->SetPadGridY( axis_draw_grid_y );

	// Marker Styles
	gStyle->SetMarkerStyle(data_marker);
	gStyle->SetMarkerSize(data_marker_size);
	gStyle->SetMarkerColor(data_color);

	gStyle->SetEndErrorSize(2);
	gStyle->SetErrorX(0.5);
}

//================================================================
// set the variables of this namespace to defaults and apply style
//================================================================
void MUPlotter::ApplyStyle( PlotUtils::t_PlotStyle style /* = kDefaultStyle */ )
{
	n_color_contours = 999;

	if( style == kDefaultStyle )
	{
		//-- chi2 calculation
		chi2_use_overflow_err = false;

		//-- do you want to draw histograms normalized to bin width
		draw_normalized_to_bin_width = true;

		//-- rainbow
		palette_style = 1;

		//-- marker settings
		data_marker = 20;
		ratio_marker = 20;
		data_marker_size = 1.0;
		ratio_marker_size = 1.0;

		//-- line settings
		data_line_width = 1;
		data_line_style = 1;
		mc_line_width = 3;
		mc_line_style = 1;
		ratio_line_width = 3;

		//-- cut arrow settings
		arrow_line_width = 4;
		arrow_line_style = 1;
		arrow_line_color = 1;
		arrow_size = 0.01;
		arrow_type = "|>";

		//-- color settings
		data_color = 1;
		mc_color   = 2;
		mc_error_color = 45;
		mc_error_style = 3001;
		ratio_color = 1;

		mc_bkgd_color = 14;
		mc_bkgd_width = 1;
		mc_bkgd_line_color = 1;
		mc_bkgd_style = 3005;

		data_bkgd_color = 12;//gray
		data_bkgd_style = 24;//circle 
		data_bkgd_size   = 1.;

		//-- correlation 
		draw_corr_max1     = false;
		draw_corr_red_blue = true;

		//-- title settings
		title_font    = 62;
		title_size = 0.06;

		//-- axis options
		hist_min_zero     = true;
		axis_draw_grid_x = false;
		axis_draw_grid_y = false;
		axis_max_digits   = 3;
		axis_title_font_x = 62;
		axis_title_font_y = 62;
		axis_title_font_z = 62;
		axis_title_offset_x = 1.15;
		axis_title_offset_y = 1.2;
		axis_title_offset_z = .75;
		axis_title_size_x = 0.06;
		axis_title_size_y = 0.06;
		axis_title_size_z = 0.06;
		axis_minimum      = MUHist::AutoAxisLimit;
		axis_maximum      = MUHist::AutoAxisLimit;

		//-- axis label options
		axis_label_font = 42;
		axis_label_size = 0.05;

		//-- margins
		extra_top_margin    = -.02; //negative means go closer to edge
		extra_bottom_margin = 0.;
		extra_left_margin   = 0.;
		extra_right_margin  = 0.;

		//-- layout
		headroom = 1.5;
		footroom = 1.;

		//-- legend
		height_nspaces_per_hist = 2.;
		width_xspace_per_letter = .5;
		legend_border_size      = 0;
		legend_fill_color       = -1;
		legend_text_size        = .04;
		legend_offset_x         = 0.;
		legend_offset_y         = 0.;
		legend_n_columns        = 1;
		legend_text_font        = 62;

		good_colors.clear();
		good_colors.push_back( kBlack );
		good_colors.push_back( kRed+2 );
		good_colors.push_back( kBlue+2 );
		good_colors.push_back( kYellow+2 );
		good_colors.push_back( kMagenta+2 );
		good_colors.push_back( kGreen+3 );
		good_colors.push_back( kOrange-3 );
		good_colors.push_back( kPink+2 );
		good_colors.push_back( kCyan+2 );

		//-- define colors of the standard errors
		error_color_map.clear();
		error_color_map["Flux_Tertiary"]  = kYellow-3;
		error_color_map["Flux_BeamFocus"] = kOrange+2;
		error_color_map["Flux_NA49"]      = kRed+2;
		error_color_map["GENIE"]          = kGreen+2;
		error_color_map["Normalization"]  = kTeal+2;
		error_color_map["Muon_Energy"]    = kPink+2;
		error_color_map["Hadronic_Energy"]= kMagenta+2;
		error_color_map["BackgroundFit"]  = kBlue+2;

		print_formats.clear();
		print_formats.push_back( "png" );
		print_formats.push_back( "eps" );

		print_topdir = "";
	}
	else if( style == kCompactStyle )
	{
		// Start from defaults
		ApplyStyle( kDefaultStyle );

		//tweak margins
		extra_top_margin = -.035; //go slightly closer to top of pad

		mc_bkgd_color = 46;
		mc_bkgd_line_color = 46;

		data_bkgd_color = 12;//gray
		data_bkgd_style = 24;//circle  

		//legend entries are closer
		height_nspaces_per_hist = 1.2;
		width_xspace_per_letter = .4;
		legend_text_size        = .03;
	}
	else if ( style == kCCNuPionIncStyle) {

		ApplyStyle( kCompactStyle );

		legend_n_columns        = 1;

		//-- define colors of the standard errors
		error_color_map.clear();
		error_summary_group_map.clear();

		//Systematic color scheme
		error_color_map["Flux"]        = kYellow-3;
		error_color_map["Interaction Model"] = kGreen+2;
		error_color_map["Michel"] = kPink+2;
		error_color_map["Normalization"] = kCyan+2;
		error_color_map["Detector Model"]  = kOrange+2;
		error_color_map["Energy Response"] = kRed+2;
		error_color_map["Angle & Vertex"]  = kViolet+2;
		error_color_map["Other"]  = kMagenta+2;

		vector<string> genieGroup;
		genieGroup.push_back("GENIE_AhtBY");
		genieGroup.push_back("GENIE_BhtBY");
		genieGroup.push_back("GENIE_CCQEPauliSupViaFK");
		genieGroup.push_back("GENIE_CV1uBY");
		genieGroup.push_back("GENIE_CV2uBY");
		genieGroup.push_back("GENIE_EtaNCEL");
		genieGroup.push_back("GENIE_MaCCQE");
		genieGroup.push_back("GENIE_MaCCQEshape");
		genieGroup.push_back("GENIE_MaNCEL");
		genieGroup.push_back("GENIE_MaRES");
		genieGroup.push_back("GENIE_MvRES");
		genieGroup.push_back("GENIE_NormCCQE");
		genieGroup.push_back("GENIE_NormCCRES");
		genieGroup.push_back("GENIE_NormDISCC");
		genieGroup.push_back("GENIE_NormNCRES");
		genieGroup.push_back("GENIE_Rvn1pi");
		genieGroup.push_back("GENIE_Rvn2pi");
		genieGroup.push_back("GENIE_Rvp1pi");
		genieGroup.push_back("GENIE_Rvp2pi");
		genieGroup.push_back("GENIE_VecFFCCQEshape");

		genieGroup.push_back("GENIE_AGKYxF1pi");
		genieGroup.push_back("GENIE_FrAbs_N");
		genieGroup.push_back("GENIE_FrAbs_pi");
		genieGroup.push_back("GENIE_FrCEx_N");
		genieGroup.push_back("GENIE_FrCEx_pi");
		genieGroup.push_back("GENIE_FrElas_N");
		genieGroup.push_back("GENIE_FrElas_pi");
		genieGroup.push_back("GENIE_FrInel_N");
		genieGroup.push_back("GENIE_FrInel_pi");
		genieGroup.push_back("GENIE_FrPiProd_N");
		genieGroup.push_back("GENIE_FrPiProd_pi");
		genieGroup.push_back("GENIE_MFP_N");
		genieGroup.push_back("GENIE_MFP_pi");
		genieGroup.push_back("GENIE_RDecBR1gamma");
		genieGroup.push_back("GENIE_Theta_Delta2Npi");
		genieGroup.push_back("GENIE_EFNUCR");
		genieGroup.push_back("GENIE_FZONE");
		genieGroup.push_back("GENIE_AKGY");
		error_summary_group_map["Interaction Model"] = genieGroup;   

		vector<string> fluxGroup;
		fluxGroup.push_back("Flux_Tertiary");
		fluxGroup.push_back("Flux_BeamFocus");
		fluxGroup.push_back("Flux_NA49");
		error_summary_group_map["Flux"] = fluxGroup;

		vector<string> michelGroup;
		michelGroup.push_back("Michel_Eff");
		michelGroup.push_back("Michel_Bkg");
		michelGroup.push_back("Michel_AlternateCuts");
		error_summary_group_map["Michel"] = michelGroup;

		vector<string> geantGroup;
		geantGroup.push_back("Geant4_TotalPionInelastic");
		geantGroup.push_back("Geant4_TotalProtonInelastic");
		geantGroup.push_back("Geant4_ComponentPionInelastic");
		error_summary_group_map["Detector Model"] = geantGroup;

		vector<string> normGroup;
		normGroup.push_back("Normalization");
		normGroup.push_back("Mass_Scale");
		normGroup.push_back("POT_Scale");
		error_summary_group_map["Normalization"] = normGroup;

		vector<string> anaGroup;
		anaGroup.push_back("BackgroundFit");
		anaGroup.push_back("Unfolding");
		anaGroup.push_back("Unfolding_Theta");
		error_summary_group_map["Other"] = anaGroup;

		vector<string> angleGroup;
		angleGroup.push_back("Track_Angle");
		angleGroup.push_back("Beam_Angle");
		angleGroup.push_back("Vertex");
		error_summary_group_map["Angle & Vertex"] = angleGroup;

		vector<string> detGroup;
		detGroup.push_back("MINOS_Muon_Energy");
		detGroup.push_back("BetheBloch");
		detGroup.push_back("Mass");
		detGroup.push_back("Birks");
		detGroup.push_back("Particle_Response");
		detGroup.push_back("Res_Calorimetry");
		error_summary_group_map["Energy Response"] = detGroup;
	}  
	else if( kNukeCCStyle == style )
	{
		// Start from compact
		ApplyStyle( kCompactStyle );

		data_marker_size = ratio_marker_size = data_bkgd_size = 1.75;
		data_line_width = 3;
		mc_line_width = 3;
		mc_bkgd_style = 3444;

		legend_text_size = .04;

		axis_draw_grid_x = true;
		axis_draw_grid_y = true;
		legend_border_size      = 1;
		legend_fill_color       = 10;

		//change the BG color scheme
		//-- define colors of the standard errors
		error_color_map.clear();
		error_summary_group_map.clear();
		/*
		//verbose errors
		error_color_map["BG Scale"]       = kViolet+2;
		error_color_map["BG MC Stat."]    = kRed;
		error_color_map["Eff. MC Stat."]  = kYellow+2;
		error_color_map["Flux"]           = kOrange+2;
		error_color_map["XSec Models"]    = kGreen+2;
		error_color_map["FSI Models"]     = kCyan+2;
		error_color_map["Normalization"]  = kTeal+2;
		error_color_map["Vertex Rec."]    = kAzure+2;
		error_color_map["Muon Energy Rec."]    = kPink+2;
		error_color_map["Muon Angle Rec."]     = kRed+2;
		error_color_map["Hadronic Energy Rec."]= kMagenta+2;
		*/
		error_color_map["Scint. BG"] = kViolet+2;
		error_color_map["XSec Models"] = kGreen+2;
		error_color_map["FSI Models"]  = kCyan+2;
		//error_color_map["Flux"]        = kYellow-3;
		error_color_map["Detector Res."] = kRed+2;
		//error_color_map["Normalization "] = kPink+2;
		error_color_map["Other"]  = kOrange+2;

		vector<string> scintBGGroup;
		scintBGGroup.push_back("BG Scale");
		scintBGGroup.push_back("BG MC Stat.");
		error_summary_group_map["Scint. BG"] = scintBGGroup;

		vector<string> detResGroup;
		detResGroup.push_back("Vertex Rec.");
		detResGroup.push_back("Muon Energy Rec.");
		detResGroup.push_back("Muon Angle Rec.");
		detResGroup.push_back("Hadronic Energy Rec.");
		error_summary_group_map["Detector Res."] = detResGroup;

		vector<string> otherGroup;
		otherGroup.push_back("Flux");
		otherGroup.push_back("Eff. MC Stat.");
		otherGroup.push_back("Normalization");
		otherGroup.push_back("C Mass");
		otherGroup.push_back("Fe Mass");
		otherGroup.push_back("Pb Mass");
		otherGroup.push_back("CH Mass");
		error_summary_group_map["Other"] = otherGroup;

		vector<string> xsecErrGroup, fsiErrGroup;
		xsecErrGroup.push_back("AhtBY");
		xsecErrGroup.push_back("BhtBY");
		xsecErrGroup.push_back("CCQEPauliSupViaFK");
		xsecErrGroup.push_back("CV1uBY");
		xsecErrGroup.push_back("CV2uBY");
		xsecErrGroup.push_back("EtaNCEL");
		xsecErrGroup.push_back("MaCCQE");
		xsecErrGroup.push_back("MaCCQEshape");
		xsecErrGroup.push_back("MaNCEL");
		xsecErrGroup.push_back("MaRES");
		xsecErrGroup.push_back("MvRES");
		xsecErrGroup.push_back("NormCCQE");
		xsecErrGroup.push_back("NormCCRES");
		xsecErrGroup.push_back("NormDISCC");
		xsecErrGroup.push_back("NormNCRES");
		xsecErrGroup.push_back("Rvn1pi");
		xsecErrGroup.push_back("Rvn2pi");
		xsecErrGroup.push_back("Rvp1pi");
		xsecErrGroup.push_back("Rvp2pi");
		xsecErrGroup.push_back("VecFFCCQEshape");

		fsiErrGroup.push_back("AGKYxF1pi");
		fsiErrGroup.push_back("FrAbs_N");
		fsiErrGroup.push_back("FrAbs_pi");
		fsiErrGroup.push_back("FrCEx_N");
		fsiErrGroup.push_back("FrCEx_pi");
		fsiErrGroup.push_back("FrElas_N");
		fsiErrGroup.push_back("FrElas_pi");
		fsiErrGroup.push_back("FrInel_N");
		fsiErrGroup.push_back("FrInel_pi");
		fsiErrGroup.push_back("FrPiProd_N");
		fsiErrGroup.push_back("FrPiProd_pi");
		fsiErrGroup.push_back("MFP_N");
		fsiErrGroup.push_back("MFP_pi");
		fsiErrGroup.push_back("RDecBR1gamma");
		fsiErrGroup.push_back("Theta_Delta2Npi");
		fsiErrGroup.push_back("Birks' Parameter");
		fsiErrGroup.push_back("Nuclear Radius");
		fsiErrGroup.push_back("Formation Time");
		fsiErrGroup.push_back("AKGY Model");

		error_summary_group_map["XSec Models"] = xsecErrGroup;
		error_summary_group_map["FSI Models"] = fsiErrGroup;
	}
	else
	{
		Error( "ApplyStyle", "This plot style is not recognized.  Using the default: kDefaultStyle" );
		ApplyStyle( kDefaultStyle );
	}

	//set the environment to apply styles
	SetRootEnv();
}


//=========================================================
// Set the global color palette to hot/cold style
//=========================================================
void MUPlotter::SetCorrelationPalette() const
{
	// A colour palette that goes blue->white->red, useful for
	// correlation matrices
	const int NRGBs = 3, NCont = 999;
	gStyle->SetNumberContours(NCont);
	Double_t stops[NRGBs] = { 0.00, 0.50, 1.00};
	Double_t red[NRGBs]   = { 0.00, 1.00, 1.00};
	Double_t green[NRGBs] = { 0.00, 1.00, 0.00};
	Double_t blue[NRGBs]  = { 1.00, 1.00, 0.00};
	TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
}

void MUPlotter::SetWhiteRainbowPalette() const
{
	const int NRGBs = 7;
	gStyle->SetNumberContours(n_color_contours);
	Double_t stops[NRGBs] = { 0.00, 0.05, 0.23, 0.45, 0.60, 0.85, 1.00 };
	Double_t red[NRGBs]   = { 1.00, 0.00, 0.00, 0.00, 1.00, 1.00, 0.33 };
	Double_t green[NRGBs] = { 1.00, 1.00, 0.30, 0.40, 1.00, 0.00, 0.00 };
	Double_t blue[NRGBs]  = { 1.00, 1.00, 1.00, 0.00, 0.00, 0.00, 0.00 };
	TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, n_color_contours);  
}

void MUPlotter::SetRedHeatPalette() const
{
	const int NRGBs = 9;
	gStyle->SetNumberContours(n_color_contours);
	Double_t stops[NRGBs] = { 0.00, 0.125, 0.250, 0.375, 0.500, 0.625, 0.750, 0.875, 1.000};
	Double_t red[NRGBs]   = { 1.00, 1.00, 0.99, 0.99, 0.98, 0.94, 0.80, 0.65, 0.40 };
	Double_t green[NRGBs] = { 0.96, 0.88, 0.73, 0.57, 0.42, 0.23, 0.09, 0.06, 0.00 };
	Double_t blue[NRGBs]  = { 0.94, 0.82, 0.63, 0.45, 0.29, 0.17, 0.11, 0.08, 0.05 };
	TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, n_color_contours);
}

//=========================================================
//  Apply the MUPlotter axis style to all histograms in a TDirectory
//=========================================================
bool MUPlotter::ApplyAxisStyle(
		TDirectory *td, 
		bool centerXTitle /*= true*/, 
		bool centerYTitle /*= false*/,
		bool centerZTitle /*= false*/
		) const
{
	//! Complain and return false if no TDirectory was given
	if( !td )
	{
		Error( "ApplyAxisStyle", "You gave me a NULL TDirectory." );
		return false;
	}

	//! Loop over all keys in the TDirectory
	TIter nextkey( td->GetListOfKeys() );
	TKey *key = 0;
	while ( (key = (TKey*)nextkey())) 
	{
		TObject *obj = key->ReadObj();

		//! Skip the object if it doesn't inherit from a TH1
		TH1 *h = dynamic_cast<TH1*>( obj );
		if( ! h )
			continue;

		ApplyAxisStyle( h, centerXTitle, centerYTitle, centerZTitle );
	}
	return true;
}

bool MUPlotter::ApplyAxisStyle(
		TH1 *h,
		bool centerXTitle /*= true*/,
		bool centerYTitle /*= false*/,
		bool centerZTitle /*= false*/
		) const
{

	//!Set the X axis
	h->GetXaxis()->SetNdivisions(509);
	h->GetXaxis()->CenterTitle( centerXTitle );
	h->GetXaxis()->SetTitleOffset( axis_title_offset_x );
	h->GetXaxis()->SetTitleSize( axis_title_size_x );
	h->GetXaxis()->SetTitleFont( axis_title_font_x );

	h->GetXaxis()->SetLabelFont( axis_label_font );
	h->GetXaxis()->SetLabelSize( axis_label_size );

	//!Set the Y axis
	h->GetYaxis()->CenterTitle( centerYTitle );
	h->GetYaxis()->SetTitleOffset( axis_title_offset_y );
	h->GetYaxis()->SetTitleSize( axis_title_size_y );
	h->GetYaxis()->SetTitleFont( axis_title_font_y );

	h->GetYaxis()->SetLabelFont( axis_label_font );
	h->GetYaxis()->SetLabelSize( axis_label_size );

	//! Set the Z axis
	if ( h->GetZaxis() != NULL )
	{
		h->GetZaxis()->CenterTitle( centerZTitle );
		h->GetZaxis()->SetTitleOffset( axis_title_offset_z );
		h->GetZaxis()->SetTitleSize( axis_title_size_z );
		h->GetZaxis()->SetTitleFont( axis_title_font_z );

		h->GetZaxis()->SetLabelFont( axis_label_font );
		h->GetZaxis()->SetLabelSize( axis_label_size );
	}

	return true;
}

bool MUPlotter::ApplyAxisStyle( 
		bool centerXTitle /*= true*/, 
		bool centerYTitle /*= false*/,
		bool centerZTitle /*= false*/
		) const
{
	return ApplyAxisStyle( gDirectory, centerXTitle, centerYTitle, centerZTitle );
}


void MUPlotter::UseAutoAxisLimits()
{
	axis_minimum = axis_maximum = MUHist::AutoAxisLimit;
}

//=========================================================


//=========================================================
// easily add Latex labels on plots
//=========================================================

void MUPlotter::AddPlotLabel( 
		const char* label,
		const double x, 
		const double y, 
		const double size /*= 0.05*/,
		const int color /*= 1*/, 
		const int font /*= 62*/, 
		const int align /*= 22*/,
		const double angle /*= 0*/
		)
{
	TLatex *latex = new TLatex( x, y, label );
	AddToTmp( latex );

	latex->SetNDC();
	latex->SetTextSize(size);
	latex->SetTextColor(color);
	latex->SetTextFont(font);
	latex->SetTextAlign(align);
	latex->SetTextAngle(angle);
	latex->Draw();
}

void MUPlotter::AddHistoTitle( 
		const char* title, 
		double titleSize, 
		int titleFont 
		)
{
	AddPlotLabel(title, 0.5, 1-gStyle->GetPadTopMargin()-extra_top_margin, titleSize, 1, titleFont, 21);
}




size_t MUPlotter::GetLegendEntrySize(
		const std::string& title
		)  const
{ 

	unsigned int raw_size = title.size();

	// Remove latex # from size count (imperfect right now)
	size_t new_pos = title.find('#');
	while ( new_pos != std::string::npos && raw_size>1 ) {
		raw_size -= 2; // do not count # variable and the following character towards size (Latex)
		new_pos = title.find('#',new_pos);
	}

	//Remove characters: {  }  ^  _   from size count
	//
	for (unsigned int i=0; i!= title.size(); ++i) {
		char c = title[i];
		if (raw_size>0) {
			if ( c=='}' || c=='{') {
				--raw_size;
			}
			if (i < title.size()-1 && (c=='^'||c=='_') && (char)title[i+1]=='{') {
				--raw_size;
			}
		}
	}

	return (size_t)raw_size;
}




void MUPlotter::DecodePosition( 
		const std::string& opts,
		double size, 
		int &align, 
		double &xLabel, 
		double &yLabel 
		) const
{
	//! if opts is a mixture of two strings like TR-TC, then use the average of those two positions and align of the first
	size_t dashLoc = opts.find("-");
	if( dashLoc != string::npos )
	{
		const string opts1 = opts.substr(0, dashLoc);
		int align1;
		double x1, y1;
		DecodePosition( opts1, size, align1, x1, y1 );

		const string opts2 = opts.substr(dashLoc+1);
		int align2;
		double x2, y2;
		DecodePosition( opts2, size, align2, x2, y2 );

		align = align1;
		xLabel = ( x1 + x2 ) / 2.;
		yLabel = ( y1 + y2 ) / 2.;
		return;
	}

	const double xLeft  = gStyle->GetPadLeftMargin() + 0.03;
	const double xCenter = .5;
	const double xRight = 1 - gStyle->GetPadRightMargin() - 0.025;

	const double yBottom = gStyle->GetPadBottomMargin() + size/2.;
	const double yCenter = .5;
	const double yTop    = 1 - gStyle->GetPadTopMargin() - size/2.;

	if( opts == "TC" || opts == ""){
		// default is TC (top center)
		align = 23;
		xLabel = xCenter;
		yLabel = yTop;
	}else if( opts == "TR" ){
		align = 33;
		xLabel = xRight;
		yLabel = yTop;
	}else if( opts == "TL" ){
		align = 13;
		xLabel = xLeft;
		yLabel = yTop;
	}else if( opts == "BR" ){
		align = 31;
		xLabel = xRight;
		yLabel = yBottom;
	}else if( opts == "BL" ){
		align = 11;
		xLabel = xLeft;
		yLabel = yBottom;
	}else if( opts == "BC" ){
		align = 21;
		xLabel = xCenter;
		yLabel = yBottom;
	}else if( opts == "L" ){
		align = 12;
		xLabel = xLeft;
		yLabel = yCenter;
	}else if( opts == "C" ){
		align = 22;
		xLabel = xCenter;
		yLabel = yCenter;
	}else if( opts == "R" ){
		align = 32;
		xLabel = xRight;
		yLabel = yCenter;
	}else{
		Warning("DecodePosition", "Position option '%s' is not valid,  No values have been set.", opts.c_str());
	}

}

size_t MUPlotter::GetLegendWidthInLetters( const std::vector<std::string>& titles ) const
{
	//find the longest title in each column
	size_t longestTitle[legend_n_columns];
	for( size_t i = 0; i != legend_n_columns; ++i )
		longestTitle[i] = 0;

	for( unsigned int i = 0; i != titles.size(); ++i )
	{
		unsigned int col = i % legend_n_columns;
		longestTitle[col] = std::max( GetLegendEntrySize( titles[i] ), longestTitle[col] );
	}
	size_t sumLongestTitles = 0;
	for( size_t i = 0; i != legend_n_columns; ++i )
		sumLongestTitles += longestTitle[i];

	return sumLongestTitles;
}



void MUPlotter::DecodeLegendPosition( 
		double &x1, 
		double &y1, 
		double &x2, 
		double &y2,
		const std::string& opts, 
		const size_t nHists,
		const size_t longestTitleSize /*=14*/, 
		const double textSize /* = legend_text_size */ 
		) const
{
	//! start by using DecodePosition
	int align = 22;
	DecodePosition( opts, textSize, align, x1, y1 );

	//! how tall does the legend need to be for this number of histograms?
	const double yspace = height_nspaces_per_hist * textSize * nHists / float( legend_n_columns );

	//! how wide does the legend need to be based on the longest title size?
	const double xspace_marker = .06;  //~space taken up by marker
	const double xspace = ((float)legend_n_columns)*xspace_marker + longestTitleSize*width_xspace_per_letter*textSize; //x extent necessary

	//! set vertical position
	if( align % 10 == 2 ) //center should be at current y_legend
		y1 -= yspace / 2.; 
	else if( align % 10 == 3 ) //right edge should be at current y_legend
		y1 -= yspace;

	//! set horizontal position
	if( align / 10 == 2 ) //center should be at current x_legend
		x1 -= xspace / 2.; 
	else if( align / 10 == 3 ) //top edge should be at current x_legend
		x1 -= xspace;

	x1 += legend_offset_x;
	y1 += legend_offset_y;

	x2 = x1 + xspace;
	y2 = y1 + yspace;
}


void MUPlotter::WritePreliminary( 
		const std::string& opts, 
		double size, 
		double yOffset, 
		double xOffset,
		bool isWorkInProgress /*=false*/ 
		)
{
	int align;
	double xLabel, yLabel;
	DecodePosition( opts, size, align, xLabel, yLabel );
	yLabel += yOffset;
	xLabel += xOffset;
	if (isWorkInProgress) AddPlotLabel( "MINER#nuA Work In-Progress", xLabel, yLabel, size, kRed+1, 112, align ); 
	else AddPlotLabel( "MINER#nuA Preliminary", xLabel, yLabel, size, 4, 112, align ); 
}

void MUPlotter::WritePreliminary( 
		double x,
		double y,
		double size,
		bool isWorkInProgress /*=false*/ 
		)
{
	if (isWorkInProgress) AddPlotLabel( "MINER#nuA Work In-Progress", x, y, size, kRed+1, 112, 22 );
	else AddPlotLabel( "MINER#nuA Preliminary", x, y, size, 4, 112, 22 );
}

void MUPlotter::WriteNorm( 
		const char *norm, 
		const std::string& opts, 
		double size, 
		double yOffset,
		double xOffset, 
		double pot )
{  
	int align;
	double xLabel, yLabel;
	DecodePosition( opts, size, align, xLabel, yLabel );
	yLabel += yOffset;
	xLabel += xOffset;
	AddPlotLabel( norm, xLabel, yLabel, size, 9, 42, align );
	if( pot != 0 ) 
		AddPlotLabel( Form("%4.2e POT", pot), xLabel, yLabel-size, 0.8*size, 1, 42, align ); 
}

void MUPlotter::WriteNorm( 
		const char *norm,  
		double x,
		double y,
		double size, 
		double pot 
		)
{
	AddPlotLabel( norm, x, y, size, 9, 42, 22 );
	if( pot != 0 ) 
		AddPlotLabel( Form("%4.2e POT", pot), x, y-size, 0.8*size, 1, 42, 22 );
}

void MUPlotter::AddPOTNormBox( 
		const double dataPOT, 
		const double mcPOT, 
		const double xLeft,
		const double yTop,
		const double size /*= .03*/ 
		)
{
	WriteNorm( "POT-Normalized",                 xLeft, yTop, size );
	WriteNorm( Form( "Data POT: %.2E", dataPOT), xLeft, yTop - size, size );
	WriteNorm( Form( "MC POT: %.2E", mcPOT),     xLeft, yTop - 2*size, size );
}

void MUPlotter::AddAreaNormBox( 
		const double dataScale, 
		const double mcScale, 
		const double xLeft,
		const double yTop, 
		double size /*= 0.03*/ 
		)
{
	WriteNorm( "Area-Normalized",                               xLeft, yTop, size );
	WriteNorm( Form( "MC Scale = %.2E ", dataScale / mcScale ), xLeft, yTop - size, size );
	WriteNorm( Form( "  = %.2E/%.2E", dataScale, mcScale ),     xLeft, yTop - 2.*size, size );
}

void MUPlotter::AddChi2Label( 
		const TH1* dataHist, 
		const TH1* mcHist, 
		const std::string& opts 
		)
{
	AddChi2Label( dataHist, mcHist, 1.0, opts );
}

void MUPlotter::AddChi2Label( 
		const MUH1D* dataHist, 
		const MUH1D* mcHist, 
		const std::string& opts,
		const bool useDataErrorMatrix,
		const bool useOnlyShapeErrors
		)
{
	AddChi2Label( dataHist, mcHist, 1.0, opts, 0.04, 0.0, useDataErrorMatrix, useOnlyShapeErrors );
}

void MUPlotter::AddChi2Label( 
		const TH1* dataHist, 
		const TH1* mcHist, 
		Double_t mcScale, 
		const std::string& opts, 
		double size, 
		double yOffset 
		)
{
	int align;
	double xLabel, yLabel;
	DecodePosition( opts, size, align, xLabel, yLabel );
	yLabel += yOffset;

	Int_t ndf;
	Double_t chi2 = Chi2DataMC( dataHist, mcHist, ndf, mcScale );

	char *words = Form("#chi^{2}/ndf = %3.2f/%d = %3.2f", chi2, ndf, chi2/(Double_t)ndf); 
	AddPlotLabel( words, xLabel, yLabel, size, 1, 62, align );

}

void MUPlotter::AddChi2Label( 
		const MUH1D* dataHist, 
		const MUH1D* mcHist, 
		Double_t mcScale, 
		const std::string& opts, 
		double size, 
		double yOffset,
		const bool useDataErrorMatrix,
		const bool useOnlyShapeErrors,
		const std::string& pre_tag /*=""*/
		)
{
	int align;
	double xLabel, yLabel;
	DecodePosition( opts, size, align, xLabel, yLabel );
	yLabel += yOffset;

	Int_t ndf;
	Double_t chi2 = Chi2DataMC( dataHist, mcHist, ndf, mcScale, useDataErrorMatrix, useOnlyShapeErrors );

	char *words = Form("%s #chi^{2}/ndf = %3.2f/%d = %3.2f", pre_tag.c_str(), chi2, ndf, chi2/(Double_t)ndf); 
	AddPlotLabel( words, xLabel, yLabel, size, 1, 62, align );
}

void MUPlotter::AddCutArrow(
		const double cut_location,
		const double y1,
		const double y2,
		const double arrow_length,
		const std::string& arrow_direction
		) const
{

	double arrow_tip = arrow_length;
	if (arrow_direction == "L") {
		arrow_tip *= -1.0;
	}
	else if (arrow_direction != "R") {
		std::cout<<"Do not understand supplied arrow direction.  L (left) and R (right) only!"<<std::endl; 
	} 

	TLine line;
	line.SetLineWidth(arrow_line_width); 
	line.SetLineStyle(arrow_line_style);
	line.SetLineColor(arrow_line_color);
	line.DrawLine(cut_location,y1,cut_location,y2);

	TArrow arrow;
	arrow.SetLineWidth(arrow_line_width); 
	arrow.SetLineStyle(arrow_line_style);
	arrow.SetLineColor(arrow_line_color); 
	arrow.DrawArrow(cut_location,y2,cut_location+arrow_tip,y2,arrow_size,arrow_type.c_str()); 
}

//==========================================================
// Get a distinct line style
//==========================================================
void MUPlotter::ApplyNextLineStyle( 
		TH1* h, 
		bool startOver /* = false */, 
		bool changeStyle /* = true */
		)const
{
	static unsigned int nseen = 0;
	if( startOver)
		nseen = 0;

	if( ! h )
		return;

	//! Pick the next good color and apply it to the line
	h->SetLineColor( good_colors[ nseen % good_colors.size() ] );

	//! If changing the line style, apply a new style
	if( changeStyle )
		h->SetLineStyle( nseen % 10 + 1 );

	h->SetLineWidth( mc_line_width );
	++nseen;
}
//==========================================================
// draw MC data points on current Pad - MUH1D overload.
//==========================================================

void MUPlotter::DrawDataMC( 
		const TH1* dataHist, 
		const TH1* mcHist, 
		const Double_t mcScale, /*= 1.0*/ 
		const std::string& legPos, /*= "L"*/ 
		const bool useHistTitles /*=false*/
		)
{
	TH1D *tmpData = (TH1D*)dataHist->Clone("tmpData");
	TH1D *tmpMC = (TH1D*)mcHist->Clone("tmpMC");

	tmpMC->Scale(mcScale);

	//respect max/min setting the user may have used
	if( MUHist::IsAutoAxisLimit( axis_minimum ) )
	{
		if( hist_min_zero && !gPad->GetLogy() )
			tmpMC->SetMinimum( 0. );
		else
			tmpMC->SetMinimum( footroom * std::min( tmpData->GetMinimum(), tmpMC->GetMinimum() ) );
	}
	else
		tmpMC->SetMinimum( axis_minimum );

	if( MUHist::IsAutoAxisLimit( axis_maximum ) )
		tmpMC->SetMaximum( headroom * std::max( tmpData->GetMaximum(), tmpMC->GetMaximum() ) );
	else
		tmpMC->SetMaximum( axis_maximum );

	tmpData->SetMarkerStyle(data_marker);
	tmpData->SetMarkerSize(data_marker_size);
	tmpData->SetMarkerColor(data_color);
	tmpData->SetLineWidth(data_line_width);
	tmpData->SetLineStyle(data_line_style);
	tmpData->SetLineColor(data_color);

	tmpMC->GetXaxis()->SetTitleFont(axis_title_font_x);
	tmpMC->GetYaxis()->SetTitleFont(axis_title_font_y);
	tmpMC->GetXaxis()->SetTitleSize(axis_title_size_x);
	tmpMC->GetYaxis()->SetTitleSize(axis_title_size_y);
	tmpMC->GetXaxis()->SetLabelFont(axis_label_font);
	tmpMC->GetYaxis()->SetLabelFont(axis_label_font);
	tmpMC->GetXaxis()->SetLabelSize(axis_label_size);
	tmpMC->GetYaxis()->SetLabelSize(axis_label_size);
	tmpMC->GetXaxis()->CenterTitle(kTRUE);

	tmpMC->SetLineColor(mc_color);
	tmpMC->SetLineWidth(mc_line_width);
	tmpMC->SetLineStyle(mc_line_style);

	tmpMC->Draw("HIST");
	tmpData->DrawCopy("E1X0SAME");


	if( legPos != "N" )
	{
		const string data_name    = (useHistTitles ? tmpData->GetTitle() : "Data" );
		const string mc_name      = (useHistTitles ? tmpMC->GetTitle()     : "Monte Carlo" );
		double x1, y1, x2, y2; 
		//figure out where to put the legend
		vector<string> titles;
		titles.push_back( data_name );
		titles.push_back( mc_name );
		size_t legendWidth = GetLegendWidthInLetters( titles );
		DecodeLegendPosition( x1, y1, x2, y2, legPos, titles.size(), legendWidth );

		TLegend *leg = new TLegend(x1, y1, x2, y2);
		leg->SetNColumns( legend_n_columns );
		static int legN = 0;
		leg->SetName( Form("This Legend %d", legN++ ) );
		AddToTmp( leg );

		leg->SetBorderSize( legend_border_size );
		leg->SetFillColor( legend_fill_color );
		if( legend_fill_color < 0 )
			leg->SetFillStyle(0);
		leg->SetTextSize( legend_text_size);
		leg->SetTextFont( legend_text_font );
		leg->AddEntry(tmpData, data_name.c_str(), "lep");
		leg->AddEntry(tmpMC, mc_name.c_str(), "l");

		leg->Draw();
	}

	gPad->RedrawAxis();
	gPad->Update();

}


//==========================================================
// draw MC with error band + data points on current Pad - MUH1D overload.
//==========================================================

void MUPlotter::DrawDataMCWithErrorBand( 
		const MUH1D* dataHist, 
		const MUH1D* mcHist, 
		const Double_t mcScale /*= 1.0*/, 
		const std::string& legPos /*= "L"*/, 
		const bool useHistTitles /*=false*/,
		const MUH1D* bkgdHist /*= NULL*/,
		const MUH1D* dataBkgdHist /*= NULL*/,
		const bool covAreaNormalize/*= false, Area Normalize considerations for covariance matrix*/
		)
{

	TH1Ptr tmpData = TH1Ptr( (TH1*)dataHist->GetCVHistoWithError(true, covAreaNormalize).Clone( "mnv_tmp_data" ) );
	TH1Ptr tmpMC   = TH1Ptr( (TH1*)mcHist  ->GetCVHistoWithError(true, covAreaNormalize).Clone( "mnv_tmp_mc"   ) );

	if( draw_normalized_to_bin_width )
	{
		if( dataHist->GetNormBinWidth() > 0 )
			tmpData.get()->Scale( dataHist->GetNormBinWidth(), "width" );

		if( mcHist->GetNormBinWidth() > 0 )
			tmpMC.get()  ->Scale( mcHist  ->GetNormBinWidth(), "width" );
	}

	if( bkgdHist != NULL )
	{
		TH1Ptr tmpBK = TH1Ptr( (TH1*)bkgdHist->GetCVHistoWithError(true, covAreaNormalize).Clone( Form("mnv_tmp_bkgd_%d", __LINE__ ) ) );
		if( draw_normalized_to_bin_width && bkgdHist->GetNormBinWidth() > 0  )
			tmpBK.get()->Scale( bkgdHist->GetNormBinWidth(), "width" );

		if( dataBkgdHist != NULL )
		{
			TH1Ptr tmpDataBK = TH1Ptr( (TH1*)dataBkgdHist->GetCVHistoWithError(true, covAreaNormalize).Clone( "mnv_tmp_data_bkgd" ) );
			if( draw_normalized_to_bin_width  && dataBkgdHist->GetNormBinWidth() > 0 )
				tmpDataBK.get()->Scale( dataBkgdHist->GetNormBinWidth(), "width" );

			DrawDataMCWithErrorBand( tmpData.get(), tmpMC.get(), mcScale, legPos, useHistTitles, tmpBK.get(), tmpDataBK.get() );
		}
		else
			DrawDataMCWithErrorBand( tmpData.get(), tmpMC.get(), mcScale, legPos, useHistTitles, tmpBK.get() );
	}
	else
		DrawDataMCWithErrorBand( tmpData.get(), tmpMC.get(), mcScale, legPos, useHistTitles );
}

//==========================================================
// draw MC with error band + data points on current Pad
//==========================================================

void MUPlotter::DrawDataMCWithErrorBand( 
		const TH1* dataHist, 
		const TH1 *mcHist, 
		const Double_t mcScale     /*= 1.0*/, 
		const std::string& legPos /*= "L"*/, 
		const bool useHistTitles    /*=false */,
		const TH1* bkgdHist        /*= NULL*/,
		const TH1* dataBkgdHist   /*= NULL*/)
{

	TH1Ptr tmpData ( (TH1*)dataHist->Clone( "tmp_data" ) );
	TH1Ptr tmpMC   ( (TH1*)mcHist  ->Clone( "tmpMC"    ) );

	TH1Ptr tmpBkgd;
	if( bkgdHist ) 
	{
		tmpBkgd = TH1Ptr( (TH1*)bkgdHist->Clone( "bktmp" ) );
		tmpBkgd->SetFillColor(mc_bkgd_color);
		tmpBkgd->SetFillStyle(mc_bkgd_style);
		tmpBkgd->SetLineColor(mc_bkgd_line_color);
		tmpBkgd->SetLineWidth(mc_bkgd_width);
	}

	TH1Ptr tmpDataBkgd;
	if( dataBkgdHist ) 
	{
		tmpDataBkgd = TH1Ptr( (TH1*)dataBkgdHist->Clone( "databktmp" ) );
		tmpDataBkgd->SetMarkerColor(data_bkgd_color);
		tmpDataBkgd->SetMarkerStyle(data_bkgd_style);
		tmpDataBkgd->SetMarkerSize(data_bkgd_size);
	}


	if( tmpData->GetSumw2N() == 0 )   
		tmpData->Sumw2();

	if( tmpMC->GetSumw2N() == 0 )     
		tmpMC->Sumw2();

	//respect max/min setting the user may have used
	if( MUHist::IsAutoAxisLimit( axis_minimum ) )
	{
		if( hist_min_zero && !gPad->GetLogy() )
			tmpMC->SetMinimum( 0. );
		else
			tmpMC->SetMinimum( footroom * std::min( tmpData->GetMinimum(), mcScale*tmpMC->GetMinimum() ) );
	}
	else
		tmpMC->SetMinimum( axis_minimum );

	if( MUHist::IsAutoAxisLimit( axis_maximum ) )
		tmpMC->SetMaximum( headroom * std::max( tmpData->GetMaximum(), mcScale*tmpMC->GetMaximum() ) );
	else
		tmpMC->SetMaximum( axis_maximum );

	{
		//temporarily set the min/max so DrawDataMCWithErrorBand doesn't change anything
		const double oldMin = axis_minimum;
		const double oldMax = axis_maximum;
		axis_minimum = tmpMC->GetMinimum();
		axis_maximum = tmpMC->GetMaximum();

		DrawMCWithErrorBand( tmpMC.get(), mcScale, tmpBkgd.get() );

		//put back the axis min/max
		axis_minimum = oldMin;
		axis_maximum = oldMax;
	}

	tmpData->SetMarkerStyle(data_marker);
	tmpData->SetMarkerSize(data_marker_size);
	tmpData->SetMarkerColor(data_color);
	tmpData->SetLineWidth(data_line_width);
	tmpData->SetLineStyle(data_line_style);
	tmpData->SetLineColor(data_color);
	tmpData->DrawCopy("SAME E1 X0");
	if( tmpDataBkgd.get() )
		tmpDataBkgd->DrawCopy("SAME HIST P");

	if( legPos != "N" )
	{
		const string data_name    = (useHistTitles ? tmpData->GetTitle() : "Data" );
		const string mc_name      = (useHistTitles ? tmpMC->GetTitle()     : "Monte Carlo" );
		const string bg_name      = (tmpBkgd     && useHistTitles ? tmpBkgd->GetTitle()     : "MC Background" );
		const string data_bg_name = (tmpDataBkgd && useHistTitles ? tmpDataBkgd->GetTitle() : "Data Background" );

		//figure out where to put the legend
		double x1, y1, x2, y2;
		vector<string> titles;
		titles.push_back( data_name );
		titles.push_back( mc_name );
		if( tmpBkgd )
			titles.push_back( bg_name );
		if( tmpDataBkgd )
			titles.push_back( data_bg_name );
		size_t legendWidth = GetLegendWidthInLetters( titles );
		DecodeLegendPosition( x1, y1, x2, y2, legPos, titles.size(), legendWidth );

		tmpMC->SetLineColor(mc_color);
		tmpMC->SetLineWidth(mc_line_width);
		tmpMC->SetLineStyle(mc_line_style);
		tmpMC->SetFillColor(mc_error_color);
		tmpMC->SetFillStyle(mc_error_style);

		TLegend *leg = new TLegend(x1, y1, x2, y2);
		leg->SetNColumns( legend_n_columns );
		static int legN = 0;
		leg->SetName( Form("This Legend %d", legN++ ) );
		AddToTmp( leg );

		leg->SetBorderSize( legend_border_size );
		leg->SetFillColor( legend_fill_color );
		if( legend_fill_color < 0 )
			leg->SetFillStyle(0);
		leg->SetTextSize( legend_text_size);
		leg->SetTextFont( legend_text_font );
		leg->AddEntry(tmpData.get(), data_name.c_str(), "lep");
		leg->AddEntry(tmpMC.get(), mc_name.c_str(), "fl");
		if( tmpBkgd.get() )
			leg->AddEntry(tmpBkgd.get(), bg_name.c_str(), "f" );
		if( tmpDataBkgd.get() )
			leg->AddEntry(tmpDataBkgd.get(), data_bg_name.c_str(), "p" );

		leg->Draw();
	}

	gPad->RedrawAxis();
	gPad->Update();
}


//=========================================================
// draw data/MC ratio in current pad
//=========================================================

void MUPlotter::DrawMCWithErrorBand( 
		const TH1 *mcHist,
		const Double_t mcScale /*= 1.0*/, 
		const TH1* bkgdHist /*= NULL*/ )
{
	TH1Ptr tmpMC( (TH1*)mcHist->Clone( "tmpMC" ) );
	tmpMC->Scale(mcScale);

	//respect max/min setting the user may have used
	if( MUHist::IsAutoAxisLimit( axis_minimum ) )
	{
		if( hist_min_zero && !gPad->GetLogy() )
			tmpMC->SetMinimum( 0. );
		else
			tmpMC->SetMinimum( footroom * mcScale*tmpMC->GetMinimum() );
	}
	else
		tmpMC->SetMinimum( axis_minimum );

	if( MUHist::IsAutoAxisLimit( axis_maximum ) )
		tmpMC->SetMaximum( headroom * mcScale*tmpMC->GetMaximum() );
	else
		tmpMC->SetMaximum( axis_maximum );

	tmpMC->GetXaxis()->SetTitleFont(axis_title_font_x);
	tmpMC->GetYaxis()->SetTitleFont(axis_title_font_y);
	tmpMC->GetXaxis()->SetTitleSize(axis_title_size_x);
	tmpMC->GetYaxis()->SetTitleSize(axis_title_size_y);
	tmpMC->GetXaxis()->SetLabelFont(axis_label_font);
	tmpMC->GetYaxis()->SetLabelFont(axis_label_font);
	tmpMC->GetXaxis()->SetLabelSize(axis_label_size);
	tmpMC->GetYaxis()->SetLabelSize(axis_label_size);
	tmpMC->GetXaxis()->CenterTitle(kTRUE);

	//draw the error part of the MC histogram
	tmpMC->SetFillColor(mc_error_color);
	tmpMC->SetFillStyle(mc_error_style);
	tmpMC->SetMarkerStyle(0);
	tmpMC->DrawCopy("E2");
	tmpMC->Draw("SAME AXIS");
	//draw the line part of the MC histogram
	tmpMC->SetFillColor(0);
	tmpMC->SetLineColor(mc_color);
	tmpMC->SetLineStyle(mc_line_style);
	tmpMC->SetLineWidth(mc_line_width);
	tmpMC->DrawCopy("SAME HIST");


	if( bkgdHist != NULL )
	{
		TH1Ptr bkTmp( (TH1*)bkgdHist->Clone( "tmp_bkgd_hist") );

		bkTmp->Scale(mcScale);
		bkTmp->SetFillColor(mc_bkgd_color);
		bkTmp->SetFillStyle(mc_bkgd_style);
		bkTmp->SetLineColor(mc_bkgd_line_color);
		bkTmp->SetLineWidth(mc_bkgd_width);
		bkTmp->DrawCopy("SAME HIST");
	}

	gPad->Update();
}


bool MUPlotter::AddSysError( TH1D *h, const TH1D *hErr, const bool sumSquares /*= true*/) const
{
	if( h->GetNbinsX() != hErr->GetNbinsX() ){
		Error("MUPlotter::AddSysError", "The histogram of errors must have the same number of bins as the histgram you are adding errors to");
		return false;
	}

	for( Int_t i = 0; i <= h->GetNbinsX(); i++ ){
		double err = hErr->GetBinContent(i) + h->GetBinError(i);
		if( sumSquares )
			err = sqrt( pow(err,2) + pow(h->GetBinError(i),2) );
		h->SetBinError( i, err );
	}

	return true;
}


void MUPlotter::DrawErrorBand( 
		const TH1 *h,
		const int color)
{
	TH1Ptr tmpErrUp(   (TH1D*)h->Clone("tmp_err_up"  ) );
	TH1Ptr tmpErrDown( (TH1D*)h->Clone("tmp_err_down") );

	for( Int_t i = 1; i <= h->GetNbinsX(); i++ )
	{
		double err = h->GetBinError(i);
		tmpErrUp  ->SetBinContent(i,h->GetBinContent(i) + err );
		tmpErrDown->SetBinContent(i,h->GetBinContent(i) - err );
	}

	tmpErrUp->SetFillColor(color);
	tmpErrUp->SetFillStyle(3001);
	tmpErrUp->SetLineColor(color);
	tmpErrUp->SetLineWidth(1);

	tmpErrDown->SetFillColor(10);
	tmpErrDown->SetLineColor(color);
	tmpErrDown->SetLineWidth(1);

	tmpErrUp  ->DrawCopy("A HIST SAME");
	tmpErrDown->DrawCopy("SAME HIST");

	gPad->Update();
}


//==========================================================
// draw MC with error band + data points on current Pad
//==========================================================

void MUPlotter::DrawDataMCRatio( 
		const TH1* dataHist, 
		const TH1 *mcHist, 
		const Double_t mcScale,    /*= 1.0*/ 
		const bool     drawOneLine, /*= true*/
		const double   plotMin,     /*=-1 (auto)*/ 
		const double   plotMax,     /*=-1 (auto)*/ 
		const char*    yaxisLabel   /*="Data / MC"*/ 
		)
{

	TH1Ptr tmpData( (TH1*)dataHist->Clone("tmp_data") );
	TH1Ptr tmpMC(   (TH1*)mcHist  ->Clone("tmp_MC"  ) );

	if( tmpData->GetSumw2N() == 0 ) 
		tmpData->Sumw2();
	if( tmpMC->GetSumw2N() == 0 ) 
		tmpMC->Sumw2();

	tmpMC->Scale(mcScale);

	TH1Ptr tmpRatio( (TH1*)tmpMC->Clone( "tmp_ratio" ) );

	tmpRatio->Divide(tmpData.get(), tmpMC.get());

	tmpRatio->GetYaxis()->SetTitle( yaxisLabel );
	tmpRatio->GetXaxis()->SetTitleFont(axis_title_font_x);
	tmpRatio->GetYaxis()->SetTitleFont(axis_title_font_y);
	tmpRatio->GetXaxis()->SetTitleSize(axis_title_size_x);
	tmpRatio->GetYaxis()->SetTitleSize(axis_title_size_y);
	tmpRatio->GetXaxis()->SetLabelFont(axis_label_font);
	tmpRatio->GetYaxis()->SetLabelFont(axis_label_font);
	tmpRatio->GetXaxis()->SetLabelSize(axis_label_size);
	tmpRatio->GetYaxis()->SetLabelSize(axis_label_size);
	tmpRatio->GetXaxis()->CenterTitle(kTRUE);
	tmpRatio->SetMarkerStyle(ratio_marker);
	tmpRatio->SetMarkerSize(ratio_marker_size);
	tmpRatio->SetLineWidth(ratio_line_width);
	tmpRatio->SetLineColor(ratio_color);
	tmpRatio->GetXaxis()->SetNdivisions(509);

	//!set the max bin based on the taller of the ratio or error
	double setMax = headroom*std::max(1.0, tmpRatio->GetBinContent(tmpRatio->GetMaximumBin()) + tmpRatio->GetBinError(tmpRatio->GetMaximumBin()) );

	tmpRatio->SetMaximum( setMax );
	if( plotMin != -1 )
		tmpRatio->SetMinimum( plotMin );
	if( plotMax != -1 )
		tmpRatio->SetMaximum( plotMax );


	tmpRatio->DrawCopy("X0");

	if( drawOneLine )
	{
		const TAxis *axis = tmpRatio->GetXaxis();
		double lowX  = axis->GetBinLowEdge( axis->GetFirst() );
		double highX = axis->GetBinUpEdge(  axis->GetLast() );

		TLine line;
		line.SetLineStyle(2);
		line.SetLineWidth(3);
		line.SetLineColor(36);
		line.DrawLine(lowX, 1., highX, 1.); //creates a new line which is owned by gPad
	}

	gPad->RedrawAxis();
	gPad->Update();
}

//=====================================================================
// draw MC with error band + data points on current Pad with stat error
// draw sys error band as an envelope around 1
//=====================================================================
void MUPlotter::DrawDataMCRatio( 
		const MUH1D* dataHist, 
		const MUH1D *mcHist, 
		const Double_t mcScale,     /*= 1.0*/
		const bool     drawSysLines, /*= true*/
		const bool     drawOneLine,  /*= true*/ 
		const double   plotMin,      /*=-1 (auto)*/ 
		const double   plotMax,      /*=-1 (auto)*/ 
		const char*    yaxisLabel,    /*="Data / MC"*/ 
		const bool     covAreaNormalize /*= false*/ 
		)
{

	//!make clones
	TH1Ptr tmpData( (TH1*)dataHist->Clone("tmp_data") );
	TH1Ptr tmpMC  ( (TH1*)mcHist  ->Clone("tmp_mc"  ) );

	if( draw_normalized_to_bin_width )
	{
		if( dataHist->GetNormBinWidth() > 0 )
			tmpData.get()->Scale( dataHist->GetNormBinWidth(), "width" );

		if( mcHist->GetNormBinWidth() > 0 )
			tmpMC.get()  ->Scale( mcHist  ->GetNormBinWidth(), "width" );
	}


	//scale MC to data
	tmpMC->Scale(mcScale);

	//get the ratio
	TH1Ptr tmpRatio( (TH1*)tmpMC->Clone("tmp_ratio") );
	tmpRatio->Divide(tmpData.get(), tmpMC.get());

	//get the total systematic error as a fraction of the CV
	TH1D sysErr = mcHist->GetTotalError(false, true, covAreaNormalize); //no stat error, get as fraction

	//!set the max bin based on the taller of the ratio or error
	double setMax = headroom * max( 
			tmpRatio->GetBinContent( tmpRatio->GetMaximumBin() ),
			1. + sysErr.GetBinContent( sysErr.GetMaximumBin() )
			);

	tmpRatio->SetMaximum( setMax );
	if( plotMin != -1 )
		tmpRatio->SetMinimum( plotMin );
	if( plotMax != -1 )
		tmpRatio->SetMaximum( plotMax );


	//apply style
	tmpRatio->GetYaxis()->SetTitle( yaxisLabel );
	tmpRatio->GetXaxis()->SetTitleFont(axis_title_font_x);
	tmpRatio->GetYaxis()->SetTitleFont(axis_title_font_y);
	tmpRatio->GetXaxis()->SetTitleSize(axis_title_size_x);
	tmpRatio->GetYaxis()->SetTitleSize(axis_title_size_y);
	tmpRatio->GetXaxis()->SetLabelFont(axis_label_font);
	tmpRatio->GetYaxis()->SetLabelFont(axis_label_font);
	tmpRatio->GetXaxis()->SetLabelSize(axis_label_size);
	tmpRatio->GetYaxis()->SetLabelSize(axis_label_size);
	tmpRatio->GetXaxis()->CenterTitle(kTRUE);
	tmpRatio->SetMarkerStyle(ratio_marker);
	tmpRatio->SetMarkerSize(ratio_marker_size);
	tmpRatio->SetLineWidth(ratio_line_width);
	tmpRatio->SetLineColor(ratio_color);
	tmpRatio->GetXaxis()->SetNdivisions(509);
	tmpRatio->DrawCopy("X0");

	//add sys error lines if desired
	if( drawSysLines )
	{
		//get the total sys error.  then fill a histogram of total negative sys error for the lower lines
		sysErr.SetMaximum( setMax );

		//content of sys error histogram should be 1 (data=MC)
		//error should be the error
		int firstNonZeroBin = 0 ;
		int lastNonZeroBin = 0 ;
		for( int ibin = 1; ibin <= sysErr.GetNbinsX(); ++ibin )
		{
			if( firstNonZeroBin == 0 && sysErr.GetBinContent(ibin) > 0 )
				firstNonZeroBin = ibin;
			if( sysErr.GetBinContent(ibin) > 0 )
				lastNonZeroBin = ibin;

			double err = sysErr.GetBinContent(ibin);
			sysErr.SetBinContent( ibin, 1. );
			sysErr.SetBinError( ibin, err );
		}

		if( lastNonZeroBin < 0 ) //just in case
			lastNonZeroBin = sysErr.GetNbinsX();

		if( firstNonZeroBin != lastNonZeroBin ) {

			sysErr.GetXaxis()->SetRange( firstNonZeroBin, lastNonZeroBin );
			sysErr.SetFillColor( mc_error_color );
			sysErr.SetFillStyle( mc_error_style );
			sysErr.SetLineColor( mc_error_color );
			sysErr.SetLineWidth( mc_line_width );
			sysErr.SetMarkerStyle( 0 );
			sysErr.DrawCopy("E2 same ][");
			tmpRatio->DrawCopy("SAME AXIS");//get tickmarks back
			tmpRatio->DrawCopy("SAME X0"); //need to draw the ratio again
		}
	}

	//draw line at 1. if desired
	if( drawOneLine )
	{
		const TAxis *axis = tmpRatio->GetXaxis();
		double lowX  = axis->GetBinLowEdge( axis->GetFirst() );
		double highX = axis->GetBinUpEdge(  axis->GetLast() );

		TLine line;
		line.SetLineStyle(2);
		line.SetLineWidth(3);
		line.SetLineColor(36);
		line.DrawLine(lowX, 1., highX, 1.); //creates a new line which is owned by gPad
	}

	gPad->RedrawAxis();
	gPad->Update();
}

//==============================================================
// calculate the chi2 between two histograms
//==============================================================

Double_t MUPlotter::Chi2DataMC(
		const TH1* dataHist, 
		const TH1 *mcHist, 
		Int_t& ndf, 
		const Double_t mcScale /*= 1.0*/ 
		)
{
	TH1Ptr tmpData( (TH1D*)dataHist->Clone("tmp_data_chi2") );
	TH1Ptr tmpMC  ( (TH1D*)mcHist  ->Clone("tmp_mc_chi2"  ) );

	if( tmpData->GetSumw2N() == 0 ) 
		tmpData->Sumw2();
	if( tmpMC->GetSumw2N() == 0 )   
		tmpMC->Sumw2();

	tmpMC->Scale(mcScale);

	Double_t chi2 = 0;
	ndf = 0;

	const int lowBin  = tmpMC->GetXaxis()->GetFirst();
	const int highBin = tmpMC->GetXaxis()->GetLast();

	for( int i = lowBin; i <= highBin; ++i )
	{
		if( tmpData->GetBinError(i)*tmpData->GetBinError(i) + tmpMC->GetBinError(i)*tmpMC->GetBinError(i) > 0 )
		{
			chi2 += (tmpData->GetBinContent(i) - tmpMC->GetBinContent(i))
				*(tmpData->GetBinContent(i) - tmpMC->GetBinContent(i))
				/(tmpData->GetBinError(i)*tmpData->GetBinError(i) + tmpMC->GetBinError(i)*tmpMC->GetBinError(i));

			++ndf;
		}
	}

	return chi2;

}

Double_t MUPlotter::Chi2DataMC( 
		const MUH1D* dataHist, 
		const MUH1D* mcHist, 
		Int_t& ndf,
		const Double_t mcScale,
		const bool useDataErrorMatrix,
		const bool useOnlyShapeErrors 
		)
{
	//We get the number of bins and make sure it's compatible with the NxN matrix given
	if ( dataHist->GetNbinsX() != mcHist->GetNbinsX() )
	{
		Error("MUPlotter::Chi2DataMC", "The number of bins from Data and MC histograms differ. Returning -1.");
		return -1.;
	}

	//only consider the plotted range
	const int lowBin  = mcHist->GetXaxis()->GetFirst();
	const int highBin = mcHist->GetXaxis()->GetLast();

	//Scaling MC to Data
	MUH1D* tmpMCHist = (MUH1D*) mcHist->Clone("tmpMCHist");
	tmpMCHist->Scale(mcScale);

	// Defining Error Matrix dimensions
	// Either use the requested range or the full error matrix with under/overflow
	Int_t Nbins = highBin - lowBin + 1;
	if( chi2_use_overflow_err )
		Nbins = tmpMCHist->GetNbinsX()+2;

	//get the covariance matrix
	TMatrixD covMatrix(Nbins, Nbins);
	{
		const Int_t NbinsTotal = tmpMCHist->GetNbinsX() + 2; //+2 for under/overflow
		TMatrixD covMatrixTmp( NbinsTotal, NbinsTotal );
		const bool includeStatError = true;
		const bool errorAsFraction  = false;

		// Use Error Matrix from Data or MC?
		if( useDataErrorMatrix )
		{
			covMatrixTmp = dataHist->GetTotalErrorMatrix( includeStatError, errorAsFraction, useOnlyShapeErrors);
			covMatrixTmp += tmpMCHist->GetStatErrorMatrix( );
		}
		else
		{
			covMatrixTmp = tmpMCHist->GetTotalErrorMatrix(  includeStatError, errorAsFraction, useOnlyShapeErrors);
			covMatrixTmp += dataHist->GetStatErrorMatrix( );
		}

		//select only covariance elements in the histogram range
		// unless using the contributions to covariance from overflow 
		if( chi2_use_overflow_err )
		{
			covMatrix = covMatrixTmp;
		}
		else
		{
			for( int i = 0; i != Nbins; ++i )
			{
				for( int j = 0; j != Nbins; ++j )
				{
					covMatrix[i][j] = covMatrixTmp[i+lowBin][j+lowBin];
				}
			}
		}

	}// end of block to get covarance

	//Now, we invert the covariance error Matrix and store the result in "errorMatrix"
	// Note: TDecompSVD can handle singular matrices
	TDecompSVD error(covMatrix);
	TMatrixD errorMatrix(covMatrix);
	if( ! error.Invert( errorMatrix ) )
	{
		Warning("MUPlotter::Chi2DataMC", "Cannot invert total covariance matrix.  Using statistical errors only for Chi2 calculation.");
		return Chi2DataMC( (TH1D*)dataHist, (TH1D*)mcHist, ndf, mcScale );
	}

	//Calculating chi2
	ndf = 0;
	Double_t chi2 = 0.;
	// under/overflow bins not taken into account in the chi2 calculation
	for (int i=lowBin; i<=highBin ; ++i)
	{
		const int iErrBin = i - lowBin;
		const Double_t x_data_i = dataHist->GetBinContent(i);
		const Double_t x_mc_i   = tmpMCHist->GetBinContent(i);

		for (int j=lowBin; j<=highBin; ++j)
		{
			const int jErrBin = j - lowBin;
			const Double_t x_data_j = dataHist->GetBinContent(j);
			const Double_t x_mc_j   = tmpMCHist->GetBinContent(j);
			const double chi2_ij = (x_data_i - x_mc_i) * errorMatrix[iErrBin][jErrBin] * (x_data_j - x_mc_j);
			chi2 += chi2_ij;
		}
		++ndf; // Is this the right way to calcualte ndf?
	}

	// if this is a shape comparison, subtract one degree of freedom?
	if(useOnlyShapeErrors)
		--ndf;

	delete tmpMCHist;

	return chi2;
}


Double_t MUPlotter::Chi2MCMC(
		const MUH1D* histA,
		const MUH1D* histB,
		Int_t& ndf,
		const Double_t bScale /*= 1.0 */,
		const bool useOnlyShapeErrors /*= false*/
		)
{
	ndf = 0;

	//We get the number of bins and make sure it's compatible with the NxN matrix given
	if ( histA->GetNbinsX() != histB->GetNbinsX() )
	{
		Error("MUPlotter::Chi2MCMC", "The number of bins from histA and histB histograms differ. Returning -1.");
		return -1.;
	}


	// Make a copy of B
	MUH1D bCopy(*histB);
	bCopy.SetName( "tmpBCopy" );
	bCopy.SetDirectory(0); //destroy when out of scope

	//take the ratio B/A and apply bScale
	bCopy.Divide( histB, histA, bScale );

	//create hist with all bin values of 1 with no error
	MUH1D oneHist( *dynamic_cast<const TH1D*>(histA) );
	for( int iBin = 0; iBin <= histA->GetNbinsX()+1; ++iBin )
	{
		oneHist.SetBinContent( iBin, 1. );
		oneHist.SetBinError(   iBin, 0. );
	}

	bCopy.GetXaxis()->SetRange( histA->GetXaxis()->GetFirst(), histA->GetXaxis()->GetLast() );

	// compare ratio to line of 1
	const double tmpScale = 1.;
	const bool useDataSys = false;
	const double chi2 = Chi2DataMC( &oneHist, &bCopy, ndf, tmpScale, useDataSys, useOnlyShapeErrors );

	return chi2;
}



void MUPlotter::DrawDataMCVariations(
		const MUH1D* dataHist,
		const TObjArray* mcHists,
		const Double_t mcScale     /*= 1.0   */,
		const std::string& legPos /*= "TR"  */,
		const bool dataAsPoints     /*= true  */,
		const bool allSolidLines    /*= false */,
		const bool leaveStyleAlone  /*= false */,
		const bool covAreaNormalize /*=false*/
		)
{
	unsigned int nHists = mcHists->GetEntries();

	vector<string> titles;
	titles.push_back( dataHist->GetTitle() );

	// Find the maximum among all histograms
	// be sure to consider a logY axis.
	// todo: when logY, we really want to know the minimum non-zero value
	double maxmax = dataHist->GetMaximum();
	double minmin = dataHist->GetMinimum();
	if( gPad->GetLogy() && minmin < 1.E-8 )
		minmin = -1.; //-1 for not set if the y-axis is log scale

	for( unsigned int i = 0; i != nHists; ++i ) 
	{
		MUH1D *mnv = (MUH1D*)mcHists->At(i);
		maxmax = std::max( maxmax, mcScale * mnv->GetMaximum() );
		if( 1.E-8 < mnv->GetMinimum() )
			minmin = std::min( minmin, mcScale * mnv->GetMinimum() );
		titles.push_back( mnv->GetTitle() );
	}
	if( gPad->GetLogy() )
		minmin = std::max( minmin, 1.E-8 );

	size_t titleWidth = GetLegendWidthInLetters( titles );
	double x1, x2, y1, y2;
	DecodeLegendPosition( x1, y1, x2, y2, legPos, nHists+1, titleWidth, legend_text_size );

	TLegend *leg  = new TLegend(x1, y1, x2, y2);
	leg->SetNColumns( legend_n_columns );
	static int legB_n = 0;
	leg->SetName( Form("legend B - %d", legB_n++) );
	AddToTmp( leg );

	leg->SetBorderSize( legend_border_size );
	leg->SetFillColor( legend_fill_color );
	if( legend_fill_color < 0 )
		leg->SetFillStyle(0);
	leg->SetTextSize( legend_text_size );
	leg->SetTextFont( legend_text_font );

	if( draw_normalized_to_bin_width && covAreaNormalize )
		Warning("MUPlotter::DrawDataMCVariations","Area normalized covariance matrix may be incorrect because it is taken after bin normalization.  See DrawDataMCWithErrorBand(MUH1Ds) for an example what to do");

	//create as clone because it gets added to the leged?
	TH1* tmpData(0);
	if( draw_normalized_to_bin_width )
		tmpData = (TH1*)dataHist->GetBinNormalizedCopy().GetCVHistoWithError(true, covAreaNormalize).Clone( Form("tmpData_%d", __LINE__) );
	else
		tmpData = (TH1*)dataHist->GetCVHistoWithError(true, covAreaNormalize).Clone( Form("tmpData_%d", __LINE__) );
	AddToTmp( tmpData );

	//respect max/min setting the user may have used
	if( MUHist::IsAutoAxisLimit( axis_minimum ) )
	{
		if( hist_min_zero && !gPad->GetLogy() )
			tmpData->SetMinimum( 0. );
		else
			tmpData->SetMinimum( footroom * minmin );
	}
	else
		tmpData->SetMinimum( axis_minimum );

	if( MUHist::IsAutoAxisLimit( axis_maximum ) )
	{
		if( gPad->GetLogy() )
			tmpData->SetMaximum( pow(headroom * maxmax,10.) );
		else
			tmpData->SetMaximum( headroom * maxmax );
	}
	else
		tmpData->SetMaximum( axis_maximum );


	//! Use ApplyNextLineStyle on data always, to make sure it resets and uses black
	if( ! leaveStyleAlone ) 
	{
		tmpData->SetMarkerStyle(data_marker);
		tmpData->SetMarkerSize(data_marker_size);
		ApplyNextLineStyle( tmpData, true, !allSolidLines );
		tmpData->GetXaxis()->SetTitleFont(axis_title_font_x);
		tmpData->GetYaxis()->SetTitleFont(axis_title_font_y);
		tmpData->GetXaxis()->SetTitleSize(axis_title_size_x);
		tmpData->GetYaxis()->SetTitleSize(axis_title_size_y);
		tmpData->GetXaxis()->SetLabelFont(axis_label_font);
		tmpData->GetYaxis()->SetLabelFont(axis_label_font);
		tmpData->GetXaxis()->SetLabelSize(axis_label_size);
		tmpData->GetYaxis()->SetLabelSize(axis_label_size);
		tmpData->GetXaxis()->CenterTitle(kTRUE);
	}

	if( dataAsPoints ) 
	{
		leg->AddEntry(tmpData,tmpData->GetTitle(),"ple");
		tmpData->Draw( "X0 E1" );
	}
	else
	{
		leg->AddEntry(tmpData,tmpData->GetTitle(),"l");
		tmpData->Draw( "HIST" );
	}

	//! Loop over the MC histograms.  Apply style, add to legend and draw.
	for( unsigned int i = 0; i < nHists; i++ ) 
	{
		const MUH1D *mnvMC = dynamic_cast<const MUH1D*>( mcHists->At(i) );
		if( !mnvMC )
		{
			Error( "DrawDataMCVariations", "Could not cast one of the MC histograms to MUH1D.  Your draw is incomplete." );
			continue;
		}


		TH1 *hst(0);
		if( draw_normalized_to_bin_width )
			hst = (TH1*)mnvMC->GetBinNormalizedCopy().GetCVHistoWithError(true, covAreaNormalize).Clone( Form("tmp_MCHist_%d_%d", i, __LINE__) );
		else
			hst = (TH1*)mnvMC->GetCVHistoWithError(true, covAreaNormalize).Clone( Form("tmp_MCHist_%d_%d", i, __LINE__) );
		AddToTmp( hst );

		if( ! leaveStyleAlone ) 
		{
			ApplyNextLineStyle( hst, false, !allSolidLines );
			hst->SetMarkerStyle(0);
		}
		leg->AddEntry( hst, mnvMC->GetTitle(), "l" );
		hst->Scale( mcScale );
		hst->Draw("HIST SAME");
	}

	if( legPos != "N" )
		leg->Draw();

	gPad->Update();
}





//================================================================
// adds a legend to the plot
//================================================================
void MUPlotter::AddPlotLegend( 
		const std::vector< TH1* >       & hists, 
		const std::vector< std::string >& names, 
		const std::vector< std::string >& opts,
		const std::string& legPos /*="R"*/ 
		)
{
	double x1, y1, x2, y2;
	size_t legendWidth = GetLegendWidthInLetters( names );
	DecodeLegendPosition( x1, y1, x2, y2, legPos, names.size(), legendWidth );
	AddPlotLegend( hists, names, opts, x1, y1, x2-x1, y2-y1 );
}

void MUPlotter::AddPlotLegend( 
		const std::vector< TH1* >       & hists, 
		const std::vector< std::string >& names, 
		const std::vector< std::string >& opts , 
		const double x, 
		const double y,
		const double x_width /*= 0.25*/,
		const double y_width /*= 0.15*/, 
		const double textSize /*= 0.02*/
		)
{
	if( hists.size() == 0 )
	{
		Error("MUPlotter::AddPlotLegend","You didn't give any histograms to  AddPlotLegend.");
		return;
	}

	if( hists.size() != names.size() )
	{
		Error("MUPlotter::AddPlotLegend", "You gave AddPlotLegend a different number of hists (%d) and names (%d)", (int)hists.size(), (int)names.size());
		return;
	}

	if( hists.size() != opts.size() )
	{
		Error("MUPlotter::AddPlotLegend", "You gave AddPlotLegend a different number of hists (%d) and opts (%d)", (int)hists.size(), (int)opts.size());
		return;
	}


	TLegend *leg = new TLegend(x ,y ,x + x_width,y + y_width);
	leg->SetNColumns( legend_n_columns );
	static int legN = 0;
	leg->SetName( Form("add_leg_N = %d", legN++) );
	AddToTmp( leg );

	leg->SetBorderSize( legend_border_size );
	leg->SetFillColor( legend_fill_color );
	if( legend_fill_color < 0 )
		leg->SetFillStyle(0);
	leg->SetTextSize( textSize );
	leg->SetTextFont( legend_text_font );
	for( unsigned int i = 0; i < hists.size(); i++ )
		leg->AddEntry(hists[i],names[i].c_str(),opts[i].c_str());
	leg->Draw();
	gPad->Update();
}


//=================================================
// print a canvas in multiple formats
//=================================================
void MUPlotter::MultiPrint( 
		TCanvas *c, 
		const std::string& name
		) const
{
	if( print_formats.size() == 0 )
	{
		Error("MultiPrint", "Cannot use default print formats because there are none!  Nothing will be printed.");
		return;
	}

	const string printName = name.size() ? name : std::string(c->GetName());
	string typeStr = print_formats.front();
	for( vector<string>::const_iterator i = ++print_formats.begin(); i != print_formats.end(); ++i )
		typeStr += "," + *i;

	MultiPrint( c, printName, typeStr );
}

//supply a comma-separated list of formats you want to print
void MUPlotter::MultiPrint( 
		TCanvas *c,
		const std::string& name, 
		const std::string& typeStr
		) const
{
	std::vector<std::string> types;
	size_t i = 0;
	size_t j = typeStr.find(',');
	while( j!=std::string::npos ){
		types.push_back( typeStr.substr(i,j-i) );
		i = ++j;
		j = typeStr.find(',',i);
	}
	if( j == std::string::npos )
		types.push_back( typeStr.substr(i, typeStr.size()) );

	//we don't need an info statement here...
	const int oldVerbosity = gErrorIgnoreLevel;
	gErrorIgnoreLevel = kWarning;

	for( vector<string>::const_iterator itType = types.begin(); itType != types.end(); ++itType )
	{
		if( print_topdir.empty() )
			c->Print( Form("%s.%s", name.c_str(), itType->c_str()), itType->c_str() );
		else
			c->Print( Form("%s/%s.%s", print_topdir.c_str(), name.c_str(), itType->c_str()), itType->c_str() );
	}

	gErrorIgnoreLevel = oldVerbosity;
}



//=========================================================
// get the mean of a histogram in some restricted range

double MUPlotter::GetHistoMean( 
		const TH1 * hist, 
		const int minBin /*= -1*/, 
		const int maxBin /*= -1*/ 
		) const
{
	double err = 0.0;
	return GetHistoMean( hist, err, minBin, maxBin );
}


double MUPlotter::GetHistoMean( 
		const TH1 * hist,
		double & err, 
		const int i_minBin /*= -1*/, 
		const int i_maxBin /*= -1*/
		) const
{
	double mean = 0.;
	err = 0.;

	int maxBin = i_maxBin, minBin = i_minBin;

	if( i_minBin == -1 )
		minBin = 1;
	if( i_maxBin == -1 )
		maxBin = hist->GetNbinsX();

	for( int i = minBin; i < maxBin; i++ )
	{
		if( hist->GetBinError(i) == 0.0 )
			continue;
		mean += hist->GetBinContent(i) / pow( hist->GetBinError(i), 2.0 );
		err += 1.0 / pow( hist->GetBinError(i), 2.0 );
	}

	if( err > 0. )
	{
		mean /= err;
		err = sqrt( 1.0 / err );
	}
	else
	{
		err = 0.;
		mean = 0.;
	}

	return mean;
}


//====================================================
// Draw a hexagon
//====================================================
//all in mm
void MUPlotter::DrawHex( 
		const double apothem,
		const double xCenter /*= 0.0*/,
		const double yCenter /*= 0.0*/ 
		) const
{
	/*
		 Common Values
		 fiducial volume    apothem = 850
		 outter edge of ID: apothem = 1070   (width = 2140 )
		 outter edge of OD: apothem = 1727.2 (width = 3454.4)

*/
	double w = apothem; // distance from center to nearest edge == apothem
	double h = apothem * 2.0 / sqrt(3); // distance from center to farthest point = length of a side = apothem / cos(pi/6)

	TLine line;
	line.SetLineWidth(2);
	line.DrawLine( xCenter    , yCenter + h    , xCenter + w, yCenter + h/2.0 );//top to upper right
	line.DrawLine( xCenter + w, yCenter + h/2.0, xCenter + w, yCenter - h/2.0 ); //upper right to lower right
	line.DrawLine( xCenter + w, yCenter - h/2.0, xCenter    , yCenter - h     ); //lower right to bottom
	line.DrawLine( xCenter    , yCenter - h    , xCenter - w, yCenter - h/2.0 );//bottom to lower left
	line.DrawLine( xCenter - w, yCenter - h/2.0, xCenter - w, yCenter + h/2.0 ); //lower left to upper left
	line.DrawLine( xCenter - w, yCenter + h/2.0, xCenter    , yCenter + h     ); //upper left to top

	gPad->Update();
}

//////////////////////////////////////////////
// Reverse the axis
// usually TObject is a histogram
//
//////////////////////////////////////////////
void MUPlotter::ReverseXAxis( TH1 *h )
{
	if( !h )
	{
		Warning("MUPlotter::ReverseXAxis"," I can't reverse and axis if you pass me a NULL object.");
		return;
	}

	// Remove the current axis
	h->GetXaxis()->SetLabelOffset(999);
	h->GetXaxis()->SetTickLength(0);

	// Redraw the new axis 
	gPad->Update();
	TGaxis *newaxis = new TGaxis(gPad->GetUxmax(), 
			gPad->GetUymin(),
			gPad->GetUxmin(),
			gPad->GetUymin()-.0001,
			h->GetXaxis()->GetXmin(),
			h->GetXaxis()->GetXmax(),
			510,"+");
	AddToTmp( newaxis );


	newaxis->SetLabelOffset(0.03);
	newaxis->SetLabelSize(0.03);
	newaxis->Draw();
}

void MUPlotter::ReverseXAxis( TH2 *h )
{
	if( !h )
	{
		Warning("MUPlotter::ReverseXAxis"," I can't reverse and axis if you pass me a NULL object.");
		return;
	}

	// Remove the current axis
	h->GetXaxis()->SetLabelOffset(999);
	h->GetXaxis()->SetTickLength(0);

	// Redraw the new axis 
	gPad->Update();
	TGaxis *newaxis = new TGaxis(gPad->GetUxmax(),
			gPad->GetUymin(),
			gPad->GetUxmin(),
			gPad->GetUymin()-.0001,
			h->GetXaxis()->GetXmin(),
			h->GetXaxis()->GetXmax(),
			510,"+");
	AddToTmp( newaxis );


	newaxis->SetLabelOffset(0.03);
	newaxis->SetLabelSize(0.03);
	newaxis->Draw();
}

//===========================================
// Find the useful range in bins
//===========================================
bool MUPlotter::GetBinRangeWithMinimumBinContent( 
		const TH1 *h,
		int &lowbin, 
		int &highbin,
		double minContentLow /*= 0.*/,
		double minContentHigh /*= 0.*/ 
		) const
{
	lowbin = -1;
	highbin = h->GetNbinsX() + 2;
	for( int ibin = 0; ibin <= h->GetNbinsX(); ++ibin )
	{
		if( lowbin == -1 && h->GetBinContent(ibin) > minContentLow )
			lowbin = ibin;
		if( h->GetBinContent(ibin) > minContentHigh )
			highbin = ibin;
	}
	return ( lowbin != -1 && highbin !=  h->GetNbinsX() + 2 );
}

bool MUPlotter::GetNonZeroBinRange( 
		const TH1 *h, 
		int &lowbin, 
		int &highbin 
		) const
{
	return GetBinRangeWithMinimumBinContent( h, lowbin, highbin, 0., 0. );
}


bool MUPlotter::GetHistsRange( const TObjArray& hists, double& minmin, double& maxmax ) const
{
	maxmax = minmin = 0.;

	if( hists.IsEmpty() )
	{
		Warning("MUPlotter::GetHistsMax", "You gave me an empty array.  You get 0 in return.");
		return false;
	}

	const TH1 *firstHist = dynamic_cast<const TH1*>( hists.At(0) );
	if(!firstHist)
	{
		Error("MUPlotter::GetHistsMax", "Objects in the array must be castable to TH1*");
		return false;
	}
	minmin = firstHist->GetBinContent( firstHist->GetMinimumBin() );
	maxmax = firstHist->GetBinContent( firstHist->GetMaximumBin() );


	for( Int_t i = 1, nhists=hists.GetEntriesFast(); i != nhists; ++i )
	{
		const TH1 *hist = dynamic_cast<const TH1*>( hists.At(i) );
		if(!hist)
		{
			Error("MUPlotter::GetHistsMax", "Objects in the array must be castable to TH1*");
			minmin = maxmax = 0.;
			return false;
		}
		minmin = std::min( minmin, hist->GetBinContent( hist->GetMinimumBin() ) );
		maxmax = std::min( maxmax, hist->GetBinContent( hist->GetMaximumBin() ) );
	}

	return true;
}



//=================================================
// Create bins suitable for plotting in log10 scale
//=================================================
bool MUPlotter::SetLogBins( 
		const int nbins, 
		const double min, 
		const double max, 
		double *bins 
		)
{
	if( min == 0  || max == 0 ){
		Error("SetLogBins","Min and Max bins must be positive.");
		return false;
	}
	const double lmin =  log10(min);
	const double lmax =  log10(max);
	const double width = (double)(lmax - lmin) / nbins;
	bins[0] = min;
	for( int i=1; i <= nbins; ++i )
		bins[i] = min + pow(10, lmin + i*width);


	return true;
}

//=======================================
// Get histograms of errors using user defined error groups
//=======================================
std::vector<TH1*> MUPlotter::GetSysErrorGroupHists( 
		MUH1D* h,
		const bool doFractional/* = false */,
		const bool covAreaNormalize/* = false*/,
		const double ignoreThreshold /* = 0.00001 */
		) const
{
	//the return vector
	vector<TH1*> hists;

	if( ! h )
	{
		Error("MUPlotter::GetSysErrorGroupHists", "You passed me a NULL MUH1D.  Nothing to do.");
		return hists;
	}


	map<string,TH1D*> errGroupHists;

	vector<string> vertNames = h->GetVertErrorBandNames();
	for( unsigned int i = 0; i != vertNames.size(); ++i )
	{
		const MUVertErrorBand *errBand = h->GetVertErrorBand(vertNames[i]);
		TH1D *hErr = (TH1D*)errBand->GetErrorBand( doFractional, covAreaNormalize ).Clone(Form("tmp_vert_error%d_%d", i, __LINE__) );
		hErr->SetTitle( vertNames[i].c_str() );

		//is this histogram part of a group?
		bool inGroup = false;
		for(ErrorSummaryGroupMap::const_iterator itGroup = error_summary_group_map.begin(); itGroup != error_summary_group_map.end(); ++itGroup )
		{
			const string& errName = itGroup->first;
			const vector<string>& histNames = itGroup->second;

			//if this histogram is in the group add it to the group
			if( find( histNames.begin(), histNames.end(), vertNames[i]) != histNames.end() )
			{
				map<string,TH1D*>::iterator itGroupHist = errGroupHists.find(errName);

				//if this group has no histogram yet, add it
				//otherwise, add in quadrature
				if( errGroupHists.end() == itGroupHist )
					errGroupHists[errName] = hErr;
				else
				{
					MUHist::AddInQuadrature( itGroupHist->second, hErr );
					delete hErr;
				}

				inGroup = true;
				break;
			}
		}
		//if we added to the group, move on
		if( inGroup )
			continue;

		//delet and move on if it is too small
		if( 0 < ignoreThreshold && hErr->GetBinContent( hErr->GetMaximumBin() ) < ignoreThreshold )
		{
			delete hErr;
			continue;
		}

		hists.push_back( hErr );
	}

	// Plot each of the fractional contributions from lateral errors
	vector<string> latNames = h->GetLatErrorBandNames();
	for( unsigned int i = 0; i != latNames.size(); ++i )
	{
		const MULatErrorBand *errBand = h->GetLatErrorBand( latNames[i] );
		TH1D *hErr = (TH1D*)errBand->GetErrorBand( doFractional, covAreaNormalize ).Clone(Form("tmp_lat_error%d_%d", i, __LINE__) );
		hErr->SetTitle( latNames[i].c_str() );

		//is this histogram part of a group?
		bool inGroup = false;
		for(ErrorSummaryGroupMap::const_iterator itGroup = error_summary_group_map.begin(); itGroup != error_summary_group_map.end(); ++itGroup )
		{
			const string& errName = itGroup->first;
			const vector<string>& histNames = itGroup->second;

			//if this histogram is in the group add it to the group
			if( find( histNames.begin(), histNames.end(), latNames[i]) != histNames.end() )
			{
				map<string,TH1D*>::iterator itGroupHist = errGroupHists.find(errName);

				//if this group has no histogram yet, add it
				//otherwise, add in quadrature
				if( errGroupHists.end() == itGroupHist )
					errGroupHists[ errName ] = hErr;
				else
				{
					MUHist::AddInQuadrature( itGroupHist->second, hErr );
					delete hErr;
				}

				inGroup = true;
				break;
			}
		}
		//if we added to the group, move on
		if( inGroup )
			continue;

		//delete and move on if too small
		if( 0 < ignoreThreshold && hErr->GetBinContent( hErr->GetMaximumBin() ) < ignoreThreshold )
		{
			delete hErr;
			continue;
		}

		hists.push_back( hErr );
	}


	// Plot each of the fractional contributions from uncorrelated sys errors
	vector<string> uncorrNames = h->GetUncorrErrorNames();
	for( unsigned int i = 0; i != uncorrNames.size(); ++i )
	{
		TH1D *hErr = (TH1D*)h->GetUncorrErrorAsHist( uncorrNames[i], doFractional ).Clone(Form("tmp_uncorr_error%d_%d", i, __LINE__) );
		hErr->SetTitle( uncorrNames[i].c_str() );

		//is this histogram part of a group?
		bool inGroup = false;
		for(ErrorSummaryGroupMap::const_iterator itGroup = error_summary_group_map.begin(); itGroup != error_summary_group_map.end(); ++itGroup )
		{
			const string& errName = itGroup->first;
			const vector<string>& histNames = itGroup->second;

			//if this histogram is in the group add it to the group
			if( find( histNames.begin(), histNames.end(), uncorrNames[i]) != histNames.end() )
			{
				map<string,TH1D*>::iterator itGroupHist = errGroupHists.find(errName);

				//if this group has no histogram yet, add it
				//otherwise, add in quadrature
				if( errGroupHists.end() == itGroupHist )
					errGroupHists[ errName ] = hErr;
				else
				{
					MUHist::AddInQuadrature( itGroupHist->second, hErr );
					delete hErr;
				}

				inGroup = true;
				break;
			}
		}
		//if we added to the group, move on
		if( inGroup )
			continue;

		//delete and move on if too small
		if( 0 < ignoreThreshold && hErr->GetBinContent( hErr->GetMaximumBin() ) < ignoreThreshold )
		{
			delete hErr;
			continue;
		}

		hists.push_back( hErr ); 
	}//end of uncorr errors

	//add error groups
	for( map<string,TH1D*>::iterator itGroup = errGroupHists.begin(); itGroup != errGroupHists.end(); ++itGroup )
	{
		TH1* hist = itGroup->second;

		if( 0 < ignoreThreshold && hist->GetBinContent( hist->GetMaximumBin() ) < ignoreThreshold )
		{
			//throw away an error group if it is too small, or it will leak
			delete hist;
			continue;
		}
		hist->SetTitle( itGroup->first.c_str() );

		hists.push_back( hist );
	}

	//YOU OWN ALL THESE OBJECTS!
	return hists;
}

//=======================================
// Draw a summary of MC only errors
//=======================================
bool MUPlotter::DrawErrorSummary( 
		MUH1D* h, 
		const std::string& legPos  /* = "TR"    */, 
		const bool   includeStat     /* = true    */, 
		const bool   solidLinesOnly  /* = true    */,
		const double ignoreThreshold /* = 0.00001 */,
		const bool covAreaNormalize/* = false*/,
		const std::string& errorGroupName /* = "" */
		)
{
	if( ! h )
	{
		Error("DrawErrorSummary", "You passed me a NULL MUH1D.  Nothing to do.");
		return false;
	}

	//set max digits to the default, since we almost never want scientific notation for errors.
	//restore the setting before returning.
	const int oldMaxDigits = TGaxis::GetMaxDigits();
	TGaxis::SetMaxDigits( axis_max_digits );


	// Store the pieces for a legend
	vector<TH1*>   hists;
	vector<string> names;
	vector<string> opts;

	bool useDifferentLineStyles = !solidLinesOnly;

	//! Get the total error and apply styles
	TH1D *hTotalErr = (TH1D*)h->GetTotalError( includeStat, true, covAreaNormalize ).Clone( Form("h_total_err_errSum_%d", __LINE__) );
	AddToTmp( hTotalErr );
	ApplyAxisStyle(hTotalErr);

	ApplyNextLineStyle( hTotalErr, true, useDifferentLineStyles );

	//respect max/min setting the user may have used
	if( MUHist::IsAutoAxisLimit( axis_minimum ) )
		hTotalErr->SetMinimum( 0. );
	else
		hTotalErr->SetMinimum( axis_minimum );

	if( MUHist::IsAutoAxisLimit( axis_maximum ) )
		hTotalErr->SetMaximum( headroom * hTotalErr->GetMaximum() );
	else
		hTotalErr->SetMaximum( axis_maximum );

	hTotalErr->GetYaxis()->SetTitle( "Fractional Uncertainty" );
	if (errorGroupName == "") {
		hTotalErr->Draw( "HIST" );

		const string totalName = ( includeStat ? "Total Error" : "Total Sys. Error" );
		hists.push_back( hTotalErr ); names.push_back( totalName ); opts.push_back( "l" );
	}

	if( includeStat && errorGroupName == "")
	{
		TH1D *statErr = (TH1D*)h->GetStatError(true).Clone( Form("this_stat_err_%d", __LINE__) );
		AddToTmp( statErr );

		statErr->SetLineColor( 12 );//dark gray
		statErr->SetLineStyle( 2 ); //dashed
		statErr->SetLineWidth( 3 );
		statErr->Draw("HIST SAME");
		hists.push_back( statErr ); names.push_back( "Statistical" ); opts.push_back( "l" );
	}

	map<string,TH1D*> errGroupHists;

	//! Plot each of the fractional contributions from vertical errors
	bool plotted_1st = (errorGroupName == "") ? true : false;
	vector<string> vertNames = h->GetVertErrorBandNames();
	for( unsigned int i = 0; i != vertNames.size(); ++i )
	{
		TH1D *hErr = (TH1D*)h->GetVertErrorBand(vertNames[i])->GetErrorBand( true, covAreaNormalize ).Clone( Form("tmp_vertError_%d_%d", i, __LINE__) );

		//is this histogram part of a group?
		bool inGroup = false;
		for(ErrorSummaryGroupMap::const_iterator itGroup = error_summary_group_map.begin(); itGroup != error_summary_group_map.end(); ++itGroup )
		{
			const string& errName = itGroup->first;
			const vector<string>& histNames = itGroup->second;

			//if this histogram is in the group add it to the group
			if( find( histNames.begin(), histNames.end(), vertNames[i]) != histNames.end() )
			{

				//for plotting the errors from only one group
				if (errorGroupName==errName) {
					inGroup=true;
					break;
				}
				else if (errorGroupName == "") {

					map<string,TH1D*>::iterator itGroupHist = errGroupHists.find(errName);

					//if this group has no histogram yet, add it
					//otherwise, add in quadrature
					if( errGroupHists.end() == itGroupHist )
						errGroupHists[ errName ] = hErr;
					else
					{
						MUHist::AddInQuadrature( itGroupHist->second, hErr );
						delete hErr;
					}

					inGroup = true;
					break;
				}
			}
		}
		//if we added to the group, move on
		if( inGroup && errorGroupName=="")
			continue;

		if (errorGroupName=="" || inGroup) {
			AddToTmp( hErr );

			if( 0 < ignoreThreshold && hErr->GetBinContent( hErr->GetMaximumBin() ) < ignoreThreshold )
				continue;

			ApplyNextLineStyle( hErr, false, useDifferentLineStyles);

			map<string,int>::const_iterator itErrCol = error_color_map.find( vertNames[i] );
			if( error_color_map.end() != itErrCol )
				hErr->SetLineColor( itErrCol->second );

			hErr->SetLineWidth( mc_line_width );
			if (plotted_1st) {
				hErr->Draw( "HIST SAME" );
			}
			else {
				plotted_1st = true;
				ApplyAxisStyle(hErr);
				//respect max/min setting the user may have used
				if( MUHist::IsAutoAxisLimit( axis_minimum ) )
					hErr->SetMinimum( 0. );
				else
					hErr->SetMinimum( axis_minimum );

				if( MUHist::IsAutoAxisLimit( axis_maximum ) )
					hErr->SetMaximum( headroom * hTotalErr->GetMaximum() );
				else
					hErr->SetMaximum( axis_maximum );

				hErr->GetYaxis()->SetTitle( "Fractional Uncertainty" );
				hErr->Draw("HIST");
			}
			hists.push_back( hErr ); names.push_back( vertNames[i] ); opts.push_back( "l" );
		}
	}

	//! Plot each of the fractional contributions from lateral errors
	vector<string> latNames = h->GetLatErrorBandNames();
	for( unsigned int i = 0; i != latNames.size(); ++i )
	{
		TH1D *hErr = (TH1D*)h->GetLatErrorBand(latNames[i])->GetErrorBand( true, covAreaNormalize ).Clone( Form("tmp_latError_%d_%d", i, __LINE__) );

		//is this histogram part of a group?
		bool inGroup = false;
		for(ErrorSummaryGroupMap::const_iterator itGroup = error_summary_group_map.begin(); itGroup != error_summary_group_map.end(); ++itGroup )
		{
			const string& errName = itGroup->first;
			const vector<string>& histNames = itGroup->second;

			//if this histogram is in the group add it to the group
			if( find( histNames.begin(), histNames.end(), latNames[i]) != histNames.end() )
			{       

				//for plotting the errors from only one group
				if (errorGroupName==errName) {
					inGroup=true;
					break;
				}
				else if (errorGroupName == "") {

					map<string,TH1D*>::iterator itGroupHist = errGroupHists.find(errName);

					//if this group has no histogram yet, add it
					//otherwise, add in quadrature
					if( errGroupHists.end() == itGroupHist )
						errGroupHists[ errName ] = hErr;
					else
					{
						MUHist::AddInQuadrature( itGroupHist->second, hErr );
						delete hErr;
					}

					inGroup = true;
					break;
				}
			}
		}
		//if we added to the group, move on
		if( inGroup && errorGroupName=="")
			continue;

		if (errorGroupName=="" || inGroup) {
			AddToTmp( hErr );

			if( 0 < ignoreThreshold && hErr->GetBinContent( hErr->GetMaximumBin() ) < ignoreThreshold )
				continue;

			ApplyNextLineStyle( hErr, false, useDifferentLineStyles);

			map<string,int>::const_iterator itErrCol = error_color_map.find( latNames[i] );
			if( error_color_map.end() != itErrCol )
				hErr->SetLineColor( itErrCol->second );

			hErr->SetLineWidth( mc_line_width );
			if (plotted_1st) {
				hErr->Draw( "HIST SAME" );
			}
			else {
				plotted_1st = true;
				ApplyAxisStyle(hErr);
				//respect max/min setting the user may have used
				if( MUHist::IsAutoAxisLimit( axis_minimum ) )
					hErr->SetMinimum( 0. );
				else
					hErr->SetMinimum( axis_minimum );

				if( MUHist::IsAutoAxisLimit( axis_maximum ) )
					hErr->SetMaximum( headroom * hTotalErr->GetMaximum() );
				else
					hErr->SetMaximum( axis_maximum );

				hErr->GetYaxis()->SetTitle( "Fractional Uncertainty" );
				hErr->Draw("HIST");
			}
			hists.push_back( hErr ); names.push_back( latNames[i] ); opts.push_back( "l" );
		}
	}


	// Plot each of the fractional contributions from uncorrelated sys errors
	vector<string> uncorrNames = h->GetUncorrErrorNames();
	for( unsigned int i = 0; i != uncorrNames.size(); ++i )
	{
		TH1D *hErr = (TH1D*)h->GetUncorrErrorAsHist( uncorrNames[i], true ).Clone(Form("tmp_uncorr_error%d_%d", i, __LINE__) );

		//is this histogram part of a group?
		bool inGroup = false;
		for(ErrorSummaryGroupMap::const_iterator itGroup = error_summary_group_map.begin(); itGroup != error_summary_group_map.end(); ++itGroup )
		{
			const string& errName = itGroup->first;
			const vector<string>& histNames = itGroup->second;

			//if this histogram is in the group add it to the group
			if( find( histNames.begin(), histNames.end(), uncorrNames[i]) != histNames.end() )
			{       

				//for plotting the errors from only one group
				if (errorGroupName==errName) {
					inGroup=true;
					break;
				}
				else if (errorGroupName == "") {

					map<string,TH1D*>::iterator itGroupHist = errGroupHists.find(errName);

					//if this group has no histogram yet, add it
					//otherwise, add in quadrature
					if( errGroupHists.end() == itGroupHist )
						errGroupHists[ errName ] = hErr;
					else
					{
						MUHist::AddInQuadrature( itGroupHist->second, hErr );
						delete hErr;
					}

					inGroup = true;
					break;
				}
			}
		}
		//if we added to the group, move on
		if( inGroup && errorGroupName=="")
			continue;

		if (errorGroupName=="" || inGroup) {
			AddToTmp( hErr );

			if( 0 < ignoreThreshold && hErr->GetBinContent( hErr->GetMaximumBin() ) < ignoreThreshold )
				continue;

			ApplyNextLineStyle( hErr, false, useDifferentLineStyles);

			map<string,int>::const_iterator itErrCol = error_color_map.find( uncorrNames[i] );
			if( error_color_map.end() != itErrCol )
				hErr->SetLineColor( itErrCol->second );

			hErr->SetLineWidth( mc_line_width );
			if (plotted_1st) {
				hErr->Draw( "HIST SAME" );
			}
			else {
				plotted_1st = true;
				ApplyAxisStyle(hErr);
				//respect max/min setting the user may have used
				if( MUHist::IsAutoAxisLimit( axis_minimum ) )
					hErr->SetMinimum( 0. );
				else
					hErr->SetMinimum( axis_minimum );

				if( MUHist::IsAutoAxisLimit( axis_maximum ) )
					hErr->SetMaximum( headroom * hTotalErr->GetMaximum() );
				else
					hErr->SetMaximum( axis_maximum );

				hErr->GetYaxis()->SetTitle( "Fractional Uncertainty" );
				hErr->Draw("HIST");
			}
			hists.push_back( hErr ); names.push_back( uncorrNames[i] ); opts.push_back( "l" );
		}
	}//end of uncorr errors

	//add error groups
	for( map<string,TH1D*>::iterator itGroup = errGroupHists.begin(); itGroup != errGroupHists.end(); ++itGroup )
	{
		const string& name = itGroup->first;
		TH1* hist = itGroup->second;

		if( 0 < ignoreThreshold && hist->GetBinContent( hist->GetMaximumBin() ) < ignoreThreshold )
			continue;

		ApplyNextLineStyle( hist, false, useDifferentLineStyles);

		map<string,int>::const_iterator itErrCol = error_color_map.find( name );
		if( error_color_map.end() != itErrCol )
			hist->SetLineColor( itErrCol->second );

		hist->SetLineWidth( mc_line_width );
		hist->Draw( "HIST SAME" );
		hists.push_back( hist ); names.push_back( name ); opts.push_back( "l" );
	}

	if( legPos != "N" )
	{
		size_t legendWidth = GetLegendWidthInLetters( names );
		double x1,y1,x2,y2;
		DecodeLegendPosition( x1, y1, x2, y2, legPos, hists.size(), legendWidth );
		AddPlotLegend( hists, names, opts, x1, y1, x2-x1, y2-y1, legend_text_size );
	}

	gPad->RedrawAxis();
	gPad->Update();

	TGaxis::SetMaxDigits( oldMaxDigits );

	return true;
}


bool MUPlotter::DrawDataMCErrorSummary(
		MUH1D* hMC,
		MUH1D* hData,
		const std::string& legPos  /* = "TR"    */,
		const bool   includeMCStat  /* = false    */,
		const bool   solidLinesOnly  /* = true    */,
		const double ignoreThreshold /* = 0.00001 */,
		const bool covAreaNormalize/* = false*/,
		const std::string& errorGroupName /*=""*/
		)
{
	if( !hMC || !hData )
	{
		Error("DrawDataMCErrorSummary", "You passed me a NULL MUH1D.  Nothing to do.");
		return false;
	}

	//! Store the pieces for a legend
	vector<TH1*>   hists;
	vector<string> names;
	vector<string> opts;

	bool useDifferentLineStyles = !solidLinesOnly;

	//! Get the total error and apply styles
	TH1D *hTotalErr = (TH1D*)hMC->GetTotalError( includeMCStat, true, covAreaNormalize ).Clone( Form("tmp_total_err_%d", __LINE__) );
	AddToTmp( hTotalErr );
	ApplyAxisStyle(hTotalErr);

	ApplyNextLineStyle( hTotalErr, true, useDifferentLineStyles );

	//! Add stat error from data to total error
	TH1D *hStatErr = (TH1D*)hData->GetStatError(true).Clone( Form("tmp_stat_err_%d", __LINE__) );
	AddToTmp( hStatErr );

	{
		TH1D hStatErrMC = hMC->GetStatError(true);
		for(int ibin = 0; ibin != hStatErr->GetNbinsX() + 1; ++ibin )
		{
			const double err2 = pow( hStatErr->GetBinContent( ibin ), 2 ) + pow( hTotalErr->GetBinContent( ibin ), 2 );
			const double err  = (err2 == 0) ? 0 : sqrt(err2);
			hTotalErr->SetBinContent( ibin, err );

			//! Add MC stat error to total stat error if desired
			if( includeMCStat )
			{
				const double statErr2 = pow( hStatErr->GetBinContent( ibin ), 2 ) + pow( hStatErrMC.GetBinContent( ibin ), 2 );
				const double statErr  = (statErr2 == 0) ? 0 : sqrt(statErr2);
				hStatErr->SetBinContent( ibin, statErr );
			}
		}
	}

	//respect max/min setting the user may have used
	if( MUHist::IsAutoAxisLimit( axis_minimum ) )
		hTotalErr->SetMinimum( 0. );
	else
		hTotalErr->SetMinimum( axis_minimum );

	if( MUHist::IsAutoAxisLimit( axis_maximum ) )
		hTotalErr->SetMaximum( headroom * hTotalErr->GetMaximum() );
	else
		hTotalErr->SetMaximum( axis_maximum );


	hTotalErr->GetYaxis()->SetTitle( "Fractional Uncertainty" );
	hTotalErr->Draw( "HIST" );

	const string totalName = "Total Error";
	hists.push_back( hTotalErr ); names.push_back( totalName ); opts.push_back( "l" );


	//! Draw the statistical error
	hStatErr->SetLineColor( 12 );//dark gray
	hStatErr->SetLineStyle( 2 ); //dashed
	hStatErr->SetLineWidth( 3 );
	hStatErr->Draw("HIST SAME");
	const string statErrName = ( includeMCStat ? "Statistical (Data & MC)" : "Statistical" );
	hists.push_back( hStatErr ); names.push_back( statErrName ); opts.push_back( "l" );

	map<string,TH1D*> errGroupHists;

	//! Plot each of the fractional contributions from vertical errors
	vector<string> vertNames = hMC->GetVertErrorBandNames();
	for( unsigned int i = 0; i != vertNames.size(); ++i )
	{
		TH1D *hErr = (TH1D*)hMC->GetVertErrorBand(vertNames[i])->GetErrorBand( true, covAreaNormalize ).Clone( Form("tmp_vertError_%d_%d", i, __LINE__) );

		//is this histogram part of a group?
		bool inGroup = false;
		for(ErrorSummaryGroupMap::const_iterator itGroup = error_summary_group_map.begin(); itGroup != error_summary_group_map.end(); ++itGroup )
		{
			const string& errName = itGroup->first;
			const vector<string>& histNames = itGroup->second;

			//if this histogram is in the group add it to the group
			if( find( histNames.begin(), histNames.end(), vertNames[i]) != histNames.end() )
			{

				//for plotting the errors from only one group
				if (errorGroupName==errName) {
					inGroup=true;
					break;
				}
				else if (errorGroupName=="") {

					map<string,TH1D*>::iterator itGroupHist = errGroupHists.find(errName);

					//if this group has no histogram yet, add it
					//otherwise, add in quadrature
					if( errGroupHists.end() == itGroupHist )
						errGroupHists[ errName ] = hErr;
					else
					{
						MUHist::AddInQuadrature( itGroupHist->second, hErr );
						delete hErr;
					}

					inGroup = true;
					break;
				}
			}
		}
		//if we added to the group, move on
		if( inGroup && errorGroupName=="")
			continue;

		if (errorGroupName=="" || inGroup) {
			AddToTmp( hErr );

			if( 0 < ignoreThreshold && hErr->GetBinContent( hErr->GetMaximumBin() ) < ignoreThreshold )
				continue;

			ApplyNextLineStyle( hErr, false, useDifferentLineStyles);

			map<string,int>::const_iterator itErrCol = error_color_map.find( vertNames[i] );
			if( error_color_map.end() != itErrCol )
				hErr->SetLineColor( itErrCol->second );

			hErr->SetLineWidth( mc_line_width );

			/* for flux highlight
				 if( itErrCol->first == "Flux" )
				 {
				 hErr->SetLineWidth(8);
				 hErr->SetLineStyle(9);
				 hErr->SetLineColor(kRed);
				 }
				 */

			hErr->Draw( "HIST SAME" );
			hists.push_back( hErr ); names.push_back( vertNames[i] ); opts.push_back( "l" );
		}
	}

	//! Plot each of the fractional contributions from lateral errors
	vector<string> latNames = hMC->GetLatErrorBandNames();
	for( unsigned int i = 0; i != latNames.size(); ++i )
	{
		TH1D *hErr = (TH1D*)hMC->GetLatErrorBand(latNames[i])->GetErrorBand( true, covAreaNormalize ).Clone( Form("tmp_latError_%d_%d", i, __LINE__) );

		//is this histogram part of a group?
		bool inGroup = false;
		for(ErrorSummaryGroupMap::const_iterator itGroup = error_summary_group_map.begin(); itGroup != error_summary_group_map.end(); ++itGroup )
		{
			const string& errName = itGroup->first;
			const vector<string>& histNames = itGroup->second;

			//if this histogram is in the group add it to the group
			if( find( histNames.begin(), histNames.end(), latNames[i]) != histNames.end() )
			{       

				//for plotting the errors from only one group
				if (errorGroupName==errName) {
					inGroup=true;
					break;
				}
				else if (errorGroupName=="") {

					map<string,TH1D*>::iterator itGroupHist = errGroupHists.find(errName);

					//if this group has no histogram yet, add it
					//otherwise, add in quadrature
					if( errGroupHists.end() == itGroupHist )
						errGroupHists[ errName ] = hErr;
					else
					{
						MUHist::AddInQuadrature( itGroupHist->second, hErr );
						delete hErr;
					}

					inGroup = true;
					break;
				}
			}
		}
		//if we added to the group, move on
		if( inGroup && errorGroupName=="")
			continue;

		if (errorGroupName=="" || inGroup) {
			AddToTmp( hErr );

			if( 0 < ignoreThreshold && hErr->GetBinContent( hErr->GetMaximumBin() ) < ignoreThreshold )
				continue;

			ApplyNextLineStyle( hErr, false, useDifferentLineStyles);

			map<string,int>::const_iterator itErrCol = error_color_map.find( latNames[i] );
			if( error_color_map.end() != itErrCol )
				hErr->SetLineColor( itErrCol->second );

			hErr->SetLineWidth( mc_line_width );
			hErr->Draw( "HIST SAME" );
			hists.push_back( hErr ); names.push_back( latNames[i] ); opts.push_back( "l" );
		}
	}


	// Plot each of the fractional contributions from uncorrelated sys errors
	vector<string> uncorrNames = hMC->GetUncorrErrorNames();
	for( unsigned int i = 0; i != uncorrNames.size(); ++i )
	{
		TH1D *hErr = (TH1D*)hMC->GetUncorrErrorAsHist( uncorrNames[i], true ).Clone(Form("tmp_uncorr_error%d_%d", i, __LINE__) );

		//is this histogram part of a group?
		bool inGroup = false;
		for(ErrorSummaryGroupMap::const_iterator itGroup = error_summary_group_map.begin(); itGroup != error_summary_group_map.end(); ++itGroup )
		{
			const string& errName = itGroup->first;
			const vector<string>& histNames = itGroup->second;

			//if this histogram is in the group add it to the group
			if( find( histNames.begin(), histNames.end(), uncorrNames[i]) != histNames.end() )
			{       

				//for plotting the errors from only one group
				if (errorGroupName==errName) {
					inGroup=true;
					break;
				}
				else if (errorGroupName=="") {

					map<string,TH1D*>::iterator itGroupHist = errGroupHists.find(errName);

					//if this group has no histogram yet, add it
					//otherwise, add in quadrature
					if( errGroupHists.end() == itGroupHist )
						errGroupHists[ errName ] = hErr;
					else
					{
						MUHist::AddInQuadrature( itGroupHist->second, hErr );
						delete hErr;
					}

					inGroup = true;
					break;
				}
			}
		}
		//if we added to the group, move on
		if( inGroup && errorGroupName=="")
			continue;

		if (errorGroupName=="" || inGroup) {
			AddToTmp( hErr );

			if( 0 < ignoreThreshold && hErr->GetBinContent( hErr->GetMaximumBin() ) < ignoreThreshold )
				continue;

			ApplyNextLineStyle( hErr, false, useDifferentLineStyles);

			map<string,int>::const_iterator itErrCol = error_color_map.find( uncorrNames[i] );
			if( error_color_map.end() != itErrCol )
				hErr->SetLineColor( itErrCol->second );

			hErr->SetLineWidth( mc_line_width );
			hErr->Draw( "HIST SAME" );
			hists.push_back( hErr ); names.push_back( uncorrNames[i] ); opts.push_back( "l" );
		}
	}//end of uncorr errors

	//add error groups
	for( map<string,TH1D*>::iterator itGroup = errGroupHists.begin(); itGroup != errGroupHists.end(); ++itGroup )
	{
		const string& name = itGroup->first;
		TH1* hist = itGroup->second;

		if( 0 < ignoreThreshold && hist->GetBinContent( hist->GetMaximumBin() ) < ignoreThreshold )
			continue;

		ApplyNextLineStyle( hist, false, useDifferentLineStyles);

		map<string,int>::const_iterator itErrCol = error_color_map.find( name );
		if( error_color_map.end() != itErrCol )
			hist->SetLineColor( itErrCol->second );

		hist->SetLineWidth( mc_line_width );
		hist->Draw( "HIST SAME" );
		hists.push_back( hist ); names.push_back( name ); opts.push_back( "l" );
	}

	if( legPos != "N" )
	{
		size_t legendWidth = GetLegendWidthInLetters( names );
		double x1,y1,x2,y2;
		DecodeLegendPosition( x1, y1, x2, y2, legPos, hists.size(), legendWidth );
		AddPlotLegend( hists, names, opts, x1, y1, x2-x1, y2-y1, legend_text_size );
	}

	gPad->RedrawAxis();
	gPad->Update();

	return true;
}


//==============================================================
// DrawStackedMC - vector of hists to turn into a stack
//==============================================================  
void MUPlotter::DrawStackedMC(
		const TObjArray* mcHists,
		const Double_t mcScale,
		const std::string& legPos,
		const Int_t mcBaseColor,               // Color of first stacked histo.
		const Int_t mcColorOffset,             // 2nd histo in stack has color mcBaseColor+mcColorOffset, etc.
		const Int_t mcFillStyle,               // Fill style for histograms only.
		const char* xaxislabel,
		const char* yaxislabel
		)
{

	//----------------------
	// start creating legend
	//----------------------

	unsigned int nHists = mcHists->GetEntries();
	if (nHists==0) return;

	//find the longest title in each column
	vector<string> titles;
	for( unsigned int i = 0; i != nHists; ++i )
	{
		MUH1D *mnv = (MUH1D*)mcHists->At(i);
		titles.push_back( mnv->GetTitle() );
	}

	size_t legendWidth = GetLegendWidthInLetters( titles );
	double x1, x2, y1, y2;
	DecodeLegendPosition( x1, y1, x2, y2, legPos, nHists+1, legendWidth, legend_text_size );

	TLegend *leg  = new TLegend(x1, y1, x2, y2);
	leg->SetNColumns( legend_n_columns );
	AddToTmp(leg);

	leg->SetNColumns( legend_n_columns );
	leg->SetBorderSize( legend_border_size );
	leg->SetFillColor( legend_fill_color );
	if( legend_fill_color < 0 )
		leg->SetFillStyle(0);
	leg->SetTextSize( legend_text_size );
	leg->SetTextFont( legend_text_font );

	THStack *hs  = new THStack("hs", "Stacked 1D histograms");
	AddToTmp(hs);

	TH1     *hst(NULL);
	MUH1D  *mnvhst(NULL);

	int fillCol = mcBaseColor;
	std::string first_xaxis_label, first_yaxis_label;
	for( unsigned int i = 0; i < nHists; i++ ) 
	{
		fillCol += i*mcColorOffset;
		mnvhst = (MUH1D*)mcHists->At(i);

		if( draw_normalized_to_bin_width )
			hst    = (TH1*)mnvhst->GetBinNormalizedCopy().GetCVHistoWithError().Clone( Form( "tmpMC_%04d_%d", i, __LINE__) );
		else
			hst    = (TH1*)mnvhst->GetCVHistoWithError().Clone( Form( "tmpMC_%04d_%d", i, __LINE__) );
		AddToTmp( hst );

		if( mcBaseColor > 0 )
			hst->SetFillColor( fillCol );
		if( mcFillStyle > 0 )
			hst->SetFillStyle( mcFillStyle );
		hst->SetLineWidth(mc_line_width);
		hst->Scale(mcScale);
		hs->Add( hst );

		if (i==0) {
			first_xaxis_label = hst->GetXaxis()->GetTitle();
			first_yaxis_label = hst->GetYaxis()->GetTitle();
		}
	}

	for( unsigned int i = 0; i != nHists; ++i )
	{

		mnvhst = (MUH1D*)mcHists->At(nHists-1-i);
		hst    = (TH1*)hs->GetHists()->At(nHists-1-i);
		leg->AddEntry( hst, mnvhst->GetTitle(), "f" );
	}

	//HACK -need to draw a smaller histogram first to set the x-axis range correctly 
	//for stacks with variable-width bins
	MUH1D* tmp_mnv = (MUH1D*)hs->GetHists()->At(0);
	tmp_mnv->GetXaxis()->SetTitle( xaxislabel );
	tmp_mnv->GetYaxis()->SetTitle( yaxislabel );
	tmp_mnv->GetXaxis()->SetTitleFont(axis_title_font_x);
	tmp_mnv->GetYaxis()->SetTitleFont(axis_title_font_y);
	tmp_mnv->GetXaxis()->SetTitleSize(axis_title_size_x);
	tmp_mnv->GetYaxis()->SetTitleSize(axis_title_size_y);
	tmp_mnv->GetXaxis()->SetLabelFont(axis_label_font);
	tmp_mnv->GetYaxis()->SetLabelFont(axis_label_font);
	tmp_mnv->GetXaxis()->SetLabelSize(axis_label_size);
	tmp_mnv->GetYaxis()->SetLabelSize(axis_label_size);
	tmp_mnv->GetXaxis()->CenterTitle(kTRUE);
	if( strlen(xaxislabel) == 0 )
		tmp_mnv->GetXaxis()->SetTitle( first_xaxis_label.c_str() );
	if( strlen(yaxislabel) == 0 )
		tmp_mnv->GetYaxis()->SetTitle( first_yaxis_label.c_str() );

	//respect max/min setting the user may have used
	if( MUHist::IsAutoAxisLimit( axis_minimum ) )
	{
		if( hist_min_zero && !gPad->GetLogy() )
			tmp_mnv->SetMinimum( 0. );
		else
			tmp_mnv->SetMinimum( footroom * hs->GetMinimum() );
	}
	else
		tmp_mnv->SetMinimum( axis_minimum );

	if( MUHist::IsAutoAxisLimit( axis_maximum ) )
		tmp_mnv->SetMaximum( headroom * hs->GetMaximum() );
	else
		tmp_mnv->SetMaximum( axis_maximum );


	tmp_mnv->Draw("HIST");
	hs->Draw("SAME HIST");
	hs->Draw("SAME AXIS");

	if( legPos != "N" )
		leg->Draw();

	gPad->Update();
}




//==============================================================
// DrawDataStackedMC - vector of hists to turn into a stack & "data"
//==============================================================

void MUPlotter::DrawDataStackedMC(
		const MUH1D* dataHist,               
		const TObjArray* mcHists,
		const Double_t mcScale,
		const std::string& legPos,
		const std::string& dataName,
		const Int_t mcBaseColor,               // Color of first stacked histo.
		const Int_t mcColorOffset,             // 2nd histo in stack has color mcBaseColor+mcColorOffset, etc.
		const Int_t mcFillStyle,               // Fill style for histograms only.
		const char* xaxislabel,
		const char* yaxislabel
		)
{

	//----------------------
	// start creating legend
	//----------------------

	unsigned int nHists = mcHists->GetEntries();
	vector<string> titles;
	titles.push_back( dataName );
	for( unsigned int i = 0; i != nHists; ++i )
	{
		MUH1D *mnv = (MUH1D*)mcHists->At(i);
		titles.push_back( mnv->GetTitle() );
	}

	size_t legendWidth = GetLegendWidthInLetters( titles );
	double x1, x2, y1, y2;
	DecodeLegendPosition( x1, y1, x2, y2, legPos, nHists+1, legendWidth, legend_text_size );

	TLegend *leg  = new TLegend(x1, y1, x2, y2);
	leg->SetNColumns( legend_n_columns );
	AddToTmp(leg);

	leg->SetBorderSize( legend_border_size );
	leg->SetFillColor( legend_fill_color );
	if( legend_fill_color < 0 )
		leg->SetFillStyle(0);
	leg->SetTextSize( legend_text_size );
	leg->SetTextFont( legend_text_font );

	THStack *hs  = new THStack("hs", "Stacked 1D histograms");
	AddToTmp(hs);

	TH1     *hst(NULL);
	MUH1D  *mnvhst(NULL);

	// Note, Sumw2 is enforced automatically for MUH1D's.
	TH1* tmpData(0);
	if( draw_normalized_to_bin_width )
		tmpData = (TH1*)dataHist->GetBinNormalizedCopy().GetCVHistoWithError().Clone( Form("tmp_data_%d", __LINE__) );
	else
		tmpData = (TH1*)dataHist->GetCVHistoWithError().Clone( Form("tmp_data_%d", __LINE__) );
	AddToTmp( tmpData );

	//style data
	tmpData->SetMarkerStyle(data_marker);
	tmpData->SetMarkerSize(data_marker_size);
	tmpData->SetLineWidth(data_line_width);
	tmpData->SetLineStyle(data_line_style);
	tmpData->SetLineColor(data_color);

	leg->AddEntry(tmpData,dataName.c_str(),"ple");
	int fillCol = mcBaseColor;
	for( unsigned int i = 0; i < nHists; i++ ) 
	{
		fillCol += i*mcColorOffset;
		mnvhst = (MUH1D*)mcHists->At(i);

		if( draw_normalized_to_bin_width )
			hst    = (TH1*)mnvhst->GetBinNormalizedCopy().GetCVHistoWithError().Clone( Form( "tmpMC_%04d_%d", i, __LINE__) );
		else
			hst    = (TH1*)mnvhst->GetCVHistoWithError().Clone( Form( "tmpMC_%04d_%d", i, __LINE__) );
		AddToTmp( hst );

		if( mcBaseColor > 0 )
			hst->SetFillColor( fillCol );
		if( mcFillStyle > 0 )
			hst->SetFillStyle( mcFillStyle );
		hst->SetLineWidth(mc_line_width);
		hst->Scale(mcScale);
		hs->Add( hst );
	}

	//add the legend in reverse order so vertical alignment is same as stack
	for( unsigned int i = 0; i != nHists; ++i )
	{

		mnvhst = (MUH1D*)mcHists->At(nHists-1-i);
		hst    = (TH1*)hs->GetHists()->At(nHists-1-i);
		leg->AddEntry( hst, mnvhst->GetTitle(), "f" );
	}

	//HACK -need to draw a smaller histogram first to set the x-axis range correctly 
	//for stacks with variable-width bins
	MUH1D* tmp_mnv = (MUH1D*)hs->GetHists()->At(0);
	tmp_mnv->GetXaxis()->SetRange(  tmpData->GetXaxis()->GetFirst(), tmpData->GetXaxis()->GetLast() );
	tmp_mnv->GetXaxis()->SetTitle( xaxislabel );
	tmp_mnv->GetYaxis()->SetTitle( yaxislabel );
	tmp_mnv->GetXaxis()->SetTitleFont(axis_title_font_x);
	tmp_mnv->GetYaxis()->SetTitleFont(axis_title_font_y);
	tmp_mnv->GetXaxis()->SetTitleSize(axis_title_size_x);
	tmp_mnv->GetYaxis()->SetTitleSize(axis_title_size_y);
	tmp_mnv->GetXaxis()->SetLabelFont(axis_label_font);
	tmp_mnv->GetYaxis()->SetLabelFont(axis_label_font);
	tmp_mnv->GetXaxis()->SetLabelSize(axis_label_size);
	tmp_mnv->GetYaxis()->SetLabelSize(axis_label_size);
	tmp_mnv->GetXaxis()->CenterTitle(kTRUE);
	if( strlen(xaxislabel) == 0 )
		tmp_mnv->GetXaxis()->SetTitle( dataHist->GetXaxis()->GetTitle() );
	if( strlen(yaxislabel) == 0 )
		tmp_mnv->GetYaxis()->SetTitle( dataHist->GetYaxis()->GetTitle() );

	//respect max/min setting the user may have used
	if( MUHist::IsAutoAxisLimit( axis_minimum ) )
	{
		if( hist_min_zero && !gPad->GetLogy() )
			tmp_mnv->SetMinimum( 0. );
		else
			tmp_mnv->SetMinimum( footroom * std::min( tmpData->GetMinimum(), hs->GetMinimum() ) );
	}
	else
		tmp_mnv->SetMinimum( axis_minimum );

	if( MUHist::IsAutoAxisLimit( axis_maximum ) )
		tmp_mnv->SetMaximum( headroom * std::max( tmpData->GetMaximum(), hs->GetMaximum() ) );
	else
		tmp_mnv->SetMaximum( axis_maximum );


	tmp_mnv->Draw("HIST");
	hs->Draw("HIST SAME");

	tmpData->SetMarkerStyle(data_marker);
	tmpData->SetMarkerSize(data_marker_size);
	tmpData->SetLineWidth(data_line_width);
	tmpData->SetLineStyle(data_line_style);
	tmpData->SetLineColor(data_color);
	tmpData->Draw("SAME E1 X0");
	tmp_mnv->Draw("AXIS SAME");

	if( legPos != "N" )
		leg->Draw();



	gPad->Update();
}


//==============================================================
// DrawDataStackedMCWithErrorBand - vector of hists to turn into a stack & "data"
//==============================================================
void MUPlotter::DrawDataStackedMCWithErrorBand( ) 
{
	std::cout << "Implement me by editing DrawDataStackedMC()!" << std::endl;
}

void MUPlotter::DrawNormalizedMigrationHistogram(
		const TH2D* h_migration,
		const bool drawAsMatrix,
		const bool coarseContours /* = false */
		)
{

	Int_t nbins = h_migration->GetNbinsX()+2; //under/overflow bins

	TMatrixD m_migration(nbins,nbins);
	TH2D tmp(*h_migration);
	tmp.Reset();
	for (int y=0; y<nbins; ++y)
	{
		Double_t norm = 0.;
		for (int x=0; x<nbins; ++x)
			norm += h_migration->GetBinContent(x,y);

		if (fabs(norm)>1E-8)
		{
			for (int x=0; x<nbins; ++x)
			{
				double percentage =  100 * h_migration->GetBinContent(x,y) / norm;
				m_migration[y][x] = percentage; //yeah that's right  y/x
				tmp.SetBinContent( x, y, percentage);
			}
		}
	}

	if( drawAsMatrix )
	{
		tmp = TH2D( m_migration );
		tmp.GetXaxis()->SetTitle( "Reco Bins" );
		tmp.GetYaxis()->SetTitle( "True Bins" );
	}
	tmp.GetXaxis()->SetTitleOffset( axis_title_offset_x );
	tmp.GetYaxis()->SetTitleOffset( axis_title_offset_y );
	tmp.GetZaxis()->SetTitleOffset( axis_title_offset_z );
	tmp.GetZaxis()->SetTitle( "Fraction of Row in Cell" );

	if( coarseContours )
	{
		//set a low,ok,high,too high color scale
		double contours[20] = {
			0, 0.00001, 0.0002, 0.0003, 0.0004,
			25, 25.00001, 25.0002, 25.0003, 25.0004,
			50, 50.00001, 50.0002, 50.0003, 50.0004,
			75, 75.00001, 75.0002, 75.0003, 75.0004};
		tmp.SetContour( 20, contours );
	}

	gStyle->SetPaintTextFormat("2.0f");
	tmp.DrawCopy("colz text");
}

void MUPlotter::DrawAllUniverses(
		const MUH1D *h,
		const bool covAreaNormalize, /* = false */ 
		const bool binWidthNormalize /* = true */
		)
{
	//! Figure out how to divide the canvas to fit all plots
	const int nplots   = h->GetErrorBandNames().size();
	const int nrows    = (int)( sqrt( nplots ) );
	int ncolumns = (int)( nplots/nrows );
	if( nplots%nrows!=0 ) 
		++ncolumns;

	//have to cast gPad to TVirtualPad to divide it (not sure why)
	TVirtualPad *thePad = gPad;
	thePad->Divide( ncolumns, nrows );

	//! Plot all vertical error bands
	int nplot = 0;
	vector<string> names;
	names = h->GetVertErrorBandNames();
	for( vector<string>::iterator name=names.begin(); name!=names.end(); ++name ){
		const MUVertErrorBand *error = h->GetVertErrorBand( *name );
		++nplot;
		thePad->cd( nplot );
		//! copy the MUH1D axes attributes to the error band, so they are propagates to the draw
		h->GetXaxis()->Copy( *error->GetXaxis() );
		h->GetYaxis()->Copy( *error->GetYaxis() );
		error->DrawAll( "HIST", true/*draw CV*/, covAreaNormalize, binWidthNormalize ? h->GetNormBinWidth() : 0.0 );//0.0 means do not bin width normalize
		AddHistoTitle( (*name).c_str() );
	}

	//! Plot all vertical error bands
	names = h->GetLatErrorBandNames();
	for( vector<string>::iterator name=names.begin(); name!=names.end(); ++name ){
		const MULatErrorBand *error = h->GetLatErrorBand( *name );
		++nplot;
		thePad->cd(nplot);
		//! copy the MUH1D axes attributes to the error band, so they are propagates to the draw
		h->GetXaxis()->Copy( *error->GetXaxis() );
		h->GetYaxis()->Copy( *error->GetYaxis() );
		error->DrawAll( "HIST", true/*draw CV*/, covAreaNormalize, binWidthNormalize ? h->GetNormBinWidth() : 0.0 );//0.0 means do not bin width normalize
		AddHistoTitle( (*name).c_str() );
	}

}

void MUPlotter::DrawErrorMatrices( 
		const MUH1D *h, 
		const bool area_norm, 
		const bool asCorr, 
		const bool asFrac 
		)
{

	// Figure out how to divide the canvas to fit all plots
	// one plot for each systematic, one for stat, one for total
	const int nplots   = h->GetErrorBandNames().size() + 2;
	const int nrows    = (int)( sqrt( nplots ) );
	int ncolumns = (int)( nplots/nrows );
	if( nplots%nrows!=0 ) 
		++ncolumns;

	//have to cast gPad to TVirtualPad to divide it (not sure why)
	TVirtualPad *thePad = gPad;
	thePad->Divide( ncolumns, nrows );

	// draw systematic error matrices 
	int nplot = 0;
	vector<string> names;
	names = h->GetSysErrorMatricesNames();
	for( vector<string>::iterator name=names.begin(); name!=names.end(); ++name )
	{

		++nplot;
		thePad->cd(nplot);

		TMatrixD sysMatrix = asCorr ? h->GetSysCorrelationMatrix( *name, area_norm ) : h->GetSysErrorMatrix( *name, asFrac, area_norm );
		DrawErrorMatrix( sysMatrix, h->GetXaxis() );

		if( asCorr )
		{
			this->AddHistoTitle( Form( "%s Cor. Matrix", (*name).c_str() ) );
		}
		else{
			this->AddHistoTitle( Form( "%s Cov. Matrix", (*name).c_str() ) );
		}

	}

	string name = "";

	// draw the statistical error matrix 
	// no correlation for stat. matrix
	if( ! asCorr )
	{
		name = "Stat.";
		++nplot;
		thePad->cd(nplot);
		TMatrixD statMatrix =  h->GetStatErrorMatrix( asFrac );
		DrawErrorMatrix( statMatrix , h->GetXaxis() );
		this->AddHistoTitle( Form( "%s Cov. Matrix", name.c_str() ) );
	}

	// draw the total error matrix 
	name = "Total";
	++nplot;
	thePad->cd(nplot);
	TMatrixD totalMatrix = asCorr ? h->GetTotalCorrelationMatrix( area_norm ) : h->GetTotalErrorMatrix( true, asFrac, area_norm );
	DrawErrorMatrix( totalMatrix, h->GetXaxis() );
	if( asCorr )
		this->AddHistoTitle( Form( "%s Cor. Matrix", name.c_str() ) );
	else
		this->AddHistoTitle( Form( "%s Cov. Matrix", name.c_str() ) );
}

void MUPlotter::DrawErrorMatrix( 
		const TMatrixD &matrix, 
		const TAxis* axis )
{

	// create a 2D histogram with the matrix elements 
	TH2D *h2D = new TH2D( "h_matrix",
			Form( "matrix;%s;%s", axis->GetTitle(), axis->GetTitle()),
			axis->GetNbins(), axis->GetXbins()->GetArray(),
			axis->GetNbins(), axis->GetXbins()->GetArray() );

	for( int i = 0; i < matrix.GetNrows(); i++ )
	{
		for( int j = 0; j < matrix.GetNcols(); j++ )
		{
			int sign = matrix[i][j]>0 ? 1 : -1;
			h2D->SetBinContent( h2D->GetBin(i,j), sign*sqrt( fabs( matrix[i][j] ) ) );
		}
	}

	h2D->GetXaxis()->SetRange( axis->GetFirst(), axis->GetLast() );
	h2D->GetYaxis()->SetRange( axis->GetFirst(), axis->GetLast() );
	h2D->SetTitleSize( axis->GetTitleSize(), "XY");

	// set minimum and maximum by type
	const double min = h2D->GetBinContent( h2D->GetMinimumBin() );
	const double max = h2D->GetBinContent( h2D->GetMaximumBin() );

	// for correlation matrix fix maximum to 1.
	// this test usually means this is a correlation matrix.
	if( draw_corr_max1 && 0 <= min && min < 1. && 0 < max && max < 1. )
	{
		h2D->SetMinimum( floor( min*10. )/10. );
		h2D->SetMaximum( 1.0 );
	}
	else
	{

		// separate minimum and maximum if they are very close
		if( (max-min)<0.1 )
		{
			int middle = (int)floor( 0.5+(min+max)/2.0 );
			h2D->SetMinimum( middle-0.5 );
			h2D->SetMaximum( middle+0.5 );
		}

		// if minimum is negative and maximum is positive
		// center z axis in zero and make limits symetric
		else if( min*max<0 )
		{
			const double absmax = std::max( fabs(min), fabs(max) );
			h2D->SetMinimum( -absmax );
			h2D->SetMaximum(  absmax );
		}

		//
		// I don't understand why one would want to always separate by at least 1,
		// but I don't want to force the change.
		// I think the symmetric min/max is what we normally would want
		if( draw_corr_red_blue )
		{
			const double absmax = std::max( fabs(min), fabs(max) );
			h2D->SetMinimum( -absmax );
			h2D->SetMaximum(  absmax );

		}

	}

	// draw the 2d histogram
	if( draw_corr_red_blue )
		SetCorrelationPalette();

	h2D->DrawCopy( "colz" );

	//if( draw_corr_red_blue )
	//  gStyle->SetPalette( palette_style );

	// clean up 
	delete h2D;

}

void MUPlotter::DrawDoubleGausFit(
		const  TH1D *h,
		double lowFitBound,
		double highFitBound,
		const char* legPos,
		double* parameters,
		double* errors,
		double  chisquared,
		int     ndf )
{

	TH1D *htmp = (TH1D*)h->Clone( "htmp" );

	// if the boundaries are not set, use the histogram to get the initial guess parameters and bin edges 
	double lowbin  = lowFitBound;
	double highbin = highFitBound;

	if( lowFitBound == highFitBound ) 
	{
		lowbin  = htmp->GetBinLowEdge( htmp->GetMaximumBin() - 4 );
		highbin = htmp->GetBinLowEdge( htmp->GetMaximumBin() + 4 );
	} 

	// get an initial guess at the fit parameters
	TF1* tmp_fit = new TF1("tmp_fit","gaus",lowbin,highbin);
	htmp->Fit(tmp_fit,"QR");

	double tmp_pars[3];
	tmp_fit->GetParameters(tmp_pars);

	double minbin    = lowFitBound == highFitBound ? htmp->GetXaxis()->GetXmin() : lowbin;
	double maxbin    = lowFitBound == highFitBound ? htmp->GetXaxis()->GetXmax() : highbin;
	double maxvalue  = htmp->GetMaximum();
	double rmsvalue  = tmp_pars[2];
	double meanvalue = tmp_pars[1];
	delete tmp_fit;

	// fit function
	TF1* fit = new TF1("fit","gaus(0)+gaus(3)",minbin,maxbin);
	fit->SetLineColor(data_color);

	// set initial guess parameters
	fit->SetParameters(maxvalue,meanvalue,rmsvalue,maxvalue/4.0,meanvalue,2*rmsvalue);

	// fit the histogram
	htmp->Fit(fit,"QR");

	// get the parameters  
	double pars[6];
	fit->GetParameters(pars);

	if( parameters != (double*)NULL ) 
	{
		for(int i = 0; i < 6; ++i) 
			parameters[i] = pars[i];   
	}

	double* errs = fit->GetParErrors();

	if( errors != (double*)NULL ) 
	{
		for(int i = 0; i < 6; ++i) 
			errors[i] = fabs(errs[i]);
	}

	chisquared = fit->GetChisquare();
	ndf = fit->GetNDF();

	// set attributes
	if( MUHist::IsAutoAxisLimit( axis_maximum ) ) 
		htmp->SetMaximum( headroom * htmp->GetMaximum() );
	else 
		htmp->SetMaximum( axis_maximum );

	htmp->GetXaxis()->SetTitleFont(axis_title_font_x);
	htmp->GetYaxis()->SetTitleFont(axis_title_font_y);
	htmp->GetXaxis()->SetTitleSize(axis_title_size_x);
	htmp->GetYaxis()->SetTitleSize(axis_title_size_y);
	htmp->GetXaxis()->SetLabelFont(axis_label_font);
	htmp->GetYaxis()->SetLabelFont(axis_label_font);
	htmp->GetXaxis()->SetLabelSize(axis_label_size);
	htmp->GetYaxis()->SetLabelSize(axis_label_size);
	htmp->GetXaxis()->CenterTitle(kTRUE);

	htmp->SetLineColor(data_color);
	htmp->SetLineWidth(data_line_width);
	htmp->SetLineStyle(data_line_style);
	htmp->SetMarkerStyle(data_marker);
	htmp->SetMarkerSize(data_marker_size);
	htmp->SetMarkerColor(data_color);

	int gaus1_color = kRed;
	int gaus2_color = kBlue;

	int line_style  = 1;

	// draw histogram
	htmp->DrawCopy("p e1 x0"); 

	// draw curves
	double min2 = htmp->GetXaxis()->GetXmin();
	double max2 = htmp->GetXaxis()->GetXmax();  

	double min1 = min2/2.0;
	double max1 = max2/2.0;

	TF1 *f1 = new TF1("f1","gaus",min1,max1);
	TF1 *f2 = new TF1("f2","gaus",min2,max2);

	f1->SetLineColor(gaus1_color);  
	f1->SetLineStyle(line_style); 
	f1->SetLineWidth(mc_line_width);

	f2->SetLineColor(gaus2_color); 
	f2->SetLineStyle(line_style); 
	f2->SetLineWidth(mc_line_width);

	f1->SetParameters(pars);     
	f2->SetParameters(&pars[3]); 

	f1->Draw("same");
	f2->Draw("same");

	// print parameters information
	int text_align = 11;
	int index[4]   = { 1, 2, 4, 5 };

	double x1, y1, x2, y2;
	DecodeLegendPosition( x1, y1, x2, y2, legPos, 1,10 );

	for(int i = 0; i < 4; ++i) 
	{
		int    text_color = i < 2 ? gaus1_color : gaus2_color;
		string text_line  = Form("%s = %.2f#pm%.2f",i%2==0?"#mu":"#sigma",pars[index[i]],fabs(errs[index[i]]));
		AddPlotLabel(text_line.c_str(),x1,y1,legend_text_size,text_color,legend_text_font,text_align);    
		y1 -= i == 1 ? 0.055 : 0.037;
	}

	y1 -= 0.05;

	string text_line = Form("#chi^{2} / ndf = %.2f / %d",fit->GetChisquare(),fit->GetNDF());
	AddPlotLabel(text_line.c_str(),x1,y1,legend_text_size,kBlack,legend_text_font,text_align);

	gPad->Update();

	// clean up
	delete htmp;
	delete fit;
}


//#####################################################################################
//
// END
//
//#####################################################################################

#endif
