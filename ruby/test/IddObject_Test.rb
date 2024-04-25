########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class IddObject_Test < Minitest::Test

  # def setup
  # end

  # def teardown
  # end

  def test_ExtensibleIndex
    factory = OpenStudio::IddFileAndFactoryWrapper.new("EnergyPlus".to_IddFileType)
    oio = factory.getObject("BuildingSurface:Detailed".to_IddObjectType)
    assert((not oio.empty?))
    iddObj = oio.get()

    extInd = OpenStudio::ExtensibleIndex.new(0,1)
    ind = iddObj.index(extInd)
    assert_equal(12,ind)

    extInd.group = 2
    extInd.field = 2
    ind = iddObj.index(extInd)
    assert_equal(19,ind)

    ind = 27
    extInd = iddObj.extensibleIndex(ind)
    assert_equal(5,extInd.group)
    assert_equal(1,extInd.field)

  end

end
