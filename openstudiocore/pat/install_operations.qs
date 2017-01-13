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
  }
}
