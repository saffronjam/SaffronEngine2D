#pragma once

#include <SFML/Audio/SoundBuffer.hpp>

#include "Saffron/Resource/ResourceStore.h"

namespace Se
{
class SoundBufferStore : public ResourceStore<sf::SoundBuffer>
{
public:
	static auto Get(const Path& path, bool copy) -> Shared<sf::SoundBuffer>
	{
		return Instance().Fetch(path, copy);
	}

private:
	auto Copy(const Shared<sf::SoundBuffer>& value) -> Shared<sf::SoundBuffer> override
	{
		return CreateShared<sf::SoundBuffer>(*value);
	}

	auto Location() -> Path override
	{
		return "Assets/Sounds/";
	}	

private:
	auto Load(Path path) -> Shared<sf::SoundBuffer> override
	{
		auto resource = CreateShared<sf::SoundBuffer>();
		const auto result = resource->loadFromFile(path.string());
		Debug::Assert(result, "Failed to load SoundBuffer");
		return resource;
	}
};
}
