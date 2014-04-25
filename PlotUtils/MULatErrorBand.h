#ifndef MNV_MULatErrorBand_H
#define MNV_MULatErrorBand_H 1

#include "TObject.h"
#include "TString.h"
#include "TError.h"
#include "TNamed.h"
#include "TH1D.h"
#include "TMatrixD.h"
#include "TMatrixDBase.h"

#include <assert.h>
#include <vector>
#include <string>
#include <iostream>
#include <math.h>

namespace PlotUtils
{

	/*! @brief Create a systematic error band and covariance matrix using the many universes method where universes differ in a lateral shift amount.

		Error in each bin is taken to be:
		<ul>
		<li>the maximum among universes (including central value) for small numbers of universes ( nUniverses < 10 ).
		<li>The standard deviation among universes for large numbers of universes (nUniverses >= 10 )
		</ul>
		Normally the lateral shift has two universes for +/-1 sigma shifts.

		@author Brian Tice, Gabriel Perdue
		*/

	class MULatErrorBand : public TH1D
	{
		public:
			//! Default constructor
			MULatErrorBand( ) : TH1D() {};

			//==== Copy Constructors from TH1D ====//
			//! Construct from vector 
			/*
				 MULatErrorBand( const TVectorD& v ) : TH1D(v) {};

			//! Copy constructor
			MULatErrorBand( const TH1D& h1d ) : TH1D(h1d) {};

			//! Construct with variable bin sizes
			MULatErrorBand( const char* name, const char* title, Int_t nbinsx, const Float_t* xbins): TH1D(name,title,nbinsx,xbins) {} ;

			//! Construct with variable bin sizes
			MULatErrorBand( const char* name, const char* title, Int_t nbinsx, const Double_t* xbins): TH1D(name,title,nbinsx,xbins) {} ;

			//! Construct with constant bin sizes
			MULatErrorBand( const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup) : TH1D(name,title,nbinsx,xlow,xup) {} ;
			*/


			/*! Standard Constructor 
				@param[in] name Name the error band
				@param[in] base Pointer to histogram we are applying an error band to
				@param[in] nHists Number of universes.  Default=2, expected to be +/-1 sigma from central value
				*/
			MULatErrorBand( const std::string& name, const TH1D* base, const unsigned int nHists = 2 );

			//! Add a new Constructor for already Filled vector of Histogram Error Bands
			MULatErrorBand( const std::string& name, const TH1D* base, const std::vector<TH1D*>& hists );

			//! Deep copy constructor
			MULatErrorBand( const MULatErrorBand& h );

			//! Deep assignment operator
			MULatErrorBand& operator=( const MULatErrorBand& h );

		private:
			//! A helper function which sets variables for the deep copy and assignment
			void DeepCopy( const MULatErrorBand& h );

		public:

			/*! Quiet warnings about hidden overloaded virtual functions.
				It is not clear if this is the behavior we want (may prefer
				to ``not use'' these.
				*/ 
			using TH1::Add;
			using TH1::Divide; 
			using TH1::Fill;
			using TH1::Multiply;

			//!Destructor (note: root cleans up histograms for us)
			virtual ~MULatErrorBand() {};

			//! Fill the CVHist and all the universes' histos
			virtual bool Fill( const double val, const double *shifts, const double cvweight = 1.0, const bool fillcv = true, const double *weights = 0 );

			/*! Fill the CV histo and 2 universes with these shifts
				Only works if nUniverses = 2 (assumed to be +/- nSigma)
				@param[in] val Central value to fill
				@param[in] shiftDown Distance from the cetral value for the symmetric -nSigma shift
				@param[in] shiftUp Distance from the cetral value for the symmetric +nSigma shift
				@param[in] cvweight Central value weight
				*/
			virtual bool Fill( const double val, const double shiftDown, const double shiftUpconst, double cvweight = 1.0, const bool fillcv = true );

