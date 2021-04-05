# clock 0.1.1

* Errors resulting from invalid dates or nonexistent/ambiguous times are now
  a little nicer to read through the usage of an info bullet (#200).

* Fixed a Solaris ambiguous behavior issue from calling `pow(int, int)`.

* Linking against cpp11 0.2.7 is now required to fix a rare memory leak issue.

# clock 0.1.0

* Added a `NEWS.md` file to track changes to the package.
