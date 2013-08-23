/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include <openstudio_lib/UtilityBillsView.hpp>

#include <openstudio_lib/OSItem.hpp>
#include <openstudio_lib/ModelObjectItem.hpp>
#include <openstudio_lib/UtilityBillFuelTypeListView.hpp>
#include <openstudio_lib/UtilityBillAllFuelTypesListView.hpp>

#include "../shared_gui_components/Buttons.hpp"
#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSDoubleEdit.hpp"
#include "../shared_gui_components/OSIntegerEdit.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSUnsignedEdit.hpp"

#include <model/Model_Impl.hpp>
#include <model/RunPeriod.hpp>
#include <model/RunPeriod_Impl.hpp>
#include <model/UtilityBill_Impl.hpp>
#include <model/WeatherFile.hpp>
#include <model/WeatherFile_Impl.hpp>
#include <model/YearDescription.hpp>
#include <model/YearDescription_Impl.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/time/Date.hpp>

#include <boost/bind.hpp>

#include <QBoxLayout>
#include <QButtonGroup>
#include <QDate>
#include <QDateEdit>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QStackedWidget>
#include <QString>

#define OS_UTILITY_WIDTH 150
#define OS_EDIT_WIDTH 300

namespace openstudio {

// "Utility Bills"

UtilityBillsView::UtilityBillsView(const openstudio::model::Model& model, QWidget * parent)
                     : ModelSubTabView(new UtilityBillAllFuelTypesListView(UtilityBillsView::utilityBillFuelTypesAndNames(), 
                                                                           model, 
                                                                           true, 
                                                                           OSItem::COLLAPSIBLE_LIST_HEADER, 
                                                                           parent),
                                       new UtilityBillsInspectorView(model,parent),
                                       parent)
{
}

std::vector<std::pair<FuelType, std::string> > UtilityBillsView::utilityBillFuelTypesAndNames()
{
  std::vector<std::pair<FuelType, std::string> > result;
  result.push_back(std::make_pair<FuelType, std::string>(FuelType::Electricity, "Electric Utility Bill"));
  result.push_back(std::make_pair<FuelType, std::string>(FuelType::Gas, "Gas Utility Bill"));
  result.push_back(std::make_pair<FuelType, std::string>(FuelType::DistrictHeating, "District Heating Utility Bill"));
  result.push_back(std::make_pair<FuelType, std::string>(FuelType::DistrictCooling, "District Cooling Utility Bill"));
  result.push_back(std::make_pair<FuelType, std::string>(FuelType::Gasoline, "Gasoline Utility Bill"));
  result.push_back(std::make_pair<FuelType, std::string>(FuelType::Diesel, "Diesel Utility Bill"));
  result.push_back(std::make_pair<FuelType, std::string>(FuelType::FuelOil_1, "Fuel Oil #1 Utility Bill"));
  result.push_back(std::make_pair<FuelType, std::string>(FuelType::FuelOil_2, "Fuel Oil #2 Utility Bill"));
  result.push_back(std::make_pair<FuelType, std::string>(FuelType::Propane, "Propane Utility Bill"));
  result.push_back(std::make_pair<FuelType, std::string>(FuelType::Water, "Water Utility Bill"));
  result.push_back(std::make_pair<FuelType, std::string>(FuelType::Steam, "Steam Utility Bill"));
  result.push_back(std::make_pair<FuelType, std::string>(FuelType::EnergyTransfer, "Energy Transfer Utility Bill"));

  return result;
}


//**********************************************************************************************************


UtilityBillsInspectorView::UtilityBillsInspectorView(const model::Model & model,
                                                     bool addScrollArea,
                                                     QWidget * parent)
  : ModelObjectInspectorView(model,
                             true,
                             parent),
    m_billFormat(STARTDATE_ENDDATE),
    m_showPeak(0),
    m_buttonGroup(0),
    m_name(0),
    m_consumptionUnits(0),
    m_peakDemandUnits(0),
    m_windowTimesteps(0),
    m_addBillingPeriod(0),
    m_billGridLayout(0),
    m_billGridLayoutWidget(0),
    m_hiddenWidgetIndex(0),
    m_warningWidgetIndex(0),
    m_visibleWidgetIndex(0)
{
  createWidgets();
}

boost::optional<QString> UtilityBillsInspectorView::runPeriodDates()
{
  boost::optional<QString> beginAndEndDates;

  boost::optional<int> calendarYear;
  boost::optional<model::YearDescription> yearDescription = m_model.yearDescription();
  if(yearDescription){
    calendarYear = yearDescription.get().calendarYear();
    if(calendarYear){
      boost::optional<model::WeatherFile> weatherFile = m_model.weatherFile();
      if(weatherFile){
        boost::optional<model::RunPeriod> runPeriod = m_model.getOptionalUniqueModelObject<model::RunPeriod>();
        if(runPeriod.is_initialized()){
          int beginMonth = runPeriod.get().getBeginMonth();
          int beginDayOfMonth = runPeriod.get().getBeginDayOfMonth();
          int beginYear = calendarYear.get();
          int endMonth = runPeriod.get().getEndMonth();
          int endDayOfMonth = runPeriod.get().getEndDayOfMonth();
          int endYear = calendarYear.get();

          // Check for wrap-around runPeriod
          if( (beginMonth > endMonth) || (beginMonth == endMonth && beginDayOfMonth > endDayOfMonth)){
            endYear++;
          }
          
          QString string;
          QString number;

          string += "Start Date ";
          string += number.setNum(beginMonth);
          string += "/";
          string += number.setNum(beginDayOfMonth);
          string += "/";
          string += number.setNum(beginYear);
          string += "   End Date ";
          string += number.setNum(endMonth);
          string += "/";
          string += number.setNum(endDayOfMonth);
          string += "/";
          string += number.setNum(endYear);

          beginAndEndDates = string;
        }
      } 
    } 
  }
  return beginAndEndDates;
}

void UtilityBillsInspectorView::createWidgets()
{
  QWidget* hiddenWidget = new QWidget();
  m_hiddenWidgetIndex = this->stackedWidget()->insertWidget(0, hiddenWidget);

  QWidget* warningWidget = new QWidget();
  m_warningWidgetIndex = this->stackedWidget()->addWidget(warningWidget);

  QWidget* visibleWidget = new QWidget();
  m_visibleWidgetIndex = this->stackedWidget()->addWidget(visibleWidget);

  this->stackedWidget()->setCurrentIndex(m_hiddenWidgetIndex);

  bool isConnected = false;

  QLabel * label = NULL;

  QVBoxLayout * vLayout = NULL;

  // Warning widget if no weather file

  vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  vLayout->setSpacing(10);

  label = new QLabel();
  label->setPixmap(QPixmap(":/images/utility_calibration_warning.png"));
  label->setAlignment(Qt::AlignCenter);
  vLayout->addWidget(label,0,Qt::AlignCenter);
  warningWidget->setLayout(vLayout);
  warningWidget->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

  // Regular inspector body

  QVBoxLayout * mainLayout = new QVBoxLayout();
  mainLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  mainLayout->setContentsMargins(10,10,10,10);
  mainLayout->setSpacing(20);
  visibleWidget->setLayout(mainLayout);

  vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  vLayout->setSpacing(10);

  m_buttonGroup = new QButtonGroup(this);

  isConnected = connect(m_buttonGroup, SIGNAL(buttonClicked(int)),
    this, SLOT(deleteBillingPeriod(int)));
  OS_ASSERT(isConnected);
  
  // Name

  vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  vLayout->setSpacing(10);

  label = new QLabel();
  label->setText("Name");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_name = new OSLineEdit2();
  m_name->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_name);

