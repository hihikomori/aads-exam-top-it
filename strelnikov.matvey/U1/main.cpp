#include <cctype>
#include <fstream>
#include <iostream>
#include <cstddef>
#include <istream>
#include <ostream>
#include <string>
#include "list.hpp"

struct Person
{
  size_t id;
  std::string info;
};

bool containsId(List< Person > *l, size_t id)
{
  for (List< Person > *c = l; c; c = c->next) {
    if (c->val.id == id) {
      return true;
    }
  }

  return false;
}

bool parsePerson(const std::string &line, Person &per)
{
  size_t i = 0;
  size_t len = line.length();

  std::string inft = "";
  size_t idt = 0;
  while (i < len && std::isspace(static_cast< unsigned char >(line[i]))) {
    ++i;
  }

  if (i >= len || !std::isdigit(static_cast< unsigned char >(line[i]))) {
    return false;
  }

  size_t num_start = i;
  while (i < len && std::isdigit(static_cast< unsigned char >(line[i]))) {
    ++i;
  }

  try {
    idt = std::stoull(line.substr(num_start, i - num_start));
  } catch (...) {
    return false;
  }

  bool needSpace = false;

  while (i < len) {
    while (i < len && std::isspace(static_cast< unsigned char >(line[i]))) {
      ++i;
    }

    if (i >= len) {
      break;
    }

    size_t word_start = i;

    while (i < len && !std::isspace(static_cast< unsigned char >(line[i]))) {
      ++i;
    }

    if (needSpace) {
      inft += " ";
    }
    inft += line.substr(word_start, i - word_start);
    needSpace = true;
  }

  if (inft.empty()) {
    return false;
  }

  per.id = idt;
  per.info = inft;
  return true;
}

List< Person > *getLine(std::istream &is, size_t &ign, size_t &acpt)
{
  List< Person > *l = nullptr;
  List< Person > *tail = l;

  std::string currentLine = "";
  char ch;

  while (is.get(ch)) {
    if (ch == '\n') {
      Person p;
      if (!parsePerson(currentLine, p)) {
        ++ign;
        currentLine.clear();
        continue;
      }

      if (containsId(l, p.id)) {
        ++ign;
        currentLine.clear();
        continue;
      }
      if (!l) {
        l = createList(p);
        tail = l;
      } else {
        tail = pushback(tail, p);
      }
      currentLine.clear();
      ++acpt;
    } else {
      currentLine += ch;
    }
  }

  if (!currentLine.empty()) {
    Person p;
    if (!parsePerson(currentLine, p)) {
      ++ign;
      currentLine.clear();
    } else if (containsId(l, p.id)) {
      ++ign;
      currentLine.clear();
    } else {
      ++acpt;
    }

    if (!l) {
      l = createList(p);
      tail = l;
    } else {
      tail = pushback(tail, p);
    }
  }

  return l;
}

void printPersons(List< Person > *l, std::ostream &os)
{
  for (List< Person > *cur = l; cur; cur = cur->next) {
    os << cur->val.id << ' ' << cur->val.info << '\n';
  }
}

int main(int argc, char *argv[])
{
  if (argc > 3) {
    return 1;
  }

  std::string inputFile;
  std::string outputFile;

  for (int i = 1; i < argc; ++i) {
    std::string arg(argv[i]);

    if (arg.find("in:") == 0) {
      if (!inputFile.empty()) {
        return 1;
      }
      inputFile = arg.substr(3);
    } else if (arg.find("out:") == 0) {
      if (!outputFile.empty()) {
        return 1;
      }
      outputFile = arg.substr(4);
    } else {
      return 1;
    }
  }

  std::ifstream fin;
  std::ofstream fout;

  std::istream *in = &std::cin;
  std::ostream *out = &std::cout;

  if (!inputFile.empty()) {
    fin.open(inputFile);

    if (!fin.is_open()) {
      return 2;
    }

    in = &fin;
  }

  if (!outputFile.empty()) {
    fout.open(outputFile);

    if (!fout.is_open()) {
      return 2;
    }

    out = &fout;
  }

  size_t ign = 0;
  size_t acpt = 0;

  List< Person > *persons = getLine(*in, ign, acpt);

  printPersons(persons, *out);

  std::cerr << acpt << ' ' << ign << '\n';

  clearList(persons);

  return 0;
}
