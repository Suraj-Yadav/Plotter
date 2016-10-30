#include <SFML/Graphics/RenderTarget.hpp>
#include "../include/util.h"
#include "../include/Curve.h"

extern float xSep;
extern float ySep;
Curve::Curve() {
	m_color = sf::Color::Black;
	m_type = sf::Lines;
}

Curve::Curve(std::string str, sf::Color c) {
	m_color = c;
	m_expr = str;
	m_e.assign(str);
	m_type = sf::Lines;
}

Curve::~Curve() {
	//dtor
}

void Curve::setExpression(std::string str) {
	m_expr = str;
	m_e.assign(m_expr);
}

void Curve::setColor(sf::Color c) {
	m_color = c;
}

sf::Color Curve::getColor() {
	return m_color;
}

void Curve::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(m_curve.size()>0)
	target.draw(&m_curve.at(0), m_curve.size(), m_type, states);
}

void Curve::processStrip(double x, double del, int depth, const float &vertLimit) {
	sf::Vector2f p, q, r;
	p.x = x;				p.y = -m_e.evaluate(x);
	if(std::isinf(p.y)) {
		p.x += del / 25.0;
		p.y = -m_e.evaluate(p.x);
	}
	q.x = x + del / 2.0;	q.y = -m_e.evaluate(x + del / 2.0);
	r.x = x + del;			r.y = -m_e.evaluate(x + del);
	if(std::isinf(r.y)) {
		r.x -= del / 25.0;
		r.y = -m_e.evaluate(r.x);
	}
	double angle = fabs(atan2((p.y - q.y) * (q.x - r.x) - (q.y - r.y) * (p.x - q.x), (p.x - q.x) * (q.x - r.x) + (p.y - q.y) * (q.y - r.y)));
	if(angle >= 1.5)
		return;
	if(angle > 0.5 && depth < 5) {
		this->processStrip(x, del / 2.0, depth + 1, vertLimit);
		this->processStrip(x + del / 2.0, del / 2.0, depth + 1, vertLimit);
	}
	else {
		if(p.y * q.y >= 0 || fabsf(p.y - q.y) < vertLimit) {
			m_curve.emplace_back(p, m_color);
			m_curve.emplace_back(q, m_color);
		}
		if(r.y * q.y >= 0 || fabsf(r.y - q.y) < vertLimit) {
			m_curve.emplace_back(q, m_color);
			m_curve.emplace_back(r, m_color);
		}
	}
}

size_t Curve::plotYvsXAdap(sf::View view) {
	m_curve.clear();
	float Left = view.getCenter().x - view.getSize().x / 2;
	float Right = view.getCenter().x + view.getSize().x / 2;
	float del = xSep / 25;
	double x = Left - fmod(Left, del) - del;
	Right += del;
	for(; x < Right ; x += del) {
		processStrip(x, del, 0, view.getSize().y);
	}
	return m_curve.size();
}

bool Curve::isOnCurve(sf::Vector2f pos) {
	if(m_curve.size() == 0)
		return false;
	unsigned long lo = 0;
	unsigned long hi = m_curve.size() / 2 - 1;
//	val(lo);
//	val(hi);
	while(lo <= hi) {
//		int mid = lo + (hi - lo) / 2;
		unsigned long mid = lo + hi >> 1;
		if(pos.x < m_curve.at(mid << 1).position.x && pos.x < m_curve.at(2 * mid + 1).position.x) hi = mid - 1;
		else if(pos.x > m_curve.at(2 * mid).position.x && pos.x > m_curve.at(2 * mid + 1).position.x) lo = mid + 1;
		else {
			double dist = (xSep * xSep + ySep * ySep) * 0.001;

			return distFromLine(m_curve.at(2 * mid).position, m_curve.at(2 * mid + 1).position, pos) < dist;
		}
	}
	return false;
}
