#ifndef MNV_MUH1D_H
#define MNV_MUH1D_H 1

#include "TObject.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVectorD.h"

#include "PlotUtils/MUVertErrorBand.h"
#include "PlotUtils/MULatErrorBand.h"

#include <string>
#include <vector>
#include <map>


namespace PlotUtils
{

	/*! @brief An extension of the TH1D class which has knowledge of uncorrelated pieces contributing to systematic error.

		There are two types of systematic error bands:
		<ul>
		<li>Vertical - The weight of the event varies (e.g. for flux error)
		<li>Lateral  - There are shifts in the variable being histogrammed (e.g. for energy resolution)
		</ul>

		The statistical errors are stored in this classes bin errors, and you may use GetBinError(int) to access them.
		When the total error band is returned, the contributions in each bin are systematic (and statistical is desired) errors added in quadrature.

		TH1D::Sumw2() is always called in the constructor.

		@author Brian Tice, Gabriel Perdue
		*/
	class MUH1D: public TH1D
	{
		public:
			//! Default constructor
			MUH1D();

			//! Default Constructor with specified normalized bin width
			explicit MUH1D( Double_t normBinWidth );

			//==== Copy Constructors ====//
			//! Construct from vector and Sumw2
			MUH1D( const TVectorD& v );

			//! Copy constructor and Sumw2
			MUH1D( const TH1D& h1d );

			//==== Copy Constructors with specified normalized bin width ====//
			//! Construct from vector and Sumw2
			MUH1D( const TVectorD& v, Double_t normBinWidth );

			//! Copy constructor and Sumw2
			MUH1D( const TH1D& h1d, Double_t normBinWidth );

			//! Deep copy constructor (the real copy)
			MUH1D( const MUH1D& h );

			//! Deep assignment
			MUH1D& operator=( const MUH1D& h );

		private:
			//! A helper function which set variables for the deep copy and assignment
			void DeepCopy( const MUH1D& h );

			//! A helper function to check if this string has that ending
			bool HasEnding (std::string const &fullString, std::string const &ending) const;

			//! Does sumw2 and prints no warning
			void SilentSumw2();

		public:

			/*! Quiet warnings about hidden overloaded virtual functions.
				It is not clear if this is the behavior we want (may prefer
				to ``not use'' these.
				*/ 
			using TH1::Divide; 
			using TH1::Multiply;

			//==== Constructors with default normalized bin width ====//
			//! Construct with variable bin sizes and Sumw2
			MUH1D( const char* name, const char* title, Int_t nbinsx, const Float_t* xbins);

			//! Construct with variable bin sizes and Sumw2
			MUH1D( const char* name, const char* title, Int_t nbinsx, const Double_t* xbins);

			//! Construct with constant bin sizes and Sumw2
			MUH1D( const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup);

			//=== Constructors which specify a default normalization bin width ===//
			//! Construct with variable bin sizes and use this binWidth to normalize bins by default
			MUH1D( const char* name, const char* title, Int_t nbinsx, const Float_t* xbins, Double_t normBinWidth );

			//! Construct with variable bin sizes and use this binWidth to normalize bins by default
			MUH1D( const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Double_t normBinWidth);

			//! Construct with variable bin sizes and use this binWidth to normalize bins by default
			MUH1D( const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Double_t normBinWidth);


			//!Destructor (note: root cleans up histograms for us)
			virtual ~MUH1D() {};

			//! Get the default bin width to which bin content/error is normalized
			inline Double_t GetNormBinWidth() const { return fNormBinWidth; };

			//! Set the default bin width to which bin content/error is normalized
			inline void     SetNormBinWidth(const Double_t x ) { fNormBinWidth = x; };

			void RenameHistosAndErrorBands( const std::string& newname);

			//! Delete all Error Bands 
			void ClearAllErrorBands();

			//! Add a new MULatErrorBand (nhists=-1 uses LatErrorBand's default)
			bool AddLatErrorBand( const std::string& name, const int nhists = -1 );
			//! Add a customed MULatErrorBand
			bool AddLatErrorBand( const std::string& name, const std::vector<TH1D*>& base );
			//! Add a new MULatErrorBand and fill its universes with the CV 
			bool AddLatErrorBandAndFillWithCV( const std::string& name, const int nhists );
			//! Add a new MUVertErrorBand (nhists=-1 uses VertErrorBand's default)
			bool AddVertErrorBand( const std::string& name, const int nhists = -1 );
			//! Add a customed MUVertErrorBand
			bool AddVertErrorBand( const std::string& name, const std::vector<TH1D*>& base );
			//! Add a new MUVertErrorBand and fill its universes with the CV 
			bool AddVertErrorBandAndFillWithCV( const std::string& name, const int nhists );

