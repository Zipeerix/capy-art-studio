#include <filesystem>
#include "ConfigurationManager.h"

namespace cas {
ConfigurationManager::ConfigurationManager() {

}

std::shared_ptr<ConfigurationManager> ConfigurationManager::get_instance() {
    static std::shared_ptr<ConfigurationManager> singleton_entity(new ConfigurationManager());
    return singleton_entity;
}
} // sv