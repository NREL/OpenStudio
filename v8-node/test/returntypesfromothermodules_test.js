var assert = require('assert')
openstudio = require('../OpenStudio.js').openstudio;

openstudio.Logger.instance().standardOutLogger().setLogLevel(-2)

var outdir = openstudio.tempDir().append(new openstudio.path("TestJavaScriptReturnTypes"));
openstudio.create_directory(outdir);

var osmpath = outdir.append(new openstudio.path("test.osm"));
console.log("Saving OSM to " + openstudio.toString(osmpath));

model = openstudio.model.exampleModel();
model.save(osmpath, true);

vt = new openstudio.osversion.VersionTranslator();
model2 = vt.loadModel(osmpath);

// not being able to manipulate an OptionalModel returned from the VersionTranslator was the
// first sign of a problem here
assert.equal(true, model2.is_initialized())
assert.equal(model.modelObjects().size(), model2.get().modelObjects().size());
