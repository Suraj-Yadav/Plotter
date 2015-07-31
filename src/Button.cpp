#include <iostream>
#include "Button.h"
#include <SFML/Graphics/RenderTarget.hpp>

Button::Button() {
	focused = false;
	down = false;
};
Button::~Button() {

};

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(box, states);
}
bool Button::loadFromFile(const std::string& filename, const sf::IntRect& area) {
	bool res = texture.loadFromFile(filename, area);
	if(res)
		box.setTexture(texture);
	return false;
}
void Button::setPosition(float x, float y) {
	box.setPosition(x, y);
}
void Button::setPosition(const sf::Vector2f& position) {
	box.setPosition(position);
}
void Button::setScale(float x, float y) {
	box.setScale(x, y);
}
void Button::setScale(const sf::Vector2f& scale) {
	box.setScale(scale);
}
const sf::Vector2f& Button::getPosition() const {
	return box.getPosition();
}
const sf::Vector2f& Button::getScale() const {
	return box.getScale();
}
void Button::move(float offsetX, float offsetY) {
	box.move(offsetX, offsetY);
}
void Button::move(const sf::Vector2f& offset) {
	box.move(offset);
}
void Button::scale(float scaleX, float scaleY) {
	box.scale(scaleX, scaleY);
}
void Button::scale(const sf::Vector2f& scale) {
	box.move(scale);
}
sf::FloatRect Button::getLocalBounds() const {
	return box.getLocalBounds();
}
sf::FloatRect Button::getGlobalBounds() const {
	return box.getGlobalBounds();
}
bool Button::handleEvent(const sf::Event &event, sf::Vector2i mouse) {
	if(box.getGlobalBounds().contains(mouse.x, mouse.y)) {
		if(event.type == sf::Event::MouseButtonPressed){
            down = true;
            box.setTextureRect(sf::IntRect(128,0,64,64));
			onClick(mouse);
			return true;
		}
		if(event.type == sf::Event::MouseButtonReleased){
            down = false;
            focused = true;
            box.setTextureRect(sf::IntRect(64,0,64,64));
            return true;
		}
		else if(down)
			return true;
        else {
            focused = true;
            box.setTextureRect(sf::IntRect(64,0,64,64));
            return false;
        }
	}
    else if(focused){
        focused = false;
        box.setTextureRect(sf::IntRect(0,0,64,64));
    }
    return false;
}
void Button::connect(std::function<void(sf::Vector2i)> Click) {
	onClick = Click;
}
