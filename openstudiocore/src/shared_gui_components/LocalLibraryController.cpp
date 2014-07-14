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

#include "LocalLibraryController.hpp"

#include "HeaderViews.hpp"
#include "LocalLibraryView.hpp"
#include "MeasureDragData.hpp"
#include "MeasureManager.hpp"
#include "OSListController.hpp"
#include "OSListView.hpp"
#include "OSViewSwitcher.hpp"

#include "../openstudio_lib/OSItem.hpp"

#include "MeasureBadge.hpp"

#include "../utilities/bcl/LocalBCL.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"

#include <OpenStudio.hxx>

#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>
#include <QDomDocument>
#include <QDrag>
#include <QFile>
#include <QLabel>
#include <QMimeData>
#include <QSettings>
#include <QVariant>

#include <map>

namespace openstudio{
  
LocalLibraryController::LocalLibraryController(BaseApp *t_app, bool onlyShowModelMeasures)
  : QObject(),
    m_app(t_app),
    m_onlyShowModelMeasures(onlyShowModelMeasures)
{
  LOG(Debug, "Creating LocalLibraryController with base app " << t_app);
  QDomDocument doc("taxonomy");
  QFile file(":/shared_gui_components/taxonomy.xml");
  file.open(QIODevice::ReadOnly);
  doc.setContent(&file);
  file.close();

  QSharedPointer<LibraryTypeListController> libraryListController = createLibraryListController(doc,LocalLibrary::COMBINED);

  libraryView = new OSListView(true);
  libraryView->setContentsMargins(0,0,0,0);
  libraryView->setSpacing(0);
  libraryView->setDelegate(QSharedPointer<LibraryTypeItemDelegate>(new LibraryTypeItemDelegate(m_app)));
  libraryView->setListController(libraryListController); 

  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings m_settings(organizationName, applicationName);

  localLibraryView = new LocalLibraryView();

  showMeasures();

  bool bingo = false;

  bingo = connect(localLibraryView->addMeasureButton,SIGNAL(clicked()), this,SLOT(addMeasure()));

  OS_ASSERT(bingo);

  bingo = connect(localLibraryView->duplicateMeasureButton,SIGNAL(clicked()),this,SLOT(duplicateSelectedMeasure()));

  OS_ASSERT(bingo);

  bingo = connect(localLibraryView->myMeasuresFolderButton,SIGNAL(clicked()),this,SLOT(showMyMeasuresFolder()));

  OS_ASSERT(bingo);


  bingo = connect(localLibraryView->addBCLMeasureButton,SIGNAL(clicked()), this,SLOT(openBclDlg()));

  OS_ASSERT(bingo);
}

void LocalLibraryController::addMeasure()
{
  m_app->addMeasure();
}

void LocalLibraryController::duplicateSelectedMeasure()
{
  m_app->duplicateSelectedMeasure();
}

void LocalLibraryController::downloadUpdatedBCLMeasures()
{
  m_app->downloadUpdatedBCLMeasures();
}

void LocalLibraryController::openBclDlg()
{
  m_app->openBclDlg();
}

LocalLibraryController::~LocalLibraryController()
{
  if( localLibraryView ) { delete localLibraryView; } // TODO what is this?

  if( libraryView ) { delete libraryView; }
}

void LocalLibraryController::reset()
{
  libraryView->listController().objectCast<LibraryTypeListController>()->reset();
}

QPointer<LibraryItem> LocalLibraryController::selectedItem() const
{

  std::vector<QPointer<OSListItem> > items = libraryView->listController()->selectionController()->selectedItems();

  if( items.size() > 0 )
  {
    return qobject_cast<LibraryItem *>(items.front());    
  }

  return QPointer<LibraryItem>();
}


void LocalLibraryController::showMeasures()
{
  localLibraryView->mainViewSwitcher->setView(libraryView);

  m_app->updateSelectedMeasureState();
}

void LocalLibraryController::showMyMeasuresFolder()
{
  openstudio::path userMeasuresDir = BCLMeasure::userMeasuresDir();
  QString path = QDir::toNativeSeparators(toQString(userMeasuresDir));
  QDesktopServices::openUrl(QUrl("file:///" + path));
}

QSharedPointer<LibraryTypeListController> LocalLibraryController::createLibraryListController(const QDomDocument & taxonomy, LocalLibrary::LibrarySource source)
{
  QSharedPointer<LibraryTypeListController> libraryTypeListController = QSharedPointer<LibraryTypeListController>(new LibraryTypeListController());  

  QSharedPointer<LibraryTypeItem> libraryTypeItem = QSharedPointer<LibraryTypeItem>(new LibraryTypeItem("Measures"));

  libraryTypeListController->addItem(libraryTypeItem);

  QSharedPointer<LibraryGroupListController> libraryGroupListController = libraryTypeItem->libraryGroupListController();

  libraryGroupListController->setSelectionController(libraryTypeListController->selectionController());

  QDomElement docElem = taxonomy.documentElement();

  for( QDomNode n = docElem.firstChild(); !n.isNull(); n = n.nextSibling() )
  {
    QDomElement taxonomyElement = n.toElement();

    QString groupName = taxonomyElement.firstChildElement("name").text();

    QSharedPointer<LibraryGroupItem> groupItem = QSharedPointer<LibraryGroupItem>(new LibraryGroupItem(groupName, m_app));
    libraryGroupListController->addItem(QSharedPointer<LibraryGroupItem>(groupItem));
    QSharedPointer<LibrarySubGroupListController> subGroupListController = groupItem->librarySubGroupListController();

    subGroupListController->setSelectionController(libraryGroupListController->selectionController());

    QDomNodeList termElements = taxonomyElement.elementsByTagName("term");

    for( int j = 0; j < termElements.length(); j++ )
    {
      QDomElement termElement = termElements.at(j).toElement();

      QString subGroupName = termElement.firstChildElement("name").text();

      QString taxonomyTag = groupName + "." + subGroupName;

      QSharedPointer<LibrarySubGroupItem> item = QSharedPointer<LibrarySubGroupItem>(new LibrarySubGroupItem(subGroupName,taxonomyTag,source, m_app, m_onlyShowModelMeasures));
  
      subGroupListController->addItem(item);

      QSharedPointer<LibraryListController> libraryListController = item->libraryListController();

      libraryListController->setSelectionController(subGroupListController->selectionController());
    }
  }

  return libraryTypeListController;
}

LibraryTypeItem::LibraryTypeItem(const QString & name)
  : OSListItem(),
    m_name(name)
{
  m_libraryGroupListController = QSharedPointer<LibraryGroupListController>( new LibraryGroupListController() );
}

LibraryTypeItemDelegate::LibraryTypeItemDelegate(BaseApp *t_app)
  : m_app(t_app)
{
}

QWidget * LibraryTypeItemDelegate::view(QSharedPointer<OSListItem> dataSource) 
{ 
  if(QSharedPointer<LibraryTypeItem> item = dataSource.dynamicCast<LibraryTypeItem>())
  {
    auto groupCollapsibleView = new OSCollapsibleView(nullptr);

    auto header = new DarkGradientHeader(); 
    header->label->setText(item->name());
    groupCollapsibleView->setHeader(header);

    QSharedPointer<LibraryGroupListController> groupListController = item->libraryGroupListController();
    QSharedPointer<LibraryGroupItemDelegate> groupItemDelegate = QSharedPointer<LibraryGroupItemDelegate>(new LibraryGroupItemDelegate(m_app));

    auto groupListView = new OSListView();
    groupListView->setContentsMargins(0,0,0,0);
    groupListView->setSpacing(0);
    groupListView->setListController(groupListController);
    groupListView->setDelegate(groupItemDelegate);

    groupCollapsibleView->setContent(groupListView);

    groupCollapsibleView->setExpanded(true);

    return groupCollapsibleView;
  }
  else
  {
    return new QWidget();
  }
}

QSharedPointer<OSListItem> LibraryTypeListController::itemAt(int i)
{
  return m_items[i];
}

int LibraryTypeListController::count()
{
  return m_items.size();
}

void LibraryTypeListController::addItem(QSharedPointer<OSListItem> item)
{
  if( QSharedPointer<LibraryTypeItem> libraryTypeItem = item.objectCast<LibraryTypeItem>() )
  {
    m_items.push_back(libraryTypeItem);

    emit itemInserted(m_items.size() - 1);
  }
}

void LibraryTypeListController::reset()
{
  for( QList<QSharedPointer<LibraryTypeItem> >::iterator it = m_items.begin();
       it != m_items.end();
       ++it )
  {
    (*it)->libraryGroupListController()->reset();
  }
}

LibraryGroupItem::LibraryGroupItem(const QString & name, BaseApp *t_app)
  : OSListItem(),
    m_name(name)
{
  m_librarySubGroupListController = QSharedPointer<LibrarySubGroupListController>( new LibrarySubGroupListController(t_app) );
}

LibraryGroupItemDelegate::LibraryGroupItemDelegate(BaseApp *t_app)
  : m_app(t_app)
{
}

QWidget * LibraryGroupItemDelegate::view(QSharedPointer<OSListItem> dataSource) 
{ 
  if(QSharedPointer<LibraryGroupItem> item = dataSource.dynamicCast<LibraryGroupItem>())
  {
    auto groupCollapsibleView = new OSCollapsibleView(nullptr);

    auto header = new LibraryGroupItemHeader(); 
    header->label->setText(item->name());

    bool bingo = connect(item->librarySubGroupListController().data(),SIGNAL(libraryItemCountChanged(int)),header,SLOT(setCount(int)));
    OS_ASSERT(bingo);

    groupCollapsibleView->setHeader(header);

    QSharedPointer<LibrarySubGroupListController> subGroupListController = item->librarySubGroupListController();
    QSharedPointer<LibrarySubGroupItemDelegate> subGroupItemDelegate = QSharedPointer<LibrarySubGroupItemDelegate>(new LibrarySubGroupItemDelegate(m_app));

    auto subGroupListView = new OSListView();
    subGroupListView->setContentsMargins(0,0,0,0);
    subGroupListView->setSpacing(0);
    subGroupListView->setListController(subGroupListController);
    subGroupListView->setDelegate(subGroupItemDelegate);

    groupCollapsibleView->setContent(subGroupListView);

    return groupCollapsibleView;
  }
  else
  {
    return new QWidget();
  }
}

QSharedPointer<OSListItem> LibraryGroupListController::itemAt(int i)
{
  return m_items[i];
}

int LibraryGroupListController::count()
{
  return m_items.size();
}

void LibraryGroupListController::addItem(QSharedPointer<OSListItem> item)
{
  if( QSharedPointer<LibraryGroupItem> libraryGroupItem = item.objectCast<LibraryGroupItem>() )
  {
    m_items.push_back(libraryGroupItem);

    emit itemInserted(m_items.size() - 1);
  }
}

void LibraryGroupListController::reset()
{
  for( const auto & libraryGroupItem : m_items)
  {
    libraryGroupItem->librarySubGroupListController()->reset();    
  }
}

LibrarySubGroupItem::LibrarySubGroupItem(const QString & name, const QString & taxonomyTag, 
                                         LocalLibrary::LibrarySource source,
                                         BaseApp *t_app,
                                         bool onlyShowModelMeasures)
  : OSListItem(),
    m_app(t_app),
    m_name(name)
{
  m_libraryListController = QSharedPointer<LibraryListController>( new LibraryListController(taxonomyTag, source, m_app, onlyShowModelMeasures) );
}


LibrarySubGroupItemDelegate::LibrarySubGroupItemDelegate(BaseApp *t_app)
  : m_app(t_app)
{
}

QWidget * LibrarySubGroupItemDelegate::view(QSharedPointer<OSListItem> dataSource) 
{ 
  if(QSharedPointer<LibrarySubGroupItem> item = dataSource.dynamicCast<LibrarySubGroupItem>())
  {
    auto subGroupCollapsibleView = new OSCollapsibleView(nullptr);

    auto header = new LibrarySubGroupItemHeader(); 

    header->label->setText(item->name());

    bool bingo = connect(item->libraryListController().data(),SIGNAL(countChanged(int)),header,SLOT(setCount(int)));
    OS_ASSERT(bingo);

    subGroupCollapsibleView->setHeader(header);

    QSharedPointer<LibraryListController> libraryListController = item->libraryListController();
    QSharedPointer<LibraryItemDelegate> measureLibraryItemDelegate = QSharedPointer<LibraryItemDelegate>( new LibraryItemDelegate(m_app) );

    auto measureListView = new OSListView();
    measureListView->setContentsMargins(5,0,5,0);
    measureListView->setSpacing(5);
    measureListView->setListController(libraryListController);
    measureListView->setDelegate(measureLibraryItemDelegate);
    
    subGroupCollapsibleView->setContent(measureListView);

    return subGroupCollapsibleView;
  }
  else
  {
    return new QWidget();
  }
}

LibrarySubGroupListController::LibrarySubGroupListController(BaseApp *t_app)
  : m_app(t_app)
{
}

QSharedPointer<OSListItem> LibrarySubGroupListController::itemAt(int i)
{
  return m_items[i];
}

int LibrarySubGroupListController::count()
{
  return m_items.size();
}

void LibrarySubGroupListController::addItem(QSharedPointer<OSListItem> item)
{
  if( QSharedPointer<LibrarySubGroupItem> librarySubGroupItem = item.objectCast<LibrarySubGroupItem>() )
  {
    m_items.push_back(librarySubGroupItem);

    emit itemInserted(m_items.size() - 1);
  }
}

void LibrarySubGroupListController::reset()
{
  int count = 0;

  for( const auto & librarySubGroupItem : m_items)
  {
    librarySubGroupItem->libraryListController()->reset();
    count += librarySubGroupItem->libraryListController()->count();
  }

  emit libraryItemCountChanged(count);
}

LibraryItem::LibraryItem(const BCLMeasure & bclMeasure, LocalLibrary::LibrarySource source, BaseApp *t_app):
  OSListItem(),
  m_source(source),
  m_bclMeasure(bclMeasure),
  m_app(t_app)
{
  std::string componentVersion;
  for (const BCLFileReference & file : bclMeasure.files()) {
    if (file.usageType() == "script" && file.softwareProgram() == "OpenStudio"){
      componentVersion = file.softwareProgramVersion();
      break;
    }
  }
  if (componentVersion.empty() || VersionString(componentVersion) > VersionString(openStudioVersion())){
    m_available = false;
  }else{
    m_available = true;
  }
}

void LibraryItem::dragItem(const OSDragPixmapData & dragPixmapData)
{
  MeasureDragData measureDragData(m_bclMeasure.uuid());

  auto drag = new QDrag(m_app->mainWidget());

  auto mimeData = new QMimeData;
  mimeData->setData(MeasureDragData::mimeType(m_bclMeasure.measureType()), measureDragData.data());
  drag->setMimeData(mimeData);

  drag->setPixmap(dragPixmapData.pixmap);
  drag->setHotSpot(dragPixmapData.hotSpot);

  drag->exec(Qt::CopyAction);
}

LibraryItemDelegate::LibraryItemDelegate(BaseApp *t_app)
  : m_app(t_app)
{
}

QWidget * LibraryItemDelegate::view(QSharedPointer<OSListItem> dataSource) 
{ 

  if( QSharedPointer<LibraryItem> libraryItem = dataSource.objectCast<LibraryItem>() )
  {
    MeasureType measureType = libraryItem->m_bclMeasure.measureType();

    // NOTE: replaces needed to trim unwanted curly braces
    QString measureUUID = libraryItem->m_bclMeasure.uuid().toString().replace("{", "").replace("}", "").toStdString().c_str();

    std::vector<std::string> localUUIDs = (LocalBCL::instance().measureUids());

    auto widget = new LibraryItemView();

    if (measureType == MeasureType::ModelMeasure){
      widget->m_measureTypeBadge->setPixmap(QPixmap(":/shared_gui_components/images/openstudio_measure_icon.png").scaled(25,25,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
      widget->m_measureTypeBadge->setVisible(true);
    }else if (measureType == MeasureType::EnergyPlusMeasure){
      widget->m_measureTypeBadge->setPixmap(QPixmap(":/shared_gui_components/images/energyplus_measure_icon.png").scaled(25,25,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
      widget->m_measureTypeBadge->setVisible(true);
    }else if (measureType == MeasureType::ReportingMeasure){
      widget->m_measureTypeBadge->setPixmap(QPixmap(":/shared_gui_components/images/report_measure_icon.png").scaled(25,25,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
      widget->m_measureTypeBadge->setVisible(true);
    }

    if(libraryItem->m_source == LocalLibrary::BCL){
      widget->m_measureBadge->setMeasureBadgeType(MeasureBadgeType::BCLMeasure);
    }

    // Name

    widget->label->setText(libraryItem->name());
    widget->setToolTip(libraryItem->name());

    // Drag
    
    bool bingo = connect(widget,SIGNAL(dragRequested(const OSDragPixmapData &)),libraryItem.data(),SLOT(dragItem(const OSDragPixmapData &)));

    OS_ASSERT(bingo);

    // Selection

    widget->setHasEmphasis(libraryItem->isSelected());

    bingo = connect(widget,SIGNAL(clicked()),libraryItem.data(),SLOT(toggleSelected()));

    OS_ASSERT(bingo);

    bingo = connect(libraryItem.data(),SIGNAL(selectedChanged(bool)),widget,SLOT(setHasEmphasis(bool)));

    OS_ASSERT(bingo);

    bingo = connect(libraryItem.data(),SIGNAL(selectedChanged(bool)),this,SLOT(selectedChanged()));

    OS_ASSERT(bingo);

    return widget;
  }
  
  return new QWidget();
}

void LibraryItemDelegate::selectedChanged()
{
  m_app->updateSelectedMeasureState();
}

LibraryListController::LibraryListController(const QString & taxonomyTag, 
                                             LocalLibrary::LibrarySource source, 
                                             BaseApp *t_app,
                                             bool onlyShowModelMeasures)
  : OSListController(),
    m_app(t_app),
    m_taxonomyTag(taxonomyTag),
    m_source(source),
    m_onlyShowModelMeasures(onlyShowModelMeasures)
{
  createItems();
}

QSharedPointer<OSListItem> LibraryListController::itemAt(int i)
{
  if( i >= 0 && i < (int)m_items.size() )
  {
    return m_items[i];
  }

  // DLM: I was just fixing build error, Kyle is this correct?
  return QSharedPointer<OSListItem>();
}

int LibraryListController::count()
{
  return m_items.size();
}

// simple struct for sorting measures
struct MeasureSorter{
  // sort by type and then name
  bool operator()(const BCLMeasure& lhs, const BCLMeasure& rhs){
    if (lhs.measureType() != rhs.measureType()){
      return lhs.measureType() < rhs.measureType();
    }
    return (lhs.name() < rhs.name());
  }
};

bool nonModelMeasureToRemove(const BCLMeasure& measure){
  return (measure.measureType() != MeasureType::ModelMeasure);
}

void LibraryListController::createItems()
{
  m_items.clear();

  std::vector<BCLMeasure> measures;

  if( m_source == LocalLibrary::USER )
  {
    measures = m_app->measureManager().myMeasures();
  }
  else if( m_source == LocalLibrary::BCL )
  {
    measures = m_app->measureManager().bclMeasures();
  }
  else if( m_source == LocalLibrary::COMBINED )
  {
    measures = m_app->measureManager().combinedMeasures();
  }
  else
  {
    // should never get here
    OS_ASSERT(false);
  }

  // filter measures
  if(m_onlyShowModelMeasures){
    measures.erase( std::remove_if( measures.begin(), measures.end(), nonModelMeasureToRemove ), measures.end() ); 
  }
  
  // sort measures
  std::sort(measures.begin(), measures.end(), MeasureSorter());

  // create items
  openstudio::path userMeasuresDir = BCLMeasure::userMeasuresDir();
  for( const auto & measure : measures )
  {
    if( m_taxonomyTag.compare(QString::fromStdString(measure.taxonomyTag()),Qt::CaseInsensitive) == 0 )
    {
      // cannot use measures that rely on SketchUpAPI in this application
      if (measure.usesSketchUpAPI()){
        continue;
      }

      LocalLibrary::LibrarySource source = m_source;
      if (source == LocalLibrary::COMBINED){
        // check if this measure is in the my measures directory
        if (userMeasuresDir == measure.directory().parent_path()){
          source = LocalLibrary::USER;
        }else{
          source = LocalLibrary::BCL;
        }
      }

      QSharedPointer<LibraryItem> item = QSharedPointer<LibraryItem>(new LibraryItem(measure, source, m_app));

      item->setController(this);

      // Don't show measures that were created with a newer version of OpenStudio
      if (item->isAvailable()){
        m_items.push_back(item);
      }
    }
  }
}

void LibraryListController::reset()
{
  createItems();

  emit modelReset();

  emit countChanged(count());
}

} // openstudio

