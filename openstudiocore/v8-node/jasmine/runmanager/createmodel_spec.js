var openstudio = require('../../OpenStudio.js').openstudio;


function addExampleSchedules(model) {

  // nice to pre-define Temperature limits because default limits do not provide bounds
  var temperatureLimits = new openstudio.model.ScheduleTypeLimits(model);
  temperatureLimits.setName("Temperature");
  temperatureLimits.setLowerLimitValue(-60.0);
  temperatureLimits.setUpperLimitValue(200.0);
  temperatureLimits.setNumericType("CONTINUOUS");
  temperatureLimits.setUnitType("Temperature");

  var defaultScheduleSet = new openstudio.model.DefaultScheduleSet(model);
  defaultScheduleSet.setName("Default Schedules");

  var numberOfPeopleSchedule = new openstudio.model.ScheduleRuleset(model);
  numberOfPeopleSchedule.setName("Medium Office Number of People Schedule");
  numberOfPeopleSchedule.defaultDaySchedule().setName("Medium Office Number of People All Other Days Schedule");
  var numberOfPeopleSummerDesignDay = new openstudio.model.ScheduleDay(model);
  numberOfPeopleSummerDesignDay.setName("Medium Office Number of People Summer Design Day Schedule");
  numberOfPeopleSummerDesignDay.clearValues();
  numberOfPeopleSummerDesignDay.addValue(new openstudio.Time(0,6,0,0),0.0);
  numberOfPeopleSummerDesignDay.addValue(new openstudio.Time(0,22,0,0),1.0);
  numberOfPeopleSummerDesignDay.addValue(new openstudio.Time(0,24,0,0),0.05);
  numberOfPeopleSchedule.setSummerDesignDaySchedule(numberOfPeopleSummerDesignDay);
  var numberOfPeopleWeekdaysRule = new openstudio.model.ScheduleRule(numberOfPeopleSchedule);
  numberOfPeopleWeekdaysRule.setName("Medium Office Number of People Schedule Weekdays Rule");
  numberOfPeopleWeekdaysRule.setApplyMonday(true);
  numberOfPeopleWeekdaysRule.setApplyTuesday(true);
  numberOfPeopleWeekdaysRule.setApplyWednesday(true);
  numberOfPeopleWeekdaysRule.setApplyThursday(true);
  numberOfPeopleWeekdaysRule.setApplyFriday(true);
  numberOfPeopleWeekdaysRule.daySchedule().setName("Medium Office Number of People Weekdays Schedule");
  numberOfPeopleWeekdaysRule.daySchedule().clearValues();
  numberOfPeopleWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,6,0,0),0.0);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,7,0,0),0.1);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,8,0,0),0.2);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,12,0,0),0.95);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,13,0,0),0.5);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,17,0,0),0.95);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,18,0,0),0.7);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,20,0,0),0.4);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,22,0,0),0.1);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,24,0,0),0.05);
  var numberOfPeopleSaturdayRule = new openstudio.model.ScheduleRule(numberOfPeopleSchedule);
  numberOfPeopleSaturdayRule.setName("Medium Office Number of People Schedule Saturday Rule");
  numberOfPeopleSaturdayRule.setApplySaturday(true);
  numberOfPeopleSaturdayRule.daySchedule().setName("Medium Office Number of People Saturday Schedule");
  numberOfPeopleSaturdayRule.daySchedule().clearValues();
  numberOfPeopleSaturdayRule.daySchedule().addValue(new openstudio.Time(0,6,0,0),0.0);
  numberOfPeopleSaturdayRule.daySchedule().addValue(new openstudio.Time(0,8,0,0),0.1);
  numberOfPeopleSaturdayRule.daySchedule().addValue(new openstudio.Time(0,14,0,0),0.5);
  numberOfPeopleSaturdayRule.daySchedule().addValue(new openstudio.Time(0,17,0,0),0.1);
  numberOfPeopleSaturdayRule.daySchedule().addValue(new openstudio.Time(0,24,0,0),0.0);

  defaultScheduleSet.setNumberofPeopleSchedule(numberOfPeopleSchedule);
  
  var peopleActivityLevelSchedule = new openstudio.model.ScheduleRuleset(model);
  peopleActivityLevelSchedule.setName("Medium Office People Activity Level Schedule");
  peopleActivityLevelSchedule.defaultDaySchedule().setName("Medium Office People Activity Level All Days Schedule");
  peopleActivityLevelSchedule.defaultDaySchedule().clearValues();
  peopleActivityLevelSchedule.defaultDaySchedule().addValue(new openstudio.Time(0,24,0,0),120.0);

  defaultScheduleSet.setPeopleActivityLevelSchedule(peopleActivityLevelSchedule);
  
  var lightingSchedule = new openstudio.model.ScheduleRuleset(model);
  lightingSchedule.setName("Medium Office Lighting Schedule");
  lightingSchedule.defaultDaySchedule().setName("Medium Office Lighting All Other Days Schedule");
  lightingSchedule.defaultDaySchedule().clearValues();
  lightingSchedule.defaultDaySchedule().addValue(new openstudio.Time(0,24,0,0),0.05);
  var lightingSummerDesignDay = new openstudio.model.ScheduleDay(model);
  lightingSummerDesignDay.setName("Medium Office Lighting Summer Design Day Schedule");
  lightingSummerDesignDay.clearValues();
  lightingSummerDesignDay.addValue(new openstudio.Time(0,6,0,0),1.0);
  lightingSchedule.setSummerDesignDaySchedule(lightingSummerDesignDay);
  var lightingWinterDesignDay = new openstudio.model.ScheduleDay(model);
  lightingWinterDesignDay.setName("Medium Office Lighting Winter Design Day Schedule");
  lightingSchedule.setWinterDesignDaySchedule(lightingWinterDesignDay);
  var lightingWeekdaysRule = new openstudio.model.ScheduleRule(lightingSchedule);
  lightingWeekdaysRule.setName("Medium Office Lighting Schedule Weekdays Rule");
  lightingWeekdaysRule.setApplyMonday(true);
  lightingWeekdaysRule.setApplyTuesday(true);
  lightingWeekdaysRule.setApplyWednesday(true);
  lightingWeekdaysRule.setApplyThursday(true);
  lightingWeekdaysRule.setApplyFriday(true);
  lightingWeekdaysRule.daySchedule().setName("Medium Office Lighting Weekdays Schedule");
  lightingWeekdaysRule.daySchedule().clearValues();
  lightingWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,5,0,0),0.05);
  lightingWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,7,0,0),0.1);
  lightingWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,8,0,0),0.3);
  lightingWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,17,0,0),0.9);
  lightingWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,18,0,0),0.7);
  lightingWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,20,0,0),0.5);
  lightingWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,22,0,0),0.3);
  lightingWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,23,0,0),0.1);
  lightingWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,24,0,0),0.05);
  var lightingSaturdayRule = new openstudio.model.ScheduleRule(lightingSchedule);
  lightingSaturdayRule.setName("Medium Office Lighting Schedule Saturday Rule");
  lightingSaturdayRule.setApplySaturday(true);
  lightingSaturdayRule.daySchedule().setName("Medium Office Lighting Saturday Schedule");
  lightingSaturdayRule.daySchedule().clearValues();
  lightingSaturdayRule.daySchedule().addValue(new openstudio.Time(0,6,0,0),0.05);
  lightingSaturdayRule.daySchedule().addValue(new openstudio.Time(0,8,0,0),0.1);
  lightingSaturdayRule.daySchedule().addValue(new openstudio.Time(0,14,0,0),0.5);
  lightingSaturdayRule.daySchedule().addValue(new openstudio.Time(0,17,0,0),0.15);
  lightingSaturdayRule.daySchedule().addValue(new openstudio.Time(0,24,0,0),0.05);

  defaultScheduleSet.setLightingSchedule(lightingSchedule);
  
  var electricEquipmentSchedule = new openstudio.model.ScheduleRuleset(model);
  electricEquipmentSchedule.setName("Medium Office Electric Equipment Schedule");
  electricEquipmentSchedule.defaultDaySchedule().setName("Medium Office Electric Equipment All Other Days Schedule");
  electricEquipmentSchedule.defaultDaySchedule().clearValues();
  electricEquipmentSchedule.defaultDaySchedule().addValue(new openstudio.Time(0,24,0,0),0.3);
  var electricEquipmentSummerDesignDay = new openstudio.model.ScheduleDay(model);
  electricEquipmentSummerDesignDay.setName("Medium Office Electric Equipment Summer Design Day Schedule");
  electricEquipmentSummerDesignDay.clearValues();
  electricEquipmentSummerDesignDay.addValue(new openstudio.Time(0,6,0,0),1.0);
  electricEquipmentSchedule.setSummerDesignDaySchedule(electricEquipmentSummerDesignDay);
  var electricEquipmentWinterDesignDay = new openstudio.model.ScheduleDay(model);
  electricEquipmentWinterDesignDay.setName("Medium Office Electric Equipment Winter Design Day Schedule");
  electricEquipmentSchedule.setWinterDesignDaySchedule(electricEquipmentWinterDesignDay);
  var electricEquipmentWeekdaysRule = new openstudio.model.ScheduleRule(electricEquipmentSchedule);
  electricEquipmentWeekdaysRule.setName("Medium Office Electric Equipment Schedule Weekdays Rule");
  electricEquipmentWeekdaysRule.setApplyMonday(true);
  electricEquipmentWeekdaysRule.setApplyTuesday(true);
  electricEquipmentWeekdaysRule.setApplyWednesday(true);
  electricEquipmentWeekdaysRule.setApplyThursday(true);
  electricEquipmentWeekdaysRule.setApplyFriday(true);
  electricEquipmentWeekdaysRule.daySchedule().setName("Medium Office Electric Equipment Weekdays Schedule");
  electricEquipmentWeekdaysRule.daySchedule().clearValues();
  electricEquipmentWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,8,0,0),0.4);
  electricEquipmentWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,12,0,0),0.9);
  electricEquipmentWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,13,0,0),0.8);
  electricEquipmentWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,17,0,0),0.9);
  electricEquipmentWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,18,0,0),0.8);
  electricEquipmentWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,20,0,0),0.6);
  electricEquipmentWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,22,0,0),0.5);
  electricEquipmentWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,24,0,0),0.4);
  var electricEquipmentSaturdayRule = new openstudio.model.ScheduleRule(electricEquipmentSchedule);
  electricEquipmentSaturdayRule.setName("Medium Office Electric Equipment Schedule Saturday Rule");
  electricEquipmentSaturdayRule.setApplySaturday(true);
  electricEquipmentSaturdayRule.daySchedule().setName("Medium Office Electric Equipment Saturday Schedule");
  electricEquipmentSaturdayRule.daySchedule().clearValues();
  electricEquipmentSaturdayRule.daySchedule().addValue(new openstudio.Time(0,6,0,0),0.3);
  electricEquipmentSaturdayRule.daySchedule().addValue(new openstudio.Time(0,8,0,0),0.4);
  electricEquipmentSaturdayRule.daySchedule().addValue(new openstudio.Time(0,14,0,0),0.5);
  electricEquipmentSaturdayRule.daySchedule().addValue(new openstudio.Time(0,17,0,0),0.35);
  electricEquipmentSaturdayRule.daySchedule().addValue(new openstudio.Time(0,24,0,0),0.3);

  defaultScheduleSet.setElectricEquipmentSchedule(electricEquipmentSchedule);
  
  var gasEquipmentSchedule = new openstudio.model.ScheduleRuleset(model);
  gasEquipmentSchedule.setName("Medium Office Gas Equipment Schedule");
  gasEquipmentSchedule.defaultDaySchedule().setName("Medium Office Gas Equipment All Other Days Schedule");
  gasEquipmentSchedule.defaultDaySchedule().clearValues();
  gasEquipmentSchedule.defaultDaySchedule().addValue(new openstudio.Time(0,24,0,0),0.3);
  var gasEquipmentSummerDesignDay = new openstudio.model.ScheduleDay(model);
  gasEquipmentSummerDesignDay.setName("Medium Office Gas Equipment Summer Design Day Schedule");
  gasEquipmentSummerDesignDay.clearValues();
  gasEquipmentSummerDesignDay.addValue(new openstudio.Time(0,6,0,0),1.0);
  gasEquipmentSchedule.setSummerDesignDaySchedule(gasEquipmentSummerDesignDay);
  var gasEquipmentWinterDesignDay = new openstudio.model.ScheduleDay(model);
  gasEquipmentWinterDesignDay.setName("Medium Office Gas Equipment Winter Design Day Schedule");
  gasEquipmentSchedule.setWinterDesignDaySchedule(gasEquipmentWinterDesignDay);
  var gasEquipmentWeekdaysRule = new openstudio.model.ScheduleRule(gasEquipmentSchedule);
  gasEquipmentWeekdaysRule.setName("Medium Office Gas Equipment Schedule Weekdays Rule");
  gasEquipmentWeekdaysRule.setApplyMonday(true);
  gasEquipmentWeekdaysRule.setApplyTuesday(true);
  gasEquipmentWeekdaysRule.setApplyWednesday(true);
  gasEquipmentWeekdaysRule.setApplyThursday(true);
  gasEquipmentWeekdaysRule.setApplyFriday(true);
  gasEquipmentWeekdaysRule.daySchedule().setName("Medium Office Gas Equipment Weekdays Schedule");
  gasEquipmentWeekdaysRule.daySchedule().clearValues();
  gasEquipmentWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,8,0,0),0.4);
  gasEquipmentWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,12,0,0),0.9);
  gasEquipmentWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,13,0,0),0.8);
  gasEquipmentWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,17,0,0),0.9);
  gasEquipmentWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,18,0,0),0.8);
  gasEquipmentWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,20,0,0),0.6);
  gasEquipmentWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,22,0,0),0.5);
  gasEquipmentWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,24,0,0),0.4);
  var gasEquipmentSaturdayRule = new openstudio.model.ScheduleRule(gasEquipmentSchedule);
  gasEquipmentSaturdayRule.setName("Medium Office Gas Equipment Schedule Saturday Rule");
  gasEquipmentSaturdayRule.setApplySaturday(true);
  gasEquipmentSaturdayRule.daySchedule().setName("Medium Office Gas Equipment Saturday Schedule");
  gasEquipmentSaturdayRule.daySchedule().clearValues();
  gasEquipmentSaturdayRule.daySchedule().addValue(new openstudio.Time(0,6,0,0),0.3);
  gasEquipmentSaturdayRule.daySchedule().addValue(new openstudio.Time(0,8,0,0),0.4);
  gasEquipmentSaturdayRule.daySchedule().addValue(new openstudio.Time(0,14,0,0),0.5);
  gasEquipmentSaturdayRule.daySchedule().addValue(new openstudio.Time(0,17,0,0),0.35);
  gasEquipmentSaturdayRule.daySchedule().addValue(new openstudio.Time(0,24,0,0),0.3);  

  defaultScheduleSet.setGasEquipmentSchedule(gasEquipmentSchedule);
  
  var hotWaterEquipmentSchedule = new openstudio.model.ScheduleRuleset(model);
  hotWaterEquipmentSchedule.setName("Medium Office Hot Water Equipment Schedule");
  hotWaterEquipmentSchedule.defaultDaySchedule().setName("Medium Office Hot Water Equipment Default Schedule");
  hotWaterEquipmentSchedule.defaultDaySchedule().clearValues();
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(new openstudio.Time(0,5,0,0),0.04);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(new openstudio.Time(0,6,0,0),0.07);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(new openstudio.Time(0,11,0,0),0.04);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(new openstudio.Time(0,13,0,0),0.06);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(new openstudio.Time(0,14,0,0),0.09);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(new openstudio.Time(0,15,0,0),0.06);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(new openstudio.Time(0,21,0,0),0.04);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(new openstudio.Time(0,22,0,0),0.07);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(new openstudio.Time(0,24,0,0),0.04);
  var hotWaterEquipmentSummerDesignDay = new openstudio.model.ScheduleDay(model);
  hotWaterEquipmentSummerDesignDay.setName("Medium Office Hot Water Equipment Summer Design Day Schedule");
  hotWaterEquipmentSummerDesignDay.clearValues();
  hotWaterEquipmentSummerDesignDay.addValue(new openstudio.Time(0,5,0,0),0.05);
  hotWaterEquipmentSummerDesignDay.addValue(new openstudio.Time(0,6,0,0),0.08);
  hotWaterEquipmentSummerDesignDay.addValue(new openstudio.Time(0,7,0,0),0.07);
  hotWaterEquipmentSummerDesignDay.addValue(new openstudio.Time(0,8,0,0),0.19);
  hotWaterEquipmentSummerDesignDay.addValue(new openstudio.Time(0,9,0,0),0.35);
  hotWaterEquipmentSummerDesignDay.addValue(new openstudio.Time(0,10,0,0),0.38);
  hotWaterEquipmentSummerDesignDay.addValue(new openstudio.Time(0,11,0,0),0.39);
  hotWaterEquipmentSummerDesignDay.addValue(new openstudio.Time(0,12,0,0),0.47);
  hotWaterEquipmentSummerDesignDay.addValue(new openstudio.Time(0,13,0,0),0.57);
  hotWaterEquipmentSummerDesignDay.addValue(new openstudio.Time(0,14,0,0),0.54);
  hotWaterEquipmentSummerDesignDay.addValue(new openstudio.Time(0,15,0,0),0.34);
  hotWaterEquipmentSummerDesignDay.addValue(new openstudio.Time(0,16,0,0),0.33);
  hotWaterEquipmentSummerDesignDay.addValue(new openstudio.Time(0,17,0,0),0.44);
  hotWaterEquipmentSummerDesignDay.addValue(new openstudio.Time(0,18,0,0),0.26);
  hotWaterEquipmentSummerDesignDay.addValue(new openstudio.Time(0,19,0,0),0.21);
  hotWaterEquipmentSummerDesignDay.addValue(new openstudio.Time(0,20,0,0),0.15);
  hotWaterEquipmentSummerDesignDay.addValue(new openstudio.Time(0,21,0,0),0.17);
  hotWaterEquipmentSummerDesignDay.addValue(new openstudio.Time(0,22,0,0),0.08);
  hotWaterEquipmentSummerDesignDay.addValue(new openstudio.Time(0,24,0,0),0.05);
  hotWaterEquipmentSchedule.setSummerDesignDaySchedule(hotWaterEquipmentSummerDesignDay);
  var hotWaterEquipmentWeekdaysRule = new openstudio.model.ScheduleRule(hotWaterEquipmentSchedule,hotWaterEquipmentSummerDesignDay);
  hotWaterEquipmentWeekdaysRule.setName("Medium Office Hot Water Schedule Weekdays Rule");
  hotWaterEquipmentWeekdaysRule.setApplyMonday(true);
  hotWaterEquipmentWeekdaysRule.setApplyTuesday(true);
  hotWaterEquipmentWeekdaysRule.setApplyWednesday(true);
  hotWaterEquipmentWeekdaysRule.setApplyThursday(true);
  hotWaterEquipmentWeekdaysRule.setApplyFriday(true);
  hotWaterEquipmentWeekdaysRule.daySchedule().setName("Medium Office Hot Water Equipment Weekdays Schedule");
  var hotWaterEquipmentWinterDesignDay = new openstudio.model.ScheduleDay(model);
  hotWaterEquipmentWinterDesignDay.setName("Medium Office Hot Water Equipment Winter Design Day Schedule");
  hotWaterEquipmentWinterDesignDay.clearValues();
  hotWaterEquipmentWinterDesignDay.addValue(new openstudio.Time(0,5,0,0),0.05);
  hotWaterEquipmentWinterDesignDay.addValue(new openstudio.Time(0,6,0,0),0.08);
  hotWaterEquipmentWinterDesignDay.addValue(new openstudio.Time(0,7,0,0),0.07);
  hotWaterEquipmentWinterDesignDay.addValue(new openstudio.Time(0,8,0,0),0.11);
  hotWaterEquipmentWinterDesignDay.addValue(new openstudio.Time(0,9,0,0),0.15);
  hotWaterEquipmentWinterDesignDay.addValue(new openstudio.Time(0,10,0,0),0.21);
  hotWaterEquipmentWinterDesignDay.addValue(new openstudio.Time(0,11,0,0),0.19);
  hotWaterEquipmentWinterDesignDay.addValue(new openstudio.Time(0,12,0,0),0.23);
  hotWaterEquipmentWinterDesignDay.addValue(new openstudio.Time(0,13,0,0),0.20);
  hotWaterEquipmentWinterDesignDay.addValue(new openstudio.Time(0,14,0,0),0.19);
  hotWaterEquipmentWinterDesignDay.addValue(new openstudio.Time(0,15,0,0),0.15);
  hotWaterEquipmentWinterDesignDay.addValue(new openstudio.Time(0,16,0,0),0.13);
  hotWaterEquipmentWinterDesignDay.addValue(new openstudio.Time(0,17,0,0),0.14);
  hotWaterEquipmentWinterDesignDay.addValue(new openstudio.Time(0,21,0,0),0.07);
  hotWaterEquipmentWinterDesignDay.addValue(new openstudio.Time(0,22,0,0),0.09);
  hotWaterEquipmentWinterDesignDay.addValue(new openstudio.Time(0,24,0,0),0.05);
  hotWaterEquipmentSchedule.setWinterDesignDaySchedule(hotWaterEquipmentWinterDesignDay);
  var hotWaterEquipmentSaturdayRule = new openstudio.model.ScheduleRule(hotWaterEquipmentSchedule,hotWaterEquipmentWinterDesignDay);
  hotWaterEquipmentSaturdayRule.setName("Medium Office Hot Water Schedule Saturday Rule");
  hotWaterEquipmentSaturdayRule.setApplySaturday(true);
  hotWaterEquipmentSaturdayRule.daySchedule().setName("Medium Office Hot Water Equipment Saturday Schedule");  

  defaultScheduleSet.setHotWaterEquipmentSchedule(hotWaterEquipmentSchedule);
  
  var infiltrationSchedule = new openstudio.model.ScheduleRuleset(model);
  infiltrationSchedule.setName("Medium Office Infiltration Schedule");
  infiltrationSchedule.defaultDaySchedule().setName("Medium Office Infiltration Default Schedule");
  infiltrationSchedule.defaultDaySchedule().clearValues();
  infiltrationSchedule.defaultDaySchedule().addValue(new openstudio.Time(0,24,0,0),1.0);
  var infiltrationSummerDesignDay = new openstudio.model.ScheduleDay(model);
  infiltrationSummerDesignDay.setName("Medium Office Infiltration Summer Design Day Schedule");
  infiltrationSummerDesignDay.clearValues();
  infiltrationSummerDesignDay.addValue(new openstudio.Time(0,6,0,0),1.0);
  infiltrationSummerDesignDay.addValue(new openstudio.Time(0,22,0,0),0.25);
  infiltrationSummerDesignDay.addValue(new openstudio.Time(0,24,0,0),1.0);
  infiltrationSchedule.setSummerDesignDaySchedule(infiltrationSummerDesignDay);
  var infiltrationWeekdaysRule = new openstudio.model.ScheduleRule(infiltrationSchedule,infiltrationSummerDesignDay);
  infiltrationWeekdaysRule.setName("Medium Office Infiltration Schedule Weekdays Rule");
  infiltrationWeekdaysRule.setApplyMonday(true);
  infiltrationWeekdaysRule.setApplyTuesday(true);
  infiltrationWeekdaysRule.setApplyWednesday(true);
  infiltrationWeekdaysRule.setApplyThursday(true);
  infiltrationWeekdaysRule.setApplyFriday(true);
  infiltrationWeekdaysRule.daySchedule().setName("Medium Office Infiltration Weekdays Schedule");
  var infiltrationWinterDesignDay = new openstudio.model.ScheduleDay(model);
  infiltrationWinterDesignDay.setName("Medium Office Infiltration Winter Design Day Schedule");
  infiltrationWinterDesignDay.clearValues();
  infiltrationWinterDesignDay.addValue(new openstudio.Time(0,6,0,0),1.0);
  infiltrationWinterDesignDay.addValue(new openstudio.Time(0,18,0,0),0.25);
  infiltrationWinterDesignDay.addValue(new openstudio.Time(0,24,0,0),1.0);
  infiltrationSchedule.setWinterDesignDaySchedule(infiltrationWinterDesignDay);
  var infiltrationSaturdayRule = new openstudio.model.ScheduleRule(infiltrationSchedule,infiltrationWinterDesignDay);
  infiltrationSaturdayRule.setName("Medium Office Infiltration Schedule Saturday Rule");
  infiltrationSaturdayRule.setApplySaturday(true);
  infiltrationSaturdayRule.daySchedule().setName("Medium Office Infiltration Saturday Schedule");  

  defaultScheduleSet.setInfiltrationSchedule(infiltrationSchedule);
  
  var coolingSetpointSchedule = new openstudio.model.ScheduleRuleset(model);
  coolingSetpointSchedule.setName("Medium Office Cooling Setpoint Schedule");
  coolingSetpointSchedule.setScheduleTypeLimits(temperatureLimits);
  coolingSetpointSchedule.defaultDaySchedule().setName("Medium Office Cooling Setpoint All Other Days Schedule");  
  coolingSetpointSchedule.defaultDaySchedule().clearValues();
  coolingSetpointSchedule.defaultDaySchedule().addValue(new openstudio.Time(0,24,0,0),26.7);
  var coolingSetpointSummerDesignDay = new openstudio.model.ScheduleDay(model);
  coolingSetpointSummerDesignDay.setName("Medium Office Cooling Setpoint Summer Design Day Schedule");
  coolingSetpointSummerDesignDay.setScheduleTypeLimits(temperatureLimits);
  coolingSetpointSummerDesignDay.clearValues();
  coolingSetpointSummerDesignDay.addValue(new openstudio.Time(0,6,0,0),26.7);
  coolingSetpointSummerDesignDay.addValue(new openstudio.Time(0,22,0,0),24.0);
  coolingSetpointSummerDesignDay.addValue(new openstudio.Time(0,24,0,0),26.7);
  coolingSetpointSchedule.setSummerDesignDaySchedule(coolingSetpointSummerDesignDay);
  var coolingSetpointWeekdaysRule = new openstudio.model.ScheduleRule(coolingSetpointSchedule,coolingSetpointSummerDesignDay);
  coolingSetpointWeekdaysRule.setName("Medium Office Cooling Setpoint Schedule Weekdays Rule");
  coolingSetpointWeekdaysRule.setApplyMonday(true);
  coolingSetpointWeekdaysRule.setApplyTuesday(true);
  coolingSetpointWeekdaysRule.setApplyWednesday(true);
  coolingSetpointWeekdaysRule.setApplyThursday(true);
  coolingSetpointWeekdaysRule.setApplyFriday(true);
  coolingSetpointWeekdaysRule.daySchedule().setName("Medium Office Cooling Setpoint Weekdays Schedule");
  var coolingSetpointSaturdayRule = new openstudio.model.ScheduleRule(coolingSetpointSchedule);
  coolingSetpointSaturdayRule.setName("Medium Office Cooling Setpoint Schedule Saturday Rule");
  coolingSetpointSaturdayRule.setApplySaturday(true);
  coolingSetpointSaturdayRule.daySchedule().setName("Medium Office Cooling Setpoint Saturday Schedule");
  coolingSetpointSaturdayRule.daySchedule().clearValues();
  coolingSetpointSaturdayRule.daySchedule().addValue(new openstudio.Time(0,6,0,0),26.7);
  coolingSetpointSaturdayRule.daySchedule().addValue(new openstudio.Time(0,18,0,0),24.0);
  coolingSetpointSaturdayRule.daySchedule().addValue(new openstudio.Time(0,24,0,0),26.7);
    
  var heatingSetpointSchedule = new openstudio.model.ScheduleRuleset(model);
  heatingSetpointSchedule.setName("Medium Office Heating Setpoint Schedule");
  heatingSetpointSchedule.setScheduleTypeLimits(temperatureLimits);
  heatingSetpointSchedule.defaultDaySchedule().setName("Medium Office Heating Setpoint All Other Days Schedule");
  heatingSetpointSchedule.defaultDaySchedule().clearValues();
  heatingSetpointSchedule.defaultDaySchedule().addValue(new openstudio.Time(0,24,0,0),15.6);
  var heatingSetpointWinterDesignDay = new openstudio.model.ScheduleDay(model);
  heatingSetpointWinterDesignDay.setName("Medium Office Heating Setpoint Winter Design Day Schedule");
  heatingSetpointWinterDesignDay.setScheduleTypeLimits(temperatureLimits);
  heatingSetpointWinterDesignDay.clearValues();
  heatingSetpointWinterDesignDay.addValue(new openstudio.Time(0,24,0,0),21.0);
  heatingSetpointSchedule.setWinterDesignDaySchedule(heatingSetpointWinterDesignDay);
  var heatingSetpointWeekdaysRule = new openstudio.model.ScheduleRule(heatingSetpointSchedule);
  heatingSetpointWeekdaysRule.setName("Medium Office Heating Setpoint Schedule Weekdays Rule");
  heatingSetpointWeekdaysRule.setApplyMonday(true);
  heatingSetpointWeekdaysRule.setApplyTuesday(true);
  heatingSetpointWeekdaysRule.setApplyWednesday(true);
  heatingSetpointWeekdaysRule.setApplyThursday(true);
  heatingSetpointWeekdaysRule.setApplyFriday(true);
  heatingSetpointWeekdaysRule.daySchedule().setName("Medium Office Heating Setpoint Weekdays Schedule");
  heatingSetpointWeekdaysRule.daySchedule().clearValues();
  heatingSetpointWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,6,0,0),15.6);
  heatingSetpointWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,22,0,0),21.0);
  heatingSetpointWeekdaysRule.daySchedule().addValue(new openstudio.Time(0,24,0,0),15.6);
  var heatingSetpointSaturdayRule = new openstudio.model.ScheduleRule(heatingSetpointSchedule);
  heatingSetpointSaturdayRule.setName("Medium Office Heating Setpoint Schedule Saturday Rule");
  heatingSetpointSaturdayRule.setApplySaturday(true);
  heatingSetpointSaturdayRule.daySchedule().setName("Medium Office Heating Setpoint Saturday Schedule");
  heatingSetpointSaturdayRule.daySchedule().clearValues();
  heatingSetpointSaturdayRule.daySchedule().addValue(new openstudio.Time(0,6,0,0),15.6);
  heatingSetpointSaturdayRule.daySchedule().addValue(new openstudio.Time(0,18,0,0),21.0);
  heatingSetpointSaturdayRule.daySchedule().addValue(new openstudio.Time(0,24,0,0),15.6);
}

