/*
 * blockparameters.hpp
 *
 *  Created on: 1 May 2012
 *      Author: Holger Schmitz
 *       Email: h.schmitz@imperial.ac.uk
 */

#ifndef BLOCKPARAMETERS_HPP_
#define BLOCKPARAMETERS_HPP_

#include "parsercontext.hpp"
#include "../variables/types.hpp"
#include "../variables/variables.hpp"

#include <boost/foreach.hpp>
#include <map>

namespace schnek {

class ParameterBase
{
  protected:
    std::string varName;
    pVariable variable;
  public:
    ParameterBase(std::string varName_, pVariable variable_)
      : varName(varName_), variable(variable_)
    {}

    bool canEvaluate() { return (variable) && (variable->isInitialised()); }

    virtual void evaluate() = 0;
};

typedef boost::shared_ptr<ParameterBase> pParameterBase;

template<typename T>
class Parameter : public ParameterBase
{
  protected:
    T *value;
  public:
    Parameter(std::string varName_, pVariable variable_, T *value_)
      : ParameterBase(varName_, variable_), value(value_) {}

    void evaluate()
    {
      if (! variable->isInitialised())
        throw VariableNotInitialisedException();

      if (variable->isConstant())
        *value = boost::get<T>(variable->getValue());
      else
        *value = boost::get<T>(variable->evaluateExpression());
    }
};

class BlockParameters
{
  private:
    pBlockVariables block;
    std::map<std::string, pParameterBase> parameterMap;
  public:
    void setContext(ParserContext context)
    {
      block = context.variables->getCurrentBlock();
    }

    template<typename T>
    void addParameter(std::string varName, T* var)
    {
      T defaultValue = T(); // ensure default values

      pVariable variable(new Variable(defaultValue, false));
      block->addVariable(varName, variable);

      pParameterBase par(new Parameter<T>(varName, variable, var));
      parameterMap[varName] = par;
    }

    void evaluate()
    {
      typedef std::pair<std::string, pParameterBase> ParameterPair;
      BOOST_FOREACH(ParameterPair par, parameterMap)
      {
        par.second->evaluate();
      }
    }
};

} //namespace

#endif /* BLOCKPARAMETERS_HPP_ */
