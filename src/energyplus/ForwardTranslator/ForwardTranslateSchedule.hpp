#ifndef ENERGYPLUS_FORWARDTRANSLATOR_FORWARDTRANSLATESCHEDULE_HPP
#define ENERGYPLUS_FORWARDTRANSLATOR_FORWARDTRANSLATESCHEDULE_HPP

#include <fmt/format.h>
#include <fmt/printf.h>

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/time/Date.hpp"

namespace openstudio {

namespace energyplus {

static unsigned startNewDay(IdfObject &idfObject,unsigned fieldIndex,Date date)
{
  std::string string = fmt::sprintf("Through: %02d/%02d",date.monthOfYear().value(),date.dayOfMonth());
  idfObject.setString(fieldIndex, string);
  ++fieldIndex;
  idfObject.setString(fieldIndex, "For: AllDays");
  ++fieldIndex;
  return fieldIndex;
}

static unsigned addUntil(IdfObject &idfObject,unsigned fieldIndex,int hours,int minutes,double value)
{
  std::string string = fmt::sprintf("Until: %02d:%02d",hours,minutes);
  idfObject.setString(fieldIndex, string);
  ++fieldIndex;
  idfObject.setDouble(fieldIndex, value);
  ++fieldIndex;
  return fieldIndex;
}

}

}


#endif