			//! Get the error band histogram
			virtual TH1D GetErrorBand( bool asFrac = false , bool cov_area_normalize = false) const;

			//! Will the error band come from max spread?
			bool GetUseSpreadError() const { return fUseSpreadError; };

			//! Set the error band to come from max spread or standard dev
			void SetUseSpreadError( bool use ) { fUseSpreadError = use; };

			//! How many histograms (universes) does this have?
			unsigned int GetNHists() const { return fNHists; };

			//! Get the universes' histograms (const)
			const std::vector<TH1D*>& GetHists() const { return fHists; };
			//! Get the universes' histograms (nonconst)
			std::vector<TH1D*> GetHists() { return fHists; };

			//! Get a specific universe's histogram (const)
			const TH1D* GetHist(const unsigned int i) const;
			//! Get a specific universe's histogram (nonconst)
			TH1D* GetHist(const unsigned int i);

			//! Draw all the histograms, including CVHist if the option is present
			void DrawAll( const char *option = "", bool drawCV = false, bool area_normalize = false, double normBinWidth = 0.0 ) const;

			//! Scale all universes by some constant using TH1D::Scale
			void Scale( Double_t c1 = 1., Option_t *option = "" );

			/*! Replace this MULatErrorBand's contents with the result of a division of two other MULatErrorBands
				see http://root.cern.ch/root/html/TH1.html#TH1:Divide@2
				*/
			Bool_t Divide( const MULatErrorBand* h1, const MULatErrorBand* h2, Double_t c1 = 1, Double_t c2 = 1, Option_t* option="" );

			/*! Replace this MULatErrorBand's contents with the result of a division of one MULatErrorBand by a TH1
			*/
			Bool_t DivideSingle( const MULatErrorBand* h1, const TH1* h2, Double_t c1 = 1, Double_t c2 = 1, Option_t* option="" );

			//! Replace this MULatErrorBand's contents with the result of a multiplication of two other MULatErrorBands
			Bool_t Multiply( const MULatErrorBand* h1, const MULatErrorBand* h2, Double_t c1 = 1, Double_t c2 = 1 );

			//! Replace this MULatErrorBand's contents with the result of a multiplication of another MULatErrorband and a TH1
			Bool_t MultiplySingle( const MULatErrorBand* h1, const TH1* h2, Double_t c1 = 1, Double_t c2 = 1 );

			//! Add a histogram to all universes
			Bool_t AddSingle( const TH1* h1, const Double_t c1 = 1. );

			//! Add MULatErrorband*c1 to this error band
			Bool_t Add( const MULatErrorBand* h1, const Double_t c1 = 1. );

			//! Rebin this error band
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual" // if we ``use'' the base, the overload is ambiguous
			Bool_t Rebin( const int ngroup = 2 );
#pragma clang diagnostic pop

			//! Reset all histograms known to this error band
			void Reset( Option_t *option = "" );

			//! Set/Reset histogram bit and propagate to universes
			void SetBit(UInt_t f, Bool_t set);
			//! Set histogram bit and propagate to universes
			void SetBit(UInt_t f) { SetBit(f, true); };


			//! Calculate Covariance Matrix
			TMatrixD CalcCovMx(bool area_normalize = false, bool asFrac = false) const;

			//! Calculate Correlation Matrix
			TMatrixD CalcCorrMx(bool area_normalize = false) const;

		protected:
			unsigned int fNHists;         ///< Number of histograms (universes)
			bool fUseSpreadError;         ///< Are we using spread in histos to get the error
			std::vector<TH1D*> fHists;    ///< Vector of histograms for the universes
			std::vector<int> fGoodColors; ///< Current list of good colors to use for universe histos

		private:
			//!define a class named MULatErrorBand, at version 3 
			ClassDef( MULatErrorBand, 3); //Create a systematic error band and covariance matrix using the many universes method where universes differ in a lateral shift amount
	}; //end of MULatErrorBand

} //end of PlotUtils

#endif
