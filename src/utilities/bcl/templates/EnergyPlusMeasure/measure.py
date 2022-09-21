"""insert your copyright here.

# see the URL below for information on how to write OpenStudio measures
# http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/
"""

import openstudio


class EnergyPlusMeasureName(openstudio.measure.EnergyPlusMeasure):
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
        return "NAME_TEXT"

    def description(self):
        """Human readable description.

        The measure description is intended for a general audience and should not assume
        that the reader is familiar with the design and construction practices suggested by the measure.
        """
        return "DESCRIPTION_TEXT"

    def modeler_description(self):
        """Human readable description of modeling approach.

        The modeler description is intended for the energy modeler using the measure.
        It should explain the measure's intent, and include any requirements about
        how the baseline model must be set up, major assumptions made by the measure,
        and relevant citations or references to applicable modeling resources
        """
        return "MODELER_DESCRIPTION_TEXT"

    def arguments(self, workspace: openstudio.Workspace):
        """Prepares user arguments for the measure.

        Measure arguments define which -- if any -- input parameters the user may set before running the measure.
        """
        args = openstudio.measure.OSArgumentVector()

        zone_name = openstudio.measure.OSArgument.makeStringArgument("zone_name", True)
        zone_name.setDisplayName("New zone name")
        zone_name.setDescription("This name will be used as the name of the new zone.")
        args.append(zone_name)

        return args

    def run(
        self,
        workspace: openstudio.Workspace,
        runner: openstudio.measure.OSRunner,
        user_arguments: openstudio.measure.OSArgumentMap,
    ):
        """Defines what happens when the measure is run."""
        super().run(workspace, runner, user_arguments)

        if not (runner.validateUserArguments(self.arguments(workspace), user_arguments)):
            return False

        # assign the user inputs to variables
        zone_name = runner.getStringArgumentValue("zone_name", user_arguments)

        # check the zone_name for reasonableness
        if not zone_name:
            runner.registerError("Empty zone name was entered.")
            return False

        # get all thermal zones in the starting workspace
        zones = workspace.getObjectsByType("Zone")
        # report initial condition of workspace
        runner.registerInitialCondition(f"The building started with {len(zones)} zones.")

        # add a new zone to the model with the new name
        # http://apps1.eere.energy.gov/buildings/energyplus/pdfs/inputoutputreference.pdf#nameddest=Zone
        new_zone_string = f"""
  Zone,
    {zone_name},             !- Name
    0,                       !- Direction of Relative North {{deg}}
    0,                       !- X Origin {{m}}
    0,                       !- Y Origin {{m}}
    0,                       !- Z Origin {{m}}
    1,                       !- Type
    1,                       !- Multiplier
    autocalculate,           !- Ceiling Height {{m}}
    autocalculate;           !- Volume {{m3}}
          """

        idfObject_: openstudio.OptionalIdfObject = openstudio.IdfObject.load(new_zone_string)
        idfObject: openstudio.IdfObject = idfObject_.get()
        wsObject: openstudio.OptionalWorkspaceObject = workspace.addObject(idfObject)
        new_zone: openstudio.WorkspaceObject = wsObject.get()

        runner.registerInfo(f"A zone named '{new_zone.nameString()}' was added.")

        # report final condition of model
        finishing_zones = workspace.getObjectsByType("Zone")
        runner.registerFinalCondition(f"The building finished with {len(finishing_zones)} zones.")

        return True


# register the measure to be used by the application
EnergyPlusMeasureName().registerWithApplication()
