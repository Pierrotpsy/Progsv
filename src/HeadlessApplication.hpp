/*!
 * HeadlessApplication
 */


#pragma once


#include <Application.hpp>


class HeadlessApplication : public Application {
 public:
  HeadlessApplication(int argc, char const** argv)
    : Application(argc, argv) {}

  void run() override;
};

