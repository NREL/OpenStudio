########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

require("openstudio/sketchup_plugin/lib/observers/MaterialsObserver.rb")

module OpenStudio

  RenderWaiting = -1
  RenderByClass = 0
  RenderByDataValue = 1
  RenderByBoundary = 2
  RenderByLayer = 3
  RenderBySurfaceNormal = 4
  RenderByConstruction = 5
  RenderBySpaceType = 6
  RenderByThermalZone = 7
  RenderByBuildingStory = 8

  class MaterialsInterface

    #for surface type render mode
    attr_reader :floor_ext, :floor_int, :wall_ext, :wall_int, :roof_ext, :roof_int, :window_ext, :window_int, :door_ext, :door_int
    attr_reader :space_shading, :building_shading, :site_shading, :interior_partition_surface
    attr_reader :space_shading_back, :building_shading_back, :site_shading_back, :interior_partition_surface_back

    #for boundary render mode
    attr_reader :surface_ext, :adiabatic_ext, :space_ext, :ground_ext, :groundfcfactormethod_ext, :groundslabpreprocessoraverage_ext
    attr_reader :groundslabpreprocessorcore_ext, :groundslabpreprocessorperimeter_ext, :groundbasementpreprocessoraveragewall_ext
    attr_reader :groundbasementpreprocessoraveragefloor_ext, :groundbasementpreprocessorupperwall_ext, :groundbasementpreprocessorlowerwall_ext
    attr_reader :othersidecoefficients_ext, :othersideconditionsmodel_ext
    attr_reader :surface_int, :adiabatic_int, :space_int, :outdoors_int, :ground_int, :groundfcfactormethod_int, :groundslabpreprocessoraverage_int
    attr_reader :groundslabpreprocessorcore_int, :groundslabpreprocessorperimeter_int, :groundbasementpreprocessoraveragewall_int
    attr_reader :groundbasementpreprocessoraveragefloor_int, :groundbasementpreprocessorupperwall_int, :groundbasementpreprocessorlowerwall_int
    attr_reader :othersidecoefficients_int, :othersideconditionsmodel_int
    attr_reader :outdoors_ext, :outdoorssun_ext, :outdoorswind_ext, :outdoorssunwind_ext
    attr_reader :outdoors_int, :outdoorssun_int, :outdoorswind_int, :outdoorssunwind_int
    attr_reader :subext_ext, :subext_int, :subint_ext, :subint_int
    attr_reader :air_wall, :solar_collector, :photovoltaic

    # for testing
    attr_accessor :observer

    def initialize(model_interface)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      @model_interface = model_interface
      @materials = @model_interface.skp_model.materials

      @observer = MaterialsObserver.new(self)
      @observer_added = false # true if observer has been added to the entity

      # rendering settings
      @render_defaults = false
      @rendering_mode = 0

      # Materials must be 'in model' to be used on a face
      # 'in model" materials change when a new SU file is created, or an old SU file is opened

      @floor_ext = get_material("OpenStudio_Floor_Ext", Sketchup::Color.new(128, 128, 128, 1.0))  # old DXF: "Gray"
      @floor_int = get_material("OpenStudio_Floor_Int", Sketchup::Color.new(191, 191, 191, 1.0))

      @wall_ext = get_material("OpenStudio_Wall_Ext", Sketchup::Color.new(204, 178, 102, 1.0))  # old DXF: "Lime"
      @wall_int = get_material("OpenStudio_Wall_Int", Sketchup::Color.new(235, 226, 197, 1.0))

      @roof_ext = get_material("OpenStudio_Roof_Ext", Sketchup::Color.new(153, 76, 76, 1.0))  # old DXF: "Yellow"
      @roof_int = get_material("OpenStudio_Roof_Int", Sketchup::Color.new(202, 149, 149, 1.0))

      @window_ext = get_material("OpenStudio_Window_Ext", Sketchup::Color.new(102, 178, 204, 0.6))  # old DXF: "Cyan"
      @window_int = get_material("OpenStudio_Window_Int", Sketchup::Color.new(192, 226, 235, 0.6))

      @door_ext = get_material("OpenStudio_Door_Ext", Sketchup::Color.new(153, 133, 76, 1.0))  # old DXF: "Cyan"
      @door_int = get_material("OpenStudio_Door_Int", Sketchup::Color.new(202, 188, 149, 1.0))

      # The detached colors may be breaking from the new DXF color scheme

      @site_shading = get_material("OpenStudio_Site_Shading", Sketchup::Color.new(75, 124, 149, 1.0))
      @site_shading_back = get_material("OpenStudio_Site_Shading_back", Sketchup::Color.new(187, 209, 220, 1.0))  # old DXF: "Blue"

      @building_shading = get_material("OpenStudio_Building_Shading", Sketchup::Color.new(113, 76, 153, 1.0))  # old DXF: "Light Gray"
      @building_shading_back = get_material("OpenStudio_Building_Shading_back", Sketchup::Color.new(216, 203, 229, 1.0))

      @space_shading = get_material("OpenStudio_Space_Shading", Sketchup::Color.new(76, 110, 178, 1.0))  # old DXF: "Magenta"
      @space_shading_back = get_material("OpenStudio_Space_Shading_back", Sketchup::Color.new(183, 197, 224, 1.0))  # old DXF: "Magenta"

      @interior_partition_surface = get_material("OpenStudio_Interior_Partition_Surface", Sketchup::Color.new(158, 188, 143, 1.0))   # "Yellow"
      @interior_partition_surface_back = get_material("OpenStudio_Interior_Partition_Surface_back", Sketchup::Color.new(213, 226, 207, 1.0))

      # start textures for boundary conditions
      @surface_ext = get_material("OpenStudio_Surface_Ext", Sketchup::Color.new(0, 153, 0, 1.0))
      @surface_int = get_material("OpenStudio_Surface_Int", Sketchup::Color.new(0, 153, 0, 1.0))

      @adiabatic_ext = get_material("OpenStudio_Adiabatic_Ext", Sketchup::Color.new(255, 101, 178, 1.0))
      @adiabatic_int = get_material("OpenStudio_Adiabatic_Int", Sketchup::Color.new(255, 101, 178, 1.0))

      @space_ext = get_material("OpenStudio_Space_Ext", Sketchup::Color.new(255, 0, 0, 1.0))
      @space_int = get_material("OpenStudio_Space_Int", Sketchup::Color.new(255, 0, 0, 1.0))

      @outdoors_ext = get_material("OpenStudio_Outdoors_Ext", Sketchup::Color.new(163, 204, 204, 1.0))
      @outdoors_int = get_material("OpenStudio_Outdoors_Int", Sketchup::Color.new(163, 204, 204, 1.0))

      @outdoorssun_ext = get_material("OpenStudio_Outdoorssun_Ext", Sketchup::Color.new(40, 204, 204, 1.0))
      @outdoorssun_int = get_material("OpenStudio_Outdoorssun_Int", Sketchup::Color.new(40, 204, 204, 1.0))

      @outdoorswind_ext = get_material("OpenStudio_Outdoorswind_Ext", Sketchup::Color.new(9, 159, 162, 1.0))
      @outdoorswind_int = get_material("OpenStudio_Outdoorswind_Int", Sketchup::Color.new(9, 159, 162, 1.0))

      @outdoorssunwind_ext = get_material("OpenStudio_Outdoorssunwind_Ext", Sketchup::Color.new(68, 119, 161, 1.0))
      @outdoorssunwind_int = get_material("OpenStudio_Outdoorssunwind_Int", Sketchup::Color.new(68, 119, 161, 1.0))

      @ground_ext = get_material("OpenStudio_Ground_Ext", Sketchup::Color.new(204, 183, 122, 1.0))
      @ground_int = get_material("OpenStudio_Ground_Int", Sketchup::Color.new(204, 183, 122, 1.0))

      @groundfcfactormethod_ext = get_material("OpenStudio_Groundfcfactormethod_Ext", Sketchup::Color.new(153, 122, 30, 1.0))
      @groundfcfactormethod_int = get_material("OpenStudio_Groundfcfactormethod_Int", Sketchup::Color.new(153, 122, 30, 1.0))

      @groundslabpreprocessoraverage_ext = get_material("OpenStudio_Groundslabpreprocessoraverage_Ext", Sketchup::Color.new(255, 191, 0, 1.0))
      @groundslabpreprocessoraverage_int = get_material("OpenStudio_Groundslabpreprocessoraverage_Int", Sketchup::Color.new(255, 191, 0, 1.0))

      @groundslabpreprocessorcore_ext = get_material("OpenStudio_Groundslabpreprocessorcore_Ext", Sketchup::Color.new(255, 182, 50, 1.0))
      @groundslabpreprocessorcore_int = get_material("OpenStudio_Groundslabpreprocessorcore_Int", Sketchup::Color.new(255, 182, 50, 1.0))

      @groundslabpreprocessorperimeter_ext = get_material("OpenStudio_Groundslabpreprocessorperimeter_Ext", Sketchup::Color.new(255, 178, 101, 1.0))
      @groundslabpreprocessorperimeter_int = get_material("OpenStudio_Groundslabpreprocessorperimeter_Int", Sketchup::Color.new(255, 178, 101, 1.0))

      @groundbasementpreprocessoraveragewall_ext = get_material("OpenStudio_Groundbasementpreprocessoraveragewall_Ext", Sketchup::Color.new(204, 51, 0, 1.0))
      @groundbasementpreprocessoraveragewall_int = get_material("OpenStudio_Groundbasementpreprocessoraveragewall_Int", Sketchup::Color.new(204, 51, 0, 1.0))

      @groundbasementpreprocessoraveragefloor_ext = get_material("OpenStudio_Groundbasementpreprocessoraveragefloor_Ext", Sketchup::Color.new(204, 81, 40, 1.0))
      @groundbasementpreprocessoraveragefloor_int = get_material("OpenStudio_Groundbasementpreprocessoraveragefloor_Int", Sketchup::Color.new(204, 81, 40, 1.0))

      @groundbasementpreprocessorupperwall_ext = get_material("OpenStudio_Groundbasementpreprocessorupperwall_Ext", Sketchup::Color.new(204, 112, 81, 1.0))
      @groundbasementpreprocessorupperwall_int = get_material("OpenStudio_Groundbasementpreprocessorupperwall_Int", Sketchup::Color.new(204, 112, 81, 1.0))

      @groundbasementpreprocessorlowerwall_ext = get_material("OpenStudio_Groundbasementpreprocessorlowerwall_Ext", Sketchup::Color.new(204, 173, 163, 1.0))
      @groundbasementpreprocessorlowerwall_int = get_material("OpenStudio_Groundbasementpreprocessorlowerwall_Int", Sketchup::Color.new(204, 173, 163, 1.0))

      @othersidecoefficients_ext = get_material("OpenStudio_Othersidecoefficients_Ext", Sketchup::Color.new(63, 63, 63, 1.0))
      @othersidecoefficients_int = get_material("OpenStudio_Othersidecoefficients_Int", Sketchup::Color.new(63, 63, 63, 1.0))

      @othersideconditionsmodel_ext = get_material("OpenStudio_Othersideconditionsmodel_Ext", Sketchup::Color.new(153, 0, 76, 1.0))
      @othersideconditionsmodel_int = get_material("OpenStudio_Othersideconditionsmodel_Int", Sketchup::Color.new(153, 0, 76, 1.0))

      # end textures for boundary conditions

      # start textures for boundary conditions - subsurfaces

      @subext_ext = get_material("OpenStudio_SubExt_Ext", Sketchup::Color.new(111, 157, 194, 1.0))
      @subext_int = get_material("OpenStudio_SubExt_Int", Sketchup::Color.new(111, 157, 194, 1.0))

      @subint_ext = get_material("OpenStudio_SubInt_Ext", Sketchup::Color.new(38, 216, 38, 1.0))
      @subint_int = get_material("OpenStudio_SubInt_Int", Sketchup::Color.new(38, 216, 38, 1.0))

      # end textures for boundary conditions - subsurfaces

      # todo: get smarter about this for different rendering modes
      @air_wall = @model_interface.skp_model.materials["OpenStudio_AirWall"]
      if (@air_wall.nil?)
        @air_wall = @model_interface.skp_model.materials.add("OpenStudio_AirWall")
        @air_wall.texture = Plugin.dir + "/lib/resources/AirWall.png"
        @air_wall.texture.size = 180
        @air_wall.alpha = 0.25
      end

      @solar_collector = @model_interface.skp_model.materials["OpenStudio_SolarCollector"]
      if (@solar_collector.nil?)
        @solar_collector = @model_interface.skp_model.materials.add("OpenStudio_SolarCollector")
        @solar_collector.texture = Plugin.dir + "/lib/resources/SolarCollector.png"
        @solar_collector.texture.size = 40
        @solar_collector.alpha = 1.0
      end

      @photovoltaic = @model_interface.skp_model.materials["OpenStudio_Photovoltaic"]
      if (@photovoltaic.nil?)
        @photovoltaic = @model_interface.skp_model.materials.add("OpenStudio_Photovoltaic")
        @photovoltaic.texture = Plugin.dir + "/lib/resources/Photovoltaic.png"
        @photovoltaic.texture.size = 40
        @photovoltaic.alpha = 1.0
      end

    end

    def destroy
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      @model_interface = nil
      @materials = nil
      @observer = nil
    end

    def valid_entity?
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      return((not @materials.nil?) and (not @materials.deleted?) and @materials.valid? and (@materials.entityID > 0))
    end

    def add_observers(recursive = false)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      if $OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OBSERVERS
        if not @observer_added
          @materials.add_observer(@observer)
          @observer_added = true
        end
        @observer.enable
      else
        @materials.add_observer(@observer)
        @observer_added = true
        @observer.enable
      end

      if recursive
        @model_interface.rendering_colors.each { |interface| interface.add_observers(recursive) }
      end

    end

    def remove_observers(recursive = false)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      had_observers = false
      if (valid_entity?)
        if $OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OBSERVERS
          if @observer_added
            had_observers = @observer.disable
          end
        else
          had_observers = @materials.remove_observer(@observer)
          @observer.disable
          @observer_added = false
        end
      end

      if recursive
        @model_interface.rendering_colors.each { |interface| interface.remove_observers(recursive) }
      end

      return had_observers

    end

    def destroy_observers(recursive = false)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      result = false
      if @observer
        if (valid_entity?)
          if $OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OBSERVERS
            # actually do remove here
            @materials.remove_observer(@observer)
            @observer.disable
            @observer_added = false
          else
            @materials.remove_observer(@observer)
            @observer.disable
            @observer_added = false
          end
        end
        @observer.destroy
        @observer = nil
        result = true

        if recursive
          @model_interface.rendering_colors.each { |interface| interface.destroy_observers(recursive) }
        end
      end

      return result

    end

    def get_material(name, color)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      material = @model_interface.skp_model.materials[name]
      if (material.nil?)
        material = @model_interface.skp_model.materials.add(name)
        material.color = color
        material.alpha = color.alpha / 255.0
      end
      return(material)
    end

    def render_defaults
      #Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      return @render_defaults
    end

    # DLM: can we remove this?
    #def render_defaults=(render_defaults)
    #  #Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #
    #  @render_defaults = render_defaults
    #
    #  @model_interface.request_paint
    #end

    def rendering_mode
      #Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      return @rendering_mode
    end

    def rendering_mode=(rendering_mode)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      if rendering_mode == RenderByConstruction or rendering_mode == RenderBySpaceType
        if @rendering_mode == rendering_mode
          # toggle render defaults
          @render_defaults = (not @render_defaults)
        else
          # set render defaults back to true
          @render_defaults = true
        end
      end

      @rendering_mode = rendering_mode

      rendering_options = @model_interface.skp_model.rendering_options

      if @rendering_mode == RenderWaiting
        rendering_options["FaceFrontColor"] = "white"
        rendering_options["FaceBackColor"] = "white"
        rendering_options["RenderMode"] = 0  # wire frame
        rendering_options["DisplayColorByLayer"] = false
      elsif @rendering_mode == RenderBySurfaceNormal
        rendering_options["FaceFrontColor"] = "white"
        rendering_options["FaceBackColor"] = "red"
        rendering_options["RenderMode"] = 5  # render by front/back
        rendering_options["DisplayColorByLayer"] = false
      elsif @rendering_mode == RenderByLayer
        rendering_options["FaceFrontColor"] = "white"
        rendering_options["FaceBackColor"] = Sketchup::Color.new(171, 176, 204, 255) # Sketchup default
        rendering_options["RenderMode"] = 2 # render by material
        rendering_options["DisplayColorByLayer"] = true
      else
        rendering_options["FaceFrontColor"] = "white"
        rendering_options["FaceBackColor"] = Sketchup::Color.new(171, 176, 204, 255) # Sketchup default
        rendering_options["RenderMode"] = 2 # render by material
        rendering_options["DisplayColorByLayer"] = false
      end

      dialog_manager = Plugin.dialog_manager
      if dialog_manager
        dialog_manager.selection_changed
      end

      @model_interface.request_paint

    end

  end

end
