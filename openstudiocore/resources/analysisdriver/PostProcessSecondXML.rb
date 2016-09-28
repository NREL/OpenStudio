########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

require 'openstudio'
require 'optparse'

options = Hash.new
optparse = OptionParser.new do|opts|

  opts.on('--modelPath MODELPATH', String, "Path to OpenStudio Model" ) do |modelPath|
    options[:modelPath] = modelPath
  end
  
  opts.on('--sqlPath SQLPATH', String, "SQLite output file" ) do |sqlPath|
    options[:sqlPath] = sqlPath
  end
  
end

optparse.parse!

if not options[:modelPath]
  options[:modelPath] = "in.osm"
end

if not options[:sqlPath]
  options[:sqlPath] = "eplusout.sql"
end

# set up logging
logSink = OpenStudio::FileLogSink.new(OpenStudio::Path.new("PostProcess.log"))
logSink.setLogLevel(-4)

model_path = OpenStudio::Path.new(options[:modelPath])
model = OpenStudio::Model::Model::load(model_path)
raise "Unable to load OSM from " + model_path.to_s + "." if model.empty?
model = model.get

sql_path = OpenStudio::Path.new(options[:sqlPath])
raise "No EnergyPlus SQLite file at " + sql_path.to_s + "." if not OpenStudio::exists(sql_path)
sqlFile = OpenStudio::SqlFile.new(sql_path)
model.setSqlFile(sqlFile)

# No longer have to append to existing report.xml.
# Can just write a new one and both will be picked up.
report_vector = OpenStudio::AttributeVector.new

peak_value = model.sqlFile().get().execAndReturnFirstDouble("SELECT Value FROM tabulardatawithstrings WHERE ReportName='EnergyMeters'" +
    " AND ReportForString='Entire Facility' AND TableName='Annual and Peak Values - Electricity' AND RowName='Electricity:Facility'" +
    " AND ColumnName='Electricity Maximum Value' AND Units='W'")
if not peak_value.empty?
  report_vector << OpenStudio::Attribute.new("peakAnnualDemand", peak_value.get.to_f, "W")
end
 
report = OpenStudio::Attribute.new("Report", report_vector)

report.saveToXml(OpenStudio::Path.new("report.xml"))