  mainLayout->addLayout(vLayout);

  // Consumption Units and Peak Demand Units

  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  gridLayout->setContentsMargins(0,0,0,0);
  gridLayout->setSpacing(10);

  vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  vLayout->setSpacing(10);

  m_consumptionUnitsLabel = new QLabel();
  m_consumptionUnitsLabel->setText("Consumption Units");
  m_consumptionUnitsLabel->setObjectName("H2");
  vLayout->addWidget(m_consumptionUnitsLabel);

  m_consumptionUnits = new OSComboBox2();
  m_consumptionUnits->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_consumptionUnits);

  gridLayout->addLayout(vLayout,0,0,Qt::AlignLeft | Qt::AlignTop);

  vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  vLayout->setSpacing(10);

  m_peakDemandUnitsLabel = new QLabel();
  m_peakDemandUnitsLabel->setText("Peak Demand Units");
  m_peakDemandUnitsLabel->setObjectName("H2");
  vLayout->addWidget(m_peakDemandUnitsLabel);

  m_peakDemandUnits = new OSComboBox2();
  m_peakDemandUnits->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_peakDemandUnits);

  gridLayout->addLayout(vLayout,0,1,Qt::AlignLeft | Qt::AlignTop);

  vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  vLayout->setSpacing(10);

  m_windowTimestepsLabel = new QLabel();
  m_windowTimestepsLabel->setText("Peak Demand Window Timesteps");
  m_windowTimestepsLabel->setObjectName("H2");
  vLayout->addWidget(m_windowTimestepsLabel);

  m_windowTimesteps = new OSUnsignedEdit2();
  m_windowTimesteps->setFixedWidth(OS_UTILITY_WIDTH);
  vLayout->addWidget(m_windowTimesteps);

  gridLayout->addLayout(vLayout,0,2,Qt::AlignLeft | Qt::AlignTop);

  gridLayout->setColumnStretch(100,100);
  
  mainLayout->addLayout(gridLayout);

  // Run Period

  vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  vLayout->setSpacing(10);

  label = new QLabel();
  label->setText("Run Period");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  label = new QLabel();
  label->setText("N/A");
  vLayout->addWidget(label);

  mainLayout->addLayout(vLayout);

  boost::optional<QString> dates;
  dates = runPeriodDates();
  if(dates) label->setText(*dates);

  // Billing Period

  vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  vLayout->setSpacing(5);

  label = new QLabel();
  label->setText("Billing Period");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  label = new QLabel();
  label->setText("Select the best match for you utility bill");
  vLayout->addWidget(label);

  QButtonGroup * buttonGroup = new QButtonGroup(this);

  isConnected = connect(buttonGroup, SIGNAL(buttonClicked(int)),
    this, SLOT(setBillFormat(int)));
  OS_ASSERT(isConnected);

  QRadioButton * radioButton = NULL; 

  radioButton = new QRadioButton("Start Date and End Date");
  buttonGroup->addButton(radioButton,0);
  vLayout->addWidget(radioButton);

  radioButton = new QRadioButton("Start Date and Number of Days in Billing Period");
  buttonGroup->addButton(radioButton,1);
  vLayout->addWidget(radioButton);

  radioButton = new QRadioButton("End Date and Number of Days in Billing Period");
  buttonGroup->addButton(radioButton,2);
  vLayout->addWidget(radioButton);

  buttonGroup->button(0)->click();

  mainLayout->addLayout(vLayout);

  // Bill Widget Grid Layouts

  m_billGridLayout = new QGridLayout();
  m_billGridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  m_billGridLayout->setContentsMargins(0,0,0,0);
  m_billGridLayout->setSpacing(10);

  m_billGridLayoutWidget = new QWidget;
  m_billGridLayoutWidget->setLayout(m_billGridLayout);

  mainLayout->addWidget(m_billGridLayoutWidget);

  // Add Bill Button

  QHBoxLayout * hLayout = new QHBoxLayout();
  hLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  m_addBillingPeriod = new QPushButton();
  m_addBillingPeriod->setFlat(true);
  m_addBillingPeriod->setObjectName("AddButton");
  m_addBillingPeriod->setToolTip("Add new object");
  m_addBillingPeriod->setFixedSize(24,24);
  hLayout->addWidget(m_addBillingPeriod);

  isConnected = connect(m_addBillingPeriod, SIGNAL(clicked(bool)),
    this, SLOT(addBillingPeriod(bool)));
  OS_ASSERT(isConnected);

  label = new QLabel();
  label->setObjectName("H2");
  label->setText("Add New Billing Period");
  hLayout->addWidget(label,0, Qt::AlignLeft | Qt::AlignVCenter);
  mainLayout->addLayout(hLayout);
}

