import logging
import openstudio
import sys

# Root logger
logger = logging.getLogger()
logger.setLevel(logging.WARNING)

formatter = logging.Formatter('LOGGER - %(message)s')
handler = logging.StreamHandler(sys.stdout)
handler.setLevel(logging.WARNING)
handler.setFormatter(formatter)
logger.addHandler(handler)

openstudio.Logger_instance().standardOutLogger().setLogLevel(openstudio.Error)

logger.info("STDOUT Info")
logger.warning("STDOUT Warn")
logger.error("STDOUT Error")

openstudio.logFree(openstudio.Info, "test", "Info")
openstudio.logFree(openstudio.Warn, "test", "Warn")
openstudio.logFree(openstudio.Error, "test", "Error")
