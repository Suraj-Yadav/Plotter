#ifndef GRIDFUNC_H
#define GRIDFUNC_H

void plotYvsXAdap(Expression &expr, sf::RenderWindow& window, std::vector<sf::Vertex> &Curve);
void plotFunc(Expression &expr, sf::RenderWindow& window);
void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom, bool useMidPoint);
void drawGrid(sf::RenderWindow &window, bool change);
bool isOnYXCurve(const std::vector<sf::Vertex> &curve, sf::Vector2f pos);


#endif // GRIDFUNC_H
