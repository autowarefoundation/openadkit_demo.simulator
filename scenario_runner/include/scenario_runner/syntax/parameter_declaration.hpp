#ifndef SCENARIO_RUNNER__SYNTAX__PARAMETER_DECLARATION_HPP_
#define SCENARIO_RUNNER__SYNTAX__PARAMETER_DECLARATION_HPP_

#include <scenario_runner/reader/attribute.hpp>

namespace scenario_runner
{inline namespace syntax
{
/* ==== ParameterDeclaration =================================================
 *
 * <xsd:complexType name="ParameterDeclaration">
 *   <xsd:attribute name="name" type="String" use="required"/>
 *   <xsd:attribute name="parameterType" type="ParameterType" use="required"/>
 *   <xsd:attribute name="value" type="String" use="required"/>
 * </xsd:complexType>
 *
 * ======================================================================== */
struct ParameterDeclaration
{
  const String name;

  const ParameterType parameter_type;

  const String value;

  ParameterDeclaration() = default;

  template<typename Node, typename Scope>
  explicit ParameterDeclaration(const Node & node, Scope & scope)
  : name{readAttribute<String>(node, scope, "name")},
    parameter_type{readAttribute<ParameterType>(node, scope, "parameterType")},
    value{readAttribute<String>(node, scope, "value")}
  {
    scope.parameters.emplace(name, evaluate());
  }

  Object evaluate() const
  {
    switch (parameter_type) {
      case ParameterType::INTEGER:
        return make<Integer>(value);

      case ParameterType::DOUBLE:
        return make<Double>(value);

      case ParameterType::STRING:
        return make<String>(value);

      case ParameterType::UNSIGNED_INT:
        return make<UnsignedInt>(value);

      case ParameterType::UNSIGNED_SHORT:
        return make<UnsignedShort>(value);

      case ParameterType::BOOLEAN:
        return make<Boolean>(value);

      case ParameterType::DATE_TIME:
        return make<String>(value);

      default:
        return unspecified;
    }
  }
};

template<typename ... Ts>
std::basic_ostream<Ts...> & operator<<(
  std::basic_ostream<Ts...> & os,
  const ParameterDeclaration & declaration)
{
  return os << indent <<
         blue << "<ParameterDeclaration" <<
         " " << highlight("name", declaration.name) <<
         " " << highlight("parameterType", declaration.parameter_type) <<
         " " << highlight("value", declaration.value) <<
         blue << "/>" <<
         reset;
}
}}  // namespace scenario_runner::syntax

#endif  // SCENARIO_RUNNER__SYNTAX__PARAMETER_DECLARATION_HPP_