			//! Add an uncorrelated error that you intend to fill
			bool AddUncorrError( const std::string& name );
			//! Add an uncorrelated error whose information is already stored in this histogram (either content or errors)
			bool AddUncorrError( const std::string& name, const TH1D* hist, bool errInContent = false );
			//! Add a new uncorrelated error with 0 error (silly but makes things simpler later)
			bool AddUncorrErrorAndFillWithCV( const std::string& name );

			//! Add missing MUVertErrorBands or MULatErrorBands and fill their universes with the CV 
			bool AddMissingErrorBandsAndFillWithCV( const MUH1D& ref );

			//! Check for the existence of a lateral error band
			bool HasLatErrorBand( const std::string& name ) const;
			//! Check for the existence of a vertical error band
			bool HasVertErrorBand( const std::string& name ) const;
			//! Check for the existence of an uncorrelated error
			bool HasUncorrError( const std::string& name ) const;
			//! Check for the existence of an error band (of any type)
			bool HasErrorBand( const std::string& name ) const;
			//! Check for the existence of an error Matrix (of any type)
			bool HasErrorMatrix( const std::string& name ) const;
			//! Check for the existence of an error Matrix in the fRemovedSysErrorMatrix map
			bool HasRemovedErrorMatrix( const std::string& name ) const;

			//! Get a pointer to this MULatErrorBand
			MULatErrorBand* GetLatErrorBand( const std::string& name );
			//! Get a pointer to this MUVertErrorBand
			MUVertErrorBand* GetVertErrorBand( const std::string& name );
			//! Get a pointer to this uncorrelated error
			TH1D* GetUncorrError( const std::string& name );

			//! Get this uncorrelated error as a histogram (like MUVertErrorBand::GetErrorBand)
			TH1D GetUncorrErrorAsHist( const std::string& name, bool asFrac = false ) const;

			//! Get a const pointer to this MULatErrorBand
			const MULatErrorBand* GetLatErrorBand( const std::string& name ) const;
			//! Get a const pointer to this MUVertErrorBand
			const MUVertErrorBand* GetVertErrorBand( const std::string& name ) const;
			//! Get a const pointer to this uncorrelated error
			const TH1D* GetUncorrError( const std::string& name ) const;

			//! How many MULatErrorBands are there?
			size_t GetNLatErrorBands() const { return fLatErrorBandMap.size(); };
			//! How many MUVertErrorBands are there?
			size_t GetNVertErrorBands() const { return fVertErrorBandMap.size(); };
			//! How many uncorrelated errors are there?
			size_t GetNUncorrErrors() const { return fUncorrErrorMap.size(); };
			//! How many SysErrorMatrices are there?
			size_t GetNSysErrorMatrices() const { return fSysErrorMatrix.size(); };
			//! How many SysErrorMatrices are there?
			size_t GetNRemovedSysErrorMatrices() const { return fRemovedSysErrorMatrix.size(); };
			//! How many error sources are there (exlcudes removed SysErrorMatrices)?
			size_t GetNErrorSources() const { return fLatErrorBandMap.size() + fVertErrorBandMap.size() + fUncorrErrorMap.size() + fSysErrorMatrix.size(); };



			//! Get a vector of the names of all error bands
			std::vector<std::string> GetErrorBandNames() const;
			//! Get a vector of the names of lateral error bands
			std::vector<std::string> GetLatErrorBandNames() const;
			//! Get a vector of the names of vertical error bands
			std::vector<std::string> GetVertErrorBandNames() const;
			//! Get a vector of the names of uncorrelated errors
			std::vector<std::string> GetUncorrErrorNames() const;
			//! Get a vector of the names of Systematic Error Matrices
			std::vector<std::string> GetSysErrorMatricesNames() const;
			//! Get a vector of the names of Systematic Error Matrices
			//! that were removed and are now in fRemovedSysErrorMatrix
			std::vector<std::string> GetRemovedSysErrorMatricesNames() const;

			//! Transfer all lat and vert error bands to hist
			bool TransferErrorBands( MUH1D *hist, bool removeFromMe = true );
			//! Transfer a lat error band to hist
			bool TransferVertErrorBand( MUH1D *hist, const std::string& errName, bool removeFromMe = true );
			//! Transfer a lat error band to hist
			bool TransferLatErrorBand( MUH1D *hist, const std::string& errName, bool removeFromMe = true );

