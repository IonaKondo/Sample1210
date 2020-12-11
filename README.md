# Up-Ramp-Group Sampling Code

This code is a C++ implementation of the Offenberg et al 2005 algorithim to perform memory efficent up the ramp group sampling.

## Compilation
This code depends on CFISTIO. Please ensure that CFITSIO is installed to /usr/local.

To compile code:

```
 cd src
 make Up-the-Ramp_test
```

## Usage
To run the program on test data, execute:
```
./Up-the-Ramp_test -frame make_fits11.fits make_fits12.fits make_fits13.fits

```