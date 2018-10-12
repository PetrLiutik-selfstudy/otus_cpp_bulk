#pragma once

#include "../inc/IStreamWriter.h"

#include<memory>

namespace bulk {

/**
 * @brief Интерфейс обозреваемого.
 */
class IObservable {
  public:
    virtual ~IObservable() = default;

    /**
     * @brief Подписка на получение блоков команд на вывод.
     * @param observer - подписчик.
     */
    virtual void subscribe(const std::shared_ptr<IStreamWriter>& observer) = 0;

    /**
     * @brief Отписка от получения блоков команд на вывод.
     * @param observer - подписчик.
     */
    virtual void unsubscribe(const std::shared_ptr<IStreamWriter>& observer) = 0;
};

} // namespace bulk.
