#ifndef MNV_MUVertErrorBand2D_H
#define MNV_MUVertErrorBand2D_H 1

#include "TObject.h"
#include "TString.h"
#include "TError.h"
#include "TNamed.h"
#include "TH2D.h"
#include "TMatrixD.h"
#include "TMatrixDBase.h"

#include <assert.h>
#include <vector>
#include <string>
#include <iostream>
#include <math.h>

namespace PlotUtils
{

	class MUVertErrorBand2D : public TH2D
	{
		public:
			//! Default constructor 
			MUVertErrorBand2D( ) : TH2D() {};

			MUVertErrorBand2D( const std::string& name, const TH2D* base, const unsigned int nHists = 1000 );

			//! Add a new Constructor for already Filled vector of Histogram Error Bands
			MUVertErrorBand2D( const std::string& name, const TH2D* base, const std::vector<TH2D*>& hists );

			//! Deep copy constructor
			MUVertErrorBand2D( const MUVertErrorBand2D& h );

			//! Deep assignment operator
			MUVertErrorBand2D& operator=( const MUVertErrorBand2D& h );

		private:
			//! A helper function which sets variables for the deep copy and assignment
			void DeepCopy( const MUVertErrorBand2D& h );

		public:
			/*! Quiet warnings about hidden overloaded virtual functions.
				It is not clear if this is the behavior we want (may prefer
				to ``not use'' these.
				*/ 
			using TH2::Add;
			using TH2::Divide; 
			using TH2::Fill;
			using TH2::Multiply;

			//!Destructor (note: root cleans up histograms for us)
			virtual ~MUVertErrorBand2D() {};

			//! Fill the CV histo and all the universes' histos
			virtual bool Fill( const double xval, const double yval, const double *weights, const double cvweight = 1, double cvWeightFromMe = 1. );

			virtual bool Fill( const double xval, const double yval, const double weightDown, const double weightUp, const double cvweight = 1.0, double cvWeightFromMe = 1. );

			//! Get the error band histogram
			virtual TH2D GetErrorBand(bool asFrac = false , bool cov_area_normalize = false) const;

			//! Will the error band come from max spread?
			bool GetUseSpreadError() const { return fUseSpreadError; };

			//! Set the error band to come from max spread or standard dev
			void SetUseSpreadError( bool use ) { fUseSpreadError = use; };

			//! How many histograms (universes) does this have?
			unsigned int GetNHists() const { return fNHists; };

			//! Get the universes' histograms (const)
			const std::vector<TH2D*>& GetHists() const { return fHists; };

			//! Get a specific universe's histogram (const)
			const TH2D* GetHist(const unsigned int i) const;

			//! Get a specific universe's histogram (nonconst)
			TH2D* GetHist(const unsigned int i);

			//! Get the universes' histograms (nonconst)
			std::vector<TH2D*> GetHists() { return fHists; };

			//! Calculate Covariance Matrix
			TMatrixD CalcCovMx(bool area_normalize = false, bool asFrac = false) const;

			//! Add h1*c1 to this error band
			Bool_t Add( const MUVertErrorBand2D* h1, const Double_t c1 = 1. );

			//! Replace this MUVertErrorBand2D's contents with the result of a multiplication of two other MUVertErrorBands2D
			Bool_t Multiply( const MUVertErrorBand2D* h1, const MUVertErrorBand2D* h2, Double_t c1 = 1, Double_t c2 = 1 );

			/*! Replace this MUVertErrorBand2D's contents with the result of a division of one MUVertErrorBand2D by a TH2
			*/
			Bool_t DivideSingle( const MUVertErrorBand2D* h1, const TH2* h2, Double_t c1 = 1, Double_t c2 = 1, Option_t* option="" );

			/*! Replace this MUVertErrorBand2D's contents with the result of a division of two other MUVertErrorBand2Ds
			*/
			Bool_t Divide( const MUVertErrorBand2D* h1, const MUVertErrorBand2D* h2, Double_t c1 = 1, Double_t c2 = 1, Option_t* option="" );

			//! Scale all universes by some constant using TH2D::Scale
			void Scale( Double_t c1 = 1., Option_t *option = "" );


		protected:
			unsigned int fNHists;         ///< Number of histograms (universes)
			bool fUseSpreadError;         ///< Are we using spread in histos to get the error
			std::vector<TH2D*> fHists;    ///< Vector of histograms for the universes
			std::vector<int> fGoodColors; ///< Current list of good colors to use for universe histos

		private:
			//!define a class named MUVertErrorBand2D, at version 1 
			ClassDef( MUVertErrorBand2D, 1 );
	}; //end of MUVertErrorBand2D

} //end of PlotUtils

#endif
