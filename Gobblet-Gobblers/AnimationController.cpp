#include "AnimationController.h"
#include <iostream>

AnimationController::AnimationController(sf::Sprite& sprite)
    : m_targetSprite(sprite), m_currentAnim(nullptr)
{
}

void AnimationController::addAnim(const std::string& name, int row, const sf::Texture& texture, int cellSize, float speed, bool loop)
{
    sf::Image img = texture.copyToImage();
    sf::Vector2u size = img.getSize();

    int startY = row * cellSize;

    if (startY >= size.y) {
        std::cout << "Randul " << row << " nu exista in textura!\n";
        return;
    }

    int maxCols = size.x / cellSize;
    int calculatedFrames = 0;

    for (int i = 0; i < maxCols; i++)
    {
        int startX = i * cellSize;

        if (!isCellEmpty(img, startX, startY, cellSize)) {
            calculatedFrames++;
        }
        else {
            break;
        }
    }

    if (calculatedFrames == 0) {
        std::cout << "Nu am gasit niciun frame pe randul " << row << "!\n";
        return;
    }

    sf::Vector2i frameSize(cellSize, cellSize);

    m_animations.emplace(name, Animation(row, calculatedFrames, frameSize, speed, loop));
}

bool AnimationController::isCellEmpty(const sf::Image& img, int startX, int startY, int size)
{
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (img.getPixel(sf::Vector2u(startX + x, startY + y )).a > 0) {
                return false;
            }
        }
    }
    return true;
}

void AnimationController::play(const std::string& name) {
    auto it = m_animations.find(name);
    if (it != m_animations.end()) {
        if (m_currentAnim != &it->second) {
            m_currentAnim = &it->second;
            m_currentAnim->reset();
        }
    }
}

void AnimationController::update(float dt) {
    if (m_currentAnim) {
        sf::IntRect newRect = m_currentAnim->update(dt);
        m_targetSprite.setTextureRect(newRect);
    }
}

void AnimationController::prepareTexture(const sf::Texture& templateTexture, const Character& character) {
    sf::Image img = templateTexture.copyToImage();
    sf::Vector2u size = img.getSize();

    sf::Color keyBase = sf::Color(0, 255, 0);
    sf::Color keyShadow = sf::Color(0, 150, 0);

    sf::Color playerBase = character.palette[character.bodyColor];

    sf::Color playerShadow = sf::Color(
        static_cast<std::uint8_t>(playerBase.r * 0.6f),
        static_cast<std::uint8_t>(playerBase.g * 0.6f),
        static_cast<std::uint8_t>(playerBase.b * 0.6f),
        playerBase.a
    );

    for (unsigned int y = 0; y < size.y; y++) {
        for (unsigned int x = 0; x < size.x; x++) {
            sf::Color current = img.getPixel({ x, y });

            if (current.a == 0) continue;

            if (isSimilar(current, keyBase, 60)) {
                img.setPixel({ x, y }, playerBase);
            }
            else if (isSimilar(current, keyShadow, 60)) {
                img.setPixel({ x, y }, playerShadow);
            }
        }
    }

    m_customTexture.loadFromImage(img);
    m_targetSprite.setTexture(m_customTexture);
}

bool AnimationController::isSimilar(sf::Color a, sf::Color b, int tolerance) {
    return (std::abs((int)a.r - (int)b.r) < tolerance &&
        std::abs((int)a.g - (int)b.g) < tolerance &&
        std::abs((int)a.b - (int)b.b) < tolerance);
}