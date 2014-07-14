/*
 * example_setup_first.cpp
 *
 *  Created on: 8 Jul 2014
 *      Author: Holger Schmitz
 *       Email: holger@notjustphysics.com
 */


#include "../src/parser/parser.hpp"
#include "../src/variables/block.hpp"

#include <iostream>
#include <fstream>

using namespace schnek;

class SimulationBlock : public Block
{
  private:
    int size;
    double dx;
    std::string name;

  protected:
    void initParameters(BlockParameters &parameters)
    {
      parameters.addParameter("size", &size);
      parameters.addParameter("dx", &dx);
      parameters.addParameter("name", &name);
    }

  public:
    void writeValues()
    {
      std::cout << "size = " << size << std::endl;
      std::cout << "dx = " << dx << std::endl;
      std::cout << "name = " << name << std::endl;
    }
};

int main()
{
  BlockClasses blocks;

  blocks.addBlock("sim").setClass<SimulationBlock>();

//  blocks.addBlock("sim");
//  blocks("sim").setClass<SimulationBlock>();

  std::ifstream in("example_setup_first03.setup");

  Parser P("my_simulation", "sim", blocks);
  registerCMath(P.getFunctionRegistry());
  pBlock application = P.parse(in);

  SimulationBlock &mysim = dynamic_cast<SimulationBlock&>(*application);
  mysim.evaluateParameters();
  mysim.writeValues();

  return 0;
}




