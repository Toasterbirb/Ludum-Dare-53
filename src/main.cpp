#include "Birb2D.hpp"
#include <iostream>

/* Game headers */
#include "Frames.hpp"
#include <unordered_map>

using namespace Birb;

/* Function declarations */
static void start(Game& game);
static void input(Game& game);
static void update(Game& game);
static void render(Game& game);
static void post_render();
static void cleanup();

bool fullscreen = false;

/* Debugging variables */
Input::MouseDrag drag_controller;
Rect drag_rect(0, 0, 0, 0, Colors::Nord::Aurora::nord14);
bool drag_result_printed = true;
Scene drag_scene;

/* Game variables */
Texture temp_tex;
Entity::Image* current_frame_entity;
Frame* current_frame;
Scene scene;

Rect end_fade(0, 0, 1600, 900, Colors::Nord::PolarNight::nord0);
float end_fade_timer = 0.0f;
bool neighbor_blown_up = false;
Audio::SoundFile explosion("boom.wav");
Entity::Text credits_text;
Font* credits_font;
const int credits_scroll_speed = 40;

/* Game progression */
/* 0: No progress has been made yet
 * 1: Steel pipe
 * 2: Gunpowder
 * 3: Brass caps
 * 4: Fuse rope */
int step = 0;

Entity::Image* custom_cursor;
Texture custom_cursor_texture;
Scene cursor_scene;

/* Dialogue stuff */
size_t dialogue_progress = 0;
Scene dialogue_box_scene;
Rect dialogue_background(400, 600, 800, 180, Colors::Nord::PolarNight::nord1);
Entity::Text dialogue_text_entity;
Font* dialogue_font;
bool dialogue_was_shown_during_this_frame = false;

Entity::Text dialogue_tutorial;
int dialogue_tutorial_counter = 0;
int dialogue_tutorial_max_count = 1;
int random_dialogue_chance = 20; /* The higher the number, the smaller the chance */
bool is_random_dialogue = false;

/* Music */
Audio::MusicFile background_music("ukkonooa.wav");
float max_volume = 0.0f;

/* Set this to true in the input() func if the user clicks */
bool click_queued = false;

std::unordered_map<std::string, Frame> frames;

int main(int argc, char** argv)
{
	Game::WindowOpts window_options;
	window_options.title 				= "Ludum Dare 53";
	window_options.window_dimensions 	= { 1600, 900 };
	window_options.refresh_rate 		= 75;
	window_options.resizable 			= false;

	Game game_loop(window_options, start, input, update, render);

	/* Check the passed in CLI arguments */
	if (argc == 2)
	{
		if (!strcmp(argv[1], "-f"))
		{
			Debug::Log("Launching in fullscreen...");
			fullscreen = true;
		}
	}

	/* Optional extra functions */
	game_loop.post_render = post_render;
	game_loop.cleanup = cleanup;

	/* Start the game loop */
	game_loop.Start();

	return 0;
}

/* Enable / Disable the custom cursor */
bool current_cursor_status = false;
void SetCustomCursorStatus(bool status)
{
	if (status != current_cursor_status)
	{
		SDL_ShowCursor(!status);

		if (status)
			cursor_scene.Activate();
		else
			cursor_scene.Deactivate();

		current_cursor_status = status;
	}
}
/* Change the current frame entity to the current frame texture */
void UpdateCurrentFrameEntity()
{
	current_frame_entity->sprite = current_frame->get_texture(step);

	/* Set the volume level accordingly */
	Audio::SetGlobalVolume(max_volume * current_frame->distance_from_neighbor);
}

/* start() is called before the game loop starts.
 * Useful for doing stuff that will only run once before
 * the game starts */
