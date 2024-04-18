"""insert your copyright here.

# see the URL below for information on how to write OpenStudio measures
# http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/
"""

from pathlib import Path

import openstudio


class ReportingMeasureName(openstudio.measure.ReportingMeasure):
    """An ReportingMeasure."""

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

    def arguments(self, model: openstudio.model.Model):
        """Prepares user arguments for the measure.

        Measure arguments define which -- if any -- input parameters the user may set before running the measure.
        """
        args = openstudio.measure.OSArgumentVector()

        report_drybulb_temp = openstudio.measure.OSArgument.makeBoolArgument("report_drybulb_temp", True)
        report_drybulb_temp.setDisplayName("Add output variables for Drybulb Temperature")
        report_drybulb_temp.setDescription("Will add drybulb temp and report min/max values in html.")
        report_drybulb_temp.setValue(True)
        args.append(report_drybulb_temp)

        return args

    def outputs(self):
        """Define the outputs that the measure will create."""
        outs = openstudio.measure.OSOutputVector()

        # this measure does not produce machine readable outputs with registerValue, return an empty list

        return outs

    def energyPlusOutputRequests(
        self, runner: openstudio.measure.OSRunner, user_arguments: openstudio.measure.OSArgumentMap
    ):
        """Returns a vector of IdfObject's to request EnergyPlus objects needed by the run method."""
        super().energyPlusOutputRequests(runner, user_arguments)  # Do **NOT** remove this line

        result = openstudio.IdfObjectVector()

        # To use the built-in error checking we need the model...
        # get the last model and sql file
        model = runner.lastOpenStudioModel()
        if not model.is_initialized():
            runner.registerError("Cannot find last model.")
            return False

        model = model.get()

        # use the built-in error checking
        if not runner.validateUserArguments(self.arguments(model), user_arguments):
            return False

        if runner.getBoolArgumentValue("report_drybulb_temp", user_arguments):
            request = openstudio.IdfObject.load(
                "Output:Variable, , Site Outdoor Air Drybulb Temperature, Hourly;"
            ).get()
            result.append(request)

        return result

    def run(
        self,
        runner: openstudio.measure.OSRunner,
        user_arguments: openstudio.measure.OSArgumentMap,
    ):
        """Defines what happens when the measure is run."""
        super().run(runner, user_arguments)

        # get the last model and sql file
        model = runner.lastOpenStudioModel()
        if not model.is_initialized():
            runner.registerError("Cannot find last model.")
            return False
        model = model.get()

        # use the built-in error checking (need model)
        if not runner.validateUserArguments(self.arguments(model), user_arguments):
            return False

        # get measure arguments
        report_drybulb_temp = runner.getBoolArgumentValue("report_drybulb_temp", user_arguments)

        # load sql file
        sql_file = runner.lastEnergyPlusSqlFile()
        if not sql_file.is_initialized():
            runner.registerError("Cannot find last sql file.")
            return False

        sql_file = sql_file.get()
        model.setSqlFile(sql_file)

        style_str = """/* Style the titles */
      #pagetitle {
        text-align: center;
        font-size: 32px;
        font-weight: 700;
        color: #E2231A; /* #1b1e24; */
        font-variant: small-caps;
        border-top: none;
      }

      h1 {
        font-size: 26px;
        font-weight: 700;
        color: #3e8ddd; /* #1b1e24; */
        font-variant: small-caps;
        border-top: 1px solid #000;

      }

      h2 {
        font-size: 20px;
        color: #428BCA;
      }
"""
        output = f"""
<!DOCTYPE html>
<html lang="en">
  <head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <title>ReportingMeasure</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3" crossorigin="anonymous">

    <script type="text/javascript" src="http://cdnjs.cloudflare.com/ajax/libs/jquery/3.6.1/jquery.min.js"></script>
    <script type="text/javascript" src="http://cdnjs.cloudflare.com/ajax/libs/d3/7.6.1/d3.min.js"></script>

    <style>
      {style_str}
    </style>
  </head>
  <body>

    <div id="content" class="container-fluid">
      <div class="row">

        <div id="topofpage" class="col-md-12" role="main">
          <h1 id="pagetitle">Reporting Measure Example</h1>
        </div>  <!-- End of first main -->
      </div> <!-- End of first row (toc) -->

      <div class="row">
        <h2>Basic HTML</h2>
        <p>Measure Name = {self.name()}<br/>
           Building Name = {model.getBuilding().nameString()}<br/>
           Floor Area = {model.getBuilding().floorArea():.0f} m<sup>2</sup><br/>
           Net Site Energy = {sql_file.netSiteEnergy().get():.2f} GJ<br/>
        </p>
      </div>

      <div class="row">
        <h2>Example Table</h2>
        <table class="table table-sm table-hover table-striped">
          <thead>
            <tr>
              <th scope="col">#</th>
              <th scope="col">Value</th>
              <th scope="col">Units</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <th scope="row">Measure Name</th>
              <td>{self.name()}</td>
              <td></td>
            </tr>
            <tr>
              <th scope="row">Building Name</th>
              <td>{model.getBuilding().nameString()}</td>
              <td></td>
            </tr>
            <tr>
              <th scope="row">Floor Area</th>
              <td>{model.getBuilding().floorArea():.0f}</td>
              <td>m<sup>2</sup></td>
            </tr>
            <tr>
              <th scope="row">Net Site Energy</th>
              <td>{sql_file.netSiteEnergy().get():.2f}</td>
              <td>GJ</td>
            </tr>
"""

        if report_drybulb_temp:
            # get the weather file run period (as opposed to design day run period)
            ann_env_pd = None
            for env_pd in sql_file.availableEnvPeriods():
                env_type = sql_file.environmentType(env_pd)
                if not env_type.is_initialized():
                    continue
                if env_type.get() == openstudio.EnvironmentType("WeatherRunPeriod"):
                    ann_env_pd = env_pd
                    break
            if ann_env_pd is None:
                runner.registerWarning("No annual environment period found.")
            else:
                # get desired variable
                key_value = "Environment"
                time_step = "Hourly"  # "Zone Timestep", "Hourly", "HVAC System Timestep"
                variable_name = "Site Outdoor Air Drybulb Temperature"

                # key value would go at the end if we used it.
                output_timeseries = sql_file.timeSeries(ann_env_pd, time_step, variable_name, key_value)
                if not output_timeseries.is_initialized():
                    runner.registerWarning("Timeseries not found.")
                else:
                    runner.registerInfo("Found timeseries.")

                    output_timeseries = output_timeseries.get()
                    values = output_timeseries.values()
                    vals = [values[i] for i in range(values.size())]
                    times = output_timeseries.dateTimes()

                    data_lines = []

                    def format_dt(dt):
                        d = dt.date()
                        t = dt.time()
                        # return datetime.datetime(d.year(), d.monthOfYear().value(), d.dayOfMonth(), t.hours(), t.minutes(), t.seconds(), tzinfo=datetime.timezone.utc)
                        return f"new Date({d.year()}, {d.monthOfYear().value()}, {d.dayOfMonth()}, {t.hours()}, {t.minutes()}, {t.seconds()})"

                    for i, v in enumerate(vals):
                        data_lines.append(f"{{date: {format_dt(times[i])}, value: {v}}}")
                    date_lines_str = ",\n        ".join(data_lines)

                    output += f"""
            <tr>
              <th scope="row">Minimum Site Outdoor Air Drybulb Temperature</th>
              <td>{min(vals):.1f}</td>
              <td>C</td>
            </tr>
            <tr>
              <th scope="row">Maximum Site Outdoor Air Drybulb Temperature</th>
              <td>{max(vals):.1f}</td>
              <td>C</td>
            </tr>
         </tbody>
        </table>
      </div> <!-- End table row -->

      <div class="row">
        <h2>Site Outdoor Air Drybulb Temperature</h2>
        <div id="chartArea" class="col-xs-12 col-lg-6"></div>
      </div>
    </div> <!-- End main container fluid -->

    <script>

    var data = [
        {date_lines_str}
    ];
"""
                    output += """
    // var data = [
    //     { date: new Date(2013, 17, 1), value: 1 },
    //     { date: new Date(2013, 17, 2), value: 2 },
    //     { date: new Date(2013, 17, 3), value: 3 },
    // ];

    // set the dimensions and margins of the graph
    var margin = {top: 10, right: 30, bottom: 30, left: 60};
    // var width = 460 - margin.left - margin.right;
    var width = document.getElementById('chartArea').clientWidth;
    // var height = 400 - margin.top - margin.bottom;
    // golden rectangle ratio.
    var height = width / 3.236;

    // append the svg object to the body of the page
    var svg = d3.select("#chartArea")
      .append("svg")
        .attr("width", width + margin.left + margin.right)
        .attr("height", height + margin.top + margin.bottom)
      .append("g")
        .attr("transform",
              "translate(" + margin.left + "," + margin.top + ")");

    //We will build a basic function to handle window resizing.
    function resize() {
        width = document.getElementById('chartArea').clientWidth;
        height = width / 3.236;
        d3.select('#chartArea svg')
          .attr('width', width)
          .attr('height', height);
    }

    window.onresize = resize;

    //Call our resize function if the window size is changed.

    // Add X axis --> it is a date format
    var x = d3.scaleTime()
      .domain(d3.extent(data, function(d) { return d.date; }))
      .range([ 0, width ]);
    svg.append("g")
      .attr("transform", "translate(0," + height + ")")
      .call(d3.axisBottom(x));

    // Add Y axis
    var y = d3.scaleLinear()
      .domain([0, d3.max(data, function(d) { return +d.value; })])
      .range([ height, 0 ]);
    svg.append("g")
      .call(d3.axisLeft(y));

    // Add the line
    svg.append("path")
      .datum(data)
      .attr("fill", "none")
      .attr("stroke", "steelblue")
      .attr("stroke-width", 1.5)
      .attr("d", d3.line()
        .x(function(d) { return x(d.date) })
        .y(function(d) { return y(d.value) })
        )

    </script>
  </body>
</html>
"""
        else:
            output += """

         </tbody>
        </table>
      </div> <!-- End table row -->

    </div> <!-- End main container fluid -->

  </body>
</html>"""

        # write html file: any file named 'report*.*' in the current working directory
        # will be copied to the ./reports/ folder as 'reports/<measure_class_name>_<filename>.html'
        html_out_path = Path("./report.html").absolute()
        with open(html_out_path, "w") as f:
            f.write(output)

        # Close the sql file
        sql_file.close()

        return True


# register the measure to be used by the application
ReportingMeasureName().registerWithApplication()