void UtilityBillsInspectorView::attach(openstudio::model::UtilityBill & utilityBill)
{
  m_utilityBill = utilityBill;

  if(m_name){
  m_name->bind(
    *m_utilityBill,
    OptionalStringGetter(boost::bind(&model::UtilityBill::name,m_utilityBill.get_ptr(),true)),
    boost::optional<StringSetter>(boost::bind(&model::UtilityBill::setName,m_utilityBill.get_ptr(),_1)));
  }

  if(m_consumptionUnits){
  m_consumptionUnits->bind(
      *m_utilityBill,
      boost::bind(&model::UtilityBill::consumptionUnitValues,m_utilityBill.get_ptr()),
      OptionalStringGetter(boost::bind(&model::UtilityBill::consumptionUnit,m_utilityBill.get_ptr())),
      boost::optional<StringSetter>(boost::bind(&model::UtilityBill::setConsumptionUnit,m_utilityBill.get_ptr(),_1)));
  }

  if(m_peakDemandUnits){
  if (m_utilityBill->fuelType() == FuelType::Electricity){
    m_peakDemandUnitsLabel->setVisible(true);
    m_peakDemandUnits->setVisible(true);
    m_peakDemandUnits->bind(
        *m_utilityBill,
        boost::bind(&model::UtilityBill::peakDemandUnitValues,m_utilityBill.get_ptr()),
        OptionalStringGetter(boost::bind(&model::UtilityBill::peakDemandUnit,m_utilityBill.get_ptr())),
        boost::optional<StringSetter>(boost::bind(&model::UtilityBill::setPeakDemandUnit,m_utilityBill.get_ptr(),_1)));
  }

    if(m_windowTimesteps){
    m_windowTimestepsLabel->setVisible(true);
    m_windowTimesteps->setVisible(true);
    m_windowTimesteps->bind(
      *m_utilityBill,
      OptionalUnsignedGetter(boost::bind(&model::UtilityBill::timestepsInPeakDemandWindow,m_utilityBill.get_ptr())),
      boost::optional<UnsignedSetter>(boost::bind(&model::UtilityBill::setTimestepsInPeakDemandWindow,m_utilityBill.get_ptr(),_1)));
    }
  }else{
    if(m_peakDemandUnits && m_windowTimesteps){
    m_peakDemandUnitsLabel->setVisible(false);
    m_peakDemandUnits->setVisible(false);
    m_windowTimestepsLabel->setVisible(false);
    m_windowTimesteps->setVisible(false);
    }
  }
  
  setCorrectCurrentIndex();

  deleteBillingPeriods();
  addBillingPeriods();
}

