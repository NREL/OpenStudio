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

#ifndef MODELEDITOR_MODALDIALOGS_HPP
#define MODELEDITOR_MODALDIALOGS_HPP

#include "ModelEditorAPI.hpp"

#include "../model/Model.hpp"
#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement

#include "../utilities/idd/IddEnums.hpp"
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

class MODELEDITOR_API ModelObjectSelectorDialog : public QDialog, public Nano::Observer
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

  void onPushButtonOK(bool);

  void onPushButtonCancel(bool);

signals:

  // emitted when the dialog is closed
  void closed(const boost::optional<openstudio::model::ModelObject>&);

private slots:

  void onAddWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl, const openstudio::IddObjectType& type, const openstudio::UUID& uuid);
  void onRemoveWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl, const openstudio::IddObjectType& type, const openstudio::UUID& uuid);

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

class MODELEDITOR_API ModelObjectSelectorDialogWatcher : public QObject, public Nano::Observer
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
