#define val(x) std::cout<<#x"="<<x<<"\n"
#define show(y,x) std::cout<<y"="<<x<<"\n"
#define deb std::cout<<__LINE__<<"\n"
#define debf std::cout<<__FILE__<<","<<__LINE__<<"\n"
template <typename T> float dissq(const sf::Vector2<T> &a, const sf::Vector2<T> &b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}
template <typename T> float dot(const sf::Vector2<T> &a, const sf::Vector2<T> &b) {
	return a.x * b.x + a.y * b.y;
}
template <typename T> float distFromLine(const sf::Vector2<T> &start, const sf::Vector2<T> &end, const sf::Vector2<T> &pt){
    float param = dot(pt-start,end-start)/dissq(end,start);
    if(param<0)
        return dissq(start,pt);
    if(param>1)
		return dissq(end,pt);
    else
		return dissq(pt,start+param*(end-start));
}