void UtilityBillsInspectorView::setCorrectCurrentIndex()
{
  if(runPeriodDates()){
    this->stackedWidget()->setCurrentIndex(m_visibleWidgetIndex);
  } else {
    this->stackedWidget()->setCurrentIndex(m_warningWidgetIndex);
  }
}

void UtilityBillsInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(m_hiddenWidgetIndex);

  if(m_name){
    m_name->unbind();
  }

  if(m_consumptionUnits){
    m_consumptionUnits->unbind();
  }

  if(m_peakDemandUnits){
    m_peakDemandUnits->unbind();
  }
   
  if(m_windowTimesteps){
    m_windowTimesteps->unbind();
  }
}

void UtilityBillsInspectorView::refresh()
{
}

void UtilityBillsInspectorView::onSelectItem(OSItem *item)
{
  ModelObjectItem* modelObjectItem = qobject_cast<ModelObjectItem*>(item);
  OS_ASSERT(modelObjectItem);
  onSelectModelObject(modelObjectItem->modelObject());
}

void UtilityBillsInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void UtilityBillsInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::UtilityBill utilityBill = modelObject.cast<model::UtilityBill>();
  attach(utilityBill);
  refresh();
}

void UtilityBillsInspectorView::onUpdate()
{
  refresh();
}

void UtilityBillsInspectorView::showAddButton()
{
  m_addBillingPeriod->show();
}

void UtilityBillsInspectorView::hideAddButton()
{
  m_addBillingPeriod->hide();
}

void UtilityBillsInspectorView::enableAddButton()
{
  m_addBillingPeriod->setEnabled(true);
}

