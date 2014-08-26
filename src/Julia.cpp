#include <cassert>
#include <cstring>

#include <algorithm>

#include <Julia.h>
#include <GSOHandler.h>
#include <ModelFactory.h>
#include <Math.h>

char *JBuildSingle(const int *base, int size, int order, bool susy) {
  // Setup the basis
  Gauge::Basis basis(1,size);
  Gauge::BasisVector &bvector = basis.base[0];
  for (size_t i = 0; i < size; ++i) bvector.base[i] = base[i];
  bvector.den = order;
  bvector.SetLeadingTrailing();

  // Create a basis handler
  Gauge::GSOHandler gsohandler;
  if (susy) {
    gsohandler.Setup(basis, Gauge::Input::kFullSUSY);
  } else {
    gsohandler.Setup(basis, Gauge::Input::kReducedSUSY);
  }

  if (!gsohandler.NextGSOMatrix()) {
    return strdup("\0");
  }

  Gauge::Geometry geometry(basis, gsohandler.GSOMatrix());

  // Create a model builder
  Gauge::ModelFactory builder;
  builder.Setup(&geometry);

  // Build the model
  if (builder.Build())
    return strdup(builder.Group().c_str());
  return strdup("\0");
}
