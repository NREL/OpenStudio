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

#include "DesignDayGridView.hpp"

#include "../shared_gui_components/OSGridView.hpp"

#include "ModelObjectItem.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"


#include "../model/DesignDay.hpp"
#include "../model/DesignDay_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"

#include "../utilities/idd/IddEnums.hxx"
#include "../utilities/idd/SizingPeriod_DesignDay_FieldEnums.hxx"

#include <QBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QScrollArea>
#include <QSettings>
#include <QTimer>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

#define NAME "Some Object Name"
#define SELECTED "All"

// DATE
#define DAYOFMONTH "Day Of Month"
#define MONTH "Month"
#define DAYTYPE "Day Type"
#define DAYLIGHTSAVINGTIMEINDICATOR "Daylight Saving Time Indicator"

// TEMPERATURE
#define MAXIMUMDRYBULBTEMPERATURE "Maximum Dry Bulb Temperature"
#define DAILYDRYBULBTEMPERATURERANGE "Daily Dry Bulb Temperature Range"
#define DAILYWETBULBTEMPERATURERANGE "Daily Wet Bulb Temperature Range"
#define DRYBULBTEMPERATURERANGEMODIFIERTYPE "Dry Bulb Temperature Range Modifier Type"
#define DRYBULBTEMPERATURERANGEMODIFIERSCHEDULE "Dry Bulb Temperature Range Modifier Schedule"

// HUMIDITY
#define HUMIDITYINDICATINGCONDITIONSATMAXIMUMDRYBULB "Humidity Indicating Conditions At Maximum Dry Bulb"
#define HUMIDITYINDICATINGTYPE "Humidity Indicating Type"
#define HUMIDITYINDICATINGDAYSCHEDULE "Humidity Indicating Day Schedule"

// PRESSURE / WIND / PRECIP
#define BAROMETRICPRESSURE "Barometric Pressure"
#define WINDSPEED "Wind Speed"
#define WINDDIRECTION "Wind Direction"
#define RAININDICATOR "Rain Indicator"
#define SNOWINDICATOR "Snow Indicator"
#define SKYCLEARNESS "Sky Clearness"

// SOLAR
#define SOLARMODELINDICATOR "Solar Model Indicator"
#define BEAMSOLARDAYSCHEDULE "Beam Solar Day Schedule"
#define DIFFUSESOLARDAYSCHEDULE "Diffuse Solar Day Schedule"
#define ASHRAETAUB "ASHRAE Taub"
#define ASHRAETAUD "ASHRAE Taud"

