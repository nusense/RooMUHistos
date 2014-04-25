RooMUHistos
===========

Multiple-universe histograms based on ROOT.

Build
===========

Currently, the build works only on Mac OSX. There is an environment-setting 
script in the home area that looks for a ROOT configuration script in the 
user `$HOME` area. If it doesn't find it, it complains. If it gets past this, 
it adds the RooMUHistos library directories to the `$LD_LIBRARY_PATH` or 
`$DYLD_LIBRARY_PATH` depending on OS.

Once the environment is configured, run `make` in the `PlotUtils` directory
and then run `make` in the macros directory. Then test the build with 
`macros/tryToRead` and `macros/tryToWrite`.

To-Do
===========

1. Build on Scientific Linux!
2. Figure out what is missing from the port from Minerva software that is important.
3. Bring in anything from Minerva software that is important.
4. Think about unfolding and other Minerva analysis packages that might be useful.
5. Add lots of other good items to this list and make the package fantastic.

