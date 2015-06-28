#ifndef _ANIMATE_H_
#define _ANIMATE_H_

#define MAX_ANIMATIONS 20

class AnimationData //single animation
{
	//class within a class?

public:
	int count;
	int m_curFrame; // the current frame in the animation we are in
	int m_timeForNextFrame; // time for the next frame to shows

	short m_frames[10]; //frames that are part of the animation
	int m_intervals[10]; //how long each frame is displayed

	void SetFrame(int frame, int value);
	void SetInterval(int interval, int value);
};

class AnimatorData
{
	int m_inuse;

	int m_maxAnimations; // maximum amount of animations we are managing
	int m_maxFrames; // max frames for each animation, from file
	int m_maxIntervals;

	

	struct EntityData * m_owner; // who called the initiaion of this Animator?
public:
	AnimationData m_animations[MAX_ANIMATIONS]; // organized list of anims. and their intervals
	AnimatorData();
	AnimatorData(std::string file);
	void Animate(TextureSheetData sheet, AnimationData * animation, SDL_Rect r, SDL_RendererFlip flipType);
};


#endif