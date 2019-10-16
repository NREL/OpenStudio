/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "ConstructionAirBoundaryInspectorView.hpp"
#include "StandardsInformationConstructionWidget.hpp"

#include "OSItem.hpp"

#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "../shared_gui_components/OSComboBox.hpp"

#include "../model/ConstructionAirBoundary.hpp"
#include "../model/ConstructionAirBoundary_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QComboBox>

namespace openstudio {

ConstructionAirBoundaryInspectorView::ConstructionAirBoundaryInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_isIP(isIP),
    m_nameEdit(nullptr),
    m_standardsInformationWidget(nullptr),
    m_solarAndDaylightingMethod(nullptr),
    m_radiantExchangeMethod(nullptr),
    m_airExchangeMethod(nullptr),
    m_simpleMixingAirChangesPerHour(nullptr),
    m_scheduleDropZone(nullptr)
{
  createLayout();
}

void ConstructionAirBoundaryInspectorView::createLayout()
{
  auto hiddenWidget = new QWidget();
  this->stackedWidget()->addWidget(hiddenWidget);

  auto visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  auto mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7, 7, 7, 7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  int row = mainGridLayout->rowCount();

  QLabel * label = nullptr;

  // Name

  label = new QLabel("Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_nameEdit = new OSLineEdit2();
  mainGridLayout->addWidget(m_nameEdit, row, 0, 1, 3);

  ++row;

  // Standards Information

  m_standardsInformationWidget = new StandardsInformationConstructionWidget(m_isIP, mainGridLayout, row);
  m_standardsInformationWidget->hideFenestration();
  m_standardsInformationWidget->disableFenestration();

  ++row;

  // Solar and Daylighting Method

  label = new QLabel("Solar and Daylighting Method: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_solarAndDaylightingMethod = new OSComboBox2(nullptr, true);
  mainGridLayout->addWidget(m_solarAndDaylightingMethod, row, 0);

  ++row;

  // Radiant Exchange Method

  label = new QLabel("Radiant Exchange Method: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_radiantExchangeMethod = new OSComboBox2(nullptr, true);
  mainGridLayout->addWidget(m_radiantExchangeMethod, row, 0);

  ++row;

  // Air Exchange Method

  label = new QLabel("Air Exchange Method: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_airExchangeMethod = new OSComboBox2(nullptr, true);
  mainGridLayout->addWidget(m_airExchangeMethod, row, 0);

  ++row;

  // Simple Mixing Air Changes per Hour

  label = new QLabel("Simple Mixing Air Changes per Hour: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_simpleMixingAirChangesPerHour = new OSQuantityEdit2("1/hr", "1/hr", "1/hr", m_isIP);
  mainGridLayout->addWidget(m_simpleMixingAirChangesPerHour, row, 0);

  ++row;

  // Simple Mixing Schedule

  //label = new QLabel("Simple Mixing Schedule: ");
  //label->setObjectName("H2");
  //mainGridLayout->addWidget(label, row, 0);

  //++row;

  //m_scheduleDropZone = new OSDropZone("", "", "", m_isIP);
  //mainGridLayout->addWidget(m_scheduleDropZone, row, 0);

  //++row;

  // Stretch

  mainGridLayout->setRowStretch(row, 100);

  mainGridLayout->setColumnStretch(100, 100);
}

void ConstructionAirBoundaryInspectorView::onClearSelection()
{
  detach();

  this->stackedWidget()->setCurrentIndex(0);
}

void ConstructionAirBoundaryInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::ConstructionAirBoundary construction = modelObject.cast<model::ConstructionAirBoundary>();
  attach(construction);


  this->stackedWidget()->setCurrentIndex(1);
}

void ConstructionAirBoundaryInspectorView::onUpdate()
{
}

void ConstructionAirBoundaryInspectorView::attach(openstudio::model::ConstructionAirBoundary & constructionAirBoundary)
{
  m_constructionAirBoundary = constructionAirBoundary;

  m_nameEdit->bind(
    *m_constructionAirBoundary,
    OptionalStringGetter(std::bind(&model::ConstructionAirBoundary::name, m_constructionAirBoundary.get_ptr(), true)),
    boost::optional<StringSetter>(std::bind(&model::ConstructionAirBoundary::setName, m_constructionAirBoundary.get_ptr(), std::placeholders::_1))
  );

  m_solarAndDaylightingMethod->bind<std::string>(
    *m_constructionAirBoundary,
    static_cast<std::string(*)(const std::string&)>(&openstudio::toString),
    &model::ConstructionAirBoundary::solarAndDaylightingMethodValues,
    std::bind(&model::ConstructionAirBoundary::solarAndDaylightingMethod, m_constructionAirBoundary.get_ptr()),
    std::bind(&model::ConstructionAirBoundary::setSolarAndDaylightingMethod, m_constructionAirBoundary.get_ptr(), std::placeholders::_1),
    boost::optional<NoFailAction>(std::bind(&model::ConstructionAirBoundary::resetSolarAndDaylightingMethod, m_constructionAirBoundary.get_ptr())),
    boost::optional<BasicQuery>(std::bind(&model::ConstructionAirBoundary::isSolarAndDaylightingMethodDefaulted, m_constructionAirBoundary.get_ptr())));

  m_radiantExchangeMethod->bind<std::string>(
    *m_constructionAirBoundary,
    static_cast<std::string(*)(const std::string&)>(&openstudio::toString),
    &model::ConstructionAirBoundary::radiantExchangeMethodValues,
    std::bind(&model::ConstructionAirBoundary::radiantExchangeMethod, m_constructionAirBoundary.get_ptr()),
    std::bind(&model::ConstructionAirBoundary::setRadiantExchangeMethod, m_constructionAirBoundary.get_ptr(), std::placeholders::_1),
    boost::optional<NoFailAction>(std::bind(&model::ConstructionAirBoundary::resetRadiantExchangeMethod, m_constructionAirBoundary.get_ptr())),
    boost::optional<BasicQuery>(std::bind(&model::ConstructionAirBoundary::isRadiantExchangeMethodDefaulted, m_constructionAirBoundary.get_ptr())));

  m_airExchangeMethod->bind<std::string>(
    *m_constructionAirBoundary,
    static_cast<std::string(*)(const std::string&)>(&openstudio::toString),
    &model::ConstructionAirBoundary::airExchangeMethodValues,
    std::bind(&model::ConstructionAirBoundary::airExchangeMethod, m_constructionAirBoundary.get_ptr()),
    std::bind(&model::ConstructionAirBoundary::setAirExchangeMethod, m_constructionAirBoundary.get_ptr(), std::placeholders::_1),
    boost::optional<NoFailAction>(std::bind(&model::ConstructionAirBoundary::resetAirExchangeMethod, m_constructionAirBoundary.get_ptr())),
    boost::optional<BasicQuery>(std::bind(&model::ConstructionAirBoundary::isAirExchangeMethodDefaulted, m_constructionAirBoundary.get_ptr())));

  m_simpleMixingAirChangesPerHour->bind(
    m_isIP,
    *m_constructionAirBoundary,
    DoubleGetter(std::bind(&model::ConstructionAirBoundary::simpleMixingAirChangesPerHour, m_constructionAirBoundary.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::ConstructionAirBoundary::*)(double)>(&model::ConstructionAirBoundary::setSimpleMixingAirChangesPerHour), m_constructionAirBoundary.get_ptr(), std::placeholders::_1))
  );

  m_standardsInformationWidget->attach(m_constructionAirBoundary.get());
}

void ConstructionAirBoundaryInspectorView::detach()
{
  m_nameEdit->unbind();
  m_solarAndDaylightingMethod->unbind();
  m_radiantExchangeMethod->unbind();
  m_airExchangeMethod->unbind();
  m_simpleMixingAirChangesPerHour->unbind();

  m_standardsInformationWidget->detach();

  m_constructionAirBoundary = boost::none;
}

} // openstudio

