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
    }
}