void UtilityBillsInspectorView::disableAddButton()
{
  m_addBillingPeriod->setEnabled(false);
}

void UtilityBillsInspectorView::addBillingPeriods()
{
  if(m_utilityBill.is_initialized()){
    std::vector<model::BillingPeriod> billingPeriods = m_utilityBill.get().billingPeriods();
    for(unsigned i = 0; i < billingPeriods.size(); i++){
      addBillingPeriod(billingPeriods.at(i));
    }
  }
}

void UtilityBillsInspectorView::addBillingPeriod(model::BillingPeriod & billingPeriod)
{
  if(m_utilityBill.is_initialized()){
    BillingPeriodWidget * billingPeriodWidget = new BillingPeriodWidget(m_billGridLayout,billingPeriod,m_utilityBill.get().fuelType(),m_billFormat);
    m_buttonGroup->addButton(billingPeriodWidget->m_deleteBillWidget,m_buttonGroup->buttons().size());
  }
}

void UtilityBillsInspectorView::deleteBillingPeriods()
{
  if(m_billGridLayout ==0) return;

  QLayoutItem * child;
  while((child = m_billGridLayout->takeAt(0)) != 0 ){
    QWidget* widget = child->widget();
    if (widget){
      delete widget;
    }
    delete child;
  }
  delete m_billGridLayout;
  m_billGridLayout = new QGridLayout();
  m_billGridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  m_billGridLayout->setContentsMargins(0,0,0,0);
  m_billGridLayout->setSpacing(10);
  m_billGridLayoutWidget->setLayout(m_billGridLayout);
}

////// SLOTS ///////

void UtilityBillsInspectorView::addUtilityBill()
{
  m_model; // TODO

}

void UtilityBillsInspectorView::addBillingPeriod(bool checked)
{
  if(m_utilityBill.is_initialized()){
    addBillingPeriod(m_utilityBill.get().addBillingPeriod());
  }
}

void UtilityBillsInspectorView::deleteBillingPeriod(int idx)
{
  if(m_utilityBill.is_initialized()){
    std::vector<model::BillingPeriod> billingPeriods =  m_utilityBill.get().billingPeriods();
    model::BillingPeriod billingPeriod = billingPeriods.at(idx);
    unsigned index = billingPeriod.groupIndex();
    m_utilityBill.get().eraseExtensibleGroup(index);
    deleteBillingPeriods();
    addBillingPeriods();
  }
}

void UtilityBillsInspectorView::setBillFormat(BillFormat billFormat)
{
  m_billFormat = billFormat;
  deleteBillingPeriods();
  addBillingPeriods();
}

void UtilityBillsInspectorView::setBillFormat(int index)
{
  if(index == 0){
    m_billFormat = STARTDATE_ENDDATE;
  }
  else if(index == 1){
    m_billFormat = STARTDATE_NUMDAYS;
  }
  else if(index == 2){
    m_billFormat = ENDDATE_NUMDAYS;
  }
  else{
    OS_ASSERT(false);
  }
  setBillFormat(m_billFormat);
}

//**********************************************************************************************************


BillingPeriodWidget::BillingPeriodWidget(QGridLayout * gridLayout,
  model::BillingPeriod billingPeriod,
  FuelType fuelType,
  BillFormat billFormat,
  QWidget * parent)
  : QWidget(parent),
  m_billingPeriod(billingPeriod),
  m_startDateEdit(0),
  m_endDateEdit(0),
  m_billingPeriodIntEdit(0),
  m_energyUseDoubleEdit(0),
  m_peakDoubleEdit(0),
  m_costDoubleEdit(0)
{
  OS_ASSERT(m_billingPeriod.is_initialized());

  createWidgets(gridLayout,fuelType,billFormat);

  attach(*m_billingPeriod);
}

