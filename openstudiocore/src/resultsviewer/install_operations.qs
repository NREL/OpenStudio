function Component()
{
  Component.prototype.createOperations = function()
  {
    // call default implementation
    component.createOperations();

    // ... add custom operations

    var kernel = systemInfo.kernelType;
    if( kernel == "winnt" ) {
      component.addOperation("CreateShortcut", "@TargetDir@/bin/ResultsViewer.exe", "@StartMenuDir@/ResultsViewer.lnk");
    }
  }
}



