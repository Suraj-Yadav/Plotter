#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/View.hpp>

#ifndef TEXTBOX_H
#define TEXTBOX_H
class TextBox : public sf::Drawable {
  public:
	/** Default constructor*/
	TextBox(unsigned int len, unsigned int size);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setPosition(float x, float y);

	////////////////////////////////////////////////////////////
	/// \brief set the position of the object
	///
	/// This function completely overwrites the previous position.
	/// See the move function to apply an offset based on the previous position instead.
	/// The default position of a transformable object is (0, 0).
	///
	/// \param position New position
	///
	/// \see move, getPosition
	///
	////////////////////////////////////////////////////////////
	void setPosition(const sf::Vector2f& position);

	////////////////////////////////////////////////////////////
	/// \brief get the position of the object
	///
	/// \return Current position
	///
	/// \see setPosition
	///
	////////////////////////////////////////////////////////////
	const sf::Vector2f& getPosition() const;

	////////////////////////////////////////////////////////////
	/// \brief Move the object by a given offset
	///
	/// This function adds to the current position of the object,
	/// unlike setPosition which overwrites it.
	/// Thus, it is equivalent to the following code:
	/// \code
	/// sf::Vector2f pos = object.getPosition();
	/// object.setPosition(pos.x + offsetX, pos.y + offsetY);
	/// \endcode
	///
	/// \param offsetX X offset
	/// \param offsetY Y offset
	///
	/// \see setPosition
	///
	////////////////////////////////////////////////////////////
	void move(float offsetX, float offsetY);

	////////////////////////////////////////////////////////////
	/// \brief Move the object by a given offset
	///
	/// This function adds to the current position of the object,
	/// unlike setPosition which overwrites it.
	/// Thus, it is equivalent to the following code:
	/// \code
	/// object.setPosition(object.getPosition() + offset);
	/// \endcode
	///
	/// \param offset Offset
	///
	/// \see setPosition
	///
	////////////////////////////////////////////////////////////
	void move(const sf::Vector2f& offset);


	////////////////////////////////////////////////////////////
	/// \brief Set the fill color of the shape
	///
	/// This color is modulated (multiplied) with the shape's
	/// texture if any. It can be used to colorize the shape,
	/// or change its global opacity.
	/// You can use sf::Color::Transparent to make the inside of
	/// the shape transparent, and have the outline alone.
	/// By default, the shape's fill color is opaque white.
	///
	/// \param color New color of the shape
	///
	/// \see getFillColor, setOutlineColor
	///
	////////////////////////////////////////////////////////////
	void setBackgroundColor(const sf::Color& color);

	////////////////////////////////////////////////////////////
	/// \brief Set the outline color of the shape
	///
	/// By default, the shape's outline color is opaque white.
	///
	/// \param color New outline color of the shape
	///
	/// \see getOutlineColor, setFillColor
	///
	////////////////////////////////////////////////////////////
	void setOutlineColor(const sf::Color& color);

	////////////////////////////////////////////////////////////
	/// \brief Set the thickness of the shape's outline
	///
	/// Note that negative values are allowed (so that the outline
	/// expands towards the center of the shape), and using zero
	/// disables the outline.
	/// By default, the outline thickness is 0.
	///
	/// \param thickness New outline thickness
	///
	/// \see getOutlineThickness
	///
	////////////////////////////////////////////////////////////
	void setOutlineThickness(float thickness);


	////////////////////////////////////////////////////////////
	/// \brief Get the fill color of the shape
	///
	/// \return Fill color of the shape
	///
	/// \see setFillColor
	///
	////////////////////////////////////////////////////////////
	const sf::Color& getFillColor() const;

	////////////////////////////////////////////////////////////
	/// \brief Get the outline color of the shape
	///
	/// \return Outline color of the shape
	///
	/// \see setOutlineColor
	///
	////////////////////////////////////////////////////////////
	const sf::Color& getOutlineColor() const;

