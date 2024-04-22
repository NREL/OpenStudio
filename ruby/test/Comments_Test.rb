########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class Comments_Test < Minitest::Test

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
