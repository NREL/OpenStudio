/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef ANALYSIS_DISCRETEVARIABLE_IMPL_HPP
#define ANALYSIS_DISCRETEVARIABLE_IMPL_HPP

#include <analysis/AnalysisAPI.hpp>
#include <analysis/InputVariable_Impl.hpp>

#include <analysis/Measure.hpp>

namespace openstudio {
namespace analysis {

class DiscreteVariable;

namespace detail {

  /** DiscreteVariable_Impl is an InputVariable_Impl that is the implementation class
   *  for DiscreteVariable.*/
  class ANALYSIS_API DiscreteVariable_Impl : public InputVariable_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Construct from a vector of measures (concrete values of the variable). Performs the
     *  following error checking and clean-up operations:
     *
     *  \li Allows at most one NullMeasure (keeping the first and deleting all others).
     *  \li Ensures that the variable always acts on the same type of model, for instance, idf or osm.
     *
     *  createJob is implemented assuming QVariant value is an integer index into the measure
     *  vector.
     */
    DiscreteVariable_Impl(const std::string& name,
                          const std::vector<Measure>& measures);

    /** Constructor provided for deserialization; not for general use. */
    DiscreteVariable_Impl(const UUID& uuid,
                          const UUID& versionUUID,
                          const std::string& name,
                          const std::string& displayName,
                          const std::string& description,
                          const boost::optional<UncertaintyDescription>& udesc,
                          const std::vector<Measure>& measures);

    DiscreteVariable_Impl(const DiscreteVariable_Impl& other);

    virtual ~DiscreteVariable_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual AnalysisObject clone() const;

    /** Get the variable value from a dataPoint. */
    virtual double getValue(const DataPoint& dataPoint) const;

    virtual boost::optional<FileReferenceType> inputFileType() const;

    virtual boost::optional<FileReferenceType> outputFileType() const;

    /** Returns true if value is valid for this Variable. If returns false, createJob(value) should
     *  throw. */
    virtual bool isValid(const QVariant& value) const;

    /** Returns true if udesc is valid for this Variable. If returns false,
     *  setUncertaintyDescription will fail. */
    virtual bool isValid(const UncertaintyDescription& udesc) const;

    virtual runmanager::WorkItem createWorkItem(const QVariant& value,
                                                const openstudio::path& rubyIncludeDirectory) const;

    //@}
    /** @name Getters and Queries */
    //@{

    std::vector<Measure> measures(bool selectedMeasuresOnly) const;

    Measure getMeasure(int index) const;

    boost::optional<Measure> getMeasureByUUID(const UUID& uuid) const;

    unsigned numMeasures(bool selectedMeasuresOnly) const;

    boost::optional<int> getIndexByUUID(const Measure& measure) const;

    Measure getMeasure(const DataPoint& dataPoint) const;

    //@}
    /** @name Setters */
    //@{

    bool push(const Measure& measure);

    bool insert(int index, const Measure& measure);

    bool erase(const Measure& measure);

    bool swap(const Measure& measure1, const Measure& measure2);

    bool setMeasures(const std::vector<Measure>& measures);

    void clearMeasures();

    //@}
    /** @name Protected in Public Class */
    //@{

    bool fileTypesAreCompatible(const Measure& childMeasure,
                                const FileReferenceType& proposedInputFileType,
                                const FileReferenceType& proposedOutputFileType) const;

    //@}
   protected:
    std::vector<Measure> m_measures;
   private:
    REGISTER_LOGGER("openstudio.analysis.DiscreteVariable");

    bool measuresAreCompatible(const std::vector<Measure>& measures) const;
  };

  /** Returns true and the result if measures are consistent. Otherwise, returns
   *  false and a value to ignore. */
  std::pair<bool,boost::optional<FileReferenceType> > inputFileType(
      const std::vector<Measure>& measures);

  /** Returns true and the result if measures are consistent. Otherwise, returns
   *  false and a value to ignore. */
  std::pair<bool,boost::optional<FileReferenceType> > outputFileType(
      const std::vector<Measure>& measures);

} // detail

} // model
} // openstudio

#endif // ANALYSIS_DISCRETEVARIABLE_IMPL_HPP

