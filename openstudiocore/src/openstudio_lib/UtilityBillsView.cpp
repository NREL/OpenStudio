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

#include "../shared_gui_components/Buttons.hpp"
#include "../shared_gui_components/OSDoubleEdit.hpp"
#include "../shared_gui_components/OSIntegerEdit.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"

#include <model/Model_Impl.hpp>

#include <QBoxLayout>
#include <QButtonGroup>
#include <QDate>
#include <QDateEdit>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QStackedWidget>

#define OS_UTILITY_WIDTH 150
#define OS_EDIT_WIDTH 300

namespace openstudio {

// "Utility Calibration"

UtilityBillsView::UtilityBillsView(const model::Model & model, QWidget * parent)
  : ModelSubTabView(new ModelObjectListView(IddObjectType::OS_BuildingStory, model, true, parent),// TODO use OS_UtilityBill
               new UtilityBillsInspectorView(model,true),
               parent)
{
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
    this, SLOT(deleteBill(int)));
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

  m_name = new OSLineEdit();
  m_name->setReadOnly(true);
  m_name->setFixedWidth(OS_EDIT_WIDTH);
  //m_name->bind();
  vLayout->addWidget(m_name);

  mainLayout->addLayout(vLayout);


  // Consumption Units and Energy Demand Units

  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  gridLayout->setContentsMargins(0,0,0,0);
  gridLayout->setSpacing(10);

  vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  vLayout->setSpacing(10);

  label = new QLabel();
  label->setText("Consumption Units");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_consumptionUnits = new OSLineEdit();
  m_consumptionUnits->setReadOnly(true);
  m_consumptionUnits->setFixedWidth(OS_EDIT_WIDTH);
  //m_consumptionUnits->bind();
  vLayout->addWidget(m_consumptionUnits);

  gridLayout->addLayout(vLayout,0,0,Qt::AlignLeft | Qt::AlignTop);

  vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  vLayout->setSpacing(10);

  label = new QLabel();
  label->setText("Energy Demand Units");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_energyDemandUnits = new OSLineEdit();
  m_energyDemandUnits->setReadOnly(true);
  m_energyDemandUnits->setFixedWidth(OS_EDIT_WIDTH);
  //m_energyDemandUnits->bind();
  vLayout->addWidget(m_energyDemandUnits);

  gridLayout->addLayout(vLayout,0,1,Qt::AlignLeft | Qt::AlignTop);

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

  m_weatherFile = new OSLineEdit();
  m_weatherFile->setReadOnly(true);
  m_weatherFile->setFixedWidth(OS_EDIT_WIDTH);
  //m_weatherFile->bind();
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

  m_beforeRangeGridLayout = new QGridLayout();
  m_beforeRangeGridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  m_beforeRangeGridLayout->setContentsMargins(0,0,0,0);
  m_beforeRangeGridLayout->setSpacing(10);
  mainLayout->addLayout(m_beforeRangeGridLayout);

  m_inRangeGridLayout = new QGridLayout();
  m_inRangeGridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  m_inRangeGridLayout->setContentsMargins(0,0,0,0);
  m_inRangeGridLayout->setSpacing(10);

  m_groupBox = new QGroupBox("Within Range");
  m_groupBox->setLayout(m_inRangeGridLayout);
  mainLayout->addWidget(m_groupBox);

  showGroupBoxIfBills();

  m_afterRangeGridLayout = new QGridLayout();
  m_afterRangeGridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  m_afterRangeGridLayout->setContentsMargins(0,0,0,0);
  m_afterRangeGridLayout->setSpacing(10);
  mainLayout->addLayout(m_afterRangeGridLayout);

  // Add Bill Button
  QHBoxLayout * hLayout = new QHBoxLayout();
  hLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  m_addNewBill = new QPushButton();
  m_addNewBill->setFlat(true);
  m_addNewBill->setObjectName("AddButton");
  m_addNewBill->setToolTip("Add new object");
  m_addNewBill->setFixedSize(24,24);
  hLayout->addWidget(m_addNewBill);

