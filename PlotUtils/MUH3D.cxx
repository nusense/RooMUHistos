#ifndef MNV_MUH3D_cxx
#define MNV_MUH3D_cxx 1

#include "PlotUtils/MUH3D.h"

using namespace PlotUtils;

//==================================================================================
// CONSTRUCTORS
//==================================================================================

//--------------------------------------------------------
// Copy constructors with default normalized bin width
//--------------------------------------------------------
MUH3D::MUH3D() : 
	TH3D(),
	fNormBinWidthX(1.),
	fNormBinWidthY(1.),
	fNormBinWidthZ(1.)
{ 
	Sumw2(); 
}

MUH3D::MUH3D(Double_t normBinWidthX, Double_t normBinWidthY, Double_t normBinWidthZ) :
	TH3D(),
	fNormBinWidthX(normBinWidthX),
	fNormBinWidthY(normBinWidthY),
	fNormBinWidthZ(normBinWidthZ)
{
	Sumw2();
}

MUH3D::MUH3D( const TH3D& h2d ) :
	TH3D( h2d ) 
{ 
	fNormBinWidthX = h2d.GetXaxis()->GetBinWidth(1);
	fNormBinWidthY = h2d.GetYaxis()->GetBinWidth(1);
	fNormBinWidthZ = h2d.GetZaxis()->GetBinWidth(1);
	Sumw2();
}

//----------------------------------------------------------------------------------
// Copy constructors with specified normalized bin width for the X and Y projections
//----------------------------------------------------------------------------------
MUH3D::MUH3D(const TH3D& h2d, Double_t normBinWidthX, Double_t normBinWidthY, Double_t normBinWidthZ) :
	TH3D( h2d ),
	fNormBinWidthX(normBinWidthX),
	fNormBinWidthY(normBinWidthY),
	fNormBinWidthZ(normBinWidthZ)
{
	Sumw2();
}

//--------------------------------------------------------
// Constructors with default normalization bin width
//--------------------------------------------------------
MUH3D::MUH3D( const char* name, const char* title, Int_t nbinsx, const Float_t* xbins, Int_t nbinsy, const Float_t* ybins, Int_t nbinsz, const Float_t* zbins) :
	TH3D( name, title, nbinsx, xbins, nbinsy, ybins, nbinsz, zbins) 
{
	//! Default normlization bin width is the first bin width
	fNormBinWidthX = xbins[1] - xbins[0];
	fNormBinWidthY = ybins[1] - ybins[0];
	fNormBinWidthZ = zbins[1] - zbins[0];
	Sumw2();
}

MUH3D::MUH3D( const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, const Double_t* ybins, Int_t nbinsz, const Double_t* zbins) :
	TH3D( name, title, nbinsx, xbins, nbinsy, ybins, nbinsz, zbins)
{
	//! Default normlization bin width is the first bin width
	fNormBinWidthX = xbins[1] - xbins[0];
	fNormBinWidthY = ybins[1] - ybins[0];
	fNormBinWidthZ = zbins[1] - zbins[0];
	Sumw2();
}

MUH3D::MUH3D( const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nbinsz, Double_t zlow, Double_t zup):
	TH3D( name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup )
{ 
	//! Default normalization bin width is the constant width of the bins
	fNormBinWidthX = (xup - xlow) / float(nbinsx);
	fNormBinWidthY = (yup - ylow) / float(nbinsy);
	fNormBinWidthY = (zup - zlow) / float(nbinsz);
	Sumw2();
}

//! Deep copy constructor (the real copy)
MUH3D::MUH3D( const MUH3D& h ) :
	TH3D( h )
{
	//! Deep copy the variables
	DeepCopy( h );
}

