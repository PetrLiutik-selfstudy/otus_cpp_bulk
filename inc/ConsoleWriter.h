#pragma once

#include "../inc/IStreamWriter.h"

#include <iostream>
#include <ostream>

namespace bulk {

/**
 * @brief Класс вывода блока команд в консоль.
 */
class ConsoleWriter : public IStreamWriter {
  public:
    /**
     * @brief Консруктор.
     * @param os - поток для вывода.
     */
    explicit ConsoleWriter(std::ostream& os = std::cout) : os_(os) {
    }

    ~ConsoleWriter() override = default;

    /**
     * @brief Запись блока команд в поток.
     * @param time - время начала блока команд.
     * @param bulk - блок команд.
     */
    void write(const std::time_t& time, const std::vector<std::string>& bulk) final;

  private:
    /// Поток для вывода.
    std::ostream& os_;
};

} // namespace bulk.
