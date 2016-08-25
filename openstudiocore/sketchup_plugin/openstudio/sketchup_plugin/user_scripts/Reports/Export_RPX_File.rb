######################################################################
#  Copyright (c) 2008-2010, Alliance for Sustainable Energy.  
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

# Each user script is implemented within a class that derives from OpenStudio::Ruleset::UserScript
class ExportRPXFile < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Export RPX File"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    
    #make an argument for thermal_zone
    thermal_zone_handles = OpenStudio::StringVector.new
    thermal_zone_display_names = OpenStudio::StringVector.new
    model.getThermalZones.each do |thermal_zone|
      thermal_zone_handles << thermal_zone.handle.to_s
      thermal_zone_display_names << thermal_zone.name.to_s
    end
    thermal_zone = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("thermal_zone", thermal_zone_handles, thermal_zone_display_names, true)
    thermal_zone.setDisplayName("Thermal Zone to Export")
    result << thermal_zone
    
    save_path = OpenStudio::Ruleset::OSArgument::makePathArgument("save_path", false, "rpx", true)
    save_path.setDisplayName("Output Path")
    save_path.setDefaultValue("#{OpenStudio::Plugin.model_manager.model_interface.openstudio_name}.rpx")
    result << save_path
    
    return result
  end
  
  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)
        
    if not runner.validateUserArguments(arguments(model),user_arguments)  
      return false
    end
    
    thermal_zone = runner.getOptionalWorkspaceObjectChoiceValue("thermal_zone",user_arguments,model) #model is passed in because of argument type
    save_path = runner.getStringArgumentValue("save_path",user_arguments)

    if thermal_zone.empty?
      handle = runner.getStringArgumentValue("thermal_zone",user_arguments)
      if handle.empty?
        runner.registerError("No thermal zone was chosen.")
      else
        runner.registerError("The selected thermal zone with handle '#{handle}' was not found in the model. It may have been removed by another measure.")
      end
      return false
    else
      if not thermal_zone.get.to_ThermalZone.empty?
        thermal_zone = thermal_zone.get.to_ThermalZone.get
      else
        runner.registerError("Script Error - argument not showing up as thermal zone.")
        return false
      end
    end  
    
    site = model.getSite
    
    report_string = ""
    report_string << "ROOM, #{thermal_zone.name}, SI\n"
    report_string << "SITE, #{site.latitude}, #{site.longitude}, #{site.timeZone}, #{model.getSite.elevation}\n"

    # loop over spaces in thermal zone
    thermal_zone.spaces.each do |space|
      
      # loop over surfaces
      space.surfaces.each do |surface|
      
        # skip surfaces internal to this thermal zone
        
        # write surface
        # TODO: get real emittance value
        emittance = 0.9

        vertices = surface.vertices
        all_vertices = []
        vertices.each do |vertex|
          all_vertices << vertex.x
          all_vertices << vertex.y
          all_vertices << vertex.z
        end
        report_string << "S, #{surface.name}, #{emittance}, #{vertices.size}, #{all_vertices.join(', ')}\n"
        
        # loop over sub surfaces
        surface.subSurfaces.each do |sub_surface|
        
          # write sub surface
          # TODO: get real emittance value
          sub_surface_type = "P"
          emittance = 0.9
          subSurfaceType = sub_surface.subSurfaceType
          if (subSurfaceType == "FixedWindow" || subSurfaceType == "FixedWindow" || subSurfaceType == "GlassDoor" || subSurfaceType == "Skylight")
            sub_surface_type = "W"
            emittance = 0.84
          end
  
          vertices = sub_surface.vertices
          all_vertices = []
          vertices.each do |vertex|
            all_vertices << vertex.x
            all_vertices << vertex.y
            all_vertices << vertex.z
          end

          report_string << "#{sub_surface_type}, #{sub_surface.name}, #{emittance}, #{vertices.size}, #{all_vertices.join(', ')}\n"
        
        end
      end
    end
    
    # TODO: see if there is output data available to get surface temperatures, etc
    # CASE, <CaseDesc> ;
    # RC, <RoomAirTemp (C)>,<RoomHumidity (0-1)>,<RoomPressure (Pa)>, <Surf1T>, <SurfT>, etc

    # write file
    File.open(save_path, 'w') do |file|
      file << report_string
    end    
    
    return(true)
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
ExportRPXFile.new.registerWithApplication
