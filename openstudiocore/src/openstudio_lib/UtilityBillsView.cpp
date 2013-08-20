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

#define OS_UTILITY_WIDTH 150
#define OS_EDIT_WIDTH 300

namespace openstudio {

// "Utility Calibration"

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
    m_billFormatDialog(0), 
    m_beginAndEndDates(QString()),
    m_showPeak(0),
    m_buttonGroup(0),
    m_name(0),
    m_runPeriodLineEdit(0),
    m_consumptionUnits(0),
    m_energyDemandUnits(0),
    m_windowTimesteps(0),
    m_addBillingPeriod(0),
    m_billGridLayout(0)
{
  boost::optional<int> calendarYear;
  boost::optional<model::YearDescription> yearDescription = model.yearDescription();
  if(yearDescription){
    calendarYear = yearDescription.get().calendarYear();
    if(calendarYear){
      boost::optional<model::WeatherFile> weatherFile = model.weatherFile();
      if(weatherFile){
        boost::optional<model::RunPeriod> runPeriod = model.getOptionalUniqueModelObject<model::RunPeriod>();
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
          
          m_beginAndEndDates += "Start Date ";
          m_beginAndEndDates += m_beginAndEndDates.setNum(beginMonth);
          m_beginAndEndDates += "/";
          m_beginAndEndDates += m_beginAndEndDates.setNum(beginDayOfMonth);
          m_beginAndEndDates += "/";
          m_beginAndEndDates += m_beginAndEndDates.setNum(beginYear);
          m_beginAndEndDates += "   End Date ";
          m_beginAndEndDates += m_beginAndEndDates.setNum(endMonth);
          m_beginAndEndDates += "/";
          m_beginAndEndDates += m_beginAndEndDates.setNum(endDayOfMonth);
          m_beginAndEndDates += "/";
          m_beginAndEndDates += m_beginAndEndDates.setNum(endYear);
          
        }
      } 
    } 
  }

  createWidgets();
  addBillingPeriods();
}

void UtilityBillsInspectorView::createWidgets()
{
  QWidget* hiddenWidget = new QWidget();
  this->stackedWidget()->insertWidget(0, hiddenWidget);

  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  this->stackedWidget()->setCurrentIndex(0);

  bool isConnected = false;

  QLabel * label = NULL;

  QVBoxLayout * vLayout = NULL;

  QVBoxLayout * mainLayout = new QVBoxLayout();
  mainLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  mainLayout->setContentsMargins(10,10,10,10);
  mainLayout->setSpacing(20);
  visibleWidget->setLayout(mainLayout);

  if(m_beginAndEndDates.length() == 0){
    label = new QLabel();
    label->setPixmap(QPixmap(":/images/utility_calibration_warning.png"));
    label->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(label,0,Qt::AlignCenter);
    visibleWidget->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    return;
  }

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

  label = new QLabel();
  label->setText("\nConsumption Units");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_consumptionUnits = new OSComboBox2();
  m_consumptionUnits->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_consumptionUnits);

  gridLayout->addLayout(vLayout,0,0,Qt::AlignLeft | Qt::AlignTop);

  FuelType fuelType = FuelType::Electricity;
  if(m_utilityBill.is_initialized()){
    fuelType = m_utilityBill.get().fuelType();
  }

  if(fuelType == FuelType::Electricity){
    vLayout = new QVBoxLayout();
    vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    vLayout->setSpacing(10);

    label = new QLabel();
    label->setText("\nPeak Demand Units");
    label->setObjectName("H2");
    vLayout->addWidget(label);

    m_energyDemandUnits = new OSComboBox2();
    if(m_utilityBill.is_initialized()){
      std::vector<std::string> peakDemandUnitValues = m_utilityBill.get().peakDemandUnitValues();
      Q_FOREACH(const std::string & text, peakDemandUnitValues){
         m_energyDemandUnits->addItem(text.c_str());
      }
    }
    m_energyDemandUnits->setFixedWidth(OS_EDIT_WIDTH);
    vLayout->addWidget(m_energyDemandUnits);

    gridLayout->addLayout(vLayout,0,1,Qt::AlignLeft | Qt::AlignTop);
  
    vLayout = new QVBoxLayout();
    vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    vLayout->setSpacing(10);

    label = new QLabel();
    label->setText("Peak Demand\nWindow Timesteps");
    label->setObjectName("H2");
    vLayout->addWidget(label);

    m_windowTimesteps = new OSIntegerEdit2();
    // TODO replace below with validator
    //m_windowTimesteps->setMinimum(1);
    //m_windowTimesteps->setDecimals(0);
    m_windowTimesteps->setFixedWidth(OS_EDIT_WIDTH);
    vLayout->addWidget(m_windowTimesteps);

    gridLayout->addLayout(vLayout,0,2,Qt::AlignLeft | Qt::AlignTop);
  }

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

  m_runPeriodLineEdit = new QLineEdit(m_beginAndEndDates);
  m_runPeriodLineEdit->setReadOnly(true);
  m_runPeriodLineEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_runPeriodLineEdit);

  mainLayout->addLayout(vLayout);

  // Billing Period

  vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  vLayout->setSpacing(5);

  label = new QLabel();
  label->setText("Billing Period");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  mainLayout->addLayout(vLayout);

  // Bill Widget Grid Layouts

  m_billGridLayout = new QGridLayout();
  m_billGridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  m_billGridLayout->setContentsMargins(0,0,0,0);
  m_billGridLayout->setSpacing(10);
  mainLayout->addLayout(m_billGridLayout);

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
  
  // Make Format Dialog

  m_billFormatDialog = new BillFormatDialog();
  m_billFormat = m_billFormatDialog->billFormat();

  isConnected = connect(m_billFormatDialog, SIGNAL(billFormatSignal(BillFormat)),
    this, SLOT(setBillFormat(BillFormat)));
  OS_ASSERT(isConnected);
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
   
  if(m_energyDemandUnits){
    m_energyDemandUnits->bind(
        *m_utilityBill,
        boost::bind(&model::UtilityBill::peakDemandUnitValues,m_utilityBill.get_ptr()),
        OptionalStringGetter(boost::bind(&model::UtilityBill::peakDemandUnit,m_utilityBill.get_ptr())),
        boost::optional<StringSetter>(boost::bind(&model::UtilityBill::setPeakDemandUnit,m_utilityBill.get_ptr(),_1)));
  }

  //  TODO may need bind for unsigned
  if(m_windowTimesteps){
    //m_windowTimesteps->bind(
    //  *m_utilityBill,
    //  OptionalIntGetter(boost::bind(&model::UtilityBill::timestepsInPeakDemandWindow,m_utilityBill.get_ptr())),
    //  boost::optional<IntSetter>(boost::bind(&model::UtilityBill::setTimestepsInPeakDemandWindow,m_utilityBill.get_ptr(),_1)));
  }

  if(m_name || m_consumptionUnits || m_energyDemandUnits || m_windowTimesteps){
    this->stackedWidget()->setCurrentIndex(1);
  }
}

void UtilityBillsInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  if(m_name){
    m_name->unbind();
  }

  if(m_consumptionUnits){
    m_consumptionUnits->unbind();
  }

  if(m_energyDemandUnits){
    m_energyDemandUnits->unbind();
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

void UtilityBillsInspectorView::showBillFormatDialog()
{
  m_billFormatDialog->show();
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
      addBillingPeriod(billingPeriods.at(i),i);
    }
  }
}

void UtilityBillsInspectorView::addBillingPeriod(model::BillingPeriod & billingPeriod)
{
  addBillingPeriod(billingPeriod,m_utilityBill.get().billingPeriods().size());
}

void UtilityBillsInspectorView::addBillingPeriod(model::BillingPeriod & billingPeriod, unsigned index)
{
  if(m_utilityBill.is_initialized()){
    BillingPeriodWidget * billingPeriodWidget = new BillingPeriodWidget(m_billGridLayout,billingPeriod,m_utilityBill.get().fuelType(),m_billFormat,index);
    m_buttonGroup->addButton(billingPeriodWidget->m_deleteBillWidget,billingPeriodWidget->m_index);
  }
}

void UtilityBillsInspectorView::deleteBillingPeriods()
{
  QLayoutItem * child;
  while((child = m_billGridLayout->takeAt(0)) != 0 ){
    QWidget* widget = child->widget();
    if (widget){
      delete widget;
    }
    delete child;
  }
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
}


//**********************************************************************************************************


BillingPeriodWidget::BillingPeriodWidget(QGridLayout * gridLayout,
  model::BillingPeriod billingPeriod,
  FuelType fuelType,
  BillFormat billFormat,
  unsigned index,
  QWidget * parent)
  : QWidget(parent),
  m_billingPeriod(billingPeriod),
  m_startDateEdit(0),
  m_endDateEdit(0),
  m_billingPeriodIntEdit(0),
  m_energyUseDoubleEdit(0),
  m_peaklDoubleEdit(0),
  m_costDoubleEdit(0),
  m_index(index)
{
  OS_ASSERT(m_billingPeriod.is_initialized());

  createWidgets(gridLayout,fuelType,billFormat);
}

