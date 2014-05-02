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

#include <openstudio_lib/LifeCycleCostsTabView.hpp>

#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSDoubleEdit.hpp"
#include "../shared_gui_components/OSIntegerEdit.hpp"

#include <model/LifeCycleCostParameters_Impl.hpp>
#include <model/Model_Impl.hpp>

#include <utilities/core/Assert.hpp>

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

  m_AnalysisLengthIntegerEdit = new OSIntegerEdit2();  // TODO must cap max at 25 for FEMP, or 30 for NIST, MAX_lengthOfStudyPeriodInYears
  m_AnalysisLengthIntegerEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_AnalysisLengthIntegerEdit);

  gridLayout->addLayout(vLayout,0,0,Qt::AlignLeft);

  vLayout = new QVBoxLayout();
  vLayout->setSpacing(5);

  m_realDiscountRateLabel = new QLabel();
  m_realDiscountRateLabel->setText("Real Discount Rate (%)");
  m_realDiscountRateLabel->setObjectName("H2");
  vLayout->addWidget(m_realDiscountRateLabel);

  m_RealDiscountRateDoubleEdit = new OSDoubleEdit2();
  m_RealDiscountRateDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_RealDiscountRateDoubleEdit);

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
  label->setText("Electricity (%)");
  vLayout->addWidget(label);

  m_ElectricityDoubleEdit = new OSDoubleEdit2();
  m_ElectricityDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_ElectricityDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);

  gridLayout->setColumnStretch(100,100);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Natural Gas (%)");
  vLayout->addWidget(label);

  m_NaturalGasDoubleEdit = new OSDoubleEdit2();
  m_NaturalGasDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_NaturalGasDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);


  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Steam (%)");
  vLayout->addWidget(label);

  m_SteamDoubleEdit = new OSDoubleEdit2();
  m_SteamDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_SteamDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Gasoline (%)");
  vLayout->addWidget(label);

  m_GasolineDoubleEdit = new OSDoubleEdit2();
  m_GasolineDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_GasolineDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount,Qt::AlignLeft);


  rowCount++;
  columnCount = 0;

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Diesel (%)");
  vLayout->addWidget(label);

  m_DieselDoubleEdit = new OSDoubleEdit2();
  m_DieselDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_DieselDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);


  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Propane (%)");
  vLayout->addWidget(label);

  m_PropaneDoubleEdit = new OSDoubleEdit2();
  m_PropaneDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_PropaneDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);


  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Coal (%)");
  vLayout->addWidget(label);

  m_CoalDoubleEdit = new OSDoubleEdit2();
  m_CoalDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_CoalDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);


  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Fuel Oil #1 (%)");
  vLayout->addWidget(label);

  m_FuelOil_1DoubleEdit = new OSDoubleEdit2();
  m_FuelOil_1DoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_FuelOil_1DoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount,Qt::AlignLeft);


  rowCount++;
  columnCount = 0;

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Fuel Oil #2 (%)");
  vLayout->addWidget(label);

  m_FuelOil_2DoubleEdit = new OSDoubleEdit2();
  m_FuelOil_2DoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_FuelOil_2DoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);


  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Water (%)");
  vLayout->addWidget(label);

  m_WaterDoubleEdit = new OSDoubleEdit2();
  m_WaterDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  vLayout->addWidget(m_WaterDoubleEdit);

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
      boost::bind(&model::LifeCycleCostParameters::validNistRegionValues,m_lifeCycleCostParameters.get_ptr()),
      OptionalStringGetter(boost::bind(&model::LifeCycleCostParameters::nistRegion,m_lifeCycleCostParameters.get_ptr())),
      boost::bind(&model::LifeCycleCostParameters::setNISTRegion,m_lifeCycleCostParameters.get_ptr(),_1));
  }

  if(m_nistSectorComboBox){
    m_nistSectorComboBox->bind<std::string>(
      *m_lifeCycleCostParameters,
      static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
      boost::bind(&model::LifeCycleCostParameters::validNistSectorValues,m_lifeCycleCostParameters.get_ptr()),
      OptionalStringGetter(boost::bind(&model::LifeCycleCostParameters::nistSector,m_lifeCycleCostParameters.get_ptr())),
      boost::bind(&model::LifeCycleCostParameters::setNISTSector,m_lifeCycleCostParameters.get_ptr(),_1));
  }

  if(m_AnalysisLengthIntegerEdit){
    m_AnalysisLengthIntegerEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalIntGetter(boost::bind(&model::LifeCycleCostParameters::lengthOfStudyPeriodInYears,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<IntSetter>(boost::bind(&model::LifeCycleCostParameters::setLengthOfStudyPeriodInYears,m_lifeCycleCostParameters.get_ptr(),_1)),
      boost::optional<NoFailAction>(boost::bind(&model::LifeCycleCostParameters::resetLengthOfStudyPeriodInYears,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_RealDiscountRateDoubleEdit){
    m_RealDiscountRateDoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(boost::bind(&model::LifeCycleCostParameters::realDiscountRate,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(boost::bind(&model::LifeCycleCostParameters::setRealDiscountRate,m_lifeCycleCostParameters.get_ptr(),_1)));
  }

  if(m_ElectricityDoubleEdit){
    m_ElectricityDoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(boost::bind(&model::LifeCycleCostParameters::electricityInflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(boost::bind(&model::LifeCycleCostParameters::setElectricityInflation,m_lifeCycleCostParameters.get_ptr(),_1)),
      boost::optional<NoFailAction>(boost::bind(&model::LifeCycleCostParameters::resetElectricityInflation,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_NaturalGasDoubleEdit){
    m_NaturalGasDoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(boost::bind(&model::LifeCycleCostParameters::naturalGasInflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(boost::bind(&model::LifeCycleCostParameters::setNaturalGasInflation,m_lifeCycleCostParameters.get_ptr(),_1)),
      boost::optional<NoFailAction>(boost::bind(&model::LifeCycleCostParameters::resetNaturalGasInflation,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_SteamDoubleEdit){
    m_SteamDoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(boost::bind(&model::LifeCycleCostParameters::steamInflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(boost::bind(&model::LifeCycleCostParameters::setSteamInflation,m_lifeCycleCostParameters.get_ptr(),_1)),
      boost::optional<NoFailAction>(boost::bind(&model::LifeCycleCostParameters::resetSteamInflation,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_GasolineDoubleEdit){
    m_GasolineDoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(boost::bind(&model::LifeCycleCostParameters::gasolineInflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(boost::bind(&model::LifeCycleCostParameters::setGasolineInflation,m_lifeCycleCostParameters.get_ptr(),_1)),
      boost::optional<NoFailAction>(boost::bind(&model::LifeCycleCostParameters::resetGasolineInflation,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_DieselDoubleEdit){
    m_DieselDoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(boost::bind(&model::LifeCycleCostParameters::dieselInflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(boost::bind(&model::LifeCycleCostParameters::setDieselInflation,m_lifeCycleCostParameters.get_ptr(),_1)),
      boost::optional<NoFailAction>(boost::bind(&model::LifeCycleCostParameters::resetDieselInflation,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_PropaneDoubleEdit){
    m_PropaneDoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(boost::bind(&model::LifeCycleCostParameters::propaneInflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(boost::bind(&model::LifeCycleCostParameters::setPropaneInflation,m_lifeCycleCostParameters.get_ptr(),_1)),
      boost::optional<NoFailAction>(boost::bind(&model::LifeCycleCostParameters::resetPropaneInflation,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_CoalDoubleEdit){
    m_CoalDoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(boost::bind(&model::LifeCycleCostParameters::coalInflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(boost::bind(&model::LifeCycleCostParameters::setCoalInflation,m_lifeCycleCostParameters.get_ptr(),_1)),
      boost::optional<NoFailAction>(boost::bind(&model::LifeCycleCostParameters::resetCoalInflation,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_FuelOil_1DoubleEdit){
    m_FuelOil_1DoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(boost::bind(&model::LifeCycleCostParameters::fuelOil1Inflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(boost::bind(&model::LifeCycleCostParameters::setFuelOil1Inflation,m_lifeCycleCostParameters.get_ptr(),_1)),
      boost::optional<NoFailAction>(boost::bind(&model::LifeCycleCostParameters::resetFuelOil1Inflation,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_FuelOil_2DoubleEdit){
    m_FuelOil_2DoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(boost::bind(&model::LifeCycleCostParameters::fuelOil2Inflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(boost::bind(&model::LifeCycleCostParameters::setFuelOil2Inflation,m_lifeCycleCostParameters.get_ptr(),_1)),
      boost::optional<NoFailAction>(boost::bind(&model::LifeCycleCostParameters::resetFuelOil2Inflation,m_lifeCycleCostParameters.get_ptr())));
  }

  if(m_WaterDoubleEdit){
    m_WaterDoubleEdit->bind(
      *m_lifeCycleCostParameters,
      OptionalDoubleGetter(boost::bind(&model::LifeCycleCostParameters::waterInflation,m_lifeCycleCostParameters.get_ptr())),
      boost::optional<DoubleSetter>(boost::bind(&model::LifeCycleCostParameters::setWaterInflation,m_lifeCycleCostParameters.get_ptr(),_1)),
      boost::optional<NoFailAction>(boost::bind(&model::LifeCycleCostParameters::resetWaterInflation,m_lifeCycleCostParameters.get_ptr())));
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
  m_AnalysisLengthIntegerEdit->unbind();
  m_RealDiscountRateDoubleEdit->unbind();
  m_ElectricityDoubleEdit->unbind();
  m_NaturalGasDoubleEdit->unbind();
  m_SteamDoubleEdit->unbind();
  m_GasolineDoubleEdit->unbind();
  m_DieselDoubleEdit->unbind();
  m_PropaneDoubleEdit->unbind();
  m_CoalDoubleEdit->unbind();
  m_FuelOil_1DoubleEdit->unbind();
  m_FuelOil_2DoubleEdit->unbind();
  m_WaterDoubleEdit->unbind();
}

////// SLOTS ///////

void LifeCycleCostsView::fempGroupClicked(int index)
{  
  // 0 = FEMP, 1 = Custom
  m_analysisLengthLabel->setEnabled(index);
  m_realDiscountRateLabel->setEnabled(index);
  m_AnalysisLengthIntegerEdit->setEnabled(index);
  m_RealDiscountRateDoubleEdit->setEnabled(index);

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
