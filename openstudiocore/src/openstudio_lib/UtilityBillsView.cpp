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

#include <openstudio_lib/ModelObjectListView.hpp>
#include <openstudio_lib/ModelObjectTypeListView.hpp>
#include <openstudio_lib/OSItem.hpp>

#include "../shared_gui_components/Buttons.hpp"

#include <model/Model_Impl.hpp>
#include <model/UtilityBill_Impl.hpp>
#include <model/WeatherFile.hpp>
#include <model/WeatherFile_Impl.hpp>
#include <model/YearDescription.hpp>
#include <model/YearDescription_Impl.hpp>

#include <utilities/data/DataEnums.hpp>
#include <utilities/time/Date.hpp>

#include <QBoxLayout>
#include <QButtonGroup>
#include <QDate>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QStackedWidget>

#define OS_UTILITY_WIDTH 150
#define OS_EDIT_WIDTH 300

namespace openstudio {

// "Utility Calibration"

UtilityBillsView::UtilityBillsView(const openstudio::model::Model& model, QWidget * parent)
                     : ModelSubTabView(new ModelObjectTypeListView(UtilityBillsView::modelObjectTypesAndNames(), 
                                                                   model, 
                                                                   false, 
                                                                   OSItem::COLLAPSIBLE_LIST_HEADER, 
                                                                   parent),
                                       new UtilityBillsInspectorView(model,parent),
                                       parent)
{

  boost::optional<model::YearDescription> yearDescription = model.yearDescription();
  if(yearDescription){
    boost::optional<int> calendarYear = yearDescription.get().calendarYear();
    if(calendarYear){
      int temp = *calendarYear;
    }
  }

  boost::optional<model::WeatherFile> weatherFile = model.weatherFile();
  if(weatherFile){
    // TODO weatherFile.get().startDate();
    // TODO weatherFile.get().endDate();
  }

}

std::vector<std::pair<IddObjectType, std::string> > UtilityBillsView::modelObjectTypesAndNames()
{
 //Electric Utility Bills
//Gas Utility Bills
//District Cooling Utility Bills
//District Heating Utility Bills
//Gasoline Bills
//Diesel Bills
//Coal Bills
//Fuel Oil #1 Bills
//Fuel Oil #2 Bills
//Propane Bills
//Water Bills

//OPENSTUDIO_ENUM(FuelType,
//  ((Electricity))
//  ((Gas)(NaturalGas))
//  ((Gasoline))
//  ((Diesel))
//  ((FuelOil_1))
//  ((FuelOil_2))
//  ((Propane))
//  ((Water))
//  ((Steam))
//  ((DistrictCooling))
//  ((DistrictHeating))
//  ((EnergyTransfer))); 
  
  std::vector<std::pair<IddObjectType, std::string> > result;
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_UtilityBill, "Electric Utility Bills"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_UtilityBill, "Gas Utility Bills"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_UtilityBill, "District Heating Utility Bills"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_UtilityBill, "District Cooling Utility Bills"));

  return result;
}


//**********************************************************************************************************


