########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
require 'openstudio'
require 'logger'
require 'openstudio/measure/ShowRunnerOutput'
require 'fileutils'
require 'minitest/autorun'
require 'csv'
require_relative 'model.rb'
require 'logger'

class Issue_1322_Test < Minitest::Test
    def test_Issue_1322
        # path1 = OpenStudio::Path.new(File.dirname(__FILE__) + "/7-7 Windows Complete.osm")
        # path2 = OpenStudio::Path.new(File.dirname(__FILE__) + "/two_stories_pre_intersect")
        modelPath =File.join(File.expand_path('../../.') + "/build/resources/model/","7-7_Windows_Complete.osm") 

        # translator = OsVersion::VersionTranslator.new 
        # model = translator.loadModel(modelPath)
        model = ModelFile.load_model_NoTranslator(modelPath)
        assert(model);
      
        spaces = model.getSpaces
        spacesX = OpenStudio::Model::SpaceVector.new
        spaces.each do |sp|
            spacesX.push(sp)
        end
        OpenStudio::Model::intersectSurfaces(spacesX)
        OpenStudio::Model::matchSurfaces(spacesX)
      
        outpath = File.join(File.expand_path('../../.') + "/build/resources/model/","7-7 Windows Complete_after.osm") 
        model.save(outpath, true)
    end
end
