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
