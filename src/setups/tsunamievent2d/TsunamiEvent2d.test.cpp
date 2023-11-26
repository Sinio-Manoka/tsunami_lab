#include <catch2/catch.hpp>
#include "TsunamiEvent2d.h"
#include "string"

TEST_CASE("Test the two-dimensional tsunamiEvent setup.", "[TsunamiEvent2d]")
{
  tsunami_lab::setups::TsunamiEvent2d* l_tsunamiEvent2d = new tsunami_lab::setups::TsunamiEvent2d(20);

  tsunami_lab::t_real l_momentumX = l_tsunamiEvent2d->getMomentumX(0,0);
  REQUIRE(l_momentumX == Approx(0));

  tsunami_lab::t_real l_momentumY = l_tsunamiEvent2d->getMomentumY(0,0);
  REQUIRE(l_momentumY == Approx(0));

  tsunami_lab::t_real l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(0,0);
  REQUIRE(l_bathymetryValue == Approx(-20));
  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(9,0);
  REQUIRE(l_bathymetryValue == Approx(-20));
  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(0,4);
  REQUIRE(l_bathymetryValue == Approx(-20));
  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(9,4);
  REQUIRE(l_bathymetryValue == Approx(-20));

  tsunami_lab::t_real l_heightValue = l_tsunamiEvent2d->getHeight(2,1);
  l_heightValue = l_tsunamiEvent2d->getHeight(0,0);
  REQUIRE(l_heightValue == 20);
  l_heightValue = l_tsunamiEvent2d->getHeight(0,4);
  REQUIRE(l_heightValue == 20);
  l_heightValue = l_tsunamiEvent2d->getHeight(9,0);
  REQUIRE(l_heightValue == 20);
  l_heightValue = l_tsunamiEvent2d->getHeight(9,4);
  REQUIRE(l_heightValue == 20);

}