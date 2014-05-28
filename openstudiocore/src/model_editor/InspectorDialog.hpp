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

#ifndef MODELEDITOR_INSPECTORDIALOG_HPP
#define MODELEDITOR_INSPECTORDIALOG_HPP

#include "ModelEditorAPI.hpp"

#include "../model/Model.hpp"

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/idd/IddFile.hpp"
#include "../utilities/core/UUID.hpp"
#include "../utilities/core/Enum.hpp"

#include <QWidget>
#include <QMainWindow>

class QLabel;
class QListWidget;
class QStackedWidget;
class QTableWidget;
class QPushButton;
class QShowEvent;
class QCloseEvent;
class InspectorGadget;

namespace openstudio{
  class WorkspaceObject;

  namespace model{
    class ModelObject;
  }
}

#ifndef Q_MOC_RUN
OPENSTUDIO_ENUM( InspectorDialogClient,
  ((AllOpenStudio))
  ((SketchUpPlugin))
);
#endif


class MODELEDITOR_API InspectorDialog : public QMainWindow
{
  Q_OBJECT

public:

  /// create a new dialog with empty model
  InspectorDialog(InspectorDialogClient client = InspectorDialogClient::AllOpenStudio, QWidget * parent = nullptr);

  /// create a new dialog for existing model
  InspectorDialog(openstudio::model::Model& model,
                  InspectorDialogClient client = InspectorDialogClient::AllOpenStudio,
                  QWidget * parent = nullptr);

  virtual ~InspectorDialog();

  // get the idd object type
  openstudio::IddObjectType iddObjectType() const;

  // set the idd object type, must be an allowable type
  bool setIddObjectType(const openstudio::IddObjectType&, bool force=false);

  // get handles of the selected objects
  std::vector<openstudio::Handle> selectedObjectHandles() const;

  // set the selected object handles, all handles must be found in current model and be of the 
  // same allowable idd object type
  bool setSelectedObjectHandles(const std::vector<openstudio::Handle>&, bool force=false);

  // get the current model
  openstudio::model::Model model() const;

  // point the dialog at a new model
  void setModel(openstudio::model::Model& model, bool force=false);

  // void rebuild inspector gadget
  void rebuildInspectorGadget(bool recursive);

  // save the state
  void saveState();

  // restore the state
  void restoreState();

  void displayIP(const bool displayIP);

public slots:

  virtual void onIddObjectTypeChanged(const openstudio::IddObjectType&);

  virtual void onSelectedObjectHandlesChanged(const std::vector<openstudio::Handle>&);

  virtual void onModelChanged(openstudio::model::Model&);

signals:

  // emitted when user selects a new idd object type, will be an allowable type
  void iddObjectTypeChanged(const openstudio::IddObjectType&);

  // emitted when selected objects change, all handles will be in model and of the same allowable type
  void selectedObjectHandlesChanged(const std::vector<openstudio::Handle>&);
  
  // emitted when inspected model changes
  void modelChanged(openstudio::model::Model&);

  void toggleUnitsClicked(bool);
 
protected:

  // handle show event
  virtual void showEvent(QShowEvent *t_event);

  // handle close event
  virtual void closeEvent(QCloseEvent *t_event);

private slots:

  // for testing
  friend class ModelEditorFixture;

  void onPushButtonNew(bool);
  void onPushButtonCopy(bool);
  void onPushButtonDelete(bool);
  void onPushButtonPurge(bool);
  //void onCheckBox(bool checked);
  void onListWidgetSelectionChanged();
  void onTableWidgetSelectionChanged();
  void onAddWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl);
  void onWorkspaceChange();
  void onTimeout();
  void onRemoveWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl);

private:

  QListWidget* m_listWidget;
  QStackedWidget* m_stackedWidget;
  QLabel* m_selectionLabel;
  QTableWidget* m_tableWidget;
  QPushButton* m_pushButtonNew;
  QPushButton* m_pushButtonCopy;
  QPushButton* m_pushButtonDelete;
  QPushButton* m_pushButtonPurge;
  InspectorGadget * m_inspectorGadget;

  openstudio::IddFile m_iddFile;
  std::set<openstudio::IddObjectType> m_typesToDisplay;
  std::set<openstudio::IddObjectType> m_disableSelectionTypes;
  std::set<openstudio::IddObjectType> m_disableAddTypes;
  std::set<openstudio::IddObjectType> m_disableCopyTypes;
  std::set<openstudio::IddObjectType> m_disableRemoveTypes;
  std::set<openstudio::IddObjectType> m_disablePurgeTypes;

  openstudio::IddObjectType m_iddObjectType;
  std::vector<openstudio::Handle> m_objectHandles;
  std::vector<openstudio::Handle> m_selectedObjectHandles;
  openstudio::model::Model m_model;
  bool m_workspaceChanged;
  bool m_workspaceObjectAdded;
  bool m_workspaceObjectRemoved;
 
  void init(InspectorDialogClient client);
  void createWidgets();
  void connectSelfSignalsAndSlots();
  void connectModelSignalsAndSlots();
  void hideSelectionWidget(bool hide);
  void loadStyleSheet();
  void loadListWidgetData();
  void updateListWidgetData();
  void loadTableWidgetData();
  void setTableWidgetHeader();
  void getTableWidgetSelected(std::vector<openstudio::Handle>& selectedHandles);
  void loadModel();
};

#endif // MODELEDITOR_INSPECTORDIALOG_HPP
