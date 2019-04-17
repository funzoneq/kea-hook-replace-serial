#include <hooks/hooks.h>
#include <dhcp/pkt4.h>
#include <dhcp/option_string.h>
#include <log/logger.h>
#include <log/macros.h>

#include <string>
#include <vector>

using namespace isc::dhcp;
using namespace isc::hooks;
using namespace std;

extern "C" {
    std::string toText(const std::vector<uint8_t>& binary) {
        return std::string(binary.begin(), binary.end());
    }

    std::string extract_string_option(int optionNr, const Pkt4Ptr pkt4)
    {
        OptionPtr option = pkt4->getOption(optionNr);
        if (option) {
            return option->toString();
        }
        return "false";
    }

    std::string extract_hex_sub_option(int optionNr, int subOptionNr, const Pkt4Ptr pkt4)
    {
        OptionPtr option = pkt4->getOption(optionNr);
        if (option) {
            OptionPtr SubPtr = option->getOption(subOptionNr);
            if (SubPtr) {
                return toText(SubPtr->toBinary(false));
            }
        }
        return "false";
    }

    void replace_option(int optionNr, std::string optionValue, const Pkt4Ptr pkt4) {
        OptionPtr option = pkt4->getOption(optionNr);
        if (option) {
            pkt4->delOption(optionNr);
            option.reset(new OptionString(Option::V4, optionNr, optionValue));
            pkt4->addOption(option);
        }
    }

    int pkt4_send(CalloutHandle& handle) {
        Pkt4Ptr response4_ptr;
        handle.getArgument("response4", response4_ptr);

        string bootfile = extract_string_option(67, response4_ptr);
        string search = "@SERIALNUMBER@";
        size_t found = bootfile.find(search);
        if (found != std::string::npos) {
            /* Lookup incoming packet and extract serialnr */
            Pkt4Ptr query4_ptr;
            handle.getArgument("query4", query4_ptr);
            string serialnumber = extract_hex_sub_option(43, 4, query4_ptr);

            /* Replace @SERIALNUMBER@ with $serialnumber */
            bootfile.replace(found, search.size(), serialnumber);

            replace_option(67, bootfile, response4_ptr);
        }

        return(0);
    }
}
