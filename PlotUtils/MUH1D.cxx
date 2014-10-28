#ifndef MNV_MUH1D_cxx
#define MNV_MUH1D_cxx 1

#include "PlotUtils/MUH1D.h"
#include "HistogramUtils.h"

#include <TMath.h>
#include <TDirectory.h>

using namespace PlotUtils;

ClassImp(MUH1D);

//==================================================================================
// CONSTRUCTORS
//==================================================================================

//--------------------------------------------------------
// Copy constructors with default normalized bin width
//--------------------------------------------------------
MUH1D::MUH1D() : 
  TH1D(),
  fNormBinWidth(1.)
{ 
  SilentSumw2(); 
}

MUH1D::MUH1D( const TVectorD& v ) :
  TH1D( v ),
  fNormBinWidth(1.)
{ 
  SilentSumw2(); 
}

MUH1D::MUH1D( const TH1D& h1d ) :
  TH1D( h1d ) 
{ 
  fNormBinWidth = h1d.GetBinWidth(1);
  SilentSumw2();
}


//--------------------------------------------------------
// Copy constructors with specified normalized bin width
//--------------------------------------------------------
MUH1D::MUH1D(Double_t normBinWidth) :
  TH1D(),
  fNormBinWidth(normBinWidth)
{
  SilentSumw2();
}

MUH1D::MUH1D( const TVectorD& v, Double_t normBinWidth ) :
  TH1D( v ),
  fNormBinWidth(normBinWidth)
{
  SilentSumw2(); 
}

MUH1D::MUH1D( const TH1D& h1d, Double_t normBinWidth ) :
  TH1D( h1d ),
  fNormBinWidth(normBinWidth)
{ 
  SilentSumw2();
}

MUH1D::MUH1D( const MUH1D& h ) :
  TH1D( h )
{
  // Deep copy the variables
  DeepCopy( h );
}