			//! Remove the error band from the MUH1D and return the pointer to the object (caller owns the memory)
			MUVertErrorBand* PopVertErrorBand( const std::string& name );
			//! Remove the error band from the MUH1D and return the pointer to the object (caller owns the memory)
			MULatErrorBand* PopLatErrorBand( const std::string& name );
			//! Remove the uncorrelated error from the MUH1D and return the pointer to the object (caller owns the memory)
			TH1D* PopUncorrError( const std::string& name );

			//! Add this vert error band (with memory ownership) to the MUH1D 
			bool PushErrorBand( const std::string& name, MUVertErrorBand* errBand );
			//! Add this lat error band (with memory ownership) to the MUH1D 
			bool PushErrorBand( const std::string& name, MULatErrorBand* errBand );
			//! Add this uncorrelated error hsit (with memory ownership) to the MUH1D 
			bool PushUncorrError( const std::string& name, TH1D* err );

			//! Fill the shifts of an MULatErrorBand's universes from a vector
			bool FillLatErrorBand( const std::string& name, const double val, const std::vector<double>& shifts, const double cvweight = 1.0, const bool fillcv = true, const double *weights = 0 );
			//! Fill the shifts of an MULatErrorBand's universes from array
			bool FillLatErrorBand( const std::string& name, const double val, const double * shifts, const double cvweight = 1.0, const bool fillcv = true, const double *weights = 0 );
			//! Fill the shifts of an MULatErrorBand's 2 universes with these 2 shifts (must have 2)
			bool FillLatErrorBand( const std::string& name, const double val, const double shiftDown, const double shiftUp, const double cvweight = 1.0, const bool fillcv = true );

			//! Fill the weights of an MUVertErrorBand's universes from a vector
			bool FillVertErrorBand( const std::string& name, const double val, const std::vector<double>& weights, const double cvweight  = 1.0, double cvWeightFromMe = 1.);
			//! Fill the weights of an MUVertErrorBand's universes from array
			bool FillVertErrorBand( const std::string& name, const double val, const double * weights, const double cvweight  = 1.0, double cvWeightFromMe = 1.);
			//! Fill the weights of an MUVertErrorBand's 2 universes with these 2 weights (must have 2)
			bool FillVertErrorBand( const std::string& name, const double val, const double weightDown, const double weightUp, const double cvweight  = 1.0, double cvWeightFromMe = 1. );

			//! Fill the uncorrelated error
			bool FillUncorrError( const std::string& name, const double val, const double err, const double cvweight = 1.0 );

			//! Fill the weights of these ErrorBands' universes (defunct for now)
			//bool FillErrorBands( const std::map<std::string, std::vector<double> >& weightMap, const double val, const double cvweight  = 1.0  );

			//! Set the use spread variable for all sys error bands
			void SetUseSpreadErrorAll( bool use );

			/*! Get a new TH1D which is the central value histogram with statistical error only
				@return copy of a TH1D
				*/
			TH1D GetCVHistoWithStatError() const;

			/*! Get a new TH1D which is the central value histogram with errors added in quadrature
				@todo add param errNames to add only certain errors from these error bands.
				@return copy of a TH1D
				*/
			TH1D GetCVHistoWithError( bool includeStat = true, bool cov_area_normalize = false ) const;
			//    TH1D GetCVHistoWithError( const std::vector<std::string>& errNames ) const;

			//! Get a TH1D filled with the errors summed in quadrature
			TH1D GetTotalError( bool includeStat = true, bool asFrac = false , bool cov_area_normalize = false ) const;

			//! Get a TH1D filled with the stat error ONLY
			TH1D GetStatError( bool asFrac = false ) const;

			//! Get the Total Covariance Matrix
			TMatrixD GetTotalErrorMatrix(bool includeStat = true, bool asFrac = false, bool cov_area_normalize = false ) const;
			//! Get Total Correlation Matrix
			TMatrixD GetTotalCorrelationMatrix(bool cov_area_normalize = false ) const;

			Int_t WriteTotalErrorMatrix(const std::string& name, bool includeStat = true, bool asFrac = false, bool cov_area_normalize = false);
			Int_t WriteTotalCorrelationMatrix(const std::string& name, bool cov_area_normalize = false);

			//! Push a Covariance Matrix
			bool PushCovMatrix(const std::string& name, TMatrixD covmx, bool cov_area_normalize = false);

			//! Get Statistical Error Matrix
			TMatrixD GetStatErrorMatrix( bool asFrac = false ) const;

			//! Get a single Systematical Matrix from the map
			TMatrixD GetSysErrorMatrix(const std::string& name, bool asFrac = false, bool cov_area_normalize = false) const; 

			//! Remove a Systematic Error from the Total Systematic Container
			bool RemoveSysErrorMatrix(const std::string& name);
			//! Unremove a Systematic Error from the Total Systematic Container
			bool UnRemoveSysErrorMatrix(const std::string& name);

