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

require 'minitest/autorun'

class Component_Test < MiniTest::Unit::TestCase

  def test_lightingSchedule_fromScratch
  
    justASchedule = OpenStudio::Model::Model.new

    schedule = OpenStudio::Model::ScheduleCompact.new(justASchedule)
    ok = OpenStudio::Model::checkOrAssignScheduleTypeLimits("Lights","Lighting",schedule)
    assert_equal(true,ok)
    scheduleComponent = schedule.createComponent
    assert(scheduleComponent.componentData.numComponentObjects == 2)
    assert(scheduleComponent.numObjects == 3)
    assert_equal(false,scheduleComponent.primaryObject.to_ScheduleCompact.empty?)
    
    justLights = OpenStudio::Model::Model.new
    ocd = justLights.insertComponent(scheduleComponent)
    assert(justLights.numObjects == 3)
    assert_equal(false,ocd.empty?)
    oSchedule = ocd.get.primaryComponentObject.to_ScheduleCompact
    assert_equal(false,oSchedule.empty?)
    schedule = oSchedule.get
    lightDefinition = OpenStudio::Model::LightsDefinition.new(justLights)
    light1 = OpenStudio::Model::Lights.new(lightDefinition)
    light1.setSchedule(schedule)
    assert(justLights.numObjects == 5)
    
    ocd = justLights.insertComponent(scheduleComponent)
    assert(justLights.numObjects == 5)
    assert_equal(false,ocd.empty?)
    assert_equal(false,ocd.get.primaryComponentObject.to_ScheduleCompact.empty?)
    assert_equal(false,light1.schedule.empty?)
    assert(ocd.get.primaryComponentObject.to_ScheduleCompact.get == light1.schedule.get)
    
    light2 = OpenStudio::Model::Lights.new(lightDefinition)
    light2.setSchedule(schedule)
    
    lightsComponent = light1.createComponent
    assert(lightsComponent.componentData.numComponentObjects == 4)
    assert(lightsComponent.numObjects == 5)
    assert(lightsComponent.getLightss.size == 1)
    assert(lightsComponent.getScheduleCompacts.size == 1)
    resrcs = lightsComponent.getLightss[0].resources
    assert(resrcs.size == 2)
    resrcs = OpenStudio::Model::getRecursiveResources(lightsComponent.getLightss[0])
    assert(resrcs.size == 3)
    
  end
  
  def test_componentWatcher_inComponent
  
    # create component
    model = OpenStudio::Model::Model.new
    designDay = OpenStudio::Model::DesignDay.new(model)
    assert(model.numObjects == 1)
    designDayComponent = designDay.createComponent
    assert(designDayComponent.numObjects == 2)
    cd = designDayComponent.componentData
    versionUUID = cd.versionUUID
    designDay = designDayComponent.primaryObject.to_DesignDay.get
    
    # version changes on data change
    ok = designDay.setName("My Design Day")
    assert_equal(false,ok.empty?)
    assert_equal(false,versionUUID == cd.versionUUID)
    versionUUID = cd.versionUUID
    
  end
  
end

