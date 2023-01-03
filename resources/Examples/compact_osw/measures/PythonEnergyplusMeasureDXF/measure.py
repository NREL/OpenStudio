import openstudio


class PythonEnergyplusMeasureDXF(openstudio.measure.EnergyPlusMeasure):
    """An EnergyPlusMeasure."""

    def name(self):
        """Returns the human readable name.

        Measure name should be the title case of the class name.
        The measure name is the first contact a user has with the measure;
        it is also shared throughout the measure workflow, visible in the OpenStudio Application,
        PAT, Server Management Consoles, and in output reports.
        As such, measure names should clearly describe the measure's function,
        while remaining general in nature
        """
        return "Python EnergyPlusMeasure to produce a DXF"

    def description(self):
        """Human readable description.

        The measure description is intended for a general audience and should not assume
        that the reader is familiar with the design and construction practices suggested by the measure.
        """
        return "This is just a sample"

    def modeler_description(self):
        """Human readable description of modeling approach.

        The modeler description is intended for the energy modeler using the measure.
        It should explain the measure's intent, and include any requirements about
        how the baseline model must be set up, major assumptions made by the measure,
        and relevant citations or references to applicable modeling resources
        """
        return "it uses python"

    def arguments(self, workspace: openstudio.Workspace):
        """Prepares user arguments for the measure.

        Measure arguments define which -- if any -- input parameters the user may set before running the measure.
        """
        args = openstudio.measure.OSArgumentVector()

        dxf_type = openstudio.measure.OSArgument.makeChoiceArgument('dxf_type', ["DXF", "DXF:WireFrame"], True)
        dxf_type.setDisplayName("DXF Type")
        dxf_type.setDescription("DXF Type for the eplusout.dxf")

        args.append(dxf_type)

        return args

    def run(
        self,
        workspace: openstudio.Workspace,
        runner: openstudio.measure.OSRunner,
        user_arguments: openstudio.measure.OSArgumentMap,
    ):
        """Defines what happens when the measure is run."""
        super().run(workspace, runner, user_arguments)  # Do **NOT** remove this line

        if not (runner.validateUserArguments(self.arguments(workspace), user_arguments)):
            return False

        dxf_type = runner.getStringArgumentValue('dxf_type', user_arguments)

        idfObject: openstudio.IdfObject = openstudio.IdfObject(openstudio.IddObjectType("Output:Surfaces:Drawing"))
        result = idfObject.setString(0, dxf_type)
        if not result:
            runner.registerError(f"Something went wrong when trying to set the Report Type to {dxf_type}")
            return False
        if dxf_type == "DXF":
            result = idfObject.setString(1, "Triangulate3DFace")
            if not result:
                runner.registerError("Something went wrong when trying to set the Report Specifications 1 to Triangulate3DFace")
                return False
        wsObject_: openstudio.OptionalWorkspaceObject = workspace.addObject(idfObject)
        if not wsObject_.is_initialized():
            runner.registerError("Couldn't add idfObject to workspace:\n{idfObject}")

        runner.registerInfo(f"Report added:\n'{wsObject_.get()}'")

        # report final condition of model
        finishing_dxfs = workspace.getObjectsByType("Output:Surfaces:Drawing")
        runner.registerFinalCondition(f"The building finished with {len(finishing_dxfs)} zones.")

        return True


# register the measure to be used by the application
PythonEnergyplusMeasureDXF().registerWithApplication()
