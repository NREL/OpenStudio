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
  void closeEvent(QCloseEvent *event) override;

private:
  void connectSignalsAndSlots();
  void contextMenuEvent(QContextMenuEvent *event) override;
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
