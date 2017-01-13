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

#ifndef MODELEDITOR_MODELEXPLORER_HPP
#define MODELEDITOR_MODELEXPLORER_HPP

#include <QString>
#include <QWidget>

#include "../model/Model.hpp"
#include "../model/Building.hpp"
#include "ModelEditorAPI.hpp"
#include "../utilities/idf/IdfFile.hpp"

class QDialog;
class QLabel;
class QProgressBar;
class QSplitter;
class QStackedWidget;
class QToolBar;

class InspectorGadget;

namespace openstudio{
  class ProgressBar;
}

namespace modeleditor
{

  class ClassViewWidget;
  class ObjectExplorer;
  class TableModel;
  class TreeModel;
  class TreeViewWidget;
  class ViewWidget;

  class ModelExplorer : public QWidget
  {
    Q_OBJECT

    public:
    ModelExplorer(QWidget * parent = nullptr);
    virtual ~ModelExplorer();
    void loadFile(const QString &fileName);
    void addObjects(openstudio::IddObjectType type = openstudio::IddObjectType("UserCustom"));
    void removeObjects();
    void copyObjects();
    void pasteObjects();
    bool classViewUnderMouse();
    bool treeViewUnderMouse();
    bool classViewCurrentWidget();
    bool treeViewCurrentWidget();
    bool hasSelectedRows();
    bool treeViewHasRowsToPaste();
    bool classViewHasRowsToPaste();
    std::vector<openstudio::IddObjectType> getAllowableChildTypes();
    openstudio::model::Model& getModel();
    const openstudio::IddFile& getIddFile();
    void setModel(openstudio::model::Model& model);
    void setIddFile(openstudio::model::Model& model);
    void loadModel();
    int currentIndex() const;
    void expandAllNodes();
    TableModel* getTableModel();
    TreeModel* getTreeModel();
    void showComments(const bool showComments);
    void showPrecision(const bool showPrecision);

  public slots:
    void classAction();
    void systemOutlinerAction();
    void treeAction();
    void setCurrentIndex(int index);
    void on_ClassViewEventEnter();
    void on_ClassViewEventLeave();
    void on_TreeViewEventEnter();
    void on_TreeViewEventLeave();
    void toggleGUIDs();
    void showAllFields(bool);
    void createAllFields();
    void setRecursive(bool);

  signals:
    void modelDirty();
    void showStatusBarMsg(const QString& msg, const int millisecondDuration);
    void precisionDlgHidden();
    void precisionDlgFinished();

  protected:

  private:
    void createProgressDlg();
    void createWidgets();
    void createActions();
    void createToolBars();
    void createLayout();
    void connectSignalsAndSlots();
    void viewWidgetAction(modeleditor::ViewWidget * viewWidget);
    void saveState();
    void restoreState();

    ClassViewWidget * mClassViewWidget;
    TreeViewWidget * mTreeViewWidget;
    ObjectExplorer * mObjectExplorer;

    bool mClassViewUnderMouse;
    bool mTreeViewUnderMouse;

    QLabel * mProgressBarLbl;
    QSplitter * mSplitter;
    QDialog * mDlg;
    openstudio::ProgressBar * mProgressBar;
    QProgressBar * mQProgressBar;
    QAction * mClassAction;
    QAction * mSystemOutlinerAction;
    QAction * mTreeAction;
    QStackedWidget * mStackedWidget;
    QToolBar * mToolBar;
    openstudio::model::Model mModel;
    openstudio::IddFile mIddFile;
  };

} // namespace modeleditor

#endif // MODELEDITOR_MODELEXPLORER_HPP
