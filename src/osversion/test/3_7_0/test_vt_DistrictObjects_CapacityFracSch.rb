#require '/usr/local/openstudio-3.6.1/Ruby/openstudio'

include OpenStudio::Model

def test_create_schedule
  m = Model.new

  # New required-field Schedule at end
  dh = DistrictHeating.new(m)
  dh.setNominalCapacity(1000.0)

  dc = DistrictCooling.new(m)
  dc.setNominalCapacity(1000.0)

  raise if m.getSchedules.size != 0
  raise if m.getScheduleTypeLimitss.size != 0

  m.save('test_vt_DistrictObjects_CapacityFracSch_Create.osm', true)
end

def test_get_schedule
  m = Model.new

  m.alwaysOnContinuousSchedule

  # New required-field Schedule at end
  dh = DistrictHeating.new(m)
  dh.setNominalCapacity(1000.0)

  dc = DistrictCooling.new(m)
  dc.setNominalCapacity(1000.0)

  raise if m.getSchedules.size != 1
  raise if m.getScheduleTypeLimitss.size != 1

  m.save('test_vt_DistrictObjects_CapacityFracSch_Get.osm', true)
end

test_create_schedule()
test_get_schedule()
