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

#ifndef MODELEDITOR_MODALDIALOGS_HPP
#define MODELEDITOR_MODALDIALOGS_HPP

#include "ModelEditorAPI.hpp"

#include "../model/Model.hpp"

#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/UUID.hpp"

#include <QDialog>

class QLabel;
class QComboBox;
class QPushButton;

namespace openstudio{

  class WorkspaceObject;

  namespace model{

    class ModelObject;
    class Space;
    class SpaceLoadInstance;

  }
}

class MODELEDITOR_API ModelObjectSelectorDialog : public QDialog
{
  Q_OBJECT

public:

  ModelObjectSelectorDialog(const openstudio::IddObjectType& typeToDisplay,
                            const openstudio::model::Model& model,
                            QWidget * parent = nullptr);

  ModelObjectSelectorDialog(const std::vector<openstudio::IddObjectType>& typesToDisplay,
                            const openstudio::model::Model& model,
                            QWidget * parent = nullptr);

  virtual ~ModelObjectSelectorDialog();

  boost::optional<openstudio::model::ModelObject> selectedModelObject() const;

  void setWindowTitle(const std::string& text);

  void setUserText(const std::string& text);

  void setOKButtonText(const std::string& text);

  void setCancelButtonText(const std::string& text);

signals:

  // emitted when the dialog is closed
  void closed(const boost::optional<openstudio::model::ModelObject>&);

private slots:

  void onPushButtonOK(bool);
  void onPushButtonCancel(bool);
  void onAddWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl);
  void onRemoveWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl);

private:

  // for testing
  friend class ModelEditorFixture;

  QLabel* m_userTextLabel;
  QComboBox* m_comboBox;
  QPushButton* m_okButton;
  QPushButton* m_cancelButton;

  std::vector<openstudio::IddObjectType> m_typesToDisplay;
  openstudio::model::Model m_model;

  void init();
  void createWidgets();
  void connectSignalsAndSlots();
  void loadStyleSheet();
  void loadComboBoxData();
};

class MODELEDITOR_API ModelObjectSelectorDialogWatcher : public QObject
{
  Q_OBJECT
  
public:

  ModelObjectSelectorDialogWatcher(std::shared_ptr<ModelObjectSelectorDialog> modelObjectSelectorDialog);
  
  /// get the selected object
  boost::optional<openstudio::model::ModelObject> selectedModelObject() const;

  /// true if has the model object selector dialog closed
  bool isSelectionFinal() const;

private slots:

  virtual void onClose(const boost::optional<openstudio::model::ModelObject>&);

private:

  std::shared_ptr<ModelObjectSelectorDialog> m_modelObjectSelectorDialog;
  mutable boost::optional<openstudio::model::ModelObject> m_selectedModelObject;
};

MODELEDITOR_API void ensureThermalZone(openstudio::model::Space& space);

MODELEDITOR_API void ensureSpaceLoadDefinition(openstudio::model::SpaceLoadInstance& instance);

#endif //MODELEDITOR_MODALDIALOGS_HPP
