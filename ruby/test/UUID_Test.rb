########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class UUID_Test < Minitest::Test

  # def setup
  # end

  # def teardown
  # end

  def test_uuid_constuctors

    # created UUID is not null
    uuid = OpenStudio::UUID::create()
    assert(uuid)

    # copy UUID
    uuid2 = uuid
    assert(uuid2)
    assert_equal(uuid, uuid2)

    # to string
    s = uuid.to_s
    assert(s)
  end

  def test_uuid_big_set
    n = 1000
    nGroup = 3

    (0...nGroup).each do |group|
      assert(system("'#{OpenStudio::getOpenStudioCLI}' '#{File.join(File.dirname(__FILE__), 'UUID_BigSet_Aux.rb')}' '#{n}'"))
    end

    File.delete("UUIDSet.txt")
  end
  
  def test_uuid_hash
    model = OpenStudio::Model::Model.new
    space1 = OpenStudio::Model::Space.new(model)
    space1.setName("Space 1")
    space2 = OpenStudio::Model::Space.new(model)
    space2.setName("Space 2")
    
    space1_handle1 = space1.handle
    space1_handle2 = space1.handle
    space2_handle1 = space2.handle
    space2_handle2 = space2.handle
    
    assert(space1_handle1 == space1_handle1)
    assert(space1_handle1 == space1_handle2)
    assert(space1_handle1 != space2_handle1)
    assert(space1_handle1 != space2_handle2)
    
    assert(space1_handle1.to_s == space1_handle1.to_s)
    assert(space1_handle1.to_s == space1_handle2.to_s)
    assert(space1_handle1.to_s != space2_handle1.to_s)
    assert(space1_handle1.to_s != space2_handle2.to_s)
    
    assert(space1_handle1.hash == space1_handle1.hash)
    assert(space1_handle1.hash == space1_handle2.hash)
    assert(space1_handle1.hash != space2_handle1.hash)
    assert(space1_handle1.hash != space2_handle2.hash)
    
    assert(space1_handle1.eql?(space1_handle1))
    assert(space1_handle1.eql?(space1_handle2))
    assert(!space1_handle1.eql?(space2_handle1))
    assert(!space1_handle1.eql?(space2_handle2))
    
    assert(space1_handle1.to_s != space1_handle1)
    assert(space1_handle1.to_s != space1_handle2)
    assert(space1_handle1.to_s != space2_handle1)
    assert(space1_handle1.to_s != space2_handle2)
    
    handle_to_space_map = {}
    handle_to_space_map[space1_handle1] = space1
    handle_to_space_map[space2_handle1] = space2
    assert(handle_to_space_map[space1_handle1] == space1)
    assert(handle_to_space_map[space1_handle2] == space1) 
    assert(handle_to_space_map[space2_handle1] == space2)
    assert(handle_to_space_map[space2_handle2] == space2) 
    assert(handle_to_space_map[space1_handle1.to_s] != space1)
    assert(handle_to_space_map[space1_handle2.to_s] != space1)
    assert(handle_to_space_map[space2_handle1.to_s] != space2)
    assert(handle_to_space_map[space2_handle2.to_s] != space2)
    
    handle_str_to_space_map = {}
    handle_str_to_space_map[space1_handle1.to_s] = space1
    handle_str_to_space_map[space2_handle1.to_s] = space2
    assert(handle_str_to_space_map[space1_handle1.to_s] == space1)
    assert(handle_str_to_space_map[space1_handle2.to_s] == space1)
    assert(handle_str_to_space_map[space2_handle1.to_s] == space2)
    assert(handle_str_to_space_map[space2_handle2.to_s] == space2)
    assert(handle_str_to_space_map[space1_handle1] != space1)
    assert(handle_str_to_space_map[space1_handle2] != space1)
    assert(handle_str_to_space_map[space2_handle1] != space2)
    assert(handle_str_to_space_map[space2_handle2] != space2)
  end

end


