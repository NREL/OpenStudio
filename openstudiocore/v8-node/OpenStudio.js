
function merge_objects(obj1,obj2){
  var obj3 = {};
  for (var attrname in obj1) { obj3[attrname] = obj1[attrname]; }
  for (var attrname in obj2) { obj3[attrname] = obj2[attrname]; }
  return obj3;
}

function myRequire(f) {
  try {
    return require("./" + f);
  } catch (e) {
    return require(f);
  }
}


var OpenStudioUtilitiesCore = myRequire('OpenStudioUtilitiesCore');
if (typeof OpenStudioUtilitiesCore.OpenStudioUtilitiesCore != 'undefined') OpenStudioUtilitiesCore = OpenStudioUtilitiesCore.OpenStudioUtilitiesCore;
var OpenStudioUtilitiesTime = myRequire('OpenStudioUtilitiesTime');
if (typeof OpenStudioUtilitiesTime.OpenStudioUtilitiesTime != 'undefined') OpenStudioUtilitiesTime = OpenStudioUtilitiesTime.OpenStudioUtilitiesTime;
var OpenStudioUtilitiesEconomics = myRequire('OpenStudioUtilitiesEconomics');
if (typeof OpenStudioUtilitiesEconomics.OpenStudioUtilitiesEconomics != 'undefined') OpenStudioUtilitiesEconomics = OpenStudioUtilitiesEconomics.OpenStudioUtilitiesEconomics;
var OpenStudioUtilitiesTime = myRequire('OpenStudioUtilitiesTime');
if (typeof OpenStudioUtilitiesTime.OpenStudioUtilitiesTime != 'undefined') OpenStudioUtilitiesTime = OpenStudioUtilitiesTime.OpenStudioUtilitiesTime;
var OpenStudioUtilitiesData = myRequire('OpenStudioUtilitiesData');
if (typeof OpenStudioUtilitiesData.OpenStudioUtilitiesData != 'undefined') OpenStudioUtilitiesData = OpenStudioUtilitiesData.OpenStudioUtilitiesData;
var OpenStudioUtilitiesPlot = myRequire('OpenStudioUtilitiesPlot');
if (typeof OpenStudioUtilitiesPlot.OpenStudioUtilitiesPlot != 'undefined') OpenStudioUtilitiesPlot = OpenStudioUtilitiesPlot.OpenStudioUtilitiesPlot;
var OpenStudioUtilitiesGeometry = myRequire('OpenStudioUtilitiesGeometry');
if (typeof OpenStudioUtilitiesGeometry.OpenStudioUtilitiesGeometry != 'undefined') OpenStudioUtilitiesGeometry = OpenStudioUtilitiesGeometry.OpenStudioUtilitiesGeometry;
var OpenStudioUtilitiesSql = myRequire('OpenStudioUtilitiesSql');
if (typeof OpenStudioUtilitiesSql.OpenStudioUtilitiesSql != 'undefined') OpenStudioUtilitiesSql = OpenStudioUtilitiesSql.OpenStudioUtilitiesSql;
var OpenStudioUtilitiesBCL = myRequire('OpenStudioUtilitiesBCL');
if (typeof OpenStudioUtilitiesBCL.OpenStudioUtilitiesBCL != 'undefined') OpenStudioUtilitiesBCL = OpenStudioUtilitiesBCL.OpenStudioUtilitiesBCL;
var OpenStudioUtilitiesUnits = myRequire('OpenStudioUtilitiesUnits');
if (typeof OpenStudioUtilitiesUnits.OpenStudioUtilitiesUnits != 'undefined') OpenStudioUtilitiesUnits = OpenStudioUtilitiesUnits.OpenStudioUtilitiesUnits;
var OpenStudioUtilitiesIdd = myRequire('OpenStudioUtilitiesIdd');
if (typeof OpenStudioUtilitiesIdd.OpenStudioUtilitiesIdd != 'undefined') OpenStudioUtilitiesIdd = OpenStudioUtilitiesIdd.OpenStudioUtilitiesIdd;
var OpenStudioUtilitiesIdf = myRequire('OpenStudioUtilitiesIdf');
if (typeof OpenStudioUtilitiesIdf.OpenStudioUtilitiesIdf != 'undefined') OpenStudioUtilitiesIdf = OpenStudioUtilitiesIdf.OpenStudioUtilitiesIdf;
var OpenStudioUtilitiesFileTypes = myRequire('OpenStudioUtilitiesFileTypes');
if (typeof OpenStudioUtilitiesFileTypes.OpenStudioUtilitiesFileTypes != 'undefined') OpenStudioUtilitiesFileTypes = OpenStudioUtilitiesFileTypes.OpenStudioUtilitiesFileTypes;
var OpenStudioUtilities = myRequire('OpenStudioUtilities');
if (typeof OpenStudioUtilities.OpenStudioUtilities != 'undefined') OpenStudioUtilities = OpenStudioUtilities.OpenStudioUtilities;
var OpenStudioEnergyPlus = myRequire('OpenStudioEnergyPlus');
if (typeof OpenStudioEnergyPlus.OpenStudioEnergyPlus != 'undefined') OpenStudioEnergyPlus = OpenStudioEnergyPlus.OpenStudioEnergyPlus;
var OpenStudioRadiance = myRequire('OpenStudioRadiance');
if (typeof OpenStudioRadiance.OpenStudioRadiance != 'undefined') OpenStudioRadiance = OpenStudioRadiance.OpenStudioRadiance;
var OpenStudioGBXML = myRequire('OpenStudioGBXML');
if (typeof OpenStudioGBXML.OpenStudioGBXML != 'undefined') OpenStudioGBXML = OpenStudioGBXML.OpenStudioGBXML;
var OpenStudioModelCore = myRequire('OpenStudioModelCore');
if (typeof OpenStudioModelCore.OpenStudioModelCore != 'undefined') OpenStudioModelCore = OpenStudioModelCore.OpenStudioModelCore;
var OpenStudioModelGeometry = myRequire('OpenStudioModelGeometry');
if (typeof OpenStudioModelGeometry.OpenStudioModelGeometry != 'undefined') OpenStudioModelGeometry = OpenStudioModelGeometry.OpenStudioModelGeometry;
var OpenStudioModelSimulation = myRequire('OpenStudioModelSimulation');
if (typeof OpenStudioModelSimulation.OpenStudioModelSimulation != 'undefined') OpenStudioModelSimulation = OpenStudioModelSimulation.OpenStudioModelSimulation;
var OpenStudioModelResources = myRequire('OpenStudioModelResources');
if (typeof OpenStudioModelResources.OpenStudioModelResources != 'undefined') OpenStudioModelResources = OpenStudioModelResources.OpenStudioModelResources;
var OpenStudioModelHVAC = myRequire('OpenStudioModelHVAC');
if (typeof OpenStudioModelHVAC.OpenStudioModelHVAC != 'undefined') OpenStudioModelHVAC = OpenStudioModelHVAC.OpenStudioModelHVAC;
var OpenStudioModel = myRequire('OpenStudioModel');
if (typeof OpenStudioModel.OpenStudioModel != 'undefined') OpenStudioModel = OpenStudioModel.OpenStudioModel;
var OpenStudioOSVersion = myRequire('OpenStudioOSVersion');
if (typeof OpenStudioOSVersion.OpenStudioOSVersion != 'undefined') OpenStudioOSVersion = OpenStudioOSVersion.OpenStudioOSVersion;
var OpenStudioRuleset = myRequire('OpenStudioRuleset');
if (typeof OpenStudioRuleset.OpenStudioRuleset != 'undefined') OpenStudioRuleset = OpenStudioRuleset.OpenStudioRuleset;
var OpenStudioISOModel = myRequire('OpenStudioISOModel');
if (typeof OpenStudioISOModel.OpenStudioISOModel != 'undefined') OpenStudioISOModel = OpenStudioISOModel.OpenStudioISOModel;
var OpenStudioRunManager = myRequire('OpenStudioRunManager');
if (typeof OpenStudioRunManager.OpenStudioRunManager != 'undefined') OpenStudioRunManager = OpenStudioRunManager.OpenStudioRunManager;
var OpenStudioProject = myRequire('OpenStudioProject');
if (typeof OpenStudioProject.OpenStudioProject != 'undefined') OpenStudioProject = OpenStudioProject.OpenStudioProject;
var OpenStudioAnalysisDriver = myRequire('OpenStudioAnalysisDriver');
if (typeof OpenStudioAnalysisDriver.OpenStudioAnalysisDriver != 'undefined') OpenStudioAnalysisDriver = OpenStudioAnalysisDriver.OpenStudioAnalysisDriver;
//    var OpenStudioModelEditor = myRequire('OpenStudioModelEditor');
//if (typeof OpenStudioModelEditor.OpenStudioModelEditor != 'undefined') OpenStudioModelEditor = OpenStudioModelEditor.OpenStudioModelEditor;
var OpenStudioAnalysis = myRequire('OpenStudioAnalysis');
if (typeof OpenStudioAnalysis.OpenStudioAnalysis != 'undefined') OpenStudioAnalysis = OpenStudioAnalysis.OpenStudioAnalysis;
var OpenStudioLib = myRequire('OpenStudioLib');
if (typeof OpenStudioLib.OpenStudioLib != 'undefined') OpenStudioLib = OpenStudioLib.OpenStudioLib;
//    var OpenStudioPlugin = myRequire('OpenStudioPlugin');
//if (typeof OpenStudioPlugin.OpenStudioPlugin != 'undefined') OpenStudioPlugin = OpenStudioPlugin.OpenStudioPlugin;


