function Component()
{
  Component.prototype.createOperations = function()
  {
    // call default implementation
    component.createOperations();

    // ... add custom operations

    var kernel = systemInfo.kernelType;
    if( kernel == "darwin" ) {
      var exePath = installer.value("TargetDir") + "/bin/install_utility";
      component.addElevatedOperation("Execute", exePath, "InstallApp", "UNDOEXECUTE", exePath, "RemoveApp" );
    }

    if( kernel == "winnt" ) {
      component.addOperation("CreateShortcut", "@TargetDir@/bin/OpenStudioApp.exe", "@StartMenuDir@/OpenStudio.lnk");
    }
  }
}



