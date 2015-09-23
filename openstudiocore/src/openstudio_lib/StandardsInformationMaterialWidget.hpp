/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

class QLabel;

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

    StandardsInformationMaterialWidget(bool isIP, QGridLayout * mainGridLayout, int & row);

    virtual ~StandardsInformationMaterialWidget();

    void attach(openstudio::model::Material & material);

    void detach();

    void showComposite();

    void hideComposite();

    void enableComposite();

    void disableComposite();

  private:
    
    QComboBox * m_standard = nullptr;

    QComboBox * m_standardSource = nullptr;

    QComboBox * m_standardsCategory = nullptr;

    QComboBox * m_standardsIdentifier = nullptr;

    QComboBox * m_compositeFramingMaterial = nullptr;
    QLabel * m_compositeFramingMaterialLabel = nullptr;

    QComboBox * m_compositeFramingConfiguration = nullptr;
    QLabel * m_compositeFramingConfigurationLabel = nullptr;

    QComboBox * m_compositeFramingDepth = nullptr;
    QLabel * m_compositeFramingDepthLabel = nullptr;

    QComboBox * m_compositeFramingSize = nullptr;
    QLabel * m_compositeFramingSizeLabel = nullptr;

    QComboBox * m_compositeCavityInsulation = nullptr;
    QLabel * m_compositeCavityInsulationLabel = nullptr;

    bool m_isIP = false;

    boost::optional<openstudio::model::Material> m_material;

    boost::optional<openstudio::model::StandardsInformationMaterial> m_standardsInformation;

    bool m_populateFieldsRequested = false;

    void requestPopulateFields();

  public slots:

    void toggleUnits(bool displayIP);

    void populateFields();

  private slots:

    void standardChanged(const QString& text);
    void populateStandards();

    void standardSourceChanged(const QString& text);
    void populateStandardSources();

    void standardsCategoryChanged(const QString& text);
    void populateStandardsCategories();

    void standardsIdentifierChanged(const QString& text);
    void populateStandardsIdentifier();

    void compositeFramingMaterialChanged(const QString& text);
    void populateCompositeFramingMaterial();

    void compositeFramingConfigurationChanged(const QString& text);
    void populateCompositeFramingConfiguration();

    void compositeFramingDepthChanged(const QString& text);
    void populateCompositeFramingDepth();

    void compositeFramingSizeChanged(const QString& text);
    void populateCompositeFramingSize();

    void compositeCavityInsulationChanged(const QString& text);
    void populateCompositeCavityInsulation();
};

} // openstudio

#endif // OPENSTUDIO_STANDARDSINFORMATIONMATERIALWIDGET_HPP

