//#pragma once
//
//namespace vw::impex
//{
//	struct Vertex
//	{
//		glm::vec3 position;
//		glm::vec2 textureCoordinates;
//		glm::vec3 normal;
//	};
//
//	struct Mesh
//	{
//		std::vector <Vertex> vertices;
//		std::vector <GLuint> indices;
//	};
//
//	struct Scene
//	{
//		std::unordered_map <std::string, Mesh> meshes;
//		std::vector <Vertex> vertices;
//		std::vector <unsigned int> indices;
//	};
//
//	Scene LoadSceneFromFile ( std::string const & filePath );
//	Scene TransformScene ( Scene const &, glm::mat4 const & );
//	std::vector <GLfloat> InterleaveVertices ( std::vector <Vertex> const & );
//}