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

#include "LifeCycleCostsTabView.hpp"

#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSDoubleEdit.hpp"
#include "../shared_gui_components/OSIntegerEdit.hpp"

#include "../model/LifeCycleCostParameters_Impl.hpp"
#include "../model/Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QButtonGroup>
#include <QLabel>
#include <QRadioButton>
#include <QStackedWidget>
#include <QVBoxLayout>

#define OS_EDIT_WIDTH 150

namespace openstudio {

LifeCycleCostsTabView::LifeCycleCostsTabView(const model::Model & model,
                                 const QString& modelTempDir,
                                 QWidget * parent)
                                 : MainTabView("Life Cycle Costs",true,parent)
{
}

LifeCycleCostsView::LifeCycleCostsView(const model::Model & model)
  : QWidget(),
    m_model(model)
{
  m_lifeCycleCostParameters = m_model.getUniqueModelObject<model::LifeCycleCostParameters>();
  createWidgets();
  attach(m_lifeCycleCostParameters.get());
}

void LifeCycleCostsView::createWidgets()
{
  bool isConnected = false;

  QRadioButton * radioButton = NULL; 

  QLabel * label = NULL;

  QVBoxLayout * vLayout = NULL;

  QVBoxLayout * mainLayout = new QVBoxLayout();
  mainLayout->setContentsMargins(10,10,10,10);
  mainLayout->setSpacing(20);
  setLayout(mainLayout);

  vLayout = new QVBoxLayout();
  vLayout->setSpacing(10);

  label = new QLabel();
  label->setText("Life Cycle Cost Parameters");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  label = new QLabel();
  label->setText("Performed using constant dollar methodology.  The base date and service date are assumed to be January 1, 2012.");
  label->setObjectName("H3");
  vLayout->addWidget(label);
  
  label = new QLabel();
  label->setText("<a href=\"http://www1.eere.energy.gov/femp/program/lifecycle.html\">http://www1.eere.energy.gov/femp/program/lifecycle.html</a>");
  label->setOpenExternalLinks(true);
  vLayout->addWidget(label);

  mainLayout->addLayout(vLayout);

  // FEMP radio butons

  vLayout = new QVBoxLayout();
  vLayout->setSpacing(5);

  label = new QLabel();
  label->setText("Analysis Type");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_fempGroup = new QButtonGroup(this);

  isConnected = connect(m_fempGroup, SIGNAL(buttonClicked(int)),
    this, SLOT(fempGroupClicked(int)));
  OS_ASSERT(isConnected);

  radioButton = new QRadioButton("Federal Energy Management Program (FEMP)");
  m_fempGroup->addButton(radioButton,0);
  vLayout->addWidget(radioButton);

  radioButton = new QRadioButton("Custom");
  m_fempGroup->addButton(radioButton,1);
  vLayout->addWidget(radioButton);

  mainLayout->addLayout(vLayout);

  // Analysis Length and Real Discount Rate

  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(0,0,0,0);
  gridLayout->setSpacing(10);

  vLayout = new QVBoxLayout();
  vLayout->setSpacing(10);

  m_analysisLengthLabel = new QLabel();
  m_analysisLengthLabel->setText("Analysis Length (Years)");
  m_analysisLengthLabel->setObjectName("H2");
  vLayout->addWidget(m_analysisLengthLabel);

  m_analysisLengthIntegerEdit = new OSIntegerEdit2();
  m_analysisLengthIntegerEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_analysisLengthIntegerEdit);

  gridLayout->addLayout(vLayout,0,0,Qt::AlignLeft);

  vLayout = new QVBoxLayout();
  vLayout->setSpacing(5);

  m_realDiscountRateLabel = new QLabel();
  m_realDiscountRateLabel->setText("Real Discount Rate (fraction)");
  m_realDiscountRateLabel->setObjectName("H2");
  vLayout->addWidget(m_realDiscountRateLabel);

