## General Idea

When calling a method that is trying to set field to point to a `Schedule`, OpenStudio will try to enforce correspondance of `ScheduleTypeLimits` to the Schedule or assign one if need be. This is meant to avoid user errors, and is/was especially useful for the OpenStudioApplication so that switching to IP units would convert the Schedule values (the display of IP units schedule is based on the `ScheduleTypeLimits`)

All children of `ModelObject` that can point to Schedules should override the method `ModelObject_Impl::getScheduleTypeKeys(const Schedule&)` method, cf [ModelObject_Impl.hpp#L173-L177](https://github.com/NREL/OpenStudio/blob/8ae4d883ed1a6ba704aa8b60003646de69a00f39/src/model/ModelObject_Impl.hpp#L173-L177)

```c++
    /** Return the ScheduleTypeKeys indicating how schedule is used in this object. If schedule is not directly
     *  used by this object, return value will be .empty().
     *
     *  REIMPLEMENT IN ALL CONCRETE MODELOBJECTS THAT CAN POINT TO SCHEDULES. */
    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;
```

This method is used to specify what entry in the ScheduleTypeRegistry is meant to be picked up **for the specific field that's trying to be set**.

## ConcreteModelObject's setter and `ModelObject_Impl::setSchedule`

An example of such usage can be found for example in [ZoneVentilationWindandStackOpenArea.cpp#L101-L131](https://github.com/NREL/OpenStudio/blob/8ae4d883ed1a6ba704aa8b60003646de69a00f39/src/model/ZoneVentilationWindandStackOpenArea.cpp#L101-L131), but the general form is something like this:

```c++
  std::vector<ScheduleTypeKey> ZoneVentilationWindandStackOpenArea_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneVentilation_WindandStackOpenAreaFields::OpeningAreaFractionScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneVentilationWindandStackOpenArea","Opening Area Fraction Schedule"));
    }
    // if another field ....
    return result;
```

When calling the setter for a Schedule field, it will call `ModelObject_Impl::setSchedule`. Cf for example [ZoneVentilationWindandStackOpenArea::setOperiningAreaFractionSchedule](https://github.com/NREL/OpenStudio/blob/develop/src/model/ZoneVentilationWindandStackOpenArea.cpp#L154-L160)

```c++
  bool ZoneVentilationWindandStackOpenArea_Impl::setOpeningAreaFractionSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_ZoneVentilation_WindandStackOpenAreaFields::OpeningAreaFractionScheduleName,
                                                "ZoneVentilationWindandStackOpenArea",
                                                "Opening Area Fraction Schedule",
                                                schedule);
    return result;
  }
```

Here is the detail of the [ModelObject_Impl::setSchedule](https://github.com/NREL/OpenStudio/blob/8ae4d883ed1a6ba704aa8b60003646de69a00f39/src/model/ModelObject.cpp#L688-L705)

```c++
  bool ModelObject_Impl::setSchedule(unsigned index,
                                     const std::string& className,
                                     const std::string& scheduleDisplayName,
                                     Schedule& schedule)
  {
    if (schedule.model() != model()) {
      return false;
    }
    bool result = checkOrAssignScheduleTypeLimits(className,scheduleDisplayName,schedule);
    if (!result) {
      if (boost::optional<ScheduleTypeLimits> scheduleTypeLimits = schedule.scheduleTypeLimits()) {
        LOG(Warn, "For " << briefDescription() << " cannot set Schedule " << scheduleDisplayName << "=" << schedule.nameString()
               << " because it has an incompatible ScheduleTypeLimits");
      }
      return result;
    }
    return setPointer(index,schedule.handle());
  }
```

## `ScheduleTypeRegistry` and the `ScheduleTypeLimits`

Now we finally call into the `ScheduleTypeRegistry` via the free function [checkOrAssignScheduleTypeLimits](https://github.com/NREL/OpenStudio/blob/8ae4d883ed1a6ba704aa8b60003646de69a00f39/src/model/ScheduleTypeRegistry.cpp#L534-L560)

```c++
bool checkOrAssignScheduleTypeLimits(const std::string& className,
                                     const std::string& scheduleDisplayName,
                                     Schedule& schedule)
{
  ScheduleType scheduleType = ScheduleTypeRegistry::instance().getScheduleType(
      className,
      scheduleDisplayName);
  bool result(true);
  if (OptionalScheduleTypeLimits scheduleTypeLimits = schedule.scheduleTypeLimits()) {
    // isStringent = false, we do not enforce NOT having lower / upper limits if our object accepts any.
    // This is user-specified, so user is free to do this
    if (!isCompatible(scheduleType, *scheduleTypeLimits, false)) {
      result = false;
    }
  }
  else {
    Model model = schedule.model();
    ScheduleTypeLimits candidate = ScheduleTypeRegistry::instance().getOrCreateScheduleTypeLimits(
        scheduleType,
        model);
    result = schedule.setScheduleTypeLimits(candidate);
    if (!result && (candidate.directUseCount(false) == 0)) {
      candidate.remove();
    }
  }
  return result;
}
```

This should be somewhat self-explanatory since the functions are well-named, but there are a couple of things that must be noted at first:

1. If there is a `ScheduleTypeLimits` assigned to the Schedule that's being used, call `isCompatible` to check if the `ScheduleTypeLimits` in question match what is expected
2. If not, try to either find a compatible `ScheduleTypeLimits` or create a new one to set it to the `Schedule`
    * **That part can be quite confusing the first time you realize it happens behind the scenes when you call a setter and your `Schedule` now has a `ScheduleTypeLimits`. This is also why you can't have a setter take in a `const Schedule&`**: `bool setOpeningAreaFractionSchedule(Schedule&);` works, but `bool setOpeningAreaFractionSchedule(const Schedule&);` doesn't, since the `Schedule` passed can be modified.

Both of these flow branches will call into `getScheduleType`, which uses the entries you have manually set in the ScheduleTypeRegistrySingleton around [here](https://github.com/NREL/OpenStudio/blob/develop/src/model/ScheduleTypeRegistry.cpp#L118-L125):

```c++
ScheduleTypeRegistrySingleton::ScheduleTypeRegistrySingleton()
{
  // className, scheduleDisplayName, scheduleRelationshipName, isContinuous, unitType, lowerLimitValue, upperLimitValue;


  const ScheduleType scheduleTypes[] =
  {
    {"AirConditionerVariableRefrigerantFlow","Availability Schedule","availabilitySchedule",false,"Availability",0.0,1.0},
    {"AirConditionerVariableRefrigerantFlow","Thermostat Priority Schedule","thermostatPrioritySchedule",false,"ControlMode",0.0,1.0},
    [...]
    {"ZoneVentilationWindandStackOpenArea","Opening Area Fraction Schedule","openingAreaFractionSchedule",true,"",0.0,1.0},
    [...]
```

### When you already have a `ScheduleTypeLimits` assigned

For 1) above , the definition of [isCompatible](https://github.com/NREL/OpenStudio/blob/8ae4d883ed1a6ba704aa8b60003646de69a00f39/src/model/ScheduleTypeRegistry.cpp#L483-L532) will show that basically it'll check the Unit Type and the lower/upper bounds, generally speaking (when `isStringent` is false)
* Check that units are the same between expected and candidate
* If you expected a lower limit, then check that candidate has a lower limit and that it is higher the expected (more restrictive than the expected)
* If you expected a higher limit, then check that the candidate has an upper limit and that it is lower than the expected (more restrictive than the expected)

### When you don't already have a `ScheduleTypeLimits` assigned

For 2) when you are using a `Schedule` that doesn't already have a `ScheduleTypeLimits`, it'll loop on all `ScheduleTypeLimits` in the model, and call `isCompatible` on them and return the first one that matches the requirements. 

**If not it'll create a new one with the requirements**. cf [ScheduleTypeRegistrySingleton::getOrCreateScheduleTypeLimits](https://github.com/NREL/OpenStudio/blob/8ae4d883ed1a6ba704aa8b60003646de69a00f39/src/model/ScheduleTypeRegistry.cpp#L77-L116)
