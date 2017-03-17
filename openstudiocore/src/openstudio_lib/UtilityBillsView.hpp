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

#ifndef OPENSTUDIO_UTILITYBILLSVIEW_HPP
#define OPENSTUDIO_UTILITYBILLSVIEW_HPP

#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement
#include "ModelObjectInspectorView.hpp"
#include "ModelSubTabView.hpp"

#include "../model/Model.hpp"
#include "../model/UtilityBill.hpp"

#include "../utilities/data/DataEnums.hpp"

#include <QWidget>

class QButtonGroup;
class QDate;
class QDateEdit;
class QLabel;
class QLineEdit;
class QPushButton;
class QString;
class QVBoxLayout;

namespace openstudio {

class BillFormatDialog;
class BillingPeriod;
class Date;
class FuelType;
class OSComboBox2;
class OSDoubleEdit2;
class OSIntegerEdit2;
class OSLineEdit2;
class OSUnsignedEdit2;
class UtilityBillListView;

class BillingPeriodWidget;

enum BillFormat{
  STARTDATE_ENDDATE,
  STARTDATE_NUMDAYS,
  ENDDATE_NUMDAYS
};

class UtilityBillsView : public ModelSubTabView
{
  Q_OBJECT

public:

  UtilityBillsView(const model::Model & model,
    QWidget * parent = nullptr);

  virtual ~UtilityBillsView();

private:

  void resetUtilityBills();

  static std::vector<std::pair<FuelType, std::string> > utilityBillFuelTypesAndNames();

signals:

  void enableAddNewObjectButton(bool enable);

};

class UtilityBillsInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

public:

  UtilityBillsInspectorView(const openstudio::model::Model& model,
    bool addScrollArea,
    QWidget * parent = nullptr);

  virtual ~UtilityBillsInspectorView();

  boost::optional<QString> runPeriodDates();

protected:

  virtual void onSelectItem(OSItem *item) override;
  virtual void onClearSelection() override;
  virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject) override;
  virtual void onUpdate() override;

protected slots:

  //virtual void toggleUnits(bool displayIP);

private:

  void createWidgets();
  void getSortedBills();
  void showAddButton();
  void hideAddButton();
  void enableAddButton();
  void disableAddButton();
  void attach(openstudio::model::UtilityBill & utilityBill);
  void detach();
  void refresh();

  void createBillingPeriodHeaderWidget();
  void addBillingPeriodWidget(model::BillingPeriod & billingPeriod);
  void addBillingPeriodWidgets();

  void deleteBillingPeriodWidgets();

  QString getEnergyUseLabelText();
  QString getPeakLabelText();

  void updateEnergyUseLabelText();
  void updatePeakLabelText();

  boost::optional<model::UtilityBill> m_utilityBill;

  std::vector<BillingPeriodWidget *> m_billingPeriodWidgets;

  BillFormat m_billFormat;

  bool m_showPeak;

  QWidget * m_billingPeriodHeaderWidget;
  
  QButtonGroup * m_buttonGroup;

  OSLineEdit2 * m_name;

  QLabel * m_consumptionUnitsLabel;
  OSComboBox2 * m_consumptionUnits;
  QLabel * m_peakDemandUnitsLabel;
  OSComboBox2 * m_peakDemandUnits;
  QLabel * m_windowTimestepsLabel;
  OSUnsignedEdit2 * m_windowTimesteps;

  QPushButton * m_addBillingPeriod;

  QVBoxLayout * m_billPeriodLayout;

  QWidget * m_billPeriodLayoutWidget;

  QLabel * m_runPeriodDatesLabel;

  QLabel * m_energyUseLabel;
  QLabel * m_peakLabel;

  QString m_energyUseUnits;
  QString m_peakUnits;

  int m_hiddenWidgetIndex;
  int m_visibleWidgetIndex;
  
signals:
  void showSubTabView(bool showSubTabView);
  void enableAddNewObjectButton(bool enable);

private slots:
  void addBillingPeriod(bool checked);
  void deleteBillingPeriod(int index);
  void setBillFormat(int index);
  void setBillFormat(BillFormat billFormat);
  void updateEnergyUseLabelText(const QString& text);
  void updatePeakLabelText(const QString& text);
  void updateRunPeriodDates();
  void deleteAllWidgetsAndLayoutItems(QLayout * layout, bool deleteWidgets = true);

};

class BillingPeriodWidget : public QWidget, public Nano::Observer
{

  Q_OBJECT

public:

  BillingPeriodWidget(model::BillingPeriod billingPeriod,
    FuelType fuelType,
    BillFormat billFormat,
    QWidget * parent = nullptr);

  virtual ~BillingPeriodWidget() {}

  void attach(openstudio::model::BillingPeriod & billingPeriod);
  void detach();

  QDateEdit * m_startDateEdit;
  QDateEdit * m_endDateEdit;

  OSIntegerEdit2 * m_billingPeriodIntEdit;
  OSDoubleEdit2 * m_energyUseDoubleEdit;
  OSDoubleEdit2 * m_peakDoubleEdit;
  OSDoubleEdit2 * m_costDoubleEdit;

  QPushButton * m_deleteBillWidget;

private:

  void createWidgets(FuelType fuelType,
    BillFormat billFormat);

  boost::optional<model::BillingPeriod> m_billingPeriod;

private slots:

  void modelObjectChanged();
  void startDateChanged(const QDate & newdate);
  void endDateChanged(const QDate & newdate);

};

} // openstudio

#endif // OPENSTUDIO_UTILITYBILLSVIEW_HPP
