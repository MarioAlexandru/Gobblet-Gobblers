#pragma once
#include <SFML/Graphics.hpp>

struct Animation
{
    Animation(int row, int frames, sf::Vector2i size, float speed, bool loop = true);
    
    sf::Vector2i m_frameSize;
    int m_frameCount;
    int m_row;
    float m_speed;
    bool m_loop;

    float m_timer;
    int m_currentFrame;
    bool m_finished;

    sf::IntRect update(float dt);
    void reset();

private:
    sf::IntRect calculateRect(int frameIndex) const;
};