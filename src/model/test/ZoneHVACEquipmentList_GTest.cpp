/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ZoneHVACEquipmentList.hpp"
#include "../ZoneHVACEquipmentList_Impl.hpp"

#include "../ThermalZone.hpp"
#include "../ZoneHVACBaseboardConvectiveElectric.hpp"
#include "../ScheduleConstant.hpp"
#include "../Schedule.hpp"
#include "../ScheduleTypeLimits.hpp"

#include "../../utilities/idd/IddObject.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/OS_ZoneHVAC_EquipmentList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <algorithm>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneHVACEquipmentList_Logic) {
  Model m;

  ThermalZone z(m);

  EXPECT_TRUE(z.setLoadDistributionScheme("UniformPLR"));
  EXPECT_EQ(z.loadDistributionScheme(), "UniformPLR");

  ZoneHVACBaseboardConvectiveElectric b1(m);
  EXPECT_TRUE(b1.addToThermalZone(z));
  EXPECT_TRUE(z.setHeatingPriority(b1, 0));
  ZoneHVACBaseboardConvectiveElectric b2(m);
  EXPECT_TRUE(b2.addToThermalZone(z));

  // One that isn't part of the ZoneHVACEquipmentList
  ZoneHVACBaseboardConvectiveElectric bwrong(m);

  EXPECT_EQ(2u, z.equipment().size());
  ASSERT_EQ(2u, z.equipmentInCoolingOrder().size());
  EXPECT_EQ(b1, z.equipmentInCoolingOrder()[0]);
  EXPECT_EQ(b2, z.equipmentInCoolingOrder()[1]);

  // TODO: expected to fail right now due to https://github.com/NREL/EnergyPlus/issues/7193
  // ASSERT_EQ(1u, z.equipmentInHeatingOrder().size());
  // EXPECT_EQ(b2, z.equipmentInHeatingOrder()[0]);

  // Shouldn't be able to get/set anything for an equipment that isn't part of the ZoneEqList
  EXPECT_FALSE(z.setHeatingPriority(bwrong, 3));
  EXPECT_FALSE(z.setCoolingPriority(bwrong, 3));

  // Can't set/get the Sequential Fraction if:

  // 1. Equipment isn't part of the ZoneEqList
  EXPECT_TRUE(z.setLoadDistributionScheme("SequentialLoad"));
  EXPECT_FALSE(z.setSequentialCoolingFraction(bwrong, 0.5));
  EXPECT_FALSE(z.setSequentialHeatingFraction(bwrong, 0.5));
  EXPECT_FALSE(z.sequentialCoolingFraction(bwrong));
  EXPECT_FALSE(z.sequentialHeatingFraction(bwrong));

  // 2. Load Distribution isn't 'SequentialLoad'
  EXPECT_TRUE(z.setLoadDistributionScheme("UniformPLR"));
  EXPECT_FALSE(z.setSequentialCoolingFraction(b1, 0.5));
  EXPECT_FALSE(z.setSequentialHeatingFraction(b1, 0.5));
  EXPECT_FALSE(z.sequentialCoolingFraction(b1));
  EXPECT_FALSE(z.sequentialHeatingFraction(b1));

  EXPECT_TRUE(z.setLoadDistributionScheme("SequentialLoad"));

  // 3. Has a priority of zero
  // TODO: expected to fail (that is, it'll work) right now due to https://github.com/NREL/EnergyPlus/issues/7193
  // EXPECT_FALSE(z.setSequentialHeatingFraction(b1, 0.5));
  // EXPECT_FALSE(z.sequentialHeatingFraction(b1));

  // Rest should work fine
  EXPECT_TRUE(z.setSequentialCoolingFraction(b1, 0.5));
  ASSERT_TRUE(z.sequentialCoolingFraction(b1));
  EXPECT_EQ(0.5, z.sequentialCoolingFraction(b1).get());

  EXPECT_TRUE(z.setSequentialHeatingFraction(b2, 0.65));
  ASSERT_TRUE(z.sequentialHeatingFraction(b2));
  EXPECT_EQ(0.65, z.sequentialHeatingFraction(b2).get());

  EXPECT_TRUE(z.setSequentialCoolingFraction(b2, 0.7));
  ASSERT_TRUE(z.sequentialCoolingFraction(b2));
  EXPECT_EQ(0.7, z.sequentialCoolingFraction(b2).get());

  ScheduleConstant scheduleCooling(m);
  EXPECT_TRUE(scheduleCooling.setValue(0.9));
  EXPECT_TRUE(z.setSequentialCoolingFractionSchedule(b2, scheduleCooling));
  ASSERT_TRUE(z.sequentialCoolingFraction(b2));
  EXPECT_EQ(0.9, z.sequentialCoolingFraction(b2).get());
  ASSERT_TRUE(z.sequentialCoolingFractionSchedule(b2));

  ScheduleConstant scheduleHeating(m);
  EXPECT_TRUE(scheduleHeating.setValue(0.33));
  EXPECT_TRUE(z.setSequentialHeatingFractionSchedule(b2, scheduleHeating));
  ASSERT_TRUE(z.sequentialHeatingFraction(b2));
  EXPECT_EQ(0.33, z.sequentialHeatingFraction(b2).get());
  ASSERT_TRUE(z.sequentialHeatingFractionSchedule(b2));

  // Setting a priority to zero should reset the corresponding Sequential Fraction
  // TODO
  // EXPECT_TRUE(z.setCoolingPriority(b2, 0));
  EXPECT_EQ(2u, z.equipment().size());
  // TODO
  // ASSERT_EQ(1u, z.equipmentInCoolingOrder().size());
  // EXPECT_EQ(b1, z.equipmentInCoolingOrder()[0]);

  // TODO
  // EXPECT_FALSE(z.sequentialCoolingFraction(b2)); // reset
  EXPECT_TRUE(z.sequentialHeatingFraction(b2));  // untouched

  // Setting the Load Distribution Scheme to something else should reset all sequential fractions
  EXPECT_TRUE(z.setLoadDistributionScheme("UniformLoad"));
  EXPECT_FALSE(z.sequentialCoolingFraction(b1));
  EXPECT_FALSE(z.sequentialCoolingFractionSchedule(b1));
  EXPECT_FALSE(z.sequentialCoolingFraction(b2));
  EXPECT_FALSE(z.sequentialCoolingFractionSchedule(b2));
  EXPECT_FALSE(z.sequentialHeatingFraction(b1));
  EXPECT_FALSE(z.sequentialHeatingFractionSchedule(b1));
  EXPECT_FALSE(z.sequentialHeatingFraction(b2));
  EXPECT_FALSE(z.sequentialHeatingFractionSchedule(b2));
}

