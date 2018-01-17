########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

require("openstudio/sketchup_plugin/sketchup/Geom")


# This file adds new methods to native SketchUp classes.

class Array

  def is_subset_of?(other)

    for element in self

      element_matched = false

      for other_element in other
        if (other_element == element)
          element_matched = true
          break
        end
      end

      if (not element_matched)
        # no match
        return(false)
      end
    end

    return(true)
  end


  def is_same_set?(other)
    if (self.length == other.length and self.is_subset_of?(other))
      return(true)
    else
      return(false)
    end
  end

end


class Float

  def round_to(decimal_places = 0)
    if (decimal_places > 0)
      precision = (10**(decimal_places)).to_f
      return((self * precision).round / precision)
    else
      return(self.round)
    end
  end

end


class Sketchup::Model

  # This attribute is a persistent string reference to an input file.
  # It allows a SketchUp file to be reassociated with the correct input file.
  def openstudio_path
    return(get_attribute('OpenStudio', 'OpenStudioPath'))
  end

  def openstudio_path=(path)
    OpenStudio::Plugin.log(OpenStudio::Trace, "#{current_method_name}")

    set_attribute('OpenStudio', 'OpenStudioPath', path)
  end

  # returns the OpenStudio::ModelInterface associated with this Model
  def model_interface
    object = nil
    if (id_string = get_attribute('OpenStudio', 'ModelInterface'))
      begin
        object = ObjectSpace._id2ref(id_string.to_i)
      rescue
        # The id_string does not reference an existing object!  Ignore the exception.
      ensure
        # Sometimes a bad reference can turn into a real object...but a random one, not the one we want.
        if (object and not object.is_a?(OpenStudio::ModelInterface))
          puts "Model.model_interface:  bad object reference"
          object = nil
        end
      end
    end
    return(object)
  end

  def model_interface=(object)
    OpenStudio::Plugin.log(OpenStudio::Trace, "#{current_method_name}")

    set_attribute('OpenStudio', 'ModelInterface', object.object_id.to_s)
  end

  def openstudio_entities
    result = []
    entities.each {|e| result << e if e.model_object_handle }
    return result
  end

  def delete_openstudio_entities
    # DLM: for some reason there is no delete_attribute for SketchUp::Model
    # delete_attribute('OpenStudio') # deletes entire attribute dictionary
    set_attribute('OpenStudio', 'OpenStudioPath', nil)
    set_attribute('OpenStudio', 'ModelInterface', nil)
    entities.erase_entities(openstudio_entities)
  end

end

class Sketchup::Entity

  # returns a string
  def model_object_handle
    return(get_attribute('OpenStudio', 'Handle'))
  end

  # takes a OpenStudio::Handle or a string
  def model_object_handle=(handle)
    OpenStudio::Plugin.log(OpenStudio::Trace, "#{current_method_name}")

    set_attribute('OpenStudio', 'Handle', handle.to_s)
  end

  # returns the OpenStudio::DrawingInterface associated with this Entity
  def drawing_interface
    object = nil
    if (id_string = get_attribute('OpenStudio', 'DrawingInterface'))
      begin
        object = ObjectSpace._id2ref(id_string.to_i)
      rescue
        # The id_string does not reference an existing object!  Ignore the exception.
      ensure
        # Sometimes a bad reference can turn into a real object...but a random one, not the one we want.
        if (object and not object.is_a?(OpenStudio::DrawingInterface))
          puts "Entity.drawing_interface:  bad object reference"
          object = nil
          # To detect copy-paste between SketchUp sessions, could set 'object' to a value that can be detected on the
          # receiving end by whichever Observer the entity is pasted into.
        end
      end
    end
    return(object)
  end

  def drawing_interface=(object)
    OpenStudio::Plugin.log(OpenStudio::Trace, "#{current_method_name}")

    set_attribute('OpenStudio', 'DrawingInterface', object.object_id.to_s)
  end

end


