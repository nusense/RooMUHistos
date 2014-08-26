#ifndef MNV_MUH2D_H
#define MNV_MUH2D_H 1

#include "TObject.h"
#include "TString.h"
#include "TH2D.h"
#include "TVectorD.h"
#include "PlotUtils/MUH1D.h"
#include "PlotUtils/MUVertErrorBand2D.h"
#include "PlotUtils/MULatErrorBand2D.h"
#include <string>
#include <vector>
#include <map>


namespace PlotUtils
{

	class MUH2D: public TH2D
	{
		public:
			//! Default constructor
			MUH2D();

			//! Default Constructor with specified normalized bin width
			explicit MUH2D( Double_t normBinWidthX, Double_t normBinWidthY);

			//==== Copy Constructors ====//
			//! Copy constructor and Sumw2
			MUH2D( const TH2D& h2d );

			//==== Copy Constructors with specified normalized bin width ====//
			//! Copy constructor and Sumw2
			MUH2D( const TH2D& h2d, Double_t fnormBinWidthX, Double_t fnormBinWidthY );

			//==== Constructors with default normalized bin width ====//
			//! Construct with variable bin sizes and Sumw2
			MUH2D( const char* name, const char* title, Int_t nbinsx, const Float_t* xbins, Int_t nbinxy, const Float_t* ybins);

			//! Construct with variable bin sizes and Sumw2
			MUH2D( const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, const Double_t* ybins);

			//! Construct with constant bin sizes and Sumw2
			MUH2D( const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup);

			//! Deep copy constructor (the real copy)
			MUH2D( const MUH2D& h );

			//! Deep assignment
			MUH2D& operator=( const MUH2D& h );

		private:
			//! A helper function which set variables for the deep copy and assignment
			void DeepCopy( const MUH2D& h );

		public:
			/*! Quiet warnings about hidden overloaded virtual functions.
				It is not clear if this is the behavior we want (may prefer
				to ``not use'' these.
				*/ 
			using TH2::Add; 
			using TH2::Divide; 
			using TH2::Multiply;

			//=== Constructors which specify a default normalization bin width ===//
			//! Construct with variable bin sizes and use this binWidth to normalize bins by default
			MUH2D( const char* name, const char* title, Int_t nbinsx, const Float_t* xbins, Int_t nbinsy, const Float_t* ybins, Double_t normBinWidthX, Double_t normBinWidthY );

			//! Construct with variable bin sizes and use this binWidth to normalize bins by default
			MUH2D( const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, const Double_t* ybins, Double_t normBinWidthX, Double_t normBinWidthY);

			//! Construct with variable bin sizes and use this binWidth to normalize bins by default
			MUH2D( const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Double_t normBinWidthX, Double_t normBinWidthY);
			//=== end of constructors ===/

			/*! Get an MUH2D which has its bin content and errors normalized to bin width so it looks smooth
				@param[in] normBinWidthX,normBinWidthY bin width for X and Y axis to normalize to.  normalization = normBinWidthX * normBinWidthY / thisBinArea.
				where thisBinArea = thisbinWidthX*thisbinWidthY (nonpositive means use MUH2D's default if set)
				@return A copy of this MUH2D which has its bin content/error normalized to bin width
				*/
			MUH2D GetBinNormalizedCopy( Double_t normBinWidthX = -1., Double_t normBinWidthY = -1.) const;

			MUH1D *ProjectionX(const char* name = "_px", Int_t firstybin = 0, Int_t lastybin = -1, Option_t* option = "") const;

			MUH1D *ProjectionY(const char* name = "_py", Int_t firstxbin = 0, Int_t lastxbin = -1, Option_t* option = "") const;

			bool HasVertErrorBand( const std::string& name ) const;
			bool HasLatErrorBand( const std::string& name ) const;

			//! Check for the existence of an error band (of any type)
			bool HasErrorBand( const std::string& name ) const;

			bool HasErrorMatrix( const std::string& name ) const;

			bool AddVertErrorBand( const std::string& name, const int nhists = -1 );
			//! Add a customed MUVertErrorBand
			bool AddVertErrorBand( const std::string& name, const std::vector<TH2D*>& base );
			//! Add a new MUVertErrorBand and fill its universes with the CV 
			bool AddVertErrorBandAndFillWithCV( const std::string& name, const unsigned int nhists );


			bool AddLatErrorBand( const std::string& name, const int nhists = -1 );
			//! Add a customed MULatErrorBand
			bool AddLatErrorBand( const std::string& name, const std::vector<TH2D*>& base );
			//! Add a new MULattErrorBand and fill its universes with the CV 
			bool AddLatErrorBandAndFillWithCV( const std::string& name, const unsigned int nhists );


			//! Fill the weights of an MUVertErrorBand's universes from a vector
			bool FillVertErrorBand( const std::string& name, const double xval, const double yval, const std::vector<double>& weights, const double cvweight  = 1.0, double cvWeightFromMe = 1. );
			//! Fill the weights of a MUVertErrorBand's universes from array
			bool FillVertErrorBand( const std::string& name, const double xval, const double yval, const double * weights, const double cvweight  = 1.0, double cvWeightFromMe = 1. );
			//! Fill the weights of an MUVertErrorBand's 2 universes with these 2 weights (must have 2)
			bool FillVertErrorBand( const std::string& name, const double xval, const double yval, const double weightDown, const double weightUp, const double cvweight  = 1.0, double cvWeightFromMe = 1. );

