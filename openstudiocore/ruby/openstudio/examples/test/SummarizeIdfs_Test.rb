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

require 'openstudio'
require 'openstudio/energyplus/find_energyplus'

require 'test/unit'

class SummarizeIdfs_Test < Test::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_SummarizeIdfs
    ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,1)
    idfDir = OpenStudio::Path.new(ep_hash[:energyplus_examplefiles].to_s)
    outFile = OpenStudio::Path.new($OpenStudio_ResourcePath + "utilities/Idf/EnergyPlusExampleFilesSummary.html")
    cmd = "#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/examples/SummarizeIdfs.rb' '#{idfDir}' '#{outFile}' 5"
    assert(system(cmd))
  end

end


