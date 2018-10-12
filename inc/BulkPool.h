#pragma once

#include <ctime>
#include <string>
#include <vector>

namespace bulk {

/**
 * @brief Класс пула блока команд.
 */
class BulkPool {
  public:
    BulkPool() = default;
    ~BulkPool() = default;

    /**
     * @brief Поместить команду в пул.
     * @param cmd - команда.
     */
    void push(const std::string& cmd);

    /**
     * @brief Провести прочистку пула с выводом команд.
     * @tparam Pred - тип коллбэка вывода содержимого пула.
     * @param pred - коллбэк вывода содержимого пула.
     */
    template <typename Pred>
    void flush(Pred pred) {
      if(pred != nullptr)
        pred(start_bulk_time_, pool_);
      pool_.clear();
    }

  private:
    /// Собственно сам пул команд.
    std::vector<std::string> pool_{};
    /// Время начала формирования блока команд.
    std::time_t start_bulk_time_{};
};

} // namespace bulk.

