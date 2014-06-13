######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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