void BillingPeriodWidget::createWidgets(QGridLayout * gridLayout,
  FuelType fuelType,
  BillFormat billFormat)
{
  int columnIndex = 0;

  int rowIndex = gridLayout->rowCount();

  if(rowIndex == 1){
    if(billFormat == STARTDATE_ENDDATE){
      getStartDateLabel(gridLayout, rowIndex, columnIndex++);
      getEndDateLabel(gridLayout, rowIndex, columnIndex++);
    }
    else if(billFormat == STARTDATE_NUMDAYS){
      getStartDateLabel(gridLayout, rowIndex, columnIndex++);
      getBillingPeriodLabel(gridLayout, rowIndex, columnIndex++);
    }
    else if(billFormat == ENDDATE_NUMDAYS){
      getBillingPeriodLabel(gridLayout, rowIndex, columnIndex++);
      getEndDateLabel(gridLayout, rowIndex, columnIndex++);      
    }
    else{
      OS_ASSERT(false);
    }

    columnIndex++;

    getLabel(gridLayout,rowIndex,columnIndex++,QString("Energy Use (kWh)"));
    if(fuelType == FuelType::Electricity){
      getLabel(gridLayout,rowIndex,columnIndex++,QString("Peak (kW)"));
    }
    getLabel(gridLayout,rowIndex,columnIndex++,QString("Cost"));
    getLabel(gridLayout,rowIndex,columnIndex++,QString(""));

    gridLayout->setColumnStretch(1000,1000);
    
    columnIndex = 0;

    rowIndex++;
  }
  
  if(billFormat == STARTDATE_ENDDATE){
    getStartDateCalendar(gridLayout, rowIndex, columnIndex++);
    getEndDateCalendar(gridLayout, rowIndex, columnIndex++);
  }
  else if(billFormat == STARTDATE_NUMDAYS){
    getStartDateCalendar(gridLayout, rowIndex, columnIndex++);
    getBillingPeriodLineEdit(gridLayout, rowIndex, columnIndex++);
  }
  else if(billFormat == ENDDATE_NUMDAYS){
    getEndDateCalendar(gridLayout, rowIndex, columnIndex++);
    getBillingPeriodLineEdit(gridLayout, rowIndex, columnIndex++);
  }
  else{
    OS_ASSERT(false);
  }

  columnIndex++;

  m_energyUseDoubleEdit = new OSDoubleEdit2();
  m_energyUseDoubleEdit->setFixedWidth(OS_UTILITY_WIDTH);
  gridLayout->addWidget(m_energyUseDoubleEdit,rowIndex,columnIndex++,Qt::AlignLeft | Qt::AlignTop);
  
  if(fuelType == FuelType::Electricity){
    m_peakDoubleEdit = new OSDoubleEdit2();
    m_peakDoubleEdit->setFixedWidth(OS_UTILITY_WIDTH);
    gridLayout->addWidget(m_peakDoubleEdit,rowIndex,columnIndex++,Qt::AlignLeft | Qt::AlignTop);
  }

  m_costDoubleEdit = new OSDoubleEdit2();
  m_costDoubleEdit->setFixedWidth(OS_UTILITY_WIDTH);
  gridLayout->addWidget(m_costDoubleEdit,rowIndex,columnIndex++,Qt::AlignLeft | Qt::AlignTop);

  m_deleteBillWidget = new SofterRemoveButton();
  gridLayout->addWidget(m_deleteBillWidget,rowIndex,columnIndex++,Qt::AlignLeft | Qt::AlignTop);

  bool isConnected = false;

  if(m_startDateEdit){
    isConnected = connect(m_startDateEdit,SIGNAL(dateChanged(const QDate &)),
      this,SLOT(startDateChanged(const QDate &)));
    OS_ASSERT(isConnected);
  }

  if(m_endDateEdit){
    isConnected = connect(m_endDateEdit,SIGNAL(dateChanged(const QDate &)),
      this,SLOT(endDateChanged(const QDate &)));
    OS_ASSERT(isConnected);
  }

  // TODO remove below and uncomment in modelObjectChanged()
  if(m_startDateEdit){
    Date startDate = m_billingPeriod.get().startDate();
    bool wasBlocked = m_startDateEdit->blockSignals(true);
    m_startDateEdit->setDate(QDate(startDate.year(),month(startDate.monthOfYear()),startDate.dayOfMonth()));
    m_startDateEdit->blockSignals(wasBlocked);
  }

  if(m_endDateEdit){
    Date endDate = m_billingPeriod.get().endDate();
    bool wasBlocked = m_endDateEdit->blockSignals(true);
    m_endDateEdit->setDate(QDate(endDate.year(),month(endDate.monthOfYear()),endDate.dayOfMonth()));
    m_endDateEdit->blockSignals(wasBlocked);
  }

}

