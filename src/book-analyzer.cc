#include <iostream>
#include <string>

#include "analyzer/analyzer.h"
#include "commands/command.h"
#include "commands/command-controller.h"
#include "logger/log.h"
#include "data/data-handler.h"
#include "logger/log.h"
#include "readers/input-reader.h"
#include "serializers/serializer.h"
#include "viewers/viewer.h"

int main(int argc, char ** argv) {
    static char const * usage = "book-analyzer TARGET_SIZE [FILE]\n"
    "TARGET_SIZE\ttarget shares in order size\n";

    logger::Log::set_default_max_sev(logger::WARN);
    logger::Log log(__FILE__);

    if (argc <= 1) {
        log.error("missing TARGET_SIZE\n%s", usage);
        return -1;
    }

    long target_shares = std::stol(argv[1]);

    std::string filepath;

    book::InputReaderFactory factory(filepath);
    book::InputValidator validator;
    book::LocalDataHandler handler;
    book::Serializer serializer(validator);
    book::Viewer viewer(std::cout, serializer);
    book::CommandController controller;
    book::Analyzer analyzer(factory, validator, serializer, controller, handler, viewer);

    return analyzer.run(target_shares);
}
