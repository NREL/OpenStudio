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

#ifndef ISOMODEL_POPULATION_HPP
#define ISOMODEL_POPULATION_HPP

namespace openstudio {
namespace isomodel {
class Population
  {
  public:
    double hoursEnd() const {return _hoursEnd;}
    double hoursStart() const {return _hoursStart;}
    double daysEnd() const {return _daysEnd;}
    double daysStart() const {return _daysStart;}
    double densityOccupied() const {return _densityOccupied;}
    double densityUnoccupied() const {return _densityUnoccupied;}
    double heatGainPerPerson() const {return _heatGainPerPerson;}

    void setHoursEnd(double value) {_hoursEnd = value;}
    void setHoursStart(double value) {_hoursStart = value;}
    void setDaysEnd(double value) {_daysEnd = value;}
    void setDaysStart(double value) {_daysStart = value;}
    void setDensityOccupied(double value) {_densityOccupied = value;}
    void setDensityUnoccupied(double value) {_densityUnoccupied = value;}
    void setHeatGainPerPerson(double value) {_heatGainPerPerson = value;}

  private:
    double _hoursEnd;
    double _hoursStart;
    double _daysEnd;
    double _daysStart;
    double _densityOccupied;
    double _densityUnoccupied;
    double _heatGainPerPerson;

  };

} // isomodel
} // openstudio
#endif // ISOMODEL_POPULATION_HPP
