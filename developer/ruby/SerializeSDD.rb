########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
