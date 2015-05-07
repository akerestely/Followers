#include "ModelLoader.h"

#include "Errors.h"
#include "IOManager.h"
#include "ResourceMngr.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

namespace Engine
{
	Model* ModelLoader::LoadModelAssimp(char* filePath)
	{
		Model* model = new Model;

		Assimp::Importer importer;
		importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, NUM_BONES_PER_VEREX);
		const aiScene *scene = importer.ReadFile(filePath, 
			aiProcess_Triangulate | aiProcess_GenSmoothNormals);
		if(!scene)
			fatalError("Error parsing'" + std::string(filePath) + "': " +std::string(importer.GetErrorString()));

		model->meshes.resize(scene->mNumMeshes);

		//initialize meshes from model
		unsigned int nVertex=0, nIndices=0;
		for (unsigned int i=0; i<scene->mNumMeshes; i++)
		{
			model->meshes[i].materialIndex = scene->mMeshes[i]->mMaterialIndex;
			model->meshes[i].nIndices = scene->mMeshes[i]->mNumFaces * 3;  // *3 because, faces are triangles
			model->meshes[i].baseIndex = nIndices;
			model->meshes[i].baseVertex = nVertex;

			nIndices += model->meshes[i].nIndices;
			nVertex += scene->mMeshes[i]->mNumVertices;
		}

		//array containing the vertices; will be used to generate the vbo
		std::vector<Vertex> vertices(nVertex);
		//array containing the face indices; will be used to generate ibo
		std::vector<unsigned int> indices(nIndices);

		//iterate meshes from scene
		for (unsigned int iMesh=0, iVertex=0, iIndices=0; iMesh<scene->mNumMeshes; iMesh++)
		{
			aiMesh* mesh = scene->mMeshes[iMesh];

			//constant vec3 used to default normals or texture coordinates where missing
			const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

			//iterate threw the mesh vertices and construct our vertices
			bool flipYwithZ = IOManager::GetFileExtension(filePath) == std::string(".3ds");// || 
			//IOManager::GetFileExtension(filePath) == std::string(".md5mesh");
			for (unsigned int i = 0 ; i < mesh->mNumVertices ; i++, iVertex++) {
				const aiVector3D* pos = &(mesh->mVertices[i]);
				const aiVector3D* normal = mesh->HasNormals() ? &(mesh->mNormals[i]) : &Zero3D;
				const aiVector3D* uv = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &Zero3D;

				if(flipYwithZ)
					vertices[iVertex].SetPosition(pos->x, pos->z, pos->y);
				else
					vertices[iVertex].SetPosition(pos->x, pos->y, pos->z);
				vertices[iVertex].SetNormal(normal->x, normal->y, normal->z);
				vertices[iVertex].SetUV(uv->x, uv->y);
			}

			for (unsigned int i=0; i<mesh->mNumFaces; i++)
			{
				const aiFace& face = mesh->mFaces[i];
				indices[iIndices++] = face.mIndices[0] + model->meshes[iMesh].baseIndex;
				indices[iIndices++] = face.mIndices[1] + model->meshes[iMesh].baseIndex;
				indices[iIndices++] = face.mIndices[2] + model->meshes[iMesh].baseIndex;
			}
		}

		UploadData(model, vertices, indices);
		loadMaterials(model, scene, filePath);

