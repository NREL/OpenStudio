########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'
require 'minitest/autorun'

# Regression test for #5645: SWIG 4.4.0 broke methods taking a std::vector<T> parameter where T is
# only forward-declared in the module that defines the method, and the real type (plus its
# %template(...Vector)) lives in a separate, dependent SWIG module.
class Issue_5645_Test < Minitest::Test
  def test_ShadowCalculation_addShadingZoneGroup
    model = OpenStudio::Model::Model.new
    zone = OpenStudio::Model::ThermalZone.new(model)
    sc = model.getShadowCalculation

    zone_vector = OpenStudio::Model::ThermalZoneVector.new
    zone_vector.push(zone)

    assert(sc.addShadingZoneGroup(zone_vector))
    zones = sc.getShadingZoneGroup(0)
    assert_equal(1, zones.size)
    assert_equal(zone.handle, zones[0].handle)
  end

  def test_ShadingControl_addSubSurfaces_setSubSurfaces
    model = OpenStudio::Model::Model.new
    blind = OpenStudio::Model::Blind.new(model)
    shadingControl = OpenStudio::Model::ShadingControl.new(blind)

    vertices1 = OpenStudio::Point3dVector.new
    vertices1.push(OpenStudio::Point3d.new(0, 0, 1))
    vertices1.push(OpenStudio::Point3d.new(0, 0, 0))
    vertices1.push(OpenStudio::Point3d.new(1, 0, 0))
    vertices1.push(OpenStudio::Point3d.new(1, 0, 1))
    subSurface1 = OpenStudio::Model::SubSurface.new(vertices1, model)

    vertices2 = OpenStudio::Point3dVector.new
    vertices2.push(OpenStudio::Point3d.new(0, 1, 1))
    vertices2.push(OpenStudio::Point3d.new(0, 1, 0))
    vertices2.push(OpenStudio::Point3d.new(1, 1, 0))
    vertices2.push(OpenStudio::Point3d.new(1, 1, 1))
    subSurface2 = OpenStudio::Model::SubSurface.new(vertices2, model)

    subSurfaces = OpenStudio::Model::SubSurfaceVector.new
    subSurfaces.push(subSurface1)
    subSurfaces.push(subSurface2)

    assert_equal(0, shadingControl.numberofSubSurfaces)
    assert(shadingControl.addSubSurfaces(subSurfaces))
    assert_equal(2, shadingControl.numberofSubSurfaces)

    shadingControl.removeAllSubSurfaces
    assert_equal(0, shadingControl.numberofSubSurfaces)

    assert(shadingControl.setSubSurfaces(subSurfaces))
    assert_equal(2, shadingControl.numberofSubSurfaces)
  end
end
