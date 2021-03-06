#pragma once

#include <SFML/Graphics/VertexArray.hpp>

#include "Saffron/Lighting/Light.h"

namespace Se
{
class SpotLight : public Light
{
public:
	using Ptr = std::shared_ptr<SpotLight>;

public:
	static auto Create(const sf::Vector2f& position, const sf::Vector2f& direction, float theta, float radius,
	                   sf::Color color = sf::Color::White) -> SpotLight::Ptr;

	void SetDirection(const sf::Vector2f& newDirection, float theta);
};
}
