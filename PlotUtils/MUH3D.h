#ifndef MNV_MUH3D_H
#define MNV_MUH3D_H 1

#include "TObject.h"
#include "TString.h"
#include "TH3D.h"
#include "TVectorD.h"
#include "MUH1D.h"
#include "MUH2D.h"
#include "MUVertErrorBand3D.h"
#include "MULatErrorBand3D.h"
#include <string>
#include <vector>
#include <map>


namespace PlotUtils
{

	class MUH3D: public TH3D
	{
		public:
			//! Default constructor
			MUH3D();

			//! Default Constructor with specified normalized bin width
			explicit MUH3D( Double_t normBinWidthX, Double_t normBinWidthY, Double_t normBinWidthZ);

			//==== Copy Constructors ====//
			//! Copy constructor and Sumw2
			MUH3D( const TH3D& h2d );

			//==== Copy Constructors with specified normalized bin width ====//
			//! Copy constructor and Sumw2
			MUH3D( const TH3D& h2d, Double_t fnormBinWidthX, Double_t fnormBinWidthY, Double_t fnormBinWidthZ );

			//==== Constructors with default normalized bin width ====//
			//! Construct with variable bin sizes and Sumw2
			MUH3D( const char* name, const char* title, Int_t nbinsx, const Float_t* xbins, Int_t nbinsy, const Float_t* ybins, Int_t nbinsz, const Float_t* zbins);

			//! Construct with variable bin sizes and Sumw2
			MUH3D( const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, const Double_t* ybins, Int_t nbinsz, const Double_t* zbins);

			//! Construct with constant bin sizes and Sumw2
			MUH3D( const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nbinsz, Double_t zlow, Double_t zup);

			//! Deep copy constructor (the real copy)
			MUH3D( const MUH3D& h );

			//! Deep assignment
			MUH3D& operator=( const MUH3D& h );

		private:
			//! A helper function which set variables for the deep copy and assignment
			void DeepCopy( const MUH3D& h );

		public:
			/*! Quiet warnings about hidden overloaded virtual functions.
				It is not clear if this is the behavior we want (may prefer
				to ``not use'' these.
				*/ 
			using TH3::Add; 
			using TH3::Divide; 
			using TH3::Multiply;

			//=== Constructors which specify a default normalization bin width ===//
			//! Construct with variable bin sizes and use this binWidth to normalize bins by default
			MUH3D( const char* name, const char* title, Int_t nbinsx, const Float_t* xbins, Int_t nbinsy, const Float_t* ybins, Int_t nbinsz, const Float_t* zbins, Double_t normBinWidthX, Double_t normBinWidthY, Double_t normBinWidthZ );

			//! Construct with variable bin sizes and use this binWidth to normalize bins by default
			MUH3D( const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, const Double_t* ybins, Int_t nbinsz, const Double_t* zbins, Double_t normBinWidthX, Double_t normBinWidthY, Double_t normBinWidthZ);

			//! Construct with variable bin sizes and use this binWidth to normalize bins by default
			MUH3D( const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nbinsz, Double_t zlow, Double_t zup, Double_t normBinWidthX, Double_t normBinWidthY, Double_t normBinWidthZ);
			//=== end of constructors ===/

			/*! Get an MUH3D which has its bin content and errors normalized to bin width so it looks smooth
				@param[in] normBinWidthX,normBinWidthY bin width for X and Y axis to normalize to.  normalization = normBinWidthX * normBinWidthY / thisBinArea.
				where thisBinArea = thisbinWidthX*thisbinWidthY (nonpositive means use MUH3D's default if set)
				@return A copy of this MUH3D which has its bin content/error normalized to bin width
				*/
			MUH3D GetBinNormalizedCopy( Double_t normBinWidthX = -1., Double_t normBinWidthY = -1., Double_t normBinWidthZ = -1.) const;

			MUH1D *ProjectionX(const char* name = "_px", Int_t firstybin = 0, Int_t lastybin = -1, Int_t firstzbin = 0, Int_t lastzbin = -1, Option_t* option = "") const;

			MUH1D *ProjectionY(const char* name = "_py", Int_t firstxbin = 0, Int_t lastxbin = -1, Int_t firstzbin = 0, Int_t lastzbin = -1, Option_t* option = "") const;

			MUH1D *ProjectionZ(const char* name = "_pz", Int_t firstxbin = 0, Int_t lastxbin = -1, Int_t firstybin = 0, Int_t lastybin = -1, Option_t* option = "") const;

			TH1 *Project3D(Option_t* option = "x") const;

			bool HasVertErrorBand( const std::string& name ) const;
			bool HasLatErrorBand( const std::string& name ) const;

			//! Check for the existence of an error band (of any type)
			bool HasErrorBand( const std::string& name ) const;

			bool HasErrorMatrix( const std::string& name ) const;

			bool AddVertErrorBand( const std::string& name, const int nhists = -1 );
			//! Add a customed MUVertErrorBand
			bool AddVertErrorBand( const std::string& name, const std::vector<TH3D*>& base );

			bool AddLatErrorBand( const std::string& name, const int nhists = -1 );
			//! Add a customed MULatErrorBand
			bool AddLatErrorBand( const std::string& name, const std::vector<TH3D*>& base );