  isConnected = connect(m_addNewBill, SIGNAL(clicked(bool)),
    this, SLOT(addBill(bool)));
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
}

  // TODO
  //QScrollArea * centralScrollArea = new QScrollArea(this);
  //centralScrollArea->setFrameStyle(QFrame::NoFrame);
  //centralScrollArea->setObjectName("GrayWidget");
  //centralScrollArea->setWidgetResizable(true);
  //centralScrollArea->setWidget(m_centralWidget);

  //// The right pane

  //m_rightScrollArea = new QScrollArea(this);
  //m_rightScrollArea->setFrameStyle(QFrame::NoFrame);
  //m_rightScrollArea->setObjectName("GrayWidget");
  //m_rightScrollArea->setWidgetResizable(true);

  //QSplitter * splitter = new QSplitter(this);
  //splitter->setOrientation(Qt::Horizontal);
  //splitter->addWidget(leftPaneWidget);
  //splitter->addWidget(centralScrollArea);
  //splitter->addWidget(m_rightScrollArea);



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


void UtilityBillsInspectorView::showGroupBoxIfBills()
{
  if(m_inRangeGridLayout->rowCount() > 0){
    m_groupBox->show();
  }
  else{
    m_groupBox->hide();
  }
}

void UtilityBillsInspectorView::showAddButton()
{
  m_addNewBill->show();
}

void UtilityBillsInspectorView::hideAddButton()
{
  m_addNewBill->hide();
}

void UtilityBillsInspectorView::enableAddButton()
{
  m_addNewBill->setEnabled(true);
}

void UtilityBillsInspectorView::disableAddButton()
{
  m_addNewBill->setEnabled(false);
}

////// SLOTS ///////

void UtilityBillsInspectorView::addBill(bool checked)
{
  if(m_sortedBills.size() == 0){
    showBillFormatDialog();
  }

  UtilityBillWidget * utilityBillWidget = new UtilityBillWidget(m_inRangeGridLayout,m_billFormat,m_showPeak);
}

void UtilityBillsInspectorView::deleteBill(int index)
{
}

void UtilityBillsInspectorView::setBillFormat(BillFormat billFormat)
{
  m_billFormat = billFormat;
}


//**********************************************************************************************************


UtilityBillWidget::UtilityBillWidget(QGridLayout * gridLayout,
  BillFormat billFormat,
  bool showPeak,
  QWidget * parent)
  : QWidget(parent)
{
  createWidgets(gridLayout,billFormat,showPeak);
}

void UtilityBillWidget::createWidgets(QGridLayout * gridLayout,
  BillFormat billFormat,
  bool showPeak)
{
  int columnIndex = 0;

  int rowIndex = gridLayout->rowCount();
  
  if(billFormat == STARTDATE_ENDDATE){
    getStartDate(gridLayout, rowIndex, columnIndex);
    getEndDate(gridLayout, rowIndex, columnIndex);
  }
  else if(billFormat == STARTDATE_NUMDAYS){
    getStartDate(gridLayout, rowIndex, columnIndex);
    getBillingPeriod(gridLayout, rowIndex, columnIndex);
  }
  else if(billFormat == ENDDATE_NUMDAYS){
    getBillingPeriod(gridLayout, rowIndex, columnIndex);
    getEndDate(gridLayout, rowIndex, columnIndex);
  }
  else{
    Q_ASSERT(false);
  }

  QLabel * label = NULL;

  QVBoxLayout * vLayout = NULL;
  
  vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  label = new QLabel();
  label->setText("Energy Use (kWh)");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_energyUseDoubleEdit = new OSDoubleEdit();
  m_energyUseDoubleEdit->setFixedWidth(OS_UTILITY_WIDTH);
  //m_energyUseDoubleEdit->bind();
  vLayout->addWidget(m_energyUseDoubleEdit);

  gridLayout->addLayout(vLayout,rowIndex,++columnIndex,Qt::AlignLeft | Qt::AlignTop);

  vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  label = new QLabel();
  label->setText("Peak (kW)");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_peaklDoubleEdit = new OSDoubleEdit();
  m_peaklDoubleEdit->setFixedWidth(OS_UTILITY_WIDTH);
  //m_peaklDoubleEdit->bind();
  vLayout->addWidget(m_peaklDoubleEdit);

  gridLayout->addLayout(vLayout,rowIndex,++columnIndex,Qt::AlignLeft | Qt::AlignTop);


  vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  label = new QLabel();
  label->setText("Cost");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_costDoubleEdit = new OSDoubleEdit();
  m_costDoubleEdit->setFixedWidth(OS_UTILITY_WIDTH);
  //m_costDoubleEdit->bind();
  vLayout->addWidget(m_costDoubleEdit);

  gridLayout->addLayout(vLayout,rowIndex,++columnIndex,Qt::AlignLeft | Qt::AlignTop);

  
  vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  label = new QLabel();
  vLayout->addWidget(label);

  m_deleteBillWidget = new SofterRemoveButton();
  vLayout->addWidget(m_deleteBillWidget);
  
  gridLayout->addLayout(vLayout,rowIndex,++columnIndex,Qt::AlignLeft | Qt::AlignTop);


  gridLayout->setColumnStretch(100,100);
}