TEST_F(ModelFixture, ZoneHVACEquipmentList_ScheduleTypeLimits) {
  Model m;

  ThermalZone z(m);

  EXPECT_TRUE(z.setLoadDistributionScheme("SequentialLoad"));

  ZoneHVACBaseboardConvectiveElectric b1(m);
  EXPECT_TRUE(b1.addToThermalZone(z));
  EXPECT_TRUE(z.setHeatingPriority(b1, 0));
  ZoneHVACBaseboardConvectiveElectric b2(m);
  EXPECT_TRUE(b2.addToThermalZone(z));

  EXPECT_TRUE(z.setSequentialCoolingFraction(b1, 0.5));

  // This should have created a schedule, and this schedule should automatically have been assigned a schedule type lim via the ScheduleTypeRegistry
  boost::optional<Schedule> _sch = z.sequentialCoolingFractionSchedule(b1);
  ASSERT_TRUE(_sch);
  boost::optional<ScheduleTypeLimits> _sch_lim = _sch->scheduleTypeLimits();
  ASSERT_TRUE(_sch_lim);
  EXPECT_EQ("Dimensionless", _sch_lim->unitType());
}

TEST_F(ModelFixture, ZoneHVACEquipmentList_RemoveEquipment_ModelObject_Is_First) {

  Model m;
  ThermalZone z(m);

  auto eqlists = m.getConcreteModelObjects<ZoneHVACEquipmentList>();
  EXPECT_EQ(1, eqlists.size());
  auto& eqlist = eqlists.front();

  ZoneHVACBaseboardConvectiveElectric bb_delete(m);

  ScheduleConstant bb_sch(m);

  ZoneHVACBaseboardConvectiveElectric bb(m);

  bb.setName("Baseboard");
  bb_sch.setName(bb.nameString());

  // Because underneath in Workspace m_workspaceObjectMap is a std::unordered_map we have no guarantee of order, so in case it's not good (it IS on
  // Unix, but not on Windows), we redo the insertion in a different order
  {
    auto objects = m.getObjectsByName(bb.nameString());
    EXPECT_EQ(2, objects.size());
    if (objects.front().iddObject().type() != IddObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric)) {
      bb_sch.remove();
      bb_sch = ScheduleConstant(m);
      bb_sch.setName("Baseboard");
    }
  }

  // Assert this time the order is good
  {
    auto objects = m.getObjectsByName(bb.nameString());
    EXPECT_EQ(2, objects.size());
    EXPECT_EQ(IddObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric), objects.front().iddObject().type());
  }

  EXPECT_TRUE(bb.setAvailabilitySchedule(bb_sch));
  EXPECT_EQ(bb.nameString(), bb_sch.nameString());

  EXPECT_TRUE(bb_delete.addToThermalZone(z));
  EXPECT_TRUE(bb.addToThermalZone(z));

  {
    EXPECT_EQ(2, eqlist.numExtensibleGroups());
    auto idf_egs = eqlist.extensibleGroups();
    for (const auto& idf_eg : idf_egs) {
      // Using getField so we don't resolve object name, but get the handle string
      const auto val_ = idf_eg.getField(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);
      const auto uid = openstudio::toUUID(*val_);
      auto obj_ = m.getObject(uid);
      ASSERT_TRUE(obj_);
      EXPECT_EQ(IddObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric), obj_->iddObject().type());
    }
  }

  EXPECT_NO_THROW(bb_delete.remove());

  {
    EXPECT_EQ(1, eqlist.numExtensibleGroups());
    auto idf_egs = eqlist.extensibleGroups();
    for (const auto& idf_eg : idf_egs) {
      // Using getField so we don't resolve object name, but get the handle string
      const auto val_ = idf_eg.getField(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);
      const auto uid = openstudio::toUUID(*val_);
      auto obj_ = m.getObject(uid);
      ASSERT_TRUE(obj_);
      EXPECT_EQ(IddObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric), obj_->iddObject().type());
    }
  }
}

