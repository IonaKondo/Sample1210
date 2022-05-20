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
(Conduct a reference pixel correction for three raw images, respectively)
./Reference_pixel_correction_final -input ./tests/input/make_fits11.fit  -output ../tests/output/output_rc11   -file param-subframe-PRMIE.par -Rmode 1 1 
./Reference_pixel_correction_final -input ./tests/input/make_fits12.fit  -output ../tests/output/output_rc12   -file params/param-subframe-PRMIE.par -Rmode 1 1 
./Reference_pixel_correction_final -input ./tests/input/make_fits13.fit  -output ../tests/output/output_rc13   -file params/param-subframe-PRMIE.par -Rmode 1 1 

(Conduct sampling up-the-ramp using three raw images and obtain one resultant image)
./Up-the-Ramp_final_icc -input ../tests/output/output_rc11 ../tests/output/output_rc12 ../tests/output/output_rc13 -output ../tests/output/output_ramp11 
```
