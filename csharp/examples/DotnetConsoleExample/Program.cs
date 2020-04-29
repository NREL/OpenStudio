using System;

using OpenStudio;

namespace DotnetConsoleExample
{
    class Program
    {
        static void Main(string[] args)
        {

            try
            {
              OpenStudio.Model model = new OpenStudio.Model();

              OpenStudio.Space space1 = new OpenStudio.Space(model);
              space1.setName("First Space");
              OpenStudio.Space space2 = new OpenStudio.Space(model);
              space2.setName("Second Space");

              OpenStudio.SpaceVector spaces = model.getSpaces();
              var spaceArray = spaces.ToArray();


              Console.WriteLine("The model has " + spaceArray.Length + " Spaces:");
              for(int i=0; i < spaceArray.Length; ++i)
              {
                  Console.WriteLine("* {0}.", spaceArray[i].nameString());
              }
            }
            catch (Exception e)
            {
              Console.WriteLine("It appears that there was an error accessing the C# SWIG Bindings for OpenStudio. Note that the libraries installed in <installdir>/CSharp/openstudio need to be accessible to this application at runtime, either through the path or in the same directory as the exe. Nothing else will work properly have this point.\n\nError Text: " + e.InnerException.InnerException.Message, "Error loading libraries");
            }

        }
    }
}
