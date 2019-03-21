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
