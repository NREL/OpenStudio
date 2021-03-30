using System;
using System.Linq; // contains
using Xunit;
using OpenStudio;

namespace OpenStudio.Tests
{

    public class BasicTests
    {
        [Fact]
        public void BasicModel()
        {

            var model = new OpenStudio.Model();

            var space1 = new OpenStudio.Space(model);
            space1.setName("First Space");
            var space2 = new OpenStudio.Space(model);
            space2.setName("Second Space");

            var spaces = model.getSpaces().ToList();
            Assert.True(spaces.Count == 2);
            Assert.True(spaces.Any(_=>_.nameString() == "First Space"));

            
            Assert.True(true);
            var zone1 = new OpenStudio.ThermalZone(model);
            zone1.setName("First Zone");
            var zone2 = new OpenStudio.ThermalZone(model);
            zone2.setName("Second Zone");

            var zones = model.getThermalZones().ToList();
            Assert.True(zones.Count == 2);
            Assert.True(zones.Any(_=>_.nameString() == "First Zone"));
        }
    }
}