UtilityBillsInspectorView::UtilityBillsInspectorView(const model::Model & model,
  bool addScrollArea,
  QWidget * parent)
  : ModelObjectInspectorView(model,
      true,
      parent),
    m_billFormatDialog(NULL)
{
  boost::optional<model::UtilityBill> electricBill;
  boost::optional<model::UtilityBill> gasBill;
  boost::optional<model::UtilityBill> gasolineBill;
  boost::optional<model::UtilityBill> dieselBill;
  boost::optional<model::UtilityBill> coalBill;
  boost::optional<model::UtilityBill> fuelOil1Bill;
  boost::optional<model::UtilityBill> fuelOil2Bill;
  boost::optional<model::UtilityBill> propaneBill;
  boost::optional<model::UtilityBill> waterBill;
  boost::optional<model::UtilityBill> steamBill;
  boost::optional<model::UtilityBill> districtCoolingBill;
  boost::optional<model::UtilityBill> districtHeatingBill;
  boost::optional<model::UtilityBill> energyTransferBill;

  std::vector<WorkspaceObject> workspaceObjects = m_model.getObjectsByType(IddObjectType::OS_UtilityBill);

  if(workspaceObjects.size() > 0){
    bool success = true;
  }
  
  // sort by name
  std::sort(workspaceObjects.begin(), workspaceObjects.end(), WorkspaceObjectNameGreater());

  BOOST_FOREACH(WorkspaceObject workspaceObject,workspaceObjects){
    if(!workspaceObject.handle().isNull()){
      openstudio::model::ModelObject modelObject = workspaceObject.cast<openstudio::model::ModelObject>();
      if(boost::optional<model::UtilityBill> utilityBill = modelObject.optionalCast<model::UtilityBill>()) {
        if(utilityBill){
          FuelType fuelType = utilityBill.get().fuelType();

          if(fuelType ==  FuelType::Electricity){
            m_utilityBill = utilityBill.get();
            break;
            // TODO electricBill =  utilityBill.get();
          } else if(fuelType ==  FuelType::Gas){
            gasBill =  utilityBill.get();            
          } else if(fuelType ==  FuelType::Gasoline){
            gasolineBill =  utilityBill.get();
          } else if(fuelType ==  FuelType::Diesel){
            dieselBill =  utilityBill.get();
          } else if(fuelType ==  FuelType::FuelOil_1){
            fuelOil1Bill =  utilityBill.get();
          } else if(fuelType ==  FuelType::FuelOil_2){
            fuelOil2Bill =  utilityBill.get();
          } else if(fuelType ==  FuelType::Propane){
            propaneBill =  utilityBill.get();
          } else if(fuelType ==  FuelType::Water){
            waterBill =  utilityBill.get();
          } else if(fuelType ==  FuelType::Steam){
            steamBill =  utilityBill.get();
          } else if(fuelType ==  FuelType::DistrictCooling){
            districtCoolingBill =  utilityBill.get();
          } else if(fuelType ==  FuelType::DistrictHeating){
            districtHeatingBill =  utilityBill.get();
          } else if(fuelType ==  FuelType::EnergyTransfer){
            energyTransferBill =  utilityBill.get();
          } else {
            // Shouldn't be here
            Q_ASSERT(false);
          }
        }
      }
    }

// TODO BillingPeriodWidget * billingPeriodWidget = new BillingPeriodWidget(m_billGridLayout,m_utilityBill.fuelType,m_billFormat,index);
  }

  createWidgets();
}

void UtilityBillsInspectorView::createWidgets()
{
  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  bool isConnected = false;

  QLabel * label = NULL;

  QVBoxLayout * vLayout = NULL;

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
  Q_ASSERT(isConnected);

  //m_buttonGroup->addButton(radioButton,0); // TODO need to add delete button to this
  
  // Name

  vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  vLayout->setSpacing(10);

  label = new QLabel();
  label->setText("Name");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_name = new QLineEdit();
  //m_name->setReadOnly(true);
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

  m_consumptionUnits = new QLineEdit();
  //m_consumptionUnits->setReadOnly(true);
  m_consumptionUnits->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_consumptionUnits);

  gridLayout->addLayout(vLayout,0,0,Qt::AlignLeft | Qt::AlignTop);

  if(true){
  //if(fuelType == Electricity){
    vLayout = new QVBoxLayout();
    vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    vLayout->setSpacing(10);

    label = new QLabel();
    label->setText("\nPeak Demand Units");
    label->setObjectName("H2");
    vLayout->addWidget(label);

    m_energyDemandUnits = new QLineEdit();
    //m_energyDemandUnits->setReadOnly(true);
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

    m_windowTimesteps = new QDoubleSpinBox();
    m_windowTimesteps->setMinimum(1);
    m_windowTimesteps->setDecimals(0);
    m_windowTimesteps->setFixedWidth(OS_EDIT_WIDTH);
    vLayout->addWidget(m_windowTimesteps);

    gridLayout->addLayout(vLayout,0,2,Qt::AlignLeft | Qt::AlignTop);
  }

  gridLayout->setColumnStretch(100,100);
  
  mainLayout->addLayout(gridLayout);

  // Weather File

  vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  vLayout->setSpacing(10);

  label = new QLabel();
  label->setText("Weather File");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_weatherFile = new QLineEdit();
 // m_weatherFile->setReadOnly(true);
  m_weatherFile->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_weatherFile);

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
  Q_ASSERT(isConnected);

  label = new QLabel();
  label->setObjectName("H2");
  label->setText("Add New Billing Period");
  hLayout->addWidget(label,0, Qt::AlignLeft | Qt::AlignVCenter); // TODO verfiy text placement

  mainLayout->addLayout(hLayout);
  
  // Make Format Dialog
  m_billFormatDialog = new BillFormatDialog();
  m_billFormat = m_billFormatDialog->billFormat();

  isConnected = connect(m_billFormatDialog, SIGNAL(billFormatSignal(BillFormat)),
    this, SLOT(setBillFormat(BillFormat)));
  Q_ASSERT(isConnected);

  // TODO showBillFormatDialog();

}

