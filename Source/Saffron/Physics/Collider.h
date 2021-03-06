#pragma once

#include <optional>

#include <Box2D/Box2D.h>

#include "Saffron/Base.h"
#include "Saffron/Libraries/GenUtils.h"
#include "Saffron/Libraries/VecUtils.h"

namespace Se
{
class Collider
{
public:
	using Ptr = std::shared_ptr<Collider>;

protected:
	void BuildBase(b2BodyDef* bodyDef, b2FixtureDef* fixtureDef);

public:
	void AssignBody(b2Body* body) { _body = body; }

	auto GetBody() const -> b2Body* { return _body.value(); }

	auto GetFixture() const -> b2Fixture* { return _fixture.value(); }

private:
	std::optional<b2Body*> _body;
	std::optional<b2Fixture*> _fixture;
};
}
