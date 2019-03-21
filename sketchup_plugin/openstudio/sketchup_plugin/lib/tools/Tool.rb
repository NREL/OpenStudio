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

module OpenStudio

  class Tool

    def initialize
      #@cursor = UI.create_cursor(Plugin.dir + "/lib/resources/icons/Cursor.tiff", 1, 1)
    end


    def onSetCursor
      UI.set_cursor(@cursor)
    end


    # The activate method is called by SketchUp when the tool is first selected.
    # It is a good place to put most of your initialization.
    def activate
      # The Sketchup::InputPoint class is used to get 3D points from screen
      # positions.  It uses the SketchUp inferencing code.
      @ip = Sketchup::InputPoint.new
      Sketchup.active_model.selection.clear
    end


    # The draw method is called whenever the view is refreshed.  It lets the
    # tool draw any temporary geometry that it needs to.
    def draw(view)
      @ip.draw(view)
    end


    def onMouseMove(flags, x, y, view)
      @ip.pick(view, x, y)
      view.invalidate #if (@ip.display?)
    end


    def onLButtonUp(flags, x, y, view)
      @ip.pick(view, x, y)
    end


    def onLButtonDoubleClick(flags, x, y, view)
      @ip.pick(view, x, y)
    end


    # onCancel is called when the user hits the escape key.
    def onCancel(flag, view)
      Sketchup.active_model.tools.pop_tool
    end


    # See Tool documentation...can override context menu completely
    #def getMenu
    #end

    # onUserText is called when the user enters something into the VCB
    # In this implementation, we create a line of the entered length if
    # the user types a length while selecting the second point
    #def onUserText(text, view)
    #end

  end


end
