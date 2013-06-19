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

require 'pathname'

module OpenStudio

  module Analysis
  
    # returns a hash with symbols 
    # :dakota_exe - path to dakota
    def find_dakota()
    
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

        puts "Looking for Dakota at " + path

        path = Pathname.new(path)

        # check for dakota.exe
        if File.exists?(path + "bin/" + executableName)
          result = Hash.new
          result[:dakota_exe] = path + "bin/" + executableName
          puts "Found Dakota."
          return result
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
