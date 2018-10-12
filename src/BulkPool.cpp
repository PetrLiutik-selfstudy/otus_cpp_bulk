#include "../inc/BulkPool.h"

namespace bulk {

void BulkPool::push(const std::string& cmd) {
  if(pool_.empty())
    start_bulk_time_ = std::time(nullptr);
  if(!cmd.empty())
    pool_.emplace_back(cmd);
}

} // namespace bulk.