void BillingPeriodWidget::createWidgets(QGridLayout * gridLayout,
  FuelType fuelType,
  BillFormat billFormat)
{
  int columnIndex = 0;

  int rowIndex = gridLayout->rowCount();

  if(m_index == 0){
        
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
  
  m_peaklDoubleEdit = new OSDoubleEdit2();
  m_peaklDoubleEdit->setFixedWidth(OS_UTILITY_WIDTH);
  gridLayout->addWidget(m_peaklDoubleEdit,rowIndex,columnIndex++,Qt::AlignLeft | Qt::AlignTop);

  m_costDoubleEdit = new OSDoubleEdit2();
  m_costDoubleEdit->setFixedWidth(OS_UTILITY_WIDTH);
  gridLayout->addWidget(m_costDoubleEdit,rowIndex,columnIndex++,Qt::AlignLeft | Qt::AlignTop);

  m_deleteBillWidget = new SofterRemoveButton();
  gridLayout->addWidget(m_deleteBillWidget,rowIndex,columnIndex++,Qt::AlignLeft | Qt::AlignTop);

  if(m_startDateEdit){
    Date startDate = m_billingPeriod.get().startDate();
    m_startDateEdit->setDate(QDate(startDate.year(),month(startDate.monthOfYear()),startDate.dayOfMonth()));
  }

  if(m_endDateEdit){
    Date endDate = m_billingPeriod.get().endDate();
    m_endDateEdit->setDate(QDate(endDate.year(),month(endDate.monthOfYear()),endDate.dayOfMonth()));
  }

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

}

void BillingPeriodWidget::attach(openstudio::model::BillingPeriod & billingPeriod)
{
  m_billingPeriod = billingPeriod;

  // TODO 

  if(m_billingPeriodIntEdit){
    //m_billingPeriodIntEdit->bind(
    //  *m_billingPeriod,
    //  OptionalIntGetter(boost::bind(&model::BillingPeriod::numberOfDays,m_billingPeriod.get_ptr())),
    //  boost::optional<IntSetter>(boost::bind(&model::BillingPeriod::setNumberOfDays,m_billingPeriod.get_ptr(),_1)));
  }

  if(m_energyUseDoubleEdit){
    //m_energyUseDoubleEdit->bind(
    //  *m_billingPeriod,
    //  OptionalDoubleGetter(boost::bind(&model::BillingPeriod::consumption,m_billingPeriod.get_ptr())),
    //  boost::optional<DoubleSetter>(boost::bind(&model::BillingPeriod::setConsumption,m_billingPeriod.get_ptr(),_1)));
  }

  if(m_peaklDoubleEdit){
    //  m_peaklDoubleEdit->bind(
    //  *m_billingPeriod,
    //  OptionalDoubleGetter(boost::bind(&model::BillingPeriod::peakDemand,m_billingPeriod.get_ptr())),
    //  boost::optional<DoubleSetter>(boost::bind(&model::BillingPeriod::setPeakDemand,m_billingPeriod.get_ptr(),_1)));
  }

  if(m_costDoubleEdit){
    //m_costDoubleEdit->bind(
    //  *m_billingPeriod,
    //  OptionalDoubleGetter(boost::bind(&model::BillingPeriod::totalCost,m_billingPeriod.get_ptr())),
    //  boost::optional<DoubleSetter>(boost::bind(&model::BillingPeriod::setTotalCost,m_billingPeriod.get_ptr(),_1)));
  }

}

void BillingPeriodWidget::detach()
{
  if(m_billingPeriodIntEdit){
    m_billingPeriodIntEdit->unbind();
  }

  if(m_energyUseDoubleEdit){
    m_energyUseDoubleEdit->unbind();
  }

  if(m_peaklDoubleEdit){
    m_peaklDoubleEdit->unbind();
  }

  if(m_costDoubleEdit){
    m_costDoubleEdit->unbind();
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

void BillingPeriodWidget::startDateChanged(const QDate & newdate)
{ 
  m_billingPeriod.get().setStartDate(Date(newdate.day(),newdate.month(),newdate.year()));
}

void BillingPeriodWidget::endDateChanged(const QDate & newdate)
{
  m_billingPeriod.get().setStartDate(Date(newdate.day(),newdate.month(),newdate.year()));
}


//**********************************************************************************************************


BillFormatDialog::BillFormatDialog(QWidget * parent)
  : OSDialog(parent)
{
  createLayout();
}

void BillFormatDialog::createLayout()
{
  setSizeHint(QSize(400,260));

  okButton()->setText("Apply");

  cancelButton()->hide();

  QLabel * label = NULL;

  label = new QLabel("Select the best match for you utility bill",this);
  label->setObjectName("H1");
  upperLayout()->addWidget(label);

  QButtonGroup * buttonGroup = new QButtonGroup(this);

  bool isConnected = false;

  isConnected = connect(buttonGroup, SIGNAL(buttonClicked(int)),
    this, SLOT(setBillFormat(int)));
  OS_ASSERT(isConnected);

  QRadioButton * radioButton = NULL; 

  radioButton = new QRadioButton("Start Date and End Date");
  buttonGroup->addButton(radioButton,0);
  upperLayout()->addWidget(radioButton);

  radioButton = new QRadioButton("Start Date and Number of Days in Billing Period");
  buttonGroup->addButton(radioButton,1);
  upperLayout()->addWidget(radioButton);

  radioButton = new QRadioButton("End Date and Number of Days in Billing Period");
  buttonGroup->addButton(radioButton,2);
  upperLayout()->addWidget(radioButton);

  buttonGroup->button(0)->click();
  OS_ASSERT(buttonGroup->checkedId() != -1);
  setBillFormat(buttonGroup->checkedId());
}

void BillFormatDialog::setBillFormat(int index)
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
}

void BillFormatDialog::on_okButton(bool checked)
{
  emit billFormatSignal(m_billFormat);
  OSDialog::on_okButton(checked);
}

} // openstudio
