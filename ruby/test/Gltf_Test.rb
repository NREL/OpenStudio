########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'
require 'json'
require 'minitest/autorun'

class Gltf_Test < Minitest::Test

  # def setup
  # end

  # def teardown
  # end

  def common_asserts(json)
    assert(json["asset"])
    assert(json["asset"]["generator"])
    assert(json["asset"]["generator"] == "OpenStudio")

    assert(json["nodes"])
    assert(json["nodes"].size == 31)

    assert(!json['nodes'][0]['extras'])
    assert(json['nodes'][1]['extras'])
  end

  def test_gltf_ExampleModel

    m = OpenStudio::Model::exampleModel

    ft = OpenStudio::Gltf::GltfForwardTranslator.new

    json_str = ft.modelToGLTFString(m);
    json = JSON.parse(json_str)
    common_asserts(json)
  end

  def test_gltf_ExampleModel_convenience

    m = OpenStudio::Model::exampleModel

    ft = OpenStudio::Gltf::GltfForwardTranslator.new

    json = ft.modelToGLTFJSON(m);
    common_asserts(json)
  end
end
