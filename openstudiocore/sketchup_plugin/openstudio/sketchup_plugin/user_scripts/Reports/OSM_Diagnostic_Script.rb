########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

# Each user script is implemented within a class that derives from OpenStudio::Ruleset::UserScript
class DiagnosticScript < OpenStudio::Ruleset::UtilityUserScript

  # override name to return the name of your script
  def name
    return "OSM Diagnostic Script"
  end

  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments()
    result = OpenStudio::Ruleset::OSArgumentVector.new

    open_path = OpenStudio::Ruleset::OSArgument::makePathArgument("open_path", true, "osm")
    open_path.setDisplayName("Select OSM File to Run Diagnostics On ")
    result << open_path

    remove_errors = OpenStudio::Ruleset::OSArgument::makeBoolArgument("remove_errors",false)
    remove_errors.setDisplayName("Remove Objects with Errors? ")
    remove_errors.setDefaultValue(true)
    result << remove_errors

    remove_warnings = OpenStudio::Ruleset::OSArgument::makeBoolArgument("remove_warnings",false)
    remove_warnings.setDisplayName("Remove Objects with Warnings? ")
    remove_warnings.setDefaultValue(false)
    result << remove_warnings

    begin
      SKETCHUP_CONSOLE.show
    rescue => e
    end

    return result
  end

  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(runner, user_arguments)
    super(runner, user_arguments)
    
    if not runner.validateUserArguments(arguments,user_arguments)
      return false
    end
    
    open_path = runner.getStringArgumentValue("open_path",user_arguments)
    remove_errors = runner.getBoolArgumentValue("remove_errors",user_arguments)
    remove_warnings = runner.getBoolArgumentValue("remove_warnings",user_arguments)

    require 'openstudio'

    puts ""
    puts ">>diagnostic test start"

    puts ""
    puts "File - " + open_path

    idfFile = OpenStudio::IdfFile::load(OpenStudio::Path.new(open_path)).get
    workspace = OpenStudio::Workspace.new(idfFile, "None".to_StrictnessLevel)

    # Open OSM file
    model = OpenStudio::Model::Model::load(OpenStudio::Path.new(open_path)).get

    puts "Model has " + model.numObjects.to_s + " objects"

    # number of thermal zones
    thermal_zones = model.getThermalZones
    puts "Model has " + thermal_zones.size.to_s + " thermal zones"

    # number of spaces
    spaces = model.getSpaces
    puts "Model has " + spaces.size.to_s + " spaces"

    # number of surfaces
    surfaces = model.getPlanarSurfaces
    # puts "Model has " + surfaces.size.to_s + " planar surfaces"

    # number of base surfaces
    base_surfaces = model.getSurfaces
    puts "Model has " + base_surfaces.size.to_s + " base surfaces"

    # number of base surfaces
    sub_surfaces = model.getSubSurfaces
    puts "Model has " + sub_surfaces.size.to_s + " sub surfaces"

    # number of surfaces
    shading_surfaces = model.getShadingSurfaces
    puts "Model has " + shading_surfaces.size.to_s + " shading surfaces"

    # number of surfaces
    partition_surfaces = model.getInteriorPartitionSurfaces
    puts "Model has " + partition_surfaces.size.to_s + " interior partition surfaces"
    
