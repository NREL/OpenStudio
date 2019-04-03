console.log("starting up");
var assert = require('assert');
console.log("loading libraries");
var openstudio = require('./OpenStudio.js').openstudio;
console.log("libraries loaded");
openstudio.Logger.instance().standardOutLogger().setLogLevel(-3);

// model = openstudio.model.exampleModel();


var epw_path = openstudio.getSharedResourcesPath().append(new openstudio.path("runmanager")).append(new openstudio.path("USA_CO_Golden-NREL.724666_TMY3.epw"));

console.log("Loading EPW from " + openstudio.toString(epw_path));

// epw_file = new openstudio.EpwFile(epw_path);
// openstudio.model.WeatherFile.setWeatherFile(model, epw_file);

// We're constructing the runmanager here so that it can initialize
// the Qt UI for us. This is only necessary because we show some
// UI code for the sake of the demo.
var runmanager = new openstudio.runmanager.RunManager();
var co = new openstudio.runmanager.ConfigOptions();
co.findTools(true, true, true, true);

runmanager.setConfigOptions(co);

var workflow = new openstudio.runmanager.Workflow();

workflow.addJob(new openstudio.runmanager.JobType("ModelToIdf"));
workflow.addJob(new openstudio.runmanager.JobType("ExpandObjects"));
//workflow.addJob(new openstudio.runmanager.JobType("EnergyPlusPreProcess"));
workflow.addJob(new openstudio.runmanager.JobType("EnergyPlus"));
workflow.add(co.getTools());

var outdir = openstudio.tempDir().append(new openstudio.path("TestJavaScriptRun"));
console.log("Running simulation in " + openstudio.toString(outdir));
openstudio.create_directory(outdir);

var osmpath = new openstudio.path(process.argv[2])
//var osmpath = outdir.append(new openstudio.path("test.osm"));
//console.log("Saving OSM to " + openstudio.toString(osmpath));

if (process.argv[3] == "true")
{
  console.log("Optimizing OSM for performance over accuracy");
  console.log("Loading OSM");
  var model = openstudio.model.Model.load(osmpath);

  if (model.is_initialized())
  {
    console.log("Optimizing OSM");
    openstudio.runmanager.RunManager.simplifyModelForPerformance(model.get());
    console.log("Saving OSM");
    
    var newosmpath = outdir.append(new openstudio.path("optimized.osm"));
    model.get().save(newosmpath, true);
    console.log("OSM Saved");
    osmpath = newosmpath;
  } else {
    console.log("Failure to load OSM");
  }
}

if (process.argv[4] == "true")
{
  workflow.parallelizeEnergyPlus(co.getMaxLocalJobs(), 7);
}


var job = workflow.create(outdir, osmpath, epw_path);

runmanager.enqueue(job, true);

runmanager.waitForFinished();

outfiles = job.treeOutputFiles();

var sqlpath = outfiles.getLastByExtension("sql").fullPath;
console.log(openstudio.toString(sqlpath));


/// load the results from energyplus

var sqlfile = new openstudio.SqlFile(sqlpath);

var siteenergy = sqlfile.netSiteEnergy();

console.log("Site used " + siteenergy.get());
