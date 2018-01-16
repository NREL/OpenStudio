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

  class ResultsInterface

    attr_accessor :output_file_path, :run_period, :variable_type, :rendering_appearance
    attr_accessor :match_range, :range_minimum, :range_maximum, :range_units, :normalize
    attr_accessor :outside_variable_name, :inside_variable_name, :out_of_range_value


    def initialize(model_interface)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      @model_interface = model_interface
      @output_file_path = ""
      @run_period = ""
      @variable_type = "SURFACE"
      @rendering_appearance = "COLOR"
      @out_of_range_value = Float::MIN
      @normalize = false
    end

    def destroy
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      @model_interface = nil
    end

    def update
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      # update variables on all surfaces
      @model_interface.update_surface_variables
    end

    # attach an output file
    def attach_output_file(output_file_path)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      openstudio_model = @model_interface.openstudio_model
      old_sql_file = openstudio_model.sqlFile

      if File.exists?(output_file_path)
        begin
          new_sql_file = OpenStudio::SqlFile.new(OpenStudio::Path.new(output_file_path))
          openstudio_model.setSqlFile(new_sql_file)
        rescue
          puts "Failed to load SqlFile from path '#{output_file_path}'"
        end
      end
    end

  end
end
