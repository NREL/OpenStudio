#include <array>
#include <string_view>

static constexpr std::array<std::string_view, 8> c_monthlyReports = {
  R"idf(
  Output:Table:Monthly,
    Building Energy Performance - Electricity,  !- Name
      2,                       !- Digits After Decimal
      InteriorLights:Electricity,  !- Variable or Meter 1 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 1
      ExteriorLights:Electricity,  !- Variable or Meter 2 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 2
      InteriorEquipment:Electricity,  !- Variable or Meter 3 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 3
      ExteriorEquipment:Electricity,  !- Variable or Meter 4 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 4
      Fans:Electricity,        !- Variable or Meter 5 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 5
      Pumps:Electricity,       !- Variable or Meter 6 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 6
      Heating:Electricity,     !- Variable or Meter 7 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 7
      Cooling:Electricity,     !- Variable or Meter 8 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 8
      HeatRejection:Electricity,  !- Variable or Meter 9 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 9
      Humidifier:Electricity,  !- Variable or Meter 10 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 10
      HeatRecovery:Electricity,!- Variable or Meter 11 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 11
      WaterSystems:Electricity,!- Variable or Meter 12 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 12
      Cogeneration:Electricity,!- Variable or Meter 13 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 13
      Refrigeration:Electricity,!- Variable or Meter 14 Name
      SumOrAverage;            !- Aggregation Type for Variable or Meter 14)idf",
  R"idf(
  Output:Table:Monthly,
    Building Energy Performance - Natural Gas,  !- Name
      2,                       !- Digits After Decimal
      InteriorEquipment:NaturalGas,   !- Variable or Meter 1 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 1
      ExteriorEquipment:NaturalGas,   !- Variable or Meter 2 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 2
      Heating:NaturalGas,             !- Variable or Meter 3 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 3
      Cooling:NaturalGas,             !- Variable or Meter 4 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 4
      WaterSystems:NaturalGas,        !- Variable or Meter 5 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 5
      Cogeneration:NaturalGas,        !- Variable or Meter 6 Name
      SumOrAverage;            !- Aggregation Type for Variable or Meter 6)idf",
  R"idf(
  Output:Table:Monthly,
    Building Energy Performance - District Heating,  !- Name
      2,                       !- Digits After Decimal
      InteriorLights:DistrictHeating,  !- Variable or Meter 1 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 1
      ExteriorLights:DistrictHeating,  !- Variable or Meter 2 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 2
      InteriorEquipment:DistrictHeating,  !- Variable or Meter 3 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 3
      ExteriorEquipment:DistrictHeating,  !- Variable or Meter 4 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 4
      Fans:DistrictHeating,        !- Variable or Meter 5 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 5
      Pumps:DistrictHeating,       !- Variable or Meter 6 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 6
      Heating:DistrictHeating,     !- Variable or Meter 7 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 7
      Cooling:DistrictHeating,     !- Variable or Meter 8 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 8
      HeatRejection:DistrictHeating,  !- Variable or Meter 9 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 9
      Humidifier:DistrictHeating,  !- Variable or Meter 10 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 10
      HeatRecovery:DistrictHeating,!- Variable or Meter 11 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 11
      WaterSystems:DistrictHeating,!- Variable or Meter 12 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 12
      Cogeneration:DistrictHeating,!- Variable or Meter 13 Name
      SumOrAverage;            !- Aggregation Type for Variable or Meter 13)idf",
  R"idf(
  Output:Table:Monthly,
    Building Energy Performance - District Cooling,  !- Name
      2,                       !- Digits After Decimal
      InteriorLights:DistrictCooling,  !- Variable or Meter 1 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 1
      ExteriorLights:DistrictCooling,  !- Variable or Meter 2 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 2
      InteriorEquipment:DistrictCooling,  !- Variable or Meter 3 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 3
      ExteriorEquipment:DistrictCooling,  !- Variable or Meter 4 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 4
      Fans:DistrictCooling,        !- Variable or Meter 5 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 5
      Pumps:DistrictCooling,       !- Variable or Meter 6 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 6
      Heating:DistrictCooling,     !- Variable or Meter 7 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 7
      Cooling:DistrictCooling,     !- Variable or Meter 8 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 8
      HeatRejection:DistrictCooling,  !- Variable or Meter 9 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 9
      Humidifier:DistrictCooling,  !- Variable or Meter 10 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 10
      HeatRecovery:DistrictCooling,!- Variable or Meter 11 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 11
      WaterSystems:DistrictCooling,!- Variable or Meter 12 Name
      SumOrAverage,            !- Aggregation Type for Variable or Meter 12
      Cogeneration:DistrictCooling,!- Variable or Meter 13 Name
      SumOrAverage;            !- Aggregation Type for Variable or Meter 13)idf",
  R"idf(
  Output:Table:Monthly,
    Building Energy Performance - Electricity Peak Demand,  !- Name
      2,                       !- Digits After Decimal
      Electricity:Facility,  !- Variable or Meter 1 Name
      Maximum,            !- Aggregation Type for Variable or Meter 1
      InteriorLights:Electricity,  !- Variable or Meter 1 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 1
      ExteriorLights:Electricity,  !- Variable or Meter 2 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 2
      InteriorEquipment:Electricity,  !- Variable or Meter 3 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 3
      ExteriorEquipment:Electricity,  !- Variable or Meter 4 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 4
      Fans:Electricity,        !- Variable or Meter 5 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 5
      Pumps:Electricity,       !- Variable or Meter 6 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 6
      Heating:Electricity,     !- Variable or Meter 7 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 7
      Cooling:Electricity,     !- Variable or Meter 8 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 8
      HeatRejection:Electricity,  !- Variable or Meter 9 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 9
      Humidifier:Electricity,  !- Variable or Meter 10 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 10
      HeatRecovery:Electricity,!- Variable or Meter 11 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 11
      WaterSystems:Electricity,!- Variable or Meter 12 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 12
      Cogeneration:Electricity,!- Variable or Meter 13 Name
      ValueWhenMaximumOrMinimum;            !- Aggregation Type for Variable or Meter 13)idf",
  R"idf(
  Output:Table:Monthly,
    Building Energy Performance - Natural Gas Peak Demand,  !- Name
      2,                       !- Digits After Decimal
      NaturalGas:Facility,  !- Variable or Meter 1 Name
      Maximum,            !- Aggregation Type for Variable or Meter 1
      InteriorEquipment:NaturalGas,   !- Variable or Meter 1 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 1
      ExteriorEquipment:NaturalGas,   !- Variable or Meter 2 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 2
      Heating:NaturalGas,             !- Variable or Meter 3 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 3
      Cooling:NaturalGas,             !- Variable or Meter 4 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 4
      WaterSystems:NaturalGas,        !- Variable or Meter 5 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 5
      Cogeneration:NaturalGas,        !- Variable or Meter 6 Name
      ValueWhenMaximumOrMinimum;            !- Aggregation Type for Variable or Meter 6)idf",
  R"idf(
  Output:Table:Monthly,
    Building Energy Performance - District Heating Peak Demand,  !- Name
      2,                       !- Digits After Decimal
      DistrictHeating:Facility,  !- Variable or Meter 1 Name
      Maximum,            !- Aggregation Type for Variable or Meter 1
      InteriorLights:DistrictHeating,  !- Variable or Meter 1 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 1
      ExteriorLights:DistrictHeating,  !- Variable or Meter 2 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 2
      InteriorEquipment:DistrictHeating,  !- Variable or Meter 3 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 3
      ExteriorEquipment:DistrictHeating,  !- Variable or Meter 4 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 4
      Fans:DistrictHeating,        !- Variable or Meter 5 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 5
      Pumps:DistrictHeating,       !- Variable or Meter 6 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 6
      Heating:DistrictHeating,     !- Variable or Meter 7 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 7
      Cooling:DistrictHeating,     !- Variable or Meter 8 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 8
      HeatRejection:DistrictHeating,  !- Variable or Meter 9 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 9
      Humidifier:DistrictHeating,  !- Variable or Meter 10 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 10
      HeatRecovery:DistrictHeating,!- Variable or Meter 11 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 11
      WaterSystems:DistrictHeating,!- Variable or Meter 12 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 12
      Cogeneration:DistrictHeating,!- Variable or Meter 13 Name
      ValueWhenMaximumOrMinimum;            !- Aggregation Type for Variable or Meter 13)idf",
  R"idf(
  Output:Table:Monthly,
    Building Energy Performance - District Cooling Peak Demand,  !- Name
      2,                       !- Digits After Decimal
      DistrictCooling:Facility,  !- Variable or Meter 1 Name
      Maximum,            !- Aggregation Type for Variable or Meter 1
      InteriorLights:DistrictCooling,  !- Variable or Meter 1 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 1
      ExteriorLights:DistrictCooling,  !- Variable or Meter 2 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 2
      InteriorEquipment:DistrictCooling,  !- Variable or Meter 3 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 3
      ExteriorEquipment:DistrictCooling,  !- Variable or Meter 4 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 4
      Fans:DistrictCooling,        !- Variable or Meter 5 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 5
      Pumps:DistrictCooling,       !- Variable or Meter 6 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 6
      Heating:DistrictCooling,     !- Variable or Meter 7 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 7
      Cooling:DistrictCooling,     !- Variable or Meter 8 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 8
      HeatRejection:DistrictCooling,  !- Variable or Meter 9 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 9
      Humidifier:DistrictCooling,  !- Variable or Meter 10 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 10
      HeatRecovery:DistrictCooling,!- Variable or Meter 11 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 11
      WaterSystems:DistrictCooling,!- Variable or Meter 12 Name
      ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 12
      Cogeneration:DistrictCooling,!- Variable or Meter 13 Name
      ValueWhenMaximumOrMinimum;            !- Aggregation Type for Variable or Meter 13
)idf"};

static constexpr std::array<std::string_view, 7> c_metersForced{
  //These are needed for the calibration report
  "Output:Meter:MeterFileOnly,NaturalGas:Facility,Daily;",
  "Output:Meter:MeterFileOnly,Electricity:Facility,Timestep;",
  "Output:Meter:MeterFileOnly,Electricity:Facility,Daily;",

  // Always add in the timestep facility meters
  "Output:Meter,Electricity:Facility,Timestep;",
  "Output:Meter,NaturalGas:Facility,Timestep;",
  "Output:Meter,DistrictCooling:Facility,Timestep;",
  "Output:Meter,DistrictHeating:Facility,Timestep;",
};