openstudio = OpenStudioUtilitiesCore;
openstudio = merge_objects(openstudio, OpenStudioUtilitiesEconomics);
openstudio = merge_objects(openstudio, OpenStudioUtilitiesTime);
openstudio = merge_objects(openstudio, OpenStudioUtilitiesData);
openstudio = merge_objects(openstudio, OpenStudioUtilitiesPlot);
openstudio = merge_objects(openstudio, OpenStudioUtilitiesGeometry);
openstudio = merge_objects(openstudio, OpenStudioUtilitiesSql);
openstudio = merge_objects(openstudio, OpenStudioUtilitiesBCL);
openstudio = merge_objects(openstudio, OpenStudioUtilitiesUnits);
openstudio = merge_objects(openstudio, OpenStudioUtilitiesIdd);
openstudio = merge_objects(openstudio, OpenStudioUtilitiesIdf);
openstudio = merge_objects(openstudio, OpenStudioUtilitiesFileTypes);
openstudio = merge_objects(openstudio, OpenStudioUtilities);

openstudio.energyplus = OpenStudioEnergyPlus;

openstudio.radiance = OpenStudioRadiance;

openstudio.gbxml = OpenStudioGBXML;

openstudio.model = OpenStudioModelCore
openstudio.model = merge_objects(openstudio.model, OpenStudioModelGeometry);
openstudio.model = merge_objects(openstudio.model, OpenStudioModelSimulation);
openstudio.model = merge_objects(openstudio.model, OpenStudioModelResources);
openstudio.model = merge_objects(openstudio.model, OpenStudioModelHVAC);
openstudio.model = merge_objects(openstudio.model, OpenStudioModel);

openstudio.osversion = OpenStudioOSVersion;

openstudio.ruleset = OpenStudioRuleset;

openstudio.isomodel = OpenStudioISOModel;

openstudio.runmanager = OpenStudioRunManager;

openstudio.project = OpenStudioProject;

openstudio.analsysdriver = OpenStudioAnalysisDriver;

//openstudio.modeleditor = OpenStudioModelEditor;

openstudio.analysis = OpenStudioAnalysis;

openstudio.openstudiolib = OpenStudioLib;

//openstudio.openstudioplugin = OpenStudioPlugin;


exports.openstudio = openstudio;