MUH3D& MUH3D::operator=( const MUH3D& h )
{
	//! If this is me, then no copy is necessary
	if( this == &h )
		return *this;

	//call the base class assignment operator
	this->TH3D::operator=(h);

	//! Delete and clear all vert and lat error bands
	for( std::map<std::string, MUVertErrorBand3D*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
		delete it->second;
	fVertErrorBandMap.clear();

	for( std::map<std::string, MULatErrorBand3D*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
		delete it->second;
	fLatErrorBandMap.clear();

	//! Then deep copy the variables
	DeepCopy(h);

	return *this;
}

void MUH3D::DeepCopy( const MUH3D& h )
{
	//! Set bin norm width
	fNormBinWidthX = h.GetNormBinWidthX();
	fNormBinWidthY = h.GetNormBinWidthY();
	fNormBinWidthZ = h.GetNormBinWidthZ();

	//! Copy the vert and lat error bands
	std::vector<std::string> vertNames = h.GetVertErrorBandNames();
	for( std::vector<std::string>::iterator name = vertNames.begin(); name != vertNames.end(); ++name )
		fVertErrorBandMap[*name] = new MUVertErrorBand3D( *h.GetVertErrorBand(*name) );

	std::vector<std::string> latNames = h.GetLatErrorBandNames();
	for( std::vector<std::string>::iterator name = latNames.begin(); name != latNames.end(); ++name )
		fLatErrorBandMap[*name] = new MULatErrorBand3D( *h.GetLatErrorBand(*name) );
}

//--------------------------------------------------------
// Constructors with specified normalization bin width
//--------------------------------------------------------
MUH3D::MUH3D( const char* name, const char* title, Int_t nbinsx, const Float_t* xbins, Int_t nbinsy, const Float_t* ybins, Int_t nbinsz, const Float_t* zbins, Double_t normBinWidthX, Double_t normBinWidthY, Double_t normBinWidthZ) :
	TH3D( name, title, nbinsx, xbins, nbinsy, ybins, nbinsz, zbins),
	fNormBinWidthX(normBinWidthX),
	fNormBinWidthY(normBinWidthY),
	fNormBinWidthZ(normBinWidthZ)
{ 
	Sumw2();
}

MUH3D::MUH3D( const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, const Double_t* ybins, Int_t nbinsz, const Double_t* zbins, Double_t normBinWidthX, Double_t normBinWidthY, Double_t normBinWidthZ) :
	TH3D( name, title, nbinsx, xbins, nbinsy, ybins, nbinsz, zbins),
	fNormBinWidthX(normBinWidthX),
	fNormBinWidthY(normBinWidthY),
	fNormBinWidthZ(normBinWidthZ)
{ 
	Sumw2();
}

MUH3D::MUH3D( const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nbinsz, Double_t zlow, Double_t zup, Double_t normBinWidthX, Double_t normBinWidthY, Double_t normBinWidthZ):
	TH3D( name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup ),
	fNormBinWidthX(normBinWidthX),
	fNormBinWidthY(normBinWidthY),
	fNormBinWidthZ(normBinWidthZ)
{
	Sumw2();
}

//------------------------------------------------------------------------
// Get an MUH3D which has its bin content and errors normalized to bin width so it looks smooth
//------------------------------------------------------------------------
MUH3D MUH3D::GetBinNormalizedCopy( Double_t normBinWidthX /* = fNormBinWidthX */, Double_t normBinWidthY /* = fNormBinWidthY */, Double_t normBinWidthZ /* = fNormBinWidthZ */ ) const
{
	//! If normBinWidthX or normBinWidthXY is negative, use the default bin width for this MUH3D for the respective axis
	if( normBinWidthX <= 0 )
		normBinWidthX = fNormBinWidthX;
	if( normBinWidthY <= 0 )
		normBinWidthY = fNormBinWidthY;
	if( normBinWidthZ <= 0 )
		normBinWidthZ = fNormBinWidthZ;

	MUH3D rval(*this);

	if( normBinWidthX > 0 &&  normBinWidthY > 0 && normBinWidthZ > 0 )
		rval.Scale( normBinWidthX * normBinWidthY * normBinWidthZ, "width" );

	return rval;
}

MUH1D * MUH3D::ProjectionX(const char* name /*= "_px"*/, Int_t firstybin /*= 0*/, Int_t lastybin /*= -1*/, Int_t firstzbin /*= 0*/, Int_t lastzbin /*= -1*/, Option_t* option /*= ""*/) const
{
	TH1D *cv_px = this->TH3D::ProjectionX(name, firstybin, lastybin, firstzbin, lastzbin, option); 
	MUH1D *h_px = new MUH1D( *cv_px );

	//! Getting Vertical Names
	std::vector<std::string> vertNames = GetVertErrorBandNames();
	for( unsigned int i = 0; i != vertNames.size(); ++i )
	{
		std::vector<TH1D*> vert_hists;
		const MUVertErrorBand3D *errBand = GetVertErrorBand(vertNames[i]);
		int nUniverses = errBand->GetNHists();
		for (int j = 0; j != nUniverses; ++j)
		{
			const TH3D *h_universe = dynamic_cast<const TH3D*>( errBand->GetHist(j) );
			TH1D *h_universe_px = h_universe->ProjectionX( Form("%s_%s_universe%i", name, vertNames[i].c_str(), j), firstybin, lastybin, firstzbin, lastzbin, option );
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
		const MULatErrorBand3D *errBand = GetLatErrorBand(latNames[i]);
		int nUniverses = errBand->GetNHists();
		for (int j = 0; j != nUniverses; ++j)
		{
			const TH3D *h_universe = dynamic_cast<const TH3D*>( errBand->GetHist(j) );
			TH1D *h_universe_px = h_universe->ProjectionX( Form("%s_%s_universe%i", name, latNames[i].c_str(), j), firstybin, lastybin, firstzbin, lastzbin, option );
			lat_hists.push_back(h_universe_px);
		}
		h_px->AddLatErrorBand(latNames[i], lat_hists);

		//cleaning
		for (std::vector<TH1D*>::iterator itHist = lat_hists.begin() ; itHist != lat_hists.end() ; ++ itHist)
			delete *itHist;
	}

	return h_px;
}

MUH1D * MUH3D::ProjectionY(const char* name /*= "_py"*/, Int_t firstxbin /*= 0*/, Int_t lastxbin /*= -1*/, Int_t firstzbin /*= 0*/, Int_t lastzbin /*= -1*/, Option_t* option /*= ""*/) const
{
	TH1D *cv_py = TH3D::ProjectionY(name, firstxbin, lastxbin, firstzbin, lastzbin, option); 
	MUH1D *h_py = new MUH1D( *cv_py );

	//! Getting Vertical Names
	std::vector<std::string> vertNames = GetVertErrorBandNames();
	for( unsigned int i = 0; i != vertNames.size(); ++i )
	{
		std::vector<TH1D*> vert_hists;
		const MUVertErrorBand3D *errBand = GetVertErrorBand(vertNames[i]);
		int nUniverses = errBand->GetNHists();
		for (int j = 0; j != nUniverses; ++j)
		{
			const TH3D *h_universe = dynamic_cast<const TH3D*>( errBand->GetHist(j) );
			TH1D *h_universe_py = h_universe->ProjectionY( Form("%s_%s_universe%i", name, vertNames[i].c_str(), j), firstxbin, lastxbin, firstzbin, lastzbin, option );
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
		const MULatErrorBand3D *errBand = GetLatErrorBand(latNames[i]);
		int nUniverses = errBand->GetNHists();
		for (int j = 0; j != nUniverses; ++j)
		{
			const TH3D *h_universe = dynamic_cast<const TH3D*>( errBand->GetHist(j) );
			TH1D *h_universe_py = h_universe->ProjectionY( Form("%s_%s_universe%i", name, latNames[i].c_str(), j), firstxbin, lastxbin, firstzbin, lastzbin, option );
			lat_hists.push_back(h_universe_py);
		}
		h_py->AddLatErrorBand(latNames[i], lat_hists);

		//cleaning
		for (std::vector<TH1D*>::iterator itHist = lat_hists.begin() ; itHist != lat_hists.end() ; ++ itHist)
			delete *itHist;
	}

	return h_py;
}

MUH1D * MUH3D::ProjectionZ(const char* name /*= "_pz"*/, Int_t firstxbin /*= 0*/, Int_t lastxbin /*= -1*/, Int_t firstybin /*= 0*/, Int_t lastybin /*= -1*/, Option_t* option /*= ""*/) const
{
	TH1D *cv_pz = TH3D::ProjectionZ(name, firstxbin, lastxbin, firstybin, lastybin, option); 
	MUH1D *h_pz = new MUH1D( *cv_pz );

	//! Getting Vertical Names
	std::vector<std::string> vertNames = GetVertErrorBandNames();
	for( unsigned int i = 0; i != vertNames.size(); ++i )
	{
		std::vector<TH1D*> vert_hists;
		const MUVertErrorBand3D *errBand = GetVertErrorBand(vertNames[i]);
		int nUniverses = errBand->GetNHists();
		for (int j = 0; j != nUniverses; ++j)
		{
			const TH3D *h_universe = dynamic_cast<const TH3D*>( errBand->GetHist(j) );
			TH1D *h_universe_pz = h_universe->ProjectionZ( Form("%s_%s_universe%i", name, vertNames[i].c_str(), j), firstxbin, lastxbin, firstybin, lastybin, option );
			vert_hists.push_back(h_universe_pz);
		}
		h_pz->AddVertErrorBand(vertNames[i], vert_hists);

		//cleaning
		for (std::vector<TH1D*>::iterator itHist = vert_hists.begin() ; itHist != vert_hists.end() ; ++ itHist)
			delete *itHist;
	}

	//! Getting Lateral Names
	std::vector<std::string> latNames = GetLatErrorBandNames();
	for( unsigned int i = 0; i != latNames.size(); ++i )
	{
		std::vector<TH1D*> lat_hists;
		const MULatErrorBand3D *errBand = GetLatErrorBand(latNames[i]);
		int nUniverses = errBand->GetNHists();
		for (int j = 0; j != nUniverses; ++j)
		{
			const TH3D *h_universe = dynamic_cast<const TH3D*>( errBand->GetHist(j) );
			TH1D *h_universe_pz = h_universe->ProjectionZ( Form("%s_%s_universe%i", name, latNames[i].c_str(), j), firstxbin, lastxbin, firstybin, lastybin, option );
			lat_hists.push_back(h_universe_pz);
		}
		h_pz->AddLatErrorBand(latNames[i], lat_hists);

		//cleaning
		for (std::vector<TH1D*>::iterator itHist = lat_hists.begin() ; itHist != lat_hists.end() ; ++ itHist)
			delete *itHist;
	}

	return h_pz;
}

TH1 * MUH3D::Project3D(Option_t* option /*= "x"*/) const
{

	TH1 *cv_p = TH3D::Project3D(option);
	int dim = cv_p->GetDimension();

	//!Crappy way to fill the universes for MUH1D/2D
	MUH1D *h_p1D = NULL;
	MUH2D *h_p2D = NULL;

	if (dim == 1)
	{
		h_p1D = new MUH1D( *(dynamic_cast<TH1D*>( cv_p )) );
		//! Getting Vertical Names
		std::vector<std::string> vertNames = GetVertErrorBandNames();
		for( unsigned int i = 0; i != vertNames.size(); ++i )
		{
			std::vector<TH1D*> vert_hists;
			const MUVertErrorBand3D *errBand = GetVertErrorBand(vertNames[i]);
			int nUniverses = errBand->GetNHists();
			for (int j = 0; j != nUniverses; ++j)
			{
				const TH3D *h_universe = dynamic_cast<const TH3D*>( errBand->GetHist(j) );
				TH1D *h_universe_p = dynamic_cast<TH1D*>( h_universe->Project3D( option ) );
				vert_hists.push_back( h_universe_p );
			}
			h_p1D->AddVertErrorBand(vertNames[i], vert_hists);

			//cleaning
			for (std::vector<TH1D*>::iterator itHist = vert_hists.begin() ; itHist != vert_hists.end() ; ++ itHist)
				delete *itHist;
		}

		//! Getting Lateral Names
		std::vector<std::string> latNames = GetLatErrorBandNames();
		for( unsigned int i = 0; i != latNames.size(); ++i )
		{
			std::vector<TH1D*> lat_hists;
			const MULatErrorBand3D *errBand = GetLatErrorBand(latNames[i]);
			int nUniverses = errBand->GetNHists();
			for (int j = 0; j != nUniverses; ++j)
			{
				const TH3D *h_universe = dynamic_cast<const TH3D*>( errBand->GetHist(j) );
				TH1D *h_universe_p = dynamic_cast<TH1D*>( h_universe->Project3D( option ) );
				lat_hists.push_back( h_universe_p );
			}
			h_p1D->AddLatErrorBand(latNames[i], lat_hists);

			//cleaning
			for (std::vector<TH1D*>::iterator itHist = lat_hists.begin() ; itHist != lat_hists.end() ; ++ itHist)
				delete *itHist;
		}
		return dynamic_cast<TH1*>( h_p1D );
	}//end of dim==1
	else if (dim == 2)
	{
		h_p2D = new MUH2D( *(dynamic_cast<TH2D*>( cv_p )) );
		//! Getting Vertical Names
		std::vector<std::string> vertNames = GetVertErrorBandNames();
		for( unsigned int i = 0; i != vertNames.size(); ++i )
		{
			std::vector<TH2D*> vert_hists;
			const MUVertErrorBand3D *errBand = GetVertErrorBand(vertNames[i]);
			int nUniverses = errBand->GetNHists();
			for (int j = 0; j != nUniverses; ++j)
			{
				const TH3D *h_universe = dynamic_cast<const TH3D*>( errBand->GetHist(j) );
				TH2D *h_universe_p = dynamic_cast<TH2D*>( h_universe->Project3D( option ) );
				vert_hists.push_back( h_universe_p );
			}
			h_p2D->AddVertErrorBand(vertNames[i], vert_hists);

			//cleaning
			for (std::vector<TH2D*>::iterator itHist = vert_hists.begin() ; itHist != vert_hists.end() ; ++ itHist)
				delete *itHist;
		}

		//! Getting Lateral Names
		std::vector<std::string> latNames = GetLatErrorBandNames();
		for( unsigned int i = 0; i != latNames.size(); ++i )
		{
			std::vector<TH2D*> lat_hists;
			const MULatErrorBand3D *errBand = GetLatErrorBand(latNames[i]);
			int nUniverses = errBand->GetNHists();
			for (int j = 0; j != nUniverses; ++j)
			{
				const TH3D *h_universe = dynamic_cast<const TH3D*>( errBand->GetHist(j) );
				TH2D *h_universe_p = dynamic_cast<TH2D*>( h_universe->Project3D( option ) );
				lat_hists.push_back( h_universe_p );
			}
			h_p2D->AddLatErrorBand(latNames[i], lat_hists);

			//cleaning
			for (std::vector<TH2D*>::iterator itHist = lat_hists.begin() ; itHist != lat_hists.end() ; ++ itHist)
				delete *itHist;
		}
		return dynamic_cast<TH1*>( h_p2D );
	}//end of dim==2
	else
	{ // just in case
		std::cout<<"[MUH3D::Project3D]: There was an error getting the projection dimension. Returning NULL pointer"<<std::endl;
		return (TH1*)NULL;
	}

}

bool MUH3D::AddVertErrorBand( const std::string& name, const int nhists /* = -1 */ )
{
	//! Make sure there are no ErrorBands with this name already
	if( HasErrorBand( name ) )
	{
		std::cout << "Warning [MUH3D::AddVertErrorBand] : There is already an error band with name \"" << name << "\".  Doing nothing." << std::endl;
		return false;
	}

	//! Error bands we own have this MUH3D's name as a prefix
	const std::string errName( std::string(GetName()) + "_" + name );

	//! non-positive nhists means you want to use the VertErrorBand's default
	if( nhists > 0 )
		fVertErrorBandMap[name] = new MUVertErrorBand3D( errName, (TH3D*)this, nhists );
	else
		fVertErrorBandMap[name] = new MUVertErrorBand3D( errName, (TH3D*)this );

	return true;
}

bool MUH3D::AddVertErrorBand( const std::string& name, const std::vector<TH3D*>& base )
{
	//! Make sure there are no ErrorBands with this name already
	if( HasErrorBand( name ) )
	{
		std::cout << "Warning [MUH3D::AddVertErrorBand] : There is already an error band with name \"" << name << "\".  Doing nothing." << std::endl;
		return false;
	}

	//! Error bands we own have this MUH1D's name as a prefix
	const std::string errName( std::string(GetName()) + "_" + name );

	//!Set the ErrorBand
	fVertErrorBandMap[name] = new MUVertErrorBand3D( errName, (TH3D*)this, base );

	return true;
}

bool MUH3D::AddLatErrorBand( const std::string& name, const int nhists /* = -1 */ )
{
	//! Make sure there are no ErrorBands with this name already
	if( HasErrorBand( name ) )
	{
		std::cout << "Warning [MUH3D::AddLatErrorBand] : There is already an error band with name \"" << name << "\".  Doing nothing." << std::endl;
		return false;
	}

	//! Error bands we own have this MUH3D's name as a prefix
	const std::string errName( std::string(GetName()) + "_" + name );

	//! non-positive nhists means you want to use the LatErrorBand's default
	if( nhists > 0 )
		fLatErrorBandMap[name] = new MULatErrorBand3D( errName, (TH3D*)this, nhists );
	else
		fLatErrorBandMap[name] = new MULatErrorBand3D( errName, (TH3D*)this );

	return true;
}

bool MUH3D::AddLatErrorBand( const std::string& name, const std::vector<TH3D*>& base )
{
	//! Make sure there are no ErrorBands with this name already
	if( HasErrorBand( name ) )
	{
		std::cout << "Warning [MUH3D::AddLatErrorBand] : There is already an error band with name \"" << name << "\".  Doing nothing." << std::endl;
		return false;
	}

	//! Error bands we own have this MUH1D's name as a prefix
	const std::string errName( std::string(GetName()) + "_" + name );

	//!Set the ErrorBand
	fLatErrorBandMap[name] = new MULatErrorBand3D( errName, (TH3D*)this, base );

	return true;
}


bool MUH3D::FillVertErrorBand( const std::string& name, const double xval, const double yval, const double zval, const std::vector<double>& weights, const double cvweight /* = 1.0 */, double cvWeightFromMe /*= 1.*/ )
{
	return FillVertErrorBand( name, xval, yval, zval, &(weights[0]), cvweight, cvWeightFromMe );
}

bool MUH3D::FillVertErrorBand( const std::string& name, const double xval, const double yval, const double zval, const double * weights, const double cvweight /* = 1.0 */, double cvWeightFromMe /*= 1.*/ )
{
	//! Try to fill a vertical error band
	MUVertErrorBand3D* vert = GetVertErrorBand( name );
	if( vert )
		return vert->Fill( xval, yval, zval, weights, cvweight, cvWeightFromMe );

	std::cout << "Warning [MUH3D::FillVertErrorBand] : Could not find a vertical error band to fill with name = " << name << std::endl;
	return false;
}

bool MUH3D::FillVertErrorBand( const std::string& name, const double xval, const double yval, const double zval, const double weightDown, const double weightUp, const double cvweight  /*= 1.0*/, double cvWeightFromMe /*= 1.*/ )
{
	//! Try to fill a vertical error band
	MUVertErrorBand3D *vert = GetVertErrorBand( name );
	if( vert )
		return vert->Fill( xval, yval, zval, weightDown, weightUp, cvweight, cvWeightFromMe );

	std::cout << "Warning [MUH3D::FillVertErrorBand] : Could not find a vertical error band to fill with name = " << name << std::endl;
	return false;
}

bool MUH3D::FillLatErrorBand( const std::string& name, const double xval, const double yval, const double zval, const std::vector<double>& xshifts, const std::vector<double>& yshifts, const std::vector<double>& zshifts, const double cvweight  /*= 1.0*/, const bool fillcv /*= true*/, const double* weights /*= NULL*/  )
{
	return FillLatErrorBand( name, xval, yval, zval, &(xshifts[0]), &(yshifts[0]), &(zshifts[0]), cvweight, fillcv, weights );
}

bool MUH3D::FillLatErrorBand( const std::string& name, const double xval, const double yval, const double zval, const double *xshifts, const double *yshifts, const double *zshifts, const double cvweight  /*= 1.0*/, const bool fillcv /*= true*/, const double* weights /*= NULL*/ )
{
	//! Try to fill a lateral error band
	MULatErrorBand3D* lat = GetLatErrorBand( name );
	if( lat )
		return lat->Fill( xval, yval, zval, xshifts, yshifts, zshifts, cvweight, fillcv, weights );

	std::cout << "Warning [MUH3D::FillLatErrorBand] : Could not find a lateral error band to fill with name = " << name << std::endl;

	return false;
}

bool MUH3D::FillLatErrorBand( const std::string& name, const double xval, const double yval, const double zval, const double xshiftDown, const double xshiftUp, const double yshiftDown, const double yshiftUp, const double zshiftDown, const double zshiftUp, const double cvweight /*= 1.0*/, const bool fillcv /*= true*/ )
{
	//! Try to fill a vertical error band
	MULatErrorBand3D *lat = GetLatErrorBand( name );
	if( lat )
		return lat->Fill( xval, yval, zval, xshiftDown, xshiftUp, yshiftDown, yshiftUp, zshiftDown, zshiftUp, cvweight, fillcv );

	std::cout << "Warning [MUH3D::FillLatErrorBand] : Could not find a lateral error band to fill with name = " << name << std::endl;

	return false;
}


MUVertErrorBand3D* MUH3D::GetVertErrorBand( const std::string& name )
{
	std::map<std::string, MUVertErrorBand3D*>::iterator i = fVertErrorBandMap.find( name );
	if( i == fVertErrorBandMap.end() )
	{
		std::cout << "Warning [MUH3D::GetVertErrorBand] : There is no vertical error band with name \"" << name << "\".  Returning NULL." << std::endl;
		return NULL;
	}

	return i->second;
}

const MUVertErrorBand3D* MUH3D::GetVertErrorBand( const std::string& name ) const
{
	std::map<std::string, MUVertErrorBand3D*>::const_iterator i = fVertErrorBandMap.find( name );
	if( i == fVertErrorBandMap.end() )
	{
		std::cout << "Warning [MUH3D::GetVertErrorBand] : There is no vertical error band with name \"" << name << "\".  Returning NULL." << std::endl;
		return NULL;
	}

	return i->second;
}

MULatErrorBand3D* MUH3D::GetLatErrorBand( const std::string& name )
{
	std::map<std::string, MULatErrorBand3D*>::iterator i = fLatErrorBandMap.find( name );
	if( i == fLatErrorBandMap.end() )
	{
		std::cout << "Warning [MUH3D::GetLatErrorBand] : There is no lateral error band with name \"" << name << "\".  Returning NULL." << std::endl;
		return NULL;
	}

	return i->second;
}

const MULatErrorBand3D* MUH3D::GetLatErrorBand( const std::string& name ) const
{
	std::map<std::string, MULatErrorBand3D*>::const_iterator i = fLatErrorBandMap.find( name );
	if( i == fLatErrorBandMap.end() )
	{
		std::cout << "Warning [MUH3D::GetLatErrorBand] : There is no lateral error band with name \"" << name << "\".  Returning NULL." << std::endl;
		return NULL;
	}

	return i->second;
}

bool MUH3D::HasLatErrorBand( const std::string& name ) const
{
	//! Check the MULatErrorBands
	if( fLatErrorBandMap.find( name ) != fLatErrorBandMap.end() )
		return true;

	return false;
}

bool MUH3D::HasVertErrorBand( const std::string& name ) const
{
	//! Check the MUVertErrorBands
	if( fVertErrorBandMap.find( name ) != fVertErrorBandMap.end() )
		return true;

	return false;
}

bool MUH3D::HasErrorBand( const std::string& name ) const
{
	//! Check the MULatErrorBands
	if( HasLatErrorBand( name ) )
		return true;

	//! Check the MUVertErrorBands
	if( HasVertErrorBand( name ) )
		return true;

	return false;
}

bool MUH3D::HasErrorMatrix( const std::string& name ) const
{
	//! Check the fSysErrorMatrix
	if( fSysErrorMatrix.find( name ) != fSysErrorMatrix.end() )
		return true;

	return false;
}

std::vector<std::string> MUH3D::GetVertErrorBandNames() const
{
	std::vector<std::string> rval;
	for( std::map<std::string, MUVertErrorBand3D*>::const_iterator i = fVertErrorBandMap.begin(); i != fVertErrorBandMap.end(); ++i )
		rval.push_back( i->first );
	return rval;
}

std::vector<std::string> MUH3D::GetLatErrorBandNames() const
{
	std::vector<std::string> rval;
	for( std::map<std::string, MULatErrorBand3D*>::const_iterator i = fLatErrorBandMap.begin(); i != fLatErrorBandMap.end(); ++i )
		rval.push_back( i->first );
	return rval;
}

std::vector<std::string> MUH3D::GetSysErrorMatricesNames() const
{

	std::vector<std::string> rval;
	//Vertical Errors
	for( std::map<std::string, MUVertErrorBand3D*>::const_iterator i = fVertErrorBandMap.begin(); i != fVertErrorBandMap.end(); ++i )
		rval.push_back( i->first );

	//Lateral Errors
	for( std::map<std::string, MULatErrorBand3D*>::const_iterator i = fLatErrorBandMap.begin(); i != fLatErrorBandMap.end(); ++i )
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
TMatrixD MUH3D::GetSysErrorMatrix(const std::string& name, bool asFrac /*= false*/, bool cov_area_normalize /*= false*/) const
{
	if (HasEnding(name,"_asShape") )
		std::cout << "Warning [MUH3D::GetSysErrorMatrix]: You are calling the error Matrix: " << name <<".\nAssuming the error Band wanted is: " << name.substr(0,name.length()-8) << " with cov_area_normalize = true" << std::endl;

	const std::string name_condition = ( (cov_area_normalize) && !(HasEnding(name,"_asShape")) )?  "_asShape" : "";
	const std::string fname = name + name_condition ; 
	const std::string errName = HasEnding(fname,"_asShape") ? fname.substr(0,fname.length()-8) : fname;

	//! @todo what to do with underflow( bin=0 ) and overflow ( bin=nbins+1 )?
	const int highBinX = GetNbinsX() + 1;
	const int highBinY = GetNbinsY() + 1;
	const int highBinZ = GetNbinsZ() + 1;
	const int highBin  = GetBin( highBinX, highBinY, highBinZ );
	const int lowBin = 0;
	TMatrixD covmx(highBin+1,highBin+1);

	if ( HasErrorMatrix( fname ) )
		covmx = *(fSysErrorMatrix.find(fname)->second);

	else if( fLatErrorBandMap.find( errName ) != fLatErrorBandMap.end() )
	{
		std::map<std::string, MULatErrorBand3D*>::const_iterator it = fLatErrorBandMap.find( errName );
		covmx = it->second->CalcCovMx( ( HasEnding(fname,"_asShape") ) );
	}
	else if( fVertErrorBandMap.find( errName ) != fVertErrorBandMap.end() )
	{
		std::map<std::string, MUVertErrorBand3D*>::const_iterator it = fVertErrorBandMap.find( errName );
		covmx = it->second->CalcCovMx( ( HasEnding(fname,"_asShape") ) );
	}
	else 
		std::cout << "Warning [MUH3D::GetSysErrorMatrix]: There is no Covariance Matrix with name " << fname << ".Returning and empty Matrix." << std::endl;

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

TMatrixD MUH3D::GetStatErrorMatrix( bool asFrac /* =false */ ) const
{
	const int highBinX = GetNbinsX() + 1;
	const int highBinY = GetNbinsY() + 1;
	const int highBinZ = GetNbinsZ() + 1;
	const int highBin  = GetBin( highBinX, highBinY, highBinZ );
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
TMatrixD MUH3D::GetTotalErrorMatrix(
		bool includeStat /*= true*/, 
		bool asFrac /*= false*/, 
		bool cov_area_normalize /*= false*/ ) const
{

	//! @todo what to do with underflow( bin=0 ) and overflow ( bin=nbins+1 )?
	const int highBinX = GetNbinsX() + 1;
	const int highBinY = GetNbinsY() + 1;
	const int highBinZ = GetNbinsZ() + 1;
	const int highBin  = GetBin( highBinX, highBinY, highBinZ );
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

TMatrixD MUH3D::GetTotalCorrelationMatrix( bool cov_area_normalize /*= false*/ ) const
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

TH3D MUH3D::GetTotalError(
		bool includeStat /* = true */, 
		bool asFrac /* = false */ ,
		bool cov_area_normalize /*= false */) const
{
	//! Make a copy of this histogram as a TH1D and rename it
	TH3D err( *this );
	err.Reset();
	std::string tmpName( std::string(GetName()) + "_TotalError");
	err.SetName( tmpName.c_str() );

	//! @todo what to do with underflow( bin=0 ) and overlow ( bin=nbins+1 )?
	const int highBinX = GetNbinsX() + 1;
	const int highBinY = GetNbinsY() + 1;
	const int highBinZ = GetNbinsZ() + 1;
	const int highBin  = GetBin( highBinX, highBinY, highBinZ );
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
TH3D MUH3D::GetStatError( bool asFrac /* = false */ ) const
{
	//! Make a copy of this histogram as a TH1D and rename it
	TH3D err( *this );
	err.Reset();
	std::string tmpName( std::string(GetName()) + "_StatError");
	err.SetName( tmpName.c_str() );

	//! @todo what to do with underflow( bin=0 ) and overlow ( bin=nbins+1 )?
	const int highBinX = GetNbinsX() + 1;
	const int highBinY = GetNbinsY() + 1;
	const int highBinZ = GetNbinsZ() + 1;
	const int highBin  = GetBin( highBinX, highBinY, highBinZ );
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

TH3D MUH3D::GetCVHistoWithError( bool includeStat /* = true */ , bool cov_area_normalize /* = false */) const
{
	//! @todo Check the highBin value for here (in MUH1D, overflow is not set for rval.SetBinError)
	const int highBinX = GetNbinsX() + 1;
	const int highBinY = GetNbinsY() + 1;
	const int highBinZ = GetNbinsZ() + 1;
	const int highBin  = GetBin( highBinX, highBinY, highBinZ );
	const int lowBin = 0;

	//! Get the error band
	TH3D err = GetTotalError( includeStat , false, cov_area_normalize);

	//! Create a copy of this histogram and rename it
	TH3D rval( *this );
	std::string tmpName( std::string( GetName() ) + "_CV_WithErr" );
	rval.SetName( tmpName.c_str() );

	for( int iBin = lowBin; iBin <= highBin; ++iBin )
		rval.SetBinError( iBin, err.GetBinContent(iBin) );

	return rval;
}

TH3D MUH3D::GetCVHistoWithStatError() const
{
	//! @todo Check the highBin value for here (in MUH1D, overflow is not set for rval.SetBinError)
	const int highBinX = GetNbinsX() + 1;
	const int highBinY = GetNbinsY() + 1;
	const int highBinZ = GetNbinsZ() + 1;
	const int highBin  = GetBin( highBinX, highBinY, highBinZ );
	const int lowBin = 0;

	//! Get the stat. error band
	TH3D err = GetStatError( false );

	//! Create a copy of this histogram and rename it
	TH3D rval( *this );
	std::string tmpName( std::string( GetName() ) + "_CV_WithStatErr" );
	rval.SetName( tmpName.c_str() );

	for( int iBin = lowBin; iBin <= highBin; ++iBin )
		rval.SetBinError( iBin, err.GetBinContent(iBin) );

	return rval;
}

//======================================================================
// Replacements of ROOT versions of functions
//======================================================================

void MUH3D::Scale( Double_t c1 /*= 1.*/, Option_t* option /*=""*/ )
{
	//! Scale yourself using TH1D::Scale
	this->TH3D::Scale( c1, option );

	//! Scale the vertical error bands
	for( std::map<std::string, MUVertErrorBand3D*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
		it->second->Scale( c1, option );

	//! Scale the lateral error bands
	for( std::map<std::string, MULatErrorBand3D*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
		it->second->Scale( c1, option );
}

void MUH3D::Add( const TH3* h1, const Double_t c1 /*= 1.*/ )
{
	//! Try to cast the input TH3 to a MUH3D
	const MUH3D *mnv1 = dynamic_cast<const MUH3D*>(h1);

	if( mnv1 )
	{

		//! Add as a TH3D
		this->TH3D::Add( h1, c1 );

		//! Call Add for all vertical error bands
		for( std::map<std::string, MUVertErrorBand3D*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
		{
			const MUVertErrorBand3D* err1 = mnv1->GetVertErrorBand( it->first );
			if( !err1  )
			{
				Error("Add", "Could not add MUH3Ds because they all don't have the %s MUVertErrorBand3D", it->first.c_str());
				return;
			}

			Bool_t ok = it->second->Add( err1, c1 );

			if( ! ok )
			{
				Error("Add", "Could not add MUH3Ds because histogram add failed for MUVertErrorBand3D %s ", it->first.c_str());
				return;
			}
		}//done adding Vert errors

		//! Call Add for all lateral error bands
		for( std::map<std::string, MULatErrorBand3D*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
		{
			const MULatErrorBand3D* err1 = mnv1->GetLatErrorBand( it->first );
			if( !err1  )
			{
				Error("Add", "Could not add MUH3Ds because they all don't have the %s MULatErrorBand3D", it->first.c_str());
				return;
			}

			Bool_t ok = it->second->Add( err1, c1 );

			if( ! ok )
			{
				Error("Add", "Could not add MUH3Ds because histogram add failed for MULatErrorBand3D %s ", it->first.c_str());
				return;
			}
		}//done adding Lat errors

	}// end if cast to MUH3D worked
	else
	{
		Error( "MUH3D::Add", "Unable to add histogram because it could not be cast to an MUH3D.  Did nothing." );
	}

}

void MUH3D::Multiply( const MUH3D* h1, const MUH3D* h2, Double_t c1 /*= 1*/, Double_t c2 /*= 1*/ )
{
	//! @todo Would love to return a bool here, but we want this Multiply to override TH1's and that is void

	//! Call the TH1D Multiply 
	this->TH3D::Multiply( (TH3D*)h1, (TH3D*)h2, c1, c2 );

	//! Scale the vertical error bands
	for( std::map<std::string, MUVertErrorBand3D*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
	{
		const MUVertErrorBand3D* err1 = h1->GetVertErrorBand( it->first );
		const MUVertErrorBand3D* err2 = h2->GetVertErrorBand( it->first );
		if( !err1 || !err2 )
		{
			Error("Multiply", "Could not divide MUH3Ds because they all don't have the %s MUVertErrorBand3D", it->first.c_str());
			return;
		}
		it->second->Multiply( err1, err2, c1, c2 );
	}

	//! Scale the lateral error bands
	for( std::map<std::string, MULatErrorBand3D*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
	{
		const MULatErrorBand3D* err1 = h1->GetLatErrorBand( it->first );
		const MULatErrorBand3D* err2 = h2->GetLatErrorBand( it->first );
		if( !err1 || !err2 )
		{
			Error("Multiply", "Could not divide MUH3Ds because they all don't have the %s MULatErrorBand3D", it->first.c_str());
			return;
		}
		it->second->Multiply( err1, err2, c1, c2 );
	}

	return;
}

void MUH3D::Divide( const MUH3D* h1, const MUH3D* h2, Double_t c1 /*= 1*/, Double_t c2 /*= 1*/, Option_t* option /*=""*/ )
{
	//! @todo Would love to return a bool here, but we want this Divide to override TH1's and that is void

	//! Call the TH1D Divide
	this->TH3D::Divide( (TH3D*)h1, (TH3D*)h2, c1, c2, option );

	//! Scale the vertical error bands
	for( std::map<std::string, MUVertErrorBand3D*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
	{
		const MUVertErrorBand3D* err1 = h1->GetVertErrorBand( it->first );
		const MUVertErrorBand3D* err2 = h2->GetVertErrorBand( it->first );
		if( !err1 || !err2 )
		{
			Error("Divide", "Could not divide MUH3Ds because they all don't have the %s MUVertErrorBand3D", it->first.c_str());
			return;
		}
		it->second->Divide( err1, err2, c1, c2, option );
	}

	//! Scale the lateral error bands
	for( std::map<std::string, MULatErrorBand3D*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
	{
		const MULatErrorBand3D* err1 = h1->GetLatErrorBand( it->first );
		const MULatErrorBand3D* err2 = h2->GetLatErrorBand( it->first );
		if( !err1 || !err2 )
		{
			Error("Divide", "Could not divide MUH3Ds because they all don't have the %s MULatErrorBand3D", it->first.c_str());
			return;
		}
		it->second->Divide( err1, err2, c1, c2, option );
	}

	return;
}

void MUH3D::DivideSingle( const MUH3D* h1, const TH3* h2, Double_t c1 /*= 1*/, Double_t c2 /*= 1*/, Option_t* option /*=""*/ )
{
	//! Call the TH1D Divide
	this->TH3D::Divide( (TH3D*)h1, h2, c1, c2, option );

	//! Scale the vertical error bands
	for( std::map<std::string, MUVertErrorBand3D*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
	{
		const MUVertErrorBand3D* err1 = h1->GetVertErrorBand( it->first );
		if( !err1 )
		{
			Error("Divide", "Could not divide MUH3Ds because they all don't have the %s MUVertErrorBand3D", it->first.c_str());
			return;
		}
		it->second->DivideSingle( err1, h2, c1, c2, option );
	}

	//! Scale the lateral error bands
	for( std::map<std::string, MULatErrorBand3D*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
	{
		const MULatErrorBand3D* err1 = h1->GetLatErrorBand( it->first );
		if( !err1 )
		{
			Error("Divide", "Could not divide MUH3Ds because they all don't have the %s MULatErrorBand3D", it->first.c_str());
			return;
		}
		it->second->DivideSingle( err1, h2, c1, c2, option );
	}

	return;
}



//--------------------------------------------------------
// trivial helper functions
//--------------------------------------------------------
bool MUH3D::HasEnding (std::string const &fullString, std::string const &ending) const
{
	TString a(fullString);
	return a.EndsWith( ending.c_str() );
}

#endif
