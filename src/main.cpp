#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Expression.h"
#include "Button.h"
#include "TextBox.h"
#include "gridFunc.h"
#include "Curve.h"
#include <SFML/OpenGL.hpp>
#define val(x) std::cout<<#x"="<<x<<"\n"
#define show(y,x) std::cout<<y"="<<x<<"\n"
#define deb std::cout<<__LINE__<<"\n"
#define debf std::cout<<__FILE__<<","<<__LINE__<<"\n"
sf::Font font;
sf::Text text;
int getSelected(std::vector<Curve> &curves, sf::Vector2f pos) {
	for(int i = 0; i < curves.size(); ++i)
		if(curves.at(i).isOnCurve(pos))
			return i;
	return -1;
}
//extern sf::VertexArray test;
const float zoomAmount = 1.1f;
int main() {
	bool change = true;
	int selected = -1;
//	test[0].color = sf::Color::Blue;test[1].color = sf::Color::Blue;test[2].color = sf::Color::Blue;
	// Create the main window
	std::vector<Curve> curves;
	std::vector<TextBox> entries;
	curves.emplace_back();
	curves[0].setExpression("sin(2*x)+sin(6*x)/3+sin(10*x)/5+sin(14*x)/7+sin(18*x)/9");
	curves[0].setColor(sf::Color::Red);
	curves.emplace_back();
	curves[1].setExpression("tan(x)");
	curves[1].setColor(sf::Color::Blue);
	sf::RenderWindow window(sf::VideoMode(800, 600), "Plotter");
	sf::View graphView(sf::Vector2f(0, 0), sf::Vector2f(8, 6));
	sf::View uiView = window.getDefaultView();
	window.setView(graphView);
    Button zoomIn;
    zoomIn.loadFromFile("minus.png");
    zoomIn.setPosition(100,100);
    sf::Vector2i p;
    zoomIn.connect(std::bind(zoomViewAt,std::placeholders::_1,std::ref(window),1/zoomAmount,true));
	// Start the game loop
	bool draging = false;
	sf::Vector2u origSize(window.getSize());
	sf::Vector2f beforeCoord;
	while(window.isOpen()) {
		// Process events
		sf::Event event;
//		while(!window.pollEvent(event));
		while(window.pollEvent(event)){
//		do{
			// Close window: exit
            if(zoomIn.handleEvent(event,sf::Mouse::getPosition(window)));
			else if(event.type == sf::Event::Closed)
				window.close();
			else if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Escape)
					window.close();
                else if(event.key.code == sf::Keyboard::Add)
					zoomViewAt({ event.mouseWheel.x, event.mouseWheel.y }, window, (1.f / zoomAmount), true);
				else if(event.key.code == sf::Keyboard::Subtract)
					zoomViewAt({ event.mouseWheel.x, event.mouseWheel.y }, window, (zoomAmount), true);
				else if(event.key.code == sf::Keyboard::Home){
                    graphView.setCenter(0,0);
                    graphView.setSize(8,6);
                    window.setView(graphView);
				}
				change = true;
				graphView = window.getView();
			}
			else if(event.type == sf::Event::MouseWheelMoved) {
				if(event.mouseWheel.delta > 0)
					zoomViewAt({ event.mouseWheel.x, event.mouseWheel.y }, window, (1.f / zoomAmount), false);
				else
					zoomViewAt({ event.mouseWheel.x, event.mouseWheel.y }, window, zoomAmount, false);
				change = true;
				graphView = window.getView();
			}
			else if(event.type == sf::Event::MouseButtonPressed) {
				draging = true;
				beforeCoord = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				change = true;
			}
			else if(event.type == sf::Event::MouseButtonReleased) {
				draging = false;
				change = true;
			}
			else if(event.type == sf::Event::Resized) {
//				std::cout << "Resized\n";
				sf::Vector2u newSize(window.getSize());
				// update the view to the new size of the window
				graphView = window.getView();
				graphView.setSize(newSize.x * graphView.getSize().x / origSize.x, newSize.y * graphView.getSize().y / origSize.y);
				window.setView(graphView);
				origSize = newSize;
				uiView.setSize(window.getSize().x, window.getSize().y);
				uiView.setCenter(window.getSize().x / 2.0, window.getSize().y / 2.0);
				change = true;
			}
		}
		//while(window.pollEvent(event));
		// Clear screen
		window.clear(sf::Color::White);
//		std::cout<<"Drawing\n";
//		val(change);
		graphView = window.getView();
		sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		selected = getSelected(curves,mousePos);
		// Draw the string
		if(draging) {
			sf::Vector2f newCoord(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
			sf::Vector2f offset = beforeCoord - newCoord;
			graphView = window.getView();
			graphView.move(offset);
			window.setView(graphView);
		}
//		debf;
		//Draw the Graph and Things
		drawGrid(window, change);

		if(change) {
            for(Curve &c:curves){
				c.plotYvsXAdap(graphView);
            }
			change = draging;
		}

		glEnable(GL_LINE_SMOOTH);
		glLineWidth(3.0f);
		for(int i = 0; i < curves.size(); ++i){
			if(i==selected){
                glLineWidth(4.0f);
                window.draw(curves.at(i));
                glLineWidth(3.0f);
			}
			window.draw(curves.at(i));
		}
		glDisable(GL_LINE_SMOOTH);
		glLineWidth(1.0f);

		//Draw the UI and Stuff
		window.setView(uiView);
        window.draw(zoomIn);
		//Restore the view
		window.setView(graphView);
		// Update the window
		window.display();
//		std::cin>>str2;
//		window.close();
	}
	return EXIT_SUCCESS;
}


