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

module UI

  # This patch allows all file separators to be accepted and prints an error message if path does not exist.
  # Decided that the normal behavior of UI.openpanel should not be changed (even for the better).
  # New alternative method is:  UI.open_panel
  def UI.open_panel(*args)
    if (args[1])
      dir = args[1]

      if (not dir.empty?)

        if (RUBY_PLATFORM =~ /mswin/ or RUBY_PLATFORM =~ /mingw/)  # Windows
          # Replace / with \\ for the file separator
          dir = dir.split("/").join("\\")

          # Check for and append required final \\
          if (dir[dir.length - 1].chr != "\\")
            dir += "\\"
          end

        else  # Mac
          # Check for and append required final /
          if (dir[dir.length - 1].chr != "/")
            dir += "/"
          end
        end

        if (not File.directory?(dir))
          puts "UI.open_panel received bad directory: " + dir
          args[1] = ""
        else
          args[1] = dir
        end
      end
    end

    # Allow empty file name to be passed in as a valid argument
    if (args[2])
      if (args[2].strip.empty?)
        args[2] = "*.*"
      end
    else
      args[2] = "*.*"
    end

    #if (path = _openpanel(*args))
    if (path = UI.openpanel(*args))  # call the original method
      # Replace \\ with / for the file separator (works better for saving the path in a registry default)
      path = path.split("\\").join("/")
    end

    return(path)
  end


  # Decided that the normal behavior of UI.savepanel should not be changed (even for the better).
  # New alternative method is:  UI.save_panel
  def UI.save_panel(*args)
    if (args[1])
      dir = args[1]

      if (not dir.empty?)

        if (RUBY_PLATFORM =~ /mswin/ or RUBY_PLATFORM =~ /mingw/)  # Windows
          # Replace / with \\ for the file separator
          dir = dir.split("/").join("\\")

          # Check for and append required final \\
          if (dir[dir.length - 1].chr != "\\")
            dir += "\\"
          end

        else  # Mac
          # Check for and append required final /
          if (dir[dir.length - 1].chr != "/")
            dir += "/"
          end
        end

        if (not File.directory?(dir))
          puts "UI.save_panel received bad directory: " + dir
          args[1] = ""
        else
          args[1] = dir
        end
      end
    end

    # Allow empty file name to be passed in as a valid argument
    if (args[2])
      if (args[2].strip.empty?)
        args[2] = "*.*"
      end
    else
      args[2] = "*.*"
    end

    #if (path = _savepanel(*args))
    if (path = UI.savepanel(*args))  # call the original method
      # Replace \\ with / for the file separator (works better for saving the path in a registry default)
      path = path.split("\\").join("/")
    end

    return(path)
  end


end
