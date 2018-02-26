#include "main.h"

/****************************************************************
 * Main program for Pullet Assembler program.
 *
 * Author/copyright:  Duncan Buell. All rights reserved.
 * Used with permission and modified by: Katherine Haberlin
 * Date: 4 December 2017
 *
 * Note that all file names are entered without extensions.
**/

static const string kTag = "Main: ";

int main(int argc, char *argv[]) {
  string in_filename = "";
  string binary_filename = "";
  string out_filename = "";
  string log_filename = "";

  Scanner in_scanner;
  ofstream out_stream;

  Assembler assembler;

  Utils::CheckArgs(3, argc, argv, "infilename outfilename logfilename");
  in_filename = static_cast<string>(argv[1]) + ".txt";
  binary_filename = static_cast<string>(argv[2]) + ".bin";
  out_filename = static_cast<string>(argv[2]) + ".txt";
  log_filename = static_cast<string>(argv[3]) + ".txt";

  Utils::LogFileOpen(log_filename);
  in_scanner.OpenFile(in_filename);
  Utils::FileOpen(out_stream, out_filename);

  Utils::log_stream << kTag << "Beginning execution\n";
  Utils::log_stream.flush();

  Utils::log_stream << kTag << "logfile '" << log_filename << "'\n";

  assembler.Assemble(in_scanner, binary_filename, out_stream);

  Utils::log_stream << kTag << "Ending execution\n";
  Utils::log_stream.flush();

  Utils::FileClose(Utils::log_stream);

  return 0;
}

