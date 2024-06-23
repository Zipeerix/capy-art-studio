#ifndef CAPYARTSTUDIO_CONFIGURATIONMANAGER_H
#define CAPYARTSTUDIO_CONFIGURATIONMANAGER_H

#include <toml++/toml.hpp>
#include <string>
#include <memory>

namespace cas {
    class ConfigurationManager {
    public:
        ConfigurationManager(ConfigurationManager&) = delete;
        void operator=(const ConfigurationManager&) = delete;

        static std::shared_ptr<ConfigurationManager> get_instance();

    protected:
        ConfigurationManager();
    };
} // cas

#endif //CAPYARTSTUDIO_CONFIGURATIONMANAGER_H
