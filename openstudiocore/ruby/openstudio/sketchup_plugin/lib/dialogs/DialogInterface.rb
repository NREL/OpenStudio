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

  # Abstract supercalls for all dialog interfaces
  # Everything in an interface is supposed to be independent of the GUI implementation.
  # The only link to the GUI is the call to the 'new' method of the GUI Dialog class.
  class DialogInterface

    # DLM: do not expose hash as an accessor as it interferes with inserting DialogInterface into a Set
    attr_accessor :dialog #, :hash


    def initialize(container = nil)
      @hash = Hash.new
      populate_hash
      
      # This is where subclasses will create the Dialog object.
      # @dialog = Dialog.new(self, @hash)
    end

    
    def populate_hash
      # Update hash values with data from the EnergyPlus object.
      # This method can be called externally when the EnergyPlus object changes.
      # Note:  Hash keys, e.g., 'name', are case sensitive!
      # @hash['key1'] = value1
      # @hash['key2'] = value2
    end


    def update
      populate_hash
      #show
      @dialog.update
    end


    def show
      @dialog.show
      #update  # don't need this...causes double updates which is slow
    end


    def report
      # Report data from hash values back to the EnergyPlus object.
      # This method is called by the dialog when changes are finalized (usually on 'OK' or 'Apply')
      # Note:  Hash keys, e.g., 'name', are case sensitive!
      # value1 = @hash['key1']
      # value2 = @hash['key2']
      
      # Return value validates the user input and allows the dialog to close.
      # A false value can force the user to fix their input before closing is allowed.
      return(true)
    end


    def close
      @dialog.close
    end


    def delete
      Plugin.dialog_manager.remove(self)
    end


    def inspect
      return(self)
    end

  end

end