function addExampleConstructions(model) {

  var defaultConstructions = new openstudio.model.DefaultConstructionSet(model);
  defaultConstructions.setName("Default Constructions");
  var exteriorSurfaceConstructions = new openstudio.model.DefaultSurfaceConstructions(model);
  exteriorSurfaceConstructions.setName("Exterior Surface Constructions");
  defaultConstructions.setDefaultExteriorSurfaceConstructions(exteriorSurfaceConstructions);
  var interiorSurfaceConstructions = new openstudio.model.DefaultSurfaceConstructions(model);
  interiorSurfaceConstructions.setName("Interior Surface Constructions");
  defaultConstructions.setDefaultInteriorSurfaceConstructions(interiorSurfaceConstructions);
  var groundContactSurfaceConstructions = new openstudio.model.DefaultSurfaceConstructions(model);
  groundContactSurfaceConstructions.setName("Ground Contact Surface Constructions");
  defaultConstructions.setDefaultGroundContactSurfaceConstructions(groundContactSurfaceConstructions);
  var exteriorSubSurfaceConstructions = new openstudio.model.DefaultSubSurfaceConstructions(model);
  exteriorSubSurfaceConstructions.setName("Exterior SubSurface Constructions");
  defaultConstructions.setDefaultExteriorSubSurfaceConstructions(exteriorSubSurfaceConstructions);
  var interiorSubSurfaceConstructions = new openstudio.model.DefaultSubSurfaceConstructions(model);
  interiorSubSurfaceConstructions.setName("Interior SubSurface Constructions");
  defaultConstructions.setDefaultInteriorSubSurfaceConstructions(interiorSubSurfaceConstructions);

  var opaqueMaterials = new openstudio.model.OpaqueMaterialVector();

  // Exterior Wall

  var m01_100mm_brick = new openstudio.model.StandardOpaqueMaterial(model);
  m01_100mm_brick.setName("M01 100mm brick");
  m01_100mm_brick.setRoughness("MediumRough");
  m01_100mm_brick.setThickness(0.1016);
  m01_100mm_brick.setThermalConductivity(0.89);
  m01_100mm_brick.setDensity(1920.0);
  m01_100mm_brick.setSpecificHeat(790.0);

  opaqueMaterials.add(m01_100mm_brick);

  var m15_200mm_heavyweight_concrete = new openstudio.model.StandardOpaqueMaterial(model);
  m15_200mm_heavyweight_concrete.setName("M15 200mm heavyweight concrete");
  m15_200mm_heavyweight_concrete.setRoughness("MediumRough");
  m15_200mm_heavyweight_concrete.setThickness(0.2032);
  m15_200mm_heavyweight_concrete.setThermalConductivity(1.95);
  m15_200mm_heavyweight_concrete.setDensity(2240.0);
  m15_200mm_heavyweight_concrete.setSpecificHeat(900.0);

  opaqueMaterials.add(m15_200mm_heavyweight_concrete);

  var i02_50mm_insulation_board = new openstudio.model.StandardOpaqueMaterial(model);
  i02_50mm_insulation_board.setName("I02 50mm insulation board");
  i02_50mm_insulation_board.setRoughness("MediumRough");
  i02_50mm_insulation_board.setThickness(0.0508);
  i02_50mm_insulation_board.setThermalConductivity(0.03);
  i02_50mm_insulation_board.setDensity(43.0);
  i02_50mm_insulation_board.setSpecificHeat(1210.0);

  opaqueMaterials.add(i02_50mm_insulation_board);

  var f04_wall_air_space_resistance = new openstudio.model.AirGap(model);
  f04_wall_air_space_resistance.setName("F04 Wall air space resistance");
  f04_wall_air_space_resistance.setThermalResistance(0.15);

  opaqueMaterials.add(f04_wall_air_space_resistance);

  var g01a_19mm_gypsum_board = new openstudio.model.StandardOpaqueMaterial(model);
  g01a_19mm_gypsum_board.setName("G01a 19mm gypsum board");
  g01a_19mm_gypsum_board.setRoughness("MediumSmooth");
  g01a_19mm_gypsum_board.setThickness(0.019);
  g01a_19mm_gypsum_board.setThermalConductivity(0.16);
  g01a_19mm_gypsum_board.setDensity(800.0);
  g01a_19mm_gypsum_board.setSpecificHeat(1090.0);

  opaqueMaterials.add(g01a_19mm_gypsum_board);

  var exteriorWall = new openstudio.model.Construction(opaqueMaterials);
  exteriorWall.setName("Exterior Wall");
  exteriorWall.setInsulation(i02_50mm_insulation_board);
  exteriorSurfaceConstructions.setWallConstruction(exteriorWall);
  opaqueMaterials.clear();

  // Exterior Roof

  var m11_100mm_lightweight_concrete = new openstudio.model.StandardOpaqueMaterial(model);
  m11_100mm_lightweight_concrete.setName("M11 100mm lightweight concrete");
  m11_100mm_lightweight_concrete.setRoughness("MediumRough");
  m11_100mm_lightweight_concrete.setThickness(0.1016);
  m11_100mm_lightweight_concrete.setThermalConductivity(0.53);
  m11_100mm_lightweight_concrete.setDensity(1280.0);
  m11_100mm_lightweight_concrete.setSpecificHeat(840.0);

  opaqueMaterials.add(m11_100mm_lightweight_concrete);

  var f05_ceiling_air_space_resistance = new openstudio.model.AirGap(model);
  f05_ceiling_air_space_resistance.setName("F05 Ceiling air space resistance");
  f05_ceiling_air_space_resistance.setThermalResistance(0.18);

  opaqueMaterials.add(f05_ceiling_air_space_resistance);

  var f16_acoustic_tile = new openstudio.model.StandardOpaqueMaterial(model);
  f16_acoustic_tile.setName("F16 Acoustic tile");
  f16_acoustic_tile.setRoughness("MediumSmooth");
  f16_acoustic_tile.setThickness(0.0191);
  f16_acoustic_tile.setThermalConductivity(0.06);
  f16_acoustic_tile.setDensity(368.0);
  f16_acoustic_tile.setSpecificHeat(590.0);

  opaqueMaterials.add(f16_acoustic_tile);

  var exteriorRoof = new openstudio.model.Construction(opaqueMaterials);
  exteriorRoof.setName("Exterior Roof");
  exteriorSurfaceConstructions.setRoofCeilingConstruction(exteriorRoof);
  opaqueMaterials.clear();

  // Interior Floor

  opaqueMaterials.add(f16_acoustic_tile);
  opaqueMaterials.add(f05_ceiling_air_space_resistance);
  opaqueMaterials.add(m11_100mm_lightweight_concrete);

  var interiorFloor = new openstudio.model.Construction(opaqueMaterials);
  interiorFloor.setName("Interior Floor");
  interiorSurfaceConstructions.setFloorConstruction(interiorFloor);
  opaqueMaterials.clear();

  // Air Wall

  var airWallMaterial = new openstudio.model.AirWallMaterial(model);
  airWallMaterial.setName("Air Wall Material");

  var airWall = new openstudio.model.Construction(openstudio.model.toModelPartitionMaterial(airWallMaterial).get());
  airWall.setName("Air Wall");
  interiorSurfaceConstructions.setWallConstruction(airWall);

  // Interior Ceiling

  opaqueMaterials.add(m11_100mm_lightweight_concrete);
  opaqueMaterials.add(f05_ceiling_air_space_resistance);
  opaqueMaterials.add(f16_acoustic_tile);

  var interiorCeiling = new openstudio.model.Construction(opaqueMaterials);
  interiorCeiling.setName("Interior Ceiling");
  interiorSurfaceConstructions.setRoofCeilingConstruction(interiorCeiling);
  opaqueMaterials.clear();

  // Slab

  var mat_cc05_8_hw_concrete = new openstudio.model.StandardOpaqueMaterial(model);
  mat_cc05_8_hw_concrete.setName("MAT-CC05 8 HW CONCRETE");
  mat_cc05_8_hw_concrete.setRoughness("Rough");
  mat_cc05_8_hw_concrete.setThickness(0.2032);
  mat_cc05_8_hw_concrete.setThermalConductivity(1.3110);
  mat_cc05_8_hw_concrete.setDensity(2240.0);
  mat_cc05_8_hw_concrete.setSpecificHeat(836.8);
  mat_cc05_8_hw_concrete.setThermalAbsorptance(0.9);
  mat_cc05_8_hw_concrete.setSolarAbsorptance(0.7);
  mat_cc05_8_hw_concrete.setVisibleAbsorptance(0.7);

  opaqueMaterials.add(mat_cc05_8_hw_concrete);

  var cp02_carpet_pad = new openstudio.model.MasslessOpaqueMaterial(model);
  cp02_carpet_pad.setName("CP02 CARPET PAD");
  cp02_carpet_pad.setRoughness("VeryRough");
  cp02_carpet_pad.setThermalResistance(0.2165);
  cp02_carpet_pad.setThermalAbsorptance(0.9);
  cp02_carpet_pad.setSolarAbsorptance(0.7);
  cp02_carpet_pad.setVisibleAbsorptance(0.8);

  opaqueMaterials.add(cp02_carpet_pad);

  var slab = new openstudio.model.Construction(opaqueMaterials);
  slab.setName("Slab");
  groundContactSurfaceConstructions.setFloorConstruction(slab);
  opaqueMaterials.clear();

  var fenestrationMaterials = new openstudio.model.FenestrationMaterialVector();

  // Exterior Window

  var simple_glazing = new openstudio.model.SimpleGlazing(model);
  simple_glazing.setName("Simple Glazing");
  simple_glazing.setUFactor(3.23646);
  simple_glazing.setSolarHeatGainCoefficient(0.39);
  simple_glazing.setVisibleTransmittance(0.6);

  var clear_3mm = new openstudio.model.StandardGlazing(model);
  clear_3mm.setName("Clear 3mm");
  clear_3mm.setOpticalDataType("SpectralAverage");
  clear_3mm.setThickness(0.003);
  clear_3mm.setSolarTransmittance(0.837);
  clear_3mm.setFrontSideSolarReflectanceatNormalIncidence(0.075);
  clear_3mm.setBackSideSolarReflectanceatNormalIncidence(0.075);
  clear_3mm.setVisibleTransmittance(0.898);
  clear_3mm.setFrontSideVisibleReflectanceatNormalIncidence(0.081);
  clear_3mm.setBackSideVisibleReflectanceatNormalIncidence(0.081);
  clear_3mm.setInfraredTransmittance(0.0);
  clear_3mm.setFrontSideInfraredHemisphericalEmissivity(0.084);
  clear_3mm.setBackSideInfraredHemisphericalEmissivity(0.084);
  clear_3mm.setThermalConductivity(0.9);

  var air_13mm = new openstudio.model.Gas(model);
  air_13mm.setName("Air 13mm");
  air_13mm.setGasType("Air");
  air_13mm.setThickness(0.0127);

  //fenestrationMaterials.add(clear_3mm);
  //fenestrationMaterials.add(air_13mm);
  //fenestrationMaterials.add(clear_3mm);

  // DLM: use simple glazing so we can know window properties without requiring E+ run
  fenestrationMaterials.add(openstudio.model.toFenestrationMaterial(simple_glazing).get());

  var exteriorWindow = new openstudio.model.Construction(fenestrationMaterials);
  exteriorWindow.setName("Exterior Window");
  exteriorSubSurfaceConstructions.setFixedWindowConstruction(exteriorWindow);
  exteriorSubSurfaceConstructions.setOperableWindowConstruction(exteriorWindow);
  exteriorSubSurfaceConstructions.setGlassDoorConstruction(exteriorWindow);
  exteriorSubSurfaceConstructions.setSkylightConstruction(exteriorWindow);
  exteriorSubSurfaceConstructions.setTubularDaylightDomeConstruction(exteriorWindow);
  exteriorSubSurfaceConstructions.setTubularDaylightDiffuserConstruction(exteriorWindow);
  fenestrationMaterials.clear();

  // Exterior Door

  var f08_metal_surface = new openstudio.model.StandardOpaqueMaterial(model);
  f08_metal_surface.setName("F08 Metal surface");
  f08_metal_surface.setRoughness("Smooth");
  f08_metal_surface.setThickness(0.0008);
  f08_metal_surface.setThermalConductivity(45.28);
  f08_metal_surface.setDensity(7824.0);
  f08_metal_surface.setSpecificHeat(500.0);

  opaqueMaterials.add(f08_metal_surface);

  var i01_25mm_insulation_board = new openstudio.model.StandardOpaqueMaterial(model);
  i01_25mm_insulation_board.setName("I02 25mm insulation board");
  i01_25mm_insulation_board.setRoughness("MediumRough");
  i01_25mm_insulation_board.setThickness(0.0254);
  i01_25mm_insulation_board.setThermalConductivity(0.03);
  i01_25mm_insulation_board.setDensity(43.0);
  i01_25mm_insulation_board.setSpecificHeat(1210.0);

  opaqueMaterials.add(i01_25mm_insulation_board);

  var exteriorDoor = new openstudio.model.Construction(opaqueMaterials);
  exteriorDoor.setName("Exterior Door");
  // we shouldn't need to do this, something is not happening quite right with the layered construction inheritance hierarchy
  openstudio.model.toLayeredConstruction(exteriorDoor).get().setInsulation(i02_50mm_insulation_board);
  exteriorSubSurfaceConstructions.setDoorConstruction(exteriorDoor);
  exteriorSubSurfaceConstructions.setOverheadDoorConstruction(exteriorDoor);
  opaqueMaterials.clear();

  // Interior Window

  //fenestrationMaterials.add(clear_3mm);

  // DLM: use simple glazing so we can know window properties without requiring E+ run
  fenestrationMaterials.add(openstudio.model.toFenestrationMaterial(simple_glazing).get());

  var interiorWindow = new openstudio.model.Construction(fenestrationMaterials);
  interiorWindow.setName("Interior Window");
  interiorSubSurfaceConstructions.setFixedWindowConstruction(interiorWindow);
  interiorSubSurfaceConstructions.setOperableWindowConstruction(interiorWindow);
  interiorSubSurfaceConstructions.setGlassDoorConstruction(interiorWindow);
  interiorSubSurfaceConstructions.setSkylightConstruction(interiorWindow);
  interiorSubSurfaceConstructions.setTubularDaylightDomeConstruction(interiorWindow);
  interiorSubSurfaceConstructions.setTubularDaylightDiffuserConstruction(interiorWindow);
  fenestrationMaterials.clear();

  // Interior Door

  var g05_25mm_wood = new openstudio.model.StandardOpaqueMaterial(model);
  g05_25mm_wood.setName("G05 25mm wood");
  g05_25mm_wood.setRoughness("MediumSmooth");
  g05_25mm_wood.setThickness(0.0254);
  g05_25mm_wood.setThermalConductivity(0.15);
  g05_25mm_wood.setDensity(608.0);
  g05_25mm_wood.setSpecificHeat(1630.0);

  opaqueMaterials.add(g05_25mm_wood);

  var interiorDoor = new openstudio.model.Construction(opaqueMaterials);
  interiorDoor.setName("Interior Door");
  interiorSubSurfaceConstructions.setDoorConstruction(interiorDoor);
  interiorSubSurfaceConstructions.setOverheadDoorConstruction(interiorDoor);
  opaqueMaterials.clear();

  // Interior Partition

  opaqueMaterials.add(g05_25mm_wood);

  var interiorPartition = new openstudio.model.Construction(opaqueMaterials);
  interiorPartition.setName("Interior Partition");
  defaultConstructions.setInteriorPartitionConstruction(interiorPartition);
  opaqueMaterials.clear();
}