void BillingPeriodWidget::attach(openstudio::model::BillingPeriod & billingPeriod)
{
  m_billingPeriod = billingPeriod;

  if(m_billingPeriodIntEdit){
    m_billingPeriodIntEdit->bind(
      *m_billingPeriod,
      OptionalIntGetter(boost::bind(&model::BillingPeriod::numberOfDays,m_billingPeriod.get_ptr())),
      boost::optional<IntSetter>(boost::bind(&model::BillingPeriod::setNumberOfDays,m_billingPeriod.get_ptr(),_1)));
  }

  if(m_energyUseDoubleEdit){
    m_energyUseDoubleEdit->bind(
      *m_billingPeriod,
      OptionalDoubleGetter(boost::bind(&model::BillingPeriod::consumption,m_billingPeriod.get_ptr())),
      boost::optional<DoubleSetter>(boost::bind(&model::BillingPeriod::setConsumption,m_billingPeriod.get_ptr(),_1)),
      boost::optional<NoFailAction>(boost::bind(&model::BillingPeriod::resetConsumption,m_billingPeriod.get_ptr())));
  }

  if(m_peakDoubleEdit){
      m_peakDoubleEdit->bind(
      *m_billingPeriod,
      OptionalDoubleGetter(boost::bind(&model::BillingPeriod::peakDemand,m_billingPeriod.get_ptr())),
      boost::optional<DoubleSetter>(boost::bind(&model::BillingPeriod::setPeakDemand,m_billingPeriod.get_ptr(),_1)),
      boost::optional<NoFailAction>(boost::bind(&model::BillingPeriod::resetPeakDemand,m_billingPeriod.get_ptr())));
  }

  if(m_costDoubleEdit){
    m_costDoubleEdit->bind(
      *m_billingPeriod,
      OptionalDoubleGetter(boost::bind(&model::BillingPeriod::totalCost,m_billingPeriod.get_ptr())),
      boost::optional<DoubleSetter>(boost::bind(&model::BillingPeriod::setTotalCost,m_billingPeriod.get_ptr(),_1)),
      boost::optional<NoFailAction>(boost::bind(&model::BillingPeriod::resetTotalCost,m_billingPeriod.get_ptr())));
  }

  model::ModelObject modelObject = m_billingPeriod->getObject<openstudio::model::ModelObject>();
  bool isConnected = connect( modelObject.getImpl<openstudio::model::detail::ModelObject_Impl>().get(),SIGNAL(onChange()),
                              this,SLOT(modelObjectChanged()) );
  OS_ASSERT(isConnected);

  if(m_startDateEdit){
    m_startDateEdit->setEnabled(true);
  }

  if(m_endDateEdit){
    m_endDateEdit->setEnabled(true);
  }

  // TODO remove below and uncomment in modelObjectChanged()
  if(m_startDateEdit){
    Date startDate = m_billingPeriod.get().startDate();
    bool wasBlocked = m_startDateEdit->blockSignals(true);
    m_startDateEdit->setDate(QDate(startDate.year(),month(startDate.monthOfYear()),startDate.dayOfMonth()));
    m_startDateEdit->blockSignals(wasBlocked);
  }

  if(m_endDateEdit){
    Date endDate = m_billingPeriod.get().endDate();
    bool wasBlocked = m_endDateEdit->blockSignals(true);
    m_endDateEdit->setDate(QDate(endDate.year(),month(endDate.monthOfYear()),endDate.dayOfMonth()));
    m_endDateEdit->blockSignals(wasBlocked);
  }

  modelObjectChanged();

}

void BillingPeriodWidget::detach()
{
  if(m_billingPeriodIntEdit){
    m_billingPeriodIntEdit->unbind();
  }

  if(m_energyUseDoubleEdit){
    m_energyUseDoubleEdit->unbind();
  }

  if(m_peakDoubleEdit){
    m_peakDoubleEdit->unbind();
  }

  if(m_costDoubleEdit){
    m_costDoubleEdit->unbind();
  }

  if(m_startDateEdit){
    m_startDateEdit->setEnabled(false);
  }

  if(m_endDateEdit){
    m_endDateEdit->setEnabled(false);
  }
}

