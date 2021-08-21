#ifndef INPUT_READER_H
#define INPUT_READER_H

#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

namespace book {
class Page {
private:
  std::stringstream contents;
  long token;

public:
  enum { MAX_PAGE_SIZE = 200 };

  Page() : contents(), token(-1) {}
  ~Page() {}

  int read(std::function<int(std::string const &)> on_read) {
    std::string line;
    while (std::getline(contents, line)) {
      int rc = on_read(line);
      if (rc)
        return rc;
    }
    return 0;
  }

  inline long get_token(void) { return token; }
  inline void set_token(long const token) { this->token = token; }

  friend std::istream &operator>>(std::istream &in, Page &page);
};

inline std::istream &operator>>(std::istream &in, Page &page) {
  in >> page.contents.rdbuf();
  return in;
}

class InputReader {
public:
  virtual ~InputReader(void) {}
  virtual int get_page(Page &page) = 0;
};

class StdInputReader : public InputReader {
private:
  static logger::Log log;

public:
  virtual ~StdInputReader(void) {}

  int get_page(Page &page) {
    if (-1 == std::cin.tellg()) {
      log.warn("stdin is empty");
      return -1;
    }

    std::cin >> page;
    page.set_token(-1l);
    return 0;
  }
};

class FileInputReader : public InputReader {
private:
  std::string filepath;
  static logger::Log log;

public:
  FileInputReader(std::string const &filepath) : filepath(filepath) {}

  virtual ~FileInputReader(void) {}

  int get_page(Page &page) {
    std::ifstream file;
    file.open(filepath);

    if (file.is_open()) {
      file >> page;
    } else {
      log.error("File %s cannot be opened ", filepath.c_str());
      file.close();
      return -1;
    }
    file.close();
    return 0;
  }
};

class InputReaderFactory {
public:
  virtual InputReaderFactory(void) {}
  virtual std::unique_ptr<InputReader> create_reader(void) = 0;
};

class InputReaderFactoryImpl : public InputReaderFactory {
private:
  std::string filepath;

public:
  INJECT(InputReaderFactoryImpl(std::string const &filepath))
      : filepath(filepath) {}
  virtual ~InputReaderFactoryImpl(void) {}

  std::unique_ptr<InputReader> create_reader(void) {
    if (filepath.empty()) {
      return std::make_unique<StdInputReader>();
    } else {
      return std::make_unique<FileInputReader>(filepath);
    }
  }
};

logger::Log FileInputReader::log(__FILE__ "-FileInputReader", logger::INFO);
logger::Log StdInputReader::log(__FILE__ "-StdInputReader", logger::INFO);

} // namespace book

#endif // !INPUT_READER_H
