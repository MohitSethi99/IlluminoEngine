#include "ipch.h"
#include "Scene.h"

#include "Illumino/Renderer/SceneRenderer.h"
#include "Component.h"

namespace IlluminoEngine
{
	Scene::Scene()
	{
	}

	Entity Scene::CreateEntity(const char* name)
	{
		entt::entity e = m_Registry.create();
		Entity entity(e, this);
		UUID id = entity.AddComponent<IDComponent>().ID;
		entity.AddComponent<RelationshipComponent>();
		entity.AddComponent<TagComponent>().Tag = name;
		entity.AddComponent<TransformComponent>();
		m_EntityMap.emplace(id, entity);
		return entity;
	}

	void Scene::DeleteEntity(Entity entity)
	{
		UUID& parentId = entity.GetComponent<RelationshipComponent>().Parent;
		if (parentId)
			RemoveParent(entity);

		auto children = entity.GetComponent<RelationshipComponent>().Children;
		for (size_t i = 0, size = children.size(); i < size; ++i)
			DeleteEntity(m_EntityMap.at(children[i]));

		m_EntityMap.erase(entity.GetComponent<IDComponent>().ID);
		m_Registry.destroy(entity);
	}

	Entity Scene::GetParent(Entity entity)
	{
		UUID parentId = entity.GetComponent<RelationshipComponent>().Parent;
		return parentId ? m_EntityMap.at(parentId) : Entity();
	}

	void Scene::SetParent(Entity entity, Entity parent)
	{
		UUID& parentId = entity.GetComponent<RelationshipComponent>().Parent;
		if (parentId)
			RemoveParent(entity);

		parentId = parent.GetComponent<IDComponent>().ID;
		parent.GetComponent<RelationshipComponent>().Children.push_back(entity.GetComponent<IDComponent>().ID);
	}

	void Scene::RemoveParent(Entity entity)
	{
		UUID& parentID = entity.GetComponent<RelationshipComponent>().Parent;
		ILLUMINO_ASSERT(parentID, "Parent is not assigned!");

		Entity parent = m_EntityMap.at(parentID);
		UUID entityId = entity.GetComponent<IDComponent>().ID;
		auto& children = parent.GetComponent<RelationshipComponent>().Children;
		for (auto it = children.begin(); it != children.end(); ++it)
		{
			if (*it == entityId)
			{
				children.erase(it);
				break;
			}
		}
		parentID = 0;
	}

	void Scene::OnUpdateEditor(Timestep ts)
	{
		
	}

	void Scene::OnRenderEditor(const Camera& camera)
	{
		SceneRenderer::BeginScene(camera);
		{
			auto& view = m_Registry.view<TransformComponent, MeshComponent>();
			for (auto entity : view)
			{
				auto [trans, mesh] = view.get<TransformComponent, MeshComponent>(entity);
				if (mesh.MeshGeometry)
					SceneRenderer::SubmitMesh(mesh.MeshGeometry->GetSubmesh(mesh.SubmeshIndex), trans.GetTransform());
			}
		}
		SceneRenderer::EndScene();
	}
}
