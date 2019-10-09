Pull request overview
---------------------

 - Fixes #ISSUENUMBERHERE (IF THIS IS A DEFECT)
 - DESCRIBE PURPOSE OF THIS PULL REQUEST

Please read [OpenStudio Pull Requests](https://github.com/NREL/OpenStudio/wiki/OpenStudio-Pull-Requests) to better understand the OpenStudio Pull Request protocol.

### Pull Request Author

Add to this list or remove from it as applicable.  This is a simple templated set of guidelines.

 - [ ] Model API Changes
 - [ ] Any new or modified fields have been implemented in the EnergyPlus ForwardTranslator (and ReverseTranslator as appropriate)
 - [ ] Model API methods are tested (in `src/model/test`)
 - [ ] EnergyPlus ForwardTranslator Tests (in `src/energyplus/Test`
 - [ ] If a new object or method, added a test in NREL/OpenStudio-resources: Add Link
 - [ ] If needed, added VersionTranslation rules for the objects (`src/osversion/VersionTranslator.cpp`)
 - [ ] Checked behavior in OpenStudioApplication, adjusted policies as needed (`src/openstudio_lib/library/OpenStudioPolicy.xml`)
 - [ ] All new and existing tests passes
 - [ ] If methods have been deprecated, update rest of code to use the new methods

**Labels:**

 - [ ] If change to an IDD file, add the label `IDDChange`
 - [ ] If breaking existing API, add the label `APIChange`
 - [ ] If deemed ready, add label `Pull Request - Ready for CI` so that CI builds your PR

### Review Checklist

This will not be exhaustively relevant to every PR.
 - [ ] Perform a Code Review on GitHub
 - [ ] Code Style, strip trailing whitespace, etc.
 - [ ] All related changes have been implemented: model changes, model tests, FT changes, FT tests, VersionTranslation, OS App
 - [ ] Labeling is ok
 - [ ] If defect, verify by running develop branch and reproducing defect, then running PR and reproducing fix
 - [ ] If feature, test running new feature, try creative ways to break it
 - [ ] CI status: all green or justified
 - [ ] Run Unit Test(s) locally
