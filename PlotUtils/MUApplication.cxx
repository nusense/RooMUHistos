#include "PlotUtils/MUApplication.h"

void PlotUtils::Initialize()
{
	//! Enable cintex to get reflex powers with PlotUtils (necessary for I/O and various other ROOT happenings)
#ifdef ROOT5
	ROOT::Cintex::Cintex::Enable();
#endif
}
