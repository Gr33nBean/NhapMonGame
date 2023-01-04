#include "Animations.h"
#include "debug.h"

Animations* Animations::__instance = NULL;


Animations* Animations::GetInstance()
{
	if (__instance == NULL) __instance = new Animations();
	return __instance;
}

void Animations::Add(int id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION Animations::Get(int id)
{
	LPANIMATION ani = animations[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Animation ID %d not found\n", id);
	return ani;
}

void Animations::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}

// AnimationSet
AnimationSets* AnimationSets::__instance = NULL;

AnimationSets::AnimationSets()
{

}

AnimationSets* AnimationSets::GetInstance()
{
	if (__instance == NULL) __instance = new AnimationSets();
	return __instance;
}

LPANIMATION_SET AnimationSets::Get(unsigned int id)
{
	LPANIMATION_SET ani_set = animation_sets[id];
	if (ani_set == NULL)
		DebugOut(L"[ERROR] Failed to find animation set id: %d\n", id);

	return ani_set;
}

void AnimationSets::Add(int id, LPANIMATION_SET ani_set)
{
	animation_sets[id] = ani_set;
}
