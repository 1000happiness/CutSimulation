#ifndef CMP_TEXT_H
#define CMP_TEXT_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/ext.hpp>

#include "../../material/ShaderProgram/ShaderProgram.h"
#include "../Component/Component.h"

namespace CMP
{
	class Text :public Component
	{
	public:
		Text(OBJ::Object *game_object, std::string font_path, std::string content, glm::vec2 position, float scale, glm::vec3 color);
		~Text();

		void start();
		void update();
		void render(MAT::ShaderProgram *shader);

		void setContent(std::string content);
	private:
		struct Character {
			GLuint texture_id;   // ID handle of the glyph texture
			glm::ivec2 size;    // Size of glyph
			glm::ivec2 bearing;  // Offset from baseline to left/top of glyph
			GLuint advance;    // Horizontal offset to advance to next glyph
		};

		std::map<GLchar, Character> characters;
		GLuint VAO, VBO;
		std::string content;
		glm::vec2 position;
		float scale;
		glm::vec3 color;
	};
}

#endif // !CMP_TEXT_H

