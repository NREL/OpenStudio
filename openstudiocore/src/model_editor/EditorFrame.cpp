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
#include <boost/filesystem/fstream.hpp>

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
  bool connected = false;

  mNewIdkAction = new QAction(QIcon(":/images/new.png"), tr("New OSM"), this);
  //mNewIdkAction->setShortcuts(QKeySequence::New);
  mNewIdkAction->setStatusTip(tr("Create a new OSM file"));
  connected = connect(mNewIdkAction, SIGNAL(triggered()), this, SLOT(newIdk()));
  OS_ASSERT(connected);

  mOpenIdkAction = new QAction(QIcon(":/images/open.png"), tr("Open OSM..."), this);
  //mOpenIdkAction->setShortcuts(QKeySequence::Open);
  mOpenIdkAction->setStatusTip(tr("Open an existing OSM file"));
  connected = connect(mOpenIdkAction, SIGNAL(triggered()), this, SLOT(openIdk()));
  OS_ASSERT(connected);

  mSaveIdkAction = new QAction(QIcon(":/images/save.png"), tr("Save OSM"), this);
  //mSaveIdkAction->setShortcuts(QKeySequence::Save);
  mSaveIdkAction->setStatusTip(tr("Save the OSM document to disk"));
  connected = connect(mSaveIdkAction, SIGNAL(triggered()), this, SLOT(saveIdk()));
  OS_ASSERT(connected);

  mSaveIdkAsAction = new QAction(tr("Save OSM As..."), this);
  //mSaveIdkAsAction->setShortcuts(QKeySequence::SaveAs);
  mSaveIdkAsAction->setStatusTip(tr("Save the OSM document under a new name"));
  connected = connect(mSaveIdkAsAction, SIGNAL(triggered()), this, SLOT(saveIdkAs()));
  OS_ASSERT(connected);

  mImportIdfAction = new QAction(QIcon(":/images/me-import.png"), tr("Import IDF..."), this);
  //mImportIdfAction->setShortcuts(QKeySequence::Open);
  mImportIdfAction->setStatusTip(tr("Open an existing file"));
  connected = connect(mImportIdfAction, SIGNAL(triggered()), this, SLOT(importIdf()));
  OS_ASSERT(connected);

  mExportIdfAction = new QAction(QIcon(":/images/me-export.png"), tr("Export IDF"), this);
  //mExportIdfAction->setShortcuts(QKeySequence::Save);
  mExportIdfAction->setStatusTip(tr("Save the IDF document to disk"));
  connected = connect(mExportIdfAction, SIGNAL(triggered()), this, SLOT(exportIdf()));
  OS_ASSERT(connected);

  mExitAction = new QAction(tr("E&xit"), this);
  mExitAction->setShortcuts(QKeySequence::Quit);
  mExitAction->setStatusTip(tr("Exit the application"));
  connected = connect(mExitAction, SIGNAL(triggered()), this, SLOT(close()));
  OS_ASSERT(connected);

  mCopyAction = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
  mCopyAction->setShortcuts(QKeySequence::Copy);
  mCopyAction->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
  connected = connect(mCopyAction, SIGNAL(triggered()), this, SLOT(copyObjects()));
  OS_ASSERT(connected);

  mPasteAction = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
  mPasteAction->setShortcuts(QKeySequence::Paste);
  mPasteAction->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
  connected = connect(mPasteAction, SIGNAL(triggered()), this, SLOT(pasteObjects()));
  OS_ASSERT(connected);

  mAboutAction = new QAction(tr("&About"), this);
  mAboutAction->setStatusTip(tr("Show About"));
  connected = connect(mAboutAction, SIGNAL(triggered()), this, SLOT(about()));
  OS_ASSERT(connected);

  mExpandAllNodesAction = new QAction(tr("&Expand All Nodes"), this);
  mExpandAllNodesAction->setStatusTip(tr("Expand All Tree View Nodes"));
  connected = connect(mExpandAllNodesAction, SIGNAL(triggered()), this, SLOT(expandAllNodes()));
  OS_ASSERT(connected);

  mToggleGUIDsAction = new QAction(QIcon(":/images/number_off.png"), tr("Toggle GUIDs"), this);
  mToggleGUIDsAction->setStatusTip(tr("Toggle to show or hide model name GUIDs"));
  connected = connect(mToggleGUIDsAction, SIGNAL(triggered()), mModelExplorer, SLOT(toggleGUIDs()));
  OS_ASSERT(connected);
  connected = connect(mToggleGUIDsAction, SIGNAL(triggered()), this, SLOT(on_toggleGUIDs()));
  OS_ASSERT(connected);

  mToggleCommentsAction = new QAction(QIcon(":/images/comment_off.png"), tr("Toggle comments"), this);
  mToggleCommentsAction->setCheckable(true);
  mToggleCommentsAction->setStatusTip(tr("Toggle to show or hide model object comments"));
  connected = connect(mToggleCommentsAction, SIGNAL(triggered()), this, SLOT(on_toggleComments()));
  OS_ASSERT(connected);

  mShowAllFields = new QAction(tr("Show all Fields"),this);
  mShowAllFields->setCheckable(true);
  mShowAllFields->setChecked(true);
  connected = connect(mShowAllFields,SIGNAL(triggered(bool)),mModelExplorer,SLOT(showAllFields(bool)));
  OS_ASSERT(connected);
    
  mCreateAllFields = new QAction(tr("Create all Fields"),this);
  connected = connect(mCreateAllFields,SIGNAL(triggered()),mModelExplorer,SLOT(createAllFields()));
  OS_ASSERT(connected);

  mIGRecursive = new QAction(tr("Apply preferences to children objects"), this);
  mIGRecursive->setCheckable(true);
  connect(mIGRecursive,SIGNAL(triggered(bool)),mModelExplorer,SLOT(setRecursive(bool)));
  

  mTogglePrecisionAction = new QAction(QIcon(":/images/precision_off.png"), tr("Display precision panel"), this);
  mTogglePrecisionAction->setStatusTip(tr("Toggle to show or hide high precision data"));
  connected = connect(mTogglePrecisionAction, SIGNAL(triggered()), this, SLOT(on_togglePrecision()));
  OS_ASSERT(connected);

  mToggleUnitsAction = new QAction(QIcon(":/images/precision_off.png"), tr("Display precision panel"), this);
  mTogglePrecisionAction->setStatusTip(tr("Toggle to show or hide high precision data"));
  connected = connect(mTogglePrecisionAction, SIGNAL(triggered()), this, SLOT(on_toggleUnits()));
  OS_ASSERT(connected);

  mAddAction = new QAction(QIcon(":/images/edit_add.png"), tr("&Add"), this);
  mAddAction->setShortcuts(QKeySequence::New);
  mAddAction->setStatusTip(tr("Add an object"));
  connected = connect(mAddAction, SIGNAL(triggered()), this, SLOT(addObjects()));
  OS_ASSERT(connected);
  
  mRemoveAction = new QAction(QIcon(":/images/edit_remove.png"), tr("&Remove"), this);
  mRemoveAction->setShortcuts(QKeySequence::Delete);
  mRemoveAction->setStatusTip(tr("Remove an object"));
  connected = connect(mRemoveAction, SIGNAL(triggered()), this, SLOT(removeObjects()));
  OS_ASSERT(connected);
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
  bool connected = connect(mContextMenu, SIGNAL(triggered(QAction *)), this, SLOT(addObject(QAction *)));
  OS_ASSERT(connected);
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
  bool connected = false;

  connected = connect(mModelExplorer,SIGNAL(modelDirty()),
    this,SLOT(on_modelDirty()));
  OS_ASSERT(connected);

  connected = connect(mModelExplorer,SIGNAL(showStatusBarMsg(const QString&, const int)),
    this,SLOT(on_showStatusBarMsg(const QString&, const int)));
  OS_ASSERT(connected);

  connected = connect(mModelExplorer,SIGNAL(precisionDlgHidden()),
    this,SLOT(on_precisionDlgHidden()));
  OS_ASSERT(connected);

  connected = connect(mModelExplorer,SIGNAL(precisionDlgFinished()),
    this,SLOT(on_precisionDlgFinished()));
  OS_ASSERT(connected);
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

  boost::filesystem::ofstream stream(openstudio::toPath(fileName));
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
