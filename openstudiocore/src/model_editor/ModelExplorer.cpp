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

#include <QAction>
#include <QApplication>
#include <QBoxLayout>
#include <QComboBox>
#include <QDialog>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QProgressBar>
#include <QSettings>
#include <QSplitter>
#include <QStackedWidget>
#include <QTime>
#include <QToolBar>

#include <resources.hxx>
#include "../energyplus/ReverseTranslator.hpp"
#include "../model/Model.hpp"
#include "ClassViewWidget.hpp"
#include "ObjectExplorer.hpp"
#include "TreeViewWidget.hpp"
#include "ViewWidget.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/PathHelpers.hpp"
#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/idf/Workspace.hpp"
#include "../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../utilities/plot/ProgressBar.hpp"
#include "../utilities/core/Compare.hpp"

#include "ModelExplorer.hpp"

using namespace openstudio::model;
using namespace openstudio;

namespace modeleditor
{

  ModelExplorer::ModelExplorer(QWidget * parent)
    : QWidget(parent),
      mClassViewWidget(nullptr),
      mTreeViewWidget(nullptr),
      mObjectExplorer(nullptr),
      mClassViewUnderMouse(false),
      mTreeViewUnderMouse(false),
      mProgressBarLbl(nullptr),
      mSplitter(nullptr),
      mDlg(nullptr),
      mProgressBar(nullptr),
      mQProgressBar(nullptr),
      mClassAction(nullptr),
      mTreeAction(nullptr),
      mStackedWidget(nullptr),
      mToolBar(nullptr),
      mModel(openstudio::model::Model()),
      mIddFile(mModel.iddFile())
  {
    mModel.order().setDirectOrder(HandleVector());
    createProgressDlg();
    createWidgets();
    createActions();
    createToolBars();
    createLayout();
    connectSignalsAndSlots();
    restoreState();
    setIddFile(mModel);
    expandAllNodes();
  }

  ModelExplorer::~ModelExplorer()
  {
    saveState();
  }

  void ModelExplorer::saveState()
  {
    QString organizationName = QCoreApplication::organizationName();
    QString applicationName = QCoreApplication::applicationName();
    QSettings settings(organizationName, applicationName);
    settings.setValue("ModelExplorerSplitterSizes", mSplitter->saveState());
  }

  void ModelExplorer::restoreState()
  {
    QString organizationName = QCoreApplication::organizationName();
    QString applicationName = QCoreApplication::applicationName();
    QSettings settings(organizationName, applicationName);
    mSplitter->restoreState(settings.value("ModelExplorerSplitterSizes").toByteArray());
  }

  void ModelExplorer::createProgressDlg()
  {
    mDlg = new QDialog(this, Qt::WindowTitleHint);
    mDlg->setWindowTitle("STATUS");

    mProgressBarLbl = new QLabel();
    mQProgressBar = new QProgressBar(this);
    mProgressBar = new openstudio::ProgressBar(mQProgressBar);

    auto layout = new QVBoxLayout();
    layout->addWidget(mProgressBarLbl);
    layout->addWidget(mQProgressBar);
    mDlg->setLayout(layout);
  }

  void ModelExplorer::createWidgets()
  {
    mTreeViewWidget = new TreeViewWidget(this);
    mClassViewWidget = new ClassViewWidget(this);
    mObjectExplorer = new ObjectExplorer(mIddFile,this);
    mStackedWidget = new QStackedWidget(this);

    mSplitter = new QSplitter(Qt::Vertical,this);
    QSplitter splitter;
    mSplitter->setHandleWidth(2*splitter.handleWidth());

    mSplitter->addWidget(mStackedWidget);
    mSplitter->addWidget(mObjectExplorer);
  }

  void ModelExplorer::createActions()
  {
    mClassAction = new QAction(QIcon(":/images/class_off.png"), tr("Class View"), this);
    mClassAction->setStatusTip(tr("Display Class View"));

    mSystemOutlinerAction = new QAction(QIcon(":/images/hvac.png"), tr("System Outliner"), this);
    mSystemOutlinerAction->setStatusTip(tr("Display System Outliner"));

    mTreeAction = new QAction(QIcon(":/images/tree.png"), tr("Tree View"), this);
    mTreeAction->setStatusTip(tr("Display Tree View"));
  }