void start(Game& game)
{
	if (fullscreen)
		SDL_SetWindowFullscreen(game.window->win, SDL_WINDOW_FULLSCREEN_DESKTOP);

	MainMenuSettings main_menu_settings;
	Texture menu_background_texture("start.jpg");
	main_menu_settings.background_texture = menu_background_texture;
	main_menu_settings.credits_menu.credits_text = "This game was made by ToasterBirb\nfor Ludum Dare 53 in about 48 hours";
	main_menu_settings.title.text = "Silence is just a pipe dream";

	MainMenu main_menu(game, main_menu_settings);
	main_menu.Launch();

	/* Get the peak music volume */
	max_volume = Audio::GetCurrentGlobalVolume();

	/* Start loading in game resources */
	LoadFrames(frames);

	/* Set the first frame to inside the tent */
	current_frame_entity = new Entity::Image("Current frame", Rect(0, 0, game.window->dimensions.x, game.window->dimensions.y), temp_tex);

	current_frame = &frames["tent_inside.jpg"];
	UpdateCurrentFrameEntity();

	/* Add the current frame entity to the game scene */
	scene.AddObject(current_frame_entity);

	/* Initialize the debugging thingies */
	drag_scene.AddObject(drag_rect);

	/* Load in the custom mouse cursor */
	custom_cursor_texture.LoadTexture("cursor.png");
	custom_cursor = new Entity::Image("Custom cursor", Rect(0, 0, 32, 32), custom_cursor_texture);
	cursor_scene.AddObject(custom_cursor);

	/* Set the custom cursor to disabled state by default */
	current_cursor_status = true;
	SetCustomCursorStatus(false);

	/* Setup the dialogue system */
	dialogue_background.color.a = 180;

	dialogue_font = new Font("birb2d_res/fonts/freefont/FreeMonoBold.ttf", 24);
	dialogue_text_entity.Construct("Testing testing", Vector2Int(dialogue_background.x + 32, dialogue_background.y + 32), dialogue_font, Colors::Nord::SnowStorm::nord4);
	dialogue_text_entity.renderingPriority = 5;

	dialogue_tutorial.Construct("Click to continue",
			Vector2Int(dialogue_background.x + dialogue_background.w - 250, dialogue_background.y + dialogue_background.h - 40),
			dialogue_font, Colors::Nord::SnowStorm::nord6);
	dialogue_tutorial.renderingPriority = 5;

	dialogue_box_scene.AddObject(dialogue_text_entity);
	dialogue_box_scene.AddObject(dialogue_tutorial);
	dialogue_box_scene.AddObject(dialogue_background);

	dialogue_box_scene.renderingPriority = 10;
	dialogue_box_scene.Deactivate();

	scene.AddObject(dialogue_box_scene);

	/* Enable alpha blendering for that sweet sweet transparency */
	Render::AlphaBlendingToggle(true);

	/* Setup some annoying background music */
	background_music.play(true);

	/* Setup the end fade screen */
	end_fade.color.a = 0;
	end_fade.renderingPriority = 12;
	scene.AddObject(end_fade);

	credits_font = new Font("birb2d_res/fonts/freefont/FreeMonoBold.ttf", 44);

	const std::string credits_text_string =
R"~~(The pipe was incredibly effective

In fact, it was so effective, that
the neighbor stopped going outside

I haven't seen him for weeks...


Also, some weird people visited me
yesterday. They were wearing dark
clothes and sun glasses.

They blabbed amount something something
improvised explosive device bla bla...

I didn't have time for their monologue,
so I simply handed them a backpack containing
one of my finest pipes and closed the door.


I heard some frantic shouting and a nice little
thud. The silence returned once again.



All nice things in life are simply pipe dreams,
so why not use pipes to achieve them :)




THE END


No actual pipebombs were created during the
making of this video game.

Please don't make one yourself. You'll just
achieve a spot on a watchlist, get a long holiday
behind long metal pipes and possibly hurt
yourself.



HOWEVER if you need to research the topic
for whatever reason (I'm not judging),
this https://en.wikipedia.org/wiki/Pipe_bomb
wikipedia page contains some information.


THE ACTUAL END



Have you checked your mailbox today?


(in minecraft)
)~~";

	credits_text.Construct(credits_text_string,
			Vector2Int(200, 900), credits_font, Colors::Nord::SnowStorm::nord4);
	credits_text.renderingPriority = 13;
	scene.AddObject(credits_text);
}

