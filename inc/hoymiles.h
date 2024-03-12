#ifndef HOYMILES_H
#define HOYMILES_H

struct _modbus;
typedef _modbus modbus_t;

class Dtu{
    private:
    modbus_t *modbus_t;

    public:
    Dtu(const char *ip_address, int port);

    void readTest();

    ~Dtu();
};

#endif