function Component()
{
  Component.prototype.createOperations = function()
  {
    // call default implementation
    component.createOperations();

    // ... add custom operations

    var pluginsDir = "";

    var kernel = systemInfo.kernelType;
    if( kernel == "darwin" ) {
      pluginsDir = "/Library/Application Support/SketchUp 2016/SketchUp/Plugins/";
    } else {
      pluginsDir = "C:/ProgramData\SketchUp/SketchUp 2016/SketchUp/Plugins/";
    }

    var pluginFile = pluginsDir + "OpenStudio.rb";

    component.addElevatedOperation("Delete", pluginFile);
    component.addElevatedOperation("Mkdir", pluginsDir);
    var requireText = "require '" + installer.value("TargetDir") + "/SketchUpPlugin/plugin/OpenStudio.rb'"
    component.addElevatedOperation("AppendFile", pluginFile, requireText);
  }
}