function addExampleModelObjects(model) 
{
  var searchResults = new openstudio.model.SurfaceVector();

  // add Version
  var version = openstudio.model.getVersion(model);

  // add SimulationControl
  var simulationControl = openstudio.model.getSimulationControl(model);
  simulationControl.setDoZoneSizingCalculation(true);
  simulationControl.setDoSystemSizingCalculation(true);
  simulationControl.setRunSimulationforSizingPeriods(false);
  simulationControl.setRunSimulationforWeatherFileRunPeriods(true);

  // add Timestep
  var timestep = openstudio.model.getTimestep(model);

  // add RunPeriod
  var runPeriod = openstudio.model.getRunPeriod(model);
  runPeriod.setBeginMonth(1);
  runPeriod.setBeginDayOfMonth(1);
  runPeriod.setEndMonth(12);
  runPeriod.setEndDayOfMonth(31);
  runPeriod.setUseWeatherFileHolidays(false);
  runPeriod.setUseWeatherFileDaylightSavings(false);
  runPeriod.setApplyWeekendHolidayRule(false);
  runPeriod.setUseWeatherFileRainInd(true);
  runPeriod.setUseWeatherFileSnowInd(true);
  runPeriod.setNumTimePeriodRepeats(1);

  // add SurfaceConvectionAlgorithmInside
  var insideSurfaceConvectionAlgorithm = openstudio.model.getInsideSurfaceConvectionAlgorithm(model);

  // add SurfaceConvectionAlgorithmOutside
  var outsideSurfaceConvectionAlgorithm = openstudio.model.getOutsideSurfaceConvectionAlgorithm(model);

  // add HeatBalanceAlgorithm
  var heatBalanceAlgorithm = openstudio.model.getHeatBalanceAlgorithm(model);

  // add ZoneAirHeatBalanceAlgorithm
  var zoneAirHeatBalanceAlgorithm = openstudio.model.getZoneAirHeatBalanceAlgorithm(model);

  // add ConvergenceLimits
  var convergenceLimits = openstudio.model.getConvergenceLimits(model);

  // add ShadowCalculation
  var shadowCalculation = openstudio.model.getShadowCalculation(model);

  // add Site
  var site = openstudio.model.getSite(model);
  site.setLatitude(41.77);
  site.setLongitude(-87.75);
  site.setTimeZone(-6.0);
  site.setElevation(190.0);

  // add SiteGroundTemperatureBuildingSurface
  var groundTemp = openstudio.model.getSiteGroundTemperatureBuildingSurface(model);
  groundTemp.setJanuaryGroundTemperature(19.527);
  groundTemp.setFebruaryGroundTemperature(19.502);
  groundTemp.setMarchGroundTemperature(19.536);
  groundTemp.setAprilGroundTemperature(19.598);
  groundTemp.setMayGroundTemperature(20.002);
  groundTemp.setJuneGroundTemperature(21.640);
  groundTemp.setJulyGroundTemperature(22.225);
  groundTemp.setAugustGroundTemperature(22.375);
  groundTemp.setSeptemberGroundTemperature(21.449);  
  groundTemp.setOctoberGroundTemperature(20.121);
  groundTemp.setNovemberGroundTemperature(19.802);
  groundTemp.setDecemberGroundTemperature(19.633);

  // add SiteWaterMainsTemperature
  var waterTemp = openstudio.model.getSiteWaterMainsTemperature(model);
  waterTemp.setAnnualAverageOutdoorAirTemperature(9.69);
  waterTemp.setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(28.10);

  // add DesignDay
  var designDay1 = new openstudio.model.DesignDay(model);
  designDay1.setMaximumDryBulbTemperature(-20.6);
  designDay1.setDailyDryBulbTemperatureRange(0.0);
  designDay1.setHumidityIndicatingConditionsAtMaximumDryBulb(-20.6);
  designDay1.setBarometricPressure(99063.0);
  designDay1.setWindSpeed(4.9);
  designDay1.setWindDirection(270);
  designDay1.setSkyClearness(0.0);
  designDay1.setRainIndicator(false);
  designDay1.setSnowIndicator(false);
  designDay1.setDayOfMonth(21);
  designDay1.setMonth(1);
  designDay1.setDayType("WinterDesignDay");
  designDay1.setDaylightSavingTimeIndicator(false);
  designDay1.setHumidityIndicatingType("WetBulb");

  var designDay2 = new openstudio.model.DesignDay(model);
  designDay2.setMaximumDryBulbTemperature(33.2);
  designDay2.setDailyDryBulbTemperatureRange(10.7);
  designDay2.setHumidityIndicatingConditionsAtMaximumDryBulb(23.8);
  designDay2.setBarometricPressure(99063.0);
  designDay2.setWindSpeed(5.3);
  designDay2.setWindDirection(230);
  designDay2.setSkyClearness(1.0);
  designDay2.setRainIndicator(false);
  designDay2.setSnowIndicator(false);
  designDay2.setDayOfMonth(21);
  designDay2.setMonth(7);
  designDay2.setDayType("SummerDesignDay");
  designDay2.setDaylightSavingTimeIndicator(false);
  designDay2.setHumidityIndicatingType("WetBulb");

  // add schedules
  addExampleSchedules(model);
  //BOOST_ASSERT(openstudio.model.getDefaultScheduleSets(model).size() >= 1);
  var defaultScheduleSet = openstudio.model.getDefaultScheduleSets(model).get(0);

  // add constructions
  addExampleConstructions(model);
  //BOOST_ASSERT(openstudio.model.getDefaultConstructionSets(model).size() >= 1);
  var defaultConstructionSet = openstudio.model.getDefaultConstructionSets(model).get(0);

  // add a space type
  var spaceType = new openstudio.model.SpaceType(model);

  // add some lights to the space type
  var lightsDefinition = new openstudio.model.LightsDefinition(model);
  lightsDefinition.setWattsperSpaceFloorArea(10.0);
  var lights = new openstudio.model.Lights(lightsDefinition);
  lights.setSpaceType(spaceType);

  // add some electric equipment to the space type
  var electricEquipmentDefinition = new openstudio.model.ElectricEquipmentDefinition(model);
  electricEquipmentDefinition.setWattsperSpaceFloorArea(5.0);
  var electricEquipment = new openstudio.model.ElectricEquipment(electricEquipmentDefinition);
  electricEquipment.setSpaceType(spaceType);

  // add some people to the space type
  var peopleDefinition = new openstudio.model.PeopleDefinition(model);
  peopleDefinition.setPeopleperSpaceFloorArea(0.05);
  var people = new openstudio.model.People(peopleDefinition);
  people.setSpaceType(spaceType);

  // create the facility
  var facility = openstudio.model.getFacility(model);

  // create the building
  var building = openstudio.model.getBuilding(model);
  building.setSpaceType(spaceType);
  building.setDefaultConstructionSet(defaultConstructionSet);
  building.setDefaultScheduleSet(defaultScheduleSet);

  // create a thermal zone
  var thermalZone = new openstudio.model.ThermalZone(model);

  var thermostat = new openstudio.model.ThermostatSetpointDualSetpoint(model);
  thermalZone.setThermostatSetpointDualSetpoint(thermostat);

  var heatingSchedule = openstudio.model.getScheduleByName(model, "Medium Office Heating Setpoint Schedule").get();
  var coolingSchedule = openstudio.model.getScheduleByName(model, "Medium Office Cooling Setpoint Schedule").get();

  thermostat.setHeatingSchedule(heatingSchedule);
  thermostat.setCoolingSchedule(coolingSchedule);

  // create a building story
  var buildingStory = new openstudio.model.BuildingStory(model);
  buildingStory.setNominalZCoordinate(0);
  buildingStory.setNominalFloortoFloorHeight(3);

  // create spaces from floor print
  var floorPrint = new openstudio.Point3dVector();
  floorPrint.add(new openstudio.Point3d(0,0,0));
  floorPrint.add(new openstudio.Point3d(0,10,0));
  floorPrint.add(new openstudio.Point3d(10,10,0));
  floorPrint.add(new openstudio.Point3d(10,0,0));

  // make spaces
  var space1 = openstudio.model.Space.fromFloorPrint(floorPrint, 3, model);
  //BOOST_ASSERT(space1);
  space1.get().setThermalZone(thermalZone);
  space1.get().setBuildingStory(buildingStory);

  var clone = space1.get().clone(model);
  // This 'toIdfObject' should not be necessary and needs to be investigated
  var space2 = openstudio.model.toSpace(clone.toIdfObject()).get();
  space2.setXOrigin(10);

  clone = space1.get().clone(model);
  var space3 = openstudio.model.toSpace(clone.toIdfObject()).get();
  space3.setYOrigin(10);

  clone = space1.get().clone(model);
  var space4 = openstudio.model.toSpace(clone.toIdfObject()).get();
  space4.setXOrigin(10);
  space4.setYOrigin(10);

  // add a door to south wall of space1
  var doorPoints = new openstudio.Point3dVector();
  doorPoints.add(new openstudio.Point3d(2,0,2));
  doorPoints.add(new openstudio.Point3d(2,0,0));
  doorPoints.add(new openstudio.Point3d(4,0,0));
  doorPoints.add(new openstudio.Point3d(4,0,2));

  // find south wall
  searchResults = space1.get().findSurfaces(
      new openstudio.OptionalDouble(180.0),
      new openstudio.OptionalDouble(180.0),
      new openstudio.OptionalDouble(90.0),
      new openstudio.OptionalDouble(90.0));
  //BOOST_ASSERT(searchResults.size() >= 1);

  // add door
  var door = new openstudio.model.SubSurface(doorPoints, model);
  door.setSurface(searchResults.get(0));
  
  // add a window to east wall of space2
  var windowPoints = new openstudio.Point3dVector();
  windowPoints.add(new openstudio.Point3d(10,2,2));
  windowPoints.add(new openstudio.Point3d(10,2,1));
  windowPoints.add(new openstudio.Point3d(10,8,1));
  windowPoints.add(new openstudio.Point3d(10,8,2));

  // find east wall
  searchResults = space2.findSurfaces(
      new openstudio.OptionalDouble(90.0),
      new openstudio.OptionalDouble(90.0),
      new openstudio.OptionalDouble(90.0),
      new openstudio.OptionalDouble(90.0));
  //BOOST_ASSERT(searchResults.size() >= 1);

  // add window
  var window = new openstudio.model.SubSurface(windowPoints, model);
  window.setSurface(searchResults.get(0));

  // add overhang to the window
  var test = window.addOverhangByProjectionFactor(0.5, 0.1);
  //BOOST_ASSERT(test);

  // add daylighting control point to center of space2 - used by radiance
  var daylightingControl = new openstudio.model.DaylightingControl(model);
  daylightingControl.setSpace(space2);
  daylightingControl.setPosition(new openstudio.Point3d(5, 5, 1.1));

  // hook daylighting control up to zone
  test = thermalZone.setPrimaryDaylightingControl(daylightingControl);
  //BOOST_ASSERT(test);
  thermalZone.setFractionofZoneControlledbyPrimaryDaylightingControl(0.25);
  
  // add illuminance map to space2
  var illuminanceMap = new openstudio.model.IlluminanceMap(model);
  illuminanceMap.setSpace(space2);
  illuminanceMap.setOriginXCoordinate(1);
  illuminanceMap.setXLength(8);
  illuminanceMap.setOriginYCoordinate(1);
  illuminanceMap.setYLength(8);
  illuminanceMap.setOriginZCoordinate(1.1);

  // hook illuminanceMap up to zone - used by radiance
  test = thermalZone.setIlluminanceMap(illuminanceMap);
  //BOOST_ASSERT(test);


  // add a glare sensor to center of space2 - used by radiance
  var glareSensor = new openstudio.model.GlareSensor(model);
  glareSensor.setSpace(space2);
  glareSensor.setPosition(new openstudio.Point3d(5, 5, 1.1));
  
  // add a desk to space 2
  var deskGroup = new openstudio.model.InteriorPartitionSurfaceGroup(model);
  deskGroup.setSpace(space2);

  var deskPoints = new openstudio.Point3dVector();
  deskPoints.add(new openstudio.Point3d(5,8,1));
  deskPoints.add(new openstudio.Point3d(5,6,1));
  deskPoints.add(new openstudio.Point3d(8,6,1));
  deskPoints.add(new openstudio.Point3d(8,8,1));
  desk = new openstudio.model.InteriorPartitionSurface(deskPoints, model);
  desk.setInteriorPartitionSurfaceGroup(deskGroup);

  // add a printer to space4
  var printerDefinition = new openstudio.model.ElectricEquipmentDefinition(model);
  printerDefinition.setName("Printer Definition");
  printerDefinition.setDesignLevel(200.0);
  var printer = new openstudio.model.ElectricEquipment(printerDefinition);
  printer.setName("Printer");
  printer.setSpace(space4);
  
  // add a building shading device
  var canopyGroup = new openstudio.model.ShadingSurfaceGroup(model);
  canopyGroup.setShadingSurfaceType("Building");
  
  var canopyPoints = new openstudio.Point3dVector();
  canopyPoints.add(new openstudio.Point3d(2,0,2));
  canopyPoints.add(new openstudio.Point3d(2,-1,2));
  canopyPoints.add(new openstudio.Point3d(4,-1,2));
  canopyPoints.add(new openstudio.Point3d(4,0,2));
  var canopy = new openstudio.model.ShadingSurface(canopyPoints, model);
  canopy.setShadingSurfaceGroup(canopyGroup);
  
  // add a neighboring building
  var neighboringBuildingGroup = new openstudio.model.ShadingSurfaceGroup(model);
  neighboringBuildingGroup.setShadingSurfaceType("Site");

  var neighboringBuildingPoints = new openstudio.Point3dVector();
  neighboringBuildingPoints.add(new openstudio.Point3d(-30,0,20));
  neighboringBuildingPoints.add(new openstudio.Point3d(-30,0,0));
  neighboringBuildingPoints.add(new openstudio.Point3d(-30,20,0));
  neighboringBuildingPoints.add(new openstudio.Point3d(-30,20,20));
  neighboringBuilding = new openstudio.model.ShadingSurface(neighboringBuildingPoints, model);
  neighboringBuilding.setShadingSurfaceGroup(neighboringBuildingGroup);

  // match surfaces
  var spaces = openstudio.model.getSpaces(model);
  openstudio.model.matchSurfaces(spaces);

  // Add an air loop
  var alwaysOnSchedule = new openstudio.model.ScheduleCompact(model);
  alwaysOnSchedule.setName("ALWAYS_ON");
  alwaysOnSchedule.setString(3,"Through: 12/31");
  alwaysOnSchedule.setString(4,"For: AllDays");
  alwaysOnSchedule.setString(5,"Until: 24:00");
  alwaysOnSchedule.setString(6,"1");

  var fan = new openstudio.model.FanConstantVolume(model,alwaysOnSchedule);
  fan.setName("Standard Fan");
  var coilHeatingGas = new openstudio.model.CoilHeatingGas(model,alwaysOnSchedule);

  var coolingCurveFofTemp = new openstudio.model.CurveBiquadratic(model);
  coolingCurveFofTemp.setCoefficient1Constant(0.42415);
  coolingCurveFofTemp.setCoefficient2x(0.04426);
  coolingCurveFofTemp.setCoefficient3xPOW2(-0.00042);
  coolingCurveFofTemp.setCoefficient4y(0.00333);
  coolingCurveFofTemp.setCoefficient5yPOW2(-0.00008);
  coolingCurveFofTemp.setCoefficient6xTIMESY(-0.00021);
  coolingCurveFofTemp.setMinimumValueofx(17.0);
  coolingCurveFofTemp.setMaximumValueofx(22.0);
  coolingCurveFofTemp.setMinimumValueofy(13.0);
  coolingCurveFofTemp.setMaximumValueofy(46.0);
  coolingCurveFofTemp.setMinimumCurveOutput(-1000);
  coolingCurveFofTemp.setMaximumCurveOutput(1000);

  var coolingCurveFofFlow = new openstudio.model.CurveQuadratic(model);
  coolingCurveFofFlow.setCoefficient1Constant(0.77136);
  coolingCurveFofFlow.setCoefficient2x(0.34053);
  coolingCurveFofFlow.setCoefficient3xPOW2(-0.11088);
  coolingCurveFofFlow.setMinimumValueofx(0.75918);
  coolingCurveFofFlow.setMaximumValueofx(1.13877);
  coolingCurveFofFlow.setMinimumCurveOutput(-1000);
  coolingCurveFofFlow.setMaximumCurveOutput(1000);

  var energyInputRatioFofTemp = new openstudio.model.CurveBiquadratic(model);
  energyInputRatioFofTemp.setCoefficient1Constant(1.23649);
  energyInputRatioFofTemp.setCoefficient2x(-0.02431);
  energyInputRatioFofTemp.setCoefficient3xPOW2(0.00057);
  energyInputRatioFofTemp.setCoefficient4y(-0.01434);
  energyInputRatioFofTemp.setCoefficient5yPOW2(0.00063);
  energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.00038);
  energyInputRatioFofTemp.setMinimumValueofx(17.0);
  energyInputRatioFofTemp.setMaximumValueofx(22.0);
  energyInputRatioFofTemp.setMaximumValueofy(13.0);
  energyInputRatioFofTemp.setMaximumValueofy(46.0);
  energyInputRatioFofTemp.setMinimumCurveOutput(-1000);
  energyInputRatioFofTemp.setMaximumCurveOutput(1000);

  var energyInputRatioFofFlow = new openstudio.model.CurveQuadratic(model);
  energyInputRatioFofFlow.setCoefficient1Constant(1.20550);
  energyInputRatioFofFlow.setCoefficient2x(-0.32953);
  energyInputRatioFofFlow.setCoefficient3xPOW2(0.12308);
  energyInputRatioFofFlow.setMinimumValueofx(0.75918);
  energyInputRatioFofFlow.setMaximumValueofx(1.13877);
  energyInputRatioFofFlow.setMinimumCurveOutput(-1000);
  energyInputRatioFofFlow.setMaximumCurveOutput(1000);

  var partLoadFraction = new openstudio.model.CurveQuadratic(model);
  partLoadFraction.setCoefficient1Constant(0.77100);
  partLoadFraction.setCoefficient2x(0.22900);
  partLoadFraction.setCoefficient3xPOW2(0.0);
  partLoadFraction.setMinimumValueofx(0.0);
  partLoadFraction.setMaximumValueofx(1.0);
  partLoadFraction.setMinimumCurveOutput(0.71);
  partLoadFraction.setMaximumCurveOutput(1.0);

  coilCooling = new openstudio.model.CoilCoolingDXSingleSpeed( model,
                                        alwaysOnSchedule,
                                        coolingCurveFofTemp,
                                        coolingCurveFofFlow,
                                        energyInputRatioFofTemp,
                                        energyInputRatioFofFlow,
                                        partLoadFraction );
  var evaporativeCoolerDirectResearchSpecial = new openstudio.model.EvaporativeCoolerDirectResearchSpecial(model,alwaysOnSchedule);

  var airTerminalSingleDuctUncontrolled = new openstudio.model.AirTerminalSingleDuctUncontrolled(model,alwaysOnSchedule);

  var controller = new openstudio.model.ControllerOutdoorAir(model);

  var outdoorAirSystem = new openstudio.model.AirLoopHVACOutdoorAirSystem(model,controller);

  var airLoopHVAC = new openstudio.model.AirLoopHVAC(model);
  
  airLoopHVAC.addBranchForZone(thermalZone,new openstudio.model.OptionalStraightComponent(airTerminalSingleDuctUncontrolled));

  var supplyInletNode = airLoopHVAC.supplyInletNode();
  var supplyOutletNode = airLoopHVAC.supplyOutletNode();

  fan.addToNode(supplyInletNode);

  coilHeatingGas.addToNode(supplyInletNode);

  coilCooling.addToNode(supplyInletNode);

  outdoorAirSystem.addToNode(supplyInletNode);

  var mixedAirNode = openstudio.model.toNode(outdoorAirSystem.mixedAirModelObject().get()).get();
  var coolCoilOutletNode = openstudio.model.toNode(coilCooling.outletModelObject().get()).get();
  var heatingCoilOutletNode = openstudio.model.toNode(coilHeatingGas.outletModelObject().get()).get();
  var oaNode = openstudio.model.toNode(outdoorAirSystem.outdoorAirModelObject().get()).get();

  evaporativeCoolerDirectResearchSpecial.addToNode(oaNode);

  var evapOutletNode = openstudio.model.toNode(evaporativeCoolerDirectResearchSpecial.outletModelObject().get().toIdfObject()).get();

  var setpointMMA1 = new openstudio.model.SetpointManagerMixedAir(model);
  var setpointMMA2 = new openstudio.model.SetpointManagerMixedAir(model);
  var setpointMMA3 = new openstudio.model.SetpointManagerMixedAir(model);
  var setpointMMA4 = new openstudio.model.SetpointManagerMixedAir(model);
  var setpointMSZR = new openstudio.model.SetpointManagerSingleZoneReheat(model);

  setpointMSZR.addToNode(supplyOutletNode);
  setpointMMA1.addToNode(mixedAirNode);
  setpointMMA2.addToNode(coolCoilOutletNode);
  setpointMMA3.addToNode(heatingCoilOutletNode);
  setpointMMA4.addToNode(evapOutletNode);

  // add some example variables
  //

  var outputVariableNames = thermalZone.outputVariableNames();
  for (var i = 0; i < 2 && i < outputVariableNames.size(); ++i)
  {
    new openstudio.model.OutputVariable(outputVariableNames.get(i), model);
  }

  var surfaces = openstudio.model.getSurfaces(model);
  for (var i = 0; i < 2 && i < surfaces.size(); ++i)
  {
    var outputVariableNames = surfaces.get(i).outputVariableNames();

    for (var j = 0; j < 2 && j < outputVariableNames.size(); ++j)
    {
      new openstudio.model.OutputVariable(outputVariableNames.get(j), model);
    }
  }
  


  // add some meters
  var electricityMeter = new openstudio.model.Meter(model);
  electricityMeter.setFuelType(new openstudio.FuelType("Electricity"));
  electricityMeter.setReportingFrequency(new openstudio.ReportingFrequency("Hourly").valueName());
  electricityMeter.setInstallLocationType(new openstudio.InstallLocationType("Facility"));

  var gasMeter = new openstudio.model.Meter(model);
  gasMeter.setFuelType(new openstudio.FuelType("Gas"));
  gasMeter.setReportingFrequency(new openstudio.ReportingFrequency("Hourly").valueName());
  gasMeter.setInstallLocationType(new openstudio.InstallLocationType("Facility"));

  var propaneMeter = new openstudio.model.Meter(model);
  propaneMeter.setFuelType(new openstudio.FuelType("Propane"));
  propaneMeter.setReportingFrequency(new openstudio.ReportingFrequency("Hourly").valueName());
  propaneMeter.setInstallLocationType(new openstudio.InstallLocationType("Facility"));
}

