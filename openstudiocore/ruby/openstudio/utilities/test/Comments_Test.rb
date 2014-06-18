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

require 'openstudio'

require 'minitest/autorun'

class Comments_Test < MiniTest::Unit::TestCase

  # def setup
  # end

  # def teardown
  # end
  
  def test_makeComment
  
    str = "One line without endl"
    comment = OpenStudio::makeComment(str)
    assert_equal("! One line without endl",comment)
    
    str = "One line with endl\n"
    comment = OpenStudio::makeComment(str)
    assert_equal("! One line with endl\n",comment)
       
    str = "Comment block has two lines \n and no comment marks."
    comment = OpenStudio::makeComment(str)
    editorComment = OpenStudio::makeIdfEditorComment(str)
    assert_equal("! Comment block has two lines \n! and no comment marks.",comment);
    assert_equal("!- Comment block has two lines \n!- and no comment marks.",editorComment);
  
  end

end
