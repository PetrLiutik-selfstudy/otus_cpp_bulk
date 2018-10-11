#pragma once

#include "../inc/IStreamWriter.h"

namespace bulk {

/**
 * @brief Класс вывода блока команд в файл.
 */
class FileWriter : public IStreamWriter {
  public:
    FileWriter() = default;
    ~FileWriter() override = default;

    /**
     * @brief Запись блока команд в поток.
     * @param time - время начала блока команд.
     * @param bulk - блок команд.
     */
    virtual void write(const std::time_t& time, const std::vector<std::string>& bulk) final;
};

} // namespace bulk.
