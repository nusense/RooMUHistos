RooMUHistos
===========

Multiple-universe histograms based on ROOT.

Build
===========

There is an environment-setting  script in the home area that looks for a ROOT 
configuration script in the user `$HOME` area. If it doesn't find it, it complains. 
If it gets past this, it adds the RooMUHistos library directories to the `$LD_LIBRARY_PATH`
or `$DYLD_LIBRARY_PATH` depending on OS.

Once the environment is configured, run `make` in the `PlotUtils` directory
and then run `make` in the macros directory. Then test the build with 
`macros/tryToRead` and `macros/tryToWrite`.

To-Do
===========

Note: for the to-do's, feel free to add issues to the issue tracker. 