TEST_F(ModelFixture, ZoneHVACEquipmentList_RemoveEquipment_Schedule_Is_First) {

  Model m;
  ThermalZone z(m);

  auto eqlists = m.getConcreteModelObjects<ZoneHVACEquipmentList>();
  EXPECT_EQ(1, eqlists.size());
  auto& eqlist = eqlists.front();

  ZoneHVACBaseboardConvectiveElectric bb_delete(m);

  ZoneHVACBaseboardConvectiveElectric bb(m);
  ScheduleConstant bb_sch(m);
  bb.setName("Baseboard");
  bb_sch.setName(bb.nameString());
  // Because underneath in Workspace m_workspaceObjectMap is a std::unordered_map we have no guarantee of order, so in case it's not good (it IS on
  // Unix, but not on Windows), we redo the insertion in a different order
  {
    auto objects = m.getObjectsByName(bb.nameString());
    EXPECT_EQ(2, objects.size());
    if (objects.front().iddObject().type() != IddObjectType(IddObjectType::OS_Schedule_Constant)) {
      bb.remove();
      bb = ZoneHVACBaseboardConvectiveElectric(m);
      bb.setName("Baseboard");
    }
  }

  // Assert this time the order is good
  {
    auto objects = m.getObjectsByName(bb.nameString());
    EXPECT_EQ(2, objects.size());
    EXPECT_EQ(IddObjectType(IddObjectType::OS_Schedule_Constant), objects.front().iddObject().type());
  }

  EXPECT_TRUE(bb.setAvailabilitySchedule(bb_sch));
  EXPECT_EQ(bb.nameString(), bb_sch.nameString());

  EXPECT_TRUE(bb_delete.addToThermalZone(z));
  EXPECT_TRUE(bb.addToThermalZone(z));

  {
    EXPECT_EQ(2, eqlist.numExtensibleGroups());
    auto idf_egs = eqlist.extensibleGroups();
    for (const auto& idf_eg : idf_egs) {
      // Using getField so we don't resolve object name, but get the handle string
      const auto val_ = idf_eg.getField(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);
      const auto uid = openstudio::toUUID(*val_);
      auto obj_ = m.getObject(uid);
      ASSERT_TRUE(obj_);
      EXPECT_EQ(IddObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric), obj_->iddObject().type());
    }
  }

  EXPECT_NO_THROW(bb_delete.remove());

  {
    EXPECT_EQ(1, eqlist.numExtensibleGroups());
    auto idf_egs = eqlist.extensibleGroups();
    for (const auto& idf_eg : idf_egs) {
      // Using getField so we don't resolve object name, but get the handle string
      const auto val_ = idf_eg.getField(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);
      const auto uid = openstudio::toUUID(*val_);
      auto obj_ = m.getObject(uid);
      ASSERT_TRUE(obj_);
      EXPECT_EQ(IddObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric), obj_->iddObject().type());
    }
  }
}

