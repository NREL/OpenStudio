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

require 'pathname'

module OpenStudio

  module EnergyPlus
  
    # return the build version from a file path
    # This function does not provide meaningful information for versions starting at 8.2.0
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
          else
            # 8.2.0 and above uses 10 digit build SHAs which can be find in the exe output, and the idd file.
            version = 0
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
    
      warn "[DEPRECATION] `find_energyplus` is deprecated.  Please use `OpenStudio::Runmanager::ConfigOptions` instead."

      # version matches either internal or official releases
      # prior to version 8.2.0 we assume the least significant version number (third digit) is 0
      # 8.2.0 and beyond we cannot assume that
      version = ''
      if (major_version) < 8 or (major_version == 8 and minor_version < 2)
        version = "#{major_version}[-\\.]#{minor_version}[-\\.]0[-\\.]?#{build_version}"
      else
       version = "#{major_version}[-\\.]#{minor_version}[-\\.]\\d+"
      end

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

          # if we are at or above 8.2 check build number from idd file
          if major_version >= 8 and minor_version >= 2
            build = 'xxxxxxxxxx'
            File.open result[:energyplus_idd] do |f|
              f.gets # skip past the first line which has version major.minor.patch build number
              build_line = f.gets # get the build number from the second line
              build = /!IDD_BUILD ([\d\w]{10})/.match(build_line)[1]
            end
            if not build.match build_version
              next
            end
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