			//! Fill the weights of a MULatErrorBand's universes from a vector
			bool FillLatErrorBand( const std::string& name, const double xval, const double yval, const std::vector<double>& xshifts, const std::vector<double>& yshifts, const double cvweight  = 1.0, const bool fillcv = true, const double* weights = 0 );
			//! Fill the weights of an MULatErrorBand's universes from array
			bool FillLatErrorBand( const std::string& name, const double xval, const double yval, const double *xshifts, const double *yshifts, const double cvweight  = 1.0, const bool fillcv = true, const double* weights = 0 );
			//! Fill the weights of an MULatErrorBand's 2 universes with these 2 weights (must have 2)
			bool FillLatErrorBand( const std::string& name, const double xval, const double yval, const double xshiftDown, const double xshiftUp, const double yshiftDown, const double yshiftUp, const double cvweight = 1.0, const bool fillcv = true );

			//! Get a pointer to this MUVertErrorBand
			MUVertErrorBand2D* GetVertErrorBand( const std::string& name );
			//! Get a const pointer to this MUVertErrorBand
			const MUVertErrorBand2D* GetVertErrorBand( const std::string& name ) const;

			//! Get a pointer to this MULatErrorBand
			MULatErrorBand2D* GetLatErrorBand( const std::string& name );
			//! Get a const pointer to this MULatErrorBand
			const MULatErrorBand2D* GetLatErrorBand( const std::string& name ) const;

			/*! Get a new TH2D which is the central value histogram with statistical error only
				@return copy of a TH2D
				*/
			TH2D GetCVHistoWithStatError() const;

			/*! Get a new TH2D which is the central value histogram with errors added in quadrature
				@todo add param errNames to add only certain errors from these error bands.
				@return copy of a TH2D
				*/
			TH2D GetCVHistoWithError( bool includeStat = true, bool cov_area_normalize = false ) const;
			//    TH1D GetCVHistoWithError( const std::vector<std::string>& errNames ) const;

			//! Get a TH2D filled with the errors summed in quadrature
			TH2D GetTotalError( bool includeStat = true, bool asFrac = false , bool cov_area_normalize = false ) const;

			//! Get a TH2D filled with the stat error ONLY
			TH2D GetStatError( bool asFrac = false ) const;

			//! Get the Total Covariance Matrix
			TMatrixD GetTotalErrorMatrix(bool includeStat = true, bool asFrac = false, bool cov_area_normalize = false ) const;
			//! Get Total Correlation Matrix
			TMatrixD GetTotalCorrelationMatrix(bool cov_area_normalize = false ) const;

			//! Get Statistical Error Matrix
			TMatrixD GetStatErrorMatrix( bool asFrac = false ) const;

			//! Get a single Systematical Matrix from the map
			TMatrixD GetSysErrorMatrix(const std::string& name, bool asFrac = false, bool cov_area_normalize = false) const; 
			//! Get a vector of the names of all error bands
			std::vector<std::string> GetVertErrorBandNames() const;

			//! Get a vector of the names of all error bands
			std::vector<std::string> GetLatErrorBandNames() const;

			//! Get a vector of the names of Systematic Error Matrices
			std::vector<std::string> GetSysErrorMatricesNames() const;
			//=======================================================================
			// Implementations of ROOT virtual functions
			//=======================================================================
			//! When calling scale we need to scale all of the error bands histos
			virtual void Scale( Double_t c1 = 1., Option_t *option = "" );

			/*! Replace this MUH2D's contents with the result of a division of two other MUH2Ds
			*/
			virtual void Divide( const MUH2D* h1, const MUH2D* h2, Double_t c1 = 1, Double_t c2 = 1, Option_t* option="" );

			/*! Replace this MUH2D's contents with the result of dividing an MUH2D and all its errors by a TH2
			*/
			virtual void DivideSingle( const MUH2D* h1, const TH2* h2, Double_t c1 = 1, Double_t c2 = 1, Option_t* option="" );

			//! Add Histograms
			virtual void Add( const TH2* h1, const Double_t c1 = 1. );

			//! Replace this MUH2D's contents with the result of a multiplication of two other MUH2Ds
			virtual void Multiply( const MUH2D* h1, const MUH2D* h2, const Double_t c1 = 1., const Double_t c2 = 1. );

			//! Get the default bin width to which bin content/error is normalized
			inline Double_t GetNormBinWidthX() const { return fNormBinWidthX; };
			inline Double_t GetNormBinWidthY() const { return fNormBinWidthY; };

			//! Set the default bin width to which bin content/error is normalized
			inline void     SetNormBinWidthX(const Double_t x ) { fNormBinWidthX = x; };
			inline void     SetNormBinWidthY(const Double_t y ) { fNormBinWidthY = y; };

			//!Destructor (note: root cleans up histograms for us)
			virtual ~MUH2D() {};

		private:
			//! A helper function to check if this string has that ending
			bool HasEnding (std::string const &fullString, std::string const &ending) const;

			//! Stores a map from name of Systematics Error Matrices 
			std::map<std::string, TMatrixD*> fSysErrorMatrix;

			//! Stores a map from name of removed Systematic Error Matrices
			std::map<std::string, TMatrixD*> fRemovedSysErrorMatrix;

			//! Strores a map from name to error band for MULatErrorBands
			std::map<std::string, MULatErrorBand2D*> fLatErrorBandMap;

			//! Strores a map from name to error band for MUVertErrorBands
			std::map<std::string, MUVertErrorBand2D*> fVertErrorBandMap;

			//! Stores the width to which we will normalize bins (e.g. n Events per fNormBinWidth GeV)
			//! If negative, then refuse to normalize to bin width (appropriate for ratios, efficiencies)
			Double_t fNormBinWidthX;
			Double_t fNormBinWidthY;

			//!define a class named MUH2D, at version 1
			ClassDef(MUH2D, 1); //MINERvA 2-D histogram

	}; //end of MUH2D

}//end PlotUtils

#endif