bool UtilityBillsInspectorView::setStartDate(const Date& startDate)
{
  return false;
}
Date UtilityBillsInspectorView::startDate() const
{
  return Date();
}

bool UtilityBillsInspectorView::setEndDate(const Date& endDate)
{
  return false;
}
Date UtilityBillsInspectorView::endDate() const
{
  return Date();
}

bool UtilityBillsInspectorView::setNumberOfDays(unsigned numberOfDays)
{
  return false;
}
unsigned UtilityBillsInspectorView::numberOfDays() const
{
  return 0;
}

bool UtilityBillsInspectorView::setConsumption(double consumption)
{
  return false;
}
boost::optional<double> UtilityBillsInspectorView::consumption() const
{
  boost::optional<double> value;
  value = 0.1;
  return value;
}

bool UtilityBillsInspectorView::setPeakDemand(double peakDemand)
{
  return false;
}
boost::optional<double> UtilityBillsInspectorView::peakDemand() const
{
  boost::optional<double> value;
  value = 0.1;
  return value;
}

bool UtilityBillsInspectorView::setTotalCost(double totalCost)
{
  return false;
}
boost::optional<double> UtilityBillsInspectorView::totalCost() const
{
  boost::optional<double> value;
  value = 0.1;
  return value;
}

//void UtilityBillsInspectorView::onSelectItem(OSItem *item)
//{
//}

//void UtilityBillsInspectorView::onClearSelection()
//{
//}

void UtilityBillsInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  // TODO implement
}

void UtilityBillsInspectorView::onUpdate()
{
  // TODO implement
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
    for(unsigned i = 0; i < billingPeriods.size(); i++)
    addBillingPeriod(billingPeriods.at(i));
  }
}

void UtilityBillsInspectorView::addBillingPeriod(model::BillingPeriod & billingPeriod)
{
  if(m_utilityBill.is_initialized()){
    new BillingPeriodWidget(m_billGridLayout,m_utilityBill.get().fuelType(),m_billFormat,m_utilityBill.get().billingPeriods().size());
  }
}

void UtilityBillsInspectorView::addBillingPeriod()
{
  if(m_utilityBill.is_initialized()){
    new BillingPeriodWidget(m_billGridLayout,m_utilityBill.get().fuelType(),m_billFormat,m_utilityBill.get().billingPeriods().size());
  }
}

////// SLOTS ///////

void UtilityBillsInspectorView::addBillingPeriod(bool checked)
{
  // TODO BillingPeriodWidget * billingPeriodWidget = new BillingPeriodWidget(m_billGridLayout,m_utilityBill,m_billFormat,index);
}

void UtilityBillsInspectorView::deleteBillingPeriod(int index)
{
}

void UtilityBillsInspectorView::setBillFormat(BillFormat billFormat)
{
  m_billFormat = billFormat;
}

//void UtilityBillsInspectorView::resetUtilityBills()
//{
//}

//**********************************************************************************************************


