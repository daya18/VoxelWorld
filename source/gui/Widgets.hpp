#pragma once

namespace vw
{
	struct Panel
	{
		glm::vec2 position;
		glm::vec2 size;
		glm::vec4 color;
		GLuint texture;
	};
}