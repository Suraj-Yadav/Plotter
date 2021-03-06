#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>
#ifndef BUTTON_H
#define BUTTON_H
class Button : public sf::Drawable {
  public:
	/** Default constructor */
	Button();
	~Button();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool loadFromFile(const std::string& filename, const sf::IntRect& area = sf::IntRect());
	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f& position);
	void setScale(float x, float y);
	void setScale(const sf::Vector2f& position);
	const sf::Vector2f& getPosition() const;
	const sf::Vector2f& getScale() const;
	void move(float offsetX, float offsetY);
	void move(const sf::Vector2f& offset);
	void scale(float scaleX, float scaleY);
	void scale(const sf::Vector2f& scale);
	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;
	bool handleEvent(const sf::Event &event, sf::Vector2i mouse);
	void connect(std::function<void(sf::Vector2i)> onClick);
  protected:
  private:
	sf::Sprite box;
	sf::Texture texture;
	std::function<void(sf::Vector2i)> onClick;
	bool focused;
	bool down;
};
#endif // BUTTON_H