		return model;
	}

	Model* ModelLoader::LoadModelRawAssimp(char* filePath, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices)
	{
		Model* model = new Model;

		Assimp::Importer importer;
		importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, NUM_BONES_PER_VEREX);
		const aiScene *scene = importer.ReadFile(filePath, 
			aiProcess_Triangulate | aiProcess_GenSmoothNormals);
		if(!scene)
			fatalError("Error parsing'" + std::string(filePath) + "': " +std::string(importer.GetErrorString()));

		model->meshes.resize(scene->mNumMeshes);

		//initialize meshes from model
		unsigned int nVertex=0, nIndices=0;
		for (unsigned int i=0; i<scene->mNumMeshes; i++)
		{
			model->meshes[i].materialIndex = scene->mMeshes[i]->mMaterialIndex;
			model->meshes[i].nIndices = scene->mMeshes[i]->mNumFaces * 3;  // *3 because, faces are triangles
			model->meshes[i].baseIndex = nIndices;
			model->meshes[i].nVertices = scene->mMeshes[i]->mNumVertices;
			model->meshes[i].baseVertex = nVertex;

			nIndices += model->meshes[i].nIndices;
			nVertex += model->meshes[i].nVertices;
		}

		//array containing the vertices; will be used to generate the vbo
		vertices.resize(nVertex);
		//array containing the face indices; will be used to generate ibo
		indices.resize(nIndices);

		//iterate meshes from scene
		for (unsigned int iMesh=0, iVertex=0, iIndices=0; iMesh<scene->mNumMeshes; iMesh++)
		{
			aiMesh* mesh = scene->mMeshes[iMesh];

			//constant vec3 used to default normals or texture coordinates where missing
			const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

			//iterate threw the mesh vertices and construct our vertices
			bool flipYwithZ = IOManager::GetFileExtension(filePath) == std::string(".3ds");
			for (unsigned int i = 0 ; i < mesh->mNumVertices ; i++, iVertex++) {
				const aiVector3D* pos = &(mesh->mVertices[i]);
				const aiVector3D* normal = mesh->HasNormals() ? &(mesh->mNormals[i]) : &Zero3D;
				const aiVector3D* uv = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &Zero3D;

				if(flipYwithZ)
					vertices[iVertex].SetPosition(pos->x, pos->z, pos->y);
				else
					vertices[iVertex].SetPosition(pos->x, pos->y, pos->z);
				vertices[iVertex].SetNormal(normal->x, normal->y, normal->z);
				vertices[iVertex].SetUV(uv->x, uv->y);
			}

			for (unsigned int i=0; i<mesh->mNumFaces; i++)
			{
				const aiFace& face = mesh->mFaces[i];
				indices[iIndices++] = face.mIndices[0] + model->meshes[iMesh].baseIndex;
				indices[iIndices++] = face.mIndices[1] + model->meshes[iMesh].baseIndex;
				indices[iIndices++] = face.mIndices[2] + model->meshes[iMesh].baseIndex;
			}
		}

		loadMaterials(model, scene, filePath);

		return model;
	}

	AnimModel* ModelLoader::LoadAnimModelAssimp(char* filePath)
	{
		//Model containing all the data
		AnimModel* model = new AnimModel;

		model->importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, NUM_BONES_PER_VEREX);
		model->scene = model->importer.ReadFile(filePath, 
			aiProcess_Triangulate | aiProcess_GenSmoothNormals);
		if(!model->scene)
			fatalError("Error parsing'" + std::string(filePath) + "': " +std::string(model->importer.GetErrorString()));
		
		model->meshes.resize(model->scene->mNumMeshes);

		//initialize meshes from model
		unsigned int nVertex=0, nIndices=0;
		for (unsigned int i=0; i<model->scene->mNumMeshes; i++)
		{
			model->meshes[i].materialIndex = model->scene->mMeshes[i]->mMaterialIndex;
			model->meshes[i].nIndices = model->scene->mMeshes[i]->mNumFaces * 3;  // *3 because, faces are triangles
			model->meshes[i].baseIndex = nIndices;
			model->meshes[i].baseVertex = nVertex;

			nIndices += model->meshes[i].nIndices;
			nVertex += model->scene->mMeshes[i]->mNumVertices;
		}

		//array containing the vertices; will be used to generate the vbo
		std::vector<BonedVertex> vertices(nVertex);
		//array containing the face indices; will be used to generate ibo
		std::vector<unsigned int> indices(nIndices);

		//iterate meshes from scene
		for (unsigned int iMesh=0, iVertex=0, iIndices=0; iMesh<model->scene->mNumMeshes; iMesh++)
		{
			aiMesh* mesh = model->scene->mMeshes[iMesh];

			//constant vec3 used to default normals or texture coordinates where missing
			const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

			//iterate threw the mesh vertices and construct our vertices
			for (unsigned int i = 0 ; i < mesh->mNumVertices ; i++, iVertex++) 
			{
				const aiVector3D* pos = &(mesh->mVertices[i]);
				const aiVector3D* normal = mesh->HasNormals() ? &(mesh->mNormals[i]) : &Zero3D;
				const aiVector3D* uv = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &Zero3D;

				vertices[iVertex].SetPosition(pos->x, pos->y, pos->z);
				vertices[iVertex].SetNormal(normal->x, normal->y, normal->z);
				vertices[iVertex].SetUV(uv->x, uv->y);
			}

			for (unsigned int i=0; i<mesh->mNumFaces; i++)
			{
				const aiFace& face = mesh->mFaces[i];
				indices[iIndices++] = face.mIndices[0] + model->meshes[iMesh].baseIndex;
				indices[iIndices++] = face.mIndices[1] + model->meshes[iMesh].baseIndex;
				indices[iIndices++] = face.mIndices[2] + model->meshes[iMesh].baseIndex;
			}

			//load bone data
			for (unsigned int i = 0 ; i < mesh->mNumBones ; i++) 
			{                
				unsigned int boneIndex = 0;        
				std::string boneName(mesh->mBones[i]->mName.data);

				if (model->boneMapping.find(boneName) == model->boneMapping.end()) 
				{
					// Allocate an index for a new bone
					boneIndex = model->boneMapping.size();
					BoneInfo bi;
					bi.boneOffset = model->copyAiMat(mesh->mBones[i]->mOffsetMatrix); 			
					model->boneInfo.push_back(bi);
					model->boneMapping[boneName] = boneIndex;
				}
				else 
					boneIndex = model->boneMapping[boneName];

				for (unsigned int j = 0 ; j < mesh->mBones[i]->mNumWeights ; j++) 
				{
					unsigned int vertexID = model->meshes[iMesh].baseVertex + mesh->mBones[i]->mWeights[j].mVertexId;
					float weight  = mesh->mBones[i]->mWeights[j].mWeight;                   
					vertices[vertexID].boneData.Add(boneIndex, weight);
				}
			}
		}

		UploadData(model, vertices, indices);
		loadMaterials(model, model->scene, filePath);

		return model;
	}

	void ModelLoader::loadMaterials(Model *model, const aiScene *scene, const char* filePath)
	{
		//allocate memory for materials
		model->materials.resize(scene->mNumMaterials);
		//load data for materials
		for (unsigned int i=0; i<scene->mNumMaterials; i++)
		{
			aiMaterial* material = scene->mMaterials[i];
			model->materials[i] = 0;
			if(material->GetTextureCount(aiTextureType_DIFFUSE)>0)
			{
				aiString path;
				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) 
				{
					std::string fullPath = IOManager::GetPath(filePath) + "/" + path.data;
					model->materials[i] = ResourceMngr::GetTexture(fullPath.c_str()).id;

					if (!model->materials[i])
						printf("Error loading texture '%s'\n", fullPath.c_str());
				}
			}
		}
	}
}
