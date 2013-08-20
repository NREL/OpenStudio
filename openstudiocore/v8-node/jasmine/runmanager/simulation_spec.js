var openstudio = require('../../OpenStudio.js').openstudio;

describe("RunManager", function() {
  // Test that we can run a simulation with runmanager
  it("should be able to run a simulation", function() {

    model = openstudio.model.exampleModel();

    var epw_path = openstudio.getSharedResourcesPath().append(new openstudio.path("runmanager")).append(new openstudio.path("USA_CO_Golden-NREL.724666_TMY3.epw"));

    console.log("Loading EPW from " + openstudio.toString(epw_path));

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

    workflow.addJob(new openstudio.runmanager.JobType("ModelToIdf"));
    workflow.addJob(new openstudio.runmanager.JobType("ExpandObjects"));
    //workflow.addJob(new openstudio.runmanager.JobType("EnergyPlusPreProcess"));
    workflow.addJob(new openstudio.runmanager.JobType("EnergyPlus"));
    workflow.add(co.getTools());

    var outdir = openstudio.tempDir().append(new openstudio.path("TestJavaScriptRun"));
    console.log("Running simulation in " + openstudio.toString(outdir));
    openstudio.create_directory(outdir);

    var osmpath = outdir.append(new openstudio.path("test.osm"));
    console.log("Saving OSM to " + openstudio.toString(osmpath));

    expect(model.save(osmpath, true)).toBeTruthy();

    // Uncomment this line to parallelize the energyplus run
    //workflow.parallelizeEnergyPlus(co.getMaxLocalJobs(), 1);
    var job = workflow.create(outdir, osmpath);

    runmanager.setPaused(true);
    runmanager.enqueue(job, true);
    runmanager.setPaused(false);

    job.waitForFinished();
    runmanager.waitForFinished();

    expect(runmanager.workPending()).toBeFalsy();

    outfiles = job.treeOutputFiles();

    expect(outfiles.files().size() > 0);

    var sqlpath = outfiles.getLastByExtension("sql").fullPath;
    log(openstudio.toString(sqlpath));


    /// load the results from energyplus

    var sqlfile = new openstudio.SqlFile(sqlpath);

    var siteenergy = sqlfile.netSiteEnergy();
    expect(siteenergy.isNull()).toBeFalsy();
    expect(siteenergy.get()).toBeGreaterThan(0);

    log("Site used " + siteenergy.get());
  });
});
