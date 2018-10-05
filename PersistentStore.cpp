#include "PersistentStore.hpp"
#include <fstream>
#include <iomanip>

PersistentStore::PersistentStore(const std::string& file)
{
    _fname = file.empty() ? "storage.json" : file;
    std::ifstream ifs(_fname);
    ifs >> _json;
}

const std::string PersistentStore::put(const std::string& key, const std::string& value)
{
    std::lock_guard<std::mutex> guard(_lock);
    if(_json[key].empty()) {
        _json[key] = value;
    }
    return _json[key];
}

const std::string PersistentStore::get(const std::string& key)
{
    std::string value;
    std::lock_guard<std::mutex> guard(_lock);
    value = _json[key];
    if (value.empty()) {
        _json.erase(key);
    }
    return value;
}

const std::vector<std::string> PersistentStore::list()
{
    std::vector<std::string> ret;
    std::lock_guard<std::mutex> guard(_lock);
    for (json::iterator it = _json.begin(); it != _json.end(); ++it) {
        ret.emplace_back(it.key());
    }
    return ret;
}

void PersistentStore::save()
{
    std::lock_guard<std::mutex> guard(_lock);
    std::ofstream ofs(_fname);
    ofs << std::setw(4) << _json << std::endl;
}