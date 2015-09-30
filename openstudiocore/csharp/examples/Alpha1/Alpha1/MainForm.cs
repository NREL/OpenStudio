using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Alpha1
{
  public partial class MainForm : Form
  {
    public MainForm()
    {
      InitializeComponent();
    }

    private void btnGo_Click(object sender, EventArgs e)
    {
      run();
    }

    // When the user clicks the 'go' button the idf file is loaded and, optionally, so is the sql file.
    // The idf file is converted to an OpenStudio model and various pieces of the OpenStudio API are demonstrated.
    private void run()
    {
      // TODO: 
      // The OpenStudio Path class overloads the / operator to concatenate paths, e.g. 'path3 = path1 / path2'.  
      // We have not yet been able to get the __div__ method to map to the / operator in C#.

      try
      {
        // The following calls will test that the OpenStudio dlls can be loaded
        OpenStudio.Path idfPath = new OpenStudio.Path(txtIdfPath.Text);
        if (!OpenStudio.OpenStudioUtilitiesCore.exists(idfPath))
        {
          MessageBox.Show("Idf file '" + txtIdfPath.Text + "' does not exist", "Error finding idf file");
          return;
        }
      }
      catch (Exception e)
      {
        MessageBox.Show("It appears that there was an error accessing the C# SWIG Bindings for OpenStudio. Note that the libraries installed in <installdir>/CSharp/OpenStudio need to be accessable to this application at runtime, either through the path or in the same directory as the exe. Nothing else will work properly have this point.\n\nError Text: " + e.InnerException.InnerException.Message, "Error loading libraries");
        return;
      }

      try
      {
        // The OpenStudio dlls have been loaded and the idf file exists 
        OpenStudio.Path idfPath = new OpenStudio.Path(txtIdfPath.Text);
        OpenStudio.Path sqlPath = new OpenStudio.Path(txtSqlPath.Text);

        // Try to read the idf file
        OpenStudio.OptionalIdfFile oIdfFile = OpenStudio.IdfFile.load(idfPath, new OpenStudio.IddFileType("EnergyPlus"));
        
        // Throw an exception if reading the idf file fails
        if (oIdfFile.isNull())
        {
          throw new Exception("Unable to load " + idfPath.__str__() + ".");
        }

        // Dereference the optional idf file to get a real idf file
        OpenStudio.IdfFile idfFile = oIdfFile.get();

        // Translate the idf file to an OpenStudio model
        OpenStudio.EnergyPlusReverseTranslator translator = new OpenStudio.EnergyPlusReverseTranslator();
        OpenStudio.Model model = translator.translateWorkspace(new OpenStudio.Workspace(idfFile));

        // If the sql file path is valid, try to load the EnergyPlus sql file 
        OpenStudio.OptionalSqlFile oSqlFile = new OpenStudio.OptionalSqlFile();
        if (OpenStudio.OpenStudioUtilitiesCore.exists(sqlPath))
        {
          try
          {
            oSqlFile = new OpenStudio.OptionalSqlFile(new OpenStudio.SqlFile(sqlPath));
            if (!oSqlFile.isNull())
            {
              // associate results in this sqlFile with this model.
              if (!model.setSqlFile(oSqlFile.get()))
              {
                // clear the sql file if couldn't associate with this model
                oSqlFile.reset();
              }
            }
          }
          catch (Exception )
          {
            // clear the sql file if couldn't associate with this model
            oSqlFile.reset();
          }
        }

        // Get the building object
        OpenStudio.Building building = model.getBuilding();

        // Loop over the spaces in the building
        double grossArea = 0;
        foreach (OpenStudio.Space space in building.spaces())
        {
          string name = space.name().get();

          // Get floor area from space calculation
          // Note that floorArea is exported as a read only property as a proof of concept
          double floorArea = space.floorArea;
          grossArea += floorArea;

          // Other methods of the space object have not yet been converted to properties
          // These must be called as methods
          double lightingPower = space.lightingPower();

          // The method space.lights() is exported as a method
          foreach (OpenStudio.Lights light in space.lights())
          {
          }

          // The method space.surfaces() is exported as a read only property
          foreach (OpenStudio.Surface surface in space.surfaces)
          {
          }

          // The method space.spaceType() is exported as a read only property
          OpenStudio.OptionalSpaceType oSpaceType = space.spaceType;
        }

        // get floor area from building calculation
        double buildingFloorArea = building.floorArea();

        // calculated floor areas should be same
        if (Math.Abs(buildingFloorArea - grossArea) > 0.01)
        {
          MessageBox.Show("grossArea = " + grossArea.ToString() + " and buildingFloorArea = " + buildingFloorArea.ToString());
        }

        if (!oSqlFile.isNull())
        {
          // check building area, will be less because plenum is not included
          string query = "SELECT Value FROM tabulardatawithstrings  WHERE ReportName='AnnualBuildingUtilityPerformanceSummary' AND ReportForString='Entire Facility' AND TableName='Building Area' AND RowName='Total Building Area' AND ColumnName='Area' AND Units='m2'";
          double sqlBuildingFloorArea = model.sqlFile().get().execAndReturnFirstDouble(query).get();

          // calculated floor area should be same as that in sqlite database
          if (Math.Abs(buildingFloorArea - sqlBuildingFloorArea) > 0.01)
          {
            MessageBox.Show("sqlBuildingFloorArea = " + sqlBuildingFloorArea.ToString() + " and buildingFloorArea = " + buildingFloorArea.ToString());
          }
        }

        // Note that the method getLightss is an automatically generated method name (which is why it looks like a typo)
        // The pattern for getting all objects of type Foo from the model is "model.getFoos".  
        // If Bar is a unique object type then the method is "model.getFoo".
        foreach (OpenStudio.Lights light in model.getLightss())
        {
          string name = light.name().get();

          // Get the space this light belongs to
          OpenStudio.OptionalSpace oSpace = light.space();
          if (oSpace.isNull())
          {
            MessageBox.Show("Lights '" + name + "' does not have an associated space.");
            continue;
          }
          OpenStudio.Space space = oSpace.get();

          // get lighting power from calculation
          double lightingPower = light.getLightingPower(space.floorArea, space.numberOfPeople());

          if (!oSqlFile.isNull())
          {
            string query = "SELECT DesignLevel FROM nominallighting WHERE ObjectName='" + name.ToUpper() + "'";
            double sqlLightingPower = model.sqlFile().get().execAndReturnFirstDouble(query).get();
            // lighting power should be same as that in sqlite database
            if (Math.Abs(sqlLightingPower - lightingPower) > 0.000001)
            {
              MessageBox.Show("sqlLightingPower = " + sqlLightingPower + " and lightingPower = " + lightingPower);
            }
          }
        }

        MessageBox.Show("C# Alpha1 test completed successfully!");
      }
      catch (Exception e)
      {
        MessageBox.Show("Error while running C# Alpha1 test: " + e.Message);
      }
    }

    private void btnIdfBrowse_Click(object sender, EventArgs e)
    {
      FolderBrowserDialog fbd = new FolderBrowserDialog();

      OpenFileDialog ofd = new OpenFileDialog();
      ofd.InitialDirectory = "c:\\";
      ofd.Filter = "idf files (*.idf)|*.idf|All files (*.*)|*.*";
      ofd.FilterIndex = 1;
      ofd.RestoreDirectory = true;

      if (ofd.ShowDialog() == DialogResult.OK)
      {
        txtIdfPath.Text = ofd.FileName;
      }
    }

    private void btnSqlBrowse_Click(object sender, EventArgs e)
    {

      OpenFileDialog ofd = new OpenFileDialog();
      ofd.InitialDirectory = "c:\\";
      ofd.Filter = "sql files (*.sql)|*.sql|All files (*.*)|*.*";
      ofd.FilterIndex = 1;
      ofd.RestoreDirectory = true;

      if (ofd.ShowDialog() == DialogResult.OK)
      {
        txtSqlPath.Text = ofd.FileName;
      }
    }

    private void MainForm_Load(object sender, EventArgs e)
    {

    }
  }
}
