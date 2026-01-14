#pragma once
#include <SFML/Graphics.hpp>
#include "AnimationController.h"

struct Piece : public sf::Drawable, public sf::Transformable
{
    sf::Sprite m_sprite;
    AnimationController m_animator;

    Piece(const sf::Texture& texture,int cellSize, Character& character);

    void update(float dt);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};