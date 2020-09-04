var assert = require('assert')
openstudio = require('../OpenStudio.js').openstudio;

model = new openstudio.model.Model();

// this checks the ability to cast up the inheritance hierarchy when calling functions
var opaqueMaterials = new openstudio.model.OpaqueMaterialVector();

// Exterior Wall
var m01_100mm_brick = new openstudio.model.StandardOpaqueMaterial(model);
m01_100mm_brick.setName("M01 100mm brick");
m01_100mm_brick.setRoughness("MediumRough");
m01_100mm_brick.setThickness(0.1016);
m01_100mm_brick.setThermalConductivity(0.89);
m01_100mm_brick.setDensity(1920.0);
m01_100mm_brick.setSpecificHeat(790.0);

// if this function call succeeds, we can pass them up
opaqueMaterials.add(m01_100mm_brick);


