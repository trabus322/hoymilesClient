#ifndef SUNSPEC_PARAMETERS_GENERIC_H
#define SUNSPEC_PARAMETERS_GENERIC_H

#include <stdint.h>
#include <string>

class SunspecParameter {
      protected:
    std::string name;
    int registerAddressOffset;
    int registerSize;
    
      public:
    SunspecParameter(std::string name, int registerAddressOffset, int registerSize);

    virtual ~SunspecParameter();

	enum SunspecValueType { uint32, uint16, string32, string16, sunssf, int16, acc32, float32, enum16 };

	union SunspecValue {
		uint32_t u32;
		uint16_t u16;
		uint16_t ssf;
		int i16;
		uint16_t a32;
		float f;
		uint16_t e16;
	};

      protected:
    SunspecValueType valueType;
    SunspecValue value;

     public:

    virtual void getValueFromRegisters(uint16_t *registers, int addressOffset);

    std::pair<SunspecValue, SunspecValueType> getValue();
};

// class SunspecParameterUint32 : public SunspecParameter {
//       protected:
    
//       public:
//     SunspecParameterUint32(std::string name, int registerAddressOffset, int registerSize);

//     virtual void getValueFromRegisters(uint16_t *registers, int addressOffset);
// };

// class SunspecParameterUint16 : public SunspecParameter {
//       protected:

//       public:
//     SunspecParameterUint16(std::string name, int registerAddressOffset, int registerSize);

//     virtual void getValueFromRegisters(uint16_t *registers, int addressOffset);
// };

class SunspecParameterString32 : public SunspecParameter {
      protected:
    std::string value;

      public:
    SunspecParameterString32(std::string name, int registerAddressOffset, int registerSize);

    virtual void getValueFromRegisters(uint16_t *registers, int addressOffset);
};

// class SunspecParameterString16 : public SunspecParameter {
//       protected:
//     std::string value;

//       public:
//     SunspecParameterString16(std::string name, int registerAddressOffset, int registerSize);

//     virtual void getValueFromRegisters(uint16_t *registers, int addressOffset);
// };

// class SunspecParameterSunssf : public SunspecParameter {
//       protected:

//       public:
//     SunspecParameterSunssf(std::string name, int registerAddressOffset, int registerSize);

//     virtual void getValueFromRegisters(uint16_t *registers, int addressOffset);
// };

// class SunspecParameterInt16 : public SunspecParameter {
//       protected:

//       public:
//     SunspecParameterInt16(std::string name, int registerAddressOffset, int registerSize);

//     virtual void getValueFromRegisters(uint16_t *registers, int addressOffset);
// };

// class SunspecParameterAcc32 : public SunspecParameter {
//       protected:

//       public:
//     SunspecParameterAcc32(std::string name, int registerAddressOffset, int registerSize);

//     virtual void getValueFromRegisters(uint16_t *registers, int addressOffset);
// };

// class SunspecParameterFloat32 : public SunspecParameter {
//       protected:

//       public:
//     SunspecParameterFloat32(std::string name, int registerAddressOffset, int registerSize);

//     virtual void getValueFromRegisters(uint16_t *registers, int addressOffset);
// };

// class SunspecParameterEnum16 : public SunspecParameter {
//       protected:

//       public:
//     SunspecParameterEnum16(std::string name, int registerAddressOffset, int registerSize);

//     virtual void getValueFromRegisters(uint16_t *registers, int addressOffset);
// };

#endif