namespace openstudio {

struct ModelObjectNameSorter{
  // sort by name
  bool operator()(const model::ModelObject & lhs, const model::ModelObject & rhs){
    return (lhs.name() < rhs.name());
  }
};

DesignDayGridView::DesignDayGridView(bool isIP, const model::Model & model, QWidget * parent)
  : QWidget(parent),
  m_isIP(isIP)
{
  auto layout = new QVBoxLayout();
  layout->setSpacing(0);
  layout->setContentsMargins(0,0,0,0);
  setLayout(layout);
 
  auto designDays = model.getModelObjects<model::DesignDay>();
  auto designDayModelObjects = subsetCastVector<model::ModelObject>(designDays);

  auto designDayGridController = new DesignDayGridController(m_isIP, "Design Days", IddObjectType::OS_SizingPeriod_DesignDay, model, designDayModelObjects);
  auto designDayGridView = new OSGridView(designDayGridController, "Design Days", "Drop\nDesign Day", true, parent);

  bool isConnected = false;

  //isConnected = connect(caseGridView, SIGNAL(dropZoneItemClicked(OSItem*)), this, SIGNAL(dropZoneItemClicked(OSItem*)));
  //OS_ASSERT(isConnected);

  //scrollLayout->addWidget(caseGridView,0,Qt::AlignTop);

  //isConnected = connect(walkInView, SIGNAL(dropZoneItemClicked(OSItem*)), this, SIGNAL(dropZoneItemClicked(OSItem*)));
  //OS_ASSERT(isConnected);

  //scrollLayout->addWidget(walkInView,0,Qt::AlignTop);

  //scrollLayout->addStretch(1);

  //connect(this, &DesignDayGridView::toggleUnitsClicked, refrigerationCaseGridController, &DesignDayGridController::toggleUnitsClicked);
  //
  //connect(this, &DesignDayGridView::toggleUnitsClicked, refrigerationCaseGridController, &DesignDayGridController::toggleUnits);

  //std::vector<model::DesignDaySystem> refrigerationSystems = model.getModelObjects<model::DesignDaySystem>(); // NOTE for horizontal system list

}

DesignDayGridController::DesignDayGridController(bool isIP,
  const QString & headerText,
  IddObjectType iddObjectType,
  model::Model model,
  std::vector<model::ModelObject> modelObjects) :
  OSGridController(isIP, headerText, iddObjectType, model, modelObjects)
{
  setCategoriesAndFields();
}

void DesignDayGridController::setCategoriesAndFields()
{
  {
    std::vector<QString> fields;
    fields.push_back(DAYOFMONTH);
    fields.push_back(MONTH);
    fields.push_back(DAYTYPE);
    fields.push_back(DAYLIGHTSAVINGTIMEINDICATOR);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Date"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(MAXIMUMDRYBULBTEMPERATURE);
    fields.push_back(DAILYDRYBULBTEMPERATURERANGE);
    fields.push_back(DAILYWETBULBTEMPERATURERANGE);
    fields.push_back(DRYBULBTEMPERATURERANGEMODIFIERTYPE);
    //fields.push_back(DRYBULBTEMPERATURERANGEMODIFIERSCHEDULE);
    std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Temperature"), fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(HUMIDITYINDICATINGCONDITIONSATMAXIMUMDRYBULB);
    fields.push_back(HUMIDITYINDICATINGTYPE);
    //fields.push_back(HUMIDITYINDICATINGDAYSCHEDULE);
    std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Humidity"), fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(BAROMETRICPRESSURE);
    fields.push_back(WINDSPEED);
    fields.push_back(WINDDIRECTION);
    fields.push_back(RAININDICATOR);
    fields.push_back(SNOWINDICATOR);
    fields.push_back(SKYCLEARNESS);
    std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Pressure, Wind, Precipitation"), fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(SOLARMODELINDICATOR);
    //fields.push_back(BEAMSOLARDAYSCHEDULE);
    //fields.push_back(DIFFUSESOLARDAYSCHEDULE);
    fields.push_back(ASHRAETAUB);
    fields.push_back(ASHRAETAUD);
    std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString(""), fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  OSGridController::setCategoriesAndFields();

}

void DesignDayGridController::addColumns(const QString &/*category*/, std::vector<QString> & fields)
{
  // always show name column
  fields.insert(fields.begin(), { NAME, SELECTED });

  m_baseConcepts.clear();

  for (const QString& field : fields) {

    if (field == DAYLIGHTSAVINGTIMEINDICATOR) {
      addCheckBoxColumn(Heading(QString(DAYLIGHTSAVINGTIMEINDICATOR), true, false),
        std::string("Check to enable daylight saving time indicator."),
        NullAdapter(&model::DesignDay::daylightSavingTimeIndicator),
        NullAdapter(&model::DesignDay::setDaylightSavingTimeIndicator)
        );
    }
    else if (field == RAININDICATOR) {
      addCheckBoxColumn(Heading(QString(RAININDICATOR), true, false),
        std::string("Check to enable rain indicator."),
        NullAdapter(&model::DesignDay::rainIndicator),
        NullAdapter(&model::DesignDay::setRainIndicator)
        );
    }
    else if (field == SNOWINDICATOR) {
      addCheckBoxColumn(Heading(QString(SNOWINDICATOR), true, false),
        std::string("Check to enable snow indicator."),
        NullAdapter(&model::DesignDay::snowIndicator),
        NullAdapter(&model::DesignDay::setSnowIndicator)
        );
    }
    else if (field == SELECTED){
      auto checkbox = QSharedPointer<QCheckBox>(new QCheckBox());
      checkbox->setToolTip("Check to select all rows");
      connect(checkbox.data(), &QCheckBox::stateChanged, this, &DesignDayGridController::selectAllStateChanged);

      addSelectColumn(Heading(QString(SELECTED), false, false, checkbox),
        "Check to select this row"
        );
    }
    // INTEGER
    else if (field == DAYOFMONTH) {
      addValueEditColumn(Heading(QString(DAYOFMONTH)),
        NullAdapter(&model::DesignDay::dayOfMonth),
        NullAdapter(&model::DesignDay::setDayOfMonth)
        );
    }
    else if (field == MONTH) {
      addValueEditColumn(Heading(QString(MONTH)),
        NullAdapter(&model::DesignDay::month),
        NullAdapter(&model::DesignDay::setMonth)
        );
    }
    // DOUBLE
    else if (field == SKYCLEARNESS) {
      addValueEditColumn(Heading(QString(SKYCLEARNESS)),
        NullAdapter(&model::DesignDay::skyClearness),
        NullAdapter(&model::DesignDay::setSkyClearness)
        );
    }
    else if (field == ASHRAETAUB) {
      addValueEditColumn(Heading(QString(ASHRAETAUB)),
        NullAdapter(&model::DesignDay::ashraeTaub),
        NullAdapter(&model::DesignDay::setAshraeTaub)
        );
    }
    else if (field == ASHRAETAUD) {
      addValueEditColumn(Heading(QString(ASHRAETAUD)),
        NullAdapter(&model::DesignDay::ashraeTaud),
        NullAdapter(&model::DesignDay::setAshraeTaud)
        );
    }
    else if (field == WINDDIRECTION) {
      addValueEditColumn(Heading(QString(WINDDIRECTION)),
        NullAdapter(&model::DesignDay::windDirection),
        NullAdapter(&model::DesignDay::setWindDirection)
        );
    }
    else if (field == HUMIDITYINDICATINGCONDITIONSATMAXIMUMDRYBULB){
      addValueEditColumn(Heading(QString(HUMIDITYINDICATINGCONDITIONSATMAXIMUMDRYBULB)),
        NullAdapter(&model::DesignDay::humidityIndicatingConditionsAtMaximumDryBulb),
        NullAdapter(&model::DesignDay::setHumidityIndicatingConditionsAtMaximumDryBulb)
        );
    }
    // STRING
    else if (field == DAYTYPE) {
      addValueEditColumn(Heading(QString(DAYTYPE)),
        CastNullAdapter<model::DesignDay>(&model::DesignDay::dayType),
        CastNullAdapter<model::DesignDay>(&model::DesignDay::setDayType)
        );
    }
    else if (field == DRYBULBTEMPERATURERANGEMODIFIERTYPE) {
      addValueEditColumn(Heading(QString(DRYBULBTEMPERATURERANGEMODIFIERTYPE)),
        CastNullAdapter<model::DesignDay>(&model::DesignDay::dryBulbTemperatureRangeModifierType),
        CastNullAdapter<model::DesignDay>(&model::DesignDay::setDryBulbTemperatureRangeModifierType)
        );
    }
    else if (field == HUMIDITYINDICATINGTYPE) {
      addValueEditColumn(Heading(QString(HUMIDITYINDICATINGTYPE)),
        CastNullAdapter<model::DesignDay>(&model::DesignDay::humidityIndicatingType),
        CastNullAdapter<model::DesignDay>(&model::DesignDay::setHumidityIndicatingType)
        );
    }
    else if (field == SOLARMODELINDICATOR) {
      addValueEditColumn(Heading(QString(SOLARMODELINDICATOR)),
        CastNullAdapter<model::DesignDay>(&model::DesignDay::solarModelIndicator),
        CastNullAdapter<model::DesignDay>(&model::DesignDay::setSolarModelIndicator)
        );
    }
    else if (field == NAME) {
      addNameLineEditColumn(Heading(QString(NAME), false, false),
        false,
        false,
        CastNullAdapter<model::DesignDay>(&model::DesignDay::name),
        CastNullAdapter<model::DesignDay>(&model::DesignDay::setName)
        );
    }
    else if (field == MAXIMUMDRYBULBTEMPERATURE){
      addQuantityEditColumn(Heading(QString(MAXIMUMDRYBULBTEMPERATURE)),
        QString("C"),
        QString("C"),
        QString("F"),
        m_isIP,
        NullAdapter(&model::DesignDay::maximumDryBulbTemperature),
        NullAdapter(&model::DesignDay::setMaximumDryBulbTemperature)
        );
    }
    else if (field == DAILYDRYBULBTEMPERATURERANGE){
      addQuantityEditColumn(Heading(QString(DAILYDRYBULBTEMPERATURERANGE)),
        QString("C"),
        QString("C"),
        QString("F"),
        m_isIP,
        NullAdapter(&model::DesignDay::dailyDryBulbTemperatureRange),
        NullAdapter(&model::DesignDay::setDailyDryBulbTemperatureRange)
        );
    }
    else if (field == BAROMETRICPRESSURE){
      addQuantityEditColumn(Heading(QString(BAROMETRICPRESSURE)),
        QString("Pa"),
        QString("Pa"),
        QString("inHg"),
        m_isIP,
        NullAdapter(&model::DesignDay::barometricPressure),
        NullAdapter(&model::DesignDay::setBarometricPressure)
        );
    }
    else if (field == WINDSPEED){
      addQuantityEditColumn(Heading(QString(WINDSPEED)),
        QString("m/s"),
        QString("m/s"),
        QString("miles/hr"),
        m_isIP,
        NullAdapter(&model::DesignDay::windSpeed),
        NullAdapter(&model::DesignDay::setWindSpeed)
        );
    }
    else if (field == DAILYWETBULBTEMPERATURERANGE){
      addQuantityEditColumn(Heading(QString(DAILYWETBULBTEMPERATURERANGE)),
        QString("C"),
        QString("C"),
        QString("F"),
        m_isIP,
        NullAdapter(&model::DesignDay::dailyWetBulbTemperatureRange),
        NullAdapter(&model::DesignDay::setDailyWetBulbTemperatureRange)
        );
    }




    else{
      // unhandled
      OS_ASSERT(false);
    }

  }

}
  //boost::optional<ScheduleDay> dryBulbTemperatureRangeModifierSchedule() const;
  //bool setDryBulbTemperatureRangeModifierSchedule(const ScheduleDay & schedule);
  //void resetDryBulbTemperatureRangeModifierSchedule();

  //boost::optional<ScheduleDay> humidityIndicatingDaySchedule() const;
  //bool setHumidityIndicatingDaySchedule(const ScheduleDay & schedule);
  //void resetHumidityIndicatingDaySchedule();

  //boost::optional<ScheduleDay> beamSolarDaySchedule() const;
  //bool setBeamSolarDaySchedule(const ScheduleDay & schedule);
  //void resetBeamSolarDaySchedule();

  //boost::optional<ScheduleDay> diffuseSolarDaySchedule() const;
  //bool setDiffuseSolarDaySchedule(const ScheduleDay & schedule);
  //void resetDiffuseSolarDaySchedule();

 








//bool daylightSavingTimeIndicator() const;
//bool isDaylightSavingTimeIndicatorDefaulted() const;
//bool setDaylightSavingTimeIndicator(bool daylightSavingTimeIndicator);
//void resetDaylightSavingTimeIndicator();

//bool rainIndicator() const;
//bool isRainIndicatorDefaulted() const;
//bool setRainIndicator(bool rainIndicator);
//void resetRainIndicator();

//bool snowIndicator() const;
//bool isSnowIndicatorDefaulted() const;
//bool setSnowIndicator(bool snowIndicator);
//void resetSnowIndicator();

//int dayOfMonth() const;
//bool isDayOfMonthDefaulted() const;
//bool setDayOfMonth(int dayOfMonth);
//void resetDayOfMonth();

//int month() const;
//bool isMonthDefaulted() const;
//bool setMonth(int month);
//void resetMonth();

//std::string dryBulbTemperatureRangeModifierType() const;
//bool isDryBulbTemperatureRangeModifierTypeDefaulted() const;
//bool setDryBulbTemperatureRangeModifierType(std::string dryBulbTemperatureRangeModifierType);
//void resetDryBulbTemperatureRangeModifierType();

//std::string humidityIndicatingType() const;
//bool isHumidityIndicatingTypeDefaulted() const;
//bool setHumidityIndicatingType(std::string humidityIndicatingType);
//void resetHumidityIndicatingType();

//std::string solarModelIndicator() const;
//bool isSolarModelIndicatorDefaulted() const;
//bool setSolarModelIndicator(std::string solarModelIndicator);
//void resetSolarModelIndicator();

//double skyClearness() const;
//bool isSkyClearnessDefaulted() const;
//bool setSkyClearness(double skyClearness);
//void resetSkyClearness();

//double ashraeTaub() const;
//bool isAshraeTaubDefaulted() const;
//bool setAshraeTaub(double aSHRAETaub);
//void resetAshraeTaub();

//double ashraeTaud() const;
//bool isAshraeTaudDefaulted() const;
//bool setAshraeTaud(double aSHRAETaud);
//void resetAshraeTaud();

//double windDirection() const;
//bool isWindDirectionDefaulted() const;
//bool setWindDirection(double windDirection);
//void resetWindDirection();

//double maximumDryBulbTemperature() const;
//bool isMaximumDryBulbTemperatureDefaulted() const;
//bool setMaximumDryBulbTemperature(double maximumDryBulbTemperature);
//void resetMaximumDryBulbTemperature();

//double dailyDryBulbTemperatureRange() const;
//bool isDailyDryBulbTemperatureRangeDefaulted() const;
//bool setDailyDryBulbTemperatureRange(double dailyDryBulbTemperatureRange);
//void resetDailyDryBulbTemperatureRange();

//double humidityIndicatingConditionsAtMaximumDryBulb() const;
//bool isHumidityIndicatingConditionsAtMaximumDryBulbDefaulted() const;
//void setHumidityIndicatingConditionsAtMaximumDryBulb(double humidityIndicatingConditionsAtMaximumDryBulb);
//void resetHumidityIndicatingConditionsAtMaximumDryBulb();

//double barometricPressure() const;
//bool isBarometricPressureDefaulted() const;
//bool setBarometricPressure(double barometricPressure);
//void resetBarometricPressure();

//double windSpeed() const;
//bool isWindSpeedDefaulted() const;
//bool setWindSpeed(double windSpeed);
//void resetWindSpeed();

//boost::optional<double> dailyWetBulbTemperatureRange() const;
//void setDailyWetBulbTemperatureRange(double dailyWetBulbTemperatureRange);
//void resetDailyWetBulbTemperatureRange();




QString DesignDayGridController::getColor(const model:: ModelObject & modelObject)
{
  QColor defaultColor(Qt::lightGray);
  QString color(defaultColor.name());

  // TODO: The code below is currently commented out because a refresh crash bug is precluding rack color
  // updates due to rack assignments to cases and walk-ins.  No colors are better than wrong colors.

  //std::vector<model::DesignDaySystem> refrigerationSystems = m_model.getModelObjects<model::DesignDaySystem>();

  //boost::optional<model::DesignDayCase> refrigerationCase = modelObject.optionalCast<model::DesignDayCase>();
  //OS_ASSERT(refrigerationCase);

  //boost::optional<model::DesignDaySystem> refrigerationSystem = refrigerationCase->system();
  //if(!refrigerationSystem){
  //  return color;
  //}

  //std::vector<model::DesignDaySystem>::iterator it;
  //it = std::find(refrigerationSystems.begin(), refrigerationSystems.end(), refrigerationSystem.get());
  //if(it != refrigerationSystems.end()){
  //  int index = std::distance(refrigerationSystems.begin(), it);
  //  if(index >= static_cast<int>(m_colors.size())){
  //    index = m_colors.size() - 1; // similar to scheduleView's approach
  //  }
  //  color = this->m_colors.at(index).name();
  //}

  return color;
}

void DesignDayGridController::checkSelectedFields()
{
  if(!this->m_hasHorizontalHeader) return;

  OSGridController::checkSelectedFields();
}

void DesignDayGridController::onItemDropped(const OSItemId& itemId)
{
  boost::optional<model::ModelObject> modelObject = OSAppBase::instance()->currentDocument()->getModelObject(itemId);
  if (modelObject){
    if (modelObject->optionalCast<model::DesignDay>()){
      modelObject->clone(m_model);
      emit modelReset();
    }
  }
}

void DesignDayGridController::refreshModelObjects()
{
  auto desighDays = m_model.getModelObjects<model::DesignDay>();
  m_modelObjects = subsetCastVector<model::ModelObject>(desighDays);
  std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
}

void DesignDayGridController::onComboBoxIndexChanged(int index)
{
  // Note: find the correct system color on RACK change,
  // but currently unable to know which row changed.
  for(unsigned index = 0; index < m_horizontalHeader.size(); index++){
  HorizontalHeaderWidget * horizontalHeaderWidget = qobject_cast<HorizontalHeaderWidget *>(m_horizontalHeader.at(index));
    if(horizontalHeaderWidget->m_label->text() == "RACK"){
      // NOTE required due to a race condition
      // Code below commented out due to a very infrequent crash in the bowels of Qt appears to be exasperated by this refresh.
      // A new refresh scheme with finer granularity may eliminate the problem, and restore rack colors.
      //QTimer::singleShot(0, this, SLOT(reset())); TODO
      break;
    }
  }
}


} // openstudio