void UtilityBillWidget::getStartDate(QGridLayout * gridLayout, int & rowIndex, int & columnIndex)
{
  QVBoxLayout * vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  m_startDateLbl = new QLabel();
  m_startDateLbl->setText("Start Date");
  m_startDateLbl->setObjectName("H2");
  vLayout->addWidget(m_startDateLbl);
   
  m_startDateEdit = new QDateEdit();
  m_startDateEdit->setCalendarPopup(true);
  m_startDateEdit->setFixedWidth(OS_UTILITY_WIDTH);
  vLayout->addWidget(m_startDateEdit);

  bool isConnected = connect(m_startDateEdit,SIGNAL(dateChanged(const QDate &)),
    this,SIGNAL(dstStartDateChanged(const QDate &)));


  gridLayout->addLayout(vLayout,rowIndex,++columnIndex,Qt::AlignLeft| Qt::AlignTop);
}

void UtilityBillWidget::getEndDate(QGridLayout * gridLayout, int & rowIndex, int & columnIndex)
{
  
  QVBoxLayout * vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  m_endDateLbl = new QLabel();
  m_endDateLbl->setText("End Date");
  m_endDateLbl->setObjectName("H2");
  vLayout->addWidget(m_endDateLbl);

  m_endDateEdit = new QDateEdit();
  m_endDateEdit->setCalendarPopup(true);
  m_endDateEdit->setFixedWidth(OS_UTILITY_WIDTH);
  //m_endDateEdit->bind(); not available in an OS object
  vLayout->addWidget(m_endDateEdit);

  connect(m_endDateEdit,SIGNAL(dateChanged(const QDate &)),
    this,SIGNAL(dstEndDateChanged(const QDate &)));

  gridLayout->addLayout(vLayout,rowIndex,++columnIndex,Qt::AlignLeft | Qt::AlignTop);
}

void UtilityBillWidget::getBillingPeriod(QGridLayout * gridLayout, int & rowIndex, int & columnIndex)
{
  QVBoxLayout * vLayout = new QVBoxLayout();
  vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  m_billingPeriodIntLbl = new QLabel();
  m_billingPeriodIntLbl->setText("Billing Period Days");
  m_billingPeriodIntLbl->setObjectName("H2");
  vLayout->addWidget(m_billingPeriodIntLbl);

  m_billingPeriodIntEdit = new OSIntegerEdit();
  m_billingPeriodIntEdit->setFixedWidth(OS_UTILITY_WIDTH);
  //m_billingPeriodIntEdit->bind();
  vLayout->addWidget(m_billingPeriodIntEdit);

  gridLayout->addLayout(vLayout,rowIndex,++columnIndex,Qt::AlignLeft | Qt::AlignTop);
}

//// SLOTS

void UtilityBillWidget::setDstStartDate(const QDate & newdate)
{
  //model::RunPeriodControlDaylightSavingTime dst =
  //  m_model.getUniqueModelObject<model::RunPeriodControlDaylightSavingTime>();

  //dst.setStartDate(monthOfYear(newdate.month()),newdate.day());
}

void UtilityBillWidget::setDstEndDate(const QDate & newdate)
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
