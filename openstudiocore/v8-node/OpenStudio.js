
function merge_objects(obj1,obj2){
  var obj3 = {};
  for (var attrname in obj1) { obj3[attrname] = obj1[attrname]; }
  for (var attrname in obj2) { obj3[attrname] = obj2[attrname]; }
  return obj3;
}

var OpenStudioUtilitiesCore = require('OpenStudioUtilitiesCore').OpenStudioUtilitiesCore;
var OpenStudioUtilitiesTime = require('OpenStudioUtilitiesTime').OpenStudioUtilitiesTime;
var OpenStudioUtilitiesEconomics = require('OpenStudioUtilitiesEconomics').OpenStudioUtilitiesEconomics;
var OpenStudioUtilitiesTime = require('OpenStudioUtilitiesTime').OpenStudioUtilitiesTime;
var OpenStudioUtilitiesData = require('OpenStudioUtilitiesData').OpenStudioUtilitiesData;
var OpenStudioUtilitiesPlot = require('OpenStudioUtilitiesPlot').OpenStudioUtilitiesPlot;
var OpenStudioUtilitiesGeometry = require('OpenStudioUtilitiesGeometry').OpenStudioUtilitiesGeometry;
var OpenStudioUtilitiesSql = require('OpenStudioUtilitiesSql').OpenStudioUtilitiesSql;
var OpenStudioUtilitiesBCL = require('OpenStudioUtilitiesBCL').OpenStudioUtilitiesBCL;
var OpenStudioUtilitiesUnits = require('OpenStudioUtilitiesUnits').OpenStudioUtilitiesUnits;
var OpenStudioUtilitiesDocument = require('OpenStudioUtilitiesDocument').OpenStudioUtilitiesDocument;
var OpenStudioUtilitiesIdd = require('OpenStudioUtilitiesIdd').OpenStudioUtilitiesIdd;
var OpenStudioUtilitiesIdf = require('OpenStudioUtilitiesIdf').OpenStudioUtilitiesIdf;
var OpenStudioUtilitiesFileTypes = require('OpenStudioUtilitiesFileTypes').OpenStudioUtilitiesFileTypes;
var OpenStudioUtilities = require('OpenStudioUtilities').OpenStudioUtilities;
var OpenStudioEnergyPlus = require('OpenStudioEnergyPlus').OpenStudioEnergyPlus;
var OpenStudioRadiance = require('OpenStudioRadiance').OpenStudioRadiance;
var OpenStudioGBXML = require('OpenStudioGBXML').OpenStudioGBXML;
var OpenStudioModelCore = require('OpenStudioModelCore').OpenStudioModelCore;
var OpenStudioModelGeometry = require('OpenStudioModelGeometry').OpenStudioModelGeometry;
var OpenStudioModelSimulation = require('OpenStudioModelSimulation').OpenStudioModelSimulation;
var OpenStudioModelResources = require('OpenStudioModelResources').OpenStudioModelResources;
var OpenStudioModelHVAC = require('OpenStudioModelHVAC').OpenStudioModelHVAC;
var OpenStudioModel = require('OpenStudioModel').OpenStudioModel;
var OpenStudioOSVersion = require('OpenStudioOSVersion').OpenStudioOSVersion;
var OpenStudioRuleset = require('OpenStudioRuleset').OpenStudioRuleset;
var OpenStudioISOModel = require('OpenStudioISOModel').OpenStudioISOModel;
var OpenStudioRunManager = require('OpenStudioRunManager').OpenStudioRunManager;
var OpenStudioProject = require('OpenStudioProject').OpenStudioProject;
var OpenStudioAnalysisDriver = require('OpenStudioAnalysisDriver').OpenStudioAnalysisDriver;
//    var OpenStudioModelEditor = require('OpenStudioModelEditor').OpenStudioModelEditor;
var OpenStudioAnalysis = require('OpenStudioAnalysis').OpenStudioAnalysis;
var OpenStudioLib = require('OpenStudioLib').OpenStudioLib;
//    var OpenStudioPlugin = require('OpenStudioPlugin').OpenStudioPlugin;


openstudio = OpenStudioUtilitiesCore;
openstudio = merge_objects(openstudio, OpenStudioUtilitiesEconomics);
openstudio = merge_objects(openstudio, OpenStudioUtilitiesTime);
openstudio = merge_objects(openstudio, OpenStudioUtilitiesData);
openstudio = merge_objects(openstudio, OpenStudioUtilitiesPlot);
openstudio = merge_objects(openstudio, OpenStudioUtilitiesGeometry);
openstudio = merge_objects(openstudio, OpenStudioUtilitiesSql);
openstudio = merge_objects(openstudio, OpenStudioUtilitiesBCL);
openstudio = merge_objects(openstudio, OpenStudioUtilitiesUnits);
openstudio = merge_objects(openstudio, OpenStudioUtilitiesDocument);
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


