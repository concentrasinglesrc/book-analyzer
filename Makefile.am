ACLOCAL_AMFLAGS = -I m4 --install
AM_CXXFLAGS=-std=c++1y
# Program 
bin_PROGRAMS = book-analyzer
book_analyzer_SOURCES = \
src/book-analyzer.cc

nobase_include_HEADERS = 

book_analyzer_CPPFLAGS = $(BCCP_FLAGS) -I src
# Unit Tests
check_PROGRAMS = unittest
unittest_SOURCES = 

unittest_CPPFLAGS = -I ./googletest/googlemock/include -I./googletest/googletest/include $(BCCP_FLAGS)
unittest_LDADD = -L ./googletest/lib -lgmock -lgmock_main -lgtest -lgtest_main -lpthread

EXTRA_DIST = m4/NOTES
dist_doc = README.md LICENSE