function exampleModel()
{
  model = new openstudio.model.Model();
  addExampleModelObjects(model);
  return model;
}






describe("Model", function() {
  // Test that we can run a simulation with runmanager
  it("should be able to create a model which can be simulated", function() {

    model = exampleModel();

    var epw_path = openstudio.getSharedResourcesPath().append(new openstudio.path("runmanager")).append(new openstudio.path("USA_CO_Golden-NREL.724666_TMY3.epw"));

    log("Loading EPW from " + openstudio.toString(epw_path));

    epw_file = new openstudio.EpwFile(epw_path);
    openstudio.model.WeatherFile.setWeatherFile(model, epw_file);

    // We're constructing the runmanager here so that it can initialize
    // the Qt UI for us. This is only necessary because we show some
    // UI code for the sake of the demo.
    var runmanager = new openstudio.runmanager.RunManager();
    var co = new openstudio.runmanager.ConfigOptions();
    co.findTools(true, true, true, true);

    runmanager.setConfigOptions(co);

    var workflow = new openstudio.runmanager.Workflow();

    var radiancetools = co.getTools().getAllByName("rad").tools();


/*
    workflow.addStandardWorkflow(
        new openstudio.path(), // we aren't using scripts
        co.getTools().getAllByName("ruby").tools().size() > 0, // was ruby found?
        openstudio.getOpenStudioRubyIncludePath(), // where are the standard ruby scripts located?
        true, // use radiance for daylight calculations - set this to false for faster simulations
        radiancetools.get(radiancetools.size()-1).localBinPath.parent_path()); // this should throw if radiance was not found
*/

    workflow.addStandardWorkflow(
        new openstudio.path(), // we aren't using scripts
        co.getTools().getAllByName("ruby").tools().size() > 0, // was ruby found?
        openstudio.getOpenStudioRubyIncludePath(), // where are the standard ruby scripts located?
        false,
        new openstudio.path()); 

    workflow.add(co.getTools());

    var outdir = openstudio.tempDir().append(new openstudio.path("TestJavaScriptRunJasmineCreatedModel"));
    log("Running simulation in " + openstudio.toString(outdir));
    openstudio.create_directory(outdir);

    var osmpath = outdir.append(new openstudio.path("test.osm"));
    log("Saving OSM to " + openstudio.toString(osmpath));

    expect(model.save(osmpath, true)).toBeTruthy();

    var job = workflow.create(outdir, osmpath);

    runmanager.enqueue(job, true);

    runmanager.waitForFinished();

    expect(runmanager.workPending()).toBeFalsy();

    outfiles = job.treeOutputFiles();

    expect(outfiles.files().size() > 0);
    console.log(outfiles.files());
    files = outfiles.files();

    for (var i = 0; i < files.size(); ++i)
    {
      console.log(openstudio.toString(files.get(i).fullPath));
    }
    
    var sqlpath = outfiles.getLastByExtension("sql").fullPath;
    log(openstudio.toString(sqlpath));
    
    var sqlpath = outfiles.getLastByExtension("sql").fullPath;
    log(openstudio.toString(sqlpath));


    /// load the results from energyplus

    var sqlfile = new openstudio.SqlFile(sqlpath);

    var siteenergy = sqlfile.totalSiteEnergy();
    expect(siteenergy.isNull()).toBeFalsy();
    expect(siteenergy.get()).toBeGreaterThan(0);

    log("Site used " + siteenergy.get());
  });
});
