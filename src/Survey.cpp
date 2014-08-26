#include <inttypes.h>

#include <GeometryFactory.h>
#include <Logger.h>
#include <ModelFactory.h>
#include <MPI.h>

#include <Survey.h>

void Gauge::Survey::Parallel(
    int &argc, char **argv,
    Gauge::ProcessorList &&processors,
    Gauge::GeometryFactory *geometry_factory,
    Gauge::InputFactory::Generic *inputs,
    std::string log_file) {

  using namespace Gauge;
  using namespace Gauge::InputFactory;

  const int good_tag = 73, exit_tag = 81, root = 0;

  MPI_Init(&argc, &argv);
  int rank, num_procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  assert(1 < num_procs);

  if (rank == root) {
    Gauge::Logger logger(log_file);
    logger.Log(std::to_string(num_procs) + " processes started.");
    logger.Log(std::to_string(num_procs-1) + " builders started.");
    uint64_t count = 0;

    geometry_factory->Setup(inputs);
    int process = 2;
    while (geometry_factory->NextGeometry()) {
      Gauge::MPI::Send(*geometry_factory->Geometry(), process, good_tag);

      if (count % 1000000 == 0)
        logger.Log(std::to_string(count + 1) + " geometries built");
      ++count;

      process = (process + 1) % (num_procs - 1) + 1;
    }
    delete geometry_factory;

    for (process = 0; process < num_procs; ++process)
      if (process != root) {
        Gauge::MPI::SendTag(process, exit_tag);
        Gauge::ProcessorList *local = processors.LocalList();
        Gauge::MPI::Receive(process, exit_tag, local);
        processors.Merge(*local);
        delete local;
      }

    processors.Finalize();

    logger.Log("Models Constructed: " + std::to_string(count));
  } else {
    ModelFactory *factory = new ModelFactory();
    Gauge::Geometry geometry;
    while (Gauge::MPI::Receive(root, exit_tag, &geometry)) {
      factory->Setup(&geometry);
      if (factory->Build())
        processors.Process(factory->Model());
    }

    Gauge::MPI::Send(processors, root, good_tag);

    delete factory;
  }

  MPI_Finalize();
}

void Gauge::Survey::Serial(
    Gauge::ProcessorList &&processors,
    Gauge::GeometryFactory *geometry_factory,
    Gauge::InputFactory::Generic *inputs,
    std::string log_file) {

  Gauge::Logger logger(log_file);
  logger.Log("Serial processing started.");

  geometry_factory->Setup(inputs);

  ModelFactory* builder = new ModelFactory();

  uint64_t count = 0;

  while (geometry_factory->NextGeometry()) {
    builder->Setup(geometry_factory->Geometry());
    if (builder->Build()) {
      ++count;
      if (count % 10000 == 0)
        logger.Log(std::to_string(count + 1) + " models built");
      processors.Process(builder->Model());
    }
  }

  processors.Finalize();

  logger.Log("Models Constructed: " + std::to_string(count));

  delete builder;
  delete geometry_factory;
}