  m_realDiscountRateDoubleEdit = new OSDoubleEdit2();
  m_realDiscountRateDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_realDiscountRateDoubleEdit);

  gridLayout->addLayout(vLayout,0,1,Qt::AlignLeft);

  gridLayout->setColumnStretch(100,100);
  
  mainLayout->addLayout(gridLayout);

  // NIST radio buttons
  
  vLayout = new QVBoxLayout();
  vLayout->setSpacing(5);

  label = new QLabel();
  label->setText("Use National Institute of Standards and Technology (NIST) Fuel Escalation Rates");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_nistGroup = new QButtonGroup(this);

  isConnected = connect(m_nistGroup, SIGNAL(buttonClicked(int)),
    this, SLOT(nistGroupClicked(int)));
  OS_ASSERT(isConnected);

  radioButton = new QRadioButton("Yes");
  m_nistGroup->addButton(radioButton,0);
  vLayout->addWidget(radioButton);

  radioButton = new QRadioButton("No");
  m_nistGroup->addButton(radioButton,1);
  vLayout->addWidget(radioButton);

  mainLayout->addLayout(vLayout);
  
  // stacked widget
  
  m_stackedWidget = new QStackedWidget();
  m_stackedWidget->addWidget(createNistWidget());
  m_stackedWidget->addWidget(createInflationRatesWidget());
  mainLayout->addWidget(m_stackedWidget);

  mainLayout->addStretch();

}

QWidget * LifeCycleCostsView::createInflationRatesWidget()
{
  int rowCount = 0;
  int columnCount = 0;

  QLabel * label = NULL;

  QVBoxLayout * vLayout = NULL;

  QVBoxLayout * widgetLayout = new QVBoxLayout();
  widgetLayout->setContentsMargins(0,0,0,0);

  QWidget * widget = new QWidget();
  widget->setLayout(widgetLayout);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Inflation Rates (Relative to general inflation)");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  widgetLayout->addLayout(vLayout);

  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(0,0,0,0);
  gridLayout->setSpacing(10);

  widgetLayout->addLayout(gridLayout);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Electricity (fraction)");
  vLayout->addWidget(label);

  m_electricityDoubleEdit = new OSDoubleEdit2();
  m_electricityDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_electricityDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);

  gridLayout->setColumnStretch(100,100);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Natural Gas (fraction)");
  vLayout->addWidget(label);

  m_naturalGasDoubleEdit = new OSDoubleEdit2();
  m_naturalGasDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_naturalGasDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);


  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Steam (fraction)");
  vLayout->addWidget(label);

  m_steamDoubleEdit = new OSDoubleEdit2();
  m_steamDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_steamDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Gasoline (fraction)");
  vLayout->addWidget(label);

  m_gasolineDoubleEdit = new OSDoubleEdit2();
  m_gasolineDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_gasolineDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount,Qt::AlignLeft);


  rowCount++;
  columnCount = 0;

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Diesel (fraction)");
  vLayout->addWidget(label);

  m_dieselDoubleEdit = new OSDoubleEdit2();
  m_dieselDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_dieselDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);


  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Propane (fraction)");
  vLayout->addWidget(label);

  m_propaneDoubleEdit = new OSDoubleEdit2();
  m_propaneDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_propaneDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);


  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Coal (fraction)");
  vLayout->addWidget(label);

  m_coalDoubleEdit = new OSDoubleEdit2();
  m_coalDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_coalDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);


  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Fuel Oil #1 (fraction)");
  vLayout->addWidget(label);

  m_fuelOil_1DoubleEdit = new OSDoubleEdit2();
  m_fuelOil_1DoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_fuelOil_1DoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount,Qt::AlignLeft);


  rowCount++;
  columnCount = 0;

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Fuel Oil #2 (fraction)");
  vLayout->addWidget(label);

  m_fuelOil_2DoubleEdit = new OSDoubleEdit2();
  m_fuelOil_2DoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_fuelOil_2DoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);


  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Water (fraction)");
  vLayout->addWidget(label);

  m_waterDoubleEdit = new OSDoubleEdit2();
  m_waterDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_waterDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount++,columnCount,Qt::AlignLeft);

  gridLayout->setColumnStretch(100,100);

  return widget;

}

QWidget * LifeCycleCostsView::createNistWidget()
{
  QLabel * label = NULL;

  QVBoxLayout * vLayout = NULL;

  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(0,0,0,0);
  gridLayout->setSpacing(10);

  QWidget * widget = new QWidget();
  widget->setLayout(gridLayout);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("NIST Region");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_nistRegionComboBox = new OSComboBox2();
  vLayout->addWidget(m_nistRegionComboBox);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,0,0,Qt::AlignLeft);


  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("NIST Sector");

  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_nistSectorComboBox = new OSComboBox2();
  vLayout->addWidget(m_nistSectorComboBox);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,0,1,Qt::AlignLeft);

  gridLayout->setColumnStretch(100,100);

  return widget;

}

