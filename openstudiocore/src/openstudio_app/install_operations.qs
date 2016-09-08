function Component()
{
  Component.prototype.createOperations = function()
  {
    // call default implementation
    component.createOperations();

    // ... add custom operations

    var kernel = systemInfo.kernelType;
    if( kernel == "darwin" ) {
      var epPath = installer.value("TargetDir") + "/EnergyPlus";
      var appPath = installer.value("TargetDir") + "/OpenStudioApp.app/Contents/";
      var mkPath = appPath + "EnergyPLus";
      component.addElevatedOperation("Mkdir", mkPath);
      component.addElevatedOperation("CopyDirectory", epPath, appPath);
    }
  }
}



