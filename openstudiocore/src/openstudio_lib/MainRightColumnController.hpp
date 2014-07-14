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

#ifndef OPENSTUDIO_MAINRIGHTCOLUMNCONTROLLER_HPP
#define OPENSTUDIO_MAINRIGHTCOLUMNCONTROLLER_HPP

#include "../shared_gui_components/OSQObjectController.hpp"
#include <boost/smart_ptr.hpp>
#include "../model/Model.hpp"
#include "../shared_gui_components/EditController.hpp"
#include "../shared_gui_components/LocalLibraryController.hpp"

#include <QSharedPointer>

class QStackedWidget;

namespace openstudio {

class HorizontalTabWidget;
class InspectorController;
class LocalLibraryController;
class EditController;
class SystemItem;

class MainRightColumnController : public OSQObjectController
{
  Q_OBJECT

  public:

  MainRightColumnController(const model::Model & model, const openstudio::path &resourcesPath);

  virtual ~MainRightColumnController() {};

  HorizontalTabWidget * mainRightColumnView() const;

  QSharedPointer<LocalLibraryController> measureLibraryController();

  void hideMyModelTab(bool hide);

  bool isMyModelTabHidden(){return m_myModelTabIsHidden;}

  enum RightColumnTabID
  {
    MY_MODEL,
    LIBRARY,
    EDIT
  };

  // Methods to store and retrieve SystemItem objects.
  // These are used to get plenum colors and potentially other tasks.
  void registerSystemItem(const Handle & systemHandle, SystemItem * systemItem);
  void unregisterSystemItem(const Handle & systemHandle);
  SystemItem * systemItem(const Handle & systemHandle) const;

  signals:
  
  void toggleUnitsClicked(bool displayIP);

  public slots:

  void configureForSiteSubTab(int subTabID);

  void configureForSchedulesSubTab(int subTabID);

  void configureForConstructionsSubTab(int subTabID);

  void configureForLoadsSubTab(int subTabID);

  void configureForSpaceTypesSubTab(int subTabID);

  void configureForBuildingStoriesSubTab(int subTabID);

  void configureForFacilitySubTab(int subTabID);

  void configureForThermalZonesSubTab(int subTabID);

  void configureForHVACSystemsSubTab(int subTabID);

  void configureForBuildingSummarySubTab(int subTabID);

  void configureForOutputVariablesSubTab(int subTabID);

  void configureForSimulationSettingsSubTab(int subTabID);

  void configureForScriptsSubTab(int subTabID);

  void configureForRunSimulationSubTab(int subTabID);

  void configureForResultsSummarySubTab(int subTabID);

  void inspectModelObject(model::OptionalModelObject & modelObject, bool readOnly);

  void toggleUnits(bool displayIP);

  void chooseEditTab();

  QSharedPointer<LocalLibraryController> measuresLibraryController();

  QSharedPointer<EditController> measuresEditController();

  private:
  REGISTER_LOGGER("openstudio.openstudio_lib.MainRightColumnController");

  void setMyModelView(QWidget * widget);

  void setLibraryView(QWidget * widget);

  void setEditView(QWidget * widget);

  HorizontalTabWidget * m_horizontalTabWidget; 

  std::shared_ptr<InspectorController> m_inspectorController;

  QStackedWidget * m_myModelView;

  QStackedWidget * m_libraryView;

  QStackedWidget * m_editView;

  std::map<Handle,SystemItem *> m_systemItemMap;

  model::Model m_model;

  openstudio::path m_resourcesPath;

  QSharedPointer<LocalLibraryController> m_measureLibraryController;

  QSharedPointer<EditController> m_measureEditController;

  bool m_myModelTabIsHidden;
};

} // openstudio

#endif // OPENSTUDIO_MAINRIGHTCOLUMNCONTROLLER_HPP

