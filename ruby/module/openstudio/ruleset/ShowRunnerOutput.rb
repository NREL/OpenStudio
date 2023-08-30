########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "openstudio/ruleset/ShowRunnerOutput is deprecated, use openstudio/measure/ShowRunnerOutput instead.")

require 'openstudio/measure/ShowRunnerOutput'
