#pragma once

#include "../inc/IStreamWriter.h"

namespace bulk {

/**
 * @brief Класс вывода блока команд в консоль.
 */
class ConsoleWriter : public IStreamWriter {
  public:
    ConsoleWriter() = default;
    ~ConsoleWriter() override = default;

    /**
     * @brief Запись блока команд в поток.
     * @param time - время начала блока команд.
     * @param bulk - блок команд.
     */
    void write(const std::time_t& time, const std::vector<std::string>& bulk) final;
};

} // namespace bulk.
