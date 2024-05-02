########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'minitest/autorun'

class ConstantsTest < Minitest::Test

  # Note: putting all of these tests under the same one because of the way we
  # run the tests with the CLI (we run a CLI instance for each individual test,
  # and it takes almost 20 seconds for each which is ridiculous)
  def test_ruby_constants

    assert_equal("3.2.2", RUBY_VERSION)

    refute_empty(RUBY_DESCRIPTION)
    assert_match(/ruby 3.2.2 \(.*\) \[.*\]/, RUBY_DESCRIPTION)

    assert_equal("ruby", RUBY_ENGINE)

    refute_empty(RUBY_PLATFORM)

    refute_empty(RUBY_RELEASE_DATE)

    refute_empty(RUBY_COPYRIGHT)

  end

end
