#have the xcel protocol set the reporting tolerance for deltaF (DOE2 has 0.55C tolerance - Brent suggests 0.55C for E+ too)
#check the setpoints in the protocol

#could we do a custom report to show the thermostat schedules? during occupied and unoccupied times

#checks the number of unmet hours in the model
def unmet_hrs_check(model,sql)
  unmet_hrs_check = Check.new
  unmet_hrs_check.name = "Unmet Hours Check"
  unmet_hrs_check.category = "General"
  unmet_hrs_check.description = "Check that the heating and cooling systems are meeting their setpoints for the entire simulation period."   
  #setup the queries
  heating_setpoint_unmet_query = "SELECT Value FROM TabularDataWithStrings WHERE (ReportName='SystemSummary') AND (ReportForString='Entire Facility') AND (TableName='Time Setpoint Not Met') AND (RowName = 'Facility') AND (ColumnName='During Heating')"
  cooling_setpoint_unmet_query = "SELECT Value FROM TabularDataWithStrings WHERE (ReportName='SystemSummary') AND (ReportForString='Entire Facility') AND (TableName='Time Setpoint Not Met') AND (RowName = 'Facility') AND (ColumnName='During Cooling')"
  #get the info
  heating_setpoint_unmet = sql.execAndReturnFirstDouble(heating_setpoint_unmet_query)
  cooling_setpoint_unmet = sql.execAndReturnFirstDouble(cooling_setpoint_unmet_query)
  
  #make sure all the data are availalbe
  if heating_setpoint_unmet.empty? or cooling_setpoint_unmet.empty?
    unmet_hrs_check.add_flag(Flag.new($eda,"Hours heating or cooling unmet data unavailable; check not run"))
    return unmet_hrs_check
  end
  
  #aggregate heating and cooling hrs
  heating_or_cooling_setpoint_unmet = heating_setpoint_unmet.get + cooling_setpoint_unmet.get    
  #flag if heating + cooling unmet hours > 300
  if heating_or_cooling_setpoint_unmet > 300
    unmet_hrs_check.add_flag(Flag.new($eda,"Hours heating or cooling unmet is #{heating_or_cooling_setpoint_unmet}; > the Xcel EDA limit of 300 hrs"))
  end
  debug_puts unmet_hrs_check.inspect
  
  return unmet_hrs_check
end    
