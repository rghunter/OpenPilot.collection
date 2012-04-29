/*
 * $Id: test_chemical.cc 2009-01-05 motard $
 *
 * a test for the C++ ChemicalSensorProxy
 */

#include "test.h"

int test_chemical(PlayerClient *client, int index)
{
  TEST("chemical sensor");
  try
  {
    using namespace PlayerCc;

    ChemicalProxy cp(client, index);

    for (int i=0; i<10; ++i)
    {
      TEST("read chemical");
      client->Read();
      PASS();

      std::cout << cp << std::endl;

    }
  }
  catch (PlayerCc::PlayerError e)
  {
    std::cerr << e << std::endl;
    return -1;
  }
  return 1;
}
