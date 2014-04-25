#ifndef MNV_MULatErrorBand3D_H
#define MNV_MULatErrorBand3D_H 1

#include "TObject.h"
#include "TString.h"
#include "TError.h"
#include "TNamed.h"
#include "TH3D.h"
#include "TMatrixD.h"
#include "TMatrixDBase.h"

#include <assert.h>
#include <vector>
#include <string>
#include <iostream>
#include <math.h>

namespace PlotUtils
{

	class MULatErrorBand3D : public TH3D
	{
		public:
			//! Default constructor 
			MULatErrorBand3D( ) : TH3D() {};

			MULatErrorBand3D( const std::string& name, const TH3D* base, const unsigned int nHists = 1000 );

			//! Add a new Constructor for already Filled vector of Histogram Error Bands
			MULatErrorBand3D( const std::string& name, const TH3D* base, const std::vector<TH3D*>& hists );

			//! Deep copy constructor
			MULatErrorBand3D( const MULatErrorBand3D& h );

			//! Deep assignment operator
			MULatErrorBand3D& operator=( const MULatErrorBand3D& h );

		private:
			//! A helper function which sets variables for the deep copy and assignment
			void DeepCopy( const MULatErrorBand3D& h );

		public:
			/*! Quiet warnings about hidden overloaded virtual functions.
				It is not clear if this is the behavior we want (may prefer
				to ``not use'' these.
				*/ 
			using TH3::Add;
			using TH3::Divide; 
			using TH3::Fill;
			using TH3::Multiply;

			//!Destructor (note: root cleans up histograms for us)
			virtual ~MULatErrorBand3D() {};

			//! Fill the CV histo and all the universes' histos
			virtual bool Fill( const double xval, const double yval, const double zval, const double *xshifts, const double *yshifts, const double *zshifts, const double cvweight = 1.0, const bool fillcv = true, const double* weights = NULL );

			virtual bool Fill( const double xval, const double yval, const double zval, const double xshiftDown, const double xshiftUp, const double yshiftDown, const double yshiftUp, const double zshiftDown, const double zshiftUp, const double cvweight = 1.0, const bool fillcv = true );

			//! Get the error band histogram
			virtual TH3D GetErrorBand(bool asFrac = false , bool cov_area_normalize = false) const;

			//! Will the error band come from max spread?
			bool GetUseSpreadError() const { return fUseSpreadError; };

			//! Set the error band to come from max spread or standard dev
			void SetUseSpreadError( bool use ) { fUseSpreadError = use; };

			//! How many histograms (universes) does this have?
			unsigned int GetNHists() const { return fNHists; };

			//! Get the universes' histograms (const)
			const std::vector<TH3D*>& GetHists() const { return fHists; };

			//! Get a specific universe's histogram (const)
			const TH3D* GetHist(const unsigned int i) const;

			//! Get a specific universe's histogram (nonconst)
			TH3D* GetHist(const unsigned int i);

			//! Get the universes' histograms (nonconst)
			std::vector<TH3D*> GetHists() { return fHists; };

			//! Calculate Covariance Matrix
			TMatrixD CalcCovMx(bool area_normalize = false, bool asFrac = false) const;

			//! Add h1*c1 to this error band
			Bool_t Add( const MULatErrorBand3D* h1, const Double_t c1 = 1. );

			//! Replace this MULatErrorBand3D's contents with the result of a multiplication of two other MULatErrorBands3D
			Bool_t Multiply( const MULatErrorBand3D* h1, const MULatErrorBand3D* h2, Double_t c1 = 1, Double_t c2 = 1 );

			/*! Replace this MULatErrorBand3D's contents with the result of a division of one MULatErrorBand3D by a TH3
			*/
			Bool_t DivideSingle( const MULatErrorBand3D* h1, const TH3* h2, Double_t c1 = 1, Double_t c2 = 1, Option_t* option="" );

			/*! Replace this MULatErrorBand3D's contents with the result of a division of two other MULatErrorBand3Ds
			*/
			Bool_t Divide( const MULatErrorBand3D* h1, const MULatErrorBand3D* h2, Double_t c1 = 1, Double_t c2 = 1, Option_t* option="" );

			//! Scale all universes by some constant using TH3D::Scale
			void Scale( Double_t c1 = 1., Option_t *option = "" );


		protected:
			unsigned int fNHists;         ///< Number of histograms (universes)
			bool fUseSpreadError;         ///< Are we using spread in histos to get the error
			std::vector<TH3D*> fHists;    ///< Vector of histograms for the universes
			std::vector<int> fGoodColors; ///< Current list of good colors to use for universe histos

		private:
			//!define a class named MULatErrorBand3D, at version 1 
			ClassDef( MULatErrorBand3D, 1 );
	}; //end of MULatErrorBand3D

} //end of PlotUtils

#endif
