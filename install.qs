function Controller () {
  // gui.setSilent(true);
}

Controller.prototype.IntroductionPageCallback = function () {
  gui.clickButton(buttons.NextButton);
}

Controller.prototype.ComponentSelectionPageCallback = function () {
  // var widget = gui.currentPageWidget();
  // widget.deselectAll();
  // widget.selectComponent('RubyAPI');
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