/* input() is called at the beginning of the frame
 * before update(). Behind the curtains it does input
 * polling etc. and then passes the SDL_Event into
 * this function */
void input(Game& game)
{
	click_queued = false;

	drag_controller.Poll(*game.event);

	if (game.window->isMouseDown())
		click_queued = true;

	//if (game.window->isKeyDown() && Input::EventToKeycode(*game.event) == Input::J)
	//{
	//	std::cout << "Forcing tent" << std::endl;
	//	current_frame = &frames["tent_inside.jpg"];
	//	UpdateCurrentFrameEntity();
	//}
}



/* update() is called after input has been handled and
 * before the frame gets rendered. Its useful for any game
 * logic that needs to be updated before rendering */
void update(Game& game)
{
	/* Area selection debugging */
	//if (drag_controller.isDragging())
	//{
	//	drag_rect.x = drag_controller.startPos().x;
	//	drag_rect.y = drag_controller.startPos().y;
	//	drag_rect.w = drag_controller.endPos().x - drag_controller.startPos().x;
	//	drag_rect.h = drag_controller.endPos().y - drag_controller.startPos().y;
	//	drag_result_printed = false;
	//}
	//else if (!drag_result_printed)
	//{
	//	if (drag_rect.w > 1 || drag_rect.h > 1 || drag_rect.w < -1 || drag_rect.h < -1)
	//		std::cout << drag_rect << std::endl;
	//	drag_result_printed = true;
	//}

	/* Check if we need to pop open a dialogue window */
	if (current_frame->dialogue_text.size() != 0
			&& !current_frame->dialogue_text[current_frame->current_dialogue].has_been_displayed
			&& step >= current_frame->dialogue_text[current_frame->current_dialogue].required_progress
			&& current_frame->current_dialogue < current_frame->dialogue_text.size()
			&& !dialogue_box_scene.isActive())
	{
		/* Get rid of the tutorial if it has been shown enough times */
		if (dialogue_tutorial_counter >= dialogue_tutorial_max_count)
			dialogue_tutorial.SetText("");
		dialogue_tutorial_counter++;

		/* Setup a new dialogue window */
		dialogue_progress = 0;
		dialogue_text_entity.SetText(current_frame->dialogue_text[current_frame->current_dialogue].dialogue[0]);
		dialogue_box_scene.Activate();
		dialogue_was_shown_during_this_frame = true;
	}
	/* Attempt launching a "random dialogue" to fill in the empty gaps */
	else if(Global::random.RandomInt(0, random_dialogue_chance) == 0
			&& !dialogue_box_scene.isActive()
			&& !dialogue_was_shown_during_this_frame)
	{
		is_random_dialogue = true;

		/* Setup a new dialogue window */
		dialogue_progress = 0;
		dialogue_text_entity.SetText(random_dialogue_strings[Global::random.RandomInt(0, random_dialogue_strings.size() - 1)]);
		dialogue_was_shown_during_this_frame = true;
		dialogue_box_scene.Activate();
	}
	else
	{
		/* This prevents the random dialogue from showing up
		 * with every single frame */
		dialogue_was_shown_during_this_frame = true;
	}

	/* Handle dialogue progression */
	if (click_queued && dialogue_box_scene.isActive())
	{
		dialogue_progress++;
		if (is_random_dialogue || dialogue_progress > current_frame->dialogue_text[current_frame->current_dialogue].dialogue.size() - 1)
		{
			/* The dialogue box is over, close it */
			dialogue_progress = 0;
			dialogue_box_scene.Deactivate();

			/* Avoid progression with random dialogue */
			if (!is_random_dialogue)
			{
				current_frame->dialogue_text[current_frame->current_dialogue].has_been_displayed = true;

				/* Check if the dialogue should increment game progress */
				if (current_frame->dialogue_text[current_frame->current_dialogue].step_incrementing_dialogue)
				{
					step++;

					/* Make sure that the current frame is up-to-date */
					UpdateCurrentFrameEntity();
				}

				current_frame->current_dialogue++;
			}

			is_random_dialogue = false;
		}
		else
		{
			dialogue_text_entity.SetText(current_frame->dialogue_text[current_frame->current_dialogue].dialogue[dialogue_progress]);
		}

		click_queued = false;
	}

	/* Wait until the cursor is on an area that can be clicked */
	/* Also skip this part if there's a dialogue box open waiting for interaction
	 * or if the step is equal to 5 (the game is over) */
	if (current_frame->click_targets.size() > 0 && !dialogue_box_scene.isActive() && step < 6)
	{
		for (size_t i = 0; i < current_frame->click_targets.size(); ++i)
		{
			/* Allow clicking the area if its progress requirements are matched
			 * and the cursor is in the correct area */
			if (game.window->CursorInRect(current_frame->click_targets[i].area)
					&& current_frame->click_targets[i].valid_progress(step))
			{
				/* If the user clicked, go to the "next" frame */
				if (click_queued)
				{
					/* Show the cursor and hide the custom cursor */
					SetCustomCursorStatus(false);

					click_queued = false;
					dialogue_was_shown_during_this_frame = false;

					/* Update the frame */
					current_frame = &frames[current_frame->click_targets[i].new_frame];
					UpdateCurrentFrameEntity();

					//std::cout << "Frame: " << current_frame->name << std::endl;

					/* Check if the new frame has any click targets
					 * and spit out a warning if there are zero */
					if (current_frame->click_targets.size() == 0)
						Debug::Log("This frame has zero click targets", Debug::warning);
				}

				/* Change the mouse cursor */
				cursor_scene.SetPosition(game.window->CursorPosition().ToFloat() - Vector2(16, 0));
				SetCustomCursorStatus(true);

				/* No need to check the rest of the click areas */
				break;
			}
			else
			{
				/* Show the cursor and hide the custom cursor */
				SetCustomCursorStatus(false);
			}
		}
	}
	else
	{
		SetCustomCursorStatus(false);
	}

	/* If step is equal to 6, start fading the screen to black slowly */
	if (step == 6)
	{
		/* Handle the fadeout */
		if (end_fade.color.a < 253)
			end_fade.color.a += 3;

		end_fade_timer += game.time_step()->deltaTime;

		if (end_fade_timer > 2.0f && !neighbor_blown_up)
		{
			neighbor_blown_up = true;

			/* Stop the music */
			Mix_HaltMusic();

			/* Crank up the volume */
			Audio::SetGlobalVolume(max_volume);

			/* Play the explosion sound */
			explosion.play();
		}

		if (end_fade_timer > 5.0f && neighbor_blown_up)
		{
			/* Roll the credits */
			credits_text.rect.y -= game.time_step()->deltaTime * credits_scroll_speed;

			/* If the credts have rolled to the end, close the game */
			if (credits_text.rect.y < credits_text.rect.h * -1)
			{
				explosion.play();
				SDL_Delay(750);
				game.application_running = false;
			}
		}
	}
}

/* render() is called after update() has been finished.
 * Before it gets called, the window will be cleared and
 * after the function has finished running, the rendered frame
 * will be presented */
void render(Game& game)
{
	/* Render the game scene */
	scene.Render();

	/* Render the debugging drag scene */
	//drag_scene.Render();

	/* Render the custom cursor, whenever its active */
	cursor_scene.Render();
}

/* post_render() will be called after rendering has finished
 * and the timestep stalling has started. On non-windows systems
 * this function call will be done on a separate thread, so you
 * could use it to do some extra preparations for the next frame
 * while the main thread is sitting around doing nothing
 * and waiting to maintain the correct frame rate */
void post_render()
{

}

/* cleanup() gets called after the game loop has finished running
 * and the application is getting closed. This is useful for doing any
 * cleanup that is necessary, like freeing heap allocations etc. */
void cleanup()
{
	delete current_frame_entity;
	delete custom_cursor;
	delete dialogue_font;
	delete credits_font;
	background_music.free();
	explosion.free();
}
