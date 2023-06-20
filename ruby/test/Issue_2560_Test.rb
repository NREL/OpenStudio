########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'
require 'logger'
require 'openstudio/measure/ShowRunnerOutput'
require 'fileutils'
require 'minitest/autorun'
require 'csv'

class Issue_2560_Test < Minitest::Test
  def test_Issue_2560

    model = OpenStudio::Model::Model.new
    story2 = OpenStudio::Model::BuildingStory.new(model)

    perimeter = OpenStudio::Point3dVector.new
    perimeter.push(OpenStudio::Point3d.new(4, 2, 3))
    perimeter.push(OpenStudio::Point3d.new(4, 6, 3))
    perimeter.push(OpenStudio::Point3d.new(18, 6, 3))
    perimeter.push(OpenStudio::Point3d.new(18, 2, 3))
    #OptionalSpace
    sp1 = OpenStudio::Model::Space::fromFloorPrint(perimeter,3,model)
    assert(sp1)
    sp1 = sp1.get
    sp1.setBuildingStory(story2)
    tz1 = OpenStudio::Model::ThermalZone.new(model)
    sp1.setThermalZone(tz1)

    story1 = OpenStudio::Model::BuildingStory.new(model)

    perimeter.clear()
    perimeter.push(OpenStudio::Point3d.new(0, 0, 0))
    perimeter.push(OpenStudio::Point3d.new(0, 3, 0))
    perimeter.push(OpenStudio::Point3d.new(22, 3, 0))
    perimeter.push(OpenStudio::Point3d.new(22, 0, 0))
    #OptionalSpace
    sp2 = OpenStudio::Model::Space::fromFloorPrint(perimeter,3,model)
    assert(sp2)
    sp2 = sp2.get
    sp2.setBuildingStory(story1)
    tz2 = OpenStudio::Model::ThermalZone.new(model)
    sp2.setThermalZone(tz2)

    perimeter.clear()
    perimeter.push(OpenStudio::Point3d.new(0, 3, 0))
    perimeter.push(OpenStudio::Point3d.new(0, 5, 0))
    perimeter.push(OpenStudio::Point3d.new(22, 5, 0))
    perimeter.push(OpenStudio::Point3d.new(22, 3, 0))
    #OptionalSpace
    sp3 = OpenStudio::Model::Space::fromFloorPrint(perimeter,3,model)
    assert(sp3)
    sp3 = sp3.get
    sp3.setBuildingStory(story1)
    tz3 = OpenStudio::Model::ThermalZone.new(model)
    sp3.setThermalZone(tz3)

    perimeter.clear()
    perimeter.push(OpenStudio::Point3d.new(0, 5, 0))
    perimeter.push(OpenStudio::Point3d.new(0, 8, 0))
    perimeter.push(OpenStudio::Point3d.new(22, 8, 0))
    perimeter.push(OpenStudio::Point3d.new(22, 5, 0))
    #OptionalSpace
    sp4 = OpenStudio::Model::Space::fromFloorPrint(perimeter,3,model)
    assert(sp4)
    sp4 = sp4.get
    sp4.setBuildingStory(story1)
    tz4 = OpenStudio::Model::ThermalZone.new(model)
    sp4.setThermalZone(tz4)

    # Model before intersection
    # outdir = File.absolute_path(File.join(OpenStudio::getEnergyPlusDirectory.to_s, "../resources/model"))
    # outpath = File.join(outdir, "2560_before.osm")
    # model.save(outpath, true)

    spaces = model.getSpaces
    #spaces = spaces.get
    spacesX = OpenStudio::Model::SpaceVector.new
    spaces.each do |s|
      spacesX << s
    end

    OpenStudio::Model::intersectSurfaces(spacesX)
    OpenStudio::Model::matchSurfaces(spacesX)

    # outpath = File.join(outdir, "2560_after.osm")
    # model.save(outpath, true)

    # TODO: this block achieves nothing, is it for debugging purposes?
    spacesX.each do |space|
      spaceName = space.name.get
      puts spaceName
      surfaces = space.surfaces
      surfaces.each do |surface|
        surfaceName = surface.name.get
        puts surfaceName
        vertices = surface.vertices
      end
      # puts "End of Space Surfaces"
      # puts "********************"
    end

    # Verify that the floor surfaces on space 1 are matched
    space1Surfaces = sp1.surfaces;
    # index_surface1 = nil
    # index_surface27 = nil
    # index_surface28 = nil
    _space1Surface_1 = nil
    _space1Surface_27 = nil
    _space1Surface_28 = nil
    space1Surfaces.each do |space1Surface|
      # puts "->"
      if space1Surface.name.get == "Surface 1"
        puts space1Surface.name.get
        _space1Surface_1 = space1Surface
        assert(_space1Surface_1.adjacentSurface())
        _adjacentSurface_1 = _space1Surface_1.adjacentSurface()
        _adjacentSurface_1 = _adjacentSurface_1.get
        # puts _adjacentSurface.name
        assert_equal(_adjacentSurface_1.name.get, "Surface 18")
        #index_surface1 = space1Surfaces.find_index(space1Surface)
        #puts "Surface 1 Index : #{index_surface1}"
      end
      if space1Surface.name.get == "Surface 28"
        puts space1Surface.name.get
        _space1Surface_28 = space1Surface
        assert(_space1Surface_28.adjacentSurface())
        _adjacentSurface_28 = _space1Surface_28.adjacentSurface()
        _adjacentSurface_28 = _adjacentSurface_28.get
        assert_equal(_adjacentSurface_28.name.get, "Surface 24")
        #index_surface28 = space1Surfaces.find_index(space1Surface)
        #puts "Surface 28 index : #{index_surface28}"
      end
      if space1Surface.name.get == "Surface 27"
        puts space1Surface.name.get
        _space1Surface_27 = space1Surface
        assert(_space1Surface_27.adjacentSurface())
        _adjacentSurface_27 = _space1Surface_27.adjacentSurface()
        _adjacentSurface_27 = _adjacentSurface_27.get
        assert_equal(_adjacentSurface_27.name.get,"Surface 12")
        #index_surface27 = space1Surfaces.find_index(space1Surface)
        #puts "Surface 27 index : #{index_surface27}"
      end
    end
  end
end
