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
#   Creates an HTML document summarizing all Idfs in a directory.
#
# == Usage
#
#   ruby SummarizeIdfs.rb ARGV[0] ARGV[1]
# 
#   ARGV[0] - Path to directory with EnergyPlus Idf files directory.
#   ARGV[1] - Optional path and filename for output html. Default is 
#             to place summary.html in IDF directory.
#
# == Examples
#
#   ruby ValidateIdfs.rb 'C:\path\to\energyplus\dir\' 'C:\path\to\desktop\idfSummary.html'
#
######################################################################

require 'openstudio'

Dir.chdir(ARGV[0])

logFile = OpenStudio::FileLogSink.new(OpenStudio::Path.new("SummarizeIdfs.log"))
logFile.setLogLevel(OpenStudio::Info)

summaryTable = OpenStudio::Table.new
row = []
row.push("Filename")
row.push("No. Objects")
row.push("No. Model Objects")
row.push("No. Objects After Roundtrip")
row.push("Building Name")
row.push("No. Thermal Zones")
row.push("No. Surfaces")
row.push("No. SubSurfaces")
row.push("No. ShadingSurfaces")
row.push("Idf Draft Valid")
row.push("Idf Final Valid")
row.push("Roundtrip Draft Valid")
row.push("Roundtrip Final Valid")
summaryTable.appendRow(row)
summaryTable.setNHead(1)

count = 0

individualReportHeaders = []
numIndividualReportsPerMainHeader = []
individualReports = []

Dir.glob("*.idf").each do |file|

  # we don't handle parametric preprocessor
  next if /1ZoneParameterAspect.idf/.match(file)
  
  puts ""
  puts file
  puts ""
  
  OpenStudio::logFree(OpenStudio::Info,"openstudio.ruby.SummarizeIdfs","Processing " + file)

  count += 1

  # import file as Workspace
  workspace = OpenStudio::Workspace::load(OpenStudio::Path.new(file)).get
  
  # translate to Model
  translator = OpenStudio::EnergyPlus::ReverseTranslator.new
  model = translator.translateWorkspace(workspace)
  
  # report stats
  bldg = model.getBuilding # force construction if doesn't exist
  zones = model.getThermalZones
  surfaces = model.getSurfaces
  subSurfaces = model.getSubSurfaces
  shadingSurfaces = model.getShadingSurfaces
  fwdTranslator = OpenStudio::EnergyPlus::ForwardTranslator.new()
  rtWorkspace = fwdTranslator.translateModel(model)
  row = []
  row.push(file)
  row.push(workspace.numObjects.to_s)
  row.push(model.numObjects.to_s)
  row.push(rtWorkspace.numObjects.to_s)
  row.push(bldg.name().get())
  row.push(zones.size.to_s)
  row.push(surfaces.size.to_s)
  row.push(subSurfaces.size.to_s)
  row.push(shadingSurfaces.size.to_s)
  
  # check validity, including writing out draft level reports
  idfReport = workspace.validityReport("Draft".to_StrictnessLevel)
  idfFinal = workspace.isValid("Final".to_StrictnessLevel)
  rtReport = rtWorkspace.validityReport("Draft".to_StrictnessLevel)
  rtFinal = rtWorkspace.isValid("Final".to_StrictnessLevel)
  
  row.push((idfReport.numErrors == 0).to_s)
  row.push(idfFinal.to_s)
  row.push((rtReport.numErrors == 0).to_s)
  row.push(rtFinal.to_s)
  summaryTable.appendRow(row)
  
  if (idfReport.numErrors > 0) || (rtReport.numErrors > 0)
  
    individualReportHeaders.push(OpenStudio::formatUnderscore("Draft-Level Validity Reports for " + file))
    numIndividualReportsPerMainHeader.push(0)
    
    if (idfReport.numErrors > 0) 
      numIndividualReportsPerMainHeader[numIndividualReportsPerMainHeader.size-1] += 1
      individualReportHeaders.push(OpenStudio::formatUnderscore("Report for Pure Import"))
      individualReports.push(OpenStudio::Text.new(idfReport.to_s))
    end
    
    if (rtReport.numErrors > 0) 
      numIndividualReportsPerMainHeader[numIndividualReportsPerMainHeader.size-1] += 1
      individualReportHeaders.push(OpenStudio::formatUnderscore("Report after Roundtrip through OpenStudio"))
      individualReports.push(OpenStudio::Text.new(rtReport.to_s))
    end
  
  end
  
end

# create document
authors = []
authors.push("Autogenerated by SummarizeIdfs.rb")
doc = OpenStudio::Document.new("Summary of Idf Files in Directory " + ARGV[0],
                               authors,
                               "XHTML".to_DocumentFormat,
                               OpenStudio::basicXHTMLHeader(),
                               OpenStudio::basicXHTMLFooter())
doc.setTopHeadingLevel(1)                            
doc.append(OpenStudio::Text.new("The following table summarizes the " + count.to_s + " IDF files in directory " + ARGV[0] + "."))

# underscores in filenames should be treated as such (not subscripts)
for i in summaryTable.nHead()..(summaryTable.nRows()-1)
  summaryTable.setElement(i,0,OpenStudio::formatUnderscore(summaryTable[i,0].getString))
end

doc.append(summaryTable)

headerIndex = 0
reportIndex = 0
for i in 0..(numIndividualReportsPerMainHeader.size-1)

  section = doc.appendSection(individualReportHeaders[headerIndex])
  headerIndex += 1
  
  for j in 1..(numIndividualReportsPerMainHeader[i])
    section = doc.appendSection(individualReportHeaders[headerIndex])
    headerIndex += 1
    section.append(individualReports[reportIndex])
    reportIndex += 1
  end

end

# write html
out_file = OpenStudio::Path.new("summary.html")
if ARGV.size() > 1
  out_file = OpenStudio::Path.new(ARGV[1])
end
f = File.new(out_file.to_s,"w")
f << doc.print()
f.close()
