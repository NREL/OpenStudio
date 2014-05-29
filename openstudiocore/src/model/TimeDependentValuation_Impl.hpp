/*********************************************************************
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

#ifndef MODEL_TIMEDEPENDENTVALUATION_IMPL_HPP
#define MODEL_TIMEDEPENDENTVALUATION_IMPL_HPP

#include "ModelObject_Impl.hpp"

#include "../utilities/filetypes/TimeDependentValuationFile.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

class Site;
class TimeDependentValuation;

namespace detail {

  class MODEL_API TimeDependentValuation_Impl : public ModelObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(boost::optional<double> energyTimeDependentValuation READ energyTimeDependentValuation);
    Q_PROPERTY(boost::optional<double> costTimeDependentValuation READ costTimeDependentValuation);
   public:
    /** @name Constructors and Destructors */
    //@{

    // constructor
    TimeDependentValuation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    TimeDependentValuation_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                                Model_Impl* model, 
                                bool keepHandle);

    // clone copy constructor
    TimeDependentValuation_Impl(const TimeDependentValuation_Impl& other, 
                                Model_Impl* model, 
                                bool keepHandle);

    // virtual destructor
    virtual ~TimeDependentValuation_Impl(){}

    //@}
    /** @name Getters */
    //@{

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    /** Load from disk or return cached TDV file. Caches on successful load. */
    boost::optional<TimeDependentValuationFile> file(const openstudio::path& dir=openstudio::path()) const;

    /** Convert file() to SI units, or return cached converted file. Caches on successful
     *  conversion. */
    boost::optional<TimeDependentValuationFile> fileInSIUnits() const;

    /** Get the Site, if it exists. */
    boost::optional<Site> site() const;

    /** Get the url string, if it exists. */
    boost::optional<std::string> url() const;

    /** Get the path, if it exists. */
    boost::optional<openstudio::path> path() const;

    BuildingSector activeBuildingSector() const;

    /** Return the \link FuelType FuelTypes \endlink for which time dependent valuations for 
     *  activeBuildingSector() are available. */
    std::vector<FuelType> availableFuelTypes() const;

    //@}
    /** @name Setters */
    //@{

    // set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent);

    /** Save the url as relative to basePath, or just keep the file name and extension if 
     *  basePath.empty(). */
    bool makeUrlRelative(const openstudio::path& basePath=openstudio::path());

    /** Search for file and save the absolute url if successful. */
    bool makeUrlAbsolute(const openstudio::path& searchDirectory);

    void setActiveBuildingSector(const BuildingSector& sector);

    void clearCache();

    //@}
    /** @name Queries */
    //@{

    /** Get the total energy time dependent valuation (J). Requires model().sqlFile() and file(). */
    boost::optional<double> energyTimeDependentValuation(
        const std::string& environmentPeriod=std::string()) const;

    /** Get the total cost time dependent valuation (J). Requires model().sqlFile() and file(). */
    boost::optional<double> costTimeDependentValuation(
        const std::string& environmentPeriod=std::string()) const;

    /** Get the energy time dependent valuation for fuelType (J). Requires model().sqlFile() and 
     *  file(). */
    boost::optional<double> getEnergyTimeDependentValuation(
        const FuelType& fuelType,const std::string& environmentPeriod=std::string()) const;

    /** Get the cost time dependent valuation for fuelType ($).  Requires model().sqlFile() and 
     *  file(). */
    boost::optional<double> getCostTimeDependentValuation(
        const FuelType& fuelType,const std::string& environmentPeriod=std::string()) const;

    //@}
   protected:
    friend class openstudio::model::TimeDependentValuation;

   private:
    // lazy cache
    mutable boost::optional<TimeDependentValuationFile> m_file;
    mutable boost::optional<TimeDependentValuationFile> m_fileInSIUnits;

    REGISTER_LOGGER("openstudio.model.TimeDependentValuation");

    std::vector<double> mf_tdvValues(const TimeDependentValuationFile& tdvFile,
                                     const FuelType& fuelType) const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_TIMEDEPENDENTVALUATION_IMPL_HPP
