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

#ifndef OPENSTUDIO_STANDARDSINFORMATIONCONSTRUCTIONWIDGET_HPP
#define OPENSTUDIO_STANDARDSINFORMATIONCONSTRUCTIONWIDGET_HPP

#include "../model/ConstructionBase.hpp"
#include "../model/StandardsInformationConstruction.hpp"

#include <QWidget>

class QComboBox;

class QGridLayout;

class QLabel;

namespace openstudio {

namespace model {

 class ConstructionBase;

}

class OSLineEdit2;

class OSComboBox2;

class OSSwitch2;

class StandardsInformationConstructionWidget : public QWidget
{
  Q_OBJECT

  public:

    StandardsInformationConstructionWidget(bool isIP, QGridLayout* mainGridLayout, int& row);

    virtual ~StandardsInformationConstructionWidget();

    void attach(openstudio::model::ConstructionBase & construction);

    void detach();

    void showFenestration();

    void hideFenestration();

    void enableFenestration();

    void disableFenestration();

  public slots:

    void toggleUnits(bool displayIP);

  private slots:

    void standardChanged(const QString& text);
    void populateStandards();

    void standardSourceChanged(const QString& text);
    void populateStandardSources();

    void intendedSurfaceTypeChanged(const QString& text);

    void standardsConstructionTypeChanged(const QString& text);
    void populateStandardsConstructionType();

  private:

    QComboBox * m_standard = nullptr;

    QComboBox * m_standardSource = nullptr;

    OSComboBox2 * m_intendedSurfaceType = nullptr;

    QComboBox * m_standardsConstructionType = nullptr;

    OSComboBox2 * m_fenestrationType = nullptr;
    QLabel * m_fenestrationTypeLabel = nullptr;

    OSComboBox2 * m_fenestrationAssemblyContext = nullptr;
    QLabel * m_fenestrationAssemblyContextLabel = nullptr;

    OSComboBox2 * m_fenestrationNumberOfPanes = nullptr;
    QLabel * m_fenestrationNumberOfPanesLabel = nullptr;

    OSComboBox2 * m_fenestrationFrameType = nullptr;
    QLabel * m_fenestrationFrameTypeLabel = nullptr;

    OSComboBox2 * m_fenestrationDividerType = nullptr;
    QLabel * m_fenestrationDividerTypeLabel = nullptr;

    OSComboBox2 * m_fenestrationTint = nullptr;
    QLabel * m_fenestrationTintLabel = nullptr;

    OSComboBox2 * m_fenestrationGasFill = nullptr;
    QLabel * m_fenestrationGasFillLabel = nullptr;

    OSSwitch2 * m_fenestrationLowEmissivityCoating = nullptr;
    QLabel * m_fenestrationLowEmissivityCoatingLabel = nullptr;

    bool m_isIP = false;
    bool m_fenestrationEnabled = false;

    boost::optional<openstudio::model::ConstructionBase> m_construction;

    boost::optional<openstudio::model::StandardsInformationConstruction> m_standardsInformation;

};

} // openstudio

#endif // OPENSTUDIO_STANDARDSINFORMATIONCONSTRUCTIONWIDGET_HPP

