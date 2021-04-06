using System;
using Xunit;
using OpenStudio;

namespace OpenStudio.Tests
{
    public class BCLTest1
    {
        [Fact]
        public void MeasureTypeTest()
        {
            // In Test mode, the path appears evaluated not from this file but from the bin dir which is 3 extra levels down
            var fullPath = System.IO.Path.GetFullPath("../../../../../resources/Examples/compact_osw/measures/IncreaseRoofRValue/");
            // Console.WriteLine(fullPath);
            Assert.True(System.IO.Directory.Exists(fullPath));


            // Test for #2795: Creating a WorkflowJSON and calling setMeasureSteps with a MeasureType");
            var wf = new OpenStudio.WorkflowJSON();

            var type = new OpenStudio.MeasureType("ModelMeasure");
            var steps = new OpenStudio.MeasureStepVector();

            var measure = new OpenStudio.MeasureStep(fullPath);
            measure.setArgument("r_value", "45");
            steps.Add(measure);
            wf.setMeasureSteps(type, steps);

            // Console.WriteLine(wf.toString());

            var wfSteps = wf.workflowSteps().ToArray();
            // Console.WriteLine("There are {0} steps", wfSteps.Length);

            Assert.True(1 == wfSteps.Length);
        }
    }
}
