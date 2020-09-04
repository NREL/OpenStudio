function Component()
{
  Component.prototype.createOperations = function()
  {
    // call default implementation
    component.createOperations();

    // ... add custom operations

    var kernel = systemInfo.kernelType;
    if( kernel == "darwin" ) {
      //var exePath = installer.value("TargetDir") + "/bin/openstudio";
      //component.addElevatedOperation("Delete", "/usr/local/bin/openstudio");
      //component.addElevatedOperation("Mkdir", "/usr/local/bin/");
      //component.addElevatedOperation("CreateLink", "/usr/local/bin/openstudio", exePath );
    }
  }
}



