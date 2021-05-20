#ifndef ANALYZER_H
#define ANALYZER_H

#include "commands/command-controller.h"
#include "data/data-handler.h"
#include "logger/log.h"
#include "readers/input-reader.h"
#include "serializers/serializer.h"
#include "util/utility.h"
#include "validators/input-validator.h"
#include "viewers/viewer.h"
#include <functional>
#include <memory>
#include <vector>

namespace book {
class Analyzer {

  InputReaderFactory *factory;
  InputValidator *validator;
  Serializer *serializer;
  CommandController *controller;
  DataHandler *handler;
  Viewer *viewer;

  static logger::Log log;

public:
  Analyzer(InputReaderFactory &factory, InputValidator &validator,
           Serializer &serializer, CommandController &controller,
           DataHandler &handler, Viewer &viewer)
      : factory(&factory), validator(&validator), serializer(&serializer),
        controller(&controller), handler(&handler), viewer(&viewer) {}

  int run(long target_shares) {
    on_init();
    std::unique_ptr<InputReader> reader = factory->create_reader();
    Page page;

    do {
      int rc = reader->get_page(page);
      log.info("Read page of %ld entries", page.get_token());
      if (rc)
        return rc;

      rc = page.read([=](std::string const &line) {
        log.info("-------------------------------------------------------------"
                 "-------------------");
        log.info("Line: %s", line.c_str());
        auto args = utility::split_line(line);
        std::string arg_log;
        for (auto &it : args)
          arg_log += it + ",";
        if (!validator->validate_order(args))
          return -1;
        auto command =
            controller->parse_command(args[1], *serializer, *handler, *viewer);
        if (command)
          command->exec(args, target_shares);
        else
          log.warn("malformed command");

        return rc;
      });
      if (rc)
        return rc;
    } while (page.get_token() >= 0);

    return 0;
  }

private:
  void on_init(void) {
    controller->subscribe("A", std::bind(Command::make_command<AddOrderCommand>,
                                         std::placeholders::_1,
                                         std::placeholders::_2,
                                         std::placeholders::_3));
    controller->subscribe(
        "R", std::bind(Command::make_command<ReduceOrderCommand>,
                       std::placeholders::_1, std::placeholders::_2,
                       std::placeholders::_3));
  }
};

logger::Log Analyzer::log(__FILE__);
} // namespace book

#endif // !ANALYZER_H
