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
            // Console.WriteLine("\nCreate an empty model and add 2 Spaces");

            OpenStudio.Model model = new OpenStudio.Model();

            OpenStudio.Space space1 = new OpenStudio.Space(model);
            space1.setName("First Space");
            OpenStudio.Space space2 = new OpenStudio.Space(model);
            space2.setName("Second Space");

            OpenStudio.SpaceVector spaces = model.getSpaces();
            var spaceArray = spaces.ToArray();

            Assert.Equal(2, spaceArray.Length);

            Assert.True(Array.Exists(spaceArray, s => (s.nameString() == "First Space")));
            Assert.True(Array.Exists(spaceArray, s => (s.nameString() == "Second Space")));

        }

        [Fact]
        public void BasicFT_And_RT()
        {
            // Console.WriteLine("\nCreate an empty model and add 2 Spaces");
            OpenStudio.Model model = new OpenStudio.Model();
            OpenStudio.EnergyPlusForwardTranslator ft = new OpenStudio.EnergyPlusForwardTranslator();
            var workspace = ft.translateModel(model);
            Assert.True(workspace.objects().ToArray().Length > 5);

            OpenStudio.EnergyPlusReverseTranslator rt = new OpenStudio.EnergyPlusReverseTranslator();
            var model2 = rt.translateWorkspace(workspace);
            Assert.True(model2.objects().ToArray().Length > 5);
        }
    }
}