BillingPeriodWidget::BillingPeriodWidget(QGridLayout * gridLayout,
  FuelType fuelType,
  BillFormat billFormat,
  unsigned index,
  QWidget * parent)
  : QWidget(parent),
  m_index(index)
{
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
      Q_ASSERT(false);
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
    Q_ASSERT(false);
  }

  columnIndex++;

  m_energyUseDoubleEdit = new QDoubleSpinBox();
  m_energyUseDoubleEdit->setFixedWidth(OS_UTILITY_WIDTH);
  gridLayout->addWidget(m_energyUseDoubleEdit,rowIndex,columnIndex++,Qt::AlignLeft | Qt::AlignTop);
  
  m_peaklDoubleEdit = new QDoubleSpinBox();
  m_peaklDoubleEdit->setFixedWidth(OS_UTILITY_WIDTH);
  gridLayout->addWidget(m_peaklDoubleEdit,rowIndex,columnIndex++,Qt::AlignLeft | Qt::AlignTop);

  m_costDoubleEdit = new QDoubleSpinBox();
  m_costDoubleEdit->setFixedWidth(OS_UTILITY_WIDTH);
  gridLayout->addWidget(m_costDoubleEdit,rowIndex,columnIndex++,Qt::AlignLeft | Qt::AlignTop);

  m_deleteBillWidget = new SofterRemoveButton();
  gridLayout->addWidget(m_deleteBillWidget,rowIndex,columnIndex++,Qt::AlignLeft | Qt::AlignTop);
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

  bool isConnected = connect(m_startDateEdit,SIGNAL(dateChanged(const QDate &)),
    this,SLOT(startDateChanged(const QDate &)));
  Q_ASSERT(isConnected);
}

void BillingPeriodWidget::getEndDateLabel(QGridLayout * gridLayout, int rowIndex, int columnIndex)
{
  getLabel(gridLayout,rowIndex,columnIndex,QString("End Date"));
}

void BillingPeriodWidget::getEndDateCalendar(QGridLayout * gridLayout, int rowIndex, int columnIndex)
{
  getDateEdit(gridLayout, rowIndex, columnIndex, m_endDateEdit);

  bool isConnected = connect(m_endDateEdit,SIGNAL(dateChanged(const QDate &)),
    this,SLOT(endDateChanged(const QDate &)));
  Q_ASSERT(isConnected);
}

void BillingPeriodWidget::getBillingPeriodLabel(QGridLayout * gridLayout, int rowIndex, int columnIndex)
{
  getLabel(gridLayout,rowIndex,columnIndex,QString("Billing Period Days"));
}

void BillingPeriodWidget::getBillingPeriodLineEdit(QGridLayout * gridLayout, int rowIndex, int columnIndex)
{
  m_billingPeriodIntEdit = new QDoubleSpinBox();
  m_billingPeriodIntEdit->setDecimals(0);
  // TODO m_billingPeriodIntEdit->setMinimum(0);
  m_billingPeriodIntEdit->setMinimum(28);
  m_billingPeriodIntEdit->setMaximum(31);
  m_billingPeriodIntEdit->setFixedWidth(OS_UTILITY_WIDTH);

  gridLayout->addWidget(m_billingPeriodIntEdit,rowIndex,columnIndex,Qt::AlignLeft | Qt::AlignTop);
}

//// SLOTS

void BillingPeriodWidget::startDateChanged(const QDate & newdate)
{
  //model::RunPeriodControlDaylightSavingTime dst =
  //  m_model.getUniqueModelObject<model::RunPeriodControlDaylightSavingTime>();

  //dst.setStartDate(monthOfYear(newdate.month()),newdate.day());
}

void BillingPeriodWidget::endDateChanged(const QDate & newdate)
{
  //model::RunPeriodControlDaylightSavingTime dst =
  //  m_model.getUniqueModelObject<model::RunPeriodControlDaylightSavingTime>();

  //dst.setEndDate(monthOfYear(newdate.month()),newdate.day());
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
  Q_ASSERT(isConnected);

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
  Q_ASSERT(buttonGroup->checkedId() != -1);
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
    Q_ASSERT(false);
  }
}

void BillFormatDialog::on_okButton(bool checked)
{
  emit billFormatSignal(m_billFormat);
  OSDialog::on_okButton(checked);
}

} // openstudio
