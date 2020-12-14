# Up-Ramp-Group Sampling Code

This code is a C++ implementation of the Offenberg et al 2005 algorithim to perform memory efficent up the ramp group sampling.

## Compilation
This code depends on CFITSIO. Please ensure that CFITSIO is installed to `/usr/local/`.

To compile code with set of sample programs:

```
 cd src
 make tests
```

## Usage
To run a sample program on test data, execute:
```
cd src
./SampleProgram1_20201209 -frame ../tests/input/make_fits11.fits ../tests/input/make_fits12.fits ../tests/input/make_fits13.fits
./SampleProgram4_20201212 -frame ../tests/input/make_fits11.fits ../tests/input/make_fits12.fits ../tests/input/make_fits13.fits -out ../tests/output/output4.fits

```
