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

#include "fruit/fruit.h"


fruit::Component<std::ostream> get_ostream_component(void) {
  return fruit::createComponent().bindInstance(std::cout);
}

fruit::Component<book::Serializer> get_serializer_component(void) {
  return fruit::createComponent()
    .bind<book::Serializer, book::SerializerImpl>()
    .bind<book::InputValidator, book::InputValidatorImpl>();
}

fruit::Component<book::Viewer> get_viewer_component(void) {
  return fruit::createComponent().bind<book::Viewer, book::ViewerImpl>()
    .install(get_ostream_component)
    .install(get_serializer_component);
}

fruit::Component<std::string const> get_filepath_component(std::string const filepath) {
  return fruit::createComponent().bindInstance(filepath);
}

fruit::Component<book::Analyzer> get_analyzer_component(std::string const filepath) {
  return fruit::createComponent().bind<book::Analyzer, book::AnalyzerImpl>()
    .bind<book::InputReaderFactory, book::InputReaderFactoryImpl>()
    .bind<book::InputValidator, book::InputValidatorImpl>()
    .bind<book::CommandController, book::CommandControllerImpl>()
    .bind<book::DataHandler, book::LocalDataHandler>()
    .install(get_serializer_component)
    .install(get_viewer_component)
    .install(get_filepath_component, filepath);
}

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

    fruit::Injector<book::Analyzer> injector(get_analyzer_component, filepath);
    book::Analyzer * analyzer = injector.get<book::Analyzer*>();
    return analyzer->run(target_shares);
}
