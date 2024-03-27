/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef WORKFLOW_RUNPREPROCESSMONTHLY_REPORTS_HPP
#define WORKFLOW_RUNPREPROCESSMONTHLY_REPORTS_HPP

#include <array>
#include <string_view>

// NOTE: see OpenStudio/developer/ruby/GeneratePreProcessMonthlyReports.rb

static constexpr std::array<std::string_view, 8> c_monthlyReports = {
  R"idf(
Output:Table:Monthly,
  Building Energy Performance - Electricity, !- Name
  2,                                      !- Digits After Decimal
  InteriorLights:Electricity,             !- Variable or Meter Name 1
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 1
  ExteriorLights:Electricity,             !- Variable or Meter Name 2
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 2
  InteriorEquipment:Electricity,          !- Variable or Meter Name 3
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 3
  ExteriorEquipment:Electricity,          !- Variable or Meter Name 4
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 4
  Fans:Electricity,                       !- Variable or Meter Name 5
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 5
  Pumps:Electricity,                      !- Variable or Meter Name 6
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 6
  Heating:Electricity,                    !- Variable or Meter Name 7
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 7
  Cooling:Electricity,                    !- Variable or Meter Name 8
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 8
  HeatRejection:Electricity,              !- Variable or Meter Name 9
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 9
  Humidifier:Electricity,                 !- Variable or Meter Name 10
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 10
  HeatRecovery:Electricity,               !- Variable or Meter Name 11
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 11
  WaterSystems:Electricity,               !- Variable or Meter Name 12
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 12
  Cogeneration:Electricity,               !- Variable or Meter Name 13
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 13
  Refrigeration:Electricity,              !- Variable or Meter Name 14
  SumOrAverage;                           !- Aggregation Type for Variable or Meter 14
)idf",
  R"idf(
Output:Table:Monthly,
  Building Energy Performance - Natural Gas, !- Name
  2,                                      !- Digits After Decimal
  InteriorEquipment:NaturalGas,           !- Variable or Meter Name 1
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 1
  ExteriorEquipment:NaturalGas,           !- Variable or Meter Name 2
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 2
  Heating:NaturalGas,                     !- Variable or Meter Name 3
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 3
  Cooling:NaturalGas,                     !- Variable or Meter Name 4
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 4
  WaterSystems:NaturalGas,                !- Variable or Meter Name 5
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 5
  Cogeneration:NaturalGas,                !- Variable or Meter Name 6
  SumOrAverage;                           !- Aggregation Type for Variable or Meter 6
)idf",
  R"idf(
Output:Table:Monthly,
  Building Energy Performance - District Heating Water, !- Name
  2,                                      !- Digits After Decimal
  InteriorLights:DistrictHeatingWater,    !- Variable or Meter Name 1
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 1
  ExteriorLights:DistrictHeatingWater,    !- Variable or Meter Name 2
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 2
  InteriorEquipment:DistrictHeatingWater, !- Variable or Meter Name 3
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 3
  ExteriorEquipment:DistrictHeatingWater, !- Variable or Meter Name 4
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 4
  Fans:DistrictHeatingWater,              !- Variable or Meter Name 5
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 5
  Pumps:DistrictHeatingWater,             !- Variable or Meter Name 6
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 6
  Heating:DistrictHeatingWater,           !- Variable or Meter Name 7
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 7
  Cooling:DistrictHeatingWater,           !- Variable or Meter Name 8
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 8
  HeatRejection:DistrictHeatingWater,     !- Variable or Meter Name 9
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 9
  Humidifier:DistrictHeatingWater,        !- Variable or Meter Name 10
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 10
  HeatRecovery:DistrictHeatingWater,      !- Variable or Meter Name 11
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 11
  WaterSystems:DistrictHeatingWater,      !- Variable or Meter Name 12
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 12
  Cogeneration:DistrictHeatingWater,      !- Variable or Meter Name 13
  SumOrAverage;                           !- Aggregation Type for Variable or Meter 13
)idf",
  R"idf(
