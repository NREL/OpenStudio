######################################################################
#  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

######################################################################
# == Synopsis 
#
#   For the given IDF and EPW files, ensures that a SQL file will be 
#   generated, runs the EnergyPlus simulation, places select output 
#   files in outputDirectory and launches ResultsViewer. If no 
#   outputDirectory is specified, places the output files in 
#   idfPathAndFileName's parent directory.
#
# == Usage
#
#   ruby ApplyStandardsRuleset.rb ARGV[0] ARGV[1] ARGV[2]
#     
#     ARGV[0] - ruleset directory (will glob .csv files)
#     ARGV[1] - model file (must be osm, will search for eplusout.sql or 
#               [model_name].sql in subdirectories of parent folder)
#     ARGV[2] - (optional) output file name
#
######################################################################

require 'openstudio'

debug = false

# Load Ruleset
rulesetDir = OpenStudio::Path.new(ARGV[0])
ruleFilenames = Dir.glob(rulesetDir.to_s + "./**/*.csv")
rulesetName = OpenStudio::toString(rulesetDir.stem)
rulesetName = OpenStudio::toString(rulesetDir.parent_path.stem) if rulesetName.size < 3
puts "Creating " + rulesetName + " Ruleset"
ruleset = OpenStudio::Ruleset::StandardsRuleset.new(rulesetName)
ruleFilenames.each { |ruleFilename|
  ruleFilepath = OpenStudio::Path.new(ruleFilename)
  rules = OpenStudio::Ruleset::loadStandardsAttributeCheckRulesFromFile(ruleFilepath)
  puts "Loaded " + rules.size.to_s + " rules from '" + ruleFilename + "'."
  rules.each { |rule| ruleset.add(rule) }
}

# Load Model
modelFilepath = OpenStudio::Path.new(ARGV[1])
puts "Loading model '" + modelFilepath.to_s + "'."
oModel = OpenStudio::Model::Model::load(modelFilepath)
raise "Unable to open model file '" + modelFilepath.to_s + "'." if oModel.empty?
model = oModel.get
sqlFilenames = Dir.glob(modelFilepath.parent_path.to_s + "./**/*.sql")
if not sqlFilenames.empty?
  sqlFilepath = OpenStudio::Path.new
  sqlFilenames.each { |candidate|
    candidateFilepath = OpenStudio::Path.new(candidate)
    if (sqlFilepath.empty) || 
       (candidateFilepath.stem.to_s == "eplusout") ||
       ((candidateFilepath.stem == modelFilepath.stem) && (not (sqlFilepath.stem == modelFilepath.stem)))
      sqlFilepath = candidateFilepath
    end
    if (sqlFilepath.stem.to_s == "eplusout")
      break
    end
  }  
  puts "Opening SqlFile '" + sqlFilepath.to_s + "'."
  begin
    sqlFile = OpenStudio::SqlFile.new(sqlFilepath)
    model.setSqlFile(sqlFile)
  rescue
    warn "Unable to open SqlFile '" + sqlFilepath.to_s + "'. Will apply ruleset to input-only model."
  end
else
  warn "Unable to locate EnergyPlus output sql file for model '" + modelFilepath.to_s + "'."
end

# Apply Ruleset to Model
engine = OpenStudio::Rulesengine::OSRulesEngine.new
wrappedModel = OpenStudio::StandardsInterface::OSStandardsInterface.new(model)
myOptions = OpenStudio::Ruleset::StandardsRulesetOptionVector.new
ruleset.options.each { |option|
  clonedOption = option.clone.to_StandardsRulesetOption.get
  myOptions.push(clonedOption)
}
engine.applyRules(ruleset,myOptions,wrappedModel)

# Write Out Report to File
report = engine.result.report
uuidString = OpenStudio::checksum(engine.currentUuid.to_s)
outFilepath = modelFilepath.parent_path / OpenStudio::Path.new("ApplyStandardsRuleset_Report_" + uuidString + ".html")
outFilepath = OpenStudio::Path.new(ARGV[2]) if ARGV.size > 2
puts "Writing output report to '" + outFilepath.to_s + "'."
report.setFormat("XHTML".to_DocumentFormat,OpenStudio::basicXHTMLHeader,OpenStudio::basicXHTMLFooter)
report.save(outFilepath)

# Print Out Result to Screen
report.setFormat("COUT".to_DocumentFormat)
puts report.print

# Print Ruleset Out to File for Debugging
if debug
  rulesetDocument = OpenStudio::Document.new(ruleset.name + " Ruleset")
  rulesetMainSection = rulesetDocument.appendSection("Loaded Ruleset")
  text = OpenStudio::Text.new(OpenStudio::formatUnderscore(ruleset.print("Engine".to_StandardsRulesetPrintMode)))
  rulesetMainSection.append(text)
  rulesetInternalsSection = rulesetDocument.appendSection("RulesEngine Internal Constructs")
  text = OpenStudio::Text.new(OpenStudio::formatUnderscore(engine.printInternalConstructs(myOptions)))
  rulesetInternalsSection.append(text)
  outFilepath = outFilepath.parent_path / OpenStudio::Path.new("RulesetPrintOut_" + uuidString + ".html")
  rulesetDocument.setFormat("XHTML".to_DocumentFormat,OpenStudio::basicXHTMLHeader,OpenStudio::basicXHTMLFooter)
  rulesetDocument.save(outFilepath)
end