			//! Delete all Systematic Error matrices
			//! Useful because they are only calculated once and histograms may change
			void ClearSysErrorMatrices( );

			//! Get a single Systematical Covariance Matrix from the map
			TMatrixD GetSysCorrelationMatrix(const std::string& name, bool cov_area_normalize = false) const;

			/*! Get an MUH1D which has its bin content and errors normalized to bin width so it looks smooth
				@param[in] normBinWidth bin width to normalize to.  normalization = normBinWidth / thisBinWidth.  (nonpositive means use MUH1D's default if set)
				@return A copy of this MUH1D which has its bin content/error normalized to bin width
				*/
			MUH1D GetBinNormalizedCopy( Double_t normBinWidth = -1. ) const;

			//! Geting Area Normalization Scale Factor (scale MC respect to Data)
			Double_t GetAreaNormFactor(const MUH1D * h_data) const;


			/*! Draw a version of this histogram that has its bin content and errors normalized to bin width so it looks smooth
				@param[in] option see THistPainter documentation.  
				@param[in] normBinWidth bin width to normalize to.  normalization = normBinWidth / thisBinWidth.  (nonpositive means use MUH1D's default if set)
				@return A pointer to a NEW MUH1D, which the caller now owns (so delete it!)
				*/
			MUH1D *DrawBinNormalized( Option_t* option = "", Double_t normBinWidth = -1 ) const;

			//=======================================================================
			// Implementations of ROOT virtual functions
			//=======================================================================
			//! When calling scale we need to scale all of the error bands histos
			virtual void Scale( Double_t c1 = 1., Option_t *option = "" );

			/*! Replace this MUH1D's contents with the result of a division of two other MUH1Ds
				see http://root.cern.ch/root/html/TH1.html#TH1:Divide@2
				*/
			virtual Bool_t Divide( const MUH1D* h1, const MUH1D* h2, Double_t c1 = 1, Double_t c2 = 1, Option_t* option="" );

			/*! Replace this MUH1D's contents with the result of dividing an MUH1D and all its errors by a TH1
			*/
			virtual void DivideSingle( const MUH1D* h1, const TH1* h2, Double_t c1 = 1, Double_t c2 = 1, Option_t* option="" );

			//! Replace this MUH1D's contents with the result of a multiplication of two other MUH1Ds
			virtual Bool_t Multiply( const MUH1D* h1, const MUH1D* h2, const Double_t c1 = 1., const Double_t c2 = 1. );

			//! Repalce this MUH1D's contents with the results of multiplying an MUH1D and all its errors by a TH1
			virtual void MultiplySingle( const MUH1D* h1, const TH1* h2, const Double_t c1 = 1., const Double_t c2 = 1. );

			//! Our own implementation of the TH1::Add used by hadd
			virtual Bool_t Add( const TH1* h1, const Double_t c1 = 1. );

			//! Rebin and propagate to error bands
			//! @note newname and xbins arguments will not work for MUH1D yet, but I wanted to overwrite the TH1D version of this function
			virtual TH1* Rebin(  Int_t ngroup = 2, const char *newname = "", const Double_t *xbins = 0 );

			//! Reset and propagate to error bands.  clear error matrices.
			void Reset( Option_t *option = "" );

			//! Set/Reset histogram bit and propagate to error bands
			void SetBit(UInt_t f, Bool_t set);
			//! Set histogram bit and propagate to error bands
			void SetBit(UInt_t f) { SetBit(f, true); };

		private:

			//! Strores a map from name to error band for MULatErrorBands
			std::map<std::string, MULatErrorBand*> fLatErrorBandMap;

			//! Strores a map from name to error band for MUVertErrorBands
			std::map<std::string, MUVertErrorBand*> fVertErrorBandMap;

			//! Stores a map from name of Systematics Error Matrices 
			std::map<std::string, TMatrixD*> fSysErrorMatrix;

			//! Stores a map from name of removed Systematic Error Matrices
			std::map<std::string, TMatrixD*> fRemovedSysErrorMatrix;

			//! Stores a map from name to histogram that stored uncorrelated errors
			std::map<std::string, TH1D*> fUncorrErrorMap;

			//! Stores the width to which we will normalize bins (e.g. n Events per fNormBinWidth GeV)
			//! If negative, then refuse to normalize to bin width (appropriate for ratios, efficiencies)
			Double_t fNormBinWidth;

			//!define a class named MUH1D, at version 3
			//! v4 - introduce uncorrelated errors
			ClassDef(MUH1D, 4); //MINERvA 1-D histogram of doubles with a map of MUErrorBands

	}; //end of MUH1D

}//end PlotUtils

#endif
