######################################################################
#  Copyright (c) 2008-2011, Alliance for Sustainable Energy.  
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
  
  opts.on('--xmlPath XMLPATH', String, "XML report file" ) do |xmlPath|
    options[:xmlPath] = xmlPath
  end
end

optparse.parse!

if not options[:modelPath]
  options[:modelPath] = "in.osm"
end

if not options[:sqlPath]
  options[:sqlPath] = "eplusout.sql"
end

if not options[:xmlPath]
  options[:xmlPath] = "in.xml"
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

xml_path = OpenStudio::Path.new(options[:xmlPath])
xmlFile = OpenStudio::Attribute.loadFromXml(xml_path)
raise "Unable to load attribute xml file from " + xml_path.to_s + "." if xmlFile.empty?
xmlFile = xmlFile.get

report_vector = xmlFile.valueAsAttributeVector.dup # .dup makes an unfrozen copy of the vector

peak_value = model.sqlFile().get().execAndReturnFirstDouble("SELECT Value FROM tabulardatawithstrings WHERE ReportName='EnergyMeters'" +
    " AND ReportForString='Entire Facility' AND TableName='Annual and Peak Values - Electricity' AND RowName='Electricity:Facility'" +
    " AND ColumnName='Electricity Maximum Value' AND Units='W'")
if not peak_value.empty?
  report_vector << OpenStudio::Attribute.new("peakAnnualDemand", peak_value.get.to_f, "W")
end
 
report = OpenStudio::Attribute.new("Report", report_vector)

report.saveToXml(OpenStudio::Path.new("report.xml"))

