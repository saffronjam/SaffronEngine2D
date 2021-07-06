#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "Saffron/Base.h"
#include "Saffron/Core/BatchLoader.h"
#include "Saffron/Gui/FadePane.h"

namespace Se
{
class SplashScreenPane
{
public:
	explicit SplashScreenPane(const std::shared_ptr<class BatchLoader>& batchLoader);

	void OnUpdate();
	void OnGuiRender();

	auto BatchLoader() const -> const std::shared_ptr<class BatchLoader>&;

	void Show();
	void Hide();
	
	auto Idle() const -> bool;
	auto Finished() const -> bool;
	auto Hidden() const -> bool;

private:
	String _title;

	std::shared_ptr<class BatchLoader> _batchLoader;
	Shared<sf::Texture> _texture;
	bool _hidden = false;
	bool _finished = false;
	String _finalizingStatus;

	sf::Time _progressTimer = sf::Time::Zero;
	float _progressView = 0.0f;
	float _progressViewFinished = 0.0f;

	sf::Time _holdTimer = sf::Time::Zero;
	const sf::Time _holdTimerFinished = sf::seconds(0.8f);

	FadePane _fadeIn;
	FadePane _fadeOut;
};
}