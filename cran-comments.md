## R CMD check results

0 errors | 0 warnings | 0 notes

## revdepcheck results

We checked 2 reverse dependencies, comparing R CMD check results across CRAN and dev versions of this package.

 * We saw 0 new problems
 * We failed to check 0 packages
 
## 0.4.1 Submission

This is a patch release to support upcoming changes in testthat.
 
## 0.4.0 Submission

This is a minor release containing a few new functions: `date_start()`, `date_end()`, and `invalid_remove()`. It also requires tzdb 0.1.2 to hopefully fix a compilation issue on RHEL7/Centos machines.

## 0.3.1 Submission

This is a patch release mainly intended to support an upcoming breaking change in testthat.

## 0.3.0 Submission

This is a minor release that switches clock to use the date API exposed by tzdb. It also adds a new sequence generation function, `date_seq()`.

## 0.2.0 Submission

This is a minor release to fix the error on CRAN's Solaris build, and add a few new features.

## 0.1.0 Submission

This is the first release of clock. There are no references that I would like to include with the package. The installed size of clock may be large on some platforms due to the large amount of templated C++ code included with the package.