TEST_F(ModelFixture, ZoneHVACEquipmentList_RemoveEquipment_Schedule_Is_First_OnlyPop) {

  Model m;
  ThermalZone z(m);

  auto eqlists = m.getConcreteModelObjects<ZoneHVACEquipmentList>();
  EXPECT_EQ(1, eqlists.size());
  auto& eqlist = eqlists.front();

  ZoneHVACBaseboardConvectiveElectric bb_delete(m);

  ZoneHVACBaseboardConvectiveElectric bb(m);

  ScheduleConstant bb_sch(m);

  bb.setName("Baseboard");
  bb_sch.setName(bb.nameString());
  EXPECT_TRUE(bb.setAvailabilitySchedule(bb_sch));
  EXPECT_EQ(bb.nameString(), bb_sch.nameString());

  EXPECT_TRUE(bb_delete.addToThermalZone(z));
  EXPECT_TRUE(bb.addToThermalZone(z));

  EXPECT_EQ(2, eqlist.numExtensibleGroups());
  // I deliberately use idfObject.extensibleGroups so the handles aren't resolved to object name
  auto idf_egs = eqlist.idfObject().extensibleGroups();

  for (const auto& idf_eg : idf_egs) {
    const std::string val = idf_eg.getString(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment).get();
    const auto uid = openstudio::toUUID(val);
    auto obj_ = m.getObject(uid);
    ASSERT_TRUE(obj_);
    EXPECT_EQ(IddObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric), obj_->iddObject().type());
  }

  eqlist.eraseExtensibleGroup(0);

  EXPECT_EQ(1, eqlist.numExtensibleGroups());
  idf_egs = eqlist.idfObject().extensibleGroups();
  for (const auto& idf_eg : idf_egs) {
    const std::string val = idf_eg.getString(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment).get();
    const auto uid = openstudio::toUUID(val);
    auto obj_ = m.getObject(uid);
    ASSERT_TRUE(obj_);
    EXPECT_EQ(IddObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric), obj_->iddObject().type());
  }
}

TEST_F(ModelFixture, ZoneHVACEquipmentList_RemoveEquipment_Schedule_OnlyPop_Works_With_IdfObject) {

  Model m;
  ThermalZone z(m);

  auto eqlists = m.getConcreteModelObjects<ZoneHVACEquipmentList>();
  EXPECT_EQ(1, eqlists.size());
  auto& eqlist = eqlists.front();

  ZoneHVACBaseboardConvectiveElectric bb_delete(m);

  ZoneHVACBaseboardConvectiveElectric bb(m);

  ScheduleConstant bb_sch(m);

  bb.setName("Baseboard");
  bb_sch.setName(bb.nameString());
  EXPECT_TRUE(bb.setAvailabilitySchedule(bb_sch));
  EXPECT_EQ(bb.nameString(), bb_sch.nameString());

  EXPECT_TRUE(bb_delete.addToThermalZone(z));
  EXPECT_TRUE(bb.addToThermalZone(z));

  EXPECT_EQ(2, eqlist.numExtensibleGroups());
  // I deliberately use idfObject.extensibleGroups so the handles aren't resolved to object name
  auto idf_egs = eqlist.idfObject().extensibleGroups();

  for (const auto& idf_eg : idf_egs) {
    const std::string val = idf_eg.getString(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment).get();
    const auto uid = openstudio::toUUID(val);
    auto obj_ = m.getObject(uid);
    ASSERT_TRUE(obj_);
    EXPECT_EQ(IddObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric), obj_->iddObject().type());
  }

  auto i = eqlist.idfObject();
  i.eraseExtensibleGroup(0);

  EXPECT_EQ(1, i.numExtensibleGroups());
  idf_egs = i.extensibleGroups();
  for (const auto& idf_eg : idf_egs) {
    const std::string val = idf_eg.getString(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment).get();
    const auto uid = openstudio::toUUID(val);
    auto obj_ = m.getObject(uid);
    ASSERT_TRUE(obj_);
    EXPECT_EQ(IddObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric), obj_->iddObject().type());
  }
}
