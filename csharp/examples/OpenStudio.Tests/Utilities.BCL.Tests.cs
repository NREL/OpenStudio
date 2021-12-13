using System;
using NUnit.Framework;
using System.IO;

namespace OpenStudioTests
{
    public class BCLTest1
    {
        [Test]
        public void MeasureTypeTest()
        {
            // In Test mode, the path appears evaluated not from this file but from the bin dir which is 3 extra levels down
            var testRoot = Path.GetDirectoryName(this.GetType().Assembly.Location);
            string[] separatingStrings = { "OpenStudio.Tests" };
            //D:\a\OpenStudio\OpenStudio\csharp\examples\OpenStudio.Tests\
            var testProj = testRoot.Split(separatingStrings, System.StringSplitOptions.RemoveEmptyEntries)[0];

            // OpenStudio root folder
            var projectRoot = Path.GetDirectoryName(Path.GetDirectoryName(Path.GetDirectoryName(testProj)));

            var fullPath = Path.Combine(projectRoot, "resources/Examples/compact_osw/measures/IncreaseRoofRValue/");
            System.Console.WriteLine(fullPath);
            var folderExist = Directory.Exists(fullPath);
            if (!folderExist)
                throw new System.ArgumentException($"Invalid path: {fullPath}");

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
