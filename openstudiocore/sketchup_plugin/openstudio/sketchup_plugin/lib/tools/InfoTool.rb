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

require("openstudio/sketchup_plugin/lib/tools/Tool")


module OpenStudio

  class InfoTool < Tool
    # Features To Add:
    #   identify groups as spaces (use pickhelper) -- difficult
    #   doubleclick to open a group -- not possible in the API
    #   possible allow changing of selection
    #   with Ctrl key down, it displays construction objects!  --- used to work

    def initialize
      @cursor = UI.create_cursor(Plugin.dir + "/lib/resources/icons/InfoToolCursor-16x17.tiff", 1, 1)
      @flags = 0
    end


    def onMouseMove(flags, x, y, view)
      super

      # Should apply user's precision setting here
      # Also:  show relative coordinates?
      Sketchup.set_status_text("World Coordinates:  " + @ip.position.to_s)

      # flags are good here, reset them
      @flags = flags

      view.tooltip = get_tooltip(@ip.face, flags, view)
    end


    def onKeyDown(key, repeat, flags, view)

      # seem to get bad value for flags here
      if key == CONSTRAIN_MODIFIER_KEY # Shift Key
        if not ((@flags & CONSTRAIN_MODIFIER_MASK) > 0)
          @flags += CONSTRAIN_MODIFIER_MASK
        end
      elsif key == COPY_MODIFIER_KEY # Menu on Mac, Ctrl on PC
        if not ((@flags & COPY_MODIFIER_MASK) > 0)
          @flags += COPY_MODIFIER_MASK
        end
      elsif key == ALT_MODIFIER_KEY # Command on Mac, Alt on PC
        if not ((@flags & ALT_MODIFIER_MASK) > 0)
          @flags += ALT_MODIFIER_MASK
        end
      end

      view.tooltip = get_tooltip(@ip.face, @flags, view)
    end


    def onKeyUp(key, repeat, flags, view)

      # seem to get bad value for flags here
      if key == CONSTRAIN_MODIFIER_KEY # Shift Key
        if ((@flags & CONSTRAIN_MODIFIER_MASK) > 0)
          @flags -= CONSTRAIN_MODIFIER_MASK
        end
      elsif key == COPY_MODIFIER_KEY # Menu on Mac, Ctrl on PC
        if ((@flags & COPY_MODIFIER_MASK) > 0)
          @flags -= COPY_MODIFIER_MASK
        end
      elsif key == ALT_MODIFIER_KEY # Command on Mac, Alt on PC
        if ((@flags & ALT_MODIFIER_MASK) > 0)
          @flags -= ALT_MODIFIER_MASK
        end
      end

      view.tooltip = get_tooltip(@ip.face, @flags, view)
    end


    def get_tooltip(face, flags, view)

      tooltip = ""

      if (face)

        drawing_interface = face.drawing_interface

        if not drawing_interface
          parent = face.parent
          while parent and parent.is_a?(Sketchup::ComponentDefinition) or parent.is_a?(Sketchup::ComponentInstance) or parent.is_a?(Sketchup::Group)
            if drawing_interface = parent.drawing_interface
              break
            else
              if parent.is_a?(Sketchup::ComponentDefinition)
                if parent.instances.empty?
                  parent = nil
                else
                  parent = parent.instances[0]
                end
              else
                parent = parent.parent
              end
            end
          end

        end

        if (drawing_interface)

          # Determine if the camera is looking at the inside or outside of the face
          vector = @ip.position - view.camera.eye
          inside_info = false
          if (vector % face.normal < 0.0)  # Outside
            inside_info = false
          else
            inside_info = true
          end

          tooltip = drawing_interface.tooltip(flags, inside_info)

        end
      end

      return(tooltip)
    end


    def onLButtonDoubleClick(flags, x, y, view)
      super

      if (@ip.face)
        #$f = @ip.face
        #puts $f
        #puts $f.model_object_key

        #puts "relative coordinates"
        #$f.vertices.each { |v| puts v.position }
        #puts

        #puts "insertion point"
        #puts $f.parent.insertion_point

        #t = $f.parent.instances.first.transformation
        #puts "world coordinates"
        #$f.vertices.each { |v| puts (v.position).transform(t) }
        #puts

        #puts "DrawingInterface="
        #puts $f.drawing_interface
      end

      if (@ip.edge)
        #$e = @ip.edge
      end

      #$ip = @ip.position

      #puts
      #puts "Face=>       " + $f.to_s
      #puts "Interface=>  " + $f.drawing_interface.to_s
      #puts "EntityID=>   " + $f.entityID.to_s   # useless...always matched to the same Face
      #puts "Key=>        " + $f.model_object_handle.to_s
      #puts "Base Face=>  " + DrawingUtils.find_base_face($f).to_s   # this is not working right
      #puts

      #$g = $f.parent.instances.first

      #puts "Group=>      " + $g.to_s
      #puts "Grp Intrfc=> " + $g.drawing_interface.to_s
      #puts "Entities=>   " + $g.entities.to_s
      #puts "Entities[]=> " + $g.entities.to_a.to_s


      #puts $f.entityID

      #$f.drawing_interface.surface_polygon.points.each { |v| puts v.display }

      #puts $f
      #puts $f.drawing_interface
      #$f.vertices.each { |v| puts v.position.display }
      #puts

      #if ($f.contains_point?(@ip.position, include_border = true))
      #  puts "face contains point"
      #else
      #  puts "face DOES NOT contain point"
      #end

      #puts $f.classify_point($ip)
      # 1 = inside of all edges
      # 2 = on an edge
      # 4 = on a vertex
      # 8 = off the face completely, but still in the same plane
      # 16 = off the face completely, and not even on the same plane

      #PointUnknown = 0;
      #PointInside = 1;
      #PointOnEdge = 2;
      #PointOnVertex = 4
      #PointOutside = 8;
      #PointNotOnPlane = 16;

    end

  end

end
