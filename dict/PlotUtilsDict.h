#ifndef PLOTUTILSDICT_H 
#define PLOTUTILSDICT_H 1

// Include files for PlotUtils dictionary.

/** @file PlotUtilsDict.h
 *  
 *
 *  @author Jeremy Wolcott <jwolcott@fnal.gov>
 *  @date   2012-11-25
 */
// ============================================================================
// PlotUtils
// ============================================================================

// here we need to include all the header files
// for the classes we want to make dictionaries of

#include <vector>

#include "../PlotUtils/MUH1D.h" 
#include "../PlotUtils/MUH2D.h" 
#include "../PlotUtils/MUH3D.h" 
#include "../PlotUtils/MULatErrorBand.h"
#include "../PlotUtils/MULatErrorBand2D.h"
#include "../PlotUtils/MULatErrorBand3D.h"
#include "../PlotUtils/MUPlotter.h"
#include "../PlotUtils/MUVertErrorBand.h"
#include "../PlotUtils/MUVertErrorBand2D.h"
#include "../PlotUtils/MUVertErrorBand3D.h"

// this garbage is necessary so that gccxml is able to create dictionaries for these custom containers
// see: http://root.cern.ch/root/roottalk/roottalk10/0035.html
// somehow std::map<>s seem to be instantiated somewhere else, so explicit instantiation is not necessary?
#ifdef __GCCXML__
template class std::vector<PlotUtils::MUEVD::Event>;                                       // the 'Events' typedef
template class std::pair<std::string, std::vector<PlotUtils::MUEVD::Event> >;              // the 'EventGroup' typedef
#endif

#endif // PLOTUTILSDICT_H

