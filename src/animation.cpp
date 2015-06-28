#include "include.h"

extern GameData game;

void AnimationData::SetFrame(int frame, int value)
{
	m_frames[frame] = value;
}

void AnimationData::SetInterval(int interval, int value)
{
	m_intervals[interval] = value;
}

AnimatorData::AnimatorData()
{

}

AnimatorData::AnimatorData(std::string file)
{
	bool success = true;

	m_inuse = 1;
	std::ifstream anim (file.c_str());

	if (anim == NULL)
	{
		fprintf(stderr, "cannot open map file");
		success = false;
		game.End();
	}

	int maxAnimations;
	int maxFrames;
	
	anim >> maxAnimations;
	m_maxAnimations = maxAnimations;
	printf("%i\n", m_maxAnimations);

	anim >> maxFrames;
	m_maxFrames = maxFrames; // will be the same for max intervals
	m_maxIntervals = maxFrames;
	printf("%i\n", m_maxFrames);

	for (int i = 0; i < m_maxAnimations; i++)
	{
		int frame;
		int interval;

		for (int j = 0; j < m_maxFrames; j++)
		{
			anim >> frame;

			this->m_animations[i].SetFrame(j, frame);
			printf("%i\n", frame);
		}

		for (int k = 0; k < m_maxFrames; k++)
		{
			anim >> interval;

			this->m_animations[i].SetInterval(k, interval);
			printf("%i\n", interval);
		}

		m_animations[i].m_curFrame = 0;
		m_animations[i].count = 0;
		m_animations[i].m_timeForNextFrame = 100;
	}

	m_owner = NULL;

}

void AnimatorData::Animate(TextureSheetData sheet, AnimationData * animation, SDL_Rect r, SDL_RendererFlip flipType )
{
	if (animation->m_timeForNextFrame <= game.CurrentTime())
	{
		animation->m_curFrame = animation->m_frames[animation->count];
		if(animation->m_curFrame >= m_maxFrames -1)
		{
			animation->m_curFrame = 0;
		}

		if (animation->count >= m_maxFrames - 1)
		{
			animation->count = 0;
		}

		animation->m_timeForNextFrame = animation->m_intervals[animation->m_curFrame] + game.CurrentTime();

		animation->count++;
	}

	sheet.Render(game.GameRender(), animation->m_frames[animation->m_curFrame], r, flipType);
}