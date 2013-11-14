######################################################################
#  Copyright (c) 2008-2012, Alliance for Sustainable Energy.  
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

class PluginTemplates_Test < Test::Unit::TestCase

  def test_Templates
    templates_path = "#{$OpenStudio_LibPath}/openstudio/sketchup_plugin/resources/templates/"
    assert(File.exist?(templates_path))
    assert(File.directory?(templates_path))
    templates = Dir.glob(templates_path + "/*.osm")
    assert((not templates.empty?))
    templates.each do |template|
      path = OpenStudio::Path.new(template)
      
      vt = OpenStudio::OSVersion::VersionTranslator.new
      model = vt.loadModel(path)
      assert((not model.empty?))
      model = model.get
      
      # check that all space load instances are associated with a space type
      spaceLoads = model.getSpaceLoads
      spaceLoads.each do |spaceLoad|
        assert((not spaceLoad.spaceType.empty?))
      end
      
      # uncomment this to save the version translated file to the original path
      # DO NOT leave this in the test execution when you commit!
      #model.save(path, true)      
    end
  end

end

