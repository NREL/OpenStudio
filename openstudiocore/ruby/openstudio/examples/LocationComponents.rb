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

######################################################################
# == Synopsis
#
#   Parse TLocations.xls and LocationDependentSets.imf to generate XML
#   components for location related data (weather files, design days, etc.)
#
# == Usage
#
#  ruby LocationComponents.rb ARGV[0] ARGV[1]
#
#  ARGV[0] - Path to Opt-E-Plus checkout directory
#
#  ARGV[1] - EnergyPlus version (no periods)
#
# == Examples
#
#  ruby LocationComponents.rb D:/working/optimization 50
#
######################################################################

require 'win32ole'
require 'FileUtils'

class LocationComponents

  # initialize
  def initialize
    @imfText = ''
    @locText = []
    FileUtils.rm_rf 'components'
  end

  # get locations from LocationDependSets.imf
  def get_locations
    # read entire location dependent sets file
    File.open(ARGV[0] + '/common/usefiles/EnergyPlusV' + ARGV[1] + '/kit_main/include/HPBLocationDependSets.imf') do |f|
      while line = f.gets
        @imfText << line
      end
    end
    # match regular expression in IMF file text for ##def Location[]
    r = /^\#\#def.*/n
    matchtext = @imfText
    while m = r.match(matchtext)
      @locText << m.to_s.gsub('##def ', '').gsub('[]', '')
      matchtext = m.post_match
    end
    @locText.delete_at(0) # delete first element (it is a header)
  end

  #make directories based on location names
  def make_dirs
    # make directories for each location
    FileUtils.mkdir 'components'
    @compDir = './components'
    FileUtils.mkdir @compDir + '/' + 'locations'
    FileUtils.mkdir @compDir + '/' + 'groundtemps'
    FileUtils.mkdir @compDir + '/' + 'mainstemps'
    FileUtils.mkdir @compDir + '/' + 'designdays'
    FileUtils.mkdir @compDir + '/' + 'tarballs'
    @locDir = './components/locations/'
    @gndDir = './components/groundtemps/'
    @mnsDir = './components/mainstemps/'
    @ddsDir = './components/designdays/'
    @tarDir = './components/tarballs/'
    @locText.each do |loc|
      FileUtils.mkdir(@locDir + loc.to_s)
      FileUtils.mkdir(@gndDir + loc.to_s)
      FileUtils.mkdir(@mnsDir + loc.to_s)
      FileUtils.mkdir(@ddsDir + loc.to_s)
    end
  end

  # get data from LocationDependSets
  def get_location_data
    # match regular expression in IMF file text for Site:Location
    r1 = /^((Site:Location[^,;]*)[,;].*?\n)[\s]*$/m
    matchtext = @imfText
    i = 0
    while m1 = r1.match(matchtext)
      # write match to IDF file
      File.open(@locDir.to_s + @locText[i].to_s + '/' + 'SiteLocation.idf', 'w') do |f1|
        f1.write(m1.to_s)
      end
      matchtext = m1.post_match
      i = i + 1
    end
    # match regular expression in IMF file text for Site:GroundTemperature:BuildingSurface
    r2 = /^((Site:GroundTemperature:BuildingSurface[^,;]*)[,;].*?\n)[\s]*$/m
    matchtext = @imfText
    i = 0
    while m2 = r2.match(matchtext)
      # write match to IDF file
      File.open(@gndDir.to_s + @locText[i].to_s + '/' + 'GroundTemps.idf', 'w') do |f2|
        f2.write(m2.to_s)
      end
      matchtext = m2.post_match
      i = i + 1
    end
    # match regular expression in IMF file text for Site:WaterMainsTemperature
    r3 = /^((Site:WaterMainsTemperature[^,;]*)[,;].*?\n)[\s]*$/m
    matchtext = @imfText
    i = 0
    while m3 = r3.match(matchtext)
      # write match to IDF file
      File.open(@mnsDir.to_s + @locText[i].to_s + '/' + 'MainsTemps.idf', 'w') do |f3|
        f3.write(m3.to_s)
      end
      matchtext = m3.post_match
    end
    # match regular expression in IMF file text for SizingPeriod:DesignDay
    r4 = /^((.SizingPeriod:DesignDay[^,;]*)[,;].*?\n)[\s]*$/m
    matchtext = @imfText
    i = 0
    while m4 = r4.match(matchtext)
      # write match to IDF file
      File.open(@ddsDir.to_s + @locText[i].to_s + '/' + 'DesignDays.idf', 'w') do |f4|
        f4.write(m4.to_s)
      end
      matchtext = m4.post_match
      m4 = r4.match(matchtext)
      File.open(@ddsDir.to_s + @locText[i].to_s + '/' + 'DesignDays.idf', 'a') do |f4|
        f4.write(m4.to_s)
      end
      matchtext = m4.post_match
      m4 = r4.match(matchtext)
      File.open(@ddsDir.to_s + @locText[i].to_s + '/' + 'DesignDays.idf', 'a') do |f4|
        f4.write(m4.to_s)
      end
      i = i + 1
    end
  end

  # get data from TLocations
  def get_excel_data
    # enable MS Excel
    xl = WIN32OLE::new('Excel.Application')
    # open workbook
    wb = xl.workbooks.open(ARGV[0] + '/database/EDMs/TLocation.xls')
    # specify TMY2 worksheet
    tmy2ws = wb.worksheets('Location_TMY2')
    # specify TMY2 data range
    tmy2data = tmy2ws.range('A1:AQ250')['Value'].transpose
    # specify TMY3 worksheet
    tmy3ws = wb.worksheets('Location_TMY3')
    # specify TMY3 data range
    tmy3data = tmy3ws.range('A1:V1052')['Value'].transpose
    # close workbook
    wb.Close(1)
    # quit Excel
    xl.Quit
  end

end

location_components = LocationComponents.new
location_components.get_locations
location_components.make_dirs
location_components.get_location_data