Output:Table:Monthly,
  Building Energy Performance - District Cooling, !- Name
  2,                                      !- Digits After Decimal
  InteriorLights:DistrictCooling,         !- Variable or Meter Name 1
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 1
  ExteriorLights:DistrictCooling,         !- Variable or Meter Name 2
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 2
  InteriorEquipment:DistrictCooling,      !- Variable or Meter Name 3
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 3
  ExteriorEquipment:DistrictCooling,      !- Variable or Meter Name 4
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 4
  Fans:DistrictCooling,                   !- Variable or Meter Name 5
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 5
  Pumps:DistrictCooling,                  !- Variable or Meter Name 6
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 6
  Heating:DistrictCooling,                !- Variable or Meter Name 7
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 7
  Cooling:DistrictCooling,                !- Variable or Meter Name 8
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 8
  HeatRejection:DistrictCooling,          !- Variable or Meter Name 9
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 9
  Humidifier:DistrictCooling,             !- Variable or Meter Name 10
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 10
  HeatRecovery:DistrictCooling,           !- Variable or Meter Name 11
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 11
  WaterSystems:DistrictCooling,           !- Variable or Meter Name 12
  SumOrAverage,                           !- Aggregation Type for Variable or Meter 12
  Cogeneration:DistrictCooling,           !- Variable or Meter Name 13
  SumOrAverage;                           !- Aggregation Type for Variable or Meter 13
)idf",
  R"idf(
Output:Table:Monthly,
  Building Energy Performance - Electricity Peak Demand, !- Name
  2,                                      !- Digits After Decimal
  Electricity:Facility,                   !- Variable or Meter Name 1
  Maximum,                                !- Aggregation Type for Variable or Meter 1
  InteriorLights:Electricity,             !- Variable or Meter Name 2
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 2
  ExteriorLights:Electricity,             !- Variable or Meter Name 3
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 3
  InteriorEquipment:Electricity,          !- Variable or Meter Name 4
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 4
  ExteriorEquipment:Electricity,          !- Variable or Meter Name 5
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 5
  Fans:Electricity,                       !- Variable or Meter Name 6
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 6
  Pumps:Electricity,                      !- Variable or Meter Name 7
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 7
  Heating:Electricity,                    !- Variable or Meter Name 8
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 8
  Cooling:Electricity,                    !- Variable or Meter Name 9
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 9
  HeatRejection:Electricity,              !- Variable or Meter Name 10
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 10
  Humidifier:Electricity,                 !- Variable or Meter Name 11
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 11
  HeatRecovery:Electricity,               !- Variable or Meter Name 12
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 12
  WaterSystems:Electricity,               !- Variable or Meter Name 13
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 13
  Cogeneration:Electricity,               !- Variable or Meter Name 14
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 14
  Refrigeration:Electricity,              !- Variable or Meter Name 15
  ValueWhenMaximumOrMinimum;              !- Aggregation Type for Variable or Meter 15
)idf",
  R"idf(
Output:Table:Monthly,
  Building Energy Performance - Natural Gas Peak Demand, !- Name
  2,                                      !- Digits After Decimal
  NaturalGas:Facility,                    !- Variable or Meter Name 1
  Maximum,                                !- Aggregation Type for Variable or Meter 1
  InteriorEquipment:NaturalGas,           !- Variable or Meter Name 2
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 2
  ExteriorEquipment:NaturalGas,           !- Variable or Meter Name 3
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 3
  Heating:NaturalGas,                     !- Variable or Meter Name 4
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 4
  Cooling:NaturalGas,                     !- Variable or Meter Name 5
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 5
  WaterSystems:NaturalGas,                !- Variable or Meter Name 6
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 6
  Cogeneration:NaturalGas,                !- Variable or Meter Name 7
  ValueWhenMaximumOrMinimum;              !- Aggregation Type for Variable or Meter 7
)idf",
  R"idf(
