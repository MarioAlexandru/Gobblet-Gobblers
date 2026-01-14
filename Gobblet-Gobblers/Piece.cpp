#include "Piece.h"

Piece::Piece(const sf::Texture& texture, int cellSize,Character& character)
    : m_sprite(texture),m_animator(m_sprite)
{
    m_animator.prepareTexture(texture,character);

    //m_animator.addAnim("RUN", 0, texture, cellSize, 0.1f, false);
    m_animator.addAnim("IDLE", 0, texture, cellSize, 0.2f, true);

    m_sprite.setOrigin(sf::Vector2f(cellSize / 2.f, cellSize / 2.f));

    setScale(sf::Vector2f(5.f, 5.f));

    m_animator.play("IDLE");
}

void Piece::update(float dt)
{
    m_animator.update(dt);
}

void Piece::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_sprite, states);
}