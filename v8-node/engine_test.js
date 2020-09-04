console.log("starting up");
var assert = require('assert');
console.log("loading libraries");
var openstudio = require('./OpenStudio.js').openstudio;
console.log("libraries loaded");
openstudio.Logger.instance().standardOutLogger().setLogLevel(-3);


var engine = new openstudio.runmanager.SimulationEngine(new openstudio.path("/tmp/testsimulationengine"), 1);

var model = openstudio.model.exampleModel();
var b = model.building().get();

var angles = new openstudio.DoubleVector();
angles.add(0.0);
angles.add(180.0);
angles.add(90.0);
angles.add(45.0);
angles.add(22.5);
angles.add(11.25);
angles.add(5.625);

var variables = new openstudio.DoubleVector();
var discreteVariables = new openstudio.IntVector();

variables.add(0.0);
discreteVariables.add(1);

for (var i = 0; i < angles.size(); ++i)
{
  b.setNorthAxis(angles.get(i));
  variables.set(0, angles.get(i));
  var uses = engine.fuelUses(model, variables, discreteVariables, "" + i, new openstudio.path("/usr/local/EnergyPlus-8-0-0/WeatherData/USA_CO_Golden-NREL.724666_TMY3.epw"));
  console.log("confidence: " + uses.confidence() + " for: " + variables.get(0) + " Electricity: " + uses.fuelUse(new openstudio.FuelType("Electricity")) + " Gas: " + uses.fuelUse(new openstudio.FuelType("Gas")));
}




while(true)
{
  openstudio.System.msleep(30000);

  for (var i = 0; i < angles.size(); ++i)
  {
    variables.set(0, angles.get(i));
    uses = engine.fuelUses("" + i);
    console.log("confidence: " + uses.confidence() + " for: " + variables.get(0) + " Electricity: " + uses.fuelUse(new openstudio.FuelType("Electricity")) + " Gas: " + uses.fuelUse(new openstudio.FuelType("Gas")));  
  }
}

