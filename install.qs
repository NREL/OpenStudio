function Controller () {
  // silent install is not an option until QtIFW v3.0.1
  // gui.setSilent(true);
}

Controller.prototype.IntroductionPageCallback = function () {
  gui.clickButton(buttons.NextButton);
}

Controller.prototype.ComponentSelectionPageCallback = function () {
  var widget = gui.currentPageWidget();
  
  // to install a subset of components, uncomment the code below and edit as neccesary
  //widget.deselectAll();
  //widget.selectComponent('SketchUpPlugin');
  //widget.selectComponent('CLI');
  //widget.selectComponent('RubyAPI');
  //widget.selectComponent('CSharpAPI');
  //widget.selectComponent('PAT');
  //widget.selectComponent('OpenStudioApp');
  //widget.selectComponent('DView');
  //widget.selectComponent('EnergyPlus');
  //widget.selectComponent('Radiance');
  //widget.selectComponent('Resources');
  
  gui.clickButton(buttons.NextButton);
}

Controller.prototype.TargetDirectoryPageCallback = function () {
  gui.clickButton(buttons.NextButton);
}

Controller.prototype.StartMenuDirectoryPageCallback = function () {
  gui.clickButton(buttons.NextButton);
}

Controller.prototype.ReadyForInstallationPageCallback = function () {
  gui.clickButton(buttons.NextButton);
}

Controller.prototype.FinishedPageCallback = function () {
  gui.clickButton(buttons.FinishButton);
}

