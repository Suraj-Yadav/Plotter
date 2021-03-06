#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include "Expression.h"
#include <vector>
#ifndef CURVE_H
#define CURVE_H


class Curve	: public sf::Drawable {
	public:
		Curve();
		Curve(std::string str, sf::Color c);
		~Curve();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void setExpression(std::string str);
        void setColor(sf::Color c);
        sf::Color getColor();
        size_t plotYvsXAdap(sf::View view);
        bool isOnCurve(sf::Vector2f pos);
	private:
        void processStrip(double x, double del,int depth, const float &vertLimit);
        Expression m_e;
        sf::Color m_color;
		std::vector<sf::Vertex> m_curve;
        std::string m_expr;
        sf::PrimitiveType m_type;
};

#endif // CURVE_H
