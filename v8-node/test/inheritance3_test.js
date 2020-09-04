var assert = require('assert')
openstudio = require('../OpenStudio.js').openstudio;

model = new openstudio.model.Model();

// Add an air loop
var alwaysOnSchedule = new openstudio.model.ScheduleCompact(model);
alwaysOnSchedule.setName("ALWAYS_ON");
alwaysOnSchedule.setString(3,"Through: 12/31");
alwaysOnSchedule.setString(4,"For: AllDays");
alwaysOnSchedule.setString(5,"Until: 24:00");
alwaysOnSchedule.setString(6,"1");


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


var coilCooling = new openstudio.model.CoilCoolingDXSingleSpeed( model,
    alwaysOnSchedule,
    coolingCurveFofTemp,
    coolingCurveFofTemp,
    coolingCurveFofTemp,
    coolingCurveFofTemp,
    coolingCurveFofTemp);

