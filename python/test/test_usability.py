import openstudio


def test_optional_not_initialized():
    d = openstudio.OptionalDouble()
    assert not d.is_initialized()
    assert not bool(d)


def test_optionalinitialized():
    d = openstudio.OptionalDouble(10.0)
    assert d.is_initialized()
    assert bool(d)
