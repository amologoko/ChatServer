#ifndef __PERSISTENT_STORAGE_HPP__
#define __PERSISTENT_STORAGE_HPP__

#include <mutex>
#include <vector>
#include <string>
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class PersistentStore {
public:
    PersistentStore(const std::string& file);
    void init();
    const std::string put(const std::string& key, const std::string& value);
    const std::string get(const std::string& key);
    const std::vector<std::string> list();
    void save();

private:
    std::mutex _lock;
    std::string _fname;
    json _json;
};

typedef std::shared_ptr<PersistentStore> PersistentStorePtr;

#endif