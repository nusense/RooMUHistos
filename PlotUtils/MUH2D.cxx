#ifndef MNV_MUH2D_cxx
#define MNV_MUH2D_cxx 1

#include "PlotUtils/MUH2D.h"

using namespace PlotUtils;

//==================================================================================
// CONSTRUCTORS
//==================================================================================

//--------------------------------------------------------
// Copy constructors with default normalized bin width
//--------------------------------------------------------
MUH2D::MUH2D() : 
	TH2D(),
	fNormBinWidthX(1.),
	fNormBinWidthY(1.)
{ 
	Sumw2(); 
}

MUH2D::MUH2D(Double_t normBinWidthX, Double_t normBinWidthY) :
	TH2D(),
	fNormBinWidthX(normBinWidthX),
	fNormBinWidthY(normBinWidthY)
{
	Sumw2();
}

MUH2D::MUH2D( const TH2D& h2d ) :
	TH2D( h2d ) 
{ 
	fNormBinWidthX = h2d.GetXaxis()->GetBinWidth(1);
	fNormBinWidthY = h2d.GetYaxis()->GetBinWidth(1);
	Sumw2();
}

//----------------------------------------------------------------------------------
// Copy constructors with specified normalized bin width for the X and Y projections
//----------------------------------------------------------------------------------
MUH2D::MUH2D(const TH2D& h2d, Double_t normBinWidthX, Double_t normBinWidthY) :
	TH2D( h2d ),
	fNormBinWidthX(normBinWidthX),
	fNormBinWidthY(normBinWidthY)
{
	Sumw2();
}

//--------------------------------------------------------
// Constructors with default normalization bin width
//--------------------------------------------------------
MUH2D::MUH2D( const char* name, const char* title, Int_t nbinsx, const Float_t* xbins, Int_t nbinsy, const Float_t* ybins) :
	TH2D( name, title, nbinsx, xbins, nbinsy, ybins) 
{ 
	//! Default normlization bin width is the first bin width
	fNormBinWidthX = xbins[1] - xbins[0];
	fNormBinWidthY = ybins[1] - ybins[0];
	Sumw2();
}

MUH2D::MUH2D( const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, const Double_t* ybins) :
	TH2D( name, title, nbinsx, xbins, nbinsy, ybins) 
{ 
	//! Default normlization bin width is the first bin width
	fNormBinWidthX = xbins[1] - xbins[0];
	fNormBinWidthY = ybins[1] - ybins[0];
	Sumw2();
}

MUH2D::MUH2D( const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup):
	TH2D( name, title, nbinsx, xlow, xup, nbinsy, ylow, yup )
{
	//! Default normalization bin width is the constant width of the bins
	fNormBinWidthX = (xup - xlow) / float(nbinsx);
	fNormBinWidthY = (yup - ylow) / float(nbinsy);
	Sumw2();
}

//! Deep copy constructor (the real copy)
MUH2D::MUH2D( const MUH2D& h ) :
	TH2D( h )
{
	//! Deep copy the variables
	DeepCopy( h );
}

