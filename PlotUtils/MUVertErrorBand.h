#ifndef MNV_MUVertErrorBand_H
#define MNV_MUVertErrorBand_H 1

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

	/*! @brief Create a systematic error band and covariance matrix using the many universes method where universes differ in a vertical weight amount.

		By default the error in each bin is taken to be:
		<ul>
		<li>the maximum among universes (including central value) for small numbers of universes ( nUniverses < 10 ).
		<li>The standard deviation among universes for large numbers of universes (nUniverses >= 10 )
		</ul>
		Normally the vertical shift has many universes build with random Gaussian devitions about the central value.

		@author Brian Tice, Gabriel Perdue
		*/
	class MUVertErrorBand : public TH1D
	{
		public:
			//! Default constructor 
			MUVertErrorBand( ) : TH1D() {};

			/*! Standard constructor 
				@param[in] name Name the error band
				@param[in] base Pointer to histogram we are applying an error band to
				@param[in] nHists Number of universes.  Default=1000, expected to represent universes with Gaussian deviations about the central value
				*/
			MUVertErrorBand( const std::string& name, const TH1D* base, const unsigned int nHists = 1000 );

			//! Add a new Constructor for already Filled vector of Histogram Error Bands
			MUVertErrorBand( const std::string& name, const TH1D* base, const std::vector<TH1D*>& hists );

			//! Deep copy constructor
			MUVertErrorBand( const MUVertErrorBand& h );

			//! Deep assignment operator
			MUVertErrorBand& operator=( const MUVertErrorBand& h );

		private:
			//! A helper function which sets variables for the deep copy and assignment
			void DeepCopy( const MUVertErrorBand& h );

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
			virtual ~MUVertErrorBand() {};

			//! Fill the CV histo and all the universes' histos
			virtual Int_t Fill( const double val, const double *weights, const double cvweight = 1., double cvWeightFromMe = 1.);

			/*! Fill the CV histo and 2 universes with these weights
				Only works if nUniverses = 2 (assumed to be +/- nSigma)
				@param[in] val Central value to fill
				@param[in] shiftDown Weight of this event reweighted for a -nSigma variation
				@param[in] shiftUp Weight of this event reweighted for a  +nSigma variation
				@param[in] cvweight Central value weight
				@param[in] cvWeightFromMe If this error source contributed to creating the CVWeight (e.g. NA49), then we need to know how much was contributed to avoid double counting.  
				*/
			virtual Int_t Fill( const double val, const double weightDown, const double weightUp, const double cvweight = 1.0, double cvWeightFromMe = 1. );

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

			//! Get a specific universe's histogram (const)
			const TH1D* GetHist(const unsigned int i) const;

			//! Get a specific universe's histogram (nonconst)
			TH1D* GetHist(const unsigned int i);

			//! Get the universes' histograms (nonconst)
			std::vector<TH1D*> GetHists() { return fHists; };
		public:

			//! Draw all the histograms, including CVHist if the option is present
			void DrawAll( const char *option = "", bool drawCV = false, bool area_normalize = false, double normBinWidth = 0.0 ) const;

			//! Scale all universes by some constant using TH1D::Scale
			void Scale( Double_t c1 = 1., Option_t *option = "" );

			/*! Replace this MUVertErrorBand's contents with the result of a division of two other MUVertErrorBands
				see http://root.cern.ch/root/html/TH1.html#TH1:Divide@2
				*/
			Bool_t Divide( const MUVertErrorBand* h1, const MUVertErrorBand* h2, Double_t c1 = 1, Double_t c2 = 1, Option_t* option="" );

			/*! Replace this MULatErrorBand's contents with the result of a division of one MULatErrorBand by a TH1
			*/
			Bool_t DivideSingle( const MUVertErrorBand* h1, const TH1* h2, Double_t c1 = 1, Double_t c2 = 1, Option_t* option="" );

			//! Replace this MUVertErrorBand's contents with the result of a multiplication of two other MUVertErrorBands
			Bool_t Multiply( const MUVertErrorBand* h1, const MUVertErrorBand* h2, Double_t c1 = 1, Double_t c2 = 1 );

			//! Replace this MUVertErrorBand's contents with the result of a multiplication of another MULatErrorband and a TH1
			Bool_t MultiplySingle( const MUVertErrorBand* h1, const TH1* h2, Double_t c1 = 1, Double_t c2 = 1 );


			//! Add a histogram to all universes
			Bool_t AddSingle( const TH1* h1, const Double_t c1 = 1. );

			//! Add h1*c1 to this error band
			Bool_t Add( const MUVertErrorBand* h1, const Double_t c1 = 1. );

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
			//!define a class named MUVertErrorBand, at version 3 
			ClassDef( MUVertErrorBand, 3 ); //Create a systematic error band and covariance matrix using the many universes method where universes are defined by different weights
	}; //end of MUVertErrorBand

} //end of PlotUtils

#endif