  void ModelExplorer::createToolBars()
  {
    mToolBar = new QToolBar(this);
    mToolBar->addAction(mTreeAction);
    mToolBar->addAction(mClassAction);
    //mToolBar->addAction(mSystemOutlinerAction);
  }

  void ModelExplorer::createLayout()
  {
    QPixmap  pixmap(":/systemOutliner.JPG");
    auto systemOutlinerLabel = new QLabel(this);
    systemOutlinerLabel->setPixmap(pixmap);
    systemOutlinerLabel->setScaledContents(true);

    /* int treeIdx = */ mStackedWidget->addWidget(mTreeViewWidget);
    /* int classIdx = */ mStackedWidget->addWidget(mClassViewWidget);
    /* int systemOutlinerIdx = */ mStackedWidget->addWidget(systemOutlinerLabel);

    auto mainLayout = new QVBoxLayout();
    mainLayout->addWidget(mToolBar);
    mainLayout->addWidget(mSplitter);

    setLayout(mainLayout);
  }

  void ModelExplorer::connectSignalsAndSlots()
  {
    bool connected;
    connected = connect(mClassAction, SIGNAL(triggered()), this, SLOT(classAction()));
    OS_ASSERT(connected);

    connected = connect(mSystemOutlinerAction, SIGNAL(triggered()), this, SLOT(systemOutlinerAction()));
    OS_ASSERT(connected);

    connected = connect(mTreeAction, SIGNAL(triggered()), this, SLOT(treeAction()));
    OS_ASSERT(connected);

    connected = connect(mClassViewWidget,SIGNAL(eventEnter()),
                        this,SLOT(on_ClassViewEventEnter()));
    OS_ASSERT(connected);

    connected = connect(mClassViewWidget,SIGNAL(eventLeave()),
                        this,SLOT(on_ClassViewEventLeave()));
    OS_ASSERT(connected);

    connected = connect(mTreeViewWidget,SIGNAL(eventEnter()),
                        this,SLOT(on_TreeViewEventEnter()));
    OS_ASSERT(connected);

    connected = connect(mTreeViewWidget,SIGNAL(eventLeave()),
                        this,SLOT(on_TreeViewEventLeave()));
    OS_ASSERT(connected);

    connected = connect(mTreeViewWidget,SIGNAL(modelDirty()),
                        mClassViewWidget,SLOT(on_modelDirty()));
    OS_ASSERT(connected);

    connected = connect(mClassViewWidget,SIGNAL(modelDirty()),
                        mTreeViewWidget,SLOT(on_modelDirty()));
    OS_ASSERT(connected);

    connected = connect(mTreeViewWidget,SIGNAL(modelDirty()),
                        this,SIGNAL(modelDirty()));
    OS_ASSERT(connected);

    connected = connect(mClassViewWidget,SIGNAL(modelDirty()),
                        this,SIGNAL(modelDirty()));
    OS_ASSERT(connected);

    connected = connect(mTreeViewWidget,SIGNAL(precisionDlgFinished()),
                        this,SIGNAL(precisionDlgFinished()));
    OS_ASSERT(connected);

    connected = connect(mClassViewWidget,SIGNAL(precisionDlgFinished()),
                        this,SIGNAL(precisionDlgFinished()));
    OS_ASSERT(connected);
  }

  void ModelExplorer::showComments(const bool showComments)
  {
    ViewWidget * widget = nullptr;
    widget = static_cast<ViewWidget *>(mStackedWidget->currentWidget());
    if(widget){
      widget->showComments(showComments);
    }
  }

  void ModelExplorer::showPrecision(const bool showPrecision)
  {
    ViewWidget * widget = nullptr;
    widget = static_cast<ViewWidget *>(mStackedWidget->currentWidget());
    if(widget){
      widget->showPrecisionDlg(showPrecision);
    }
  }
 
  int ModelExplorer::currentIndex() const
  {
    return mStackedWidget->currentIndex();
  }

