######################################################################
#  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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


module OpenStudio

  # This is a kludge to get a selection update when a Group is closed after being edited.
  # SelectionObserver does not provide any event.  Fortunately, InstanceObserver, which
  # also happens to work for Groups, DOES give an event that can be used.
  class InstanceObserver < Sketchup::InstanceObserver
  
    def initialize(drawing_interface)
      # for drawing interfaces that want update_entity on close
      @drawing_interface = drawing_interface
      @enabled = false
    end
    
    def disable
      was_enabled = @enabled
      @enabled = false
      return was_enabled
    end
    
    def enable
      @enabled = true
    end
    
    def destroy
      @drawing_interface = nil
      @enabled = false
    end    
    
    #def onOpen(group)
    #
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")
    #
    #  return if not @enabled
    #
    #  http://www.thomthom.net/software/sketchup/observers/#note_InstanceObserver
    #  Under OSX, when using this observer SketchUp will crash when the user quits SketchUp without saving the model first. Quiting and then choosing to save will also cause crash.
    #  SketchUp under Windows does not suffer from this.
    #
    #end

    #def onClose(group)
    #
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}, @enabled = #{@enabled}")
    #
    #  return if not @enabled
    #
    #  http://www.thomthom.net/software/sketchup/observers/#note_InstanceObserver
    #  Under OSX, when using this observer SketchUp will crash when the user quits SketchUp without saving the model first. Quiting and then choosing to save will also cause crash.
    #  SketchUp under Windows does not suffer from this. 
    #
    #end

  end


end
