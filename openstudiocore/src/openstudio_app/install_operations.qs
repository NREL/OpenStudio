function Component()
{
  Component.prototype.createOperations = function()
  {
    // call default implementation
    component.createOperations();

    // ... add custom operations

    var kernel = systemInfo.kernelType;
    if( kernel == "darwin" ) {
      var appPath = installer.value("TargetDir") + "/OpenStudioApp.app/Contents/";

      var epPath = installer.value("TargetDir") + "/EnergyPlus";
      var mkPath = appPath + "EnergyPlus";
      component.addElevatedOperation("Mkdir", mkPath);
      component.addElevatedOperation("CopyDirectory", epPath, appPath);

      var radPath = installer.value("TargetDir") + "/Radiance";
      var mkRadPath = appPath + "Radiance";
      component.addElevatedOperation("Mkdir", mkRadPath);
      component.addElevatedOperation("CopyDirectory", radPath, appPath);
    }

    if( kernel == "winnt" ) {
      component.addOperation("CreateShortcut", "@TargetDir@/bin/OpenStudioApp.exe", "@StartMenuDir@/OpenStudio.lnk");
    }
  }
}



