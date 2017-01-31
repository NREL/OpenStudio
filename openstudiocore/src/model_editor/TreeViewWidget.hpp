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

#ifndef MODELEDITOR_TREEVIEWWIDGET_HPP
#define MODELEDITOR_TREEVIEWWIDGET_HPP

#include <QModelIndex>

#include "../model/Building.hpp"
#include "ModelEditorAPI.hpp"
#include "ViewWidget.hpp"

namespace modeleditor
{
  
class TreeModel;
class TreeView;

class TreeViewWidget : public ViewWidget
{
  Q_OBJECT

public:
  TreeViewWidget(openstudio::model::Model& model, QWidget *parent = nullptr);
  TreeViewWidget(QWidget *parent = nullptr);
  virtual ~TreeViewWidget();
  virtual void addObjects(openstudio::IddObjectType type = openstudio::IddObjectType("UserCustom")) override;
  virtual void loadModel() override;
  virtual void removeObjects() override;
  virtual void copyObjects() override;
  virtual void pasteObjects() override;
  virtual bool hasSelectedRows() override;
  virtual bool hasRowsToPaste() override;
  std::vector<openstudio::IddObjectType> getAllowableChildTypes();
  void expandAllNodes();
  void collapseAllNodes();
  TreeView* getTreeView();
  TreeModel* getTreeModel();
  virtual void toggleGUIDs() override;

public slots:
  virtual void viewSelection();
  virtual void viewSelection(const QModelIndex& modelIndex) override;
  virtual void on_nameChanged(QString) override;

signals:
  void expandAll();
  void collapseAll();

protected:
  TreeView * mTreeView;
  TreeModel * mTreeModel;

private:
  virtual void connectSignalsAndSlots() override;
  virtual void createLayout() override;
  virtual void createWidgets() override;
  virtual void loadData() override;
  virtual void loadModel(openstudio::model::Model& model) override;
  void expandAppropriateTreeNodes();
  void setExpandedIndexHandles();

  ///! only expanded nodes are here
  std::vector< std::pair<QModelIndex, openstudio::Handle> > mExpandedIndexHandles;

  std::vector<openstudio::model::ModelObject> mModelObjectsToPaste;
};

} // namespace modeleditor

#endif // MODELEDITOR_TREEVIEWWIDGET_HPP
