using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace RunManagerExample
{
    public partial class MainForm : Form
    {
        private OpenStudio.RunManager m_runmanager;

        public MainForm()
        {
            try
            {
                m_runmanager = new OpenStudio.RunManager(new OpenStudio.Path("runmanager.db"));
            }
            catch (Exception e)
            {
              MessageBox.Show("It appears that there was an error accessing the C# SWIG Bindings for OpenStudio. Note that the libraries installed in <installdir>/CSharp/zerokit need to be accessable to this application at runtime, either through the path or in the same directory as the exe. Nothing else will work properly have this point.\n\nError Text: " + e.InnerException.InnerException.Message, "Error loading libraries");
            }
           
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {

        }

        private void btnShowConfig_Click(object sender, EventArgs e)
        {
            m_runmanager.showConfigGui();
        }

    }
}
