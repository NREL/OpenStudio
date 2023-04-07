########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

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

