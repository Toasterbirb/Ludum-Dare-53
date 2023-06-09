#pragma once

#include "Rect.hpp"
#include "Texture.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

enum ProgressComparison
{
	MORE, LESS, EQUAL, DISABLED
};

struct ClickTarget
{
	ClickTarget(Birb::Rect click_area, std::string new_frame)
	:area(click_area), new_frame(new_frame) {}

	Birb::Rect area;
	std::string new_frame;

	int progress_requirement = 0;
	ProgressComparison comparison = DISABLED;

	bool valid_progress(int progress)
	{
		switch (comparison)
		{
			case (MORE):
				return (progress > progress_requirement);
				break;

			case (LESS):
				return (progress < progress_requirement);
				break;

			case (EQUAL):
				return (progress == progress_requirement);
				break;

			case (DISABLED):
				return true;
				break;
		};

		return true;
	}
};

struct DialogueEntry
{
	std::vector<std::string> dialogue;
	bool has_been_displayed = false;
	bool step_incrementing_dialogue = false;
	int required_progress = 0;
};

struct Frame
{
	Frame() {}

	Frame(std::string name, bool has_step_frames = false, std::vector<int> stepframe_mapping = {0, 1, 2, 3, 4})
	:name(name), step_mapping(stepframe_mapping), has_step_frames(has_step_frames)
	{
		textures[0].LoadTexture("frames/" + name);
		//std::cout << "Loading frame: " << name << std::endl;

		if (has_step_frames)
		{
			/* Validate the stepframe mapping */
			if (step_mapping.size() != 5)
				std::cout << "WARNING: Invalid stepframe mappings were used" << std::endl;

			/* Load all specified step frames */
			std::vector<int> loaded_texture_ids = { 0 };
			for (int i = 1; i < 5; ++i)
			{
				/* Check if this texture is already loaded */
				bool is_already_loaded = false;
				for (size_t j = 0; j < loaded_texture_ids.size(); ++j)
				{
					if (loaded_texture_ids[j] == stepframe_mapping[i])
					{
						is_already_loaded = true;
						break;
					}
				}

				/* Skip the already loaded texture */
				if (is_already_loaded)
					continue;

				textures[stepframe_mapping[i]].LoadTexture("frames/" + name.substr(0, name.size() - 4) + "_" + std::to_string(stepframe_mapping[i]) + ".jpg");
				loaded_texture_ids.push_back(stepframe_mapping[i]);
			}
		}
	}

	std::string name;
	std::vector<ClickTarget> click_targets;
	Birb::Texture textures[5];

	/* Map different textures to different steps */
	/* By default this means 1 to 1 mapping with the steps */
	std::vector<int> step_mapping = {
		0, 1, 2, 3, 4
	};

	bool has_step_frames;

	/* Return the current frame according to the progress made */
	/* By default this function assumes that there are no step frames */
	Birb::Texture get_texture(int step)
	{
		if (has_step_frames)
			return textures[step_mapping[step]];
		else
			return textures[0];
	}


	/* Dilogue text system */
	size_t current_dialogue = 0;
	std::vector<DialogueEntry> dialogue_text;

	/* Change neighbor music volume depending on the distance */
	/* 1: Next to the neighbor, full volume (peaking at the volume set in settings) */
	/* ... decimal values in between */
	/* 0: The furthest distance from the neighbor, really low volume */
	float distance_from_neighbor = 0.0f;
};

void LoadFrames(std::unordered_map<std::string, Frame>& frame_map);

inline std::vector<std::string> random_dialogue_strings = {
	"Thinking about yawning makes me yawn a lot",
	"Why are my eyes so dry...",
	"Why can't my neighbor just party during daytime",
	"I forgot already why I even moved to this place",
	"My neighbor is so weird...\nHe doesn't even collect pipes!",
	"My neighbor keeps calling me weird...\nAnd who is the one now listening to TikTok\ncompilations with loudspeakers?",
	"Delivering pipes to people has been a long-standing\nfavorite activity of mine",
	"High quality pipes are really difficult to come\nby these days",
};
