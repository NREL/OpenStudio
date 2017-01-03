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
