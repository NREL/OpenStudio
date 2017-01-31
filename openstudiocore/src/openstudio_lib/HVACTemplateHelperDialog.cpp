/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "HVACTemplateHelperDialog.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>

namespace openstudio {

HVACTemplateHelperDialog::HVACTemplateHelperDialog(const model::Model & model, QWidget * parent)
  : QDialog(parent),
    m_model(model)
{
  setMinimumSize(200,200);

  setSizeGripEnabled(false);

  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setSpacing(20);
  setLayout(mainVLayout);

  QLabel * message = new QLabel("Select zones to apply system to.");
  mainVLayout->addWidget(message);

  auto divider1 = new QFrame();
  divider1->setFrameShape(QFrame::HLine);
  divider1->setFrameShadow(QFrame::Sunken);
  mainVLayout->addWidget(divider1); 

  auto scrollArea = new QScrollArea();
  mainVLayout->addWidget(scrollArea);

  auto scrollWidget = new QWidget();
  scrollArea->setWidget(scrollWidget);
  scrollArea->setWidgetResizable(true);
  scrollArea->setFrameShape(QFrame::NoFrame);
  auto scrollLayout = new QVBoxLayout();
  scrollLayout->setSpacing(20);
  scrollWidget->setLayout(scrollLayout);

  m_buttonGroup = new QButtonGroup();
  m_buttonGroup->setExclusive(false);

  m_zones = m_model.getConcreteModelObjects<model::ThermalZone>();

  int i = 1;

  for( auto it = m_zones.begin();
       it < m_zones.end();
       ++it )
  {
    auto checkBox = new QCheckBox();

    checkBox->setText(QString::fromStdString(it->name().get()));

    m_buttonGroup->addButton(checkBox,i);

    scrollLayout->addWidget(checkBox);

    i++;
  }

  auto divider2 = new QFrame();
  divider2->setFrameShape(QFrame::HLine);
  divider2->setFrameShadow(QFrame::Sunken);
  mainVLayout->addWidget(divider2); 

  mainVLayout->addStretch();

  auto hLayout = new QHBoxLayout();
  mainVLayout->addLayout(hLayout);

  hLayout->addStretch();

  auto cancelButton = new QPushButton();
  cancelButton->setText("Cancel");
  hLayout->addWidget(cancelButton);
  connect(cancelButton, &QPushButton::clicked, this, &HVACTemplateHelperDialog::reject);

  auto okButton = new QPushButton();
  okButton->setText("OK");
  hLayout->addWidget(okButton);
  connect(okButton, &QPushButton::clicked, this, &HVACTemplateHelperDialog::accept);
}

std::vector<model::ThermalZone> HVACTemplateHelperDialog::selectedZones()
{
  std::vector<model::ThermalZone> result;

  int i = 1;

  for( auto it = m_zones.begin();
       it < m_zones.end();
       ++it )
  {
    QCheckBox * checkBox = qobject_cast<QCheckBox *>(m_buttonGroup->button(i));

    if( checkBox->isChecked() )
    {
      result.push_back( *it );
    }

    i++;
  }
   
  return result;
}

} // openstudio

