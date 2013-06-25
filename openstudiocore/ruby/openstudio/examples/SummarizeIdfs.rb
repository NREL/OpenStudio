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
#   ARGV[2] - Optional number of files to summarize. Default is to 
#             summarize all of them.
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
row = OpenStudio::TableRow.new
row << OpenStudio::TableElement.new("Filename")
row << OpenStudio::TableElement.new("No. Objects")
row << OpenStudio::TableElement.new("No. Model Objects")
row << OpenStudio::TableElement.new("No. Objects After Roundtrip")
row << OpenStudio::TableElement.new("Building Name")
row << OpenStudio::TableElement.new("No. Thermal Zones")
row << OpenStudio::TableElement.new("No. Surfaces")
row << OpenStudio::TableElement.new("No. SubSurfaces")
row << OpenStudio::TableElement.new("No. ShadingSurfaces")
row << OpenStudio::TableElement.new("Idf Draft Valid")
row << OpenStudio::TableElement.new("Idf Final Valid")
row << OpenStudio::TableElement.new("Roundtrip Draft Valid")
row << OpenStudio::TableElement.new("Roundtrip Final Valid")
summaryTable.appendRow(row)
summaryTable.setNHead(1)

count = 0

individualReportHeaders = []
numIndividualReportsPerMainHeader = []
individualReports = []

n = nil
if ARGV.size() > 2
  n = ARGV[2].to_i
end 

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
  row << OpenStudio::TableElement.new(file)
  row << OpenStudio::TableElement.new(workspace.numObjects)
  row << OpenStudio::TableElement.new(model.numObjects)
  row << OpenStudio::TableElement.new(rtWorkspace.numObjects)
  row << OpenStudio::TableElement.new(bldg.name().get())
  row << OpenStudio::TableElement.new(zones.size)
  row << OpenStudio::TableElement.new(surfaces.size)
  row << OpenStudio::TableElement.new(subSurfaces.size)
  row << OpenStudio::TableElement.new(shadingSurfaces.size)
  
  # check validity, including writing out draft level reports
  idfReport = workspace.validityReport("Draft".to_StrictnessLevel)
  idfFinal = workspace.isValid("Final".to_StrictnessLevel)
  rtReport = rtWorkspace.validityReport("Draft".to_StrictnessLevel)
  rtFinal = rtWorkspace.isValid("Final".to_StrictnessLevel)
  
  row << OpenStudio::TableElement.new(idfReport.numErrors == 0)
  row << OpenStudio::TableElement.new(idfFinal)
  row << OpenStudio::TableElement.new(rtReport.numErrors == 0)
  row << OpenStudio::TableElement.new(rtFinal)
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
  
  break if n and count >= n
  
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
  summaryTable.setElement(i,0,OpenStudio::formatUnderscore(summaryTable[i][0]))
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
