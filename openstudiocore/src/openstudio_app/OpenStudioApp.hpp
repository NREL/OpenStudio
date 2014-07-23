/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
 *  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *  
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef OPENSTUDIO_OPENSTUDIOAPP_HPP
#define OPENSTUDIO_OPENSTUDIOAPP_HPP

#include "../openstudio_lib/SOConstants.hpp"
#include "../openstudio_lib/OSAppBase.hpp"

#include "../model/Model.hpp"
#include "../model/AirTerminalSingleDuctUncontrolled.hpp"
#include "../model/AirTerminalSingleDuctVAVReheat.hpp"
#include "../model/FanConstantVolume.hpp"
#include "../model/CurveQuadratic.hpp"
#include "../model/CurveBiquadratic.hpp"
#include "../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../model/AvailabilityManagerScheduled.hpp"
#include "../model/ControllerOutdoorAir.hpp"
#include "../model/CoilHeatingGas.hpp"
#include "../model/CoilCoolingDXSingleSpeed.hpp"
#include "../model/EvaporativeCoolerDirectResearchSpecial.hpp"
#include "../model/ScheduleCompact.hpp"
#include "../model/SetpointManagerSingleZoneReheat.hpp"
#include "../model/SetpointManagerMixedAir.hpp"
#include "../model/SetpointManagerScheduled.hpp"
#include "../model/ChillerElectricEIR.hpp"
#include "../model/BoilerHotWater.hpp"
#include "../model/PumpVariableSpeed.hpp"
#include "../model/CoilCoolingWater.hpp"
#include "../model/CoilHeatingWater.hpp"
#include "../model/DistrictHeating.hpp"
#include "../model/DistrictCooling.hpp"

#include "../utilities/core/Path.hpp"

#include <vector>
#include <map>

class QEvent;

namespace openstudio {

namespace osversion {
  class VersionTranslator;
}

class SystemComponent;

class MainWindow;

class AddToModelView;

class StartupView;

class OSDocument;

class StartupMenu;

class OpenStudioApp : public OSAppBase
{

  Q_OBJECT

 public:

  OpenStudioApp( int & argc, char ** argv, const QSharedPointer<ruleset::RubyUserScriptArgumentGetter> &t_argumentGetter);

  virtual ~OpenStudioApp() {}

  virtual std::shared_ptr<OSDocument> currentDocument() const;

  static OpenStudioApp * instance();

  openstudio::model::Model componentLibrary() const;

  openstudio::model::Model hvacComponentLibrary() const;

  openstudio::path resourcesPath() const; 

  virtual bool notify(QObject* receiver, QEvent* event);

 signals:

 public slots:
  
  void quit();

  void importIdf(); 

  void importgbXML(); 

  void importSDD(); 

  void open();

  void loadLibrary();

  void newModel();

  void showHelp();

  void showAbout();

  virtual void reloadFile(const QString& fileToLoad, bool modified, bool saveCurrentTabs);

  void revertToSaved();

 private slots:

  void newFromTemplateSlot( NewFromTemplateEnum newFromTemplateEnum );

  bool openFromDrag(QString path);

  bool closeDocument();

  void onCloseClicked();

 private:

  enum fileType{
    SDD,
    GBXML
  };

  void import(fileType type);

  bool openFile(const QString& fileName, bool restoreTabs = false);

  void buildCompLibraries();

  void versionUpdateMessageBox(const osversion::VersionTranslator& translator, bool successful, const QString& fileName, 
      const openstudio::path &tempModelDir);

  void readSettings();

  void writeSettings();

  QString lastPath() const;

  void setLastPath(const QString& t_lastPath);

  QSharedPointer<ruleset::RubyUserScriptArgumentGetter> m_argumentGetter;

  openstudio::model::Model m_compLibrary;

  openstudio::model::Model m_hvacCompLibrary;

  std::shared_ptr<StartupView> m_startupView;

  std::shared_ptr<OSDocument> m_osDocument;

  std::shared_ptr<StartupMenu> m_startupMenu;

  QString m_lastPath;
};

} // openstudio

#endif // OPENSTUDIO_OPENSTUDIOAPP_HPP