			//! Fill the weights of an MUVertErrorBand's universes from a vector
			bool FillVertErrorBand( const std::string& name, const double xval, const double yval, const double zval, const std::vector<double>& weights, const double cvweight  = 1.0, double cvWeightFromMe = 1. );
			//! Fill the weights of a MUVertErrorBand's universes from array
			bool FillVertErrorBand( const std::string& name, const double xval, const double yval, const double zval, const double * weights, const double cvweight  = 1.0, double cvWeightFromMe = 1. );
			//! Fill the weights of an MUVertErrorBand's 2 universes with these 2 weights (must have 2)
			bool FillVertErrorBand( const std::string& name, const double xval, const double yval, const double zval, const double weightDown, const double weightUp, const double cvweight  = 1.0, double cvWeightFromMe = 1. );

			//! Fill the weights of a MULatErrorBand's universes from a vector
			bool FillLatErrorBand( const std::string& name, const double xval, const double yval, const double zval, const std::vector<double>& xshifts, const std::vector<double>& yshifts, const std::vector<double>& zshifts, const double cvweight  = 1.0, const bool fillcv = true, const double* weights = NULL );
			//! Fill the weights of an MULatErrorBand's universes from array
			bool FillLatErrorBand( const std::string& name, const double xval, const double yval, const double zval, const double *xshifts, const double *yshifts, const double *zshifts, const double cvweight  = 1.0, const bool fillcv = true, const double* weights = NULL );
			//! Fill the weights of an MULatErrorBand's 2 universes with these 2 weights (must have 2)
			bool FillLatErrorBand( const std::string& name, const double xval, const double yval, const double zval, const double xshiftDown, const double xshiftUp, const double yshiftDown, const double yshiftUp, const double zshiftDown, const double zshiftUp, const double cvweight = 1.0, const bool fillcv = true );

			//! Get a pointer to this MUVertErrorBand
			MUVertErrorBand3D* GetVertErrorBand( const std::string& name );
			//! Get a const pointer to this MUVertErrorBand
			const MUVertErrorBand3D* GetVertErrorBand( const std::string& name ) const;

			//! Get a pointer to this MULatErrorBand
			MULatErrorBand3D* GetLatErrorBand( const std::string& name );
			//! Get a const pointer to this MULatErrorBand
			const MULatErrorBand3D* GetLatErrorBand( const std::string& name ) const;

			/*! Get a new TH3D which is the central value histogram with statistical error only
				@return copy of a TH3D
				*/
			TH3D GetCVHistoWithStatError() const;

			/*! Get a new TH3D which is the central value histogram with errors added in quadrature
				@todo add param errNames to add only certain errors from these error bands.
				@return copy of a TH3D
				*/
			TH3D GetCVHistoWithError( bool includeStat = true, bool cov_area_normalize = false ) const;
			//    TH1D GetCVHistoWithError( const std::vector<std::string>& errNames ) const;

			//! Get a TH3D filled with the errors summed in quadrature
			TH3D GetTotalError( bool includeStat = true, bool asFrac = false , bool cov_area_normalize = false ) const;

			//! Get a TH3D filled with the stat error ONLY
			TH3D GetStatError( bool asFrac = false ) const;

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

			/*! Replace this MUH3D's contents with the result of a division of two other MUH3Ds
			*/
			virtual void Divide( const MUH3D* h1, const MUH3D* h2, Double_t c1 = 1, Double_t c2 = 1, Option_t* option="" );

			/*! Replace this MUH3D's contents with the result of dividing an MUH3D and all its errors by a TH3
			*/
			virtual void DivideSingle( const MUH3D* h1, const TH3* h2, Double_t c1 = 1, Double_t c2 = 1, Option_t* option="" );

			//! Add Histograms
			virtual void Add( const TH3* h1, const Double_t c1 = 1. );

			//! Replace this MUH3D's contents with the result of a multiplication of two other MUH3Ds
			virtual void Multiply( const MUH3D* h1, const MUH3D* h2, const Double_t c1 = 1., const Double_t c2 = 1. );

			//! Get the default bin width to which bin content/error is normalized
			inline Double_t GetNormBinWidthX() const { return fNormBinWidthX; };
			inline Double_t GetNormBinWidthY() const { return fNormBinWidthY; };
			inline Double_t GetNormBinWidthZ() const { return fNormBinWidthZ; };

			//! Set the default bin width to which bin content/error is normalized
			inline void     SetNormBinWidthX(const Double_t x ) { fNormBinWidthX = x; };
			inline void     SetNormBinWidthY(const Double_t y ) { fNormBinWidthY = y; };
			inline void     SetNormBinWidthZ(const Double_t z ) { fNormBinWidthZ = z; };

			//!Destructor (note: root cleans up histograms for us)
			virtual ~MUH3D() {};

		private:
			//! A helper function to check if this string has that ending
			bool HasEnding (std::string const &fullString, std::string const &ending) const;

			//! Stores a map from name of Systematics Error Matrices 
			std::map<std::string, TMatrixD*> fSysErrorMatrix;

			//! Stores a map from name of removed Systematic Error Matrices
			std::map<std::string, TMatrixD*> fRemovedSysErrorMatrix;

			//! Strores a map from name to error band for MULatErrorBands
			std::map<std::string, MULatErrorBand3D*> fLatErrorBandMap;

			//! Strores a map from name to error band for MUVertErrorBands
			std::map<std::string, MUVertErrorBand3D*> fVertErrorBandMap;

			//! Stores the width to which we will normalize bins (e.g. n Events per fNormBinWidth GeV)
			//! If negative, then refuse to normalize to bin width (appropriate for ratios, efficiencies)
			Double_t fNormBinWidthX;
			Double_t fNormBinWidthY;
			Double_t fNormBinWidthZ;

			//!define a class named MUH3D, at version 1
			ClassDef(MUH3D, 1); //MINERvA 2-D histogram

	}; //end of MUH3D

}//end PlotUtils

#endif
