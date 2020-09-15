#ifndef SCENARIO_RUNNER__SYNTAX__SCENARIO_OBJECT_HPP_
#define SCENARIO_RUNNER__SYNTAX__SCENARIO_OBJECT_HPP_

#include <scenario_runner/syntax/entity_object.hpp>
#include <scenario_runner/syntax/entity_ref.hpp>
#include <scenario_runner/syntax/object_controller.hpp>

namespace scenario_runner
{inline namespace syntax
{
/* ==== ScenarioObject =======================================================
 *
 * <xsd:complexType name="ScenarioObject">
 *   <xsd:sequence>
 *     <xsd:group ref="EntityObject"/>
 *     <xsd:element name="ObjectController" minOccurs="0" type="ObjectController"/>
 *   </xsd:sequence>
 *   <xsd:attribute name="name" type="String" use="required"/>
 * </xsd:complexType>
 *
 * ======================================================================== */
struct ScenarioObject
{
  const String name;

  Scope inner_scope;

  Object entity_object, object_controller;

  template<typename Node>
  explicit ScenarioObject(const Node & node, Scope & outer_scope)
  : name{readAttribute<String>(node, outer_scope, "name")},
    inner_scope{outer_scope}
  {
    entity_object = make<EntityObject>(node, inner_scope);

    callWithElements(node, "ObjectController", 0, 1, [&](auto && node)
      {
        return object_controller = make<ObjectController>(node, inner_scope);
      });
  }

  decltype(auto) getEntityObject()
  {
    return entity_object.as<EntityObject>(__FILE__, __LINE__);
  }

  auto evaluate()
  {
    std::cout << indent << "spawn(false, " << name << ", ...)\n" << entity_object << std::endl;
    // return asBoolean(inner_scope.connection->entity->spawn(false, name, boost::lexical_cast<String>(entity_object)));
    return unspecified;
  }
};

template<typename ... Ts>
std::basic_ostream<Ts...> & operator<<(std::basic_ostream<Ts...> & os, const ScenarioObject & rhs)
{
  return os << (indent++) << blue << "<ScenarioObject" << " " <<
         highlight("name", rhs.name) << blue << ">\n" << reset <<
         rhs.entity_object << "\n" <<
         (--indent) << blue << "</ScenarioObject>" << reset;
}
}}  // namespace scenario_runner::syntax

#endif  // SCENARIO_RUNNER__SYNTAX__SCENARIO_OBJECT_HPP_
