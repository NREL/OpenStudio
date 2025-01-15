########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'
require 'optparse'
require File.dirname(__FILE__) + '/SDDWriters/SDDWriter.rb'

# HANDLE INPUT ARGUMENTS

# define the input parameters
options = Hash.new
optparse = OptionParser.new do |opts|

  opts.on( '-i', '--inputPath INPUTPATH', String, "Path to IDF or OSM file to be written as SDD.") do |inputPath|
    options[:inputPath] = inputPath
  end

  opts.on( '-o', '--outputPath OUTPUTPATH', String, "Output file path.") do |outputPath|
    options[:outputPath] = outputPath
  end

  opts.on( '-f', '--[no-]flatFormat', "Flat format." ) do |flatFormat|
    options[:flatFormat] = flatFormat
  end

  opts.on( '-a', '--[no-]printAttributesAndRelationships', "Print attributes and relationships in flat format.") do |printAttributes|
    options[:printAttributes] = printAttributes
  end

  opts.on( '-s', '--startType STARTTYPE', String, "Top-level type for the hierarchical XML.") do |startType|
    options[:startType] = startType
  end

end

# parse the input parameters
optparse.parse!

# process the input parameters
inputPath = OpenStudio::Path.new("in.osm")
if options[:inputPath]
  inputPath = OpenStudio::Path.new(options[:inputPath].to_s)
end

outputPath = OpenStudio::Path.new("out.xml")
if options[:outputPath]
  outputPath = OpenStudio::Path.new(options[:outputPath].to_s)
end

flatFormat = false
if options[:flatFormat]
  flatFormat = true
end

printAttributes = false
if options[:printAttributes]
  printAttributes = true
end

startType = "OS:Building"
if options[:startType]
  startType = options[:startType].to_s
end


# LOAD MODEL

ext = OpenStudio::getFileExtension(inputPath)
model = nil
if ext == "idf"
  model = OpenStudio::EnergyPlus::loadAndTranslateIdf(inputPath).get
else
  model = OpenStudio::Model::Model::load(inputPath).get
end


# CREATE XML

sddWriter = SDDWriter.new(flatFormat,printAttributes)

if flatFormat

  model.order.setOrderByIddEnum
  model.objects(true).each { |object|
    sddWriter.addToDocument(sddWriter.root,object.to_ModelObject.get)
  }

else

  model.sort(model.getObjectsByType(startType.to_IddObjectType)).each { |object|
    sddWriter.addToDocument(sddWriter.root,object.to_ModelObject.get)
  }

end

# model.getLoops.each { |loop|

#   loopNode = sddWriter.addToDocument(sddWriter.root,loop)

# }



# WRITE XML TO FILE

File.open(outputPath.to_s,"w") do |file|
  formatter = REXML::Formatters::Pretty.new
  formatter.write(sddWriter.document,REXML::Output.new(file,REXML::Encoding::UTF_8))
end
