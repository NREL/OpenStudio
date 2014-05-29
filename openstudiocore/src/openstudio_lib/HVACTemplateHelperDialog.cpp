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

  QVBoxLayout * mainVLayout = new QVBoxLayout();
  mainVLayout->setSpacing(20);
  setLayout(mainVLayout);

  QLabel * message = new QLabel("Select zones to apply system to.");
  mainVLayout->addWidget(message);

  QFrame * divider1 = new QFrame();
  divider1->setFrameShape(QFrame::HLine);
  divider1->setFrameShadow(QFrame::Sunken);
  mainVLayout->addWidget(divider1); 

  QScrollArea * scrollArea = new QScrollArea();
  mainVLayout->addWidget(scrollArea);

  QWidget * scrollWidget = new QWidget();
  scrollArea->setWidget(scrollWidget);
  scrollArea->setWidgetResizable(true);
  scrollArea->setFrameShape(QFrame::NoFrame);
  QVBoxLayout * scrollLayout = new QVBoxLayout();
  scrollLayout->setSpacing(20);
  scrollWidget->setLayout(scrollLayout);

  m_buttonGroup = new QButtonGroup();
  m_buttonGroup->setExclusive(false);

  m_zones = m_model.getConcreteModelObjects<model::ThermalZone>();

  int i = 1;

  for( std::vector<model::ThermalZone>::iterator it = m_zones.begin();
       it < m_zones.end();
       ++it )
  {
    QCheckBox * checkBox = new QCheckBox();

    checkBox->setText(QString::fromStdString(it->name().get()));

    m_buttonGroup->addButton(checkBox,i);

    scrollLayout->addWidget(checkBox);

    i++;
  }

  QFrame * divider2 = new QFrame();
  divider2->setFrameShape(QFrame::HLine);
  divider2->setFrameShadow(QFrame::Sunken);
  mainVLayout->addWidget(divider2); 

  mainVLayout->addStretch();

  QHBoxLayout * hLayout = new QHBoxLayout();
  mainVLayout->addLayout(hLayout);

  hLayout->addStretch();

  QPushButton * cancelButton = new QPushButton();
  cancelButton->setText("Cancel");
  hLayout->addWidget(cancelButton);
  connect(cancelButton,SIGNAL(clicked(bool)),this,SLOT(reject()));

  QPushButton * okButton = new QPushButton();
  okButton->setText("OK");
  hLayout->addWidget(okButton);
  connect(okButton,SIGNAL(clicked(bool)),this,SLOT(accept()));
}

std::vector<model::ThermalZone> HVACTemplateHelperDialog::selectedZones()
{
  std::vector<model::ThermalZone> result;

  int i = 1;

  for( std::vector<model::ThermalZone>::iterator it = m_zones.begin();
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

