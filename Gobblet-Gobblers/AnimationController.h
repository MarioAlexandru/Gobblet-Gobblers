#pragma once
#include "Animation.h"
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>
#include "GameDefs.h"

struct AnimationController
{
    std::unordered_map<std::string, Animation> m_animations;
    Animation* m_currentAnim;
    sf::Sprite& m_targetSprite;

    explicit AnimationController(sf::Sprite& sprite);

    void prepareTexture(const sf::Texture& templateTexture, const Character& character);
    void addAnim(const std::string& name, int row, const sf::Texture& texture, int cellSize, float speed, bool loop = true);

    void play(const std::string& name);
    void update(float dt);

private:
    bool isCellEmpty(const sf::Image& img, int startX, int startY, int size);
    bool isSimilar(sf::Color a, sf::Color b, int tolerance=10);

    sf::Texture m_customTexture;
};