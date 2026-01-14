#include "Animation.h"

Animation::Animation(int row, int frames, sf::Vector2i size, float speed, bool loop)
    : m_row(row), m_frameCount(frames), m_frameSize(size),
    m_speed(speed), m_loop(loop),
    m_timer(0.f), m_currentFrame(0), m_finished(false)
{
}

sf::IntRect Animation::update(float dt)
{
    if (m_finished)
        return calculateRect(m_frameCount - 1);

    m_timer += dt;

    if (m_timer >= m_speed)
    {
        m_timer -= m_speed;
        m_currentFrame++;

        if (m_currentFrame >= m_frameCount)
        {
            if (m_loop) {
                m_currentFrame = 0;
            }
            else {
                m_currentFrame = m_frameCount - 1;
                m_finished = true;
            }
        }
    }
    return calculateRect(m_currentFrame);
}

void Animation::reset()
{
    m_timer = 0.f;
    m_currentFrame = 0;
    m_finished = false;
}

sf::IntRect Animation::calculateRect(int frameIndex) const
{
    sf::Vector2i position(
        frameIndex * m_frameSize.x,
        m_row * m_frameSize.y
    );
    return sf::IntRect(position,m_frameSize);
}