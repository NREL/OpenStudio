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
require 'json'
require 'minitest/autorun'

class EpJSON_Test < MiniTest::Unit::TestCase

  def setup

    @idfFile = OpenStudio::IdfFile.new("EnergyPlus".to_IddFileType)
    building = OpenStudio::IdfObject.new("Building".to_IddObjectType)
    building.setName("Building 1")
    building.setDouble(1, 0.0) # North Axis
    @idfFile.addObject(building)

    @ep_version = Gem::Version.new(OpenStudio::energyPlusVersion)
  end

  # def teardown
  # end


  def common_asserts(json, ep_version)
    assert(json[:Version])
    assert(Gem::Version.new(json[:Version][:"Version 1"][:version_identifier]) == ep_version)
    assert(json[:Building])
    assert(json[:Building][:"Building 1"][:north_axis] == 0.0)
  end

  def test_epJSON_String

    json_str = OpenStudio::EPJSON::toJSONString(@idfFile);
    json = JSON.parse(json_str, symbolize_names: true)

    common_asserts(json, @ep_version)
  end

  def test_epJSON

    # This should directly convert it to a a hash
    json = OpenStudio::EPJSON::toJSON(@idfFile);
    common_asserts(json, @ep_version)
  end

end


