#pragma once
#include "GameDefs.h"
#include "Piece.h"

struct PieceStack : public sf::Drawable, public sf::Transformable
{
public:
	PieceStack(sf::Font& font, sf::Texture& texture, sf::Vector2f size, int pieceSize, int animCellSize,Character& character);
	~PieceStack();

	void bindToWindow(const sf::RenderWindow& window);
	void setRelativePosition(sf::Vector2f perc);
	void update(float dt);
	void handleResize();

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	void applyPositioning();
	void alignElements();

	sf::Texture m_texture;
	std::vector<Piece> m_stack;

	sf::Text m_text;

	const sf::RenderWindow* m_window{ nullptr };

	sf::Vector2f m_relativePosPercent = { -1,-1 };

	int m_cellSize;

	float m_aspectRatio = (16.f / 9.f);
	float m_heightPercent = 0.f;

	sf::RectangleShape m_boundsBox;

	Character m_character;
};