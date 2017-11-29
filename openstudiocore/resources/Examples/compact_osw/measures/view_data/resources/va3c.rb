require 'openstudio'

if /^1\.8/.match(RUBY_VERSION)
  class Struct
    def to_h
      h = {}
      self.class.members.each{|m| h[m.to_sym] = self[m]} 
      return h
    end
  end
end

module THREE
  FrontSide = 0
  BackSide = 1
  DoubleSide = 2
end

# Va3c class converts an OpenStudio model to vA3C JSON format for rendering in Three.js
# using export at http://va3c.github.io/projects/#./osm-data-viewer/latest/index.html# as a guide
# many thanks to Theo Armour and the vA3C team for figuring out many of the details here
class VA3C

  Scene = Struct.new(:geometries, :materials, :object)
  
  Geometry = Struct.new(:uuid, :type, :data)
  GeometryData = Struct.new(:vertices, :normals, :uvs, :faces, :scale, :visible, :castShadow, :receiveShadow, :doubleSided)

  Material = Struct.new(:uuid, :type, :color, :ambient, :emissive, :specular, :shininess, :side, :opacity, :transparent, :wireframe)
  
  SceneObject = Struct.new(:uuid, :type, :matrix, :children)
  SceneChild = Struct.new(:uuid, :name, :type, :geometry, :material, :matrix, :userData)
  UserData = Struct.new(:handle, :name, :surfaceType, :constructionName, :spaceName, :thermalZoneName, :spaceTypeName, :buildingStoryName, 
                        :outsideBoundaryCondition, :outsideBoundaryConditionObjectName, 
                        :outsideBoundaryConditionObjectHandle, :coincidentWithOutsideObject,
                        :sunExposure, :windExposure, #:vertices, 
                        :surfaceTypeMaterialName, :boundaryMaterialName, :constructionMaterialName,  :thermalZoneMaterialName, 
                        :spaceTypeMaterialName, :buildingStoryMaterialName) do
    def initialize(*)
      super
      self.surfaceTypeMaterialName = 'Undefined' if self.surfaceTypeMaterialName.nil?
      self.boundaryMaterialName = 'Undefined' if self.boundaryMaterialName.nil?
      self.constructionMaterialName = 'Undefined' if self.constructionMaterialName.nil?
      self.thermalZoneMaterialName = 'Undefined' if self.thermalZoneMaterialName.nil?
      self.spaceTypeMaterialName = 'Undefined' if self.spaceTypeMaterialName.nil?
      self.buildingStoryMaterialName = 'Undefined' if self.buildingStoryMaterialName.nil?
      
      self.constructionName = '' if self.constructionName.nil?
      self.spaceName = '' if self.spaceName.nil?
      self.thermalZoneName = '' if self.thermalZoneName.nil?
      self.spaceTypeName = '' if self.spaceTypeName.nil?
      self.buildingStoryName = '' if self.buildingStoryName.nil?
      self.outsideBoundaryCondition = '' if self.outsideBoundaryCondition.nil?
      self.outsideBoundaryConditionObjectName = '' if self.outsideBoundaryConditionObjectName.nil?
      
    end
  end
  Vertex = Struct.new(:x, :y, :z)
 
  AmbientLight = Struct.new(:uuid, :type, :color, :matrix)
   
  def self.convert_model(model)
    scene = build_scene(model)
    
    boundingBox = OpenStudio::BoundingBox.new
    boundingBox.addPoint(OpenStudio::Point3d.new(0, 0, 0))
    boundingBox.addPoint(OpenStudio::Point3d.new(1, 1, 1))
    model.getPlanarSurfaceGroups.each do |group|
      boundingBox.add(group.transformation*group.boundingBox)
    end
    
    lookAtX = 0 # (boundingBox.minX.get + boundingBox.maxX.get) / 2.0
    lookAtY = 0 # (boundingBox.minY.get + boundingBox.maxY.get) / 2.0
    lookAtZ = 0 # (boundingBox.minZ.get + boundingBox.maxZ.get) / 2.0
    lookAtR = [Math.sqrt( (boundingBox.maxX.get/2.0)**2 + (boundingBox.maxY.get/2.0)**2 + (boundingBox.maxZ.get/2.0)**2 ),
               Math.sqrt( (boundingBox.minX.get/2.0)**2 + (boundingBox.maxY.get/2.0)**2 + (boundingBox.maxZ.get/2.0)**2 ),
               Math.sqrt( (boundingBox.maxX.get/2.0)**2 + (boundingBox.minY.get/2.0)**2 + (boundingBox.maxZ.get/2.0)**2 ),
               Math.sqrt( (boundingBox.maxX.get/2.0)**2 + (boundingBox.maxY.get/2.0)**2 + (boundingBox.minZ.get/2.0)**2 ),
               Math.sqrt( (boundingBox.minX.get/2.0)**2 + (boundingBox.minY.get/2.0)**2 + (boundingBox.maxZ.get/2.0)**2 ),
               Math.sqrt( (boundingBox.minX.get/2.0)**2 + (boundingBox.maxY.get/2.0)**2 + (boundingBox.minZ.get/2.0)**2 ),
               Math.sqrt( (boundingBox.maxX.get/2.0)**2 + (boundingBox.minY.get/2.0)**2 + (boundingBox.minZ.get/2.0)**2 ),
               Math.sqrt( (boundingBox.minX.get/2.0)**2 + (boundingBox.minY.get/2.0)**2 + (boundingBox.minZ.get/2.0)**2 )].max
               
    boundingBoxHash = {'minX' => boundingBox.minX.get, 'minY' => boundingBox.minY.get, 'minZ' => boundingBox.minZ.get,
                       'maxX' => boundingBox.maxX.get, 'maxY' => boundingBox.maxY.get, 'maxZ' => boundingBox.maxZ.get,
                       'lookAtX' => lookAtX, 'lookAtY' => lookAtY, 'lookAtZ' => lookAtZ, 'lookAtR' => lookAtR}
    
    buildingStoryNames = []
    model.getBuildingStorys.each do |buildingStory|
      buildingStoryNames << buildingStory.name.to_s
    end
    buildingStoryNames.sort! {|x,y| x.upcase <=> y.upcase} # case insensitive sort
    
    # build up the json hash
    result = Hash.new
    result['metadata'] = { 'version' => 4.3, 'type' => 'Object', 'generator' => 'OpenStudio', 
                           'buildingStoryNames' => buildingStoryNames, 'boundingBox' => boundingBoxHash}
    result['geometries'] = scene.geometries
    result['materials'] = scene.materials
    result['object'] = scene.object
    
    return result
  end
  
  # format a uuid
  def self.format_uuid(uuid)
    return uuid.to_s.gsub('{','').gsub('}','')
  end
    
  # format color
  def self.format_color(r, g, b)
    return "0x#{r.to_s(16).rjust(2,'0')}#{g.to_s(16).rjust(2,'0')}#{b.to_s(16).rjust(2,'0')}"
  end
  
  # create a material
  def self.make_material(name, color, opacity, side, shininess=50)

    transparent = false
    if opacity < 1
      transparent = true
    end

    material = {:uuid => "#{format_uuid(OpenStudio::createUUID)}",
                :name => name,
                :type => 'MeshPhongMaterial',
                :color => "#{color}".hex,
                :ambient => "#{color}".hex,
                :emissive => '0x000000'.hex,
                :specular => "#{color}".hex,
                :shininess => shininess,
                :opacity => opacity,
                :transparent => transparent,
                :wireframe => false,
                :side => side}
    return material
  end

  # create the standard materials
  def self.build_materials(model)
    materials = []
    
    #materials << make_material('Undefined', format_color(255, 255, 255), 1, THREE::DoubleSide) 
    materials << {:uuid => "#{format_uuid(OpenStudio::createUUID)}", :name => 'Undefined', :type => 'MeshBasicMaterial', :color => '0xffffff'.hex, :side => THREE::DoubleSide}
    
    materials << make_material('NormalMaterial', format_color(255, 255, 255), 1, THREE::DoubleSide) 
    #materials << make_material('NormalMaterial_Ext', format_color(255, 255, 255), 1, THREE::FrontSide) 
    materials << {:uuid => "#{format_uuid(OpenStudio::createUUID)}", :name => 'NormalMaterial_Ext', :type => 'MeshBasicMaterial', :color => '0xffffff'.hex, :side => THREE::FrontSide}
    materials << make_material('NormalMaterial_Int', format_color(255, 0, 0), 1, THREE::BackSide) 
    
    # materials from 'openstudio\openstudiocore\ruby\openstudio\sketchup_plugin\lib\interfaces\MaterialsInterface.rb'
    materials << make_material('Floor', format_color(128, 128, 128), 1, THREE::DoubleSide) 
    materials << make_material('Floor_Ext', format_color(128, 128, 128), 1, THREE::FrontSide) 
    materials << make_material('Floor_Int', format_color(191, 191, 191), 1, THREE::BackSide) 
    
    materials << make_material('Wall', format_color(204, 178, 102), 1, THREE::DoubleSide) 
    materials << make_material('Wall_Ext', format_color(204, 178, 102), 1, THREE::FrontSide) 
    materials << make_material('Wall_Int', format_color(235, 226, 197), 1, THREE::BackSide) 
    
    materials << make_material('RoofCeiling', format_color(153, 76, 76), 1, THREE::DoubleSide) 
    materials << make_material('RoofCeiling_Ext', format_color(153, 76, 76), 1, THREE::FrontSide) 
    materials << make_material('RoofCeiling_Int', format_color(202, 149, 149), 1, THREE::BackSide) 

    materials << make_material('Window', format_color(102, 178, 204), 0.6, THREE::DoubleSide) 
    materials << make_material('Window_Ext', format_color(102, 178, 204), 0.6, THREE::FrontSide) 
    materials << make_material('Window_Int', format_color(192, 226, 235), 0.6, THREE::BackSide) 
    
    materials << make_material('Door', format_color(153, 133, 76), 1, THREE::DoubleSide) 
    materials << make_material('Door_Ext', format_color(153, 133, 76), 1, THREE::FrontSide) 
    materials << make_material('Door_Int', format_color(202, 188, 149), 1, THREE::BackSide) 

    materials << make_material('SiteShading', format_color(75, 124, 149), 1, THREE::DoubleSide) 
    materials << make_material('SiteShading_Ext', format_color(75, 124, 149), 1, THREE::FrontSide) 
    materials << make_material('SiteShading_Int', format_color(187, 209, 220), 1, THREE::BackSide) 

    materials << make_material('BuildingShading', format_color(113, 76, 153), 1, THREE::DoubleSide) 
    materials << make_material('BuildingShading_Ext', format_color(113, 76, 153), 1, THREE::FrontSide) 
    materials << make_material('BuildingShading_Int', format_color(216, 203, 229), 1, THREE::BackSide) 
    
    materials << make_material('SpaceShading', format_color(76, 110, 178), 1, THREE::DoubleSide) 
    materials << make_material('SpaceShading_Ext', format_color(76, 110, 178), 1, THREE::FrontSide)
    materials << make_material('SpaceShading_Int', format_color(183, 197, 224), 1, THREE::BackSide) 
    
    materials << make_material('InteriorPartitionSurface', format_color(158, 188, 143), 1, THREE::DoubleSide)
    materials << make_material('InteriorPartitionSurface_Ext', format_color(158, 188, 143), 1, THREE::FrontSide)    
    materials << make_material('InteriorPartitionSurface_Int', format_color(213, 226, 207), 1, THREE::BackSide) 
    
    # start textures for boundary conditions
    materials << make_material('Boundary_Surface', format_color(0, 153, 0), 1, THREE::DoubleSide)
    materials << make_material('Boundary_Adiabatic', format_color(255, 101, 178), 1, THREE::DoubleSide)
    materials << make_material('Boundary_Space', format_color(255, 0, 0), 1, THREE::DoubleSide)
    materials << make_material('Boundary_Outdoors', format_color(163, 204, 204), 1, THREE::DoubleSide)
    materials << make_material('Boundary_Outdoors_Sun', format_color(40, 204, 204), 1, THREE::DoubleSide)
    materials << make_material('Boundary_Outdoors_Wind', format_color(9, 159, 162), 1, THREE::DoubleSide)
    materials << make_material('Boundary_Outdoors_SunWind', format_color(68, 119, 161), 1, THREE::DoubleSide)
    materials << make_material('Boundary_Ground', format_color(204, 183, 122), 1, THREE::DoubleSide)
    materials << make_material('Boundary_Groundfcfactormethod', format_color(153, 122, 30), 1, THREE::DoubleSide)
    materials << make_material('Boundary_Groundslabpreprocessoraverage', format_color(255, 191, 0), 1, THREE::DoubleSide)
    materials << make_material('Boundary_Groundslabpreprocessorcore', format_color(255, 182, 50), 1, THREE::DoubleSide)
    materials << make_material('Boundary_Groundslabpreprocessorperimeter', format_color(255, 178, 101), 1, THREE::DoubleSide)
    materials << make_material('Boundary_Groundbasementpreprocessoraveragewall', format_color(204, 51, 0), 1, THREE::DoubleSide)
    materials << make_material('Boundary_Groundbasementpreprocessoraveragefloor', format_color(204, 81, 40), 1, THREE::DoubleSide)
    materials << make_material('Boundary_Groundbasementpreprocessorupperwall', format_color(204, 112, 81), 1, THREE::DoubleSide)
    materials << make_material('Boundary_Groundbasementpreprocessorlowerwall', format_color(204, 173, 163), 1, THREE::DoubleSide)
    materials << make_material('Boundary_Othersidecoefficients', format_color(63, 63, 63), 1, THREE::DoubleSide)
    materials << make_material('Boundary_Othersideconditionsmodel', format_color(153, 0, 76), 1, THREE::DoubleSide) 
    
    # make construction materials
    model.getConstructionBases.each do |construction|
      color = construction.renderingColor
      if color.empty?
        color = OpenStudio::Model::RenderingColor.new(model)
        construction.setRenderingColor(color)
      else
        color = color.get
      end
      name = "Construction_#{construction.name.to_s}"
      materials << make_material(name, format_color(color.renderingRedValue, color.renderingGreenValue, color.renderingBlueValue), color.renderingAlphaValue / 255.to_f, THREE::DoubleSide)
    end
    
    # make thermal zone materials
    model.getThermalZones.each do |zone|
      color = zone.renderingColor
      if color.empty?
        color = OpenStudio::Model::RenderingColor.new(model)
        zone.setRenderingColor(color)
      else
        color = color.get        
      end
      name = "ThermalZone_#{zone.name.to_s}"
      materials << make_material(name, format_color(color.renderingRedValue, color.renderingGreenValue, color.renderingBlueValue), color.renderingAlphaValue / 255.to_f, THREE::DoubleSide)
    end
    
    # make space type materials
    model.getSpaceTypes.each do |spaceType|
      color = spaceType.renderingColor
      if color.empty?
        color = OpenStudio::Model::RenderingColor.new(model)
        spaceType.setRenderingColor(color)
      else
        color = color.get        
      end
      name = "SpaceType_#{spaceType.name.to_s}"
      materials << make_material(name, format_color(color.renderingRedValue, color.renderingGreenValue, color.renderingBlueValue), color.renderingAlphaValue / 255.to_f, THREE::DoubleSide)
    end
    
    # make building story materials
    model.getBuildingStorys.each do |buildingStory|
      color = buildingStory.renderingColor
      if color.empty?
        color = OpenStudio::Model::RenderingColor.new(model)
        buildingStory.setRenderingColor(color)
      else
        color = color.get        
      end
      name = "BuildingStory_#{buildingStory.name.to_s}"
      materials << make_material(name, format_color(color.renderingRedValue, color.renderingGreenValue, color.renderingBlueValue), color.renderingAlphaValue / 255.to_f, THREE::DoubleSide)
    end
    
    return materials
  end

  # get the index of a vertex out of a list
  def self.get_vertex_index(vertex, vertices, tol = 0.001)
    vertices.each_index do |i|
      if OpenStudio::getDistance(vertex, vertices[i]) < tol
        return i
      end
    end
    vertices << vertex
    return (vertices.length - 1)
  end

  # flatten array of vertices into a single array
  def self.flatten_vertices(vertices)
    result = []
    vertices.each do |vertex|
      #result << vertex.x
      #result << vertex.y
      #result << vertex.z
      
      result << vertex.x.round(3)
      result << vertex.z.round(3)
      result << -vertex.y.round(3)
    end
    return result
  end

  # turn a surface into geometries, the first one is the surface, remaining are sub surfaces
  def self.make_geometries(surface)
    geometries = []
    user_datas = []

    # get the transformation to site coordinates
    site_transformation = OpenStudio::Transformation.new
    planar_surface_group = surface.planarSurfaceGroup
    if not planar_surface_group.empty?
      site_transformation = planar_surface_group.get.siteTransformation
    end

    # get the vertices
    surface_vertices = surface.vertices
    t = OpenStudio::Transformation::alignFace(surface_vertices)
    r = t.rotationMatrix
    tInv = t.inverse
    surface_vertices = OpenStudio::reverse(tInv*surface_vertices)

    # get vertices of all sub surfaces
    sub_surface_vertices = OpenStudio::Point3dVectorVector.new
    sub_surfaces = surface.subSurfaces
    sub_surfaces.each do |sub_surface|
      sub_surface_vertices << OpenStudio::reverse(tInv*sub_surface.vertices)
    end

    # triangulate surface
    triangles = OpenStudio::computeTriangulation(surface_vertices, sub_surface_vertices)
    if triangles.empty?
      puts "Failed to triangulate surface #{surface.name} with #{sub_surfaces.size} sub surfaces"
      return geometries
    end

    all_vertices = []
    face_indices = []
    triangles.each do |vertices|
      vertices = site_transformation*t*vertices
      #normal = site_transformation.rotationMatrix*r*z

      # https://github.com/mrdoob/three.js/wiki/JSON-Model-format-3
      # 0 indicates triangle
      # 16 indicates triangle with normals
      face_indices << 0
      vertices.reverse_each do |vertex|
        face_indices << get_vertex_index(vertex, all_vertices)  
      end

      # convert to 1 based indices
      #face_indices.each_index {|i| face_indices[i] = face_indices[i] + 1}
    end

    data = GeometryData.new
    data.vertices = flatten_vertices(all_vertices)
    data.normals = [] 
    data.uvs = []
    data.faces = face_indices
    data.scale = 1
    data.visible = true
    data.castShadow = true
    data.receiveShadow = false
    data.doubleSided = true
    
    geometry = Geometry.new
    geometry.uuid = format_uuid(surface.handle)
    geometry.type = 'Geometry'
    geometry.data = data.to_h
    geometries << geometry.to_h
    
    surface_user_data = UserData.new
    surface_user_data.handle = format_uuid(surface.handle)
    surface_user_data.name = surface.name.to_s
    surface_user_data.coincidentWithOutsideObject = false
    surface_user_data.surfaceType = surface.surfaceType
    surface_user_data.surfaceTypeMaterialName = surface.surfaceType
    
    surface_user_data.outsideBoundaryCondition = surface.outsideBoundaryCondition
    adjacent_surface = surface.adjacentSurface
    if adjacent_surface.is_initialized
      surface_user_data.outsideBoundaryConditionObjectName = adjacent_surface.get.name.to_s
      surface_user_data.outsideBoundaryConditionObjectHandle = format_uuid(adjacent_surface.get.handle)
      
      other_site_transformation = OpenStudio::Transformation.new
      other_group = adjacent_surface.get.planarSurfaceGroup
      if not other_group.empty?
        other_site_transformation = other_group.get.siteTransformation
      end
      
      other_vertices = other_site_transformation*adjacent_surface.get.vertices
      if OpenStudio::circularEqual(site_transformation*surface.vertices, OpenStudio::reverse(other_vertices))
        #puts "adjacent surfaces are coincident"
        surface_user_data.coincidentWithOutsideObject = true # controls display only, not energy model
      else
        #puts "adjacent surfaces are not coincident"
        surface_user_data.coincidentWithOutsideObject = false # controls display only, not energy model
      end
            
    end
    surface_user_data.sunExposure = surface.sunExposure
    surface_user_data.windExposure = surface.windExposure
    
    if surface.outsideBoundaryCondition == 'Outdoors'
      if surface.sunExposure == 'SunExposed' && surface.windExposure == 'WindExposed'
        surface_user_data.boundaryMaterialName = 'Boundary_Outdoors_SunWind'
      elsif surface.sunExposure == 'SunExposed'
        surface_user_data.boundaryMaterialName = 'Boundary_Outdoors_Sun'
      elsif surface.sunExposure == 'WindExposed'
        surface_user_data.boundaryMaterialName = 'Boundary_Outdoors_Wind'
      else
        surface_user_data.boundaryMaterialName = 'Boundary_Outdoors'
      end
    else
      surface_user_data.boundaryMaterialName = 'Boundary_' + surface.outsideBoundaryCondition
    end
    
    construction = surface.construction
    if construction.is_initialized
      surface_user_data.constructionName = construction.get.name.to_s
      surface_user_data.constructionMaterialName = 'Construction_' + construction.get.name.to_s
    end
    
    space = surface.space
    if space.is_initialized
      space = space.get
      surface_user_data.spaceName = space.name.to_s
      
      thermal_zone = space.thermalZone
      if thermal_zone.is_initialized
        surface_user_data.thermalZoneName = thermal_zone.get.name.to_s
        surface_user_data.thermalZoneMaterialName = 'ThermalZone_' + thermal_zone.get.name.to_s
      end
      
      space_type = space.spaceType
      if space_type.is_initialized
        surface_user_data.spaceTypeName = space_type.get.name.to_s
        surface_user_data.spaceTypeMaterialName = 'SpaceType_' + space_type.get.name.to_s
      end
      
      building_story = space.buildingStory
      if building_story.is_initialized
        surface_user_data.buildingStoryName = building_story.get.name.to_s
        surface_user_data.buildingStoryMaterialName = 'BuildingStory_' + building_story.get.name.to_s
      end
    end
    
    #vertices = []
    #surface.vertices.each do |v| 
    #  vertex = Vertex.new
    #  vertex.x = v.x
    #  vertex.y = v.y
    #  vertex.z = v.z
    #  vertices << vertex.to_h
    #end
    #surface_user_data.vertices = vertices
    user_datas << surface_user_data.to_h
    
    # now add geometry for each sub surface
    sub_surfaces.each do |sub_surface|
   
      # triangulate sub surface
      sub_surface_vertices = OpenStudio::reverse(tInv*sub_surface.vertices)
      triangles = OpenStudio::computeTriangulation(sub_surface_vertices, OpenStudio::Point3dVectorVector.new)
      
      all_vertices = []
      face_indices = []
      triangles.each do |vertices|
        vertices = site_transformation*t*vertices
        #normal = site_transformation.rotationMatrix*r*z
        
        # https://github.com/mrdoob/three.js/wiki/JSON-Model-format-3
        # 0 indicates triangle
        # 16 indicates triangle with normals
        face_indices << 0
        vertices.reverse_each do |vertex|
          face_indices << get_vertex_index(vertex, all_vertices)  
        end    

        # convert to 1 based indices
        #face_indices.each_index {|i| face_indices[i] = face_indices[i] + 1}
      end
      
      data = GeometryData.new
      data.vertices = flatten_vertices(all_vertices)
      data.normals = [] 
      data.uvs = []
      data.faces = face_indices
      data.scale = 1
      data.visible = true
      data.castShadow = true
      data.receiveShadow = false
      data.doubleSided = true
      
      geometry = Geometry.new
      geometry.uuid = format_uuid(sub_surface.handle)
      geometry.type = 'Geometry'
      geometry.data = data.to_h
      geometries << geometry.to_h
      
      sub_surface_user_data = UserData.new
      sub_surface_user_data.handle = format_uuid(sub_surface.handle)
      sub_surface_user_data.name = sub_surface.name.to_s
      sub_surface_user_data.coincidentWithOutsideObject = false
      
      sub_surface_user_data.surfaceType = sub_surface.subSurfaceType
      if /Window/.match(sub_surface.subSurfaceType) || /Glass/.match(sub_surface.subSurfaceType) || 
         /Skylight/.match(sub_surface.subSurfaceType) || /TubularDaylight/.match(sub_surface.subSurfaceType) 
        sub_surface_user_data.surfaceTypeMaterialName = 'Window'
      else
        sub_surface_user_data.surfaceTypeMaterialName = 'Door'
      end
      
      sub_surface_user_data.outsideBoundaryCondition = surface_user_data.outsideBoundaryCondition
      adjacent_sub_surface = sub_surface.adjacentSubSurface
      if adjacent_sub_surface.is_initialized
        sub_surface_user_data.outsideBoundaryConditionObjectName = adjacent_sub_surface.get.name.to_s
        sub_surface_user_data.outsideBoundaryConditionObjectHandle = format_uuid(adjacent_sub_surface.get.handle)
      
        other_site_transformation = OpenStudio::Transformation.new
        other_group = adjacent_sub_surface.get.planarSurfaceGroup
        if not other_group.empty?
          other_site_transformation = other_group.get.siteTransformation
        end
        
        other_vertices = other_site_transformation*adjacent_sub_surface.get.vertices
        if OpenStudio::circularEqual(site_transformation*sub_surface.vertices, OpenStudio::reverse(other_vertices))
          #puts "adjacent sub surfaces are coincident"
          surface_user_data.coincidentWithOutsideObject = true # controls display only, not energy model
        else
          #puts "adjacent sub surfaces are not coincident"
          surface_user_data.coincidentWithOutsideObject = false # controls display only, not energy model
        end
      
        sub_surface_user_data.boundaryMaterialName = 'Boundary_Surface'
      else
        if surface_user_data.boundaryMaterialName == 'Boundary_Surface'
          sub_surface_user_data.boundaryMaterialName = 'Undefined'
        else
          sub_surface_user_data.boundaryMaterialName = surface_user_data.boundaryMaterialName
        end
      end
      sub_surface_user_data.sunExposure = surface_user_data.sunExposure
      sub_surface_user_data.windExposure = surface_user_data.windExposure
      
      construction = sub_surface.construction
      if construction.is_initialized
        sub_surface_user_data.constructionName = construction.get.name.to_s
        sub_surface_user_data.constructionMaterialName = 'Construction_' + construction.get.name.to_s
      end     
      sub_surface_user_data.spaceName = surface_user_data.spaceName
      sub_surface_user_data.thermalZoneName = surface_user_data.thermalZoneName
      sub_surface_user_data.thermalZoneMaterialName = surface_user_data.thermalZoneMaterialName
      sub_surface_user_data.spaceTypeName = surface_user_data.spaceTypeName
      sub_surface_user_data.spaceTypeMaterialName = surface_user_data.spaceTypeMaterialName
      sub_surface_user_data.buildingStoryName = surface_user_data.buildingStoryName
      sub_surface_user_data.buildingStoryMaterialName = surface_user_data.buildingStoryMaterialName

      #vertices = []
      #surface.vertices.each do |v| 
      #  vertex = Vertex.new
      #  vertex.x = v.x
      #  vertex.y = v.y
      #  vertex.z = v.z
      #  vertices << vertex.to_h
      #end
      #sub_surface_user_data.vertices = vertices
      user_datas << sub_surface_user_data.to_h     
    end

    return [geometries, user_datas]
  end
  
  # turn a shading surface into geometries
  def self.make_shade_geometries(surface)
    geometries = []
    user_datas = []

    # get the transformation to site coordinates
    site_transformation = OpenStudio::Transformation.new
    planar_surface_group = surface.planarSurfaceGroup
    if not planar_surface_group.empty?
      site_transformation = planar_surface_group.get.siteTransformation
    end
    shading_surface_group = surface.shadingSurfaceGroup
    shading_surface_type = 'Building'
    space_name = nil
    thermal_zone_name = nil
    space_type_name = nil
    building_story_name = nil
    if not shading_surface_group.empty?
      shading_surface_type = shading_surface_group.get.shadingSurfaceType
      
      space = shading_surface_group.get.space
      if space.is_initialized
        space = space.get
        space_name = space.name.to_s
        
        thermal_zone = space.thermalZone
        if thermal_zone.is_initialized
          thermal_zone_name = thermal_zone.get.name.to_s
        end
        
        space_type = space.spaceType
        if space_type.is_initialized
          space_type_name = space_type.get.name.to_s
        end
        
        building_story = space.buildingStory
        if building_story.is_initialized
          building_story_name = building_story.get.name.to_s
        end
      end
    end
    
    # get the vertices
    surface_vertices = surface.vertices
    t = OpenStudio::Transformation::alignFace(surface_vertices)
    r = t.rotationMatrix
    tInv = t.inverse
    surface_vertices = OpenStudio::reverse(tInv*surface_vertices)

    # triangulate surface
    triangles = OpenStudio::computeTriangulation(surface_vertices, OpenStudio::Point3dVectorVector.new)
    if triangles.empty?
      puts "Failed to triangulate shading surface #{surface.name}"
      return geometries
    end

    all_vertices = []
    face_indices = []
    triangles.each do |vertices|
      vertices = site_transformation*t*vertices
      #normal = site_transformation.rotationMatrix*r*z

      # https://github.com/mrdoob/three.js/wiki/JSON-Model-format-3
      # 0 indicates triangle
      # 16 indicates triangle with normals
      face_indices << 0
      vertices.reverse_each do |vertex|
        face_indices << get_vertex_index(vertex, all_vertices)  
      end

      # convert to 1 based indices
      #face_indices.each_index {|i| face_indices[i] = face_indices[i] + 1}
    end

    data = GeometryData.new
    data.vertices = flatten_vertices(all_vertices)
    data.normals = [] 
    data.uvs = []
    data.faces = face_indices
    data.scale = 1
    data.visible = true
    data.castShadow = true
    data.receiveShadow = false
    data.doubleSided = true
    
    geometry = Geometry.new
    geometry.uuid = format_uuid(surface.handle)
    geometry.type = 'Geometry'
    geometry.data = data.to_h
    geometries << geometry.to_h
    
    surface_user_data = UserData.new
    surface_user_data.handle = format_uuid(surface.handle)
    surface_user_data.name = surface.name.to_s
    surface_user_data.coincidentWithOutsideObject = false
    
    surface_user_data.surfaceType = shading_surface_type + 'Shading'
    surface_user_data.surfaceTypeMaterialName = shading_surface_type + 'Shading'
  
    #surface_user_data.outsideBoundaryCondition = nil
    #surface_user_data.outsideBoundaryConditionObjectName = nil
    #surface_user_data.outsideBoundaryConditionObjectHandle = nil
    surface_user_data.sunExposure = 'SunExposed'
    surface_user_data.windExposure = 'WindExposed'
    
    construction = surface.construction
    if construction.is_initialized
      surface_user_data.constructionName = construction.get.name.to_s
      surface_user_data.constructionMaterialName = 'Construction_' + construction.get.name.to_s
    end
    
    if space_name
      surface_user_data.spaceName = space_name
    end
    if thermal_zone_name
      surface_user_data.thermalZoneName = thermal_zone_name
      surface_user_data.thermalZoneMaterialName = 'ThermalZone_' + thermal_zone_name
    end
    if space_type_name
      surface_user_data.spaceTypeName = space_type_name
      surface_user_data.spaceTypeMaterialName = 'SpaceType_' + space_type_name
    end
    if building_story_name
      surface_user_data.buildingStoryName = building_story_name
      surface_user_data.buildingStoryMaterialName = 'BuildingStory_' + building_story_name
    end

    #vertices = []
    #surface.vertices.each do |v| 
    #  vertex = Vertex.new
    #  vertex.x = v.x
    #  vertex.y = v.y
    #  vertex.z = v.z
    #  vertices << vertex.to_h
    #end
    #surface_user_data.vertices = vertices
    user_datas << surface_user_data.to_h

    return [geometries, user_datas]
  end  

  # turn an interior partition surface into geometries
  def self.make_interior_partition_geometries(surface)
    geometries = []
    user_datas = []

    # get the transformation to site coordinates
    site_transformation = OpenStudio::Transformation.new
    planar_surface_group = surface.planarSurfaceGroup
    if not planar_surface_group.empty?
      site_transformation = planar_surface_group.get.siteTransformation
    end
    interior_partition_surface_group = surface.interiorPartitionSurfaceGroup

    space_name = nil
    thermal_zone_name = nil
    space_type_name = nil
    building_story_name = nil
    if not interior_partition_surface_group.empty?

      space = interior_partition_surface_group.get.space
      if space.is_initialized
        space = space.get
        space_name = space.name.to_s
        
        thermal_zone = space.thermalZone
        if thermal_zone.is_initialized
          thermal_zone_name = thermal_zone.get.name.to_s
        end
        
        space_type = space.spaceType
        if space_type.is_initialized
          space_type_name = space_type.get.name.to_s
        end
        
        building_story = space.buildingStory
        if building_story.is_initialized
          building_story_name = building_story.get.name.to_s
        end
      end
    end
    
    # get the vertices
    surface_vertices = surface.vertices
    t = OpenStudio::Transformation::alignFace(surface_vertices)
    r = t.rotationMatrix
    tInv = t.inverse
    surface_vertices = OpenStudio::reverse(tInv*surface_vertices)

    # triangulate surface
    triangles = OpenStudio::computeTriangulation(surface_vertices, OpenStudio::Point3dVectorVector.new)
    if triangles.empty?
      puts "Failed to triangulate interior partition surface #{surface.name}"
      return geometries
    end

    all_vertices = []
    face_indices = []
    triangles.each do |vertices|
      vertices = site_transformation*t*vertices
      #normal = site_transformation.rotationMatrix*r*z

      # https://github.com/mrdoob/three.js/wiki/JSON-Model-format-3
      # 0 indicates triangle
      # 16 indicates triangle with normals
      face_indices << 0
      vertices.reverse_each do |vertex|
        face_indices << get_vertex_index(vertex, all_vertices)  
      end

      # convert to 1 based indices
      #face_indices.each_index {|i| face_indices[i] = face_indices[i] + 1}
    end

    data = GeometryData.new
    data.vertices = flatten_vertices(all_vertices)
    data.normals = [] 
    data.uvs = []
    data.faces = face_indices
    data.scale = 1
    data.visible = true
    data.castShadow = true
    data.receiveShadow = false
    data.doubleSided = true
    
    geometry = Geometry.new
    geometry.uuid = format_uuid(surface.handle)
    geometry.type = 'Geometry'
    geometry.data = data.to_h
    geometries << geometry.to_h
    
    surface_user_data = UserData.new
    surface_user_data.handle = format_uuid(surface.handle)
    surface_user_data.name = surface.name.to_s
    surface_user_data.coincidentWithOutsideObject = false
    
    surface_user_data.surfaceType = 'InteriorPartitionSurface'
    surface_user_data.surfaceTypeMaterialName = 'InteriorPartitionSurface'
  
    #surface_user_data.outsideBoundaryCondition = nil
    #surface_user_data.outsideBoundaryConditionObjectName = nil
    #surface_user_data.outsideBoundaryConditionObjectHandle = nil
    surface_user_data.sunExposure = 'NoSun'
    surface_user_data.windExposure = 'NoWind'
    
    construction = surface.construction
    if construction.is_initialized
      surface_user_data.constructionName = construction.get.name.to_s
      surface_user_data.constructionMaterialName = 'Construction_' + construction.get.name.to_s
    end
    
    if space_name
      surface_user_data.spaceName = space_name
    end
    if thermal_zone_name
      surface_user_data.thermalZoneName = thermal_zone_name
      surface_user_data.thermalZoneMaterialName = 'ThermalZone_' + thermal_zone_name
    end
    if space_type_name
      surface_user_data.spaceTypeName = space_type_name
      surface_user_data.spaceTypeMaterialName = 'SpaceType_' + space_type_name
    end
    if building_story_name
      surface_user_data.buildingStoryName = building_story_name
      surface_user_data.buildingStoryMaterialName = 'BuildingStory_' + building_story_name
    end

    #vertices = []
    #surface.vertices.each do |v| 
    #  vertex = Vertex.new
    #  vertex.x = v.x
    #  vertex.y = v.y
    #  vertex.z = v.z
    #  vertices << vertex.to_h
    #end
    #surface_user_data.vertices = vertices
    user_datas << surface_user_data.to_h

    return [geometries, user_datas]
  end  
  
  def self.identity_matrix
    return [1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1]
  end

  def self.build_scene(model)

    materials = build_materials(model)
    
    object = Hash.new
    object[:uuid] = format_uuid(OpenStudio::createUUID)
    object[:type] = 'Scene'
    object[:matrix] = identity_matrix
    object[:children] = []
    
    floor_material = materials.find {|m| m[:name] == 'Floor'}
    wall_material = materials.find {|m| m[:name] == 'Wall'}
    roof_material = materials.find {|m| m[:name] == 'RoofCeiling'}
    window_material = materials.find {|m| m[:name] == 'Window'}
    door_material = materials.find {|m| m[:name] == 'Door'}
    site_shading_material = materials.find {|m| m[:name] == 'SiteShading'}
    building_shading_material = materials.find {|m| m[:name] == 'BuildingShading'}
    space_shading_material = materials.find {|m| m[:name] == 'SpaceShading'}
    interior_partition_surface_material = materials.find {|m| m[:name] == 'InteriorPartitionSurface'}
    
    # loop over all surfaces
    all_geometries = []
    model.getSurfaces.each do |surface|

      material = nil
      surfaceType = surface.surfaceType.upcase
      if surfaceType == 'FLOOR'
        material = floor_material
      elsif surfaceType == 'WALL'
        material = wall_material
      elsif surfaceType == 'ROOFCEILING'
        material = roof_material  
      end
  
      geometries, user_datas = make_geometries(surface)
      geometries.each_index do |i| 
        geometry = geometries[i]
        user_data = user_datas[i]
        
        all_geometries << geometry

        scene_child = SceneChild.new
        scene_child.uuid = format_uuid(OpenStudio::createUUID) 
        scene_child.name = user_data[:name]
        scene_child.type = "Mesh"
        scene_child.geometry = geometry[:uuid]

        if i == 0
          # first geometry is base surface
          scene_child.material = material[:uuid]
        else
          # sub surface
          if /Window/.match(user_data[:surfaceType]) || /Glass/.match(user_data[:surfaceType]) 
            scene_child.material =  window_material[:uuid]
          else
            scene_child.material =  door_material[:uuid]
          end
        end
        
        scene_child.matrix = identity_matrix
        scene_child.userData = user_data
        object[:children] << scene_child.to_h
      end
      
    end
    
    # loop over all shading surfaces
    model.getShadingSurfaces.each do |surface|
  
      geometries, user_datas = make_shade_geometries(surface)
      geometries.each_index do |i| 
        geometry = geometries[i]
        user_data = user_datas[i]
        
        material = nil
        if /Site/.match(user_data[:surfaceType])
          material = site_shading_material
        elsif /Building/.match(user_data[:surfaceType]) 
          material = building_shading_material
        elsif /Space/.match(user_data[:surfaceType]) 
          material = space_shading_material
        end
        
        all_geometries << geometry

        scene_child = SceneChild.new
        scene_child.uuid = format_uuid(OpenStudio::createUUID) 
        scene_child.name = user_data[:name]
        scene_child.type = 'Mesh'
        scene_child.geometry = geometry[:uuid]
        scene_child.material = material[:uuid]
        scene_child.matrix = identity_matrix
        scene_child.userData = user_data
        object[:children] << scene_child.to_h
      end
      
    end    
    
    # loop over all interior partition surfaces
    model.getInteriorPartitionSurfaces.each do |surface|
  
      geometries, user_datas = make_interior_partition_geometries(surface)
      geometries.each_index do |i| 
        geometry = geometries[i]
        user_data = user_datas[i]
        
        material = interior_partition_surface_material

        all_geometries << geometry

        scene_child = SceneChild.new
        scene_child.uuid = format_uuid(OpenStudio::createUUID) 
        scene_child.name = user_data[:name]
        scene_child.type = 'Mesh'
        scene_child.geometry = geometry[:uuid]
        scene_child.material = material[:uuid]
        scene_child.matrix = identity_matrix
        scene_child.userData = user_data
        object[:children] << scene_child.to_h
      end
      
    end    
    
    #light = AmbientLight.new
    #light.uuid = "#{format_uuid(OpenStudio::createUUID)}"
    #light.type = "AmbientLight"
    #light.color = "0xFFFFFF".hex
    #light.matrix = identity_matrix
    #object[:children] << light.to_h
      
    scene = Scene.new
    scene.geometries = all_geometries
    scene.materials = materials
    scene.object = object

    return scene
  end
  
  
end