/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

class EditController;
class HorizontalTabWidget;
class InspectorController;
class LocalLibraryController;
class SystemItem;
class OSItem;

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

  std::shared_ptr<InspectorController> inspectorController() { return m_inspectorController; };

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

  void itemRemoveClicked(OSItem *);

  public slots:

  void configureForSiteSubTab(int subTabID);

  void configureForSchedulesSubTab(int subTabID);

  void configureForConstructionsSubTab(int subTabID);

  void configureForGeometrySubTab(int subTabID);

  void configureForLoadsSubTab(int subTabID);

  void configureForRenewableEnergySubTab(int subTabID);

  void configureForSpaceTypesSubTab(int subTabID);

  void configureForFacilitySubTab(int subTabID);

  void configureForSpacesSubTab(int subTabID);

  void configureForThermalZonesSubTab(int subTabID);

  void configureForHVACSystemsSubTab(int subTabID);

  void configureForBuildingSummarySubTab(int subTabID);

  void configureForOutputVariablesSubTab(int subTabID);

  void configureForSimulationSettingsSubTab(int subTabID);

  void configureForScriptsSubTab(int subTabID);

  void configureForRunSimulationSubTab(int subTabID);

  void configureForResultsSummarySubTab(int subTabID);

  void inspectModelObject(model::OptionalModelObject & modelObject, bool readOnly);

  void inspectModelObjectByItem(OSItem * item, bool readOnly);

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

  OSItem * m_item = nullptr;

  private slots:

  void onItemRemoveClicked(OSItem *);

  void emitItemRemoveClicked(OSItem *);

  void onRemoveButtonClicked(bool checked);

  void onWorkspaceObjectRemoved();
};

} // openstudio

#endif // OPENSTUDIO_MAINRIGHTCOLUMNCONTROLLER_HPP

