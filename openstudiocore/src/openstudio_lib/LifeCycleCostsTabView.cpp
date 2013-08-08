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

#include <openstudio_lib/LifeCycleCostsTabView.hpp>

#include "../shared_gui_components/OSIntegerEdit.hpp"
#include "../shared_gui_components/OSDoubleEdit.hpp"

#include <model/Model_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <QBoxLayout>
#include <QButtonGroup>
#include <QComboBox>
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
  createWidgets();
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

  label = new QLabel();
  label->setText("Analysis Length (Years)");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_AnalysisLengthIntegerEdit = new OSIntegerEdit();  // TODO must cap max at 25
  m_AnalysisLengthIntegerEdit->setFixedWidth(OS_EDIT_WIDTH);
  //m_AnalysisLengthIntegerEdit->bind();
  vLayout->addWidget(m_AnalysisLengthIntegerEdit);

  gridLayout->addLayout(vLayout,0,0,Qt::AlignLeft);

  vLayout = new QVBoxLayout();
  vLayout->setSpacing(5);

  label = new QLabel();
  label->setText("Real Discount Rate (%)");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_RealDiscountRateDoubleEdit = new OSDoubleEdit();
  m_RealDiscountRateDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  //m_RealDiscountRateDoubleEdit->bind();
  vLayout->addWidget(m_RealDiscountRateDoubleEdit);

  gridLayout->addLayout(vLayout,0,1,Qt::AlignLeft);

  gridLayout->setColumnStretch(100,100);
  
  mainLayout->addLayout(gridLayout);

  // NIST radio buttons
  
  //  boost::optional<std::string> nistRegion() const;
  //  boost::optional<std::string> nistSector() const;
  
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
  m_stackedWidget->addWidget(createInflationRatesWidget());
  m_stackedWidget->addWidget(createNistWidget());
  mainLayout->addWidget(m_stackedWidget);

  mainLayout->addStretch();
}

QWidget * LifeCycleCostsView::createInflationRatesWidget()
{
  bool isConnected = false;

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

  m_ElectricityDoubleEdit = new OSDoubleEdit();
  m_ElectricityDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  //m_ElectricityDoubleEdit->bind();
  vLayout->addWidget(m_ElectricityDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);

  gridLayout->setColumnStretch(100,100);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Natural Gas (%)");
  vLayout->addWidget(label);

  m_NaturalGasDoubleEdit = new OSDoubleEdit();
  m_NaturalGasDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  //m_NaturalGasDoubleEdit->bind();
  vLayout->addWidget(m_NaturalGasDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);


  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Steam (%)");
  vLayout->addWidget(label);

  m_SteamDoubleEdit = new OSDoubleEdit();
  m_SteamDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  //m_SteamDoubleEdit->bind();
  vLayout->addWidget(m_SteamDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Gasoline (%)");
  vLayout->addWidget(label);

  m_GasolineDoubleEdit = new OSDoubleEdit();
  m_GasolineDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  //m_GasolineDoubleEdit->bind();
  vLayout->addWidget(m_GasolineDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount,Qt::AlignLeft);


  rowCount++;
  columnCount = 0;

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Diesel (%)");
  vLayout->addWidget(label);

  m_DieselDoubleEdit = new OSDoubleEdit();
  m_DieselDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  //m_DieselDoubleEdit->bind();
  vLayout->addWidget(m_DieselDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);


  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Propane (%)");
  vLayout->addWidget(label);

  m_PropaneDoubleEdit = new OSDoubleEdit();
  m_PropaneDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  //m_PropaneDoubleEdit->bind();
  vLayout->addWidget(m_PropaneDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);


  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Coal (%)");
  vLayout->addWidget(label);

  m_CoalDoubleEdit = new OSDoubleEdit();
  m_CoalDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  //m_CoalDoubleEdit->bind();
  vLayout->addWidget(m_CoalDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);


  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Fuel Oil #1 (%)");
  vLayout->addWidget(label);

  m_FuelOil_1DoubleEdit = new OSDoubleEdit();
  m_FuelOil_1DoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  //m_FuelOil_1DoubleEdit->bind();
  vLayout->addWidget(m_FuelOil_1DoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount,Qt::AlignLeft);


  rowCount++;
  columnCount = 0;

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Fuel Oil #2 (%)");
  vLayout->addWidget(label);

  m_FuelOil_2DoubleEdit = new OSDoubleEdit();
  m_FuelOil_2DoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  //m_FuelOil_2DoubleEdit->bind();
  vLayout->addWidget(m_FuelOil_2DoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount,columnCount++,Qt::AlignLeft);


  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Water (%)");
  vLayout->addWidget(label);

  m_WaterDoubleEdit = new OSDoubleEdit();
  m_WaterDoubleEdit->setFixedWidth(OS_EDIT_WIDTH);
  //m_WaterDoubleEdit->bind();
  vLayout->addWidget(m_WaterDoubleEdit);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,rowCount++,columnCount,Qt::AlignLeft);

  gridLayout->setColumnStretch(100,100);

  OS_ASSERT(m_fempGroup->button(0));
  m_fempGroup->button(0)->setChecked(true);

  OS_ASSERT(m_nistGroup->button(0));
  m_nistGroup->button(0)->setChecked(true);

  return widget;

}

QWidget * LifeCycleCostsView::createNistWidget()
{
  bool isConnected = false;

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

  m_nistRegionComboBox = new QComboBox();
  m_nistRegionComboBox->addItem("region 1");
  m_nistRegionComboBox->addItem("region 2");
  vLayout->addWidget(m_nistRegionComboBox);

  isConnected = connect(m_nistRegionComboBox, SIGNAL(currentIndexChanged(const QString&)),
                                        this, SLOT(nistRegionChanged(const QString&)));
  OS_ASSERT(isConnected);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,0,0,Qt::AlignLeft);


  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("NIST Sector");

  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_nistSectorComboBox = new QComboBox();
  m_nistSectorComboBox->addItem("sector 1");
  m_nistSectorComboBox->addItem("sector 2");
  vLayout->addWidget(m_nistSectorComboBox);

  isConnected = connect(m_nistSectorComboBox, SIGNAL(currentIndexChanged(const QString&)),
                                        this, SLOT(nistSectorChanged(const QString&)));
  OS_ASSERT(isConnected);

  vLayout->addStretch();
  gridLayout->addLayout(vLayout,0,1,Qt::AlignLeft);

  gridLayout->setColumnStretch(100,100);

  return widget;

}

////// SLOTS ///////

void LifeCycleCostsView::fempGroupClicked(int index)
{
  m_RealDiscountRateDoubleEdit->setEnabled(index);

  // Check to set FEMP rate
  if(index == 0){
    m_RealDiscountRateDoubleEdit->setText("3"); // TODO call actual number
  }
}

void LifeCycleCostsView::nistGroupClicked(int index)
{
  m_stackedWidget->setCurrentIndex(index);
}

void LifeCycleCostsView::nistRegionChanged(const QString& string)
{
}

void LifeCycleCostsView::nistSectorChanged(const QString& string)
{
}

} // openstudio