void BillingPeriodWidget::getLabel(QGridLayout * gridLayout, int rowIndex, int columnIndex, const QString& text)
{
  QLabel * label = new QLabel();
  label->setText(text);
  label->setObjectName("H2");

  gridLayout->addWidget(label,rowIndex,columnIndex,Qt::AlignLeft| Qt::AlignTop);
}

void BillingPeriodWidget::getStartDateLabel(QGridLayout * gridLayout, int rowIndex, int columnIndex)
{
  getLabel(gridLayout,rowIndex,columnIndex,QString("Start Date"));
}

void BillingPeriodWidget::getDateEdit(QGridLayout * gridLayout, int rowIndex, int columnIndex, QDateEdit * & dateEdit)
{
  dateEdit = new QDateEdit();
  dateEdit->setCalendarPopup(true);
  dateEdit->setFixedWidth(OS_UTILITY_WIDTH);

  gridLayout->addWidget(dateEdit,rowIndex,columnIndex,Qt::AlignLeft| Qt::AlignTop);
}

void BillingPeriodWidget::getStartDateCalendar(QGridLayout * gridLayout, int rowIndex, int columnIndex)
{
  getDateEdit(gridLayout, rowIndex, columnIndex, m_startDateEdit);
}

void BillingPeriodWidget::getEndDateLabel(QGridLayout * gridLayout, int rowIndex, int columnIndex)
{
  getLabel(gridLayout,rowIndex,columnIndex,QString("End Date"));
}

void BillingPeriodWidget::getEndDateCalendar(QGridLayout * gridLayout, int rowIndex, int columnIndex)
{
  getDateEdit(gridLayout, rowIndex, columnIndex, m_endDateEdit);
}

void BillingPeriodWidget::getBillingPeriodLabel(QGridLayout * gridLayout, int rowIndex, int columnIndex)
{
  getLabel(gridLayout,rowIndex,columnIndex,QString("Billing Period Days"));
}

void BillingPeriodWidget::getBillingPeriodLineEdit(QGridLayout * gridLayout, int rowIndex, int columnIndex)
{
  m_billingPeriodIntEdit = new OSIntegerEdit2();
  // TODO apply validator
  //m_billingPeriodIntEdit->setDecimals(0);
  //m_billingPeriodIntEdit->setMinimum(28);
  //m_billingPeriodIntEdit->setMaximum(31);
  m_billingPeriodIntEdit->setFixedWidth(OS_UTILITY_WIDTH);
  
  gridLayout->addWidget(m_billingPeriodIntEdit,rowIndex,columnIndex,Qt::AlignLeft | Qt::AlignTop);
}

///// SLOTS

void BillingPeriodWidget::modelObjectChanged()
{
  if(!m_billingPeriod.get().empty()){
  //  if(m_startDateEdit){
  //    Date startDate = m_billingPeriod.get().startDate();
  //    bool wasBlocked = m_startDateEdit->blockSignals(true);
  //    m_startDateEdit->setDate(QDate(startDate.year(),month(startDate.monthOfYear()),startDate.dayOfMonth()));
  //    m_startDateEdit->blockSignals(wasBlocked);
  //  }

  //  if(m_endDateEdit){
  //    Date endDate = m_billingPeriod.get().endDate();
  //    bool wasBlocked = m_endDateEdit->blockSignals(true);
  //    m_endDateEdit->setDate(QDate(endDate.year(),month(endDate.monthOfYear()),endDate.dayOfMonth()));
  //    m_endDateEdit->blockSignals(wasBlocked);
  //  }
  }
}

void BillingPeriodWidget::startDateChanged(const QDate & newdate)
{ 
  m_billingPeriod.get().setStartDate(Date(newdate.month(),newdate.day(),newdate.year()));
}

void BillingPeriodWidget::endDateChanged(const QDate & newdate)
{
  m_billingPeriod.get().setEndDate(Date(newdate.month(),newdate.day(),newdate.year()));
}

} // openstudio
