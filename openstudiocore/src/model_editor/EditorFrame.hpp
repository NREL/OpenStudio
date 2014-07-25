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

#ifndef MODELEDITOR_EDITORFRAME_HPP
#define MODELEDITOR_EDITORFRAME_HPP

#include <QMainWindow>
#include <QString>

#include "../model/Building.hpp"
#include "ModelEditorAPI.hpp"

class QAbstractItemModel;
class QAction;
class QCompleter;
class QLineEdit;
class QMenu;
class QStatusBar;

namespace modeleditor
{

class ModelExplorer;
class TableModel;
class TreeModel;

class MODELEDITOR_API EditorFrame : public QMainWindow
{
  Q_OBJECT

public:
  EditorFrame(QWidget * parent = nullptr);
  virtual ~EditorFrame();
  void loadStyleSheet(QString& style);

protected:
  void closeEvent(QCloseEvent *event);

private:
  void connectSignalsAndSlots();
  void contextMenuEvent(QContextMenuEvent *event);
  void createActions();
  void createLayout();
  void createMenus();
  void createStatusBar();
  void createToolBars();
  void createWidgets();
  QString getFileNameFromPath(const QString &fullFileName);
  bool maybeSave();
  void newFile();
  void openFile(const QString& action, const QString& extension);
  void readSettings();
  bool saveFile(const QString &fileName, const QString& extension);
  bool saveFileAs(const QString& action, const QString &extension);
  void setCurrentFile(const QString &fileName);
  bool updateContextMenu(bool showAllowableChildTypes);
  void writeSettings();
  bool addTreeViewContextMenuAddActions(bool clearMenu = false);
  bool addClassViewContextMenuAddActions(bool clearMenu = false);
  void setModels();

  ModelExplorer * mModelExplorer;
  TableModel * mTableModel;
  TreeModel * mTreeModel;

  QStatusBar * mStatusBar;
  QLineEdit * mSearchViews;
  QCompleter * mCompleter;

  QAction * mAboutAction;
  QAction * mAddAction;
  QAction * mCopyAction;
  QAction * mExitAction;
  QAction * mExportIdfAction;
  QAction * mImportIdfAction;
  QAction * mNewIdkAction;
  QAction * mOpenIdkAction;
  QAction * mPasteAction;
  QAction * mRemoveAction;
  QAction * mSaveIdkAction;
  QAction * mSaveIdkAsAction;
  QAction * mExpandAllNodesAction;
  QAction * mToggleGUIDsAction;
  QAction * mSearchViewsAction;
  QAction * mToggleCommentsAction;
  QAction * mShowAllFields;
  QAction * mCreateAllFields;
  QAction * mIGRecursive;
  QAction * mTogglePrecisionAction;
  QAction * mToggleUnitsAction;

  QMenu * mFileMenu;
  QMenu * mPrefMenu;
  QMenu * mHelpMenu;
  QMenu * mContextMenu;

  QToolBar * mFileToolBar;
  QToolBar * mEditToolBar;
  QToolBar * mPrefToolBar;
  QToolBar * mSearchToolBar;

  QString mActionDescriptionPrefix;
  QString mCurrentFile;
  QString mLastPathOpened;
  std::vector<openstudio::IddObjectType> mAllowableChildTypes;
  bool mShowGUIDs;
  bool mShowComments;
  bool mShowPrecision;

private slots:
  void about();

  void newIdk();
  void openIdk();
  bool saveIdk();
  bool saveIdkAs();

  void newIdf();
  void importIdf();
  bool exportIdf();
  bool exportIdfAs();

  void addObject(QAction * action);
  void addObjects();
  void removeObjects();
  void copyObjects();
  void pasteObjects();

  void on_modelDirty();
  void on_showStatusBarMsg(const QString& msg, const int millisecondDuration);
  void expandAllNodes();
  void on_toggleGUIDs();
  void on_toggleComments();
  void on_togglePrecision();
  void on_toggleUnits();

  void on_modelUpdated(QAbstractItemModel * model);
  void on_precisionDlgHidden();
  void on_precisionDlgFinished();

signals:

};

} // namespace modeleditor

#endif // MODELEDITOR_EDITORFRAME_HPP
