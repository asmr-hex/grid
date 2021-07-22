#include <iostream>
#include "conf/schema.hpp"

// FOLLOW THIS GUIDE
// https://www.codesynthesis.com/projects/xsd/documentation/cxx/tree/guide/

int main (int argc, char* argv[]) {
    try {
        std::unique_ptr<interface_t> i = Interface("../conf-xml/test.xml");

        printf("INTERFACE: %s\n", i->name().c_str());

        // print all midi outputs
        for (auto itr : i->Outputs().Midi()) {
            printf("OUTPUT: %s\n", itr.name().c_str());
        }

        // print all voltage outputs
        for (auto itr : i->Outputs().Voltage()) {
            printf("OUTPUT: %s\n", itr.name().c_str());
        }

        // print all voltage outputs
        for (auto itr : i->Instruments().Instrument()) {
            printf("Instrument: %s\n", itr.name().c_str());
            for (auto param : itr.Parameter()) {
                printf("Param: %s\n", param.name().c_str());
            }
        }
    }
    catch (const xml_schema::exception& e) {
        std::cerr << e << std::endl;
        return 1;
    }

}
