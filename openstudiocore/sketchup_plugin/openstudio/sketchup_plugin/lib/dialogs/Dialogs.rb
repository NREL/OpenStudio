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

module OpenStudio

# need support for singleton dialogs too


  # Abstract class for all dialogs.
  # Dialog and its subclasses are GUI specific and make direct calls to the GUI engine (WebDialog in this case).
  class Dialog

    attr_reader :container

    def initialize(container, interface, hash)
      @container = container  # In subclasses @container must be created before calling add_callbacks
      @interface = interface  # The dialog reports back to this object using:  @interface.report
      @hash = hash
    end


    def add_callbacks
      @container.web_dialog.add_action_callback("on_load") { on_load }
      @container.web_dialog.add_action_callback("on_change_element") { |d, p| on_change_element(d, p) }
      @container.web_dialog.add_action_callback("on_key") { |d, p| on_key(d, p) }
      @container.web_dialog.add_action_callback("on_cancel") { on_cancel }
      @container.web_dialog.set_on_close { cleanup }  # Close icon was clicked in the title bar; dialog is already closed at this point
    end


    def default_dialog_color
      # More efficient to set this one time as a Plugin attribute during startup
      if (Plugin.platform == Platform_Windows)
        return(@container.web_dialog.get_default_dialog_color)
      else
        return("#e8e8e8")
      end
    end


    def on_load
      @container.execute_function("setBackgroundColor('" + default_dialog_color + "')")
      @container.execute_function("document.body.style.borderWidth = '0px'")  # Fix for SU7
      @container.execute_function("document.body.style.overflow = 'auto'")  # Fix for SU7
      update
    end


    # not used anymore. Used to allow closing of some dialogs with esc key. Got in the way of using enter as mouse click
    def on_key(d, p)
      if (p == "27")  # Escape key
        on_cancel
      end
    end


    def on_change_element(d, p)
      args = p.split(",")

      key = args[0]
      value = args[1..-1].join(',')

      if (value.empty?)
        @hash[key] = ""
      else
        # Not sure if this is a good idea
        if (value == 'true')
          value = true
        elsif (value == 'false')
          value = false
        end
        @hash[key] = value
      end

    end


    def update
      @hash.keys.each { |key| set_element_value(key, @hash[key]) }
    end


    def show
      @container.show
    end


    def close
      @container.close
      cleanup
    end


    def cleanup
      @interface.delete
    end


 # private

    def report
      return(@interface.report)
    end


    def on_cancel
      close
    end


    def set_element_value(name, value)

      # This method works even if no 'name' attribute has been set; apparently tagName is defaulted to the 'id' attribute.
      # NOTE:  This will not work correctly for a 'select-multiple' element which requires options to be set individually.
      if (name.nil?)
        puts "Dialog:  nil name passed to set_element_value"
      else
        value = value.to_s.gsub(/'/, "\\\\'")  # Must encode any single quotes so that a JavaScript error is avoided

        if (Plugin.platform == Platform_Windows)
          value.gsub!(/\n/, "\\n")  # Replace \n with \\n for JavaScript
        else
          value.gsub!(/\r\n/, "\\n")  # Replace \n with \\n for JavaScript

          value.gsub!(/\n/, "\\n")  # Replace \n with \\n for JavaScript

          # "Unfix" a workaround for a bug in SU6 that added an extra space character at the beginning of the string.
          # SU7 corrects that bug, but the Javascript workaround in Dialogs.js still clips the first character.
          value = ' ' + value
        end

        @container.execute_function("setElementValue('" + name + "', '" + value + "')")
      end
    end


    def set_select_options(name, values, text = nil)
      if (name.nil?)
        puts "Dialog:  nil name passed to set_select_options"
      else
        if (values.nil?)
          values = []
        end
        if (text.nil?)
          text = values
        end

        values_arg = ""
        text_arg = ""

        if (not values.empty?)
          for i in 0...values.length
            if (values[i].nil?)
              values[i] = ""
            else
              values[i] = values[i].gsub(/'/, "%apos%")  # Encode any single quotes so that a JavaScript error is avoided
              # NOTE:  In other places this was done successfully using "\\\\'", but that does not work here...
            end
            if (text[i].nil?)
              text[i] = ""
            else
              text[i] = text[i].gsub(/'/, "%apos%")  # Encode any single quotes so that a JavaScript error is avoided
            end
            values_arg += values[i] + ","
            text_arg += text[i] + ","
          end
          values_arg = values_arg[0...values_arg.length - 1]
          text_arg = text_arg[0...text_arg.length - 1]
        end

        #if (Plugin.platform != Platform_Windows)
        #  values_arg = values_arg.gsub(/,/, "%comma%")
        #  text_arg = text_arg.gsub(/,/, "%comma%")
        #end

        @container.execute_function("setSelectOptions('" + name + "', '" + values_arg + "', '" + text_arg + "')")
      end
    end


    def set_element_source(name, path)
      if (name.nil?)
        puts "Dialog:  nil name passed to set_element_source"
      else
        if (path.nil?)
          path = ""
        end
        @container.execute_function("setElementSource('" + name + "', '" + path + "')")
      end
    end


    def get_element_value_by_id(id)
      # Does not work for checkboxes
      if (id.nil?)
        puts "Dialog: nil id passed to get_element_value_by_id"
        value = nil
      else
        value = @container.web_dialog.get_element_value(id)
      end
      return(value)
    end


    def disable_element(name)
      if (name.nil?)
        puts "Dialog:  nil name passed to disable_element"
      else
        @container.execute_function("disableElement('" + name + "')")
      end
    end


    def enable_element(name)
      if (name.nil?)
        puts "Dialog:  nil name passed to enable_element"
      else
        @container.execute_function("enableElement('" + name + "')")
      end
    end


    def hide_element(name)
      if (name.nil?)
        puts "Dialog:  nil name passed to hide_element"
      else
        @container.execute_function("hideElement('" + name + "')")
      end
    end


    def show_element(name)
      if (name.nil?)
        puts "Dialog:  nil name passed to show_element"
      else
        @container.execute_function("showElement('" + name + "')")
      end
    end


    def mark_element(name)
      if (name.nil?)
        puts "Dialog:  nil name passed to mark_element"
      else
        @container.execute_function("markElement('" + name + "')")
      end
    end


    def unmark_element(name)
      if (name.nil?)
        puts "Dialog:  nil name passed to unmark_element"
      else
        @container.execute_function("unmarkElement('" + name + "')")
      end
    end

  end


  class MessageDialog < Dialog

    def add_callbacks
      super
      @container.web_dialog.add_action_callback("on_ok") { on_ok }
    end


    def on_key(d, p)
      super
      if (p == "13") # Enter key
        on_ok
      end
    end


    def on_ok
      if (report)
        close
      end
    end

  end


  class PropertiesDialog < MessageDialog  # Abstract class for all properties dialogs

    def add_callbacks
      super
      @container.web_dialog.add_action_callback("on_apply") { on_apply }
    end


    #def on_load
      # could override to disable apply until the input is modified
      # on_change_element could check that new value is different from old
    #end

    def on_key(d, p)
      super
      if (p == "13") # Enter key
        on_ok
        # not quite right, should activate whichever button has focus:  apply, ok, or cancel
      end
    end


    def on_apply
      report
    end

  end


  class WizardDialog < Dialog  # Abstract class for all wizard dialogs

    def initialize(container, interface, hash)
      super
      @pages = [ Page ]
      @page_index = 0
      @current_page = nil
    end


    def on_load
      super
      #set_page(@pages[@page_index])
    end


    def add_callbacks
      super
      @container.web_dialog.add_action_callback("on_next") { on_next }
      @container.web_dialog.add_action_callback("on_back") { on_back }
    end


    def on_key(d, p)
      super
      if (p == "13" or p == "39") # Enter key
        on_next
        # not quite right, should activate whichever button has focus
      elsif (p == "37")
        on_back
      end
    end


    def on_next
      #if (report)  validate user input
      @interface.report

      if (last_page?)
        @interface.finish
      else
        @page_index = @page_index + 1
        set_page(@pages[@page_index])
      end
    end


    def on_back
      # validate user input?
      if (not first_page?)
        @interface.report
        @page_index = @page_index - 1
        set_page(@pages[@page_index])
      end
    end


    def first_page?
      return(@page_index == 0)
    end


    def last_page?
      return(@page_index == @pages.length - 1)
    end


    def set_page(page_dialog_class)
      @current_page = page_dialog_class.new(@frame, @interface, @hash)
      if (last_page?)
        set_element_value("next", "  Finish  ")
      else
        set_element_value("next", "  Next > ")
      end
      if (first_page?)
        disable_element("back")
      else
        enable_element("back")
      end
    end

  end


  # Page objects must always be inserted into a FrameContainer object.
  class Page < Dialog

    def initialize(container, interface, hash)
      super
      @container = container
      add_callbacks
    end


    def add_callbacks
      # Be careful with callbacks from FrameContainers...could accidentally overwrite the callback for the WindowContainer!
      @container.web_dialog.add_action_callback("on_load_page") { on_load_page }
      @container.web_dialog.add_action_callback("on_change_element") { |d, p| on_change_element(d, p) }  # This one does need to overwrite
    end


    def on_load_page
      update
    end

  end


# might be able to replace all WizardPage with just Page...
  class WizardPage < Page

    attr_accessor :prev_page, :next_page

    def initialize(container, interface, hash)
      super
      @prev_page = nil
      @next_page = nil
    end

  end

end
