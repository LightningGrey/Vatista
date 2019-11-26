#pragma once

#include "Scene.h"
#include <unordered_map>
class SceneManager {
public:
	struct SceneIterator {
		typedef std::unordered_map<std::string, Scene*>::iterator iterator;
		typedef std::unordered_map<std::string, Scene*>::const_iterator const_iterator;
		iterator begin() { return SceneManager::_KnownScenes.begin(); }
		iterator end() { return SceneManager::_KnownScenes.end(); }
		const_iterator begin() const { return SceneManager::_KnownScenes.begin(); }
		const_iterator end() const { return SceneManager::_KnownScenes.end(); }
	};
	static Scene* Current();
	static bool SetCurrentScene(const std::string& name);
	static bool HasScene(const std::string& name);
	static Scene* Get(const std::string& name);

	static void RegisterScene(const std::string& name, Scene* scene = nullptr);
	static SceneIterator Each();
	static void DestroyScenes();
	static entt::registry Prefabs;
private:
	static Scene* _CurrentScene;
	static std::unordered_map<std::string, Scene*> _KnownScenes;
};
// We can make some macros to shorten our calls
#define CurrentScene() SceneManager::Current()
#define CurrentRegistry() SceneManager::Current()->Registry()
#define GetRegistry(name) SceneManager::Get(name)->Registry()