MUH2D& MUH2D::operator=( const MUH2D& h )
{
	//! If this is me, then no copy is necessary
	if( this == &h )
		return *this;

	//call the base class assignment operator
	this->TH2D::operator=(h);

	//! Delete and clear all vert and lat error bands
	for( std::map<std::string, MUVertErrorBand2D*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
		delete it->second;
	fVertErrorBandMap.clear();

	for( std::map<std::string, MULatErrorBand2D*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
		delete it->second;
	fLatErrorBandMap.clear();

	//! Then deep copy the variables
	DeepCopy(h);

	return *this;
}

void MUH2D::DeepCopy( const MUH2D& h )
{
	//! Set bin norm width
	fNormBinWidthX = h.GetNormBinWidthX();
	fNormBinWidthY = h.GetNormBinWidthY();

	//! Copy the vert and lat error bands
	std::vector<std::string> vertNames = h.GetVertErrorBandNames();
	for( std::vector<std::string>::iterator name = vertNames.begin(); name != vertNames.end(); ++name )
		fVertErrorBandMap[*name] = new MUVertErrorBand2D( *h.GetVertErrorBand(*name) );

	std::vector<std::string> latNames = h.GetLatErrorBandNames();
	for( std::vector<std::string>::iterator name = latNames.begin(); name != latNames.end(); ++name )
		fLatErrorBandMap[*name] = new MULatErrorBand2D( *h.GetLatErrorBand(*name) );
}

//--------------------------------------------------------
// Constructors with specified normalization bin width
//--------------------------------------------------------
MUH2D::MUH2D( const char* name, const char* title, Int_t nbinsx, const Float_t* xbins, Int_t nbinsy, const Float_t* ybins, Double_t normBinWidthX, Double_t normBinWidthY) :
	TH2D( name, title, nbinsx, xbins, nbinsy, ybins),
	fNormBinWidthX(normBinWidthX),
	fNormBinWidthY(normBinWidthY)
{ 
	Sumw2();
}

MUH2D::MUH2D( const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, const Double_t* ybins, Double_t normBinWidthX, Double_t normBinWidthY) :
	TH2D( name, title, nbinsx, xbins, nbinsy, ybins),
	fNormBinWidthX(normBinWidthX),
	fNormBinWidthY(normBinWidthY)
{ 
	Sumw2();
}

MUH2D::MUH2D( const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Double_t normBinWidthX, Double_t normBinWidthY):
	TH2D( name, title, nbinsx, xlow, xup, nbinsy, ylow, yup ),
	fNormBinWidthX(normBinWidthX),
	fNormBinWidthY(normBinWidthY)
{
	Sumw2();
}

//------------------------------------------------------------------------
// Get an MUH2D which has its bin content and errors normalized to bin width so it looks smooth
//------------------------------------------------------------------------
MUH2D MUH2D::GetBinNormalizedCopy( Double_t normBinWidthX /* = fNormBinWidthX */, Double_t normBinWidthY /* = fNormBinWidthY */ ) const
{
	//! If normBinWidthX or normBinWidthXY is negative, use the default bin width for this MUH2D for the respective axis
	if( normBinWidthX <= 0 )
		normBinWidthX = fNormBinWidthX;
	if( normBinWidthY <= 0 )
		normBinWidthY = fNormBinWidthY;

	MUH2D rval(*this);

	if( normBinWidthX > 0 &&  normBinWidthY > 0 )
		rval.Scale( normBinWidthX * normBinWidthY, "width" );

	return rval;
}

MUH1D * MUH2D::ProjectionX(const char* name /*= "_px"*/, Int_t firstybin /*= 0*/, Int_t lastybin /*= -1*/, Option_t* option /*= ""*/) const
{
	TH1D *cv_px = this->TH2D::ProjectionX(name, firstybin, lastybin, option); 
	MUH1D *h_px = new MUH1D( *cv_px );

	//! Getting Vertical Names
	std::vector<std::string> vertNames = GetVertErrorBandNames();
	for( unsigned int i = 0; i != vertNames.size(); ++i )
	{
		std::vector<TH1D*> vert_hists;
		const MUVertErrorBand2D *errBand = GetVertErrorBand(vertNames[i]);
		int nUniverses = errBand->GetNHists();
		for (int j = 0; j != nUniverses; ++j)
		{
			const TH2D *h_universe = dynamic_cast<const TH2D*>( errBand->GetHist(j) );
			TH1D *h_universe_px = h_universe->ProjectionX( Form("%s_%s_universe%i", name, vertNames[i].c_str(), j), firstybin, lastybin, option );
			vert_hists.push_back(h_universe_px);
		}
		h_px->AddVertErrorBand(vertNames[i], vert_hists);

		//cleaning
		for (std::vector<TH1D*>::iterator itHist = vert_hists.begin() ; itHist != vert_hists.end() ; ++ itHist)
			delete *itHist;
	}

	//! Getting Lateral Names
	std::vector<std::string> latNames = GetLatErrorBandNames();
	for( unsigned int i = 0; i != latNames.size(); ++i )
	{
		std::vector<TH1D*> lat_hists;
		const MULatErrorBand2D *errBand = GetLatErrorBand(latNames[i]);
		int nUniverses = errBand->GetNHists();
		for (int j = 0; j != nUniverses; ++j)
		{
			const TH2D *h_universe = dynamic_cast<const TH2D*>( errBand->GetHist(j) );
			TH1D *h_universe_px = h_universe->ProjectionX( Form("%s_%s_universe%i", name, latNames[i].c_str(), j), firstybin, lastybin, option );
			lat_hists.push_back(h_universe_px);
		}
		h_px->AddLatErrorBand(latNames[i], lat_hists);

		//cleaning
		for (std::vector<TH1D*>::iterator itHist = lat_hists.begin() ; itHist != lat_hists.end() ; ++ itHist)
			delete *itHist;
	}

	return h_px;
}

MUH1D * MUH2D::ProjectionY(const char* name /*= "_py"*/, Int_t firstxbin /*= 0*/, Int_t lastxbin /*= -1*/, Option_t* option /*= ""*/) const
{
	TH1D *cv_py = TH2D::ProjectionY(name, firstxbin, lastxbin, option); 
	MUH1D *h_py = new MUH1D( *cv_py );

	//! Getting Vertical Names
	std::vector<std::string> vertNames = GetVertErrorBandNames();
	for( unsigned int i = 0; i != vertNames.size(); ++i )
	{
		std::vector<TH1D*> vert_hists;
		const MUVertErrorBand2D *errBand = GetVertErrorBand(vertNames[i]);
		int nUniverses = errBand->GetNHists();
		for (int j = 0; j != nUniverses; ++j)
		{
			const TH2D *h_universe = dynamic_cast<const TH2D*>( errBand->GetHist(j) );
			TH1D *h_universe_py = h_universe->ProjectionY( Form("%s_%s_universe%i", name, vertNames[i].c_str(), j), firstxbin, lastxbin, option );
			vert_hists.push_back(h_universe_py);
		}
		h_py->AddVertErrorBand(vertNames[i], vert_hists);

		//cleaning
		for (std::vector<TH1D*>::iterator itHist = vert_hists.begin() ; itHist != vert_hists.end() ; ++ itHist)
			delete *itHist;
	}

	//! Getting Lateral Names
	std::vector<std::string> latNames = GetLatErrorBandNames();
	for( unsigned int i = 0; i != latNames.size(); ++i )
	{
		std::vector<TH1D*> lat_hists;
		const MULatErrorBand2D *errBand = GetLatErrorBand(latNames[i]);
		int nUniverses = errBand->GetNHists();
		for (int j = 0; j != nUniverses; ++j)
		{
			const TH2D *h_universe = dynamic_cast<const TH2D*>( errBand->GetHist(j) );
			TH1D *h_universe_py = h_universe->ProjectionY( Form("%s_%s_universe%i", name, latNames[i].c_str(), j), firstxbin, lastxbin, option );
			lat_hists.push_back(h_universe_py);
		}
		h_py->AddLatErrorBand(latNames[i], lat_hists);

		//cleaning
		for (std::vector<TH1D*>::iterator itHist = lat_hists.begin() ; itHist != lat_hists.end() ; ++ itHist)
			delete *itHist;
	}

	return h_py;
}

bool MUH2D::AddVertErrorBand( const std::string& name, const int nhists /* = -1 */ )
{
	//! Make sure there are no ErrorBands with this name already
	if( HasErrorBand( name ) )
	{
		std::cout << "Warning [MUH2D::AddVertErrorBand] : There is already an error band with name \"" << name << "\".  Doing nothing." << std::endl;
		return false;
	}

	//! Error bands we own have this MUH2D's name as a prefix
	const std::string errName( std::string(GetName()) + "_" + name );

	//! non-positive nhists means you want to use the VertErrorBand's default
	if( nhists > 0 )
		fVertErrorBandMap[name] = new MUVertErrorBand2D( errName, (TH2D*)this, nhists );
	else
		fVertErrorBandMap[name] = new MUVertErrorBand2D( errName, (TH2D*)this );

	return true;
}

bool MUH2D::AddVertErrorBand( const std::string& name, const std::vector<TH2D*>& base )
{
	//! Make sure there are no ErrorBands with this name already
	if( HasErrorBand( name ) )
	{
		std::cout << "Warning [MUH2D::AddVertErrorBand] : There is already an error band with name \"" << name << "\".  Doing nothing." << std::endl;
		return false;
	}

	//! Error bands we own have this MUH1D's name as a prefix
	const std::string errName( std::string(GetName()) + "_" + name );

	//!Set the ErrorBand
	fVertErrorBandMap[name] = new MUVertErrorBand2D( errName, (TH2D*)this, base );

	return true;
}

bool MUH2D::AddVertErrorBandAndFillWithCV( const std::string& name, const unsigned int nhists )
{
	// Make sure there are no ErrorBands with this name already
	if( HasErrorBand( name ) )
	{
		Warning("MUH2D::AddVertErrorBandAndFillWithCV", "There is already an error band with name \"%s\".  Doing nothing.", name.c_str());
		return false;
	}

	// Make a vector of histos with the CV 
	std::vector<TH2D*> histos(nhists, 0);
	for( unsigned int universe=0; universe<nhists; ++universe )
	{
		TH2D* histo = new TH2D( *this );
		histo->SetName( Form( "tmp_universe_%i", universe ) );
		histos[universe] = histo;
	}

	// Add the error band and fill it with the vector of histos
	bool ok = this->AddVertErrorBand( name, histos );

	// Clean vector of histos
	for( std::vector<TH2D*>::iterator it=histos.begin(); it!=histos.end(); ++it )
		delete *it;
	histos.clear();

	return ok;
}


bool MUH2D::AddLatErrorBand( const std::string& name, const int nhists /* = -1 */ )
{
	//! Make sure there are no ErrorBands with this name already
	if( HasErrorBand( name ) )
	{
		std::cout << "Warning [MUH2D::AddLatErrorBand] : There is already an error band with name \"" << name << "\".  Doing nothing." << std::endl;
		return false;
	}

	//! Error bands we own have this MUH2D's name as a prefix
	const std::string errName( std::string(GetName()) + "_" + name );

	//! non-positive nhists means you want to use the LatErrorBand's default
	if( nhists > 0 )
		fLatErrorBandMap[name] = new MULatErrorBand2D( errName, (TH2D*)this, nhists );
	else
		fLatErrorBandMap[name] = new MULatErrorBand2D( errName, (TH2D*)this );

	return true;
}

bool MUH2D::AddLatErrorBand( const std::string& name, const std::vector<TH2D*>& base )
{
	//! Make sure there are no ErrorBands with this name already
	if( HasErrorBand( name ) )
	{
		std::cout << "Warning [MUH2D::AddLatErrorBand] : There is already an error band with name \"" << name << "\".  Doing nothing." << std::endl;
		return false;
	}

	//! Error bands we own have this MUH1D's name as a prefix
	const std::string errName( std::string(GetName()) + "_" + name );

	//!Set the ErrorBand
	fLatErrorBandMap[name] = new MULatErrorBand2D( errName, (TH2D*)this, base );

	return true;
}


bool MUH2D::AddLatErrorBandAndFillWithCV( const std::string& name, const unsigned int nhists )
{
	// Make sure there are no ErrorBands with this name already
	if( HasErrorBand( name ) )
	{
		Warning("MUH2D::AddLatErrorBandAndFillWithCV", "There is already an error band with name \"%s\".  Doing nothing.", name.c_str());
		return false;
	}

	// Make a vector of histos with the CV 
	std::vector<TH2D*> histos(nhists, 0);
	for( unsigned int universe=0; universe<nhists; ++universe )
	{
		TH2D* histo = new TH2D( *this );
		histo->SetName( Form( "tmp_universe_%i", universe ) );
		histos[universe] = histo;
	}

	// Add the error band and fill it with the vector of histos
	bool ok = this->AddLatErrorBand( name, histos );

	// Clean vector of histos
	for( std::vector<TH2D*>::iterator it=histos.begin(); it!=histos.end(); ++it ) 
		delete *it;
	histos.clear();

	return ok; 
}


bool MUH2D::FillVertErrorBand( const std::string& name, const double xval, const double yval, const std::vector<double>& weights, const double cvweight /* = 1.0 */, double cvWeightFromMe /*= 1.*/ )
{
	return FillVertErrorBand( name, xval, yval, &(weights[0]), cvweight, cvWeightFromMe );
}

bool MUH2D::FillVertErrorBand( const std::string& name, const double xval, const double yval, const double * weights, const double cvweight /* = 1.0 */, double cvWeightFromMe /*= 1.*/ )
{
	//! Try to fill a vertical error band
	MUVertErrorBand2D* vert = GetVertErrorBand( name );
	if( vert )
		return vert->Fill( xval, yval, weights, cvweight, cvWeightFromMe );

	std::cout << "Warning [MUH2D::FillVertErrorBand] : Could not find a vertical error band to fill with name = " << name << std::endl;
	return false;
}

bool MUH2D::FillVertErrorBand( const std::string& name, const double xval, const double yval, const double weightDown, const double weightUp, const double cvweight  /*= 1.0*/, double cvWeightFromMe /*= 1.*/ )
{
	//! Try to fill a vertical error band
	MUVertErrorBand2D *vert = GetVertErrorBand( name );
	if( vert )
		return vert->Fill( xval, yval, weightDown, weightUp, cvweight, cvWeightFromMe );

	std::cout << "Warning [MUH2D::FillVertErrorBand] : Could not find a vertical error band to fill with name = " << name << std::endl;
	return false;
}

bool MUH2D::FillLatErrorBand( const std::string& name, const double xval, const double yval, const std::vector<double>& xshifts, const std::vector<double>& yshifts, const double cvweight  /*= 1.0*/, const bool fillcv /*= true*/, const double* weights /*= 0*/  )
{
	return FillLatErrorBand( name, xval, yval, &(xshifts[0]), &(yshifts[0]), cvweight, fillcv, weights );
}

bool MUH2D::FillLatErrorBand( const std::string& name, const double xval, const double yval, const double *xshifts, const double *yshifts, const double cvweight  /*= 1.0*/, const bool fillcv /*= true*/, const double* weights /*= 0*/ )
{
	//! Try to fill a lateral error band
	MULatErrorBand2D* lat = GetLatErrorBand( name );
	if( lat )
		return lat->Fill( xval, yval, xshifts, yshifts, cvweight, fillcv, weights );

	std::cout << "Warning [MUH2D::FillLatErrorBand] : Could not find a lateral error band to fill with name = " << name << std::endl;

	return false;
}

bool MUH2D::FillLatErrorBand( const std::string& name, const double xval, const double yval, const double xshiftDown, const double xshiftUp, const double yshiftDown, const double yshiftUp, const double cvweight /*= 1.0*/, const bool fillcv /*= true*/ )
{
	//! Try to fill a vertical error band
	MULatErrorBand2D *lat = GetLatErrorBand( name );
	if( lat )
		return lat->Fill( xval, yval, xshiftDown, xshiftUp, yshiftDown, yshiftUp, cvweight, fillcv );

	std::cout << "Warning [MUH2D::FillLatErrorBand] : Could not find a lateral error band to fill with name = " << name << std::endl;

	return false;
}


MUVertErrorBand2D* MUH2D::GetVertErrorBand( const std::string& name )
{
	std::map<std::string, MUVertErrorBand2D*>::iterator i = fVertErrorBandMap.find( name );
	if( i == fVertErrorBandMap.end() )
	{
		std::cout << "Warning [MUH2D::GetVertErrorBand] : There is no vertical error band with name \"" << name << "\".  Returning NULL." << std::endl;
		return NULL;
	}

	return i->second;
}

const MUVertErrorBand2D* MUH2D::GetVertErrorBand( const std::string& name ) const
{
	std::map<std::string, MUVertErrorBand2D*>::const_iterator i = fVertErrorBandMap.find( name );
	if( i == fVertErrorBandMap.end() )
	{
		std::cout << "Warning [MUH2D::GetVertErrorBand] : There is no vertical error band with name \"" << name << "\".  Returning NULL." << std::endl;
		return NULL;
	}

	return i->second;
}

MULatErrorBand2D* MUH2D::GetLatErrorBand( const std::string& name )
{
	std::map<std::string, MULatErrorBand2D*>::iterator i = fLatErrorBandMap.find( name );
	if( i == fLatErrorBandMap.end() )
	{
		std::cout << "Warning [MUH2D::GetLatErrorBand] : There is no lateral error band with name \"" << name << "\".  Returning NULL." << std::endl;
		return NULL;
	}

	return i->second;
}

const MULatErrorBand2D* MUH2D::GetLatErrorBand( const std::string& name ) const
{
	std::map<std::string, MULatErrorBand2D*>::const_iterator i = fLatErrorBandMap.find( name );
	if( i == fLatErrorBandMap.end() )
	{
		std::cout << "Warning [MUH2D::GetLatErrorBand] : There is no lateral error band with name \"" << name << "\".  Returning NULL." << std::endl;
		return NULL;
	}

	return i->second;
}

bool MUH2D::HasLatErrorBand( const std::string& name ) const
{
	//! Check the MULatErrorBands
	if( fLatErrorBandMap.find( name ) != fLatErrorBandMap.end() )
		return true;

	return false;
}

bool MUH2D::HasVertErrorBand( const std::string& name ) const
{
	//! Check the MUVertErrorBands
	if( fVertErrorBandMap.find( name ) != fVertErrorBandMap.end() )
		return true;

	return false;
}

bool MUH2D::HasErrorBand( const std::string& name ) const
{
	//! Check the MULatErrorBands
	if( HasLatErrorBand( name ) )
		return true;

	//! Check the MUVertErrorBands
	if( HasVertErrorBand( name ) )
		return true;

	return false;
}

bool MUH2D::HasErrorMatrix( const std::string& name ) const
{
	//! Check the fSysErrorMatrix
	if( fSysErrorMatrix.find( name ) != fSysErrorMatrix.end() )
		return true;

	return false;
}

std::vector<std::string> MUH2D::GetVertErrorBandNames() const
{
	std::vector<std::string> rval;
	for( std::map<std::string, MUVertErrorBand2D*>::const_iterator i = fVertErrorBandMap.begin(); i != fVertErrorBandMap.end(); ++i )
		rval.push_back( i->first );
	return rval;
}

std::vector<std::string> MUH2D::GetLatErrorBandNames() const
{
	std::vector<std::string> rval;
	for( std::map<std::string, MULatErrorBand2D*>::const_iterator i = fLatErrorBandMap.begin(); i != fLatErrorBandMap.end(); ++i )
		rval.push_back( i->first );
	return rval;
}

std::vector<std::string> MUH2D::GetSysErrorMatricesNames() const
{

	std::vector<std::string> rval;
	//Vertical Errors
	for( std::map<std::string, MUVertErrorBand2D*>::const_iterator i = fVertErrorBandMap.begin(); i != fVertErrorBandMap.end(); ++i )
		rval.push_back( i->first );

	//Lateral Errors
	for( std::map<std::string, MULatErrorBand2D*>::const_iterator i = fLatErrorBandMap.begin(); i != fLatErrorBandMap.end(); ++i )
		rval.push_back( i->first );
	/*
	//Special Errors
	for( std::map<std::string, TMatrixD*>::const_iterator i = fSysErrorMatrix.begin(); i != fSysErrorMatrix.end(); ++i )
	if ( !HasEnding(i->first, "_asShape") )
	rval.push_back( i->first );
	*/
	return rval;
}

//------------------------------------------------------------------------
// Get a Specific Covariance Matrix from the map
//------------------------------------------------------------------------
TMatrixD MUH2D::GetSysErrorMatrix(const std::string& name, bool asFrac /*= false*/, bool cov_area_normalize /*= false*/) const
{
	if (HasEnding(name,"_asShape") )
		std::cout << "Warning [MUH2D::GetSysErrorMatrix]: You are calling the error Matrix: " << name <<".\nAssuming the error Band wanted is: " << name.substr(0,name.length()-8) << " with cov_area_normalize = true" << std::endl;

	const std::string name_condition = ( (cov_area_normalize) && !(HasEnding(name,"_asShape")) )?  "_asShape" : "";
	const std::string fname = name + name_condition ; 
	const std::string errName = HasEnding(fname,"_asShape") ? fname.substr(0,fname.length()-8) : fname;

	//! @todo what to do with underflow( bin=0 ) and overflow ( bin=nbins+1 )?
	const int highBinX = GetNbinsX() + 1;
	const int highBinY = GetNbinsY() + 1;
	const int highBin  = GetBin( highBinX, highBinY );
	const int lowBin = 0;
	TMatrixD covmx(highBin+1,highBin+1);

	if ( HasErrorMatrix( fname ) )
		covmx = *(fSysErrorMatrix.find(fname)->second);

	else if( fLatErrorBandMap.find( errName ) != fLatErrorBandMap.end() )
	{
		std::map<std::string, MULatErrorBand2D*>::const_iterator it = fLatErrorBandMap.find( errName );
		covmx = it->second->CalcCovMx( ( HasEnding(fname,"_asShape") ) );
	}
	else if( fVertErrorBandMap.find( errName ) != fVertErrorBandMap.end() )
	{
		std::map<std::string, MUVertErrorBand2D*>::const_iterator it = fVertErrorBandMap.find( errName );
		covmx = it->second->CalcCovMx( ( HasEnding(fname,"_asShape") ) );
	}
	else 
		std::cout << "Warning [MUH2D::GetSysErrorMatrix]: There is no Covariance Matrix with name " << fname << ".Returning and empty Matrix." << std::endl;

	if (asFrac)
	{
		for( int i = lowBin; i <= highBin; ++i )
		{
			for( int k = i; k <= highBin; ++k )
			{ 
				//Gettting the the CV value for bin i
				const double cv_i = GetBinContent(i);
				const double cv_k = GetBinContent(k);

				covmx[i][k]= ( (cv_i != 0.) && (cv_k !=0.) )? covmx[i][k]/(cv_i * cv_k) : 0.;
				covmx[k][i]=covmx[i][k];
			}
		}
	}

	return covmx;
}

TMatrixD MUH2D::GetStatErrorMatrix( bool asFrac /* =false */ ) const
{
	const int highBinX = GetNbinsX() + 1;
	const int highBinY = GetNbinsY() + 1;
	const int highBin  = GetBin( highBinX, highBinY );
	const int lowBin = 0;
	TMatrixD covmx(highBin+1,highBin+1);
	//! stat error
	for (int iBin=lowBin; iBin<=highBin; ++iBin )
		covmx[iBin][iBin]= GetBinError(iBin);

	if (asFrac)
	{ 
		for( int iBin = lowBin; iBin <= highBin; ++iBin )
		{
			double binCon = GetBinContent(iBin);
			double binErr = ( binCon != 0. ? covmx[iBin][iBin]/binCon : 0. ); 
			covmx[iBin][iBin] = binErr;
		}
	}

	return covmx*covmx;
}

//------------------------------------------------------------------------
// Get the Total Covariance Matrix
//------------------------------------------------------------------------
TMatrixD MUH2D::GetTotalErrorMatrix(
		bool includeStat /*= true*/, 
		bool asFrac /*= false*/, 
		bool cov_area_normalize /*= false*/ ) const
{

	//! @todo what to do with underflow( bin=0 ) and overflow ( bin=nbins+1 )?
	const int highBinX = GetNbinsX() + 1;
	const int highBinY = GetNbinsY() + 1;
	const int highBin  = GetBin( highBinX, highBinY );
	const int lowBin = 0;
	TMatrixD covmx(highBin+1,highBin+1);

	std::vector<std::string> names = GetSysErrorMatricesNames();
	for (std::vector<std::string>::const_iterator itName = names.begin() ; itName != names.end() ; ++itName)
	{
		covmx += GetSysErrorMatrix(*itName, false, cov_area_normalize);
	}

	if (includeStat)
		covmx += GetStatErrorMatrix();

	if (asFrac)
	{
		for( int i = lowBin; i <= highBin; ++i )
		{
			for( int k = i; k <= highBin; ++k )
			{ 
				//Gettting the the CV value for bin i
				const double cv_i = GetBinContent(i);
				const double cv_k = GetBinContent(k);

				covmx[i][k]= ( (cv_i != 0.) && (cv_k !=0.) )? covmx[i][k]/(cv_i * cv_k) : 0.;
				covmx[k][i]=covmx[i][k];
			}
		}
	}

	return covmx;
}

TMatrixD MUH2D::GetTotalCorrelationMatrix( bool cov_area_normalize /*= false*/ ) const
{
	TMatrixD covmx = GetTotalErrorMatrix(false, false, cov_area_normalize);
	const int size = covmx.GetNrows();
	TMatrixD corrmx(size,size);

	for( int i = 0; i < size; ++i )
	{
		for( int k = 0; k < size; ++k )
		{
			corrmx[i][k] = ( covmx[i][i] == 0. || covmx[k][k] == 0. ) ? 0. : covmx[i][k]/sqrt(covmx[i][i]*covmx[k][k]);
		}
	}
	return corrmx;
}

TH2D MUH2D::GetTotalError(
		bool includeStat /* = true */, 
		bool asFrac /* = false */ ,
		bool cov_area_normalize /*= false */) const
{
	//! Make a copy of this histogram as a TH1D and rename it
	TH2D err( *this );
	err.Reset();
	std::string tmpName( std::string(GetName()) + "_TotalError");
	err.SetName( tmpName.c_str() );

	//! @todo what to do with underflow( bin=0 ) and overlow ( bin=nbins+1 )?
	const int highBinX = GetNbinsX() + 1;
	const int highBinY = GetNbinsY() + 1;
	const int highBin  = GetBin( highBinX, highBinY );
	const int lowBin = 0;

	//!Get the Total Error Matrix
	TMatrixD errMatrix = GetTotalErrorMatrix(includeStat, asFrac, cov_area_normalize);

	for( int iBin = lowBin; iBin <= highBin; ++iBin )
	{
		double derr = errMatrix[iBin][iBin];
		err.SetBinContent( iBin, ( derr > 0 ) ? sqrt(derr): 0. );
	}

	return err;
}

//------------------------------------------------------------------------
// Get a histogram of the statistical errors only.
//------------------------------------------------------------------------
TH2D MUH2D::GetStatError( bool asFrac /* = false */ ) const
{
	//! Make a copy of this histogram as a TH1D and rename it
	TH2D err( *this );
	err.Reset();
	std::string tmpName( std::string(GetName()) + "_StatError");
	err.SetName( tmpName.c_str() );

	//! @todo what to do with underflow( bin=0 ) and overlow ( bin=nbins+1 )?
	const int highBinX = GetNbinsX() + 1;
	const int highBinY = GetNbinsY() + 1;
	const int highBin  = GetBin( highBinX, highBinY );
	const int lowBin = 0;

	//! stat error
	for( int iBin = lowBin; iBin <= highBin; ++iBin )
		err.SetBinContent( iBin, GetBinError(iBin) );

	if( asFrac )
	{
		for( int iBin = lowBin; iBin <= highBin; ++iBin )
		{
			double binCon = GetBinContent(iBin);
			double binErr = ( binCon != 0. ? err.GetBinContent(iBin) / binCon : 0. );
			err.SetBinContent( iBin, binErr );
		}
	}

	return err;
}

TH2D MUH2D::GetCVHistoWithError( bool includeStat /* = true */ , bool cov_area_normalize /* = false */) const
{
	//! @todo Check the highBin value for here (in MUH1D, overflow is not set for rval.SetBinError)
	const int highBinX = GetNbinsX() + 1;
	const int highBinY = GetNbinsY() + 1;
	const int highBin  = GetBin( highBinX, highBinY );
	const int lowBin = 0;

	//! Get the error band
	TH2D err = GetTotalError( includeStat , false, cov_area_normalize);

	//! Create a copy of this histogram and rename it
	TH2D rval( *this );
	std::string tmpName( std::string( GetName() ) + "_CV_WithErr" );
	rval.SetName( tmpName.c_str() );

	for( int iBin = lowBin; iBin <= highBin; ++iBin )
		rval.SetBinError( iBin, err.GetBinContent(iBin) );

	return rval;
}

TH2D MUH2D::GetCVHistoWithStatError() const
{
	//! @todo Check the highBin value for here (in MUH1D, overflow is not set for rval.SetBinError)
	const int highBinX = GetNbinsX() + 1;
	const int highBinY = GetNbinsY() + 1;
	const int highBin  = GetBin( highBinX, highBinY );
	const int lowBin = 0;

	//! Get the stat. error band
	TH2D err = GetStatError( false );

	//! Create a copy of this histogram and rename it
	TH2D rval( *this );
	std::string tmpName( std::string( GetName() ) + "_CV_WithStatErr" );
	rval.SetName( tmpName.c_str() );

	for( int iBin = lowBin; iBin <= highBin; ++iBin )
		rval.SetBinError( iBin, err.GetBinContent(iBin) );

	return rval;
}

//======================================================================
// Replacements of ROOT versions of functions
//======================================================================

void MUH2D::Scale( Double_t c1 /*= 1.*/, Option_t* option /*=""*/ )
{
	//! Scale yourself using TH1D::Scale
	this->TH2D::Scale( c1, option );

	//! Scale the vertical error bands
	for( std::map<std::string, MUVertErrorBand2D*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
		it->second->Scale( c1, option );

	//! Scale the lateral error bands
	for( std::map<std::string, MULatErrorBand2D*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
		it->second->Scale( c1, option );
}

void MUH2D::Add( const TH2* h1, const Double_t c1 /*= 1.*/ )
{
	//! Try to cast the input TH2 to a MUH2D
	const MUH2D *mnv1 = dynamic_cast<const MUH2D*>(h1);

	if( mnv1 )
	{

		//! Add as a TH2D
		this->TH2D::Add( h1, c1 );

		//! Call Add for all vertical error bands
		for( std::map<std::string, MUVertErrorBand2D*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
		{
			const MUVertErrorBand2D* err1 = mnv1->GetVertErrorBand( it->first );
			if( !err1  )
			{
				Error("Add", "Could not add MUH2Ds because they all don't have the %s MUVertErrorBand2D", it->first.c_str());
				return;
			}

			Bool_t ok = it->second->Add( err1, c1 );

			if( ! ok )
			{
				Error("Add", "Could not add MUH2Ds because histogram add failed for MUVertErrorBand2D %s ", it->first.c_str());
				return;
			}
		}//done adding Vert errors

		//! Call Add for all lateral error bands
		for( std::map<std::string, MULatErrorBand2D*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
		{
			const MULatErrorBand2D* err1 = mnv1->GetLatErrorBand( it->first );
			if( !err1  )
			{
				Error("Add", "Could not add MUH2Ds because they all don't have the %s MULatErrorBand2D", it->first.c_str());
				return;
			}

			Bool_t ok = it->second->Add( err1, c1 );

			if( ! ok )
			{
				Error("Add", "Could not add MUH2Ds because histogram add failed for MULatErrorBand2D %s ", it->first.c_str());
				return;
			}
		}//done adding Lat errors

	}// end if cast to MUH2D worked
	else
	{
		Error( "MUH2D::Add", "Unable to add histogram because it could not be cast to an MUH2D.  Did nothing." );
	}

}

void MUH2D::Multiply( const MUH2D* h1, const MUH2D* h2, Double_t c1 /*= 1*/, Double_t c2 /*= 1*/ )
{
	//! @todo Would love to return a bool here, but we want this Multiply to override TH1's and that is void

	//! Call the TH1D Multiply 
	this->TH2D::Multiply( (TH2D*)h1, (TH2D*)h2, c1, c2 );

	//! Scale the vertical error bands
	for( std::map<std::string, MUVertErrorBand2D*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
	{
		const MUVertErrorBand2D* err1 = h1->GetVertErrorBand( it->first );
		const MUVertErrorBand2D* err2 = h2->GetVertErrorBand( it->first );
		if( !err1 || !err2 )
		{
			Error("Multiply", "Could not divide MUH2Ds because they all don't have the %s MUVertErrorBand2D", it->first.c_str());
			return;
		}
		it->second->Multiply( err1, err2, c1, c2 );
	}

	//! Scale the lateral error bands
	for( std::map<std::string, MULatErrorBand2D*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
	{
		const MULatErrorBand2D* err1 = h1->GetLatErrorBand( it->first );
		const MULatErrorBand2D* err2 = h2->GetLatErrorBand( it->first );
		if( !err1 || !err2 )
		{
			Error("Multiply", "Could not divide MUH2Ds because they all don't have the %s MULatErrorBand2D", it->first.c_str());
			return;
		}
		it->second->Multiply( err1, err2, c1, c2 );
	}

	return;
}

void MUH2D::Divide( const MUH2D* h1, const MUH2D* h2, Double_t c1 /*= 1*/, Double_t c2 /*= 1*/, Option_t* option /*=""*/ )
{
	//! @todo Would love to return a bool here, but we want this Divide to override TH1's and that is void

	//! Call the TH1D Divide
	this->TH2D::Divide( (TH2D*)h1, (TH2D*)h2, c1, c2, option );

	//! Scale the vertical error bands
	for( std::map<std::string, MUVertErrorBand2D*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
	{
		const MUVertErrorBand2D* err1 = h1->GetVertErrorBand( it->first );
		const MUVertErrorBand2D* err2 = h2->GetVertErrorBand( it->first );
		if( !err1 || !err2 )
		{
			Error("Divide", "Could not divide MUH2Ds because they all don't have the %s MUVertErrorBand2D", it->first.c_str());
			return;
		}
		it->second->Divide( err1, err2, c1, c2, option );
	}

	//! Scale the lateral error bands
	for( std::map<std::string, MULatErrorBand2D*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
	{
		const MULatErrorBand2D* err1 = h1->GetLatErrorBand( it->first );
		const MULatErrorBand2D* err2 = h2->GetLatErrorBand( it->first );
		if( !err1 || !err2 )
		{
			Error("Divide", "Could not divide MUH2Ds because they all don't have the %s MULatErrorBand2D", it->first.c_str());
			return;
		}
		it->second->Divide( err1, err2, c1, c2, option );
	}

	return;
}

void MUH2D::DivideSingle( const MUH2D* h1, const TH2* h2, Double_t c1 /*= 1*/, Double_t c2 /*= 1*/, Option_t* option /*=""*/ )
{
	//! Call the TH1D Divide
	this->TH2D::Divide( (TH2D*)h1, h2, c1, c2, option );

	//! Scale the vertical error bands
	for( std::map<std::string, MUVertErrorBand2D*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
	{
		const MUVertErrorBand2D* err1 = h1->GetVertErrorBand( it->first );
		if( !err1 )
		{
			Error("Divide", "Could not divide MUH2Ds because they all don't have the %s MUVertErrorBand2D", it->first.c_str());
			return;
		}
		it->second->DivideSingle( err1, h2, c1, c2, option );
	}

	//! Scale the lateral error bands
	for( std::map<std::string, MULatErrorBand2D*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
	{
		const MULatErrorBand2D* err1 = h1->GetLatErrorBand( it->first );
		if( !err1 )
		{
			Error("Divide", "Could not divide MUH2Ds because they all don't have the %s MULatErrorBand2D", it->first.c_str());
			return;
		}
		it->second->DivideSingle( err1, h2, c1, c2, option );
	}

	return;
}



//--------------------------------------------------------
// trivial helper functions
//--------------------------------------------------------
bool MUH2D::HasEnding (std::string const &fullString, std::string const &ending) const
{
	TString a(fullString);
	return a.EndsWith( ending.c_str() );
}

#endif
