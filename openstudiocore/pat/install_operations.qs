function Component()
{
  Component.prototype.createOperations = function()
  {
    // call default implementation
    component.createOperations();

    // ... add custom operations

    var kernel = systemInfo.kernelType;

    if( kernel == "winnt" ) {
      component.addOperation("CreateShortcut", "@TargetDir@/pat/ParametricAnalysisTool.exe", "@StartMenuDir@/Parametric Analysis Tool.lnk");
    }

    if( kernel == "darwin" ) {
      var exePath = installer.value("TargetDir") + "/bin/install_utility";
      component.addElevatedOperation("Execute", "chmod", "-R", "a+w", "@TargetDir@/ParametricAnalysisTool.app/Contents/Resources/OpenStudio-server");
    }
  }
}
