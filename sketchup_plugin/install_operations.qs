function Component()
{
  Component.prototype.createOperations = function()
  {
    // call default implementation
    component.createOperations();

    // ... add custom operations

    var targetDir = installer.value("TargetDir");
    var sourceDir = targetDir + "/SketchUpPlugin/plugin/";
    
    // write configured file
    var file = "OpenStudio/OpenStudio-config" // no file extension
    component.addElevatedOperation("Delete", sourceDir + file);
    
    var configText = "$OPENSTUDIO_SKETCHUPPLUGIN_VERSION = \"" + installer.value("ProductVersion") + "\"\n" + 
                     "$OPENSTUDIO_APPLICATION_DIR = '" + targetDir + "/bin/'\n" + 
                     "$:.unshift('" + targetDir + "/Ruby/')\n" + 
                     "$:.unshift('" + targetDir + "/SketchUpPlugin/')"
    component.addElevatedOperation("AppendFile", sourceDir + file, configText); 
    
    // set SketchUp plugin directory
    var pluginsDir = "";
    var kernel = systemInfo.kernelType;
    if( kernel == "darwin" ) {
      pluginsDir = "/Library/Application Support/SketchUp 2017/SketchUp/Plugins/";
    } else {
      pluginsDir = "C:/ProgramData/SketchUp/SketchUp 2017/SketchUp/Plugins/";
    }
    
    // make dir
    component.addElevatedOperation("Mkdir", pluginsDir);
    component.addElevatedOperation("Mkdir", pluginsDir + "OpenStudio/");

    // copy files
    file = "OpenStudio.rb";
    component.addElevatedOperation("Delete", pluginsDir + file);
    component.addElevatedOperation("Copy", sourceDir + file, pluginsDir + file);
    
    file = "OpenStudio/Startup.rb";
    component.addElevatedOperation("Delete", pluginsDir + file);
    component.addElevatedOperation("Copy", sourceDir + file, pluginsDir + file);
    
    file = "OpenStudio/OpenStudio.hash";
    component.addElevatedOperation("Delete", pluginsDir + file);
    component.addElevatedOperation("Copy", sourceDir + file, pluginsDir + file);
    
    file = "OpenStudio/OpenStudio-config";
    component.addElevatedOperation("Delete", pluginsDir + file);
    component.addElevatedOperation("Copy", sourceDir + file, pluginsDir + file);
  
  }
}



