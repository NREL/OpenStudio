#add electric heat rejection to electric cooling

#add fans and lights? compare fans and cooling - greater than 5x different during peak cooling month - helps identify bad chiller curves

#energy use for cooling and heating as percentage of total energy
def enduse_pcts_check(model,sql)
  enduse_pcts_check = Check.new 
  enduse_pcts_check.name = "Energy Enduses Check"
  enduse_pcts_check.category = "Xcel EDA"
  enduse_pcts_check.description = "Check that heating and cooling energy make up the expected percentage of total energy consumption." 
  
  #make sure all required data are available
  if sql.electricityCooling.empty? or sql.naturalGasCooling.empty? or sql.otherFuelCooling.empty? or sql.totalSiteEnergy.empty? or sql.electricityHeating.empty? or sql.naturalGasHeating.empty? or sql.otherFuelHeating.empty?
    enduse_pcts_check.add_flag(Flag.new($eda,"Enduse energy data unavailable; check not run"))
    return enduse_pcts_check
  end
  
  pct_cooling = (sql.electricityCooling.get + sql.naturalGasCooling.get + sql.otherFuelCooling.get) / sql.totalSiteEnergy.get   
  pct_heating = (sql.electricityHeating.get + sql.naturalGasHeating.get + sql.otherFuelHeating.get) / sql.totalSiteEnergy.get
  #flag if 0% < pct_cooling < 20%
  if pct_cooling < 0.0 or pct_cooling > 0.2
     enduse_pcts_check.add_flag(Flag.new($eda,"Cooling energy = #{pct_cooling} of total energy use;  outside of 0%-20% range expected by Xcel EDA"))
  end
  #flag if 30% < pct_heating < 50%
  if pct_heating < 0.30 or pct_heating > 0.50
    enduse_pcts_check.add_flag(Flag.new($eda,"Heating energy = #{pct_heating} of total energy use; outside the 30%-50% range expected by Xcel EDA"))
  end
  debug_puts enduse_pcts_check.inspect
  
  return enduse_pcts_check
end
