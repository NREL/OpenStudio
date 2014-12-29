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

#ifndef OPENSTUDIO_CONSTRUCTIONBASEINSPECTORVIEW_HPP
#define OPENSTUDIO_CONSTRUCTIONBASEINSPECTORVIEW_HPP

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

class OSLineEdit;

class OSComboBox2;

class OSSwitch2;

class StandardsInformationConstructionWidget : public QWidget
{
  Q_OBJECT

  public:

    StandardsInformationConstructionWidget(bool isIP, QWidget * parent = 0);

    virtual ~StandardsInformationConstructionWidget() {}

    void attach(openstudio::model::ConstructionBase & construction);

    void detach();

    void addToLayout(QGridLayout* mainGridLayout, int& row);

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

    QComboBox * m_standard;

    QComboBox * m_standardSource;

    OSComboBox2 * m_intendedSurfaceType;

    QComboBox * m_standardsConstructionType;

    OSComboBox2 * m_fenestrationType;
    QLabel * m_fenestrationTypeLabel = nullptr;

    OSComboBox2 * m_fenestrationAssemblyContext;
    QLabel * m_fenestrationAssemblyContextLabel = nullptr;

    OSComboBox2 * m_fenestrationNumberOfPanes;
    QLabel * m_fenestrationNumberOfPanesLabel = nullptr;

    OSComboBox2 * m_fenestrationFrameType;
    QLabel * m_fenestrationFrameTypeLabel = nullptr;

    OSComboBox2 * m_fenestrationDividerType;
    QLabel * m_fenestrationDividerTypeLabel = nullptr;

    OSComboBox2 * m_fenestrationTint;
    QLabel * m_fenestrationTintLabel = nullptr;

    OSComboBox2 * m_fenestrationGasFill;
    QLabel * m_fenestrationGasFillLabel = nullptr;

    OSSwitch2 * m_fenestrationLowEmissivityCoating;
    QLabel * m_fenestrationLowEmissivityCoatingLabel = nullptr;

    bool m_isIP;

    boost::optional<openstudio::model::ConstructionBase> m_construction;

    boost::optional<openstudio::model::StandardsInformationConstruction> m_standardsInformation;

};

} // openstudio

#endif // OPENSTUDIO_CONSTRUCTIONBASEINSPECTORVIEW_HPP

