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
    void addObjects(openstudio::IddObjectType type = openstudio::IddObjectType::UserCustom);
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
