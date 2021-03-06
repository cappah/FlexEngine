#include "stdafx.hpp"

#include "Scene/Scenes/BaseScene.hpp"

#include <glm/vec3.hpp>

#include "Scene/GameObject.hpp"
#include "Logger.hpp"

namespace flex
{
	BaseScene::BaseScene(std::string name) :
		m_Name(name)
	{
	}

	BaseScene::~BaseScene()
	{
		auto iter = m_Children.begin();
		while (iter != m_Children.end())
		{
			delete *iter;
			iter = m_Children.erase(iter);
		}
	}

	std::string BaseScene::GetName() const
	{
		return m_Name;
	}

	void BaseScene::AddChild(const GameContext& gameContext, GameObject* pGameObject)
	{
		UNREFERENCED_PARAMETER(gameContext);

		for (auto iter = m_Children.begin(); iter != m_Children.end(); ++iter)
		{
			if (*iter == pGameObject)
			{
				Logger::LogWarning("Attempting to add child to scene again");
				return;
			}
		}

		m_Children.push_back(pGameObject);
	}

	void BaseScene::RemoveChild(GameObject* gameObject, bool deleteChild)
	{
		auto iter = m_Children.begin();
		while (iter != m_Children.end())
		{
			if (*iter == gameObject)
			{
				if (deleteChild)
				{
					SafeDelete(*iter);
				}

				iter = m_Children.erase(iter);
				return;
			}
			else
			{
				++iter;
			}
		}

		Logger::LogWarning("Attempting to remove non-existent child from scene");
	}

	void BaseScene::RemoveAllChildren(bool deleteChildren)
	{
		auto iter = m_Children.begin();
		while (iter != m_Children.end())
		{
			if (deleteChildren)
			{
				delete *iter;
			}

			iter = m_Children.erase(iter);
		}
	}

	void BaseScene::RootInitialize(const GameContext& gameContext)
	{
		Initialize(gameContext);

		for (auto iter = m_Children.begin(); iter != m_Children.end(); ++iter)
		{
			(*iter)->Initialize(gameContext);
		}
	}

	void BaseScene::RootPostInitialize(const GameContext& gameContext)
	{
		PostInitialize(gameContext);

		for (auto iter = m_Children.begin(); iter != m_Children.end(); ++iter)
		{
			(*iter)->PostInitialize(gameContext);
		}
	}

	void BaseScene::RootUpdate(const GameContext& gameContext)
	{
		Update(gameContext);

		for (auto iter = m_Children.begin(); iter != m_Children.end(); ++iter)
		{
			(*iter)->RootUpdate(gameContext);
		}
	}

	void BaseScene::RootDestroy(const GameContext& gameContext)
	{
		Destroy(gameContext);

		for (auto iter = m_Children.begin(); iter != m_Children.end(); ++iter)
		{
			(*iter)->RootDestroy(gameContext);
		}
	}
} // namespace flex