  void ModelExplorer::addObjects(openstudio::IddObjectType type)
  {
    ViewWidget * widget = nullptr;
    widget = static_cast<ViewWidget *>(mStackedWidget->currentWidget());
    if(widget){
      widget->addObjects(type);
    }
  }

  void ModelExplorer::removeObjects()
  {
    ViewWidget * widget = nullptr;
    widget = static_cast<ViewWidget *>(mStackedWidget->currentWidget());
    if(widget){
      widget->removeObjects();
    }
  }

  void ModelExplorer::copyObjects()
  {
    ViewWidget * widget = nullptr;
    widget = static_cast<ViewWidget *>(mStackedWidget->currentWidget());
    if(widget){
      widget->copyObjects();
    }
  }

  void ModelExplorer::pasteObjects()
  {
    ViewWidget * widget = nullptr;
    widget = static_cast<ViewWidget *>(mStackedWidget->currentWidget());
    if(widget){
      widget->pasteObjects();
    }
  }

  void ModelExplorer::loadFile(const QString &fileName)
  {
    mDlg->show();

    openstudio::model::OptionalModel optionalModel;

    // see what type of file we are opening based on extension
    openstudio::path path = openstudio::toPath(fileName.toStdString().c_str());
    if (openstudio::istringEqual("." + openstudio::modelFileExtension(), openstudio::toString(path.extension()))){
      QString text("Loading ");
      text += openstudio::modelFileExtension().c_str();
      text += " file";
      mProgressBarLbl->setText(text);
      OptionalIdfFile oIdfFile = IdfFile::load(path,mProgressBar);
      if(oIdfFile){
        optionalModel = Model(*oIdfFile);
      }
    }else if(openstudio::istringEqual(".idf", openstudio::toString(path.extension()))){
      mProgressBarLbl->setText("Loading IDF file");
      openstudio::OptionalIdfFile idfFile = openstudio::IdfFile::load(path,openstudio::IddFileType::EnergyPlus,mProgressBar);
      if(idfFile){
        /*
        mProgressBarLbl->setText("Creating workspace");
        openstudio::Workspace workspace(*idfFile,StrictnessLevel::None);
        // START DEBUG CODE
        openstudio::WorkspaceObjectVector wsObjects = workspace.objects();
        openstudio::WorkspaceObjectOrder order = workspace.order();
        for (const openstudio::WorkspaceObject wso : wsObjects){
          OS_ASSERT(order.inOrder(wso.handle()));
        }
        // END DEBUG CODE
        openstudio::energyplus::ReverseTranslator2 reverseTranslator(workspace);
        mProgressBarLbl->setText("Creating workspace");
        optionalModel = reverseTranslator.convert();
        // BEGIN DEBUG CODE
        if(optionalModel){
          order = optionalModel->order();
          wsObjects = optionalModel->objects();
          for (const openstudio::WorkspaceObject wso : wsObjects){
            OS_ASSERT(order.inOrder(wso.handle()));
          }
        }
        // END DEBUG CODE
        */
      }
    }

    mDlg->hide();

    if(!optionalModel){
      QMessageBox::critical(this, "Unable to obtain an OpenStudio Model", "Verify that your input file is the correct version.");
      return;
    }

    setModel(optionalModel.get());
    loadModel();
    expandAllNodes();
  }

  void ModelExplorer::expandAllNodes()
  {
    mTreeViewWidget->expandAllNodes();
  }

  openstudio::model::Model& ModelExplorer::getModel()
  {
    return mModel;
  }

  const openstudio::IddFile& ModelExplorer::getIddFile()
  {
    return mIddFile;
  }

  void ModelExplorer::setModel(openstudio::model::Model& model)
  {
    mModel = model;
    ///! always need a new file with a new model
    setIddFile(mModel);
  }

  void ModelExplorer::setIddFile(openstudio::model::Model& model)
  {
    mIddFile = model.iddFile();
    ///! tell mObjectExplorer to get the new iddFile
    mObjectExplorer->updateIddFile();
  }

  void ModelExplorer::loadModel()
  {
    //mModel.order().directOrder(); // or .iddOrder();
    mTreeViewWidget->loadModel();
    mClassViewWidget->loadModel();
  }

  bool ModelExplorer::classViewUnderMouse()
  {
    return mClassViewUnderMouse;
  }

