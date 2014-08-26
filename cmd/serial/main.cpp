
#include <Processor/ByGroup.h>
#include <Survey.h>
#include <Utility.h>

int main(int argc, char** argv) {
  const int D = 4, L = 1;
  const int lower[] = {2}, upper[] = {5};

  const std::string root_dir = "results/L=" + std::to_string(L) + "/";
  Utility::Dir::Create(root_dir);

  Gauge::Survey::Serial(
      // Processors
      { new Gauge::Process::ByGroup(root_dir + "/D=" + std::to_string(D) + "/", false) },
      // Geometry Factory
      Gauge::GeometryFactory::SystematicFactory(),
      // Input Factory
      new Gauge::InputFactory::Range(lower, upper, L, D, Gauge::Input::kSUSY),
      // Log File
      root_dir + "D=" + std::to_string(D) + ".log"
    );

  return 0;
}