class Sketchup::Color
  # There's still some work to be done here:
  #  Add hue=, saturation=, brightness=
  #  Need reference for the hsba calcs.
  #  There is not a perfect symmetry when converting back and forth between what you put into hsba and what you get back.

  def rgba
    return [red, green, blue, alpha]
  end


  def rgba=(color_array)
    # For some reason, the 'self' is required here or else it doesn't work.
    self.red = color_array[0]
    self.green = color_array[1]
    self.blue = color_array[2]
    self.alpha = color_array[3]
    return(self)
  end


  def hsba
    # HSB = Hue, Saturation, Brightness; identical to HSV = hue, brightness, value
    var_R = red / 255.to_f  # RGB values = 0 � 255
    var_G = green / 255.to_f
    var_B = blue / 255.to_f

    var_Min = [var_R, var_G, var_B].min  # value of RGB
    var_Max = [var_R, var_G, var_B].max  # value of RGB
    del_Max = var_Max - var_Min          # Delta RGB value

    v = var_Max

    if (del_Max == 0)
      # This is a gray, no chroma...
      h = 0  # HSV results = 0 � 1
      s = 0
    else
      # Chromatic data...
      s = del_Max / var_Max

      del_R = ( ( ( var_Max - var_R ) / 6 ) + ( del_Max / 2 ) ) / del_Max
      del_G = ( ( ( var_Max - var_G ) / 6 ) + ( del_Max / 2 ) ) / del_Max
      del_B = ( ( ( var_Max - var_B ) / 6 ) + ( del_Max / 2 ) ) / del_Max

      if (var_R == var_Max)
        h = del_B - del_G
      elsif (var_G == var_Max)
        h = (1 / 3) + del_R - del_B
      elsif (var_B == var_Max)
        h = (2 / 3) + del_G - del_R
      end

      h += 1 if (h < 0)
      h -= 1 if (h > 1)
    end

    return([(h * 360).to_i, (s * 100).to_i, (v * 100).to_i, alpha])
  end


  def hsba=(color_array)
    h = color_array[0] / 360.to_f  # HSV values = 0 � 1
    s = color_array[1] / 100.to_f
    v = color_array[2] / 100.to_f
    a = color_array[3]

    if (s == 0)
      self.red = v * 255
      self.green = v * 255
      self.blue = v * 255
      self.alpha = a
    else
      var_h = h * 6
      var_h = 0 if (var_h == 6)  # H must be < 1
      var_i = var_h.floor
      var_1 = v * (1 - s)
      var_2 = v * (1 - s * (var_h - var_i))
      var_3 = v * (1 - s * (1 - (var_h - var_i)))

      if (var_i == 0)
       var_r = v
       var_g = var_3
       var_b = var_1
      elsif (var_i == 1)
       var_r = var_2
       var_g = v
       var_b = var_1
      elsif (var_i == 2)
        var_r = var_1
        var_g = v
        var_b = var_3
      elsif (var_i == 3)
        var_r = var_1
        var_g = var_2
        var_b = v
      elsif (var_i == 4)
        var_r = var_3
        var_g = var_1
        var_b = v
      else
        var_r = v
        var_g = var_1
        var_b = var_2
      end

      self.red = (var_r * 255).to_i  # RGB results = 0 � 255
      self.green = (var_g * 255).to_i
      self.blue = (var_b * 255).to_i
      self.alpha = a
    end
    return(self)
  end


  def hue
    return hsba[0]
  end


  def saturation
    return hsba[1]
  end


  def brightness
    return hsba[2]
  end


  #def hue=(h)
  #end

end


class Sketchup::Loop

# should this return a polygon or a polygon loop?

  def polygon_loop
    points = []
    self.vertices.each do |vertex|
      # DLM@20100920: weird bug in SU 8 that vertices can also return attribute dictionary for a loop's vertices
      if vertex.class == Sketchup::Vertex
        points << vertex.position
      end
    end
    return(Geom::PolygonLoop.new(points))
  end

end




class Sketchup::Face

  def outer_polygon
    return(Geom::Polygon.new(self.outer_loop.polygon_loop))
  end


  def full_polygon
    this_polygon = self.outer_polygon
    for this_loop in self.loops
      if (not this_loop.outer?)
        this_polygon.add_loop(this_loop.polygon_loop.points)
      end
    end
    return(this_polygon)
  end


  def contains_point?(point, include_border = false)
    return(Geom.point_in_polygon(point, self.full_polygon, include_border))
  end


  def intersect(other_face)
    return(Geom.intersect_polygon_polygon(self.full_polygon, other_face.full_polygon))  # array of polygons
  end

end



class Sketchup::ShadowInfo

  # Still need to reconcile daylight saving time between EnergyPlus and SketchUp


  def time
    # API bug:  ShadowTime is returning the hour incorrectly in UTC/GMT time, but the time zone is (correctly) the local one.
    #           Also year is ALWAYS 2002.
    # Example:  Noon on Nov 8 returns Fri Nov 08 04:50:11 Mountain Standard Time 2002
    # SUBTRACT the utc offset to get the correct local time.
    return(convert_to_utc(self['ShadowTime']))
  end


  def time=(new_time)
    # API bug:  ShadowTime is returning the hour incorrectly in UTC/GMT time, but the time zone is (correctly) the local one.
    #           Also year is ALWAYS 2002.
    # Example:  Noon on Nov 8 returns Fri Nov 08 04:50:11 Mountain Standard Time 2002
    # ADD the utc offset to set the correct local time.
    self['ShadowTime'] = new_time + new_time.utc_offset
    # if ShadowTime is already in UTC, this won't do anything...offset = 0
    return(time)
  end


  def dst?
    return(time.dst?)
  end


  def sunrise
    return(convert_to_utc(self['SunRise']))
  end


  def sunset
    return(convert_to_utc(self['SunSet']))
  end


  def zone_offset=(new_zone_offset)
    # Sets the time zone in hours offset from UTC/GMT.  NOTE:  Negative numbers indicate Western hemisphere.
    # API bug:  Setting ShadowTime['TZOffset'] alone does not set this.
    self['TZOffset'] = new_zone_offset
    # No way to change the time zone for the Time object in Ruby...?
    # Might consider putting all time as UTC, handle daylight savings myself.
    return(nil)
  end

  def north_angle
    return(self['NorthAngle'])
  end

private
  def convert_to_utc(time)
    # API bug:  ShadowTime is returning the hour incorrectly in UTC/GMT time, but the time zone is (correctly) the local one.
    #           Also year is ALWAYS 2002.
    # Example:  Noon on Nov 8 returns Fri Nov 08 04:50:11 Mountain Standard Time 2002
    # SUBTRACT the utc offset to get the correct local time.
    a = (time - time.utc_offset).to_a
    return( Time.utc(a[0], a[1], a[2], a[3], a[4], Time.now.year, a[6], a[7], a[8], a[9]) )
  end

end