	////////////////////////////////////////////////////////////
	/// \brief Get the outline thickness of the shape
	///
	/// \return Outline thickness of the shape
	///
	/// \see setOutlineThickness
	///
	////////////////////////////////////////////////////////////
	float getOutlineThickness() const;

	////////////////////////////////////////////////////////////
	/// \brief Get the local bounding rectangle of the entity
	///
	/// The returned rectangle is in local coordinates, which means
	/// that it ignores the transformations (translation, rotation,
	/// scale, ...) that are applied to the entity.
	/// In other words, this function returns the bounds of the
	/// entity in the entity's coordinate system.
	///
	/// \return Local bounding rectangle of the entity
	///
	////////////////////////////////////////////////////////////
	sf::FloatRect getLocalBounds() const;

	////////////////////////////////////////////////////////////
	/// \brief Get the global (non-minimal) bounding rectangle of the entity
	///
	/// The returned rectangle is in global coordinates, which means
	/// that it takes into account the transformations (translation,
	/// rotation, scale, ...) that are applied to the entity.
	/// In other words, this function returns the bounds of the
	/// shape in the global 2D world's coordinate system.
	///
	/// This function does not necessarily return the \a minimal
	/// bounding rectangle. It merely ensures that the returned
	/// rectangle covers all the vertices (but possibly more).
	/// This allows for a fast approximation of the bounds as a
	/// first check; you may want to use more precise checks
	/// on top of that.
	///
	/// \return Global bounding rectangle of the entity
	///
	////////////////////////////////////////////////////////////
	sf::FloatRect getGlobalBounds() const;

	void setFont(const sf::Font& font);

	////////////////////////////////////////////////////////////
	/// \brief Set the character size
	///
	/// The default size is 30.
	///
	/// Note that if the used font is a bitmap font, it is not
	/// scalable, thus not all requested sizes will be available
	/// to use. This needs to be taken into consideration when
	/// setting the character size. If you need to display text
	/// of a certain size, make sure the corresponding bitmap
	/// font that supports that size is used.
	///
	/// \param size New character size, in pixels
	///
	/// \see getCharacterSize
	///
	////////////////////////////////////////////////////////////
	void setCharacterSize(unsigned int size);

	////////////////////////////////////////////////////////////
	/// \brief Set the global color of the text
	///
	/// By default, the text's color is opaque white.
	///
	/// \param color New color of the text
	///
	/// \see getColor
	///
	////////////////////////////////////////////////////////////
	void setColor(const sf::Color& color);

	////////////////////////////////////////////////////////////
	/// \brief Get the text's string
	///
	/// The returned string is a sf::String, which can automatically
	/// be converted to standard string types. So, the following
	/// lines of code are all valid:
	/// \code
	/// sf::String   s1 = text.getString();
	/// std::string  s2 = text.getString();
	/// std::wstring s3 = text.getString();
	/// \endcode
	///
	/// \return Text's string
	///
	/// \see setString
	///
	////////////////////////////////////////////////////////////
	const sf::String& getString();

	////////////////////////////////////////////////////////////
	/// \brief Get the character size
	///
	/// \return Size of the characters, in pixels
	///
	/// \see setCharacterSize
	///
	////////////////////////////////////////////////////////////
	unsigned int getCharacterSize() const;

	////////////////////////////////////////////////////////////
	/// \brief Get the global color of the text
	///
	/// \return Global color of the text
	///
	/// \see setColor
	///
	////////////////////////////////////////////////////////////
	const sf::Color& getTextColor() const;

	bool handleEvent(const sf::Event &event, const sf::Vector2i &mouse);

	bool hasFocus();

  private:
	std::string str;
	mutable sf::Text text;
	sf::RectangleShape box;
	mutable sf::RectangleShape cursor;
	size_t currIndex;
	unsigned int padding;
	unsigned int fontSize;
	unsigned int length;
	bool focused;
	//bool updated;
};
#endif // TEXTBOX_H
