#ifndef PORTPARAMETERS_H
#define PORTPARAMETERS_H

#include "portParametersGeneric.h"

class PortParameterMicroinverterSerialNumber : public PortParameterInt {
      private:
	void getValueFromRegisters(uint16_t *registers, int addressOffset);

      public:
	PortParameterMicroinverterSerialNumber();
};

class PortParameterPortNumber : public PortParameterInt {
      public:
	PortParameterPortNumber();
};

class PortParameterPvVoltage : public PortParameterFloat {
      public:
	PortParameterPvVoltage();
};

class PortParameterPvCurrentMi : public PortParameterFloat {
      public:
	PortParameterPvCurrentMi();
};

class PortParameterPvCurrentHm : public PortParameterFloat {
      public:
	PortParameterPvCurrentHm();
};

class PortParameterGridVoltage : public PortParameterFloat {
      public:
	PortParameterGridVoltage();
};

class PortParameterGridFrequency : public PortParameterFloat {
      public:
	PortParameterGridFrequency();
};

class PortParameterPvPower : public PortParameterFloat {
      public:
	PortParameterPvPower();
};

class PortParameterTodayProduction : public PortParameterInt {
      public:
	PortParameterTodayProduction();
};

class PortParameterTotalProduction : public PortParameterInt {
      public:
	PortParameterTotalProduction();
};

class PortParameterTemperature : public PortParameterFloat {
      public:
	PortParameterTemperature();
};

class PortParameterOperatingStatus : public PortParameterInt {
      public:
	PortParameterOperatingStatus();
};

class PortParameterAlarmCode : public PortParameterInt {
      public:
	PortParameterAlarmCode();
};

class PortParameterAlarmCount : public PortParameterInt {
      public:
	PortParameterAlarmCount();
};

class PortParameterLinkStatus : public PortParameterInt {
      public:
	PortParameterLinkStatus();
};

class PortParameterOnOff : public PortParameterInt {
      public:
      PortParameterOnOff();
};

class PortParameterLimitActivePower : public PortParameterInt {
      public:
      PortParameterLimitActivePower();
};

#endif