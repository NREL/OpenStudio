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

require 'openstudio'

require 'test/unit'

class ApplyStandardsRuleset_Test < Test::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_ApplyStandardsRuleset
  
    rulesetDir = OpenStudio::Path.new($OpenStudio_ResourcePath + "ruleset/Title24_2008/") 
    modelFilepath = OpenStudio::Path.new($OpenStudio_ResourcePath + "ruleset/scratch/in.osm")
    model = OpenStudio::Model::exampleModel
    model.save(modelFilepath,true)
    assert(system("#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/examples/ApplyStandardsRuleset.rb' '#{rulesetDir}' '#{modelFilepath}'"))
  
  end
  
end