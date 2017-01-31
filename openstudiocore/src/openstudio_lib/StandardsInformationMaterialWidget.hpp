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

class OSComboBox2;

class OSLineEdit2;

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

