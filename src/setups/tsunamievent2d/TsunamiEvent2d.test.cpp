#include <catch2/catch.hpp>
#include "TsunamiEvent2d.h"
#include "string"

TEST_CASE("Test the two-dimensional tsunamiEvent setup.", "[TsunamiEvent2d]")
{

  //create tsunami2d constructor
  tsunami_lab::setups::TsunamiEvent2d* l_tsunamiEvent2d = new tsunami_lab::setups::TsunamiEvent2d(20);

  //test momentumX, should be 0 everywhere
  tsunami_lab::t_real l_momentumX = l_tsunamiEvent2d->getMomentumX(0,0);
  REQUIRE(l_momentumX == Approx(0));

  //test momentumY, should be 0 everywhere
  tsunami_lab::t_real l_momentumY = l_tsunamiEvent2d->getMomentumY(0,0);
  REQUIRE(l_momentumY == Approx(0));

  //check if the getBathymetry function works
  tsunami_lab::t_real l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(0,0);
  REQUIRE(l_bathymetryValue == Approx(-20));
  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(9,0);
  REQUIRE(l_bathymetryValue == Approx(-20));
  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(0,4);
  REQUIRE(l_bathymetryValue == Approx(-20));
l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(9,4);
  REQUIRE(l_bathymetryValue == Approx(-20));

  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(3,1);
  REQUIRE(l_bathymetryValue == Approx(-20));
  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(4,1);
  REQUIRE(l_bathymetryValue == Approx(-17));
  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(5,1);
  REQUIRE(l_bathymetryValue == Approx(-14));
  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(6,1);
  REQUIRE(l_bathymetryValue == Approx(-11));
  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(7,1);
  REQUIRE(l_bathymetryValue == Approx(-20));

  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(3,2);
  REQUIRE(l_bathymetryValue == Approx(-19));
  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(4,2);
  REQUIRE(l_bathymetryValue == Approx(-16));
  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(5,2);
  REQUIRE(l_bathymetryValue == Approx(-13));
  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(6,2);
  REQUIRE(l_bathymetryValue == Approx(-10));
  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(7,2);
  REQUIRE(l_bathymetryValue == Approx(-19));

  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(3,3);
  REQUIRE(l_bathymetryValue == Approx(-20));
  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(4,3);
  REQUIRE(l_bathymetryValue == Approx(-17));
  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(5,3);
  REQUIRE(l_bathymetryValue == Approx(-14));
  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(6,3);
  REQUIRE(l_bathymetryValue == Approx(-11));
  l_bathymetryValue = l_tsunamiEvent2d->getBathymetry(7,3);
  REQUIRE(l_bathymetryValue == Approx(-20));


  //check if getHeight works correct in combination with displacements, should be 20 everywhere
  tsunami_lab::t_real l_heightValue = l_tsunamiEvent2d->getHeight(2,1);
  REQUIRE(l_heightValue == 20);
  l_heightValue = l_tsunamiEvent2d->getHeight(3,1);
  REQUIRE(l_heightValue == 20);
  l_heightValue = l_tsunamiEvent2d->getHeight(3,2);
  REQUIRE(l_heightValue == 20);
  l_heightValue = l_tsunamiEvent2d->getHeight(3,3);
  REQUIRE(l_heightValue == 20);
  l_heightValue = l_tsunamiEvent2d->getHeight(5,1);
  REQUIRE(l_heightValue == 20);
  l_heightValue = l_tsunamiEvent2d->getHeight(5,2);
  REQUIRE(l_heightValue == 20);
  l_heightValue = l_tsunamiEvent2d->getHeight(5,3);
  REQUIRE(l_heightValue == 20);
  l_heightValue = l_tsunamiEvent2d->getHeight(7,1);
  REQUIRE(l_heightValue == 20);
  l_heightValue = l_tsunamiEvent2d->getHeight(7,2);
  REQUIRE(l_heightValue == 20);
  l_heightValue = l_tsunamiEvent2d->getHeight(7,3);
  REQUIRE(l_heightValue == 20);

  l_heightValue = l_tsunamiEvent2d->getHeight(0,0);
  REQUIRE(l_heightValue == 20);
  l_heightValue = l_tsunamiEvent2d->getHeight(0,4);
  REQUIRE(l_heightValue == 20);
  l_heightValue = l_tsunamiEvent2d->getHeight(9,0);
  REQUIRE(l_heightValue == 20);
  l_heightValue = l_tsunamiEvent2d->getHeight(9,4);
  REQUIRE(l_heightValue == 20);
}