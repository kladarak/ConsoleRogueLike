#pragma once

#include <assert.h>
#include <unordered_map>
#include <RTTI/TypeID.h>

class World;

class Entity
{
private:
	template<typename T>
	static void sDestroyComponentFunc(void* inComponent)
	{
		delete static_cast<T*>(inComponent);
	}

	class ComponentTypeCastHelper
	{
	public:
		void	Destroy() { mDestroyFunc(mComponent); mComponent = nullptr; }

		void	(*mDestroyFunc)(void* inComponent);
		void*	mComponent;
	};

	World*												mWorld;
	std::unordered_map<TypeID, ComponentTypeCastHelper>	mComponents;

public:
	Entity(World* inWorld) : mWorld(inWorld) {}
	~Entity()
	{
		for (auto iter : mComponents)
		{
			iter.second.Destroy();
		}
	}

	World* GetWorld() const { return mWorld; }

	template<typename T>
	T* CreateAndAddComponent()
	{
		assert( !HasComponent<T>() );
		auto typeID = TypeIDFactory<T>::GetID();
		auto component = new T();

		ComponentTypeCastHelper helper;
		helper.mComponent = component;
		helper.mDestroyFunc = &sDestroyComponentFunc<T>;
		mComponents[typeID] = helper;

		return component;
	}
	
	template<typename T>
	void DestroyComponent()
	{
		auto typeID = TypeIDFactory<T>::GetID();
		auto iter = mComponents.find(typeID);
		if (iter != mComponents.end())
		{
			(*iter).second.Destroy();
			mComponents.erase(iter);
		}
	}

	template<typename T>
	bool HasComponent() const
	{
		auto typeID = TypeIDFactory<T>::GetID();
		return mComponents.find(typeID) != mComponents.end();
	}

	template<typename T>
	T* GetComponent() const
	{
		auto typeID = TypeIDFactory<T>::GetID();
		auto iter = mComponents.find(typeID);
		return (iter != mComponents.end()) ? static_cast<T*>((*iter).second.mComponent) : nullptr;
	}

};
