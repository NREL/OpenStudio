function Component()
{
  Component.prototype.createOperations = function()
  {
    // call default implementation
    component.createOperations();

    // ... add custom operations

    var kernel = systemInfo.kernelType;
    if( kernel == "winnt" ) {
      var exePath = installer.value("TargetDir") + "\\bin\\install_utility.exe";
      component.addElevatedOperation("Execute", exePath, "Install", "UNDOEXECUTE", exePath, "Remove" );
    }

    if( kernel == "darwin" ) {
      var exePath = installer.value("TargetDir") + "/bin/install_utility";
      component.addElevatedOperation("Execute", exePath, "Install", "UNDOEXECUTE", exePath, "Remove" );
    }
  }
}



