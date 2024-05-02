########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class ConstantsTest < Minitest::Test

  def test_ruby_version
    assert_equal("3.2.2", RUBY_VERSION)
  end

  def test_ruby_description
    refute_empty(RUBY_DESCRIPTION)
    assert_match(/ruby 3.2.2 \(.*\) \[.*\]/, RUBY_DESCRIPTION)
  end

  def test_ruby_engine
    assert_equal("ruby", RUBY_ENGINE)
  end

  def test_ruby_platform
    refute_empty(RUBY_PLATFORM)
  end

  def test_ruby_release_date
    refute_empty(RUBY_RELEASE_DATE)
  end

  def test_ruby_copyright
    refute_empty(RUBY_COPYRIGHT)
  end

end
