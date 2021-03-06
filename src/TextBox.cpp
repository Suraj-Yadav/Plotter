#include<iostream>
#include "TextBox.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <cctype>
#include "util.h"
#define show(y,x) std::cout<<y"="<<x<<"\n"
//namespace sf {
TextBox::TextBox(unsigned int l, unsigned int size): fontSize(size), length(l) {
	str = "";
	cursor.setSize(sf::Vector2f(2, padding + fontSize));
	currIndex = 0;
	padding = 2;
	focused = false;
	box.setSize(sf::Vector2f(length, size + 2.0f * padding));
	text.setCharacterSize(size);
    box.setOutlineThickness(1.0f);
    box.setOutlineColor(sf::Color::Black);
}
void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
//	text.setCharacterSize(fontSize);
	target.draw(box, states);
	sf::View viewPort = target.getView();
	float xScale = target.getSize().x / viewPort.getSize().x;
	float yScale = target.getSize().y / viewPort.getSize().y;
	text.setString(str);
	sf::View textView(sf::Vector2f(length / 2.0, fontSize / 2.0), sf::Vector2f(length, fontSize));
	textView.setViewport(sf::FloatRect(
			((float)target.mapCoordsToPixel(box.getPosition()).x) / (target.getSize().x),
			((float)target.mapCoordsToPixel(box.getPosition()).y + padding * yScale) / (target.getSize().y),
			((float)length * xScale) / (target.getSize().x),
			((float)fontSize * yScale) / (target.getSize().y)
			));
	target.setView(textView);
	if(!focused) {
		text.setPosition(5, -0.2f * fontSize);
		target.draw(text, states);
		target.setView(viewPort);
		return;
	}
	cursor.setPosition(text.findCharacterPos(currIndex));
	if(cursor.getPosition().x > box.getSize().x - 5.0f) {
		text.move(box.getSize().x - cursor.getPosition().x - 5.0f, 0);
		cursor.move(box.getSize().x - cursor.getPosition().x - 5.0f, 0);
	}
	else if(cursor.getPosition().x < 5.0f) {
		text.move(5.0 - cursor.getPosition().x, 0);
		cursor.move(5.0 - cursor.getPosition().x, 0);
	}
	target.draw(text, states);
	cursor.move(0, 0.2f * fontSize + 0.5f * padding);
	target.draw(cursor, states);
	target.setView(viewPort);
}
void TextBox::setPosition(float x, float y) {
	box.setPosition(x, y);
}
void TextBox::setPosition(const sf::Vector2f& position) {
	box.setPosition(position);
}
const sf::Vector2f& TextBox::getPosition() const {
	return box.getPosition();
}
void TextBox::move(float offsetX, float offsetY) {
	box.move(offsetX, offsetY);
}
void TextBox::move(const sf::Vector2f& offset) {
	box.move(offset);
}
void TextBox::setBackgroundColor(const sf::Color& color) {
	box.setFillColor(color);
}
void TextBox::setOutlineColor(const sf::Color& color) {
	box.setOutlineColor(color);
}
void TextBox::setOutlineThickness(float thickness) {
	box.setOutlineThickness(thickness);
}
const sf::Color& TextBox::getFillColor() const {
	return box.getFillColor();
}
const sf::Color& TextBox::getOutlineColor() const {
	return box.getOutlineColor();
}
float TextBox::getOutlineThickness() const {
	return box.getOutlineThickness();
}
sf::FloatRect TextBox::getLocalBounds() const {
	return box.getLocalBounds();
}
sf::FloatRect TextBox::getGlobalBounds() const {
	return box.getGlobalBounds();
}
void TextBox::setFont(const sf::Font& font) {
	text.setFont(font);
}
void TextBox::setCharacterSize(unsigned int size) {
	fontSize = size;
//		text.setCharacterSize(size);
	box.setSize(sf::Vector2f(length, size + 2.0f * padding));
	cursor.setSize(sf::Vector2f(2, fontSize + padding));
}
void TextBox::setColor(const sf::Color& color) {
	text.setColor(color);
	cursor.setFillColor(color);
}
const sf::String& TextBox::getString() {
	return text.getString();
}
unsigned int TextBox::getCharacterSize() const {
	return fontSize;
}
const sf::Color& TextBox::getTextColor() const {
	return text.getColor();
}
bool TextBox::handleEvent(const sf::Event &event, const sf::Vector2i &mouse) {
	if(event.type == sf::Event::MouseButtonPressed) {
		if(box.getGlobalBounds().contains(mouse.x, mouse.y)) {
			currIndex = str.length();
			focused = true;
			return true;
		}
		else {
			focused = false;
			return false;
		}
	}
	if(focused && event.type == sf::Event::TextEntered) {
		// Handle ASCII characters only
        debf;
		if(event.text.unicode == 8) {
			if(currIndex > 0) {
				str.erase(currIndex - 1, 1);
				text.setString(str);
				currIndex--;
			}
			return true;
		}
		else if(event.text.unicode == 13 || event.text.unicode == 10) {
			focused = false;
			return true;
		}
		else if(isprint(event.text.unicode)) {
			str.insert(currIndex, 1, static_cast<char>(event.text.unicode));
			text.setString(str);
			currIndex++;
			return true;
		}
		return false;
	}
	else if(focused && event.type == sf::Event::KeyPressed) {
		if(event.key.code == sf::Keyboard::Left && currIndex > 0)
		{currIndex--; return true;}
		else if(event.key.code == sf::Keyboard::Right && currIndex < str.size())
		{currIndex++; return true;}
		else if(event.key.code == sf::Keyboard::Delete && currIndex < str.size()) {
			str.erase(currIndex, 1);
			text.setString(str);
			return true;
//						currIndex--;
		}
		return false;
	}
	return false;
}
bool TextBox::hasFocus() {return focused;}
//}
