/** Copyright 2008, 2009, 2010, 2011, 2012 Roland Olbricht
*
* This file is part of Overpass_API.
*
* Overpass_API is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as
* published by the Free Software Foundation, either version 3 of the
* License, or (at your option) any later version.
*
* Overpass_API is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with Overpass_API.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "../data/tag_store.h"
#include "../data/utils.h"
#include "unary_operators.h"


Evaluator_Prefix_Operator::Evaluator_Prefix_Operator(int line_number_) : Evaluator(line_number_), rhs(0) {}


void Evaluator_Prefix_Operator::add_statement(Statement* statement, std::string text)
{
  Evaluator* tag_value_ = dynamic_cast< Evaluator* >(statement);
  if (!tag_value_)
    substatement_error(get_name(), statement);
  else if (!rhs)
    rhs = tag_value_;
  else
    add_static_error(get_name() + " must have exactly one evaluator substatements.");
}


Eval_Task* Evaluator_Prefix_Operator::get_task(const Prepare_Task_Context& context)
{
  Eval_Task* rhs_task = rhs ? rhs->get_task(context) : 0;
  return new Unary_Eval_Task(rhs_task, this);
}


std::string Unary_Eval_Task::eval(const std::string* key) const
{
  return evaluator->process(rhs ? rhs->eval(key) : "");
}


std::string Unary_Eval_Task::eval(const Node_Skeleton* elem,
    const std::vector< std::pair< std::string, std::string > >* tags, const std::string* key) const
{
  return evaluator->process(rhs ? rhs->eval(elem, tags, key) : "");
}


std::string Unary_Eval_Task::eval(const Attic< Node_Skeleton >* elem,
    const std::vector< std::pair< std::string, std::string > >* tags, const std::string* key) const
{
  return evaluator->process(rhs ? rhs->eval(elem, tags, key) : "");
}


std::string Unary_Eval_Task::eval(const Way_Skeleton* elem,
    const std::vector< std::pair< std::string, std::string > >* tags, const std::string* key) const
{
  return evaluator->process(rhs ? rhs->eval(elem, tags, key) : "");
}


std::string Unary_Eval_Task::eval(const Attic< Way_Skeleton >* elem,
    const std::vector< std::pair< std::string, std::string > >* tags, const std::string* key) const
{
  return evaluator->process(rhs ? rhs->eval(elem, tags, key) : "");
}


std::string Unary_Eval_Task::eval(const Relation_Skeleton* elem,
    const std::vector< std::pair< std::string, std::string > >* tags, const std::string* key) const
{
  return evaluator->process(rhs ? rhs->eval(elem, tags, key) : "");
}


std::string Unary_Eval_Task::eval(const Attic< Relation_Skeleton >* elem,
    const std::vector< std::pair< std::string, std::string > >* tags, const std::string* key) const
{
  return evaluator->process(rhs ? rhs->eval(elem, tags, key) : "");
}


std::string Unary_Eval_Task::eval(const Area_Skeleton* elem,
    const std::vector< std::pair< std::string, std::string > >* tags, const std::string* key) const
{
  return evaluator->process(rhs ? rhs->eval(elem, tags, key) : "");
}


std::string Unary_Eval_Task::eval(const Derived_Skeleton* elem,
    const std::vector< std::pair< std::string, std::string > >* tags, const std::string* key) const
{
  return evaluator->process(rhs ? rhs->eval(elem, tags, key) : "");
}
  

std::pair< std::vector< Set_Usage >, uint > Evaluator_Prefix_Operator::used_sets() const
{
  if (rhs)
    return rhs->used_sets();
  return std::make_pair(std::vector< Set_Usage >(), 0u);
}


std::vector< std::string > Evaluator_Prefix_Operator::used_tags() const
{
  if (rhs)
    return rhs->used_tags();
  
  return std::vector< std::string >();
}


//-----------------------------------------------------------------------------


Generic_Statement_Maker< Evaluator_Not > Evaluator_Not::statement_maker("eval-not");


Evaluator_Not::Evaluator_Not
    (int line_number_, const std::map< std::string, std::string >& input_attributes, Parsed_Query& global_settings)
    : Evaluator_Prefix_Operator(line_number_)
{
  std::map< std::string, std::string > attributes;  
  eval_attributes_array(get_name(), attributes, input_attributes);
}


std::string Evaluator_Not::process(const std::string& rhs_s) const
{
  double rhs_d = 0;  
  if (try_double(rhs_s, rhs_d))
    return !rhs_d ? "1" : "0";
  
  return rhs_s == "" ? "1" : "0";
}


//-----------------------------------------------------------------------------


Generic_Statement_Maker< Evaluator_Negate > Evaluator_Negate::statement_maker("eval-negate");


Evaluator_Negate::Evaluator_Negate
    (int line_number_, const std::map< std::string, std::string >& input_attributes, Parsed_Query& global_settings)
    : Evaluator_Prefix_Operator(line_number_)
{
  std::map< std::string, std::string > attributes;  
  eval_attributes_array(get_name(), attributes, input_attributes);
}


std::string Evaluator_Negate::process(const std::string& rhs_s) const
{
  int64 rhs_l = 0;  
  if (try_int64(rhs_s, rhs_l))
    return to_string(-rhs_l);
  
  double rhs_d = 0;  
  if (try_double(rhs_s, rhs_d))
    return to_string(-rhs_d);
  
  return "NaN";
}


//-----------------------------------------------------------------------------


Generic_Statement_Maker< Evaluator_Number > Evaluator_Number::statement_maker("eval-number");


Evaluator_Number::Evaluator_Number
    (int line_number_, const std::map< std::string, std::string >& input_attributes, Parsed_Query& global_settings)
    : Evaluator_Prefix_Operator(line_number_)
{
  std::map< std::string, std::string > attributes;  
  eval_attributes_array(get_name(), attributes, input_attributes);
}


std::string Evaluator_Number::process(const std::string& rhs_s) const
{
  int64 rhs_l = 0;
  if (try_int64(rhs_s, rhs_l))
    return to_string(rhs_l);
  
  double rhs_d = 0;  
  if (try_double(rhs_s, rhs_d))
    return to_string(rhs_d);
  
  return "NaN";
}


//-----------------------------------------------------------------------------


Generic_Statement_Maker< Evaluator_Is_Num > Evaluator_Is_Num::statement_maker("eval-is-num");


Evaluator_Is_Num::Evaluator_Is_Num
    (int line_number_, const std::map< std::string, std::string >& input_attributes, Parsed_Query& global_settings)
    : Evaluator_Prefix_Operator(line_number_)
{
  std::map< std::string, std::string > attributes;  
  eval_attributes_array(get_name(), attributes, input_attributes);
}


std::string Evaluator_Is_Num::process(const std::string& rhs_s) const
{
  int64 rhs_l = 0;
  if (try_int64(rhs_s, rhs_l))
    return "1";
  
  double rhs_d = 0;  
  if (try_double(rhs_s, rhs_d))
    return "1";
  
  return "0";
}