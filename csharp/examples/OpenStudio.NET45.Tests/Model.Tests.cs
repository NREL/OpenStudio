using System;
using System.Linq; // contains
using NUnit.Framework;
using OpenStudio;

namespace OpenStudio.Tests
{

    public class BasicTests
    {
        [Test]
        public void Model_test()
        {

            var model = new OpenStudio.Model();

            var space1 = new OpenStudio.Space(model);
            space1.setName("First Space");
            var space2 = new OpenStudio.Space(model);
            space2.setName("Second Space");

            var spaces = model.getSpaces().ToList();
            Assert.True(spaces.Count == 2);
            var found = spaces.Any(_ => _.nameString() == "First Space");
            Assert.True(found);

            
        }


        [Test]
        public void ThermalZoneTest()
        {

            var model = new OpenStudio.Model();

            var zone1 = new OpenStudio.ThermalZone(model);
            zone1.setName("First Zone");
            var zone2 = new OpenStudio.ThermalZone(model);
            zone2.setName("Second Zone");

            var zones = model.getThermalZones().ToList();
            Assert.True(zones.Count == 2);
            var found = zones.Any(_ => _.nameString() == "First Zone");
            Assert.True(found);
        }


        [Test]
        public void HVAC_Test()
        {

            var model = new OpenStudio.Model();

            var obj = new OpenStudio.CoilHeatingWater(model);
            obj.setName("A New Coil");

            var objList = model.getCoilHeatingWaters().ToList();
            Assert.True(objList.Count == 1);
            Assert.True(objList.First().nameString() == "A New Coil");
        }
    }
}
