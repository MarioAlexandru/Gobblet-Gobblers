#include "PieceStack.h"

PieceStack::PieceStack(sf::Font& font, sf::Texture& texture, sf::Vector2f size, int pieceSize, int animCellSize,Character& character)
    : m_text(font), m_cellSize(animCellSize),m_character(character)
{
    m_boundsBox.setSize(sf::Vector2f(size.x, size.y));
    m_boundsBox.setOrigin({0.f,0.f});

    m_text.setString("x2");
    m_text.setFont(font);
    m_text.setCharacterSize(30);
    m_text.setFillColor(sf::Color::Black);
    sf::FloatRect textBounds = m_text.getLocalBounds();
    m_text.setOrigin({ textBounds.size.x / 2,textBounds.size.y / 2 });
    
    m_stack.emplace_back(texture, animCellSize,m_character);
    m_stack.emplace_back(texture, animCellSize,m_character);

    setOrigin(sf::Vector2f{ size.x / 2, size.y / 2 });

    alignElements();
}

PieceStack::~PieceStack() {};

void PieceStack::bindToWindow(const sf::RenderWindow& window)
{
    m_window = &window;

    sf::Vector2u windowSize = window.getSize();
    sf::Vector2f boxSize = m_boundsBox.getSize();

    if (windowSize.y > 0)
    {
        m_heightPercent = boxSize.y / static_cast<float>(windowSize.y);
    }

    handleResize();
}

void PieceStack::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_stack.back(),states);
    target.draw(m_text, states);
}

void PieceStack::setRelativePosition(sf::Vector2f perc)
{
    m_relativePosPercent = perc;

    applyPositioning();
}

void PieceStack::applyPositioning()
{
    if (m_window == nullptr || m_relativePosPercent.x < 0) return;

    sf::Vector2u windowSize = m_window->getSize();

    float x = (m_relativePosPercent.x / 100.f) * static_cast<float>(windowSize.x);
    float y = (m_relativePosPercent.y / 100.f) * static_cast<float>(windowSize.y);

    setPosition(sf::Vector2f{ x, y });
}

void PieceStack::update(float dt)
{
    m_stack.back().update(dt);
}

void PieceStack::alignElements()
{
    sf::Vector2f boxSize = m_boundsBox.getSize();

    float targetHeight = boxSize.y * 0.8f;
    float scaleFactor = targetHeight / static_cast<float>(m_cellSize);

    unsigned int textSize = static_cast<unsigned int>(boxSize.y * 0.5f);
    m_text.setCharacterSize(textSize);
    
    float pieceX = boxSize.x * 0.3f;
    float pieceY = boxSize.y * 0.5f;

    float textX = boxSize.x * 0.7f;
    float textY = boxSize.y * 0.5f;

    for (auto& p : m_stack)
    {
        p.setScale({ scaleFactor,scaleFactor });
        p.setPosition({ pieceX, pieceY });
    }

    sf::FloatRect textBounds = m_text.getLocalBounds();

    m_text.setOrigin(textBounds.getCenter());
    m_text.setPosition({ textX,textY });
}

void PieceStack::handleResize()
{
    if (m_window == nullptr) return;

    sf::Vector2u winSize = m_window->getSize();
    float winW = static_cast<float>(winSize.x);
    float winH = static_cast<float>(winSize.y);

    float maxAllowedWidth = winW * 0.30f;
    float maxAllowedHeight = winH * 0.20f;

    float newWidth, newHeight;

    newHeight = maxAllowedHeight;
    newWidth = newHeight * m_aspectRatio;

    if (newWidth > maxAllowedWidth) {
        newWidth = maxAllowedWidth;
        newHeight = newWidth / m_aspectRatio;
    }

    sf::Vector2f newSize(newWidth, newHeight);

    m_boundsBox.setSize(newSize);
    
    setOrigin({newSize.x / 2.f, newSize.y / 2.f});

    alignElements();
    applyPositioning();
}