import openstudio

assert openstudio.openStudioVersion() == "3.6.1"

FIELD_IDENTIFIERS = [
    "CoolingSAFMethod",
    "CoolingSAFMethod_SAFlow",
    "CoolingSAFMethod_SAFlowPerFloorArea",
    "CoolingSAFMethod_FracOfAutosizedCoolingSAFlow",
    "CoolingSAFMethod_FlowPerCoolingCapacity",
    "HeatingSAFMethod",
    "HeatingSAFMethod_SAFlow",
    "HeatingSAFMethod_SAFlowPerFloorArea",
    "HeatingSAFMethod_FracOfAutosizedHeatingSAFlow",
    "HeatingSAFMethod_FlowPerHeatingCapacity",
    "NoCoolHeatSAFMethod",
    "NoCoolHeatSAFMethod_SAFlow",
    "NoCoolHeatSAFMethod_SAFlowPerFloorArea",
    "NoCoolHeatSAFMethod_FracOfAutosizedCoolingSAFlow",
    "NoCoolHeatSAFMethod_FracOfAutosizedHeatingSAFlow",
    "NoCoolHeatSAFMethod_FlowPerCoolingCapacity",
    "NoCoolHeatSAFMethod_FlowPerHeatingCapacity",
]

COOLINGSAFMETHODINDEX = 18


def get_info(unitary):
    return {
        field_identifier: unitary.getString(i + COOLINGSAFMETHODINDEX, False, False).get()
        for i, field_identifier in enumerate(FIELD_IDENTIFIERS)
    }


def add_cooling_coil(unitary: openstudio.model.AirLoopHVACUnitarySystem, model: openstudio.model.Model):
    cc = openstudio.model.CoilCoolingDXSingleSpeed(model)
    assert unitary.setCoolingCoil(cc)


def add_heating_coil(unitary: openstudio.model.AirLoopHVACUnitarySystem, model: openstudio.model.Model):
    hc = openstudio.model.CoilHeatingDXSingleSpeed(model)
    assert unitary.setHeatingCoil(hc)


def apply_coils_or_not(m: openstudio.model.Model, prefix: str):
    coil_methods = [None, add_cooling_coil, add_heating_coil]

    unitarys = m.getAirLoopHVACUnitarySystems()
    assert len(unitarys) == 3

    test_infos = []

    for coil_method, u in zip(coil_methods, unitarys):
        coil_method_name = "None"
        has_cc = False
        has_hc = False
        if coil_method is not None:
            coil_method(unitary=u, model=m)
            coil_method_name = coil_method.__name__
            if coil_method_name == "add_cooling_coil":
                has_cc = True
            else:
                has_hc = True

        test_case = f"{prefix} - {coil_method_name}"
        u.setName(test_case)
        print(test_case)
        coolingSAFMethod = ""
        heatingSAFMethod = ""
        noCoolHeatSAFMethod = ""
        if (coolingSAFMethod_ := u.supplyAirFlowRateMethodDuringCoolingOperation()).is_initialized():
            coolingSAFMethod = coolingSAFMethod_.get()

        if (heatingSAFMethod_ := u.supplyAirFlowRateMethodDuringHeatingOperation()).is_initialized():
            heatingSAFMethod = heatingSAFMethod_.get()

        if (noCoolHeatSAFMethod_ := u.supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired()).is_initialized():
            noCoolHeatSAFMethod = noCoolHeatSAFMethod_.get()

        test_infos.append(
            dict(
                {
                    "test_case": test_case,
                    "CC": has_cc,
                    "HC": has_hc,
                    "coolingSAFMethod": coolingSAFMethod,
                    "heatingSAFMethod": heatingSAFMethod,
                    "noCoolHeatSAFMethod": noCoolHeatSAFMethod,
                },
                **get_info(unitary=u),
            )
        )

    return test_infos


def model_default():
    """In this test, this is the Default constructed Unitary System, just adding coils (or not).

    The Supply Air Flow During XXX field is set to Autosize, yet the Method field isn't "SupplyAirFlowRate" but blank.
    """

    m = openstudio.model.Model()
    for i in range(3):
        openstudio.model.AirLoopHVACUnitarySystem(m)
    test_infos = apply_coils_or_not(m, prefix="Default")
    return m, test_infos


def test_SAF_not_null_but_correct_combos():
    """In this test, the SAF method is filled out, and ALL flow fields are set.

    We respect the SAF method choice, we keep the corresponding flow field, and clear out all the others
    """
    m = openstudio.model.Model()
    for i in range(3):
        u = openstudio.model.AirLoopHVACUnitarySystem(m)

        # Supply Air Flow Rate Method During Cooling Operation
        assert u.setSupplyAirFlowRateMethodDuringCoolingOperation("FlowPerFloorArea")
        assert u.setSupplyAirFlowRateDuringCoolingOperation(1.0)
        assert u.setSupplyAirFlowRatePerFloorAreaDuringCoolingOperation(1.1)
        assert u.setFractionofAutosizedDesignCoolingSupplyAirFlowRate(1.2)
        assert u.setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation(1.3)

        # Supply Air Flow Rate Method During Heating Operation
        assert u.setSupplyAirFlowRateMethodDuringHeatingOperation("FlowPerHeatingCapacity")
        assert u.setSupplyAirFlowRateDuringHeatingOperation(2.0)
        assert u.setSupplyAirFlowRatePerFloorAreaduringHeatingOperation(2.1)
        assert u.setFractionofAutosizedDesignHeatingSupplyAirFlowRate(2.2)
        assert u.setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation(2.3)

        # Supply Air Flow Rate Method When No Cooling or Heating is Required
        assert u.setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired("FractionOfAutosizedHeatingValue")
        assert u.setSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(0.1)
        assert u.setSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired(0.2)
        assert u.setFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(0.3)
        assert u.setFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(0.4)
        assert u.setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired(0.5)
        assert u.setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired(0.6)

    test_infos = apply_coils_or_not(m, prefix="SAF_correct")
    return m, test_infos


def test_SAF_not_null_and_incorrect_combos():
    """In this test, the SAF method is filled out, but the corresponding flow field isn't.

    We respect the SAF method choice, and we set the corresponding flow field field to zero
    """
    m = openstudio.model.Model()
    for i in range(3):
        u = openstudio.model.AirLoopHVACUnitarySystem(m)
        assert u.setSupplyAirFlowRateMethodDuringCoolingOperation("FractionOfAutosizedCoolingValue")
        assert u.setSupplyAirFlowRatePerFloorAreaDuringCoolingOperation(1.0)

        assert u.setSupplyAirFlowRateMethodDuringHeatingOperation("FlowPerFloorArea")
        assert u.setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation(3.0)

        assert u.setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired("FlowPerCoolingCapacity")
        assert u.setFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(0.4)
    test_infos = apply_coils_or_not(m, prefix="SAF_incorrect")
    return m, test_infos


if __name__ == "__main__":
    m1, test_infos = model_default()
    m1.save("test_vt_UnitarySystem_SAFMethods_default.osm", True)

    m2, test_infos2 = test_SAF_not_null_but_correct_combos()
    m2.save("test_vt_UnitarySystem_SAFMethods_CorrectCombos.osm", True)
    test_infos += test_infos2

    m3, test_infos3 = test_SAF_not_null_and_incorrect_combos()
    m3.save("test_vt_UnitarySystem_SAFMethods_IncorrectCombos.osm", True)
    test_infos += test_infos3
    print(test_infos)
