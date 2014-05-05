
public class TestOpenStudio
{
  public static void main(String [] args)
  {
    System.loadLibrary("OpenStudioAnalysisDriver_java");
    System.loadLibrary("OpenStudioAnalysis_java");
    System.loadLibrary("OpenStudioContam_java");
    System.loadLibrary("OpenStudioEnergyPlus_java");
    System.loadLibrary("OpenStudioGBXML_java");
    System.loadLibrary("OpenStudioISOModel_java");
    System.loadLibrary("OpenStudioLib_java");
    System.loadLibrary("OpenStudioModelCore_java");
    System.loadLibrary("OpenStudioModelEditor_java");
    System.loadLibrary("OpenStudioModelGeometry_java");
    System.loadLibrary("OpenStudioModelHVAC_java");
    System.loadLibrary("OpenStudioModel_java");
    System.loadLibrary("OpenStudioModelRefrigeration_java");
    System.loadLibrary("OpenStudioModelResources_java");
    System.loadLibrary("OpenStudioModelSimulation_java");
    System.loadLibrary("OpenStudioOSVersion_java");
    System.loadLibrary("OpenStudioProject_java");
    System.loadLibrary("OpenStudioRadiance_java");
    System.loadLibrary("OpenStudioRuleset_java");
    System.loadLibrary("OpenStudioRunManager_java");
    System.loadLibrary("OpenStudioSDD_java");
    System.loadLibrary("OpenStudioUtilitiesBCL_java");
    System.loadLibrary("OpenStudioUtilitiesCloud_java");
    System.loadLibrary("OpenStudioUtilitiesCore_java");
    System.loadLibrary("OpenStudioUtilitiesData_java");
    System.loadLibrary("OpenStudioUtilitiesDocument_java");
    System.loadLibrary("OpenStudioUtilitiesEconomics_java");
    System.loadLibrary("OpenStudioUtilitiesFileTypes_java");
    System.loadLibrary("OpenStudioUtilitiesGeometry_java");
    System.loadLibrary("OpenStudioUtilitiesIdd_java");
    System.loadLibrary("OpenStudioUtilitiesIdf_java");
    System.loadLibrary("OpenStudioUtilities_java");
    System.loadLibrary("OpenStudioUtilitiesPlot_java");
    System.loadLibrary("OpenStudioUtilitiesSql_java");
    System.loadLibrary("OpenStudioUtilitiesTime_java");
    System.loadLibrary("OpenStudioUtilitiesUnits_java");

    System.out.println(gov.nrel.openstudio.utilitiescore_global.toString(gov.nrel.openstudio.utilitiescore_global.getApplicationBuildDirectory()));

    gov.nrel.openstudio.modelcore_global.exampleModel().save(gov.nrel.openstudio.utilitiescore_global.toPath("example.osm"), true);



    gov.nrel.openstudio.Model model = gov.nrel.openstudio.modelcore_global.exampleModel();


    gov.nrel.openstudio.SimulationControl simulationControl = gov.nrel.openstudio.modelsimulation_global.getSimulationControl(model);

    assert false == simulationControl.isDoZoneSizingCalculationDefaulted(); // from self
    assert 8 == simulationControl.children().size(); // from modelcore ParentObject
    assert 24 == simulationControl.attributes().size(); // from modelcore ModelObject
    assert false == simulationControl.isTarget(); // from openstudiocore WorkspaceObject
    assert true == simulationControl.model().Equals(model);

    /*
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
    */



  }
};