  bool ModelExplorer::treeViewUnderMouse()
  {
    return mTreeViewUnderMouse;
  }

  bool ModelExplorer::classViewCurrentWidget()
  {
    return (mClassViewWidget == mStackedWidget->currentWidget());
  }

  bool ModelExplorer::treeViewCurrentWidget()
  {
    return (mTreeViewWidget == mStackedWidget->currentWidget());
  }

  bool ModelExplorer::hasSelectedRows()
  {
    return(mTreeViewWidget->hasSelectedRows() || mClassViewWidget->hasSelectedRows());
  }

  std::vector<openstudio::IddObjectType> ModelExplorer::getAllowableChildTypes()
  {
    return mTreeViewWidget->getAllowableChildTypes();
  }

  bool ModelExplorer::treeViewHasRowsToPaste()
  {
    return mTreeViewWidget->hasRowsToPaste();
  }

  bool ModelExplorer::classViewHasRowsToPaste()
  {
    return mClassViewWidget->hasRowsToPaste();
  }

  void ModelExplorer::viewWidgetAction(modeleditor::ViewWidget * viewWidget)
  {
    if(viewWidget->getModelDirty()){
      QTime time;
      time.start();
      viewWidget->loadModel();
      QString msg("Time to reload model already in memory: ");
      int millisecondsElapsed = time.elapsed();
      QString temp;
      temp.setNum(millisecondsElapsed);
      msg += temp;
      ///! uncomment to display model load time
      //emit showStatusBarMsg(msg, 5000);
    }
    mStackedWidget->setCurrentWidget(static_cast<QWidget *>(viewWidget));
  }

  TableModel* ModelExplorer::getTableModel()
  {
    return mClassViewWidget->getTableModel();
  }

  TreeModel* ModelExplorer::getTreeModel()
  {
    return mTreeViewWidget->getTreeModel();
  }

  ///! Slots
  void ModelExplorer::classAction()
  {
    mClassAction->setIcon(QIcon(":/images/class.png"));
    mTreeAction->setIcon(QIcon(":/images/tree_off.png"));

    viewWidgetAction(mClassViewWidget);
  }

  void ModelExplorer::treeAction()
  {
    mTreeAction->setIcon(QIcon(":/images/tree.png"));
    mClassAction->setIcon(QIcon(":/images/class_off.png"));

    viewWidgetAction(mTreeViewWidget);
  }

  void ModelExplorer::systemOutlinerAction()
  {
    mStackedWidget->setCurrentIndex(mStackedWidget->count()-1);
  }

  void ModelExplorer::setCurrentIndex(int index)
  {
    if(index != currentIndex()){
      mStackedWidget->setCurrentIndex(index);
    }
  }

  void ModelExplorer::on_ClassViewEventEnter()
  {
    mClassViewUnderMouse = true;
  }

  void ModelExplorer::on_ClassViewEventLeave()
  {
    mClassViewUnderMouse = false;
  }

  void ModelExplorer::on_TreeViewEventEnter()
  {
    mTreeViewUnderMouse = true;
  }

  void ModelExplorer::on_TreeViewEventLeave()
  {
    mTreeViewUnderMouse = false;
  }

  void ModelExplorer::toggleGUIDs()
  {
    mClassViewWidget->toggleGUIDs();
    mTreeViewWidget->toggleGUIDs();
    loadModel();
  }
  void ModelExplorer::showAllFields(bool state)
  {
    ViewWidget * widget = nullptr;
    widget = static_cast<ViewWidget *>(mStackedWidget->currentWidget());
    if(widget){
      widget->showAllFields(state);
    }
  }

  void ModelExplorer::createAllFields()
  {
    ViewWidget * widget = nullptr;
    widget = static_cast<ViewWidget *>(mStackedWidget->currentWidget());
    if(widget){
      widget->createAllFields();
    }
  }

  void ModelExplorer::setRecursive(bool state)
  {
    ViewWidget * widget = nullptr;
    widget = static_cast<ViewWidget *>(mStackedWidget->currentWidget());
    if(widget){
      widget->setRecursive(state);
    }
  }

} // namespace modeleditor