MUH1D& MUH1D::operator=( const MUH1D& h )
{
  // If this is me, then no copy is necessary
  if( this == &h )
    return *this;

  //call the base class assignment operator
  this->TH1D::operator=(h);

  // Delete and clear all vert and lat error bands
  for( std::map<std::string, MUVertErrorBand*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
    delete it->second;
  fVertErrorBandMap.clear();

  for( std::map<std::string, MULatErrorBand*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
    delete it->second;
  fLatErrorBandMap.clear();

  //delete and clear all uncorr errors
  for( std::map<std::string, TH1D*>::iterator it = fUncorrErrorMap.begin(); it != fUncorrErrorMap.end(); ++it )
    delete it->second;
  fUncorrErrorMap.clear();


  // Then deeop copy the variables
  DeepCopy(h);

  return *this;
}

void MUH1D::DeepCopy( const MUH1D& h )
{
  // Set bin norm width
  fNormBinWidth = h.GetNormBinWidth();

  //change to this's directory so children are in the same place
  const TString oldDir = gDirectory->GetPath();
  this->GetDirectory()->cd();


  // Copy the vert and lat error bands
  std::vector<std::string> vertNames = h.GetVertErrorBandNames();
  for( std::vector<std::string>::iterator name = vertNames.begin(); name != vertNames.end(); ++name )
    fVertErrorBandMap[*name] = new MUVertErrorBand( *h.GetVertErrorBand(*name) );

  std::vector<std::string> latNames = h.GetLatErrorBandNames();
  for( std::vector<std::string>::iterator name = latNames.begin(); name != latNames.end(); ++name )
    fLatErrorBandMap[*name] = new MULatErrorBand( *h.GetLatErrorBand(*name) );

  //copy all uncorr errors
  std::vector<std::string> uncorrNames = h.GetUncorrErrorNames();
  for( std::vector<std::string>::iterator name = uncorrNames.begin(); name != uncorrNames.end(); ++name )
    fUncorrErrorMap[*name] = new TH1D( *h.GetUncorrError(*name) );

  //copy all "special" error matrices
  //todo: These were unfortunately named.  There is not always a clear distinction between these "special" error matrices and those derived from error bands.
  //todo: It's not clear how we want to handle shape only stuff here. GetSysErrorMatricesNames does not include them but that might be fine.
  std::vector<std::string> allSysNames = h.GetSysErrorMatricesNames();
  for( std::vector<std::string>::iterator name = allSysNames.begin(); name != allSysNames.end(); ++name )
  {
    //this restricts us to only the special matrices
    if( h.HasErrorMatrix(*name) )
     fSysErrorMatrix[*name] = new TMatrixD( h.GetSysErrorMatrix(*name) );
  }

  //add removed "special" error matrices
  std::vector<std::string> removedSysNames = h.GetRemovedSysErrorMatricesNames();
  for( std::vector<std::string>::iterator name = removedSysNames.begin(); name != removedSysNames.end(); ++name )
     fRemovedSysErrorMatrix[*name] = new TMatrixD( h.GetSysErrorMatrix(*name) );

  gDirectory->cd(oldDir);

}

//--------------------------------------------------------
// Constructors with default normalization bin width
//--------------------------------------------------------
MUH1D::MUH1D( const char* name, const char* title, Int_t nbinsx, const Float_t* xbins) :
  TH1D( name, title, nbinsx, xbins) 
{ 
  // Default normlization bin width is the first bin width
  fNormBinWidth = xbins[1] - xbins[0];
  SilentSumw2();
}

MUH1D::MUH1D( const char* name, const char* title, Int_t nbinsx, const Double_t* xbins) :
  TH1D( name, title, nbinsx, xbins) 
{ 
  // Default normlization bin width is the first bin width
  fNormBinWidth = xbins[1] - xbins[0];
  SilentSumw2();
}

MUH1D::MUH1D( const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup):
  TH1D( name, title, nbinsx, xlow, xup )
{ 
  // Default normalization bin width is the constant width of the bins
  fNormBinWidth = (xup - xlow) / float(nbinsx);
  SilentSumw2();
}

//--------------------------------------------------------
// Constructors with specified normalization bin width
//--------------------------------------------------------
MUH1D::MUH1D( const char* name, const char* title, Int_t nbinsx, const Float_t* xbins, Double_t normBinWidth) :
  TH1D( name, title, nbinsx, xbins),
  fNormBinWidth(normBinWidth)
{ 
  SilentSumw2();
}

MUH1D::MUH1D( const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Double_t normBinWidth) :
  TH1D( name, title, nbinsx, xbins),
  fNormBinWidth(normBinWidth)
{ 
  SilentSumw2();
}

MUH1D::MUH1D( const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Double_t normBinWidth):
  TH1D( name, title, nbinsx, xlow, xup ),
  fNormBinWidth(normBinWidth)
{
  SilentSumw2();
}


//--------------------------------------------------------
// trivial helper functions
//--------------------------------------------------------
bool MUH1D::HasEnding (std::string const &fullString, std::string const &ending) const
{
  TString a(fullString);
  return a.EndsWith( ending.c_str() );
}


void MUH1D::SilentSumw2()
{
  if( 0 == GetSumw2N() )
    Sumw2();
}


//! Rename all histograms inside MUH1D + Error Bands
void MUH1D::RenameHistosAndErrorBands( const std::string& name )
{

  this->SetName( name.c_str() );

  std::vector<std::string> vert_errBandNames = this->GetVertErrorBandNames();
  std::vector<std::string> lat_errBandNames  = this->GetLatErrorBandNames();
  std::vector<std::string> uncorr_errBandNames  = this->GetUncorrErrorNames();
  for (std::vector<std::string>::iterator itName = vert_errBandNames.begin(); itName != vert_errBandNames.end(); ++itName) 
  {
    MUVertErrorBand* tmp_band = this->GetVertErrorBand(*itName);
    std::string band_name = std::string(name + "_" + *itName);
    tmp_band->SetName( band_name.c_str() );
    for (unsigned int i=0; i < tmp_band->GetNHists(); ++i)
      tmp_band->GetHist(i)->SetName( Form("%s_universe%d",band_name.c_str(),i) );
  }

  for (std::vector<std::string>::iterator itName = lat_errBandNames.begin(); itName != lat_errBandNames.end(); ++itName) 
  {
    MULatErrorBand* tmp_band = this->GetLatErrorBand(*itName);
    std::string band_name = std::string(name + "_" + *itName);
    tmp_band->SetName( band_name.c_str() );
    for (unsigned int i=0; i < tmp_band->GetNHists(); ++i)
      tmp_band->GetHist(i)->SetName( Form("%s_universe%d",band_name.c_str(),i) );
  }

  for (std::vector<std::string>::iterator itName = uncorr_errBandNames.begin(); itName != uncorr_errBandNames.end(); ++itName)
  {
    TH1D* tmp_band = this->GetUncorrError(*itName);
    std::string band_name = std::string(name + "_" + *itName);
    tmp_band->SetName( band_name.c_str() );
  }

}

//! Delete all Error Bands 
void MUH1D::ClearAllErrorBands()
{

  ClearSysErrorMatrices( );

  // Delete and clear all vert and lat error bands
  for( std::map<std::string, MUVertErrorBand*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
    delete it->second;
  fVertErrorBandMap.clear();

  for( std::map<std::string, MULatErrorBand*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
    delete it->second;
  fLatErrorBandMap.clear();

  for( std::map<std::string, TH1D*>::iterator it = fUncorrErrorMap.begin(); it != fUncorrErrorMap.end(); ++it )
    delete it->second;
  fUncorrErrorMap.clear();

}

bool MUH1D::AddLatErrorBand( const std::string& name, const int nhists /* = -1 */ )
{
  // Make sure there are no ErrorBands with this name already
  if( HasErrorBand( name ) )
  {
    Warning("MUH1D::AddLatErrorBand", "There is already an error band with name \"%s\".  Doing nothing.", name.c_str());
    return false;
  }

  // Error bands we own have this MUH1D's name as a prefix
  const std::string errName( std::string(GetName()) + "_" + name );

  //change to this's directory so children are in the same place
  const TString oldDir = gDirectory->GetPath();
  this->GetDirectory()->cd();

  // non-positive nhists means you want to use the LatErrorBandDefault
  if( nhists > 0 )
    fLatErrorBandMap[name] = new MULatErrorBand( errName, (TH1D*)this, nhists );
  else
    fLatErrorBandMap[name] = new MULatErrorBand( errName, (TH1D*)this );

  gDirectory->cd(oldDir);

  return true;
}

bool MUH1D::AddLatErrorBand( const std::string& name, const std::vector<TH1D*>& base )
{
  // Make sure there are no ErrorBands with this name already
  if( HasErrorBand( name ) )
  {
    Warning("MUH1D::AddLatErrorBand", "There is already an error band with name \"%s\".  Doing nothing.", name.c_str());
    return false;
  }

  // Error bands we own have this MUH1D's name as a prefix
  const std::string errName( std::string(GetName()) + "_" + name );

  //change to this's directory so children are in the same place
  const TString oldDir = gDirectory->GetPath();
  this->GetDirectory()->cd();

  // Set the ErrorBand
  fLatErrorBandMap[name] = new MULatErrorBand( errName, (TH1D*)this, base );
  
  gDirectory->cd(oldDir);

  return true;
}

bool MUH1D::AddLatErrorBandAndFillWithCV( const std::string& name, const int nhists )
{
  // Make sure there are no ErrorBands with this name already
  if( HasErrorBand( name ) )
  {
    Warning("MUH1D::AddLatErrorBandAndFillWithCV", "There is already an error band with name \"%s\".  Doing nothing.", name.c_str());
    return false;
  }

  // Make sure number of requested number of histos is not negative 
  if( nhists < 0 )
  {
    Warning("MUH1D::AddLatErrorBandAndFillWithCV", "Passing a negative number of universes to create error band with name \"%s\".  Doing nothing.", name.c_str());
    return false;
  }

  // Make a vector of histos with the CV 
  std::vector<TH1D*> histos;
  for( int universe=0; universe<nhists; ++universe )
  {
    TH1D* histo = new TH1D( *this );
    histo->SetName( Form( "tmp_universe_%i", universe ) );
    histos.push_back( histo );
  }

  // Add the error band and fill it with the vector of histos
  bool ok = this->AddLatErrorBand( name, histos );

  // Clean vector of histos
  for( std::vector<TH1D*>::iterator it=histos.begin(); it!=histos.end(); ++it )
  {
    if( *it ) delete *it;
  }

  return ok;
}

bool MUH1D::AddVertErrorBand( const std::string& name, const int nhists /* = -1 */ )
{
  // Make sure there are no ErrorBands with this name already
  if( HasErrorBand( name ) )
  {
    Warning("MUH1D::AddVertErrorBand", "There is already an error band with name \"%s\".  Doing nothing.", name.c_str());
    return false;
  }

  // Error bands we own have this MUH1D's name as a prefix
  const std::string errName( std::string(GetName()) + "_" + name );

  //change to this's directory so children are in the same place
  const TString oldDir = gDirectory->GetPath();
  this->GetDirectory()->cd();

  // non-positive nhists means you want to use the VertErrorBand's default
  if( nhists > 0 )
    fVertErrorBandMap[name] = new MUVertErrorBand( errName, (TH1D*)this, nhists );
  else
    fVertErrorBandMap[name] = new MUVertErrorBand( errName, (TH1D*)this );

  gDirectory->cd(oldDir);

  return true;
}

bool MUH1D::AddVertErrorBand( const std::string& name, const std::vector<TH1D*>& base )
{
  // Make sure there are no ErrorBands with this name already
  if( HasErrorBand( name ) )
  {
    Warning("MUH1D::AddVertErrorBand", "There is already an error band with name \"%s\".  Doing nothing.", name.c_str());
    return false;
  }

  // Error bands we own have this MUH1D's name as a prefix
  const std::string errName( std::string(GetName()) + "_" + name );

  //change to this's directory so children are in the same place
  const TString oldDir = gDirectory->GetPath();
  this->GetDirectory()->cd();

  // Set the ErrorBand
  fVertErrorBandMap[name] = new MUVertErrorBand( errName, (TH1D*)this, base );
  
  gDirectory->cd(oldDir);

  return true;
}


bool MUH1D::AddUncorrError( const std::string& name )
{
  // Make sure there are no ErrorBands with this name already
  if( HasErrorBand( name ) )
  {
    Warning("MUH1D::AddUncorrError", "There is already an error band with name \"%s\".  Doing nothing.", name.c_str());
    return false;
  }

  // Histogram will have this MUH1D's name as a prefix
  const std::string errName( std::string(GetName()) + "_" + name );

  //create the uncorrelated error histogram by making a copy of self
  //reset the histo because we intend to fill it ourselves
  TH1D *uncorrHist = dynamic_cast<TH1D*>( this->TH1D::Clone( errName.c_str() ) );
  uncorrHist->Reset();

  // Set the ErrorBand in the map
  fUncorrErrorMap[name] = uncorrHist;

  return true;
}


bool MUH1D::AddUncorrError( const std::string& name, const TH1D *hist, bool errInContent /*=false*/ )
{
  // Make sure there are no ErrorBands with this name already
  if( HasErrorBand( name ) )
  {
    Warning("MUH1D::AddUncorrError", "There is already an error band with name \"%s\".  Doing nothing.", name.c_str());
    return false;
  }

  // Histogram will have this MUH1D's name as a prefix
  const std::string errName( std::string(GetName()) + "_" + name );

  //create the uncorrelated error histogram by making a copy of self
  //keep content intact since we assume no more filling will happen
  TH1D *uncorrHist = dynamic_cast<TH1D*>( this->TH1D::Clone( errName.c_str() ) );

  //replace the errors with those from provided hist
  for( int i = 0; i <= hist->GetNbinsX()+1; ++i )
  {
    const double binErr = errInContent ?
      hist->GetBinContent(i):
      hist->GetBinError(i);
    uncorrHist->SetBinError(i, binErr);
  }

  // Set the ErrorBand in the map
  fUncorrErrorMap[name] = uncorrHist;

  return true;
}

bool MUH1D::AddUncorrErrorAndFillWithCV( const std::string& name )
{
  // Make sure there are no Errors with this name already
  if( HasErrorBand( name ) )
  {
    Warning("MUH1D::AddUncorrErrorAndFillWithCV", "There is already an uncorr error with name \"%s\".  Doing nothing.", name.c_str());
    return false;
  }

  // Histogram will have this MUH1D's name as a prefix
  const std::string errName( std::string(GetName()) + "_" + name );

  //create the uncorrelated error histogram by making a copy of self
  //keep content intact since we assume no more filling will happen
  TH1D *uncorrHist = dynamic_cast<TH1D*>( this->TH1D::Clone( errName.c_str() ) );

  //replace the errors with 0
  for( int i = 0; i <= uncorrHist->GetNbinsX()+1; ++i )
    uncorrHist->SetBinError(i, 0.);

  // Set the ErrorBand in the map
  fUncorrErrorMap[name] = uncorrHist;

  return true;
}




bool MUH1D::AddVertErrorBandAndFillWithCV( const std::string& name, const int nhists )
{
  // Make sure there are no ErrorBands with this name already
  if( HasErrorBand( name ) )
  {
    Warning("MUH1D::AddVertErrorBandAndFillWithCV", "There is already an error band with name \"%s\".  Doing nothing.", name.c_str());
    return false;
  }

  // Make sure requested number of histos is not negative 
  if( nhists < 0 )
  {
    Warning("MUH1D::AddVertErrorBandAndFillWithCV", "Passing a negative number of universes to create error band with name \"%s\".  Doing nothing.", name.c_str());
    return false;
  }

  // Make a vector of histos with the CV 
  std::vector<TH1D*> histos;
  for( int universe=0; universe<nhists; ++universe )
  {
    TH1D* histo = new TH1D( *this );
    histo->SetName( Form( "tmp_universe_%i", universe ) );
    histos.push_back( histo );
  }

  // Add the error band and fill it with the vector of histos
  bool ok = this->AddVertErrorBand( name, histos );

  // Clean vector of histos
  for( std::vector<TH1D*>::iterator it=histos.begin(); it!=histos.end(); ++it )
  {
    if( *it ) delete *it;
  }

  return ok;
}


bool MUH1D::AddMissingErrorBandsAndFillWithCV( const MUH1D& ref )
{
  // Declare container for error band names
  std::vector<std::string> names = ref.GetVertErrorBandNames();

  // Add vertical error bands found in reference MUH1D
  for( std::vector<std::string>::iterator name=names.begin(); name!=names.end(); ++name )
  {
    // Skip already added vertical error bands
    if( HasVertErrorBand( *name ) ) 
      continue;
    unsigned int nunis = ref.GetVertErrorBand( *name )->GetNHists();
    if( ! this->AddVertErrorBandAndFillWithCV( *name, nunis ) )
      return false;
  }

  // Add lateral error bands found in reference MUH1D
  names = ref.GetLatErrorBandNames();
  for( std::vector<std::string>::iterator name=names.begin(); name!=names.end(); ++name )
  { 
    // Skip already added lateral error bands
    if( HasLatErrorBand( *name ) ) 
      continue;
    unsigned int nunis = ref.GetLatErrorBand( *name )->GetNHists();
    if( ! this->AddLatErrorBandAndFillWithCV( *name, nunis ) ) 
      return false;
  }

  return true;

}

bool MUH1D::HasLatErrorBand( const std::string& name ) const
{
  // Check the MULatErrorBands
  if( fLatErrorBandMap.find( name ) != fLatErrorBandMap.end() )
    return true;

  return false;
}

bool MUH1D::HasVertErrorBand( const std::string& name ) const
{
  // Check the MUVertErrorBands
  if( fVertErrorBandMap.find( name ) != fVertErrorBandMap.end() )
    return true;

  return false;
}

bool MUH1D::HasUncorrError( const std::string& name ) const
{
  // Check the uncorr errors
  if( fUncorrErrorMap.find( name ) != fUncorrErrorMap.end() )
    return true;

  return false;
}


bool MUH1D::HasErrorBand( const std::string& name ) const
{
  // Check the MULatErrorBands
  if( HasLatErrorBand( name ) )
    return true;

  // Check the MUVertErrorBands
  if( HasVertErrorBand( name ) )
    return true;

  // Check the uncorrelated errors
  if( HasUncorrError( name ) )
    return true;

  return false;
}

bool MUH1D::HasErrorMatrix( const std::string& name ) const
{
  // Check the fSysErrorMatrix
  if( fSysErrorMatrix.find( name ) != fSysErrorMatrix.end() )
    return true;

  return false;
}

bool MUH1D::HasRemovedErrorMatrix( const std::string& name ) const
{
  // Check the fSysErrorMatrix
  if( fRemovedSysErrorMatrix.find( name ) != fRemovedSysErrorMatrix.end() )
    return true;

  return false;
}

MULatErrorBand* MUH1D::GetLatErrorBand( const std::string& name )
{
  std::map<std::string, MULatErrorBand*>::iterator i = fLatErrorBandMap.find( name );
  if( i == fLatErrorBandMap.end() )
  {
    Warning( "MUH1D::GetLatErrorBand", "There is no lateral error band with name \"%s\".  Returning NULL.", name.c_str());
    return NULL;
  }

  return i->second;
}

MUVertErrorBand* MUH1D::GetVertErrorBand( const std::string& name )
{
  std::map<std::string, MUVertErrorBand*>::iterator i = fVertErrorBandMap.find( name );
  if( i == fVertErrorBandMap.end() )
  {
    Warning( "MUH1D::GetVertErrorBand", "There is no vertical error band with name \"%s\".  Returning NULL.", name.c_str());
    return NULL;
  }

  return i->second;
}


const MULatErrorBand* MUH1D::GetLatErrorBand( const std::string& name ) const
{
  std::map<std::string, MULatErrorBand*>::const_iterator i = fLatErrorBandMap.find( name );
  if( i == fLatErrorBandMap.end() )
  {
    Warning( "MUH1D::GetLatErrorBand", "There is no lateral error band with name \"%s\".  Returning NULL.", name.c_str());
    return NULL;
  }

  return i->second;
}

const MUVertErrorBand* MUH1D::GetVertErrorBand( const std::string& name ) const
{
  std::map<std::string, MUVertErrorBand*>::const_iterator i = fVertErrorBandMap.find( name );
  if( i == fVertErrorBandMap.end() )
  {
    Warning( "MUH1D::GetVertErrorBand", "There is no vertical error band with name \"%s\".  Returning NULL.", name.c_str());
    return NULL;
  }

  return i->second;
}

TH1D* MUH1D::GetUncorrError( const std::string& name )
{
  std::map<std::string, TH1D*>::const_iterator i = fUncorrErrorMap.find( name );
  if( i == fUncorrErrorMap.end() )
  {
    Warning( "MUH1D::GetUncorrError", "There is no uncorrelated error with name \"%s\".  Returning NULL.", name.c_str());
    return NULL;
  }

  return i->second;
}

const TH1D* MUH1D::GetUncorrError( const std::string& name ) const
{
  std::map<std::string, TH1D*>::const_iterator i = fUncorrErrorMap.find( name );
  if( i == fUncorrErrorMap.end() )
  {
    Warning( "MUH1D::GetUncorrError", "There is no uncorrelated error with name \"%s\".  Returning NULL.", name.c_str());
    return NULL;
  }

  return i->second;
}

TH1D MUH1D::GetUncorrErrorAsHist( const std::string& name, bool asFrac ) const
{
  //copy self as a blank TH1D
  TH1D rval( *dynamic_cast<const TH1D*>(this) );
  rval.Reset();

  //if the uncorrelated error exists copy the error from its bins to the content of rval
  const TH1D *uncorr = GetUncorrError(name);
  if(uncorr)
  {
    int lowBin = 0;
    int highBin = uncorr->GetNbinsX()+1;
    for( int i = lowBin; i <= highBin; ++i )
    {
      const double err = uncorr->GetBinError(i);

      if(asFrac)
      {
        //not sure what to do if err!=0 but content=0
        //use 0 for now
        const double content = this->GetBinContent(i);
        const double fracErr = ( 0.==content ) ? 0. : fabs(err/content); 
        rval.SetBinContent(i, fracErr );
      }
      else
        rval.SetBinContent( i, err );
    }
  }

  return rval;
}

//==================================
// Transfer error bands
//==================================
bool MUH1D::TransferErrorBands( MUH1D *hist, bool removeFromMe )
{
  bool allOK = true;

  std::vector<std::string> names = GetVertErrorBandNames();
  for( std::vector<std::string>::iterator i = names.begin(); i != names.end(); ++i )
    allOK = TransferVertErrorBand( hist, *i, removeFromMe ) && allOK;

  names = GetLatErrorBandNames();
  for( std::vector<std::string>::iterator i = names.begin(); i != names.end(); ++i )
    allOK = TransferLatErrorBand( hist, *i, removeFromMe ) && allOK;

  return allOK;
}

bool MUH1D::TransferVertErrorBand( MUH1D *hist, const std::string& errName, bool removeFromMe )
{
  MUVertErrorBand *errBand(0);
  if( removeFromMe )
    errBand = PopVertErrorBand( errName );
  else
  {
    MUVertErrorBand *myErrBand = GetVertErrorBand( errName );
    if( 0 != myErrBand )
    {
      //change to this's directory so children are in the same place
      const TString oldDir = gDirectory->GetPath();
      this->GetDirectory()->cd();

      errBand = new MUVertErrorBand( *myErrBand );

      gDirectory->cd(oldDir);
    }
  }

  if( 0 == errBand )
  {
    Error( "MUH1D::TransferVertErrorBand", "Could not find error band %s to transfer.", errName.c_str());
    return false;
  }

  //deep scale the error band by ratio CV histos
  TH1D ratio( *( dynamic_cast<TH1D*>(this) ) );
  ratio.Divide( hist, this );
  errBand->MultiplySingle( errBand, &ratio);

  bool pushOK = hist->PushErrorBand( errName, errBand );

  return pushOK;
}

bool MUH1D::TransferLatErrorBand( MUH1D *hist, const std::string& errName, bool removeFromMe )
{
  MULatErrorBand *errBand(0);
  if( removeFromMe )
    errBand = PopLatErrorBand( errName );
  else
  {
    MULatErrorBand *myErrBand = GetLatErrorBand( errName );
    if( 0 != myErrBand )
    {
      //change to this's directory so children are in the same place
      const TString oldDir = gDirectory->GetPath();
      this->GetDirectory()->cd();

      errBand = new MULatErrorBand( *myErrBand );

      gDirectory->cd(oldDir);
    }
  }

  if( 0 == errBand )
  {
    Error( "MUH1D::TransferLatErrorBand", "Could not find error band %s to transfer.", errName.c_str());
    return false;
  }

  //deep scale the error band by ratio CV histos
  TH1D ratio( *( dynamic_cast<TH1D*>(this) ) );
  ratio.Divide( hist, this );
  errBand->MultiplySingle( errBand, &ratio);


  bool pushOK = hist->PushErrorBand( errName, errBand );

  return pushOK;
}



MULatErrorBand* MUH1D::PopLatErrorBand( const std::string& name )
{
  std::map<std::string, MULatErrorBand*>::iterator i = fLatErrorBandMap.find( name );
  if( i == fLatErrorBandMap.end() )
  {
    Warning( "MUH1D::PopLatErrorBand", "There is no lateral error band with name \"%s\".  Returning NULL.", name.c_str());
    return NULL;
  }

  //get a pointer to the error band and remove it from the MUH1D's vector
  MULatErrorBand* rval = i->second;
  fLatErrorBandMap.erase(i);

  return rval;
}



MUVertErrorBand* MUH1D::PopVertErrorBand( const std::string& name )
{
  std::map<std::string, MUVertErrorBand*>::iterator i = fVertErrorBandMap.find( name );
  if( i == fVertErrorBandMap.end() )
  {
    Warning( "MUH1D::PopVertErrorBand", "There is no vertical error band with name \"%s\".  Returning NULL.", name.c_str());
    return NULL;
  }

  //get a pointer to the error band and remove it from the MUH1D's vector
  MUVertErrorBand* rval = i->second;
  fVertErrorBandMap.erase(i);

  return rval;
}

TH1D* MUH1D::PopUncorrError( const std::string& name )
{ 
  std::map<std::string, TH1D*>::iterator i = fUncorrErrorMap.find( name );
  if( i == fUncorrErrorMap.end() )
  {
    Warning( "MUH1D::PopUncorrError", "There is no uncorrelated error with name \"%s\".  Returning NULL.", name.c_str());
    return NULL;
  }

  //get a pointer to the uncorr error and remove it from the MUH1D's vector
  TH1D* rval = i->second;
  fUncorrErrorMap.erase(i);

  return rval;
} 




bool MUH1D::PushErrorBand( const std::string& name, MUVertErrorBand* err )
{
  if( HasVertErrorBand(name) )
  {
    Warning( "MUH1D::PushErrorBand", "I already had vert error band %s.  I'm deleting it and adding the new one.", name.c_str());
    delete PopVertErrorBand(name);
  }
  fVertErrorBandMap[name] = err;
  return true;
}

bool MUH1D::PushErrorBand( const std::string& name, MULatErrorBand* err )
{ 
  if( HasLatErrorBand(name) )
  {
    Warning( "MUH1D::PushErrorBand", "I already had lat error band %s.  I'm deleting it and adding the new one.", name.c_str());
    delete PopLatErrorBand(name);
  }
  fLatErrorBandMap[name] = err;
  return true;
}

bool MUH1D::PushUncorrError( const std::string& name, TH1D* err )
{ 
  if( HasUncorrError(name) )
  {
    Warning( "MUH1D::PushUncorrError", "I already had uncorrelated error %s.  I'm deleting it and adding the new one.", name.c_str());
    delete PopUncorrError(name);
  }
  //make sure it has sumw2 so div/mult/etc work correctly even though the errors probably arent sqrt(w*w)
  if( 0 == err->GetSumw2N() )
    err->Sumw2();

  fUncorrErrorMap[name] = err;
  return true;
}


std::vector<std::string> MUH1D::GetErrorBandNames() const
{
  std::vector<std::string> rval;
  for( std::map<std::string, MUVertErrorBand*>::const_iterator i = fVertErrorBandMap.begin(); i != fVertErrorBandMap.end(); ++i )
    rval.push_back( i->first );
  for( std::map<std::string, MULatErrorBand*>::const_iterator i = fLatErrorBandMap.begin(); i != fLatErrorBandMap.end(); ++i )
    rval.push_back( i->first );
  return rval;
}

std::vector<std::string> MUH1D::GetVertErrorBandNames() const
{
  std::vector<std::string> rval;
  for( std::map<std::string, MUVertErrorBand*>::const_iterator i = fVertErrorBandMap.begin(); i != fVertErrorBandMap.end(); ++i )
    rval.push_back( i->first );
  return rval;
}

std::vector<std::string> MUH1D::GetLatErrorBandNames() const
{
  std::vector<std::string> rval;
  for( std::map<std::string, MULatErrorBand*>::const_iterator i = fLatErrorBandMap.begin(); i != fLatErrorBandMap.end(); ++i )
    rval.push_back( i->first );
  return rval;
}

std::vector<std::string> MUH1D::GetUncorrErrorNames() const
{
  std::vector<std::string> rval;
  for( std::map<std::string, TH1D*>::const_iterator i = fUncorrErrorMap.begin(); i != fUncorrErrorMap.end(); ++i )
    rval.push_back( i->first );
  return rval;
}


std::vector<std::string> MUH1D::GetSysErrorMatricesNames() const
{

  std::vector<std::string> rval;
  //Vertical Errors
  for( std::map<std::string, MUVertErrorBand*>::const_iterator i = fVertErrorBandMap.begin(); i != fVertErrorBandMap.end(); ++i )
    rval.push_back( i->first );

  //Lateral Errors
  for( std::map<std::string, MULatErrorBand*>::const_iterator i = fLatErrorBandMap.begin(); i != fLatErrorBandMap.end(); ++i )
    rval.push_back( i->first );

  //Uncorrelated Errors
  for( std::map<std::string, TH1D*>::const_iterator i = fUncorrErrorMap.begin(); i != fUncorrErrorMap.end(); ++i )
    rval.push_back( i->first );

  //Special Errors
  for( std::map<std::string, TMatrixD*>::const_iterator i = fSysErrorMatrix.begin(); i != fSysErrorMatrix.end(); ++i )
  {
    if ( !HasEnding(i->first, "_asShape") )
      rval.push_back( i->first );
  }

  return rval;

}

std::vector<std::string> MUH1D::GetRemovedSysErrorMatricesNames() const
{
  std::vector<std::string> rval;
  for( std::map<std::string, TMatrixD*>::const_iterator i = fRemovedSysErrorMatrix.begin(); i != fRemovedSysErrorMatrix.end(); ++i )
  {
    if ( !HasEnding(i->first, "_asShape") )
      rval.push_back( i->first );
  }

  return rval;
}

bool MUH1D::FillLatErrorBand( const std::string& name, const double val, const std::vector<double>& shifts, const double cvweight /* = 1.0 */, const bool fillcv /* = true */, const double *weights /* = 0 */ )
{
  return FillLatErrorBand( name, val, &(shifts[0]), cvweight, fillcv, weights );
}

bool MUH1D::FillLatErrorBand( const std::string& name, const double val, const double * shifts, const double cvweight /* = 1.0 */, const bool fillcv /* = true */, const double * weights /* = 0 */ )
{
  // Try to fill a lateral error band
  MULatErrorBand *lat = GetLatErrorBand( name );
  if( lat )
    return lat->Fill( val, shifts, cvweight, fillcv, weights );

  Warning( "MUH1D::FillLatErrorBand", "Could not find a lateral error band to fill with name = %s", name.c_str());
  return false;
}

bool MUH1D::FillLatErrorBand( const std::string& name, const double val, const double shiftDown, const double shiftUp, const double cvweight  /*= 1.0*/, const bool fillcv /* = true */ )
{
  // Try to fill a lateral error band
  MULatErrorBand *lat = GetLatErrorBand( name );
  if( lat )
    return lat->Fill( val, shiftDown, shiftUp, cvweight, fillcv );

  Warning( "MUH1D::FillLatErrorBand", "Could not find a lateral error band to fill with name = %s", name.c_str());
  return false;
}


bool MUH1D::FillVertErrorBand( const std::string& name, const double val, const std::vector<double>& weights, const double cvweight /* = 1.0 */, double cvWeightFromMe /*= 1.*/)
{
  return FillVertErrorBand( name, val, &(weights[0]), cvweight, cvWeightFromMe );
}

bool MUH1D::FillVertErrorBand( const std::string& name, const double val, const double * weights, const double cvweight /* = 1.0 */, double cvWeightFromMe /*= 1.*/)
{
  // Try to fill a vertical error band
  MUVertErrorBand* vert = GetVertErrorBand( name );
  if( vert )
    return vert->Fill( val, weights, cvweight, cvWeightFromMe );

  Warning( "MUH1D::FillVertErrorBand", "Could not find a vertical error band to fill with name = %s", name.c_str());
  return false;
}

bool MUH1D::FillVertErrorBand( const std::string& name, const double val, const double weightDown, const double weightUp, const double cvweight  /*= 1.0*/, double cvWeightFromMe /*= 1.*/ )
{
  // Try to fill a vertical error band
  MUVertErrorBand *vert = GetVertErrorBand( name );
  if( vert )
    return vert->Fill( val, weightDown, weightUp, cvweight, cvWeightFromMe );

  Warning( "MUH1D::FillVertErrorBand", "Could not find a vertical error band to fill with name = %s", name.c_str());
  return false;
}


bool MUH1D::FillUncorrError( const std::string& name, const double val, const double err, const double cvweight /*= 1.0*/ )
{
  TH1D *hist = GetUncorrError(name);
  if( !hist )
  {
    Warning("MUH1D::FillUncorrError", "Could not find an uncorrelated error to fill with name = %s", name.c_str());
    return false;
  }

  //find the bin
  int bin = hist->FindBin(val);
  //add to bin content, which doesn't change error
  hist->AddBinContent( bin, cvweight );
  //add to bin error so that binErr/binContent = avg( err/cvweight ) always
  hist->SetBinError( bin, err + hist->GetBinError(bin) );

  //may have to copy global stats from CV
  //it can matter when averaging in some cases
  //not sure how long it will take
  double stats[4] = {0.};
  this->GetStats(stats);
  hist->PutStats(stats);

  return true;
}


void MUH1D::SetUseSpreadErrorAll( bool use )
{
  for( std::map<std::string, MUVertErrorBand*>::iterator i = fVertErrorBandMap.begin(); i != fVertErrorBandMap.end(); ++i )
    i->second->SetUseSpreadError( use );

  for( std::map<std::string, MULatErrorBand*>::iterator i = fLatErrorBandMap.begin(); i != fLatErrorBandMap.end(); ++i )
    i->second->SetUseSpreadError( use );
}


TH1D MUH1D::GetCVHistoWithStatError() const
{
  //! @todo I think GetCVHistoWithStatError is unnecessary because it is identical to the TH1D part of self.
  // Get the stat. error band
  TH1D err = GetStatError( false );

  // Create a copy of this histogram and rename it
  TH1D rval( *this );
  std::string tmpName( std::string( GetName() ) + "_CV_WithStatErr" );
  rval.SetName( tmpName.c_str() );

  for( int iBin = 0; iBin <= GetNbinsX(); ++iBin )
    rval.SetBinError( iBin, err.GetBinContent(iBin) );

  return rval;
}

TH1D MUH1D::GetCVHistoWithError( bool includeStat /* = true */ , bool cov_area_normalize /* = false */) const
{
  // Get the error band
  TH1D err = GetTotalError( includeStat , false, cov_area_normalize);

  // Create a copy of this histogram and rename it
  TH1D rval( *this );
  std::string tmpName( std::string( GetName() ) + "_CV_WithErr" );
  rval.SetName( tmpName.c_str() );

  for( int iBin = 0; iBin <= GetNbinsX(); ++iBin )
    rval.SetBinError( iBin, err.GetBinContent(iBin) );

  return rval;
}

TH1D MUH1D::GetTotalError(
    bool includeStat /* = true */, 
    bool asFrac /* = false */ ,
    bool cov_area_normalize /*= false */) const
{
  // Make a copy of this histogram as a TH1D and rename it
  TH1D err( *this );
  err.Reset();
  std::string tmpName( std::string(GetName()) + "_TotalError");
  err.SetName( tmpName.c_str() );

  const int highBin = GetNbinsX() + 1;
  const int lowBin = 0;

  // Get the Total Error Matrix
  TMatrixD errMatrix = GetTotalErrorMatrix(includeStat, asFrac, cov_area_normalize);

  for( int iBin = lowBin; iBin <= highBin; ++iBin )
  {
    double derr = errMatrix[iBin][iBin];
    err.SetBinContent( iBin, ( derr > 0 ) ? sqrt(derr): 0. );
  }

  //if you manually set the MUH1D min/max it exists on err.
  //use default min/max for err
  err.SetMinimum();
  err.SetMaximum();

  return err;
}

//------------------------------------------------------------------------
// Get the Total Covariance Matrix
//------------------------------------------------------------------------
TMatrixD MUH1D::GetTotalErrorMatrix(
    bool includeStat /*= true*/, 
    bool asFrac /*= false*/, 
    bool cov_area_normalize /*= false*/ ) const
{
  const int highBin = GetNbinsX() + 1;
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

TMatrixD MUH1D::GetTotalCorrelationMatrix( bool cov_area_normalize /*= false*/ ) const
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

Int_t MUH1D::WriteTotalErrorMatrix(const std::string& name, bool includeStat /*= true*/, bool asFrac /*= false*/, bool cov_area_normalize /*= false*/)
{
  TMatrixD covmx = GetTotalErrorMatrix(includeStat, asFrac, cov_area_normalize);
  Int_t state = covmx.Write(name.c_str());
  return state;
}

Int_t MUH1D::WriteTotalCorrelationMatrix(const std::string& name, bool cov_area_normalize /*= false*/)
{
  TMatrixD corrmx = GetTotalCorrelationMatrix(cov_area_normalize);
  Int_t state = corrmx.Write(name.c_str());
  return state;
}

TMatrixD MUH1D::GetSysCorrelationMatrix(
    const std::string& name,
    bool cov_area_normalize /*= false*/ ) const
{
  TMatrixD covmx = GetSysErrorMatrix(name, false, cov_area_normalize);
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

//--------------------------------------------------------------------------------
// Push a Covariance Matrix to the Systematic/Statistical fSys/StatErrorMatrix map
//--------------------------------------------------------------------------------
bool MUH1D::PushCovMatrix(
    const std::string& name,
    TMatrixD covmx, 
    bool cov_area_normalize /*=false*/)
{

  if ( HasEnding(name,"_asShape") )
  {   
    std::cout << "Warning [MUH1D::pushCovMatrix] : You cannot push a Covariance matrix with prefix: \"_asShape\", at the end. Doing Nothing."<< std::endl;
    std::cout << "\nTo Add an \"only shape\" error Matrix, set the last boolean in this constructor to true:\n        PushCovMatrix(\"errorMatrix_name\", TMatrixD errorMatrix, true).\n  "<< std::endl;

    return false;
  }

  const std::string name_condition = (cov_area_normalize)? "_asShape" : "";
  const std::string fname = name + name_condition ;

  if ( HasRemovedErrorMatrix( fname ) )
  {
    std::cout << "Warning [MUH1D::pushCovMatrix] : The systematic you are trying to push is on the Removed Systematics Map List, if you want to re-add this systematic. \nPlease use the function: \nMUH1D::UnRemoveSysErrorMatrix( std::string name ) "<< std::endl;
    return false;
  }

  const int highBin = GetNbinsX() + 1;

  if (covmx.GetNrows()!=highBin+1 || covmx.GetNcols()!=highBin+1 )
  {
    std::cout << "Warning [MUH1D::pushCovMatrix] : The pushed covariance matrix dimensions are incorrect (it should be a " << highBin+1 << "x" << highBin+1 << " matrix). Doing nothing." <<std::endl;
    return false;
  }

  // Make sure there are no Covariance Matrices with this name already
  if( HasErrorMatrix( fname ) )
  {
    std::cout << "Warning [MUH1D::PushCovMatrix] : There is a Matrix with name \"" << fname << "\" already.  Doing nothing." << std::endl;
    return false;
  }
  else if ( HasErrorBand( name ) )
  {
    std::cout << "Warning [MUH1D::PushCovMatrix] : " << name << " is already in either a Vertical or Lateral Error" << std::endl;
    return false;
  }

  // Adding matrix to fSysErrorMatrix
  //! @todo can't we just use the copy constructor when creating a new TMatrixD?
  TMatrixD* temp = new TMatrixD(covmx.GetNrows(), covmx.GetNcols() );
  *temp = covmx;
  fSysErrorMatrix[fname] = temp;

  return true;
}

//------------------------------------------------------------------------
// Get a Specific Covariance Matrix from the map
//------------------------------------------------------------------------
TMatrixD MUH1D::GetSysErrorMatrix(const std::string& name, bool asFrac /*= false*/, bool cov_area_normalize /*= false*/) const
{
  if (HasEnding(name,"_asShape") )
    std::cout << "Warning [MUH1D::GetSysErrorMatrix]: You are calling the error Matrix: " << name <<".\nAssuming the error Band wanted is: " << name.substr(0,name.length()-8) << " with cov_area_normalize = true" << std::endl;

  const std::string name_condition = ( (cov_area_normalize) && !(HasEnding(name,"_asShape")) )?  "_asShape" : "";
  const std::string fname = name + name_condition ; 
  const std::string errName = HasEnding(fname,"_asShape") ? fname.substr(0,fname.length()-8) : fname;

  const int highBin = GetNbinsX() + 1;
  const int lowBin = 0;
  TMatrixD covmx(highBin+1,highBin+1);

  if ( HasErrorMatrix( fname ) )
    covmx = *(fSysErrorMatrix.find(fname)->second);
  else if( fLatErrorBandMap.find( errName ) != fLatErrorBandMap.end() )
  {
    std::map<std::string, MULatErrorBand*>::const_iterator it = fLatErrorBandMap.find( errName );
    covmx = it->second->CalcCovMx( ( HasEnding(fname,"_asShape") ) );
  }
  else if( fVertErrorBandMap.find( errName ) != fVertErrorBandMap.end() )
  {
    std::map<std::string, MUVertErrorBand*>::const_iterator it = fVertErrorBandMap.find( errName );
    covmx = it->second->CalcCovMx( ( HasEnding(fname,"_asShape") ) );
  }
  else if( fUncorrErrorMap.find( errName ) != fUncorrErrorMap.end() )
  {
    std::map<std::string, TH1D*>::const_iterator it = fUncorrErrorMap.find( errName );
    covmx = MUHist::GetErrorsAsMatrix( it->second );
  }
  else
    std::cout << "Warning [MUH1D::GetSysErrorMatrix]: There is no Covariance Matrix with name " << fname << ".Returning and empty Matrix." << std::endl;

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

bool MUH1D::RemoveSysErrorMatrix(const std::string& name)
{
  const std::string shapeName = name + "_asShape";
  if ( !HasErrorMatrix( name) && !HasErrorMatrix( shapeName ) )
  {
    std::cout << "Warning [MUH1D::RemoveSysErrorMatrix] : The Systematic \"" << name << "\" was not found on the systematic map in neither absolute nor shape component.\nNotice Vertical/Lateral Errors cannot be removed. Doing nothing." << std::endl;
    return false;
  }

  if ( HasErrorMatrix( name ) )
  {  
    // Removing systematic from fSysErrorMatrix
    // and Adding it to fRemovedSysErrorMatrix
    fRemovedSysErrorMatrix[name] = fSysErrorMatrix[name];
    fSysErrorMatrix.erase( fSysErrorMatrix.find( name ) );
  }

  if ( HasErrorMatrix(shapeName) )
  {
    // Doing the same for shape component matrix (if any)
    fRemovedSysErrorMatrix[shapeName] = fSysErrorMatrix[shapeName];
    fSysErrorMatrix.erase( fSysErrorMatrix.find( shapeName ) );
  }

  return true;
}

bool MUH1D::UnRemoveSysErrorMatrix(const std::string& name)
{
  // Moving Removed Systematics (Absolute and Shape) from
  // fRemovedSysErrorMatrix back to fSysErrorMatrix

  const std::string shapeName = name + "_asShape";
  if ( !HasRemovedErrorMatrix(name) && !HasRemovedErrorMatrix(shapeName) )
  {
    std::cout << "Warning [MUH1D::UnRemoveSysErrorMatrix] : The Systematic \"" << name << "\" was not found on the map of removed systematics neither in absolute nor shape component. Doing nothing." << std::endl;
    return false;
  }

  if( HasRemovedErrorMatrix( name ) )
  {
    fSysErrorMatrix[name] = fRemovedSysErrorMatrix[name];
    fRemovedSysErrorMatrix.erase( fRemovedSysErrorMatrix.find( name ) );
  }

  if( HasRemovedErrorMatrix( shapeName ) )
  {
    fSysErrorMatrix[name] = fRemovedSysErrorMatrix[shapeName];
    fRemovedSysErrorMatrix.erase( fRemovedSysErrorMatrix.find( shapeName ) );
  }
  return true;
}

void MUH1D::ClearSysErrorMatrices()
{
  // Loop over all SysErrorMatrices and delete them, even the RemovedSysErrorMatrices.
  for( std::map<std::string, TMatrixD*>::const_iterator i = fSysErrorMatrix.begin(); i != fSysErrorMatrix.end(); ++i )
  {
    if( 0 != i->second )
      delete i->second;
  }
  fSysErrorMatrix.clear();

  for( std::map<std::string, TMatrixD*>::const_iterator i = fRemovedSysErrorMatrix.begin(); i != fRemovedSysErrorMatrix.end(); ++i )
  {
    if( 0 != i->second )
      delete i->second;
  }
  fRemovedSysErrorMatrix.clear();
}

TMatrixD MUH1D::GetStatErrorMatrix( bool asFrac /* =false */ ) const
{
  const int highBin = GetNbinsX() + 1;
  const int lowBin = 0;
  TMatrixD covmx(highBin+1,highBin+1);

  // stat error
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
// Get a histogram of the statistical errors only.
//------------------------------------------------------------------------
TH1D MUH1D::GetStatError( bool asFrac /* = false */ ) const
{
  // Make a copy of this histogram as a TH1D and rename it
  TH1D err( *this );
  err.Reset();
  std::string tmpName( std::string(GetName()) + "_StatError");
  err.SetName( tmpName.c_str() );

  const int highBin = GetNbinsX() + 1;
  const int lowBin = 0;

  // stat error
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

  //if you manually set the MUH1D min/max it exists on err.
  //use default min/max for err
  err.SetMinimum();
  err.SetMaximum();

  return err;
}

//------------------------------------------------------------------------
// Get an MUH1D which has its bin content and errors normalized to bin width so it looks smooth
//------------------------------------------------------------------------
MUH1D MUH1D::GetBinNormalizedCopy( Double_t normBinWidth /* = fNormBinWidth */ ) const
{
  // If normBinWidth is negative, use the default bin width for this MUH1D
  if( normBinWidth <= 0 )
    normBinWidth = fNormBinWidth;

  MUH1D rval(*this);

  // If the default bin width for this MUH1D is negative and the user wanted to use that default, just return self (already normalized or normalization not appropriate).
  // Otherwise, normalize to the this bin width
  if( normBinWidth > 0 )
    rval.Scale( normBinWidth, "width" );

  return rval;
}


Double_t MUH1D::GetAreaNormFactor(const MUH1D * h_data) const
{
  //considering overflow
  const int highBin_data = h_data->GetNbinsX()+1;
  const int highBin_mc   = this->GetNbinsX()+1;
  //considering underflow
  const int lowBin = 0;
  Double_t area_scale = 1.;

  if( ! CheckConsistency( this, h_data ) )
    Warning("GetAreaNormFactor", "Data and MC axes do not match" );

  if (this->Integral( lowBin, highBin_mc )==0 ){
    Warning("GetAreaNormFactor","MC Area Histogram is zero. No Scale Factor calculated");
  }
  else {
    area_scale = h_data->Integral( lowBin, highBin_data ) / this->Integral( lowBin, highBin_mc ) ;
  }
  return area_scale;

}

//=======================================================================
// DrawBinNormalized
//=======================================================================
MUH1D* MUH1D::DrawBinNormalized( Option_t* option /* = ""*/, Double_t normBinWidth /* = -1 */ ) const
{
  // Create a (probably) unique name for the clone to avoid potential memory leaks
  static int nNormDraws = 0;
  const char *cloneName = Form( "%s_normClone_%d", GetName(), ++nNormDraws );

  // Create the clone from the bin normalized version of this MUH1D.
  // Clone is a new object, which is necessary so the drawn histogram stays drawn after the function ends.
  MUH1D *clone = (MUH1D*)this->GetBinNormalizedCopy( normBinWidth ).Clone( cloneName );

  // Call draw with the clone
  clone->Draw( option );

  // Return the clone so that the caller can delete the object when it is no longer needed
  return clone;
}


//======================================================================
// Replacements of ROOT versions of functions
//======================================================================

void MUH1D::Scale( Double_t c1 /*= 1.*/, Option_t* option /*=""*/ )
{
  // Scale yourself using TH1D::Scale
  this->TH1D::Scale( c1, option );

  // Scale the lateral error bands
  for( std::map<std::string, MULatErrorBand*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
    it->second->Scale( c1, option );

  // Scale the vertical error bands
  for( std::map<std::string, MUVertErrorBand*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
    it->second->Scale( c1, option );

  // Scale the uncorr errors
  for( std::map<std::string, TH1D*>::iterator it = fUncorrErrorMap.begin(); it != fUncorrErrorMap.end(); ++it )
    it->second->Scale( c1, option );


  // Scale special covariance matrices including the removed ones
  for( std::map<std::string, TMatrixD*>::const_iterator it = fSysErrorMatrix.begin(); it != fSysErrorMatrix.end(); ++it )
  {
    if( 0 != it->second )
      (*it->second) *= pow(c1, 2.);
  }

  for( std::map<std::string, TMatrixD*>::const_iterator it = fRemovedSysErrorMatrix.begin(); it != fRemovedSysErrorMatrix.end(); ++it )
  {
    if( 0 != it->second )
      (*it->second) *= pow(c1, 2.);
  }

}



Bool_t MUH1D::Divide( const MUH1D* h1, const MUH1D* h2, Double_t c1 /*= 1*/, Double_t c2 /*= 1*/, Option_t* option /*=""*/ )
{
  Bool_t success = kFALSE;
  // Call the TH1D Divide
  success = this->TH1D::Divide( (TH1D*)h1, (TH1D*)h2, c1, c2, option );

  // Call Divide for all lateral error bands
  for( std::map<std::string, MULatErrorBand*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
  {
    const MULatErrorBand* err1 = h1->GetLatErrorBand( it->first );
    const MULatErrorBand* err2 = h2->GetLatErrorBand( it->first );
    if( !err1 || !err2 )
    {
      Error("Divide", "Could not divide MUH1Ds because they all don't have the %s MULatErrorBand", it->first.c_str());
      return kFALSE;
    }
    success = it->second->Divide( err1, err2, c1, c2, option );
  }

  // Divide the vertical error bands
  for( std::map<std::string, MUVertErrorBand*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
  {
    const MUVertErrorBand* err1 = h1->GetVertErrorBand( it->first );
    const MUVertErrorBand* err2 = h2->GetVertErrorBand( it->first );
    if( !err1 || !err2 )
    {
      Error("Divide", "Could not divide MUH1Ds because they all don't have the %s MUVertErrorBand", it->first.c_str());
      return kFALSE;
    }
    success = it->second->Divide( err1, err2, c1, c2, option );
  }

  // Divide the uncorr errors
  for( std::map<std::string, TH1D*>::iterator it = fUncorrErrorMap.begin(); it != fUncorrErrorMap.end(); ++it )
  {
    const TH1D* err1 = h1->GetUncorrError( it->first );
    const TH1D* err2 = h2->GetUncorrError( it->first );
    if( !err1 || !err2 )
    {
      Error("Divide", "Could not divide MUH1Ds because they all don't have the %s uncorrelated error", it->first.c_str());
      return kFALSE;
    }
    success = it->second->Divide( err1, err2, c1, c2, option );
  }


  // Do we have special errors in the systemics?
  if ( !( fSysErrorMatrix.empty() ) || !( fRemovedSysErrorMatrix.empty() ) )
  {
    Warning("MUH1D::Divide", "Customized error matrices were found (errors that come from neither Vertical nor Lateral Error Bands). They will be cleared.");
    ClearSysErrorMatrices( );
  }  

  return success;
}


//=====================================================================
// Divide the the numerator and its error bands by the same denominator
//=====================================================================
void MUH1D::DivideSingle( const MUH1D* h1, const TH1* h2, Double_t c1 /*= 1*/, Double_t c2 /*= 1*/, Option_t* option /*=""*/ )
{
  // Call the TH1D Divide
  this->TH1D::Divide( (TH1D*)h1, h2, c1, c2, option );

  // Call Divide for all lateral error bands
  for( std::map<std::string, MULatErrorBand*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
  {
    const MULatErrorBand* err1 = h1->GetLatErrorBand( it->first );
    if( !err1  )
    {
      Error("Divide", "Could not divide MUH1Ds because they all don't have the %s MULatErrorBand", it->first.c_str());
      return;
    }
    it->second->DivideSingle( err1, h2, c1, c2, option );
  }

  // Divide the vertical error bands
  for( std::map<std::string, MUVertErrorBand*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
  {
    const MUVertErrorBand* err1 = h1->GetVertErrorBand( it->first );
    if( !err1 )
    {
      Error("Divide", "Could not divide MUH1Ds because they all don't have the %s MUVertErrorBand", it->first.c_str());
      return;
    }
    it->second->DivideSingle( err1, h2, c1, c2, option );
  }

  // Divide the uncorr errors
  for( std::map<std::string, TH1D*>::iterator it = fUncorrErrorMap.begin(); it != fUncorrErrorMap.end(); ++it )
  {
    const TH1D* err1 = h1->GetUncorrError( it->first );
    if( !err1 )
    {
      Error("Divide", "Could not divide MUH1Ds because they all don't have the %s uncorrelated error", it->first.c_str());
      return;
    }
    it->second->Divide( err1, h2, c1, c2, option );
  }


  // Do we have special errors in the systemics?
  if ( !( fSysErrorMatrix.empty() ) || !( fRemovedSysErrorMatrix.empty() ) )
  {
    Warning( "MUH1D::DivideSingle", "Customized error matrices were found (errors that come from neither Vertical nor Lateral Error Bands) . They will be cleared.");
    ClearSysErrorMatrices( );
  }

  return;
}


Bool_t MUH1D::Multiply( const MUH1D* h1, const MUH1D* h2, Double_t c1 /*= 1*/, Double_t c2 /*= 1*/ )
{
  Bool_t success = kFALSE;
  // Call the TH1D Multiply 
  success = this->TH1D::Multiply( (TH1D*)h1, (TH1D*)h2, c1, c2 );

  // Call Multiply for all lateral error bands
  for( std::map<std::string, MULatErrorBand*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
  {
    const MULatErrorBand* err1 = h1->GetLatErrorBand( it->first );
    const MULatErrorBand* err2 = h2->GetLatErrorBand( it->first );
    if( !err1 || !err2 )
    {
      Error("Multiply", "Could not multiply MUH1Ds because they all don't have the %s MULatErrorBand", it->first.c_str());
      return kFALSE;
    }
    success = it->second->Multiply( err1, err2, c1, c2 );
  }

  // multiply the vertical error bands
  for( std::map<std::string, MUVertErrorBand*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
  {
    const MUVertErrorBand* err1 = h1->GetVertErrorBand( it->first );
    const MUVertErrorBand* err2 = h2->GetVertErrorBand( it->first );
    if( !err1 || !err2 )
    {
      Error("Multiply", "Could not multiply MUH1Ds because they all don't have the %s MUVertErrorBand", it->first.c_str());
      return kFALSE;
    }
    success = it->second->Multiply( err1, err2, c1, c2 );
  }

  // multiply the uncorr errors
  for( std::map<std::string, TH1D*>::iterator it = fUncorrErrorMap.begin(); it != fUncorrErrorMap.end(); ++it )
  {
    const TH1D* err1 = h1->GetUncorrError( it->first );
    const TH1D* err2 = h2->GetUncorrError( it->first );
    if( !err1 || !err2 )
    {
      Error("Multiply", "Could not multiply MUH1Ds because they all don't have the %s uncorrelated error", it->first.c_str());
      return kFALSE;
    }
    success = it->second->Multiply( err1, err2, c1, c2 );
  }


  // Do we have special errors in the systemics?
  if ( !( fSysErrorMatrix.empty() ) || !( fRemovedSysErrorMatrix.empty() ) )
  {
    Warning("MUH1D::Multiply", "Customized error matrices were found (errors that come from neither Vertical nor Lateral Error Bands) . They will be cleared.");
    ClearSysErrorMatrices( );
  }  

  return success;
}



//=====================================================================
// Multiply the the hist and its error bands by the same denominator
//=====================================================================
void MUH1D::MultiplySingle( const MUH1D* h1, const TH1* h2, const Double_t c1 /*= 1*/, const Double_t c2 /*= 1*/ )
{
  // Call the TH1D Multiply
  this->TH1D::Multiply( (TH1D*)h1, h2, c1, c2 );

  // Call Multiply for all lateral error bands
  for( std::map<std::string, MULatErrorBand*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
  {
    const MULatErrorBand* err1 = h1->GetLatErrorBand( it->first );
    if( !err1  )
    {
      Error("MultiplySingle", "Could not multiply MUH1Ds because they all don't have the %s MULatErrorBand", it->first.c_str());
      return;
    }
    it->second->MultiplySingle( err1, h2, c1, c2 );
  }

  // Call Multiply for all vertical error bands
  for( std::map<std::string, MUVertErrorBand*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
  {
    const MUVertErrorBand* err1 = h1->GetVertErrorBand( it->first );
    if( !err1 )
    {
      Error("MultiplySingle", "Could not multiply MUH1Ds because they all don't have the %s MUVertErrorBand", it->first.c_str());
      return;
    }
    it->second->MultiplySingle( err1, h2, c1, c2 );
  }

  // multiply the uncorr errors
  for( std::map<std::string, TH1D*>::iterator it = fUncorrErrorMap.begin(); it != fUncorrErrorMap.end(); ++it )
  {
    const TH1D* err1 = h1->GetUncorrError( it->first );
    if( !err1 )
    {
      Error("Multiply", "Could not multiply MUH1Ds because they all don't have the %s uncorrelated error", it->first.c_str());
      return;
    }
    it->second->Multiply( err1, h2, c1, c2 );
  }

  // Do we have special errors in the systemics?
  if ( !( fSysErrorMatrix.empty() ) || !( fRemovedSysErrorMatrix.empty() ) )
  {
    Warning( "MUH1D::MultiplySingle", "Customized error matrices were found (errors that come from neither Vertical nor Lateral Error Bands) . They will be cleared.");
    ClearSysErrorMatrices( );
  }

  return;
}



Bool_t MUH1D::Add( const TH1* h1, const Double_t c1 /*= 1.*/ )
{
  // Try to cast the input TH1 to a MUH1D
  // If cast doesn't work, I don't know what to do. How do we add a MUH1D and TH1?
  const MUH1D *mnv1 = dynamic_cast<const MUH1D*>(h1);

  if( mnv1 )
  {
    // If cast works...
    //! @todo do the consistency checks on axes before adding

    //save orig stat uncertainty in case we need it for averaging uncorrelated errors
    const TH1D origStatErr = this->GetStatError();

    // Add as a TH1D
    this->TH1D::Add( h1, c1 );

    // Call Add for all lateral error bands
    for( std::map<std::string, MULatErrorBand*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
    {
      Bool_t ok = false;
      const MULatErrorBand* err1 = mnv1->GetLatErrorBand( it->first );
      if( !err1  )
      {
        Warning("MUH1D::Add", "Additive MUH1D lacks %s MULatErrorBand.  Add central value to all universes.", it->first.c_str());
        ok = it->second->AddSingle( h1, c1 );
      }
      else
        ok = it->second->Add( err1, c1 );

      if( ! ok )
      {
        Error("Add", "Could not add MUH1Ds because histogram add failed for MULatErrorBand %s ", it->first.c_str());
        return kFALSE;
      }
    }//done adding Lat errors

    // Call Add for all vertical error bands
    for( std::map<std::string, MUVertErrorBand*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
    {
      Bool_t ok = false;
      const MUVertErrorBand* err1 = mnv1->GetVertErrorBand( it->first );
      if( !err1  )
      {
        Warning("MUH1D::Add", "Additive MUH1D lacks %s MUVertErrorBand.  Add central value to all universes.", it->first.c_str());
        ok = it->second->AddSingle( h1, c1 );
      }
      else
        ok = it->second->Add( err1, c1 );

      if( ! ok )
      {
        Error("Add", "Could not add MUH1Ds because histogram add failed for MUVertErrorBand %s ", it->first.c_str());
        return kFALSE;
      }
    }//done adding Vert errors

    //call add for all uncorrelated errors
    for( std::map<std::string, TH1D*>::iterator it = fUncorrErrorMap.begin(); it != fUncorrErrorMap.end(); ++it )
    {
      //note - TH1D::Add is void so we just hope that it works
      const TH1D* err1 = mnv1->GetUncorrError( it->first );
      if( !err1  )
      {
        //todo: a better way would be to add content but keep error equal to the one hist that has error
        Warning("MUH1D::Add", "Additive MUH1D lacks %s uncorrelated error.  Add central value to all universes.", it->first.c_str());
        it->second->TH1D::Add( (const TH1D*)h1, c1 );
      }
      else
      {
        //if this is an average add then we need to force correct behavior
        if( it->second->TestBit( TH1::kIsAverage ) && err1->TestBit( TH1::kIsAverage ) )
        {
          int nbins = it->second->GetNbinsX()+1;
          for( int ibin = 0; ibin <= nbins; ++ibin )
          {
            //apply same operations to error as were applied to CV, propagage the uncorr error
            double uncorrA = it->second->GetBinError(ibin);
            double uncorrB = err1->GetBinError(ibin);
            double errA = origStatErr.GetBinContent(ibin);
            double errB = h1->GetBinError(ibin);
            double wA = ( 0. < errA ) ? 1./(errA*errA) : 1.E200; //use err=sqrt(w) or very large value
            double wB = ( 0. < errB ) ? c1/(errB*errB) : 1.E200; //use err=sqrt(w) or very large value
            double errPieceA = (uncorrA*wA) / (wA+wB);
            double errPieceB = (uncorrB*wB) / (wA+wB);
            double err = sqrt( TMath::Power(errPieceA,2) +  TMath::Power(errPieceB,2) );
            it->second->SetBinError(ibin,err);
            //content is same as CV
            it->second->SetBinContent(ibin, this->GetBinContent(ibin) );
          }
        }
        else
          it->second->TH1D::Add( (const TH1D*)err1, c1 );
      }

    }//done adding uncorrelated errors

    // Do we have special errors in the systemics?
    if ( !( fSysErrorMatrix.empty() ) || !( fRemovedSysErrorMatrix.empty() ) )
    {
      Warning( "MUH1D::Add", "Customized error matrices were found (errors that come from neither Vertical nor Lateral Error Bands). They will be cleared.");
      ClearSysErrorMatrices( );
    }

    return kTRUE;

  }// end if cast to MUH1D worked
  else
  {
    Error( "MUH1D::Add", "Unable to add histogram because it could not be cast to an MUH1D.  Did nothing." );
  }

  return kFALSE; 

}

TH1* MUH1D::Rebin(  Int_t ngroup /*= 2*/, const char *newname /*= ""*/, const Double_t *xbins /*= 0*/ )
{
  // If a clone is specified or necessary (because bins have been specified) then give up for now
  if( (newname && strlen(newname) > 0) || xbins )
  {
    Error( "Rebin", "MUH1D's Rebin does not know how to create clones yet.  You can't specify a newname or new xbins." );
    throw 1;
  }

  // Call TH1D's Rebin and store the return value, which we return if everything went well
  TH1 *rval = this->TH1D::Rebin( ngroup );

  //we only need the rebin warning once
  const int oldVerbosity = gErrorIgnoreLevel;
  gErrorIgnoreLevel = kError;

  // Call Rebin for all lateral error bands
  for( std::map<std::string, MULatErrorBand*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
  {
    TH1 *tmpErrBand = it->second->Rebin( ngroup );

    if( NULL == tmpErrBand )
    {
      Error("Rebin", "Could not rebin MUH1Ds because Rebin failed for MULatErrorBand %s ", it->first.c_str());
      gErrorIgnoreLevel = oldVerbosity;
      return NULL;
    }
  }//done adding Lat errors

  // Call Rebin for all vertical error bands
  for( std::map<std::string, MUVertErrorBand*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
  {
    TH1 *tmpErrBand = it->second->Rebin( ngroup );

    if( NULL == tmpErrBand )
    {
      Error("Rebin", "Could not rebin MUH1Ds because Rebin failed for MUVertErrorBand %s ", it->first.c_str());
      gErrorIgnoreLevel = oldVerbosity;
      return NULL;
    }
  }//done adding Vert errors

  // Call Rebin for all uncorrelated errors
  for( std::map<std::string, TH1D*>::iterator it = fUncorrErrorMap.begin(); it != fUncorrErrorMap.end(); ++it )
  {
    TH1 *tmpErrBand = it->second->Rebin( ngroup );

    if( NULL == tmpErrBand )
    {
      Error("Rebin", "Could not rebin MUH1Ds because Rebin failed for uncorrelated error %s ", it->first.c_str());
      gErrorIgnoreLevel = oldVerbosity;
      return NULL;
    }
  }//done adding uncorr errors

  // Do we have special errors in the systemics?
  if ( !( fSysErrorMatrix.empty() ) || !( fRemovedSysErrorMatrix.empty() ) )
  {
    Warning("MUH1D::Rebin", "Customized error matrices were found (errors that come from neither Vertical nor Lateral Error Bands). They will be cleared.");
    ClearSysErrorMatrices( );
  }

  gErrorIgnoreLevel = oldVerbosity;

  return rval;
}


//=========================================
// Reset
//=========================================
void MUH1D::Reset(Option_t *option)
{
  //reset the base class
  this->TH1D::Reset(option);

  //reset all vert and lat error bands, but do not remove them
  for( std::map<std::string, MULatErrorBand*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
    it->second->Reset(option);
  for( std::map<std::string, MUVertErrorBand*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
    it->second->Reset(option);
  for( std::map<std::string, TH1D*>::iterator it = fUncorrErrorMap.begin(); it != fUncorrErrorMap.end(); ++it )
    it->second->Reset(option);

  //if there are any error matrices, clear them
  ClearSysErrorMatrices( );
}


//===========================================
// SetBit
//===========================================
void MUH1D::SetBit(UInt_t f, Bool_t set)
{
  //set the base class bit
  this->TH1D::SetBit(f,set);

  //reset all vert and lat error bands
  for( std::map<std::string, MULatErrorBand*>::iterator it = fLatErrorBandMap.begin(); it != fLatErrorBandMap.end(); ++it )
    it->second->SetBit(f,set);
  for( std::map<std::string, MUVertErrorBand*>::iterator it = fVertErrorBandMap.begin(); it != fVertErrorBandMap.end(); ++it )
    it->second->SetBit(f,set);
  for( std::map<std::string, TH1D*>::iterator it = fUncorrErrorMap.begin(); it != fUncorrErrorMap.end(); ++it )
    it->second->SetBit(f,set);

  //maybe we need to clear error matrices.
  //there are cases where you wouldn't, so leave this to the caller
}

#endif
