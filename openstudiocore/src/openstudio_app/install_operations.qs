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
      var linktarget = appPath + "/Frameworks/QtWebEngineCore.framework/Versions/5/Helpers/QtWebEngineProcess.app/Contents/Frameworks";
      var linksource = "../../../../../../../Frameworks";
      component.addElevatedOperation("Execute", "ln", "-s", linksource, linktarget );
    }

    if( kernel == "winnt" ) {
      component.addElevatedOperation("CreateShortcut", "@TargetDir@/bin/OpenStudioApp.exe", "@StartMenuDir@/OpenStudio.lnk");
      component.addElevatedOperation("RegisterFileType", "osm", "@TargetDir@/bin/OpenStudioApp.exe %1", "OpenStudio Model File", "text/plain", "@TargetDir@/bin/OpenStudioApp.exe,1");
    }
  }
}



