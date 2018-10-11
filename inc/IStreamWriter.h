#pragma once

#include <ctime>
#include <string>
#include <vector>

namespace bulk {

/**
 * @brief Интерфейс для классов выводящих блок команд в поток.
 */
class IStreamWriter {
  public:
    virtual ~IStreamWriter() = default;

    /**
     * @brief Запись блока команд в поток.
     * @param time - время начала блока команд.
     * @param bulk - блок команд.
     */
    virtual void write(const std::time_t& time, const std::vector<std::string>& bulk) = 0;
};

} // namespace bulk.
