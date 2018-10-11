#pragma once

#include "../inc/BulkPool.h"
#include "../inc/CmdInterpreter.h"
#include "../inc/IObservable.h"

#include <functional>
#include <list>
#include <memory>

namespace bulk {

/**
 * @brief Класс обработчика команд.
 */
class CmdProcessor : public IObservable  {
  public:
    /**
     * @brief Конструктор.
     * @param bulk_size - макисмальный размер блока команд.
     */
    explicit CmdProcessor(size_t bulk_size) : interpreter_{bulk_size} {
    }

    ~CmdProcessor() override = default;

    /**
     * @brief Подписка на получение блоков команд на вывод.
     * @param observer - подписчик.
     */
    void subscribe(const std::shared_ptr<IStreamWriter>& observer) final;

    /**
     * @brief Отписка от получения блоков команд на вывод.
     * @param observer - подписчик.
     */
    void unsubscribe(const std::shared_ptr<IStreamWriter>& observer) final;

    /**
     * @brief Обработка входной команды.
     * @param is - входной поток, из которого будут вычитываться команды.
     */
    void process(std::istream& is);

  private:
    /// Тип обозревателя.
    using observer_t = std::weak_ptr<IStreamWriter>;
    /// Тип коллбека для вывода содержимого пула.
    using publish_t = std::function<void(const std::time_t& bulk_time,
                                         const std::vector<std::string>& cmds)>;

    /// Коллбек для вывода содержимого пула.
    publish_t publish = [&](const std::time_t& bulk_time,
                            const std::vector<std::string>& bulk) {
      for(auto& it: observers_) {
        if(!it.expired()) {
          auto p = it.lock();
          p->write(bulk_time, bulk);
        }
      }
    };

    /// Список обозревателей, ожидающих вывод содержимого пула.
    std::list<observer_t> observers_{};
    /// Пул блоков команд.
    BulkPool bulk_pool_{};
    /// Интерпретатор команд.
    CmdInterpreter interpreter_;
};

} // namespace bulk.
