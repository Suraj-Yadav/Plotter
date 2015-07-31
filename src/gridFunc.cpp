//#include "gridFunc.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <iostream>
#include <sstream>
#include <cmath>
#include "Expression.h"
#include "util.h"
#define abso(x) ((x>0)?(x):(-(x)))
#define val(x) std::cout<<#x"="<<x<<"\n"
#include <SFML/OpenGL.hpp>
float xSep;
float ySep;
template <typename T> std::string to_string(const T &n) {
	std::ostringstream stm ;
	stm << n ;
	return stm.str() ;
}
//bool change;
void draw(int num, double x, double y, double xLen, double yLen, double a, double b, double c, double d, std::vector<sf::Vertex> &Curve) {
	switch(num) {
		//	drawOne
		case 1: case 14:
			//Curve.push_back(sf::Vertex(sf::Vector2f(x+xLen/2,y),sf::Color::Red));
			Curve.push_back(sf::Vertex(sf::Vector2f(x + a * xLen / (a - b), y), sf::Color::Red));
			//Curve.push_back(sf::Vertex(sf::Vector2f(x,y+yLen/2),sf::Color::Red));
			Curve.push_back(sf::Vertex(sf::Vector2f(x, y + a * yLen / (a - d)), sf::Color::Red));
			break;
		case 2: case 13:
			//Curve.push_back(sf::Vertex(sf::Vector2f(x+xLen/2,y),sf::Color::Red));
			Curve.push_back(sf::Vertex(sf::Vector2f(x + a * xLen / (a - b), y), sf::Color::Red));
			//Curve.push_back(sf::Vertex(sf::Vector2f(x+xLen,y+yLen/2),sf::Color::Red));
			Curve.push_back(sf::Vertex(sf::Vector2f(x + xLen, y + b * yLen / (b - c)), sf::Color::Red));
			break;
		case 4: case 11:
			//Curve.push_back(sf::Vertex(sf::Vector2f(x+xLen/2,y+yLen),sf::Color::Red));
			Curve.push_back(sf::Vertex(sf::Vector2f(x + d * xLen / (d - c), y + yLen), sf::Color::Red));
			//Curve.push_back(sf::Vertex(sf::Vector2f(x+xLen,y+yLen/2),sf::Color::Red));
			Curve.push_back(sf::Vertex(sf::Vector2f(x + xLen, y + b * yLen / (b - c)), sf::Color::Red));
			break;
		case 8: case 7:
			//Curve.push_back(sf::Vertex(sf::Vector2f(x+xLen/2,y+yLen),sf::Color::Red));
			Curve.push_back(sf::Vertex(sf::Vector2f(x + d * xLen / (d - c), y + yLen), sf::Color::Red));
			//Curve.push_back(sf::Vertex(sf::Vector2f(x,y+yLen/2),sf::Color::Red));
			Curve.push_back(sf::Vertex(sf::Vector2f(x, y + a * yLen / (a - d)), sf::Color::Red));
			break;
		//  drawTwo
		case 3: case 12:
			//Curve.push_back(sf::Vertex(sf::Vector2f(x,y+yLen/2),sf::Color::Red));
			Curve.push_back(sf::Vertex(sf::Vector2f(x, y + a * yLen / (a - d)), sf::Color::Red));
			//Curve.push_back(sf::Vertex(sf::Vector2f(x+xLen,y+yLen/2),sf::Color::Red));
			Curve.push_back(sf::Vertex(sf::Vector2f(x + xLen, y + b * yLen / (b - c)), sf::Color::Red));
			break;
		case 6: case 9:
			//Curve.push_back(sf::Vertex(sf::Vector2f(x+xLen/2,y),sf::Color::Red));
			Curve.push_back(sf::Vertex(sf::Vector2f(x + a * xLen / (a - b), y), sf::Color::Red));
			//Curve.push_back(sf::Vertex(sf::Vector2f(x+xLen/2,y+yLen),sf::Color::Red));
			Curve.push_back(sf::Vertex(sf::Vector2f(x + d * xLen / (d - c), y + yLen), sf::Color::Red));
			break;
	}
}
void processCell(Expression &expr, double x, double y, double xLen, double yLen, std::vector<sf::Vertex> &Curve) {
	double a, b, c, d;
	a = expr.evaluate(x, -y);
	b = expr.evaluate(x + xLen, -y);
	c = expr.evaluate(x + xLen, -y - yLen);
	d = expr.evaluate(x, -y - yLen);
	int num = ((a > 0.0) ? 1 : 0) + ((b > 0.0) ? 2 : 0) + ((c < 0.0) ? 4 : 0) + ((d > 0.0) ? 8 : 0);
	switch(num) {
		case 1: case 2: case 4: case 7: case 8: case 11: case 13: case 14: case 3: case 6: case 12: case 9:
			draw(num, x, y, xLen, yLen, a, b, c, d, Curve);
//			std::cout<<"["<<x<<","<<y<<","<<num<<"]\t";
			break;
		case 5: case 10:
			std::cout << "Recursion Happens\n";
			processCell(expr, x, y, xLen / 2, yLen / 2, Curve);
			processCell(expr, x + xLen / 2, y, xLen / 2, yLen / 2, Curve);
			processCell(expr, x + xLen / 2, y + yLen / 2, xLen / 2, yLen / 2, Curve);
			processCell(expr, x, y + xLen / 2, xLen / 2, yLen / 2, Curve);
			break;
		case 0: case 15:
			break;
	}
}
void plotFunc(Expression &expr, sf::RenderWindow& window) {
	std::vector<sf::Vertex> Curve;
	sf::View view = window.getView();
	float Left = view.getCenter().x - view.getSize().x / 2;
	float Right = view.getCenter().x + view.getSize().x / 2;
	float Top = view.getCenter().y - view.getSize().y / 2;
	float Bottom = view.getCenter().y + view.getSize().y / 2;
	float xDel = xSep / 20, yDel = ySep / 20;
	Right += xDel;
	Bottom += yDel;
	for(double x = Left - fmod(Left, xDel) - xDel; x < Right ; x += xDel) {
		for(double y = Top - fmod(Top, yDel) - yDel; y > Bottom; y += yDel) {
			processCell(expr, x, y, xDel, yDel, Curve);
//			text.setPosition(x,y);
//			text.setScale(view.getSize().x/window.getSize().x,view.getSize().y/window.getSize().y);
//			text.setString(to_string(expr.evaluate(x,y)));
// window.draw(text);
		}
	}
	window.draw(&Curve[0], Curve.size(), sf::Lines);
}
void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom, bool useMidPoint) {
//	std::cout<<"hi";
	if(useMidPoint)
		pixel = static_cast<sf::Vector2i>(window.getSize() / 2u);
	const sf::Vector2f beforeCoord{window.mapPixelToCoords(pixel)};
	sf::View view{ window.getView() };
	view.zoom(zoom);
	window.setView(view);
	const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel) };
	const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	view.move(offsetCoords);
	window.setView(view);
}
float getSep(float vSize, float wSize) {
	//	val(vSize);val(wSize);
	float vals[] = {5.0, 2.5, 2.0, 1.0, 0.5, 0.25, 0.2, 0.1};
	int index = 1;
	const float maxPix = 140.0f;
	const float minPix = 120.0f;
	float sep = pow(10, floor(log10(vSize)));
	float pix = wSize * sep / vSize;
	float dist = std::min(fabs(vals[0] * pix - maxPix), fabs(vals[0] * pix - minPix));
	for(size_t i = 1; i < sizeof(vals) / sizeof(float); i++) {
		float tdist = std::min(fabs(vals[i] * pix - maxPix), fabs(vals[i] * pix - minPix));
		if(dist > tdist) {
			index = i;
			dist = tdist;
		}
	}
	//val(val);
	return vals[index] * sep;
}
void gridLine(sf::View view, float xSep, float ySep, sf::Color c, std::vector<sf::Vertex> &Line) {
	float Left = view.getCenter().x - view.getSize().x / 2;
	float Right = view.getCenter().x + view.getSize().x / 2;
	float Top = view.getCenter().y - view.getSize().y / 2;
	float Bottom = view.getCenter().y + view.getSize().y / 2;
	int noOfLinesX = Right / xSep - (Left - fmod(Left, xSep)) / xSep + 1;
	int noOfLinesY = Bottom / ySep - (Top - fmod(Top, ySep)) / ySep + 1;
	float x = Left - fmod(Left, xSep);
	for(int i = 0; i < noOfLinesX; i++) {
		Line.emplace_back(sf::Vector2f(x, Top), c);
		Line.emplace_back(sf::Vector2f(x, Bottom), c);
		x += xSep;
	}
	x = Top - fmod(Top, ySep);
	for(int i = 0; i < noOfLinesY; i++) {
		Line.emplace_back(sf::Vector2f(Left, x), c);
		Line.emplace_back(sf::Vector2f(Right, x), c);
		x += ySep;
	}
//	window.draw(&Line[0], Line.size(), sf::Lines);
}
void gridLable(sf::RenderWindow &window, float xSep, float ySep, sf::Color c) {
	sf::View view = window.getView();
	float Left = view.getCenter().x - view.getSize().x / 2;
	float Right = view.getCenter().x + view.getSize().x / 2;
	float Top = view.getCenter().y - view.getSize().y / 2;
	float Bottom = view.getCenter().y + view.getSize().y / 2;
	int noOfLinesX = Right / xSep - (Left - fmod(Left, xSep)) / xSep + 1;
	int noOfLinesY = Bottom / ySep - (Top - fmod(Top, ySep)) / ySep + 1;
	sf::Font font;
	if(!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
		return;
	std::vector<std::string> xLables;
	std::vector<std::string> yLables;
	std::vector<sf::Vector2f> xPos;
	std::vector<sf::Vector2f> yPos;
	xLables.reserve(noOfLinesX);
	yLables.reserve(noOfLinesY);
	xPos.reserve(noOfLinesX);
	yPos.reserve(noOfLinesY);
	double x = Left - fmod(Left, xSep);
	for(int i = 0; i < noOfLinesX; i++, x += xSep) {
		if(x < xSep / 8.0f && x > -xSep / 8.0f)
			xLables.push_back("0");
		else
			xLables.push_back(to_string(x));
		sf::Vector2i v = window.mapCoordsToPixel(sf::Vector2f(x + xSep / 16, 0.0f));
		xPos.emplace_back(v.x, v.y);
	}
	x = Top - fmod(Top, ySep);
	for(int i = 0; i < noOfLinesY; i++, x += ySep) {
		if(x < ySep / 8.0f && x > -ySep / 8.0f)
			yLables.push_back("0");
		else
			yLables.push_back(to_string(-x));
		sf::Vector2i v = window.mapCoordsToPixel(sf::Vector2f(xSep / 16, x));
		yPos.emplace_back(v.x, v.y);
	}
	sf::View orig = window.getView(), uiView;
	uiView.setSize(window.getSize().x, window.getSize().y);
	uiView.setCenter(window.getSize().x / 2.0, window.getSize().y / 2.0);
	window.setView(uiView);
	sf::Text text("", font, 15);
	text.setColor(c);
	for(size_t i = 0; i < xPos.size(); i++) {
		text.setString(xLables.at(i));
		text.setPosition(xPos.at(i));
		sf::Vector2f point(text.getGlobalBounds().left + text.getGlobalBounds().width, text.getGlobalBounds().top + text.getGlobalBounds().height);
		if(point.x > window.getSize().x)
			text.move(window.getSize().x - point.x, 0);
		if(point.y > window.getSize().y)
			text.move(0, window.getSize().y - point.y);
		if(text.getPosition().y < 0)
			text.move(0, -text.getPosition().y);
		window.draw(text);
	}
	for(size_t i = 0; i < yPos.size(); i++) {
		text.setString(yLables.at(i));
		text.setPosition(yPos.at(i));
		sf::Vector2f point(text.getGlobalBounds().left + text.getGlobalBounds().width, text.getGlobalBounds().top + text.getGlobalBounds().height);
//		if (yPos.at(i).y < ySep / 8.0f && yPos.at(i).y > -ySep / 8.0f)	{text.setString("0");std::cout<<"hey";}
		if(point.x > window.getSize().x)
			text.move(window.getSize().x - point.x, 0);
		if(text.getPosition().x < 0)
			text.move(-text.getPosition().x, 0);
		if(point.y > window.getSize().y)
			text.move(0, window.getSize().y - point.y);
		window.draw(text);
	}
	window.setView(orig);
}
void xyAxis(sf::RenderWindow &window) {
	sf::View view = window.getView();
	float Left = view.getCenter().x - view.getSize().x / 2;
	float Right = view.getCenter().x + view.getSize().x / 2;
	float Top = view.getCenter().y - view.getSize().y / 2;
	float Bottom = view.getCenter().y + view.getSize().y / 2;
	glLineWidth(2);
	sf::VertexArray line(sf::Lines, 2);
	line[0].color = line[1].color = sf::Color::Black;
	line[0].position.x = line[1].position.x = 0;
	line[0].position.y = Top - ySep;
	line[1].position.y = Bottom + ySep;
	window.draw(line);
	line[0].position.y = line[1].position.y = 0;
	line[0].position.x = Left - xSep;
	line[1].position.x = Right + xSep;
	window.draw(line);
	glLineWidth(1);
}
void drawGrid(sf::RenderWindow &window, bool change) {
	sf::View view = window.getView();
	static std::vector<sf::Vertex> Line;
	if(change) {
		xSep = getSep(view.getSize().x, window.getSize().x);
		ySep = getSep(view.getSize().y, window.getSize().y);
//		change = false;
//		val(xSep);
//		val(ySep);
		Line.clear();
		gridLine(view, xSep * 0.25, ySep * 0.25, sf::Color(0, 0, 0, 63), Line);
		gridLine(view, xSep, ySep, sf::Color(0, 0, 0, 100), Line);
	}
	window.draw(&Line[0], Line.size(), sf::Lines);
	gridLable(window, xSep, ySep, sf::Color::Black);
	xyAxis(window);
}
