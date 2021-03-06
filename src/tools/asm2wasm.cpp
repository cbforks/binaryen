/*
 * Copyright 2015 WebAssembly Community Group participants
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// asm2wasm console tool
//

#include "support/colors.h"
#include "support/command-line.h"
#include "support/file.h"
#include "wasm-builder.h"
#include "wasm-printing.h"

#include "asm2wasm.h"

using namespace cashew;
using namespace wasm;

int main(int argc, const char *argv[]) {
  bool opts = true;
  bool imprecise = false;

  Options options("asm2wasm", "Translate asm.js files to .wast files");
  options
      .add("--output", "-o", "Output file (stdout if not specified)",
           Options::Arguments::One,
           [](Options *o, const std::string &argument) {
             o->extra["output"] = argument;
             Colors::disable();
           })
      .add("--mapped-globals", "-n", "Mapped globals", Options::Arguments::One,
           [](Options *o, const std::string &argument) {
             std::cerr << "warning: the --mapped-globals/-m option is deprecated (a mapped globals file is no longer needed as we use wasm globals)" << std::endl;
           })
      .add("--mem-init", "-t", "Import a memory initialization file into the output module", Options::Arguments::One,
           [](Options *o, const std::string &argument) {
             o->extra["mem init"] = argument;
           })
      .add("--total-memory", "-m", "Total memory size", Options::Arguments::One,
           [](Options *o, const std::string &argument) {
             o->extra["total memory"] = argument;
           })
      .add("--no-opts", "-n", "Disable optimization passes", Options::Arguments::Zero,
           [&opts](Options *o, const std::string &) {
             opts = false;
           })
      .add("--imprecise", "-i", "Imprecise optimizations", Options::Arguments::Zero,
           [&imprecise](Options *o, const std::string &) {
             imprecise = true;
           })
      .add_positional("INFILE", Options::Arguments::One,
                      [](Options *o, const std::string &argument) {
                        o->extra["infile"] = argument;
                      });
  options.parse(argc, argv);

  const auto &tm_it = options.extra.find("total memory");
  size_t totalMemory =
      tm_it == options.extra.end() ? 16 * 1024 * 1024 : atoi(tm_it->second.c_str());
  if (totalMemory & ~Memory::kPageMask) {
    std::cerr << "Error: total memory size " << totalMemory <<
        " is not a multiple of the 64k wasm page size\n";
    exit(EXIT_FAILURE);
  }

  Asm2WasmPreProcessor pre;
  auto input(
      read_file<std::vector<char>>(options.extra["infile"], Flags::Text, options.debug ? Flags::Debug : Flags::Release));
  char *start = pre.process(input.data());

  if (options.debug) std::cerr << "parsing..." << std::endl;
  cashew::Parser<Ref, DotZeroValueBuilder> builder;
  Ref asmjs = builder.parseToplevel(start);
  if (options.debug) {
    std::cerr << "parsed:" << std::endl;
    asmjs->stringify(std::cerr, true);
    std::cerr << std::endl;
  }

  if (options.debug) std::cerr << "wasming..." << std::endl;
  Module wasm;
  wasm.memory.initial = wasm.memory.max = totalMemory / Memory::kPageSize;
  Asm2WasmBuilder asm2wasm(wasm, pre.memoryGrowth, options.debug, imprecise, opts);
  asm2wasm.processAsm(asmjs);

  // import mem init file, if provided
  const auto &memInit = options.extra.find("mem init");
  if (memInit != options.extra.end()) {
    auto filename = memInit->second.c_str();
    auto data(read_file<std::vector<char>>(filename, Flags::Binary, options.debug ? Flags::Debug : Flags::Release));
    // create the memory segment
    wasm.memory.segments.emplace_back(Builder(wasm).makeGetGlobal(Name("memoryBase"), i32), data);
  }

  if (options.debug) std::cerr << "printing..." << std::endl;
  Output output(options.extra["output"], Flags::Text, options.debug ? Flags::Debug : Flags::Release);
  WasmPrinter::printModule(&wasm, output.getStream());

  if (options.debug) std::cerr << "done." << std::endl;
}
