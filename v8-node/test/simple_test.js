var assert = require('assert')
var openstudio = require('../OpenStudio.js').openstudio;

assert.notEqual(openstudio.toString(openstudio.tempDir()), '')

model = openstudio.model.exampleModel();

var optionalbuilding = new openstudio.model.OptionalBuilding();
console.log(optionalbuilding.is_initialized());
var bldg = model.building();
console.log(bldg.is_initialized());

var runmanager = new openstudio.runmanager.RunManager();
console.log(runmanager.paused());
console.log(openstudio.toString(runmanager.dbPath()));

console.log(model.version());

