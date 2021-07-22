#include <iostream>
#include "conf/schema.hpp"

// FOLLOW THIS GUIDE
// https://www.codesynthesis.com/projects/xsd/documentation/cxx/tree/guide/

int main (int argc, char* argv[]) {
    try {
        std::unique_ptr<Interface> i = Interface_("../conf-xml/test.xml");

        std::printf("INTERFACE: %s\n", i->name());

        // for (auto output : i->Outputs) {
        //     std::printf("OUTPUT: %s", output->name);
        // }
    }
    catch (const xml_schema::exception& e) {
        std::cerr << e << std::endl;
        return 1;
    }

}
