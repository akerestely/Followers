#include "ModelLoader.h"

#include "Vertex.h"
#include "Errors.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

namespace Engine
{
	Engine::Mesh ModelLoader::LoadAssimp(char* filePath)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filePath, 
			aiProcess_Triangulate | aiProcess_JoinIdenticalVertices  | aiProcess_GenSmoothNormals | aiProcess_FlipUVs );
		if(!scene)
			fatalError("Error parsing'" + std::string(filePath) + "': " +std::string(importer.GetErrorString()));

		//array containing meshes
		std::vector<Mesh> meshes(scene->mNumMeshes);
		//iterate meshes from scene
		for (int iMesh=0; iMesh<scene->mNumMeshes; iMesh++)
		{
			aiMesh* mesh = scene->mMeshes[iMesh];

			//array containing the vertices, will be used to generate the vbo
			std::vector<Vertex> vertices(mesh->mNumVertices);

			//constant vec3 used to default normals or texture coordinates where missing
			const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

			//iterate threw the mesh vertices and construct our vertices
			for (unsigned int i = 0 ; i < mesh->mNumVertices ; i++) {
				const aiVector3D* pos = &(mesh->mVertices[i]);
				const aiVector3D* normal = mesh->HasNormals() ? &(mesh->mNormals[i]) : &Zero3D;
				const aiVector3D* uv = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &Zero3D;

				vertices[i].SetPosition(pos->x, pos->y, pos->z);
				vertices[i].position = vertices[i].position *1;
				vertices[i].SetNormal(normal->x, normal->y, normal->z);
				vertices[i].SetUV(uv->x, uv->y);
				vertices[i].SetColor(50,200,30,255);
			}

			//array containing the face indices; will be used to generate ibo
			std::vector<unsigned int> indices(mesh->mNumFaces*3); // *3 because, faces are triangles
			for (unsigned int i=0,k=0; i<mesh->mNumFaces; i++)
			{
				const aiFace& face = mesh->mFaces[i];
				indices[k++] = face.mIndices[0];
				indices[k++] = face.mIndices[1];
				indices[k++] = face.mIndices[2];
			}
			meshes[iMesh].Init(vertices, indices);
		}
		return meshes[0];
	}
}