Output:Table:Monthly,
  Building Energy Performance - District Heating Water Peak Demand, !- Name
  2,                                      !- Digits After Decimal
  DistrictHeatingWater:Facility,          !- Variable or Meter Name 1
  Maximum,                                !- Aggregation Type for Variable or Meter 1
  InteriorLights:DistrictHeatingWater,    !- Variable or Meter Name 2
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 2
  ExteriorLights:DistrictHeatingWater,    !- Variable or Meter Name 3
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 3
  InteriorEquipment:DistrictHeatingWater, !- Variable or Meter Name 4
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 4
  ExteriorEquipment:DistrictHeatingWater, !- Variable or Meter Name 5
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 5
  Fans:DistrictHeatingWater,              !- Variable or Meter Name 6
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 6
  Pumps:DistrictHeatingWater,             !- Variable or Meter Name 7
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 7
  Heating:DistrictHeatingWater,           !- Variable or Meter Name 8
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 8
  Cooling:DistrictHeatingWater,           !- Variable or Meter Name 9
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 9
  HeatRejection:DistrictHeatingWater,     !- Variable or Meter Name 10
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 10
  Humidifier:DistrictHeatingWater,        !- Variable or Meter Name 11
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 11
  HeatRecovery:DistrictHeatingWater,      !- Variable or Meter Name 12
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 12
  WaterSystems:DistrictHeatingWater,      !- Variable or Meter Name 13
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 13
  Cogeneration:DistrictHeatingWater,      !- Variable or Meter Name 14
  ValueWhenMaximumOrMinimum;              !- Aggregation Type for Variable or Meter 14
)idf",
  R"idf(
Output:Table:Monthly,
  Building Energy Performance - District Cooling Peak Demand, !- Name
  2,                                      !- Digits After Decimal
  DistrictCooling:Facility,               !- Variable or Meter Name 1
  Maximum,                                !- Aggregation Type for Variable or Meter 1
  InteriorLights:DistrictCooling,         !- Variable or Meter Name 2
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 2
  ExteriorLights:DistrictCooling,         !- Variable or Meter Name 3
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 3
  InteriorEquipment:DistrictCooling,      !- Variable or Meter Name 4
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 4
  ExteriorEquipment:DistrictCooling,      !- Variable or Meter Name 5
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 5
  Fans:DistrictCooling,                   !- Variable or Meter Name 6
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 6
  Pumps:DistrictCooling,                  !- Variable or Meter Name 7
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 7
  Heating:DistrictCooling,                !- Variable or Meter Name 8
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 8
  Cooling:DistrictCooling,                !- Variable or Meter Name 9
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 9
  HeatRejection:DistrictCooling,          !- Variable or Meter Name 10
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 10
  Humidifier:DistrictCooling,             !- Variable or Meter Name 11
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 11
  HeatRecovery:DistrictCooling,           !- Variable or Meter Name 12
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 12
  WaterSystems:DistrictCooling,           !- Variable or Meter Name 13
  ValueWhenMaximumOrMinimum,              !- Aggregation Type for Variable or Meter 13
  Cogeneration:DistrictCooling,           !- Variable or Meter Name 14
  ValueWhenMaximumOrMinimum;              !- Aggregation Type for Variable or Meter 14
)idf"};

static constexpr std::array<std::string_view, 7> c_metersForced{
  // These are needed for the calibration report
  "Output:Meter:MeterFileOnly,NaturalGas:Facility,Daily;",
  "Output:Meter:MeterFileOnly,Electricity:Facility,Timestep;",
  "Output:Meter:MeterFileOnly,Electricity:Facility,Daily;",

  // Always add in the timestep facility meters
  "Output:Meter,Electricity:Facility,Timestep;",
  "Output:Meter,NaturalGas:Facility,Timestep;",
  "Output:Meter,DistrictHeatingWater:Facility,Timestep;",
  "Output:Meter,DistrictCooling:Facility,Timestep;",
};

#endif  // WORKFLOW_RUNPREPROCESSMONTHLY_REPORTS_HPP
