######################################################################
#  Copyright (c) 2008-2012, Alliance for Sustainable Energy.  
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

require 'openstudio'

require 'minitest/autorun'

class RubyUserScriptArgumentGetter_Test < MiniTest::Unit::TestCase

  def test_BCLMeasureArguments

    argCount = 0
    patMeasures = OpenStudio::BCLMeasure::patApplicationMeasures
    assert(patMeasures.size > 0)
    patMeasures.each { |measure|
      args = OpenStudio::Ruleset::getArguments(measure)
      argCount += args.size
    }
    assert(argCount > 0)

  end

end

