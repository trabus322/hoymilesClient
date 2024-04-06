#ifndef SUNSPEC_PARAMETERS_H
#define SUNSPEC_PARAMETERS_H

#include "sunspecParametersGeneric.h"

class SunspecParameterManufacturer : public SunspecParameterString32 {
      protected:

      public:
    SunspecParameterManufacturer();

    void getValueFromRegisters(uint16_t *registers, int addressOffset);
};

#endif