#    puts ""
#    puts "Outputting Draft Validity Report"
#    puts workspace.validityReport("Draft".to_StrictnessLevel)
#
#    puts ""
#    puts "Outputting Final Validity Report (note: some errors are expected here)"
#    puts workspace.validityReport("Final".to_StrictnessLevel)

    savediagnostic = false # this will change to true later in script if necessary

    puts ""
    puts "Removing catchall objects (objects unknown to your version of OpenStudio)"
    switch = 0
    model.getObjectsByType("Catchall".to_IddObjectType).each do |obj|
      puts "*(error) '" + obj.name.to_s + "' object type is unkown to OpenStudio"
      switch = 1
      if remove_errors
        puts "**(removing object)  '#{obj.name.to_s}'"
        remove = obj.remove
        savediagnostic = true
      end
    end
    if switch == 0 then puts "none" end

    puts ""
    puts "Removing objects that fail draft validity test"
    switch = 0
    model.objects.each do |object|
      if !object.isValid("Draft".to_StrictnessLevel)
        report = object.validityReport("Draft".to_StrictnessLevel)
        puts "*(error)" + report.to_s
        switch = 1
        if remove_errors
          puts "**(removing object)  '#{object.name}'"
          remove = object.remove
          savediagnostic = true
        end
      end
    end
    if switch == 0 then puts "none" end

    base_surfaces = model.getSurfaces
    # Find base surfaces with less than three vertices
    puts ""
    puts "Surfaces with less than three vertices"
    switch = 0
    base_surfaces.each do |base_surface|
      vertices = base_surface.vertices
      if vertices.size < 3
        puts "*(warning) '" + base_surface.name.to_s + "' has less than three vertices"
        switch = 1
        if remove_errors
          puts "**(removing object) '#{base_surface.name.to_s}'"
          # remove surfaces with less than three vertices
          remove = base_surface.remove
          savediagnostic = true
        end
      end
    end
    if switch == 0 then puts "none" end

    sub_surfaces = model.getSubSurfaces
    # Find base sub-surfaces with less than three vertices
    puts ""
    puts "Surfaces with less than three vertices"
    switch = 0
    sub_surfaces.each do |sub_surface|
      vertices = sub_surface.vertices
      if vertices.size < 3
        puts "*(warning) '" + sub_surface.name.to_s + "' has less than three vertices"
        switch = 1
        if remove_errors
          puts "**(removing object) '#{sub_surface.name.to_s}'"
          # remove sub-surfaces with less than three vertices
          remove = sub_surface.remove
          savediagnostic = true
        end
      end
    end
    if switch == 0 then puts "none" end

    surfaces = model.getSurfaces
    # Find surfaces with greater than 25 vertices (split out sub-surfaces and test if they hvae more than 4 vertices)
    puts ""
    puts "Surfaces with more than 25 vertices"
    switch = 0
    surfaces.each do |surface|
      vertexcount = surface.vertices.size
      if vertexcount > 25
        puts "*(info) '" + surface.name.to_s + "' has " + vertexcount.to_s + " vertices"
        switch = 1
      end
    end
    if switch == 0 then puts "none" end

    base_surfaces = model.getSurfaces
    # Find base surfaces with area < 0.1
    puts ""
    puts "Surfaces with area less than 0.1 m^2"
    switch = 0
    base_surfaces.each do |base_surface|
      grossarea = base_surface.grossArea
      if grossarea < 0.1
        puts "*(warning) '" + base_surface.name.to_s + "' has area of " + grossarea.to_s + " m^2"
        switch = 1
        if remove_warnings
          puts "**(removing object) '#{base_surface.name.to_s}'"
          # remove base surfaces with less than 0.1 m^2
          remove = base_surface.remove
          savediagnostic = true
        end
      end
    end
    if switch == 0 then puts "none" end

    sub_surfaces = model.getSubSurfaces
    # Find sub-surfaces with area < 0.1
    puts ""
    puts "Surfaces with area less than 0.1 m^2"
    switch = 0
    sub_surfaces.each do |sub_surface|
      grossarea = sub_surface.grossArea
      if grossarea < 0.1
        puts "*(warning) '" + sub_surface.name.to_s + "' has area of " + grossarea.to_s + " m^2"
        switch = 1
        if remove_warnings
          puts "**(removing object) '#{sub_surface.name.to_s}'"
          # remove sub-surfaces with less than three vertices
          remove = sub_surface.remove
          savediagnostic = true
        end
      end
    end
    if switch == 0 then puts "none" end

    # Find surfaces within surface groups that share same vertices
    puts ""
    puts "Surfaces and SubSurfaces which have similar geometry within same surface group"
    switch = 0
    planar_surface_groups = model.getPlanarSurfaceGroups
    planar_surface_groups.each do |planar_surface_group|

      planar_surfaces = []
      planar_surface_group.children.each do |child|
        planar_surface = child.to_PlanarSurface
        next if planar_surface.empty?
        planar_surfaces << planar_surface.get
      end

      n = planar_surfaces.size

      sub_surfaces = []
      (0...n).each do |k|
        planar_surfaces[k].children.each do |l|
          sub_surface = l.to_SubSurface
          next if sub_surface.empty?
          sub_surfaces << sub_surface.get
        end
      end

      all_surfaces = []
      sub_surfaces.each do |m|  # subsurfaces first so they get removed vs. base surface
        all_surfaces << m
      end
      planar_surfaces.each do |n|
        all_surfaces << n
      end

      n2 = all_surfaces.size # updated with sub-surfaces added at the beginning
      surfaces_to_remove = Hash.new
      (0...n2).each do |i|
        (i+1...n2).each do |j|

          p1 = all_surfaces[i]
          p2 = all_surfaces[j]
       
          if p1.equalVertices(p2) or p1.reverseEqualVertices(p2)
            switch = 1
            puts "*(error) '#{p1.name.to_s}' has similar geometry to '#{p2.name.to_s}' in the surface group named '#{planar_surface_group.name.to_s}'"
            if remove_errors
              puts "**(removing object) '#{p1.name.to_s}'" # remove p1 vs. p2 to avoid failure if three or more similar surfaces in a group
              # don't remove here, just mark to remove
              surfaces_to_remove[p1.handle.to_s] = p1
              savediagnostic = true
            end
          end
        end
      end
      surfaces_to_remove.each_pair {|handle, surface| surface.remove}

    end
    if switch == 0 then puts "none" end
  
    # Find duplicate vertices within surface
    puts ""
    puts "Surfaces and SubSurfaces which have duplicate vertices"
    switch = 0
    planar_surface_groups = model.getPlanarSurfaceGroups
    planar_surface_groups.each do |planar_surface_group|

      planar_surfaces = []
      planar_surface_group.children.each do |child|
        planar_surface = child.to_PlanarSurface
        next if planar_surface.empty?
        planar_surfaces << planar_surface.get
      end

      n = planar_surfaces.size

      sub_surfaces = []
      (0...n).each do |k|
        planar_surfaces[k].children.each do |l|
          sub_surface = l.to_SubSurface
          next if sub_surface.empty?
          sub_surfaces << sub_surface.get
        end
      end

      all_surfaces = []
      sub_surfaces.each do |m|  # subsurfaces first so they get removed vs. base surface
        all_surfaces << m
      end
      planar_surfaces.each do |n|
        all_surfaces << n
      end

    all_surfaces.each do |surface|
    # make array of vertices
    vertices = surface.vertices
    
      # loop through looking for duplicates
      n2 = vertices.size
      switch2 = 0

      (0...n2).each do |i|
      (i+1...n2).each do |j|

        p1 = vertices[i]
        p2 = vertices[j]
       
       # set flag if surface needs be removed
            
       if p1.x == p2.x and p1.y == p2.y and p1.z == p2.z
        switch2 = 1
        end

      end
      end
    
      if switch2 == 1
        switch == 1
        puts "*(error) '#{surface.name.to_s}' has duplicate vertices"
        if remove_errors
          puts "**(removing object) '#{surface.name.to_s}'" # remove p1 vs. p2 to avoid failure if three or more similar surfaces in a group
          remove = surface.remove
          savediagnostic = true
        end
      end
    
    end

    end
    if switch == 0 then puts "none" end
    
    # find and remove orphan sizing:zone objects
    puts ""
    puts "Removing sizing:zone objects that are not connected to any thermal zone"
    #get all sizing:zone objects in the model
    sizing_zones = model.getObjectsByType("OS:Sizing:Zone".to_IddObjectType)
    #make an array to store the names of the orphan sizing:zone objects
    orphaned_sizing_zones = Array.new
    #loop through all sizing:zone objects, checking for missing ThermalZone field
    sizing_zones.each do |sizing_zone|
      sizing_zone = sizing_zone.to_SizingZone.get
      if sizing_zone.isEmpty(1)
        orphaned_sizing_zones << sizing_zone.handle
        puts "*(error)#{sizing_zone.name} is not connected to a thermal zone"
        if remove_errors
          puts "**(removing object)#{sizing_zone.name} is not connected to a thermal zone"
          sizing_zone.remove
          savediagnostic = true
        end
      end
    end
    #summarize the results
    if orphaned_sizing_zones.length > 0
      puts "#{orphaned_sizing_zones.length} orphaned sizing:zone objects were found"
    else
      puts "no orphaned sizing:zone objects were found"
    end
    
    # find and remove orphan ZoneHVAC:EquipmentList objects
    puts ""
    puts "Removing ZoneHVAC:EquipmentList objects that are not connected to any thermal zone"
    #get all ZoneHVAC:EquipmentList objects in the model
    zonehvac_equipmentlists = model.getObjectsByType("OS:ZoneHVAC:EquipmentList".to_IddObjectType)
    #make an array to store the names of the orphan ZoneHVAC:EquipmentList objects
    orphaned_zonehvac_equipmentlists = Array.new
    #loop through all ZoneHVAC:EquipmentList objects, checking for missing ThermalZone field
    zonehvac_equipmentlists.each do |zonehvac_equipmentlist|
      if zonehvac_equipmentlist.isEmpty(2)
        orphaned_zonehvac_equipmentlists << zonehvac_equipmentlist.handle
        puts "*(error)#{zonehvac_equipmentlist.name} is not connected to a thermal zone"
        if remove_errors
          puts "**(removing object)#{zonehvac_equipmentlist.name} is not connected to a thermal zone"
          zonehvac_equipmentlist.remove
          savediagnostic = true
        end
      end
    end
    #summarize the results
    if orphaned_zonehvac_equipmentlists.length > 0
      puts "#{orphaned_zonehvac_equipmentlists.length} orphaned ZoneHVAC:EquipmentList objects were found"
    else
      puts "no orphaned ZoneHVAC:EquipmentList objects were found"
    end
    
    # find and remove orphan PortList objects
    puts ""
    puts "Removing PortList objects that are not connected to any equipment"
    #get all PortList objects in the model
    port_lists = model.getObjectsByType("OS:PortList".to_IddObjectType)
    #make an array to store the names of the orphan PortList objects
    orphaned_port_lists = Array.new
    #loop through all PortList objects, checking for missing HVAC Component field
    port_lists.each do |port_list|
      if port_list.isEmpty(2)
        orphaned_port_lists << port_list.handle
        puts "*(error)#{port_list.name} is not connected to any equipment"
        if remove_errors
          puts "**(removing object)#{port_list.name} is not connected to any equipment"
          port_list.remove
          savediagnostic = true
        end
      end
    end
    #summarize the results
    if orphaned_port_lists.length > 0
      puts "#{orphaned_port_lists.length} orphaned PortList objects were found"
    else
      puts "no orphaned PortList objects were found"
    end
  
    # find and remove orphan LifeCycleCost objects
    puts ""
    puts "Removing LifeCycleCost objects that are not connected to any model object"
    #get all LifeCycleCost objects in the model
    lcc_objects = model.getObjectsByType("OS:LifeCycleCost".to_IddObjectType)
    #make an array to store the names of the orphan LifeCycleCost objects
    orphaned_lcc_objects = Array.new
    #loop through all LifeCycleCost objects, checking for missing Item Name
    lcc_objects.each do |lcc_object|
      if lcc_object.isEmpty(4)
        orphaned_lcc_objects << lcc_object.handle
        puts "*(error)#{lcc_object.name} is not connected to any object"
        if remove_errors
          puts "**(removing object)#{lcc_object.name} is not connected to any model object"
          lcc_object.remove
          savediagnostic = true
        end
      end
    end
    #summarize the results
    if orphaned_lcc_objects.length > 0
      puts "#{orphaned_lcc_objects.length} orphaned LifeCycleCost objects were found"
    else
      puts "no orphaned LifeCycleCost objects were found"
    end
    
    puts ""
    puts ">>diagnostic test complete"

    if savediagnostic
      newfilename = open_path.gsub(".osm","_diagnostic.osm")
      if File.exists? newfilename
        # I would like to add a prompt to ask the user if they want to overwrite their file
      end
      puts ""
      puts ">>saving temporary diagnostic version " + newfilename
      model.save(OpenStudio::Path.new(newfilename),true)

    end

    runner.registerFinalCondition("Diagnostic Test Complete")
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
DiagnosticScript.new.registerWithApplication
