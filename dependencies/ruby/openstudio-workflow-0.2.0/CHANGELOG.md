OpenStudio::Workflow Change Log
==================================

Version 0.2.0
------------------
* Use EnergyPlus that is packaged with OpenStudio. The branch of this repo is no longer tied to the version of EnergyPlus
* Update find_energyplus to use new OpenStudio method instead of RUBYLIB env var
* Update dependencies
* Support tests running on Docker
* Skip the PAT tests for now since they are broken
* Remove snake_case method as it conflicts with another libraries method, now called to_underscore
* Add test for persisting JSONs as reports. Rename reports in the form of <measure_name_underscored>_<report_name>.<ext>
* Always add in Monthly Reports (Deprecate the :run_monthly_reports option)
* Remove the old Standard Reports and use the version in the OpenStudio Release (along with the calibration report)

Version 0.1.1
------------------
* Catch exception when loading a measure file.
* Enable running simulations on windows and mac
  * Use rubyzip gem instead of system call
  * Fix the double directory which caused the zip files to behave strangely on windows
  * New find_energyplus method which tries to use OpenStudio's version of EnergyPlus
* Copy all EnergyPlus files into run directory
* Better cleanup after EnergyPlus simulation
* Read machine information from /etc/openstudio-server directory, if available

Version 0.1.0
-------------
* Tests for programmatically creating the analysis.json files from the OpenStudio-analysis-gem
* Upgrade to EnergyPlus 8.2. Right now the run energyplus and run runmanager job hard code these paths.
* Upgrade and fix Facter facts to grab the correct ip address when running on EC2

Version 0.0.4
-------------
* Include rubyXL gem for reading/writing MS Excel files
* Remove invalid characters from OpenStudio Measure Attributes. /[|!@#\$%^&\*\(\)\{\}\\\[\]|;:'",<.>\/?\+=]+/
* Fix objective functions to read from any of the results hash, not just the standard report legacy
* Add time logger and reporting measure

Version 0.0.3
--------------
* Allow measures to set weather file in a measure and have it update what EnergyPlus uses for the weather file.
* OpenStudio::Workflow.run_energyplus method added to just run energyplus.
* Remove AASM (act as state machine) and replace with simple tracking of the state. Interface is the same except there is now no need to pass in the States.
* Catch EnergyPlus errors (non-zero exits), Fatal Errors in eplusout.err, and invalid weather files.
* Force UTF-8 version upon reading the eplusout.err file

Version 0.0.2
--------------

* Support reporting measures
* Reduce logging messages
* Keep IDF files
* Remove mtr and eso files after simulation completes
* If measure changes weather file, then use the new weather file with the analysis.json weather path

Version 0.0.1
--------------

* Initial release with basic workflow implemented to support running OpenStudio measure-based workflows
