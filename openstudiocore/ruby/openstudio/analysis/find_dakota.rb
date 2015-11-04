######################################################################
#  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

  module Analysis
  
    # returns a hash with symbols 
    #   :dakota_exe - path to dakota
    #
    # example calls
    #   find_dakota(5, 3)       # most recent build of 5.3
    #   find_dakota(5, 3, '.*') # most recent build of 5.3
    #   find_dakota(5, 3, 1)    # only version 5.3.1
    def find_dakota(major_version, minor_version, patch_version = '.*')
    
      version = "#{major_version}\\.#{minor_version}\\.?#{patch_version}"
      # By default, the Dakota 5.2 install folder does not include a version number.
      # Check to see if any version is listed in the path.
      any_version = "[0-9]+\\.[0-9]+(\\.[0-9]+)?" 
    
      potential_paths = []
      executableName = "dakota"
      if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
        potential_paths = Dir.glob('/[dD][aA][kK][oO][tT][aA]*') + Dir.glob('C:/[dD][aA][kK][oO][tT][aA]*')
        if not ENV['DAKOTADIR'].nil?
          ENV['DAKOTADIR'].split(';').each { |path| 
            potential_paths << path 
          }
        end
        executableName = "dakota.exe"
      elsif /darwin/.match(RUBY_PLATFORM)
        potential_paths = Dir.glob('/Applications/[dD][aA][kK][oO][tT][aA]*')
      else
        potential_paths = Dir.glob('/usr/local/[dD][aA][kK][oO][tT][aA]*')
        if not ENV['DAKOTADIR'].nil?
          ENV['DAKOTADIR'].split(':').each { |path| 
            potential_paths << path 
          }
        end
      end

      # loop through and find the first match
      potential_paths.each do |path|

        puts "Looking for Dakota at #{path}."
        path = Pathname.new(path)
        
        keep = false
        
        if /#{any_version}/.match(path)
          if /#{version}/.match(path)
            keep = true
          end          
        else
          warn("No version information found. Will assume that the installed version is " + 
            "acceptable. To avoid this warning in the future, add a string like -" + major_version.to_s + 
            "." + minor_version.to_s + " to your Dakota folder, for example, C:\dakota-" + 
            major_version.to_s + "." + minor_version.to_s + ".");          
          keep = true
        end
        
        # check for dakota.exe
        if keep
          if File.exists?(path + "bin/" + executableName)
            result = Hash.new
            result[:dakota_exe] = path + "bin/" + executableName
            puts "Found Dakota at " + result[:dakota_exe].to_s + "."
            return result
          end
        end

      end

      # not found
      puts "Daktoa not found."
      return nil    
    end 
    
    # make this a public module method
    module_function :find_dakota
  
  end # Analysis
  
end # OpenStudio
