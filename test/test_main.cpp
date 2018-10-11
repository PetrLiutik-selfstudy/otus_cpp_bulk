#include "gtest/gtest.h"
#include "../inc/ver.h"
#include "../inc/CmdProcessor.h"
#include "../inc/ConsoleWriter.h"
#include "../inc/FileWriter.h"

#include <vector>
#include <string>
#include <sstream>

TEST(ver_test_case, ver_major_test) {
  EXPECT_GE(ver_major(), 1);
}

TEST(ver_test_case, ver_minor_test) {
  EXPECT_GE(ver_minor(), 1);
}

TEST(ver_test_case, ver_patch_test) {
  EXPECT_GE(ver_patch(), 1);
}

namespace bulk {

/**
 * @brief Класс для тестового вывода блока команд.
 */
class TestWriter : public IStreamWriter {
  public:
    TestWriter() = default;
    ~TestWriter() override = default;

    void write(const std::time_t& time, const std::vector<std::string>& bulk) final {
      bulk_ = bulk;
      time_ = time;
    }

    auto get_bulk() {
      return bulk_;
    }

    auto get_time() {
      return time_;
    }

  private:
    std::vector<std::string> bulk_{};
    std::time_t time_{};
};

} // namespace bulk.

TEST(cmd_processor_test_case, subscribe_test) {
  bulk::CmdProcessor cmd_processor{1};
  auto testWriter = std::make_shared<bulk::TestWriter>();
  cmd_processor.subscribe(testWriter);

  std::stringstream ss;
  ss << "cmd1";

  cmd_processor.process(ss);

  EXPECT_EQ(testWriter->get_bulk(), std::vector<std::string>{"cmd1"});
  EXPECT_NE(testWriter->get_time(), std::time_t{});
}

TEST(cmd_processor_test_case, unsubscribe_test) {
  bulk::CmdProcessor cmd_processor{1};
  auto testWriter = std::make_shared<bulk::TestWriter>();
  cmd_processor.subscribe(testWriter);
  cmd_processor.unsubscribe(testWriter);

  std::stringstream ss;
  ss << "cmd1";

  cmd_processor.process(ss);

  EXPECT_EQ(testWriter->get_bulk(), std::vector<std::string>{});
  EXPECT_EQ(testWriter->get_time(), std::time_t{});
}

TEST(cmd_processor_test_case, full_bulk_test) {
  bulk::CmdProcessor cmd_processor{3};
  auto testWriter = std::make_shared<bulk::TestWriter>();
  cmd_processor.subscribe(testWriter);

  std::stringstream ss;
  ss << "cmd1" << std::endl;
  ss << "cmd2" << std::endl;
  ss << "cmd3" << std::endl;

  cmd_processor.process(ss);

  std::vector<std::string> result{"cmd1", "cmd2", "cmd3"};
  EXPECT_EQ(testWriter->get_bulk(), result);
  EXPECT_NE(testWriter->get_time(), std::time_t{});
}

TEST(cmd_processor_test_case, partial_bulk_test) {
  bulk::CmdProcessor cmd_processor{3};
  auto testWriter = std::make_shared<bulk::TestWriter>();
  cmd_processor.subscribe(testWriter);

  std::stringstream ss;
  ss << "cmd1" << std::endl;
  ss << "cmd2" << std::endl;

  cmd_processor.process(ss);

  EXPECT_EQ(testWriter->get_bulk(), std::vector<std::string>{});
  EXPECT_EQ(testWriter->get_time(), std::time_t{});
}

TEST(cmd_processor_test_case, partial_dyn_bulk_test) {
  bulk::CmdProcessor cmd_processor{3};
  auto testWriter = std::make_shared<bulk::TestWriter>();
  cmd_processor.subscribe(testWriter);

  std::stringstream ss;
  ss << "cmd1" << std::endl;
  ss << "cmd2" << std::endl;
  ss << "{" << std::endl;
  ss << "cmd3" << std::endl;
  ss << "cmd4" << std::endl;
  ss << "cmd5" << std::endl;
  ss << "cmd6" << std::endl;

  cmd_processor.process(ss);

  std::vector<std::string> result{"cmd1", "cmd2"};
  //EXPECT_EQ(testWriter->get_bulk(), result);
  //EXPECT_NE(testWriter->get_time(), std::time_t{});
}

TEST(cmd_processor_test_case, full_dyn_bulk_test) {
  bulk::CmdProcessor cmd_processor{3};
  auto testWriter = std::make_shared<bulk::TestWriter>();
  cmd_processor.subscribe(testWriter);

  std::stringstream ss;
  ss << "cmd1" << std::endl;
  ss << "cmd2" << std::endl;
  ss << "{" << std::endl;
  ss << "cmd3" << std::endl;
  ss << "cmd4" << std::endl;
  ss << "cmd5" << std::endl;
  ss << "cmd6" << std::endl;
  ss << "}" << std::endl;

  cmd_processor.process(ss);

  std::vector<std::string> result{"cmd3", "cmd4", "cmd5", "cmd6"};
  EXPECT_EQ(testWriter->get_bulk(), result);
  EXPECT_NE(testWriter->get_time(), std::time_t{});
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