void LifeCycleCostsView::attach(openstudio::model::LifeCycleCostParameters & lifeCycleCostParameters)
{
  if(m_nistRegionComboBox){
    m_nistRegionComboBox->bind<std::string>(
      *m_lifeCycleCostParameters,
      static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
      std::bind(&model::LifeCycleCostParameters::validNistRegionValues,m_lifeCycleCostParameters.get_ptr()),
      OptionalStringGetter(std::bind(&model::LifeCycleCostParameters::nistRegion,m_lifeCycleCostParameters.get_ptr())),
      std::bind(&model::LifeCycleCostParameters::setNISTRegion,m_lifeCycleCostParameters.get_ptr(),std::placeholders::_1));
  }

  if(m_nistSectorComboBox){
    m_nistSectorComboBox->bind<std::string>(
      *m_lifeCycleCostParameters,
      static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
      std::bind(&model::LifeCycleCostParameters::validNistSectorValues,m_lifeCycleCostParameters.get_ptr()),
      OptionalStringGetter(std::bind(&model::LifeCycleCostParameters::nistSector,m_lifeCycleCostParameters.get_ptr())),
      std::bind(&model::LifeCycleCostParameters::setNISTSector,m_lifeCycleCostParameters.get_ptr(),std::placeholders::_1));
  }

  if(m_analysisLengthIntegerEdit){
    m_analysisLengthIntegerEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalIntGetter(std::bind(&model::LifeCycleCostParameters::lengthOfStudyPeriodInYears,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<IntSetter>(std::bind(&model::LifeCycleCostParameters::setLengthOfStudyPeriodInYears,m_lifeCycleCostParameters.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::LifeCycleCostParameters::resetLengthOfStudyPeriodInYears,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_realDiscountRateDoubleEdit){
    m_realDiscountRateDoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(std::bind(&model::LifeCycleCostParameters::realDiscountRate,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(std::bind(&model::LifeCycleCostParameters::setRealDiscountRate,m_lifeCycleCostParameters.get_ptr(),std::placeholders::_1)));
  }

  if(m_electricityDoubleEdit){
    m_electricityDoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(std::bind(&model::LifeCycleCostParameters::electricityInflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(std::bind(&model::LifeCycleCostParameters::setElectricityInflation,m_lifeCycleCostParameters.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::LifeCycleCostParameters::resetElectricityInflation,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_naturalGasDoubleEdit){
    m_naturalGasDoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(std::bind(&model::LifeCycleCostParameters::naturalGasInflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(std::bind(&model::LifeCycleCostParameters::setNaturalGasInflation,m_lifeCycleCostParameters.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::LifeCycleCostParameters::resetNaturalGasInflation,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_steamDoubleEdit){
    m_steamDoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(std::bind(&model::LifeCycleCostParameters::steamInflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(std::bind(&model::LifeCycleCostParameters::setSteamInflation,m_lifeCycleCostParameters.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::LifeCycleCostParameters::resetSteamInflation,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_gasolineDoubleEdit){
    m_gasolineDoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(std::bind(&model::LifeCycleCostParameters::gasolineInflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(std::bind(&model::LifeCycleCostParameters::setGasolineInflation,m_lifeCycleCostParameters.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::LifeCycleCostParameters::resetGasolineInflation,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_dieselDoubleEdit){
    m_dieselDoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(std::bind(&model::LifeCycleCostParameters::dieselInflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(std::bind(&model::LifeCycleCostParameters::setDieselInflation,m_lifeCycleCostParameters.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::LifeCycleCostParameters::resetDieselInflation,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_propaneDoubleEdit){
    m_propaneDoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(std::bind(&model::LifeCycleCostParameters::propaneInflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(std::bind(&model::LifeCycleCostParameters::setPropaneInflation,m_lifeCycleCostParameters.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::LifeCycleCostParameters::resetPropaneInflation,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_coalDoubleEdit){
    m_coalDoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(std::bind(&model::LifeCycleCostParameters::coalInflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(std::bind(&model::LifeCycleCostParameters::setCoalInflation,m_lifeCycleCostParameters.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::LifeCycleCostParameters::resetCoalInflation,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_fuelOil_1DoubleEdit){
    m_fuelOil_1DoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(std::bind(&model::LifeCycleCostParameters::fuelOil1Inflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(std::bind(&model::LifeCycleCostParameters::setFuelOil1Inflation,m_lifeCycleCostParameters.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::LifeCycleCostParameters::resetFuelOil1Inflation,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_fuelOil_2DoubleEdit){
    m_fuelOil_2DoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(std::bind(&model::LifeCycleCostParameters::fuelOil2Inflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(std::bind(&model::LifeCycleCostParameters::setFuelOil2Inflation,m_lifeCycleCostParameters.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::LifeCycleCostParameters::resetFuelOil2Inflation,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_waterDoubleEdit){
    m_waterDoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(std::bind(&model::LifeCycleCostParameters::waterInflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(std::bind(&model::LifeCycleCostParameters::setWaterInflation,m_lifeCycleCostParameters.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::LifeCycleCostParameters::resetWaterInflation,m_lifeCycleCostParameters.get_ptr())));
  }
 
  OS_ASSERT(m_fempGroup->button(0));
  QString type = m_lifeCycleCostParameters->analysisType().c_str();
  if(type == "FEMP"){
    m_fempGroup->button(0)->setChecked(true);
    fempGroupClicked(0);
  } else if(type == "Custom") {
    m_fempGroup->button(1)->setChecked(true);
    fempGroupClicked(1);
  } else {
    // should never get here
    OS_ASSERT(false);
  }
  
  OS_ASSERT(m_nistGroup->button(0));
  bool useNist = m_lifeCycleCostParameters->useNISTFuelEscalationRates();
  if(useNist){
    m_nistGroup->button(0)->setChecked(true);
    nistGroupClicked(0);
  } else {
    m_nistGroup->button(1)->setChecked(true);
    nistGroupClicked(1);
  }
}

void LifeCycleCostsView::detach()
{
  m_nistRegionComboBox->unbind();
  m_nistSectorComboBox->unbind();
  m_analysisLengthIntegerEdit->unbind();
  m_realDiscountRateDoubleEdit->unbind();
  m_electricityDoubleEdit->unbind();
  m_naturalGasDoubleEdit->unbind();
  m_steamDoubleEdit->unbind();
  m_gasolineDoubleEdit->unbind();
  m_dieselDoubleEdit->unbind();
  m_propaneDoubleEdit->unbind();
  m_coalDoubleEdit->unbind();
  m_fuelOil_1DoubleEdit->unbind();
  m_fuelOil_2DoubleEdit->unbind();
  m_waterDoubleEdit->unbind();
}

////// SLOTS ///////

void LifeCycleCostsView::fempGroupClicked(int index)
{  
  // 0 = FEMP, 1 = Custom
  // DLM: model object actually lets analysis length change under FEMP, it just can't be > 25
  // DLM: analysis length can't be > 30 when using NIST fuel escalation
  //m_analysisLengthLabel->setEnabled(index);
  //m_AnalysisLengthIntegerEdit->setEnabled(index);

  // this is locked under FEMP
  m_realDiscountRateLabel->setEnabled(index);
  m_realDiscountRateDoubleEdit->setEnabled(index);

  if(index == 0){
    m_lifeCycleCostParameters->setAnalysisType("FEMP");
  } else if(index == 1) {
    m_lifeCycleCostParameters->setAnalysisType("Custom");
  } else {
    // should never get here
    OS_ASSERT(false);
  }
}

void LifeCycleCostsView::nistGroupClicked(int index)
{

  // 0 = Use NIST rates, 1 = don't
  m_stackedWidget->setCurrentIndex(index);

  if(index == 0){
    m_lifeCycleCostParameters->setUseNISTFuelEscalationRates(true);
  } else if(index == 1) {
    m_lifeCycleCostParameters->setUseNISTFuelEscalationRates(false);
  } else {
    // should never get here
    OS_ASSERT(false);
  }

}

} // openstudio
