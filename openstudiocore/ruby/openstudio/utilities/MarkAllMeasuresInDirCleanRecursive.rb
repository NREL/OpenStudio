######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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
#   Marks all measures found in a directory or it's children as clean.
#
# == Usage
#
#  ruby MarkAllMeasuresInDirCleanRecursive.rb ARGV[0]
#
#  ARGV[0] - Path to directory containing measures
#
# == Examples
#
#   ruby MarkAllMeasuresInDirCleanRecursive.rb 'C:\path\to\measures\' 
#
######################################################################

require 'openstudio'

dir = ARGV[0].gsub("\\", "/")

if not dir or not File.directory?(dir)
  puts "Script requires argument which is path to directory containing measures"
  exit(false)
end

puts "Marking measures found in '#{dir}' or its children as clean"

num_measures = 0
Dir.glob("#{dir}/**/") do |measure_dir|
  puts measure_dir
  if File.directory?("#{measure_dir}")
    measure = OpenStudio::BCLMeasure::load(OpenStudio::Path.new("#{measure_dir}"))
    if measure.empty?
      puts "Directory #{measure_dir} is not a measure"
    else
      measure = measure.get
      measure.checkForUpdates
      measure.save
    end
  end
end
