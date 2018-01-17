/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include <QAbstractItemModel>
#include <QAction>
#include <QApplication>
#include <QBoxLayout>
#include <QCompleter>
#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QSettings>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTextStream>
#include <QToolBar>

#include "../energyplus/ForwardTranslator.hpp"
#include "../model/Model.hpp"
#include <model_editor/AboutBox.hpp>
#include "ModelExplorer.hpp"
#include "InspectorGadget.hpp"
#include <model_editor/tablemodel.h>
#include <model_editor/treemodel.h>
#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/idf/Workspace.hpp"
#include "../utilities/idf/WorkspaceObjectOrder.hpp"

#include "../utilities/core/Assert.hpp"

#include "EditorFrame.hpp"


namespace modeleditor
{

EditorFrame::EditorFrame(QWidget * parent)
  : QMainWindow(parent),
    mModelExplorer(nullptr),
    mTableModel(nullptr),
    mTreeModel(nullptr),
    mStatusBar(nullptr),
    mSearchViews(nullptr),
    mCompleter(nullptr),
    mAboutAction(nullptr),
    mAddAction(nullptr),
    mCopyAction(nullptr),
    mExitAction(nullptr),
    mExportIdfAction(nullptr),
    mImportIdfAction(nullptr),
    mNewIdkAction(nullptr),
    mOpenIdkAction(nullptr),
    mPasteAction(nullptr),
    mRemoveAction(nullptr),
    mSaveIdkAction(nullptr),
    mSaveIdkAsAction(nullptr),
    mExpandAllNodesAction(nullptr),
    mToggleGUIDsAction(nullptr),
    mSearchViewsAction(nullptr),
    mToggleCommentsAction(nullptr),
    mTogglePrecisionAction(nullptr),
    mToggleUnitsAction(nullptr),
    mFileMenu(nullptr),
    mHelpMenu(nullptr),
    mContextMenu(nullptr),
    mFileToolBar(nullptr),
    mEditToolBar(nullptr),
    mPrefToolBar(nullptr),
    mSearchToolBar(nullptr),
    mActionDescriptionPrefix(QString("Add ")),
    mLastPathOpened(QApplication::applicationDirPath()),
    mShowGUIDs(true),
    mShowComments(true),
    mShowPrecision(true)
{
  createWidgets();
  createActions();
  createMenus();
  createToolBars();
  createStatusBar();
  createLayout();
  connectSignalsAndSlots();
  readSettings();
  setCurrentFile("");

  ///! TODO HACK
  openstudio::model::Model model = mModelExplorer->getModel();
  openstudio::WorkspaceObjectVector objects = model.objects();
  QStringList strings;
  for (const openstudio::WorkspaceObject& object : objects){
    strings << object.iddObject().name().c_str();
  }
  mCompleter = new QCompleter(strings,this);
  mCompleter->setCaseSensitivity(Qt::CaseInsensitive);
  if(mSearchViews) mSearchViews->setCompleter(mCompleter);
}

EditorFrame::~EditorFrame()
{
}

void EditorFrame::createWidgets()
{
  mModelExplorer = new ModelExplorer(this);
  //mStatusBar = new QStatusBar(this);
}

void EditorFrame::setModels()
{
  mTableModel = mModelExplorer->getTableModel();
  mTreeModel = mModelExplorer->getTreeModel();
}

void EditorFrame::createActions()
{

  mNewIdkAction = new QAction(QIcon(":/images/new.png"), tr("New OSM"), this);
  //mNewIdkAction->setShortcuts(QKeySequence::New);
  mNewIdkAction->setStatusTip(tr("Create a new OSM file"));
  connect(mNewIdkAction, &QAction::triggered, this, &EditorFrame::newIdk);

  mOpenIdkAction = new QAction(QIcon(":/images/open.png"), tr("Open OSM..."), this);
  //mOpenIdkAction->setShortcuts(QKeySequence::Open);
  mOpenIdkAction->setStatusTip(tr("Open an existing OSM file"));
  connect(mOpenIdkAction, &QAction::triggered, this, &EditorFrame::openIdk);

  mSaveIdkAction = new QAction(QIcon(":/images/save.png"), tr("Save OSM"), this);
  //mSaveIdkAction->setShortcuts(QKeySequence::Save);
  mSaveIdkAction->setStatusTip(tr("Save the OSM document to disk"));
  connect(mSaveIdkAction, &QAction::triggered, this, &EditorFrame::saveIdk);

  mSaveIdkAsAction = new QAction(tr("Save OSM As..."), this);
  //mSaveIdkAsAction->setShortcuts(QKeySequence::SaveAs);
  mSaveIdkAsAction->setStatusTip(tr("Save the OSM document under a new name"));
  connect(mSaveIdkAsAction, &QAction::triggered, this, &EditorFrame::saveIdkAs);

  mImportIdfAction = new QAction(QIcon(":/images/me-import.png"), tr("Import IDF..."), this);
  //mImportIdfAction->setShortcuts(QKeySequence::Open);
  mImportIdfAction->setStatusTip(tr("Open an existing file"));
  connect(mImportIdfAction, &QAction::triggered, this, &EditorFrame::importIdf);

  mExportIdfAction = new QAction(QIcon(":/images/me-export.png"), tr("Export IDF"), this);
  //mExportIdfAction->setShortcuts(QKeySequence::Save);
  mExportIdfAction->setStatusTip(tr("Save the IDF document to disk"));
  connect(mExportIdfAction, &QAction::triggered, this, &EditorFrame::exportIdf);

  mExitAction = new QAction(tr("E&xit"), this);
  mExitAction->setShortcuts(QKeySequence::Quit);
  mExitAction->setStatusTip(tr("Exit the application"));
  connect(mExitAction, &QAction::triggered, this, &EditorFrame::close);

  mCopyAction = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
  mCopyAction->setShortcuts(QKeySequence::Copy);
  mCopyAction->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
  connect(mCopyAction, &QAction::triggered, this, &EditorFrame::copyObjects);

  mPasteAction = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
  mPasteAction->setShortcuts(QKeySequence::Paste);
  mPasteAction->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
  connect(mPasteAction, &QAction::triggered, this, &EditorFrame::pasteObjects);

  mAboutAction = new QAction(tr("&About"), this);
  mAboutAction->setStatusTip(tr("Show About"));
  connect(mAboutAction, &QAction::triggered, this, &EditorFrame::about);

  mExpandAllNodesAction = new QAction(tr("&Expand All Nodes"), this);
  mExpandAllNodesAction->setStatusTip(tr("Expand All Tree View Nodes"));
  connect(mExpandAllNodesAction, &QAction::triggered, this, &EditorFrame::expandAllNodes);

  mToggleGUIDsAction = new QAction(QIcon(":/images/number_off.png"), tr("Toggle GUIDs"), this);
  mToggleGUIDsAction->setStatusTip(tr("Toggle to show or hide model name GUIDs"));
  connect(mToggleGUIDsAction, &QAction::triggered, mModelExplorer, &ModelExplorer::toggleGUIDs);
  connect(mToggleGUIDsAction, &QAction::triggered, this, &EditorFrame::on_toggleGUIDs);

  mToggleCommentsAction = new QAction(QIcon(":/images/comment_off.png"), tr("Toggle comments"), this);
  mToggleCommentsAction->setCheckable(true);
  mToggleCommentsAction->setStatusTip(tr("Toggle to show or hide model object comments"));
  connect(mToggleCommentsAction, &QAction::triggered, this, &EditorFrame::on_toggleComments);

  mShowAllFields = new QAction(tr("Show all Fields"),this);
  mShowAllFields->setCheckable(true);
  mShowAllFields->setChecked(true);
  connect(mShowAllFields, &QAction::triggered, mModelExplorer, &ModelExplorer::showAllFields);

  mCreateAllFields = new QAction(tr("Create all Fields"),this);
  connect(mCreateAllFields, &QAction::triggered, mModelExplorer, &ModelExplorer::createAllFields);

  mIGRecursive = new QAction(tr("Apply preferences to children objects"), this);
  mIGRecursive->setCheckable(true);
  connect(mIGRecursive, &QAction::triggered, mModelExplorer, &ModelExplorer::setRecursive);
  mTogglePrecisionAction = new QAction(QIcon(":/images/precision_off.png"), tr("Display precision panel"), this);
  mTogglePrecisionAction->setStatusTip(tr("Toggle to show or hide high precision data"));
  connect(mTogglePrecisionAction, &QAction::triggered, this, &EditorFrame::on_togglePrecision);

  mToggleUnitsAction = new QAction(QIcon(":/images/precision_off.png"), tr("Display precision panel"), this);
  mTogglePrecisionAction->setStatusTip(tr("Toggle to show or hide high precision data"));
  connect(mTogglePrecisionAction, &QAction::triggered, this, &EditorFrame::on_toggleUnits);

  mAddAction = new QAction(QIcon(":/images/edit_add.png"), tr("&Add"), this);
  mAddAction->setShortcuts(QKeySequence::New);
  mAddAction->setStatusTip(tr("Add an object"));
  connect(mAddAction, &QAction::triggered, this, &EditorFrame::addObjects);

  mRemoveAction = new QAction(QIcon(":/images/edit_remove.png"), tr("&Remove"), this);
  mRemoveAction->setShortcuts(QKeySequence::Delete);
  mRemoveAction->setStatusTip(tr("Remove an object"));
  connect(mRemoveAction, &QAction::triggered, this, &EditorFrame::removeObjects);
}

void EditorFrame::createMenus()
{
  mFileMenu = menuBar()->addMenu(tr("&File"));
  mFileMenu->addAction(mNewIdkAction);
  mFileMenu->addAction(mOpenIdkAction);
  mFileMenu->addAction(mSaveIdkAction);
  mFileMenu->addAction(mSaveIdkAsAction);
  mFileMenu->addSeparator();
  mFileMenu->addAction(mImportIdfAction);
  mFileMenu->addAction(mExportIdfAction);
  mFileMenu->addSeparator();
  mFileMenu->addAction(mExitAction);

  menuBar()->addSeparator();

  mPrefMenu = menuBar()->addMenu(tr("&Preferences"));
  mPrefMenu->addAction(mToggleCommentsAction);
  mPrefMenu->addAction(mShowAllFields);
  mPrefMenu->addAction(mCreateAllFields);
  mPrefMenu->addAction(mIGRecursive);
  mPrefMenu->addAction(mTogglePrecisionAction);

  mHelpMenu = menuBar()->addMenu(tr("&Help"));
  mHelpMenu->addAction(mAboutAction);

  mContextMenu = new QMenu(this);
  ///! Use context menu signal to inform addObject() the text of the action clicked
  connect(mContextMenu, &QMenu::triggered, this, &EditorFrame::addObject);
}

void EditorFrame::createToolBars()
{
  mFileToolBar = addToolBar(tr("File"));
  mFileToolBar->addAction(mNewIdkAction);
  mFileToolBar->addAction(mOpenIdkAction);
  mFileToolBar->addAction(mSaveIdkAction);
  mFileToolBar->addSeparator();
  mFileToolBar->addAction(mImportIdfAction);
  mFileToolBar->addAction(mExportIdfAction);

  mEditToolBar = addToolBar(tr("Edit"));
  mEditToolBar->addAction(mCopyAction);
  mEditToolBar->addAction(mPasteAction);
  mEditToolBar->addAction(mAddAction);
  mEditToolBar->addAction(mRemoveAction);
  mEditToolBar->addAction(mToggleGUIDsAction);
  mEditToolBar->addAction(mToggleCommentsAction);
  mEditToolBar->addAction(mTogglePrecisionAction);

  mPrefToolBar = addToolBar(tr("Preferences"));
  mEditToolBar->addAction(mToggleUnitsAction);
  mEditToolBar->addAction(mToggleUnitsAction);

  //mSearchToolBar = addToolBar(tr("Search"));

  //mSearchViews = new QLineEdit(mSearchToolBar);

  //mSearchViewsAction = mSearchToolBar->addWidget(mSearchViews);

  //mSearchViewsAction->setVisible(true);
}

void EditorFrame::createStatusBar()
{
  statusBar()->showMessage(tr("Ready"));
}

void EditorFrame::connectSignalsAndSlots()
{

  connect(mModelExplorer, &ModelExplorer::modelDirty, this, &EditorFrame::on_modelDirty);

  connect(mModelExplorer, &ModelExplorer::showStatusBarMsg, this, &EditorFrame::on_showStatusBarMsg);

  connect(mModelExplorer, &ModelExplorer::precisionDlgHidden, this, &EditorFrame::on_precisionDlgHidden);

  connect(mModelExplorer, &ModelExplorer::precisionDlgFinished, this, &EditorFrame::on_precisionDlgFinished);
}

void EditorFrame::createLayout()
{
  setCentralWidget(mModelExplorer);
  ///! currently there are two toolbars being employed
  ///! and Qt does not display them correctly on Mac
  ///! with this call
  //setUnifiedTitleAndToolBarOnMac(true);
}

void EditorFrame::loadStyleSheet(QString& style)
{
  QFile data(":/openstudio.qss");
  if(data.open(QFile::ReadOnly))
  {
      QTextStream styleIn(&data);
      style = styleIn.readAll();
      data.close();
  }
}

bool EditorFrame::addClassViewContextMenuAddActions(bool clearMenu)
{
  bool success = false;

  if(!mContextMenu){
    return success;
  }

  if(clearMenu){
    mContextMenu->clear();
  }

  openstudio::IddFile iddFile = mModelExplorer->getIddFile();
  std::string group;
  std::vector<std::string> groups = iddFile.groups();
  openstudio::IddObject object;
  std::vector<openstudio::IddObject> objects;

  QAction * newAddAction = nullptr;
  QString name;
  QString string;
  QMenu * addSubMenu = nullptr;

  for(unsigned i=0 ; i<groups.size(); i++){
    group = groups.at(i);
    objects = iddFile.getObjectsInGroup(group);
    QMenu * subMenu = new QMenu(group.c_str());
    subMenu->setIcon(QIcon(":/images/edit_add.png"));
    for(unsigned j=0 ; j<objects.size(); j++){
      object = objects.at(j);
      name = object.name().c_str();
      string = mActionDescriptionPrefix + name;
      newAddAction = new QAction(QIcon(":/images/edit_add.png"), tr(string.toStdString().c_str()), this);
      ///! No connection required as the context menu will always call addObject()
      ///! Context menu signal has QAction->text() to pass to addObject()
      subMenu->addAction(newAddAction);
    }
    if(objects.size() && subMenu){
      if(!success){
        addSubMenu = new QMenu(tr("Add"));
        addSubMenu->setIcon(QIcon(":/images/edit_add.png"));
        OS_ASSERT(addSubMenu);
        mContextMenu->addMenu(addSubMenu);
        success = true;
      }
      addSubMenu->addMenu(subMenu);
    }
  }

  if(!success){
    QAction * inactiveAddAction = new QAction(QIcon(":/images/edit_add_off.png"), tr("Nothing available to add to this object"), this);
    mContextMenu->addAction(inactiveAddAction);
    success = true;
  }

  return success;
}

bool EditorFrame::addTreeViewContextMenuAddActions(bool clearMenu)
{
  bool success = false;

  if(!mContextMenu){
    return success;
  }

  if(clearMenu){
    mContextMenu->clear();
  }

  mAllowableChildTypes.clear();
  mAllowableChildTypes = mModelExplorer->getAllowableChildTypes();
  QMenu * addSubMenu = nullptr;
  //bool connected = false;
  QString name;
  QString string;
  for(unsigned i=0 ; i<mAllowableChildTypes.size(); i++){
    if(addSubMenu == nullptr){
      addSubMenu = new QMenu(tr("Add"));
      addSubMenu->setIcon(QIcon(":/images/edit_add.png"));
      OS_ASSERT(addSubMenu);
      mContextMenu->addMenu(addSubMenu);
    }
    name = mAllowableChildTypes.at(i).valueName().c_str();
    string = mActionDescriptionPrefix + name;
    QAction * newAddAction = new QAction(QIcon(":/images/edit_add.png"), tr(string.toStdString().c_str()), this);
    ///! No connection required as the context menu will always call addObject()
    ///! Context menu signal has QAction->text() to pass to addObject()
    addSubMenu->addAction(newAddAction);
  }
  if(mAllowableChildTypes.size()){
    success = true;
  }

  if(!success){
    QAction * inactiveAddAction = new QAction(QIcon(":/images/edit_add_off.png"), tr("Nothing available to add to this object"), this);
    mContextMenu->addAction(inactiveAddAction);
    success = true;
  }

  return success;
}

bool EditorFrame::updateContextMenu(bool showAllowableChildTypes)
{
  bool success = false;

  if(!mContextMenu){
    return success;
  }
  else{
    success = true;
  }

  mContextMenu->clear();

  if(showAllowableChildTypes){
    mContextMenu->addAction(mExpandAllNodesAction);
  }

  mContextMenu->addAction(mCopyAction);
  mContextMenu->addAction(mPasteAction);

  if(showAllowableChildTypes){
    mPasteAction->setDisabled(!mModelExplorer->treeViewHasRowsToPaste());
    addTreeViewContextMenuAddActions();
  }
  else{
    mPasteAction->setDisabled(!mModelExplorer->classViewHasRowsToPaste());
    addClassViewContextMenuAddActions();
  }
  mContextMenu->addAction(mRemoveAction);

  return success;
}

void EditorFrame::contextMenuEvent(QContextMenuEvent *event)
{
  if(!mModelExplorer->hasSelectedRows()){
    return;
  }

  if(mModelExplorer->classViewUnderMouse()){
    if(updateContextMenu(false)){
      mContextMenu->exec(event->globalPos());
    }
  }
  else if(mModelExplorer->treeViewUnderMouse()){
    if(updateContextMenu(true)){
      mContextMenu->exec(event->globalPos());
    }
  }
}

void EditorFrame::closeEvent(QCloseEvent *event)
{
  if(this->maybeSave()){
    writeSettings();
    event->accept();
  }
  else{
    event->ignore();
  }
}

void EditorFrame::readSettings()
{
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
  QSize size = settings.value("size", QSize(600, 400)).toSize();
  resize(size);
  move(pos);
  restoreGeometry(settings.value("geometry").toByteArray());
  restoreState(settings.value("state").toByteArray());
  mLastPathOpened = settings.value("lastPathOpened").toString();
}

void EditorFrame::writeSettings()
{
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  settings.setValue("pos", pos());
  settings.setValue("size", size());
  settings.setValue("geometry", saveGeometry());
  settings.setValue("state", saveState());
  settings.setValue("lastPathOpened", mLastPathOpened);
}

bool EditorFrame::maybeSave()
{
  if(isWindowModified()){
    QString applicationName = QCoreApplication::applicationName();
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, applicationName,
      tr("Do you want to save your changes?"),
      QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
    if (ret == QMessageBox::Save){
      return saveIdk(); // TODO might need to call exportIdf
    }
    else if (ret == QMessageBox::No){
      return true;
    }
    else if (ret == QMessageBox::Cancel){
      return false;
    }
  }
  return true;
}

void EditorFrame::setCurrentFile(const QString &fileName)
{
  mCurrentFile = fileName;
  setWindowModified(false);

  QString shownName = mCurrentFile;
  if (mCurrentFile.isEmpty()){
    shownName = "untitled";
  }
  setWindowFilePath(shownName);
}

QString EditorFrame::getFileNameFromPath(const QString &path)
{
  return QFileInfo(path).fileName();
}

void EditorFrame::newFile()
{
  openstudio::model::Model m = openstudio::model::Model();
  m.order().setDirectOrder(openstudio::HandleVector());
  mModelExplorer->setModel(m);
  mModelExplorer->loadModel();
}

void EditorFrame::openFile(const QString& action, const QString& extension)
{
  if(maybeSave()){
    QString msg(" ");
    msg.prepend(action);
    msg += extension.toUpper();

    QString fileTypes("Files (*.");
    fileTypes += extension.toLower();
    fileTypes += ")";

    QString fileName = QFileDialog::getOpenFileName(this,msg,mLastPathOpened,fileTypes);
    if(!fileName.isEmpty()){
      mModelExplorer->loadFile(fileName);
      setCurrentFile(fileName);
      mLastPathOpened = QFileInfo(fileName).absoluteFilePath();
      statusBar()->showMessage(msg, 2000);
    }
  }
}

bool EditorFrame::saveFileAs(const QString& action, const QString& extension)
{
  bool success = false;

  QString msg(" ");
  msg.prepend(action);
  msg += extension.toUpper();

  QString fileTypes("Files (*.");
  fileTypes += extension.toLower();
  fileTypes += ")";

  QString fileName = QFileDialog::getSaveFileName(nullptr,msg,"",fileTypes);
  if(!fileName.isEmpty()){
    success = saveFile(fileName, extension);
  }

  return success;
}

bool EditorFrame::saveFile(const QString& fileName, const QString& extension)
{
  openstudio::model::Model model = mModelExplorer->getModel();

  openstudio::filesystem::ofstream stream(openstudio::toPath(fileName));
  if(extension.toLower().toStdString() == "idf"){
    openstudio::energyplus::ForwardTranslator forwardTranslator;
    openstudio::Workspace workspace = forwardTranslator.translateModel(model);
    workspace.toIdfFile().print(stream);
  }
  else if(extension.toLower().toStdString() == "osm"){
    model.toIdfFile().print(stream);
  }
  else{
    return false;
  }

  stream.close();
  setCurrentFile(fileName);
  setWindowModified(false);

  return true;
}

///! Slots
void EditorFrame::about()
{
  QString applicationName = QCoreApplication::applicationName();
  QString msg(tr("About "));
  msg += applicationName;

  QMessageBox about(this);
  about.setText(MODELEDITOR_ABOUTBOX);
  about.setWindowTitle(msg);
  about.setIcon(QMessageBox::NoIcon);
  about.exec();
}

void EditorFrame::newIdk()
{
  newFile();
}

void EditorFrame::newIdf()
{
  newFile();
}

void EditorFrame::openIdk()
{
  openFile("Open", "osm");
}

void EditorFrame::importIdf()
{
  openFile("Import", "idf");
}

bool EditorFrame::saveIdk()
{
  ///! check for correct file extension
  if(mCurrentFile.isEmpty() || (mCurrentFile.right(4).toLower() != ".osm")){
    return saveIdkAs();
  }
  else{
    return saveFile(mCurrentFile, "osm");
  }
}

bool EditorFrame::exportIdf()
{
  ///! check for correct file extension
  if(mCurrentFile.isEmpty() || (mCurrentFile.right(4).toLower() != ".idf")){
    return exportIdfAs();
  }
  else{
    return saveFile(mCurrentFile, "idf");
  }
}

bool EditorFrame::saveIdkAs()
{
  return saveFileAs("Save", "osm");
}

bool EditorFrame::exportIdfAs()
{
  return saveFileAs("Export", "idf");
}

void EditorFrame::addObject(QAction * action)
{
  QString text(action->text());

  ///! Only continue if correct QAction selected
  if(!text.startsWith(mActionDescriptionPrefix)){
    return;
  }

  text.remove(0,mActionDescriptionPrefix.size());

  if(mModelExplorer->classViewCurrentWidget()){
    openstudio::IddFile iddFile = mModelExplorer->getIddFile();
    boost::optional<openstudio::IddObject> object = iddFile.getObject(text.toStdString());
    if(object){
      mModelExplorer->addObjects(object.get().type());
    }
  }
  else if(mModelExplorer->treeViewCurrentWidget()){
    for(unsigned i=0; i<mAllowableChildTypes.size(); i++){
      if(text.toStdString() == mAllowableChildTypes.at(i).valueName()){
        mModelExplorer->addObjects(mAllowableChildTypes.at(i));
        break;
      }
    }
  }
}

void EditorFrame::addObjects()
{
  bool success = false;

  if(mModelExplorer->classViewCurrentWidget()){
    success = addClassViewContextMenuAddActions(true);
  }
  else if(mModelExplorer->treeViewCurrentWidget()){
    success = addTreeViewContextMenuAddActions(true);
  }

  if(success){
    mContextMenu->exec(mEditToolBar->mapToGlobal(mEditToolBar->pos()));
  }
}

void EditorFrame::removeObjects()
{
  if(mModelExplorer->hasSelectedRows()){
    mModelExplorer->removeObjects();
  }
}

void EditorFrame::copyObjects()
{
  if(mModelExplorer->hasSelectedRows()){
    mModelExplorer->copyObjects();
  }
}

void EditorFrame::pasteObjects()
{
  if(mModelExplorer->hasSelectedRows()){
    mModelExplorer->pasteObjects();
  }
}

void EditorFrame::on_modelDirty()
{
  setWindowModified(true);
}

void EditorFrame::on_showStatusBarMsg(const QString& msg, const int millisecondDuration)
{
  statusBar()->showMessage(msg, millisecondDuration);
}

void EditorFrame::expandAllNodes()
{
  mModelExplorer->expandAllNodes();
}

void EditorFrame::on_toggleGUIDs()
{
  if(mShowGUIDs){
    mToggleGUIDsAction->setIcon(QIcon(":/images/number_on.png"));
  }
  else{
    mToggleGUIDsAction->setIcon(QIcon(":/images/number_off.png"));
  }
  mShowGUIDs = !mShowGUIDs;
}

void EditorFrame::on_toggleComments()
{
  if(mShowComments){
    mToggleCommentsAction->setIcon(QIcon(":/images/comment.png"));
  }
  else{
    mToggleCommentsAction->setIcon(QIcon(":/images/comment_off.png"));
  }
  mModelExplorer->showComments(mShowComments);
  mShowComments = !mShowComments;
}

void EditorFrame::on_togglePrecision()
{
  if(mShowPrecision){
    mTogglePrecisionAction->setIcon(QIcon(":/images/precision.png"));
  }
  else{
    mTogglePrecisionAction->setIcon(QIcon(":/images/precision_off.png"));
  }
  mModelExplorer->showPrecision(mShowPrecision);
  mShowPrecision = !mShowPrecision;
}

void EditorFrame::on_toggleUnits()
{
  if(mShowPrecision){
    mToggleUnitsAction->setIcon(QIcon(":/images/precision.png"));
  }
  else{
    mToggleUnitsAction->setIcon(QIcon(":/images/precision_off.png"));
  }
  mShowPrecision = !mShowPrecision;
}

void EditorFrame::on_modelUpdated(QAbstractItemModel * model)
{
  if(mCompleter){
    mCompleter->setModel(model);
  }
}

void EditorFrame::on_precisionDlgHidden()
{
  mShowPrecision = true;
  mTogglePrecisionAction->setIcon(QIcon(":/images/precision_off.png"));
}

void EditorFrame::on_precisionDlgFinished()
{
  mShowPrecision = true;
  mTogglePrecisionAction->setIcon(QIcon(":/images/precision_off.png"));
}

} // namespace modeleditor
