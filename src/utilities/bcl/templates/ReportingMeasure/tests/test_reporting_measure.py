"""Insert your copyright here."""

import os
import shutil
import subprocess
import sys
from pathlib import Path

import openstudio
import pytest

CURRENT_DIR_PATH = Path(__file__).parent.absolute()
sys.path.insert(0, str(CURRENT_DIR_PATH.parent))
from measure import ReportingMeasureName
sys.path.pop(0)
del sys.modules['measure']


MODEL_IN_PATH_DEFAULT = CURRENT_DIR_PATH / "example_model.osm"
EPW_IN_PATH_DEFAULT = CURRENT_DIR_PATH / "USA_CO_Golden-NREL.724666_TMY3.epw"


class TestReportingMeasureName:
    """Py.test module for ReportingMeasureName."""

    @staticmethod
    def run_dir(test_name: str) -> Path:
        return CURRENT_DIR_PATH / f"output/{test_name}"

    @staticmethod
    def model_out_path(test_name) -> Path:
        return TestReportingMeasureName.run_dir(test_name) / "example_model.osm"

    @staticmethod
    def sql_path(test_name) -> Path:
        return TestReportingMeasureName.run_dir(test_name) / "run/eplusout.sql"

    @staticmethod
    def report_path(test_name) -> Path:
        return TestReportingMeasureName.run_dir(test_name) / "report.html"

    @staticmethod
    def setup_test(
        test_name: str,
        idf_output_requests: openstudio.IdfObjectVector,
        model_in_path: Path = MODEL_IN_PATH_DEFAULT,
        epw_path: Path = EPW_IN_PATH_DEFAULT,
    ):
        run_dir = TestReportingMeasureName.run_dir(test_name)
        if run_dir.exists():
            shutil.rmtree(run_dir)
        run_dir.mkdir(parents=True)

        report_path = TestReportingMeasureName.report_path(test_name)
        if report_path.exists():
            report_path.unlink()

        assert model_in_path.exists()

        model_out_path = TestReportingMeasureName.model_out_path(test_name)
        if model_out_path.exists():
            model_out_path.unlink()

        # convert output requests to OSM for testing, OS App and PAT will add these to the E+ Idf
        workspace = openstudio.Workspace(openstudio.StrictnessLevel("Draft"), openstudio.IddFileType("EnergyPlus"))
        workspace.addObjects(idf_output_requests)
        rt = openstudio.energyplus.ReverseTranslator()
        request_model = rt.translateWorkspace(workspace)

        translator = openstudio.osversion.VersionTranslator()
        model = translator.loadModel(model_in_path)
        assert model.is_initialized()
        model = model.get()
        model.addObjects(request_model.objects())
        model.save(str(model_out_path), True)

        if os.environ.get("OPENSTUDIO_TEST_NO_CACHE_SQLFILE"):
            sql_file = TestReportingMeasureName.sql_path(test_name)
            if sql_file.exists():
                sql_file.unlink()

        osw_path = run_dir / "in.osw"

        workflow = openstudio.WorkflowJSON()

        workflow.setSeedFile(str(model_out_path))
        workflow.setWeatherFile(str(epw_path))
        workflow.saveAs(str(osw_path))

        # TODO: use the system openstudio for now, replace with openstudio.getOpenStudioCLI eventually
        cli_path = "openstudio"
        args = [cli_path, "run", "-w", str(osw_path)]
        print(" ".join(args))
        subprocess.check_call(args)

    def test_number_of_arguments_and_argument_names(self):
        """Test that the arguments are what we expect."""
        # create an instance of the measure
        measure = ReportingMeasureName()

        # make an empty model
        model = openstudio.model.Model()

        # get arguments and test that they are what we are expecting
        arguments = measure.arguments(model)
        assert arguments.size() == 1
        assert arguments[0].name() == "report_drybulb_temp"

    def test_with_drybulb_temp(self):
        """Test running the measure with appropriate arguments, with db temp."""
        test_name = "test_with_drybulb_temp"

        # create an instance of the measure
        measure = ReportingMeasureName()

        # create runner with empty OSW
        osw = openstudio.WorkflowJSON()
        runner = openstudio.measure.OSRunner(osw)

        # make an empty model
        model = openstudio.model.Model()

        # get arguments
        arguments = measure.arguments(model)
        argument_map = openstudio.measure.convertOSArgumentVectorToMap(arguments)

        # set argument values to bad value
        report_drybulb_temp = arguments[0].clone()
        assert report_drybulb_temp.setValue(True)
        argument_map["report_drybulb_temp"] = report_drybulb_temp

        # temp set path so idf_output_requests work
        runner.setLastOpenStudioModelPath(MODEL_IN_PATH_DEFAULT)

        # get the energyplus output requests, this will be done automatically by OS App and PAT
        idf_output_requests = measure.energyPlusOutputRequests(runner, argument_map)
        assert idf_output_requests.size() == 1

        # mimic the process of running this measure in OS App or PAT. Optionally set custom model_in_path and custom epw_path.
        epw_path = EPW_IN_PATH_DEFAULT
        TestReportingMeasureName.setup_test(test_name=test_name, idf_output_requests=idf_output_requests)

        model_out_path = TestReportingMeasureName.model_out_path(test_name)
        assert model_out_path.exists()
        assert epw_path.exists()
        assert TestReportingMeasureName.sql_path(test_name).exists()

        # set up runner, this will happen automatically when measure is run in PAT or OpenStudio
        runner.setLastOpenStudioModelPath(model_out_path)
        runner.setLastEpwFilePath(epw_path)
        runner.setLastEnergyPlusSqlFilePath(TestReportingMeasureName.sql_path(test_name))

        # delete the output if it exists
        report_path = TestReportingMeasureName.report_path(test_name)
        if report_path.exists():
            report_path.unlink()

        assert not report_path.exists()

        # temporarily change directory to the run directory and run the measure
        start_dir = Path.cwd()
        os.chdir(TestReportingMeasureName.run_dir(test_name))
        try:
            # run the measure
            measure.run(runner, argument_map)
        finally:
            os.chdir(start_dir)

        result = runner.result()
        print(result)
        assert result.value().valueName() == "Success"
        assert len(result.stepWarnings()) == 0

        sqlFile = runner.lastEnergyPlusSqlFile().get()
        if not sqlFile.connectionOpen():
            sqlFile.reopen()
        hours = sqlFile.hoursSimulated()
        assert hours.is_initialized()
        assert hours.get() == 8760.0

        # make sure the report file exists
        assert report_path.exists()

    def test_without_drybulb_temp(self):
        """Test running the measure with appropriate arguments, without db temp."""
        test_name = "test_without_drybulb_temp"

        # create an instance of the measure
        measure = ReportingMeasureName()

        # create runner with empty OSW
        osw = openstudio.WorkflowJSON()
        runner = openstudio.measure.OSRunner(osw)

        # make an empty model
        model = openstudio.model.Model()

        # get arguments
        arguments = measure.arguments(model)
        argument_map = openstudio.measure.convertOSArgumentVectorToMap(arguments)

        # set argument values to bad value
        report_drybulb_temp = arguments[0].clone()
        assert report_drybulb_temp.setValue(False)
        argument_map["report_drybulb_temp"] = report_drybulb_temp

        # temp set path so idf_output_requests work
        runner.setLastOpenStudioModelPath(MODEL_IN_PATH_DEFAULT)

        # get the energyplus output requests, this will be done automatically by OS App and PAT
        idf_output_requests = measure.energyPlusOutputRequests(runner, argument_map)
        assert idf_output_requests.size() == 0

        # mimic the process of running this measure in OS App or PAT. Optionally set custom model_in_path and custom epw_path.
        epw_path = EPW_IN_PATH_DEFAULT
        TestReportingMeasureName.setup_test(test_name=test_name, idf_output_requests=idf_output_requests)

        model_out_path = TestReportingMeasureName.model_out_path(test_name)
        assert model_out_path.exists()
        assert epw_path.exists()
        assert TestReportingMeasureName.sql_path(test_name).exists()

        # set up runner, this will happen automatically when measure is run in PAT or OpenStudio
        runner.setLastOpenStudioModelPath(model_out_path)
        runner.setLastEpwFilePath(epw_path)
        runner.setLastEnergyPlusSqlFilePath(TestReportingMeasureName.sql_path(test_name))

        # delete the output if it exists
        report_path = TestReportingMeasureName.report_path(test_name)
        if report_path.exists():
            report_path.unlink()

        assert not report_path.exists()

        # temporarily change directory to the run directory and run the measure
        start_dir = Path.cwd()
        os.chdir(TestReportingMeasureName.run_dir(test_name))
        try:
            # run the measure
            measure.run(runner, argument_map)
        finally:
            os.chdir(start_dir)

        result = runner.result()
        print(result)
        assert result.value().valueName() == "Success"
        assert len(result.stepWarnings()) == 0

        sqlFile = runner.lastEnergyPlusSqlFile().get()
        if not sqlFile.connectionOpen():
            sqlFile.reopen()
        hours = sqlFile.hoursSimulated()
        assert hours.is_initialized()
        assert hours.get() == 8760.0

        # make sure the report file exists
        assert report_path.exists()


# This allows running openstudio CLI on this file (`openstudio test_measure.py`, maybe with extra args)
if __name__ == "__main__":
    import sys
    if len(sys.argv) > 1:
        pytest.main([__file__] + sys.argv[1:])
    else:
        pytest.main()
