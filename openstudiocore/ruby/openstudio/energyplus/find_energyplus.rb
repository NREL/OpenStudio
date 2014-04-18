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

require 'pathname'

module OpenStudio

  module EnergyPlus
  
    # return the build version from a file path
    def build_version(f)
      version = 0 # was nil
      if match_data = /(\d+)-(\d+)-(\d+)-(\d+)/.match(f)
        version = match_data[4].to_i
      elsif match_data = /(\d+-\d+-\d+)/.match(f)
        # register each release version
        case match_data[1].to_s
          when "8-1-0"
            version = 9
          when "8-0-0"
            version = 8
          when "7-2-0"
            version = 6
          when "7-1-0"
            version = 12
          when "7-0-0"
            version = 24
          when "6-0-0"
            version = 23
          when "5-0-0"
            version = 31
          when "4-0-0"
            version = 24
          end        
      end
      return version
    end
    
    # make this a public module method
    module_function :build_version
    
    # returns a hash with symbols 
    # :energyplus_exe - path to energyplus
    # :energyplus_idd - path to Energy+.idd
    # :energyplus_weatherdata - path to WeatherData
    # :energyplus_examplefiles - path to ExampleFiles
    # :energyplus_datasets - path to DataSets
    #
    # example calls:
    #   find_energyplus(5, 0) # most recent build of 5.0
    #   find_energyplus(5, 0, '.*') # most recent build of 5.0
    #   find_energyplus(5, 0, 16) # only build 16
    #
    def find_energyplus(major_version, minor_version, build_version = '.*')

      # version matches either internal or official releases
      version = "#{major_version}-#{minor_version}-0[-]?#{build_version}"

      # default search paths by system
      potential_paths = []
      if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
        potential_paths = Dir.glob('/EnergyPlus*') + Dir.glob('C:/EnergyPlus*')
        if ENV['ENERGYPLUSDIR']
          # ETH@20100916 Not sure what to do if multiple paths are lists. 
          # Dir.glob("#{ENV['ENERGYPLUSDIR']}*").each did not work for my machine.
          ENV['ENERGYPLUSDIR'].split(';').each { |path| potential_paths += Dir.glob(path) }
        end
      elsif /darwin/.match(RUBY_PLATFORM)
        potential_paths = Dir.glob('/Applications/EnergyPlus*')
      else
        potential_paths = Dir.glob('/usr/local/EnergyPlus*')
        if ENV['ENERGYPLUSDIR']
          ENV['ENERGYPLUSDIR'].split(';').each { |path| potential_paths += Dir.glob(path) }
        end
      end
 
      # sort in reverse order (highest version to lowest)
      potential_paths.sort!{|x,y| build_version(y) <=> build_version(x) }

      # loop through and find the first match
      potential_paths.each do |path|

        if /#{version}/.match(path)
          puts "Looking for EnergyPlus at #{path}"
          path = Pathname.new(path)
          result = Hash.new

          # check for energyplus.exe
          if File.exists?(path + "EnergyPlus.exe")
            result[:energyplus_exe] = path + "EnergyPlus.exe"
          elsif File.exists?(path + "bin/EnergyPlus.exe")
            result[:energyplus_exe] = path + "bin/EnergyPlus.exe"
          elsif File.exists?(path + "bin/energyplus")
            result[:energyplus_exe] = path + "bin/energyplus"
          elsif File.exists?(path + "EnergyPlus")
            result[:energyplus_exe] = path + "EnergyPlus"
          else
            #puts "E+ not found"
            next # not found
          end

          # check for Energy+.idd
          if File.exists?(path + "Energy+.idd")
            result[:energyplus_idd] = path + "Energy+.idd"
          elsif File.exists?(path + "bin/Energy+.idd")
            result[:energyplus_idd] = path + "bin/Energy+.idd"
          else
            #puts "E+.idd not found"
            next # not found
          end

          # check for WeatherData
          if File.exists?(path + "WeatherData") and File.directory?(path + "WeatherData")
            result[:energyplus_weatherdata] = path + "WeatherData"
          else
            #puts "WeatherData not found"
            next # not found
          end
          
          # check for ExampleFiles
          if File.exists?(path + "ExampleFiles") and File.directory?(path + "ExampleFiles")
            result[:energyplus_examplefiles] = path + "ExampleFiles"
          elsif File.exists?(path + "Examples") and File.directory?(path + "Examples")
            result[:energyplus_examplefiles] = path + "Examples"
          else
            #puts "ExampleFiles not found"
            next # not found
          end
          
          # check for DataSets
          if File.exists?(path + "DataSets") and File.directory?(path + "DataSets")
            result[:energyplus_datasets] = path + "DataSets"
          else
            #puts "DataSets not found"
            next # not found
          end
          
          # found
          puts "Found EnergyPlus."
          return result
        end

      end

      # not found
      return nil  
     
    end

    # make this a public module method
    module_function :find_energyplus

  end

end
