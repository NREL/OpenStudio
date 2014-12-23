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

#ifndef OPENSTUDIO_STANDARDSINFORMATIONMATERIALWIDGET_HPP
#define OPENSTUDIO_STANDARDSINFORMATIONMATERIALWIDGET_HPP

#include "../model/Material.hpp"

#include "../model/StandardsInformationMaterial.hpp"

#include <QWidget>

class QComboBox;

class QGridLayout;

namespace openstudio {

namespace model {

  class Material;

}

class OSComboBox;

class OSLineEdit;

class StandardsInformationMaterialWidget : public QWidget
{
  Q_OBJECT

  public:

    StandardsInformationMaterialWidget(bool isIP, QWidget * parent = 0);

    virtual ~StandardsInformationMaterialWidget() {}

    void attach(openstudio::model::Material & material);

    void detach();

    void addToLayout(QGridLayout * mainGridLayout, int & row);

  private:
    
    QComboBox * m_standard = nullptr;

    QComboBox * m_standardSource = nullptr;

    QComboBox * m_standardsCategory = nullptr;

    QComboBox * m_standardsIdentifier = nullptr;

    bool m_isIP;

    boost::optional<openstudio::model::Material> m_material;

    boost::optional<openstudio::model::StandardsInformationMaterial> m_standardsInformation;

  public slots:

    void toggleUnits(bool displayIP);

  private slots:

    void standardChanged(const QString& text);
    void populateStandards();

    void standardSourceChanged(const QString& text);
    void populateStandardSources();

    void standardsCategoryChanged(const QString& text);
    void populateStandardsCategories();

    void standardsIdentifierChanged(const QString& text);
    void populateStandardsIdentifier();

};

} // openstudio

#endif // OPENSTUDIO_STANDARDSINFORMATIONMATERIALWIDGET_HPP

